#include<stdio.h>
#include<unistd.h>
int main()
{
        printf("%d",access("a.py",F_OK));
}
