#include<stdio.h>

extern "C"

void Deadloop()
{
	while(1)
	
		printf("aaa");
}
