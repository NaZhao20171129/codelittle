#include<stdio.h>

typedef struct meng
{
        int a;
        int b;
};
int main()
{
        meng m={2,5};
        void *p;
        meng * pm;
        pm=&m ;
        p=pm;
        printf("%d",((meng*)p)->b);
}
