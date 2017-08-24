
/*这个程序是一个综合的聊天室服务器
 * 这个版本的程序就可以处理 服务器端的标准输入，也能同时处理客户端的读写了
 * 每个客户链接到来，fork一个子进程进行处理。子进程之间的通讯主要采用共享内存
 *对各种信号作出反应，主要信号有 sigchld，sigterm，siginter，
 * 
 * */

#include"head.h"
#define MAX_USER 1024
#define MAX_PROCESS 65535
#define BUF_SIZE 1024 
#define MAX_EVENT_NUMBER 1024
#define USER_LIMIT 65535

typedef struct user
{
        int conn;
        int stop;
        int pipefd[2];
        int user_number;
        sockaddr_in client_addr;
        char bufread[BUF_SIZE];
        char bufwrite[BUF_SIZE];
        pid_t pid; // which process deal with this user
} user;

typedef struct subprocessData
{
        user* u;
        pid_t pid;
        pid_t ppid;
}subprocessData;


subprocessData subprocess[MAX_PROCESS];

static const char* shm_name="/mysharememory";
char * share_mem=0;
int shmfd;
int sig_pipefd[2];
int user_number=0;


/* 当sigchld信号发生时，调用这个函数来处理该信号*/

void sig_handler(int sig)
{
       int save_errno=errno;
       int msg=sig;
       send(sig_pipefd[1],(char*)&msg,sizeof(msg),0);
       errno=save_errno;

}

/*具体用来处理信号事件的函数*/
void sig_concrete_handler(int sig,int epollfd)
{
        printf("signal  chld occur\n");
        pid_t pid;
        int stat_loc;
        while(pid=waitpid(-1,&stat_loc,WNOHANG)>0)
        //pid=waitpid(-1,&stat_loc,WNOHANG)>0;
        if(pid>0)
        {
                /*作一些具体的回收工作，比如关闭子进程中打开的socket，但是，我们目前无法直接获得该socket，无法关闭；只能获取目前的子进程的pid，所以，需要建立pid与连接socket之间的联系，可以用一个简单的数组作对应。也可以用一个结构体（记录多种数据）＋一个全局化的数组来作对应,这里用subprocess将pid于user对应*/
                printf("close process %d\n",pid);
                subprocess[pid].u->stop=1;
                subprocess[pid].u->pid=-1;
                //不再监听与父进程通信的管道了
                epoll_ctl(epollfd,EPOLL_CTL_DEL,subprocess[pid].u->pipefd[0],0);
                //关闭与父进程通信的管道了
                close(subprocess[pid].u->pipefd[0]);

                //关闭与客户端的连接
                close(subprocess[pid].u->conn);
        }
}

/*添加对信号的处理，先对sigchld进行处理*/

int add_sig(int signum, void(handler)(int) )
{
        struct sigaction sa;
        memset(&sa,'\0',sizeof(sa));
        sa.sa_handler=handler;
        sa.sa_flags|=SA_RESTART;
        //这个的意思是将所有的信号都监听，然后都是调用一个handler来处理。这样看上去好像不太合理。但是，后面我们就知道，为了统一事件源，在此将所有信号一视同仁的处理，在接下来的IO复用中，会有好处的。  
        sigfillset(&sa.sa_mask);
        assert(sigaction(signum,&sa,NULL)!=-1);
}



int dowork(user curuser)
{       
        setnonblock(curuser.conn);
       // while(!curuser.stop )
        {

            int readn=read(curuser.conn,curuser.bufread,BUF_SIZE);
            if( (readn<0) && (errno!=EAGAIN))
            {
                    printf("a\n");
                    printf("read fail, client %d\n",curuser.conn);
                    close(curuser.conn);
                    curuser.conn=-1;
                    curuser.stop=1;
                    
            }
            else if (readn==0)
            {
                    printf("b\n");
                    close(curuser.conn);
                    curuser.conn=-1;
                    curuser.stop=1;
            }

            else if (readn>0)
            {
                    printf("pid= %d\n",getpid());
                    curuser.bufread[readn]='\0';
                    memcpy(curuser.bufwrite,curuser.bufread,sizeof(curuser.bufread));
                    int writen=write(curuser.conn,curuser.bufwrite,sizeof(curuser.bufwrite));
                    
                    memset(curuser.bufread,0,sizeof(curuser.bufread));
                    printf("write %d bytes to %d\n",writen,curuser.conn);
            }


        }
}


int maxfd(int * allsockets,int num)
{

}

int addfd(int epollfd, int fd)
{
        epoll_event event;
        event.data.fd=fd;
        //event.events=EPOLLIN|EPOLLET;
        event.events=EPOLLIN;
        epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
        setnonblock(fd);
}

