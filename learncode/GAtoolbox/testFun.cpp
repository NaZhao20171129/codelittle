#include"random.hpp"
#include<stdio.h>


int main()
{
	Random r;
	int array[10];
	int i=0;
	for(i=0;i<10;i++)
	{
		array[i]=r.boundedRandom(1,100);
		printf("array[%d]=%d\n",i,array[i]);
	}

	r.shuffleArray(array,10);

	for(i=0;i<10;i++)
		printf("array[%d]=%d\n",i,array[i]);
	return 0;
}
