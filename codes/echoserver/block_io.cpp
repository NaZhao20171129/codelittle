/*这个服务器的作用是，监视服务器端的 客户读写套接字和 自己端的io（0，1，2），对这些io的状态变化进行反映，也就是进行读写*/
/*结果发现服务器忙于处理客户链接，而无法处理自己IO的输入内容，怎么办*/
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



int dowork(user curuser,int listenfd)
{       
        setnonblock(curuser.conn);
        while(!curuser.stop )
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
                    dowork(users[user_number],listenfd);
        
            }

        }
        

}
