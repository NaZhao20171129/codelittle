#include"head.h"
#include"threadpool.h"


int main()
{

        initialPool(5);
        void * r;

        
        for(int i=0;i<10;i++)
        {
                r=&i;
                addwork(r);
                printf("here %d\n",i);

        }
        
}
