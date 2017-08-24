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
        int pipefd[2];   //用来建立管道，一端是父进程，一端是子进程。
} user;

//用来保存和子进程通信的信息
typedef struct subprocessData
{
        user* u;
        pid_t pid;
        pid_t ppid;

}subprocessData;


subprocessData subprocess[MAX_PROCESS];


/*子进程从父进程那里读取内容*/
int dowork_comm_with_father(user curuser)
{       

        curuser.stop=0;
        //子进程关闭写端
        close(curuser.pipefd[1]);

        setnonblock(curuser.pipefd[0]);
        while(!curuser.stop )
        {

            int readn=read(curuser.pipefd[0],curuser.bufread,BUF_SIZE);
            if( (readn<0) && (errno!=EAGAIN))
            {
                    printf("a\n");
                    printf("read fail, client ");
                    close(curuser.pipefd[0]);
                    curuser.stop=1;
                    
            }
            else if (readn==0)
            {
                    printf("father close the pipe\n");
                    close(curuser.pipefd[0]);
                    curuser.stop=1;
            }

            else if (readn>0)
            {
                    printf("pid= %d\n",getpid());
                    curuser.bufread[readn]='\0';
                    printf("read from the father\n");
                    printf("%s\n",curuser.bufread);
                    int writen=write(1,curuser.bufwrite,sizeof(curuser.bufwrite));
                    
                    memset(curuser.bufread,0,sizeof(curuser.bufread));
            }


        }
}
        int main(int argc, char **argv)
        {
                printf("start server...\n");


        /*服务器服务启动，等待客户端的链接的到来*/
            int run_flag=1;

            user users[MAX_USER];
            int user_number=0;
            /*这个pipe函数是简单的创建管道的版本，他创建的管道，pipefd[0]只能读，pipefd[1]只能写*/
            int fdret=pipe(users[user_number].pipefd);
            
            assert(fdret>=0);

            pid_t pid=fork();
            if(pid<0)
            {
                    printf("fork error\n");
                    exit(0);
            }
            else if (pid==0)
            {

                    dowork_comm_with_father(users[user_number]);
            }

            /*parent process*/
            else
            {
                    /*在父进程中向子进程写一些random内容*/

                    printf("pid=%d in parent\n",pid);
                    subprocess[pid].u=&users[user_number];
                    close(users[user_number].conn);
                    //父进程关闭管道的读端，只往子进程写内容
                    close(users[user_number].pipefd[0]);

                    int write_times=5;
                    for(int i=0;i<write_times;i++)
                    {
                            
                            char buf[100]="hello from parent at time ";
                            char bufwrite[100];
                            sprintf(bufwrite,"%s",buf);
                            sprintf(bufwrite," %d\n",time(NULL));
                            write(users[user_number].pipefd[1],bufwrite,sizeof(bufwrite));

                            sleep(1);
                    }
                    printf("parent write over\n");

                    int stat_loc;
                    /*如果想要多个进程共同服务的话，就不能在这里阻塞的等待，可以通过sigchld信号进行捕获,见信号handler*/
                 //   wait((int *)&stat_loc);
                    printf("parent, finished wait\n");
                   // sleep(3);
            }

        
}
