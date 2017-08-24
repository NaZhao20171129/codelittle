#include"head.h"
#define MAX_USER 1024
#define BUF_SIZE 1024 

typedef struct user
{
        int conn;
        sockaddr_in client_addr;
        char bufread[BUF_SIZE];
        char bufwrite[BUF_SIZE];
} user;


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

/*服务器服务启动，等待客户端的链接的到来*/
        int run_flag=1;
        char buf[1024];
        int conn=-1;
        user users[MAX_USER];
        int stop[MAX_USER];
        for(int i=0;i<MAX_USER;i++)
                stop[i]=0;
        int user_number=0;
        while(run_flag)
        {

            int acfd=accept(listenfd,NULL,NULL);
            if(acfd>=0)
            {

                    printf("acfd>0,hhahahaha");
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
                            close(listenfd);
                            setnonblock(users[user_number].conn);
                            //while(!stop[ users[user_number].conn] )
                            while(1)
                            {

                                    int readn=read(users[user_number].conn,users[user_number].bufread,BUF_SIZE);
                                   // printf("readn=%d\n",readn);
                                    //printf("errno=%d\n",errno);
                                    if( (readn<0) && (errno!=EAGAIN))
                                    {
                                            printf("a\n");
                                            printf("read fail, client %d\n",users[user_number].conn);
                                            close(users[user_number].conn);
                                            //stop[ users[user_number].conn ]=1;
                                            //break;
                                            exit(0);
                                    }
                                    else if (readn==0)
                                    {
                                            printf("b\n");
                                            close(users[user_number].conn);
                                            //stop[ users[user_number].conn ]=1;
                                            //break;
                                            exit(0);
                                    }

                                    else if (readn>0)
                                    {
                                            printf("pid= %d\n",getpid());
                                            users[user_number].bufread[readn]='\0';
                                            memcpy(users[user_number].bufwrite,users[user_number].bufread,sizeof(users[user_number].bufread));
                                            int writen=write(users[user_number].conn,users[user_number].bufwrite,sizeof(users[user_number].bufwrite));
                                            
                                            memset(users[user_number].bufread,0,sizeof(users[user_number].bufread));
                                            printf("write %d bytes to %d\n",writen,users[user_number].conn);
                                    }


                            }


                    }

                    /*parent process*/
                    else
                    {
                            close(users[user_number].conn);
                            int stat_loc;
                            /*如果想要多个进程共同服务的话，就不能在这里阻塞的等待，可以通过sigchld信号进行捕获*/
                         //   wait((int *)&stat_loc);
                            printf("parent, finished wait\n");


                    }


            
        
            }

        }
        






}
