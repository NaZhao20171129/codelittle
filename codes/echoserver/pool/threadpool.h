#include"head.h"
#include<pthread.h>


typedef struct task
{
        int num;
        //to do 
        //
};
pthread_mutex_t queuelock;
pthread_mutex_t semlock;
sem_t queuestat;
int stop=0;
std::list<void *> queue;


pthread_t * threadids;

int initialPool(int num);
void* dowork(void * arg);
int run();
int addwork(void *);
int deal(void *);

int initialPool(int num)
{
        if (num<=0)
                return -1;
        threadids=new pthread_t [num];
        if(!threadids)
                return -1;

        int res=-1;
        for(int i=0;i<num;i++)
        {
                res=pthread_create(&threadids[i],NULL,dowork,NULL);
                if(res!=0)
                {
                        delete threadids;
                        return -1;
                }
                if(pthread_detach(threadids[i]))
                {
                        delete threadids;
                        return -1;
                }

        }

        if (!pthread_mutex_init(&queuelock,NULL))
                return -1;
        if(sem_init(&queuestat,0,0)!=0)
                return -1;

        return 0;
}

void* dowork(void * arg)
{
        run();
}

int addwork(void * request)
{
        //task * t=(task*)request;
        pthread_mutex_lock(&queuelock);
        queue.push_back(request);
        printf("add one\n");
        pthread_mutex_unlock(&queuelock);
        sem_post(&queuestat);
}
int run()
{
       // printf("right?\n");
        int ret=-1;
        while(!stop)
        {
                sem_wait(&queuestat);
                //printf("r\n");
                pthread_mutex_lock(&queuelock);
                //printf("reeee\n");
                if(queue.empty())
                {
                        pthread_mutex_lock(&queuelock);
                        printf("empyty\n");
                        continue;

                }
                void * t=queue.front();
                queue.pop_front();

                if(!t)
                        continue;
                deal(t);
                pthread_mutex_unlock(&queuelock);
        }

}

int deal(void * p)
{
        printf("fuck");
        int c=*(int *)p;
        printf("%d%d%d\n",c,c,c);
        //sleep(0);
}
