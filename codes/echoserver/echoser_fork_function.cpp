#include"head.h"
#define MAX_USER 1024
#define MAX_PROCESS 65535
#define BUF_SIZE 1024 

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
        int  pid;
        int stat_loc;
        //while(pid=waitpid(-1,&stat_loc,WNOHANG)>0)
        pid=waitpid(-1,&stat_loc,WNOHANG);
	printf("exit_code=%d\n",WEXITSTATUS(stat_loc));
        if(pid>0)
        {
                /*作一些具体的回收工作，比如关闭子进程中打开的socket，但是，我们目前无法直接获得该socket，无法关闭；只能获取目前的子进程的pid，所以，需要建立pid与连接socket之间的联系，可以用一个简单的数组作对应。也可以用一个结构体（记录多种数据）＋一个全局化的数组来作对应,这里用subprocess将pid于user对应*/
                //close(****);
                printf("chld pid=%d\n",pid);
                printf("close client %d\n",subprocess[pid].u->conn);
                printf("close process %d\n",pid);
                subprocess[pid].u->stop=1;
		//前面可能已经有的地方对该套接字进行了关闭，但是，为了防泄漏，还是在这里处理了一下
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
        //这个的意思是将所有的信号都监听，然后都是调用一个handler来处理。这样看上去好像不太合理。但是，后面我们就知道，为了统一事件源，在此将所有信号一视同仁的处理，在接下来的IO复用中，会有好处的。 当然，既然对各种信号统一处理，我这里写的一个sig_concrete_handler函数只关注sigchld函数就不对了。这个是为了针对sigchld而言的，接下来会该的。嘿嘿。
        sigfillset(&sa.sa_mask);
        assert(sigaction(signum,&sa,NULL)!=-1);
}



int dowork(user *pcuruser,int listenfd)
{       
        close(listenfd);

        setnonblock(pcuruser->conn);
        while(!pcuruser->stop )
        {

            int readn=read(pcuruser->conn,pcuruser->bufread,BUF_SIZE);
            if( (readn<0) && (errno!=EAGAIN))
            {
                    printf("a\n");
                    printf("read fail, client %d\n",pcuruser->conn);
                    close(pcuruser->conn);
                    pcuruser->conn=-1;
                    pcuruser->stop=1;
                    
            }
            else if (readn==0)
            {
                    printf("b\n");
                    close(pcuruser->conn);
                    pcuruser->conn=-1;
                    pcuruser->stop=1;
            }

            else if (readn>0)
            {
                    printf("pid= %d\n",getpid());
                    pcuruser->bufread[readn]='\0';
                    memcpy(pcuruser->bufwrite,pcuruser->bufread,sizeof(pcuruser->bufread));
                    int writen=write(pcuruser->conn,pcuruser->bufwrite,sizeof(pcuruser->bufwrite));
                    
                    memset(pcuruser->bufread,0,sizeof(pcuruser->bufread));
                    printf("write %d bytes to %d\n",writen,pcuruser->conn);
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
        setnonblock(listenfd);

        int ret=bind(listenfd,(struct sockaddr *) &server_address, sizeof(server_address));
        assert(ret==0);

        ret=listen(listenfd,5);
        assert(ret==0);


        /*注册信号，等待处理*/
        handle_sig(SIGCHLD,sig_concrete_handler);


        /*服务器服务启动，等待客户端的链接的到来*/
        int run_flag=1;
        char buf[1024];
        int conn=-1;
        user users[MAX_USER];
        
        for(int i=0;i<MAX_USER;i++)
            users[i].conn=-1; // 用－1 表示目前这个conn可用
        int user_number=0;
        while(run_flag)
        {

            int acfd=accept(listenfd,NULL,NULL);
            if(acfd>=0)
            {

                    printf("acfd>0,hhahahaha\n");
                    user_number++;
                    /*users[user_number] 表示一个conn，唯一标识这个用户链接*/
                    users[user_number].conn=acfd;
                    pid_t pid=fork();
                    if(pid<0)
                    {
                            printf("fork error\n");
                            exit(0);
                    }
                    else if (pid==0)
                    {

                            dowork(&users[user_number],listenfd);
			    printf("is here?\n");
			    exit(101);

                    }

                    /*parent process*/
                    else
                    {
                            /*在父进程中记录子进程的id及其占用资源*/
                            printf("pid=%d in parent\n",pid);
                            subprocess[pid].u=&users[user_number];
                            close(users[user_number].conn);

                            int stat_loc;
                            /*如果想要多个进程共同服务的话，就不能在这里阻塞的等待，可以通过sigchld信号进行捕获,见信号handler*/
                         //   wait((int *)&stat_loc);
                            printf("parent, finished wait\n");
                    }


            
        
            }

        }
        






}
