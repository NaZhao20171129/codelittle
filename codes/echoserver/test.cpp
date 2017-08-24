#include<stdio.h>
#include<time.h>
#include<unistd.h>

int main()
{
        for(int i=0;i<5;i++)
        {
                time_t t=time(NULL);
                printf("%d\n",(int)t);
                sleep(1);
        }
}
