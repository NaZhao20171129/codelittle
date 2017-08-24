#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
void f(int sno){
        printf("%d\n");
}
int main(void){
        signal(SIGCHLD,f);
        pid_t pid=fork();
        if(pid==0)//child
        {
                printf("child,parant=%d\n",getppid());
        }else{//parent
                printf("me=%d,child=%d\n",getpid(),pid);
        }
	sleep(1);
        return 0;
}
