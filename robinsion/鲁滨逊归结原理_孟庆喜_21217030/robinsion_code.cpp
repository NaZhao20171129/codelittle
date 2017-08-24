#include<stdio.h>
#include<string.h>
#include<string>
#include<list>
#include<unistd.h>
#define size 100

using namespace std;
class Predication
{   
	public: 
	int num;
    	string  ArgList;
 };

class Clause
{   
	public: 
   	int nItems;               //标识子句中谓词的数目。
        list<Predication*> PredList; //该链表存储子句中的谓词信息，其元素是Predication指针类型。
	Clause * next, *pre;      //指针类型成员，这两个元素是为了构造子句集的双向链表设置的，分别指向前驱和后继的子句。
};

class  ClauseSet
{ 
	public:
	Clause parrayClasuse[size];       //Clause 类型的指针数组  
};

struct Function
{
    	int sign;
	int parament_num;
	char fun_name;
	char parament[10];
}function[size];

struct Sentense
{
	Function fun[10];
	int fun_num;
}sentense[size];

void Display(Sentense a)
{
	for(int i=0;i<a.fun_num;i++)
	{
		if(a.fun[i].sign==0)
		{
		printf("~");
		}
		printf("%c",a.fun[i].fun_name);
		printf("(");
		printf("%c",a.fun[i].parament[0]);
		for(int j=1;j<a.fun[i].parament_num;j++)
		{
			printf(",");
			printf("%c",a.fun[i].parament[j]);
		}
		printf(")");
		if(i<a.fun_num-1)
			printf("|");
	}
	printf("\n");
}
int  main()
{   


	freopen("in.txt","r",stdin);
	char judge='y';
	int f_n=0;
	int s_n=0;
	int m=0;
	while(1)
	{
		int i;
		char temp[30];
		sentense[s_n].fun_num=1;
	
			printf("请输入子句集，按“n”键退出。\n");
			scanf("%s",temp);
	int h=0;
	i=strlen(temp);
	while(h<i)
	{   
		if(h==0)
		{
		if(temp[h]=='~')
			function[f_n].sign=0;
		else
			  function[f_n].sign=1;
		}
	    if(temp[h]=='(')
		{  
		function[f_n].fun_name=temp[h-1];
		function[f_n].parament[0]=temp[h+1];
		function[f_n].parament_num=1;
		}
        if(temp[h]==',')
		{   
			function[f_n].parament[function[f_n].parament_num]=temp[h+1];
			function[f_n].parament_num++;
		}
		if(temp[h]=='|')
		{
			f_n++;
			if(temp[h+1]=='~')
				function[f_n].sign=0;
			else
				function[f_n].sign=1;
			sentense[s_n].fun_num++;
		}
		h++;
	}
	for(i=0;i<sentense[s_n].fun_num;i++)
	{
		sentense[s_n].fun[i]=function[m];
		m++;
	}
	scanf("%s",&judge);
	if(judge=='n')
		break;
        	s_n++;
			f_n++;
		
	for(int i=0;i<=s_n;i++)
	{
		printf("%d",i+1);
		printf("、");
		Display(sentense[i]);
	}
	printf("开始归结：\n");
	int q=s_n;
	
    while(1)
	{
	for( i=0;i<sentense[q].fun_num;i++)
	{
	   Function a=sentense[q].fun[i];
	   for(int j=0;j<q;j++)
	   {
		   for(int n=0;n<sentense[j].fun_num;n++)
		   {
			  Function b=sentense[j].fun[n];
			   if(a.fun_name==b.fun_name&&a.sign!=b.sign&&a.parament_num==b.parament_num)
			   {
				   s_n++;
				   sentense[s_n].fun_num=sentense[q].fun_num+sentense[j].fun_num-2;
				   if(sentense[s_n].fun_num==0)
				   {
					   m=j+1;
					   goto end;
				   }
				   for(int h=1;h<=sentense[s_n].fun_num;h++)
				   {
                      if(h<=i)
					  {
						  function[f_n+h]=sentense[q].fun[h-1];
						  for(int t=0;t<b.parament_num;t++)
						  {
                             if(b.parament[t]>='a'&&b.parament[t]<='c'&&a.parament[t]>='x'&&a.parament[t]<='z')
							 {
								 for(int g=0;g<sentense[q].fun[h-1].parament_num;g++)
								 {
								 if(sentense[q].fun[h-1].parament[g]=a.parament[t])
									 function[f_n+h].parament[g]=b.parament[t];
								 }
							 }
						  }
					  }
					 else if(h<sentense[q].fun_num)
					  {
						  function[f_n+h]=sentense[q].fun[h];
						   for(int t=0;t<b.parament_num;t++)
						  {
                             if(b.parament[t]>='a'&&b.parament[t]<='c'&&a.parament[t]>='x'&&a.parament[t]<='z')
							 {
								 for(int g=0;g<sentense[q].fun[h].parament_num;g++)
								 {
								 if(sentense[q].fun[h-1].parament[g]=a.parament[t])
									 function[f_n+h].parament[g]=b.parament[t];
								 }
							 }
						  }

					  }
					 else if(h<sentense[q].fun_num+n)
					 {
						 function[f_n+h]=sentense[j].fun[h-sentense[q].fun_num];
						  for(int t=0;t<a.parament_num;t++)
						  {
                             if(a.parament[t]>='a'&&a.parament[t]<='c')
							 {
								 for(int g=0;g<sentense[j].fun[h-sentense[q].fun_num].parament_num;g++)
								 {
								 if(sentense[j].fun[h-sentense[q].fun_num].parament[g]=b.parament[t])
									 function[f_n+h].parament[g]=a.parament[t];
								 }
							 }
							 else
							 {
								 if(b.parament[t]<'a'||b.parament[t]>'c')
								 {
								 for(int g=0;g<sentense[j].fun[h-sentense[q].fun_num].parament_num;g++)
								 {
								 if(sentense[j].fun[h-sentense[q].fun_num].parament[g]=b.parament[t])
									 function[f_n+h].parament[g]=a.parament[t];
								 }
								 }

							 }
						  }
					 }
					 else
					 {
						 function[f_n+h]=sentense[j].fun[h-sentense[q].fun_num+1];
						  for(int t=0;t<a.parament_num;t++)
						  {
                             if(a.parament[t]>='a'&&a.parament[t]<='c')
							 {
								 for(int g=0;g<sentense[j].fun[h-sentense[q].fun_num+1].parament_num;g++)
								 {
								 if(sentense[j].fun[h-sentense[q].fun_num+1].parament[g]=b.parament[t])
									 function[f_n+h].parament[g]=a.parament[t];
								 }
							 }
							 else
							 {
								 if(b.parament[t]<'a'||b.parament[t]>'c')
								 {
								 for(int g=0;g<sentense[j].fun[h-sentense[q].fun_num+1].parament_num;g++)
								 {
								 if(sentense[j].fun[h-sentense[q].fun_num+1].parament[g]=b.parament[t])
									 function[f_n+h].parament[g]=a.parament[t];
								 }
								 }

							 }
						  }
					 }
				  }
				     f_n+=sentense[s_n].fun_num;
                	for(int l=0;l<sentense[s_n].fun_num;l++)
					{
	                	sentense[s_n].fun[l]=function[m];
						m++;
					}
                    printf("%d",s_n+1);
		            printf("、");
					Display(sentense[s_n]);
					printf("         ");
					printf("%d",q+1);
					printf("和");
					printf("%d\n",j+1);
			   }
		   }
	   }
	 }
	 q++;
	 if(q==100)
	    break;	 
	}	
end: printf("归结成功!\n");
	 printf("%d", q+1);
	 printf("和");
	 printf("%d\n", m);
if(q==200)
	   printf("归结失败!\n");

	}
return 0;
}

