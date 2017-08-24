#include"head.h"


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
        setnonblock(listenfd);

        int ret=bind(listenfd,(struct sockaddr *) &server_address, sizeof(server_address));
        assert(ret==0);

        ret=listen(listenfd,5);
        assert(ret==0);

/*服务器服务启动，等待客户端的链接的到来*/
        int run_flag=1;
        char buf[1024];
        int conn=-1;
        while(run_flag)
        {

            int acfd=accept(listenfd,NULL,NULL);
            if(acfd>=0)
                    conn=acfd;

            printf("%d\n",conn);
            bzero(buf,1024);
            int ret=read(conn,buf,1023);
            if(ret>0)
            {
                    buf[ret]=0;
                    printf("received: %s\n",buf);
                    write(conn,buf,ret);
            }
            else if (ret==0)
            {
                    printf("read 0\n");
                    close(conn);
            }
            else
            {
                    printf("read error\n");
                    close(conn);
            }

            
        }
        






}
