
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
#define TEXT_LEN 10
#define PROCESS_NUMBER 1000

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




int addfd(int epollfd, int fd)
{
        epoll_event event;
        event.data.fd=fd;
        //event.events=EPOLLIN|EPOLLET;
        event.events=EPOLLIN;
        epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
        setnonblock(fd);
}

int randText(char * buf)
{
        assert(buf!=NULL);
        time_t curtime=time(NULL);
        srand(curtime);
        int ranLen=rand()%TEXT_LEN;
        for(int i=0;i<ranLen;i++)
        {
                buf[i]='a'+rand()%26;
        }
        return 1;

}

int runchild(user* curuser)
{
        assert(curuser!=NULL);

        int child_epollfd=epoll_create(5);
        assert(child_epollfd!=-1);

        epoll_event events[MAX_EVENT_NUMBER];

        int conn=curuser->conn;
        addfd(child_epollfd,conn);
        addfd(child_epollfd,0);

        int stop_child=0;
        int ret=0;

        while(!stop_child)
        {
                //printf("in child\n");
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
                                ret=recv(conn,curuser->bufread,BUF_SIZE,0);
                                if(ret<0 && errno!=EAGAIN)
                                        stop_child=1;
                                else if (ret==0)
                                        stop_child=1;
                                else
                                //通知父进程，有了数据啦，让父进程告诉别的子进程去读吧。哈哈
                                {
                                        printf("%s\n",curuser->bufread);
                                        time_t curtime=time(NULL);
                                        srand(curtime);
                                        int ranTime=(rand()+getpid())%10+1;
                                        sleep(ranTime);
                                        randText(curuser->bufwrite);

                                        send(curuser->conn,curuser->bufwrite,strlen(curuser->bufwrite),0);
                                        
                                }

                        }
                        else if (socket==0 && (events[i].events & EPOLLIN))
                        {
                                scanf("%s",curuser->bufwrite);
                                send(curuser->conn,curuser->bufwrite,strlen(curuser->bufwrite),0);

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

        printf("start echoback client...\n");

        char * ip=(char * ) argv[1];
        int port =atoi(argv[2]);


        /*构造客户端的地址，主要是填充sockaddr_in 结构体*/        
        struct sockaddr_in  server_address;
        bzero(&server_address,sizeof(server_address));
        server_address.sin_family=AF_INET;
        inet_pton(AF_INET,ip,&server_address.sin_addr);
        server_address.sin_port=htons(port);

        int connectfd;//=socket(AF_INET,SOCK_STREAM,0);
        int user_number=0;
        //assert(connectfd>=0);
        //int reuse=1;
        //setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));

        /*deal with epoll*/
        epoll_event events [ MAX_EVENT_NUMBER];
        int epollfd=epoll_create(5);
        assert(epollfd!=-1);

        //注册标准输入上的可读事件
        addfd(epollfd,0);

        //用来作为信号与epoll链接的管道
        int retsigpipe=socketpair(AF_UNIX,SOCK_STREAM,0,sig_pipefd);

        addfd(epollfd,sig_pipefd[0]);

        add_sig(SIGCHLD,sig_handler);
        add_sig(SIGTERM,sig_handler);
        add_sig(SIGINT,sig_handler);


        user users[MAX_USER];
        for(int i=0;i<MAX_USER;i++)
            users[i].conn=-1; // 用－1 表示目前这个conn可用

        //time out
        struct timeval timeout;
        timeout.tv_sec=0;
        timeout.tv_usec=100;

        //deal with 标准输入
        char ioinput[1024];

        for(int w=0;w<PROCESS_NUMBER;w++)
        {
                user_number++;
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
                        connectfd=socket(AF_INET,SOCK_STREAM,0);
                        if(connect(connectfd,(struct sockaddr*)&server_address,sizeof(server_address))==0)
                        {
                                printf("connect success\n");
                        }
                        users[user_number].conn=connectfd;

                        close(epollfd);
                        char * hello="hello, server\n";
                        send(connectfd,hello,strlen(hello),0);
                        runchild(&(users[user_number]));
                        exit(0);
                }
                else
                {
                        close(connectfd);
                        users[user_number].pid=pid;
                        subprocess[pid].u=&users[user_number];
                }
        }

}
