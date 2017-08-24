#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void * Print()
{
	int num=100;
	int i;
	for(i=1;i<num;i++)
	{
		printf("%d  ",i);
		if(i%10==0)
		printf("\n");
	}
}
int main()
{
	pthread_attr_t tattr;
	pthread_t  helper;
	void * status;
	void * arg;
	int ret;
	
	ret =pthread_create(&helper,NULL,Print,arg);
	if(ret!=0)
	{
	printf("pthread create failed\n");

	exit(1);
	}
	char c;
	for( c='A';c<='z';c++)
	printf("%c ",c);
	printf("\n");


	pthread_join(helper,&status);
	

	return 0;
}