int runchild(user* curuser,char *shmem)
{
        assert(curuser!=NULL);

        int child_epollfd=epoll_create(5);
        assert(child_epollfd!=-1);

        epoll_event events[MAX_EVENT_NUMBER];

        int conn=curuser->conn;
        addfd(child_epollfd,conn);

        addfd(child_epollfd,curuser->pipefd[1]);
        int stop_child=0;
        int ret=0;

        while(!stop_child)
        {
                printf("in child\n");
                int number=epoll_wait(child_epollfd,events,MAX_EVENT_NUMBER,-1);
                if(number<0 && errno!=EINTR)
                {
                        printf("epoll error in child");
                        break;
                }
                
                for(int i=0;i<number;i++)
                {
                        int sockfd=events[i].data.fd;

                        if(sockfd ==conn && (events[i].events & EPOLLIN) )
                        {
                                memset(shmem+curuser->user_number*BUF_SIZE,'\0',BUF_SIZE);
                                ret=recv(conn,shmem+curuser->user_number*BUF_SIZE,BUF_SIZE-1,0);
                                if(ret<0 && errno!=EAGAIN)
                                        stop_child=1;
                                else if (ret==0)
                                        stop_child=1;
                                else
                                //通知父进程，有了数据啦，让父进程告诉别的子进程去读吧。哈哈
                                {
                                        shmem[curuser->user_number*BUF_SIZE+ret]='\0';

                                        printf("some thing\n");
                                        send(curuser->pipefd[1],(char*)&(curuser->user_number),sizeof(curuser->user_number),0);

                                }

                        }
                        else if (sockfd==curuser->pipefd[1] && events[i].events & EPOLLIN)
                        {
                                printf("some thing from father\n");
                                
                                int client_number;
                                ret=recv(sockfd,(char*)&client_number,sizeof(client_number),0);
                                if(ret<0 && errno!=EAGAIN)
                                {
                                        stop_child=1;
                                }
                                else if (ret==0)
                                        stop_child=1;
                                else
                                //从收到的客户端那里读取内容，往自己这里写
                                {
                                       // printf("rec from father,then write to his client\n");
                                       char tmpbuf[BUF_SIZE*2];
                                       sprintf(tmpbuf,"client %d says: ",client_number);
                                       
                                      //memcpy(tmpbuf+sizeof(tmpbuf),shmem+(client_number)*BUF_SIZE,BUF_SIZE);

                                       sprintf(tmpbuf+15,"%s",shmem+(client_number*BUF_SIZE));
                                       // send(conn,tmpbuf,sizeof(tmpbuf),0);
                                        send(conn,tmpbuf,strlen(tmpbuf),0);

                                }
                                


                        }
                }
        }

        
}

