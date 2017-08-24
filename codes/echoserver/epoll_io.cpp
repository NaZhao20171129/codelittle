/*这个版本的程序就可以处理 服务器端的标准输入，也能同时处理客户端的读写了
 * 只是程序在整体逻辑上还存在一些问题。必须先让一个客户端链接，然后才能处理IO输入，因为accept函数的位置的关系
 * 另外，服务器目前只能处理一个客户端，可以采用fork，扩展到多个客户端的情况。接下来在做。嘿嘿*/

#include"head.h"
#define MAX_USER 1024
#define MAX_PROCESS 65535
#define BUF_SIZE 1024 
#define MAX_EVENT_NUMBER 1024

typedef struct user
{
        int conn;
        int stop;
        sockaddr_in client_addr;
        char bufread[BUF_SIZE];
        char bufwrite[BUF_SIZE];
} user;

typedef struct subprocessData
{
        user* u;
        pid_t pid;
        pid_t ppid;
}subprocessData;


subprocessData subprocess[MAX_PROCESS];


/* 当sigchld信号发生时，调用这个函数来处理该信号*/
void sig_concrete_handler(int sig)
{
        printf("signal occur\n");
        pid_t pid;
        int stat_loc;
        //while(pid=waitpid(-1,&stat_loc,WNOHANG)>0)
        pid=waitpid(-1,&stat_loc,WNOHANG)>0;
        if(pid>0)
        {
                /*作一些具体的回收工作，比如关闭子进程中打开的socket，但是，我们目前无法直接获得该socket，无法关闭；只能获取目前的子进程的pid，所以，需要建立pid与连接socket之间的联系，可以用一个简单的数组作对应。也可以用一个结构体（记录多种数据）＋一个全局化的数组来作对应,这里用subprocess将pid于user对应*/
                //close(****);
                printf("chld pid=%d\n",pid);
                printf("close client %d\n",subprocess[pid].u->conn);
                printf("close process %d\n",pid);
                subprocess[pid].u->stop=1;
                close(subprocess[pid].u->conn);
        }
}

/*添加对信号的处理，先对sigchld进行处理*/

int handle_sig(int signum, void(handler)(int) )
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
        event.events=EPOLLIN|EPOLLET;
        epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
        setnonblock(fd);
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

        int ret=bind(listenfd,(struct sockaddr *) &server_address, sizeof(server_address));
        assert(ret==0);

        ret=listen(listenfd,5);
        assert(ret==0);

        /*deal with fd_set*/
        epoll_event events [ MAX_EVENT_NUMBER];
        int epollfd=epoll_create(5);
        assert(epollfd!=-1);

        //注册listenfd上的可读事件
        addfd(epollfd,listenfd);
        addfd(epollfd,0);

        int allsockets[1024];

        /*服务器服务启动，等待客户端的链接的到来*/
        int run_flag=1;
        char buf[1024];
        int conn=-1;
        user users[MAX_USER];
        
        for(int i=0;i<MAX_USER;i++)
            users[i].conn=-1; // 用－1 表示目前这个conn可用
        int user_number=0;

        //time out
        struct timeval timeout;
        timeout.tv_sec=0;
        timeout.tv_usec=100;

       
        char ioinput[BUF_SIZE];
        while(run_flag)
        {

            int number=epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
            if(number<0 && errno!=EINTR)
            {
                    printf("selection error\n");
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
                                conn=acfd;
                                user_number++;
                                /*users[user_number] 表示一个conn，唯一标识这个用户链接*/
                                users[user_number].conn=acfd;
                                addfd(epollfd,acfd);
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
                    else
                        dowork(users[user_number]); 
            }
        }

}
