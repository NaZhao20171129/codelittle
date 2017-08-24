#include<stdio.h>
int main()
{
	int x,y,z;
	x=0,y=0,z=0;
	for(;z<5;z++)
	{
		if((++x>2)||(++y>2))
			x++;
	}

	printf("%d,%d",x,y);


	int a=1;
	double b=1.01;
	int c;
	c=(a=b);
	printf("c=%d",c);
	return 0;
}