int main(int argc, char **argv)
{
        if(argc<3)
        {
        printf("usage: %s ip port\n",basename(argv[0]));
        exit(0);
        }

        printf("start echoback server...\n");

        char * ip=(char * ) argv[1];
        int port =atoi(argv[2]);


        /*构造服务器端的地址，主要是填充sockaddr_in 结构体*/        
        struct sockaddr_in  server_address;
        bzero(&server_address,sizeof(server_address));
        server_address.sin_family=AF_INET;
        inet_pton(AF_INET,ip,&server_address.sin_addr);
        server_address.sin_port=htons(port);

        int listenfd=socket(AF_INET,SOCK_STREAM,0);
        assert(listenfd>=0);
        int reuse=1;
        setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
       // setnonblock(listenfd);

        int bindret=bind(listenfd,(struct sockaddr *) &server_address, sizeof(server_address));
        assert(bindret==0);

        bindret=listen(listenfd,5);
        assert(bindret==0);

        /*deal with epoll*/
        epoll_event events [ MAX_EVENT_NUMBER];
        int epollfd=epoll_create(5);
        assert(epollfd!=-1);

        //注册listenfd上的可读事件
        addfd(epollfd,listenfd);
        //注册标准输入上的可读事件
        addfd(epollfd,0);

        //用来作为信号与epoll链接的管道
        int retsigpipe=socketpair(AF_UNIX,SOCK_STREAM,0,sig_pipefd);

        addfd(epollfd,sig_pipefd[0]);

        add_sig(SIGCHLD,sig_handler);
        add_sig(SIGTERM,sig_handler);
        add_sig(SIGINT,sig_handler);


        /*服务器服务启动，等待客户端的链接的到来*/
        int run_flag=1;
        user users[MAX_USER];
        for(int i=0;i<MAX_USER;i++)
            users[i].conn=-1; // 用－1 表示目前这个conn可用

        //time out
        struct timeval timeout;
        timeout.tv_sec=0;
        timeout.tv_usec=100;

        //deal with 标准输入
        char ioinput[1024];

        //创建共享内存

        //开辟一块内存,返回一个共享内存对象
        shmfd=shm_open(shm_name,O_CREAT|O_RDWR,0666);
        assert(shmfd!=-1);

        //将这个共享内存对象的大小设定为 ＊＊
        int ret=ftruncate(shmfd,USER_LIMIT*BUF_SIZE);
        assert(ret!=-1);

        //将刚才开辟的共享内存，关联到调用进程
        share_mem=(char *) mmap(NULL,USER_LIMIT*BUF_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,shmfd,0);
        assert(share_mem!=MAP_FAILED);
        
        
       
        while(run_flag)
        {

            int number=epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
            if(number<0 && errno !=EINTR)
            {
                    perror(0);
                    printf("epoll  error\n");
                    break;
            }
            for(int i=0;i<number;i++)
            {

                    int sockfd=events[i].data.fd;
                    if(sockfd==listenfd)
                    {
                            int acfd=accept(listenfd,NULL,NULL);

                            if(acfd>=0)
                            {
                                printf("yes\n");
                                user_number++;
                                /*users[user_number] 表示一个conn，唯一标识这个用户链接*/
                                users[user_number].conn=acfd;
                                users[user_number].user_number=user_number;


                                //父子进程间的管道
                                int piperet=socketpair(AF_UNIX,SOCK_STREAM,0,users[user_number].pipefd);
                                assert(piperet==0);
                                pid_t pid=fork();
                                //此时，我们创建一个子进程，专门负责这个客户端的一切事物
                                //为了方便知道哪个子进程的事件，我们用conn作为索引，即将之视为主键,对进程和链接进行索引和对照
                                if(pid<0)
                                {
                                        printf("fork error\n");
                                        break;
                                }
                                else if(pid==0) //子进程中，处理一切事物
                                {
                                        close(listenfd);
                                        close(users[user_number].pipefd[0]);
                                        close(epollfd);

                                        printf("run child\n");
                                        runchild(&(users[user_number]),share_mem);
                                        munmap((void*)share_mem,USER_LIMIT*BUF_SIZE);
                                        exit(0);
                                }
                                else
                                {
                                        //close(users[user_number].conn);
                                        close(acfd);
                                        close(users[user_number].pipefd[1]);
                                        addfd(epollfd,users[user_number].pipefd[0]);
                                        users[user_number].pid=pid;

                                        subprocess[pid].u=&users[user_number];
                                }
                            }


                            else 
                            {
                                printf("accept error\n");
                                break;
                            }
                                
                    }

                    else if (sockfd==0)
                    {
                        scanf("%s",ioinput);
                        printf("%s\n",ioinput);

                    }

                    else if (sockfd ==sig_pipefd[0])
                    {
                            int sig_numbers[1024];
                            int resig=recv(events[i].data.fd, sig_numbers,1024,0);
                            
                            if(resig<0 )
                                    continue;
                            else if (resig==0)
                                    continue;
                            else
                            {
                                for(int k=0;k<resig;k++)
                                {
                                        switch(sig_numbers[k])
                                        {
                                                case SIGCHLD:
                                                {
                                                        //to do . a concrete function
                                                        sig_concrete_handler(SIGCHLD,epollfd);

                                                }
                                                case SIGTERM:
                                                case SIGINT:
                                                {
                                                    for(int w=0;w<user_number;w++)
                                                    {
                                                       int pid_to_kill=users[user_number].pid;
                                                       kill(pid_to_kill,SIGTERM);
                                                       
                                                    }
                                                    run_flag=0;
                                                    break;

                                                }
                                                default:
                                                {
                                                        break;
                                                }
                                        }
                                }

                            }
                    }
                    else  // 说明是某个表示客户链接的套接字有内容需要读
                    {
                            printf("is here?\n");
                            int client_number;
                            int re=recv(events[i].data.fd, &client_number,sizeof(client_number),0);
                            
                            if(re<0 && errno!=EAGAIN)
                                    run_flag=0;
                            else if (re==0)
                                    run_flag=0;
                            else
                            {
                                    for(int j=0;j<=user_number;j++)
                                    {
                                            if(users[j].pipefd[0]!=events[i].data.fd)
                                            {
                                                    send(users[j].pipefd[0],(char*)&client_number,sizeof(client_number),0);

                                            }
                                    }
                            }

                            
                            

                    }
            }
       
        }

}
