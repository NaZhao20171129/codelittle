#include"chromosome.hpp"
#include<iostream>
#include<stdlib.h>
#include"random.hpp"
#include"globalSetup.hpp"
#include<map>
#define SWAPTIMESRANDOM 200
using namespace std;

void randomseq(int *a,int n);
//extern GlobalSetup * myglobalSetup;
//extern Random * myrandom;
Chromosome & Chromosome::operator = (const Chromosome & c)
{
	for(int i=0;i<c.geneLen;i++)
	{
		if(c.genes[i]!=NULL)
			*genes[i]=*c.genes[i];
		else
			cout<<"genes[i] is empty in opeartor = in chromosome.cpp"<<endl;
	}
	

	geneLen=c.geneLen;
	return *this;
}

int Chromosome::copyChromosome(const Chromosome & c)
{
	for(int i=0;i<c.geneLen;i++)
	{
		if(c.genes[i]!=NULL)
			*genes[i]=*c.genes[i];
		else
			cout<<"genes[i] is empty in opeartor = in chromosome.cpp"<<endl;
	}
	

	geneLen=c.geneLen;
}
Chromosome ::Chromosome()  //初始化LLH串
{
	geneLen=myglobalSetup->numOfDecisionVariable;
	genes=new int* [geneLen];
	for(int j=0;j<geneLen;j++)
		genes[j]=new int;

	if(!genes)
	{
		cout<<"no more room for genes"<<endl;
		exit(0);
	}

	for(int i=0;i<geneLen;i++)
		*genes[i]=myRandom->boundedIntegerRandom(1,myglobalSetup->maxOperatorNum)%myglobalSetup->maxOperatorNum;
}



Chromosome ::Chromosome(int num)  //专门用于初始化tsp问题的解 的构造函数
{
	//cout<<"initialize individual of chromosome with parament "<<num<<endl;

	geneLen=num;
	genes=new int* [num];
	for(int j=0;j<num;j++)
		genes[j]=new int(0);

	if(!genes)
	{
		cout<<"no more room for genes"<<endl;
		exit(0);
	}
}
void Chromosome::initializeChromo()
{
	if(genes==NULL)
	{
		cout<<"the genes is null in initializechromo"<<endl;
		exit(0);
	}
	int array[geneLen];
	for(int i=0;i<geneLen;i++)
		array[i]=i;
	randomseq(array,geneLen);
	for(int i=0;i<geneLen;i++)
		*genes[i]=array[i];

}

Chromosome:: Chromosome (const Chromosome & c)
{
	if(genes!=NULL)
	{
		for(int i=0;i<geneLen;i++)
			delete genes[i];

		delete []genes;
		genes=NULL;
	}

	geneLen=c.geneLen;
	genes=new int* [geneLen];

	//geneLen=myglobalSetup->numOfDecisionVariable;
	for(int i=0;i<geneLen;i++)
		genes[i]=new int;

	if(genes==NULL)
	{
		cout<<"no more room for genes in copy Constructure"<<endl;
		exit(0);
	}

	for(int i=0;i<geneLen;i++)
		*genes[i]=*c.genes[i];
}

Chromosome::~Chromosome()
{
	if(genes!=NULL)
	{
		for(int i =0;i<geneLen;i++)
		{
			delete genes[i];
			genes[i]=NULL;
		}
		delete [] genes;
		genes=NULL;
	}
}




void Chromosome::displayChromosome()
{
	if(genes!=NULL)
	{
		for(int i=0;i<geneLen;i++)
			cout<<*genes[i]<<" ";
		cout<<endl;

	}

	else
		cout<<"the genes pointer is NULL"<<endl;
}


ostream & operator<<(ostream & out,const Chromosome & c)
{
	int i=0;
	for(i=0;i<c.geneLen;i++)
	{
		//cout<<"geneLen is "<<c.geneLen<<" in the chrosomesome << opeartor"<<endl;
		if(c.genes[i]!=NULL)
			out<<*c.genes[i]<<" ";
		else 
			cout<<"error ,empty in *c.genes[i] in Chromosome opeartor<<  i = "<<i<<endl;
	}
	//out<<endl;

	return out;
}


void randomseq(int *a,int n)
{
	int t,counter=0;
	int x,y;
//	a=new int[n];
//	for(x=0;i<n;x++)
//		a[x]=x+1;
	for(counter=0;counter<SWAPTIMESRANDOM;counter++)
	{
		x=myRandom->boundedIntegerRandom(0,n);
		y=myRandom->boundedIntegerRandom(0,n);
		t=a[x];
		a[x]=a[y];
		a[y]=t;
	}

}
Chromosome:: Chromosome (Chromosome * c)  //专门用于高层种群的拷贝clone函数调用
{

	geneLen=myglobalSetup->numOfDecisionVariable;
	genes=new int* [geneLen];

	for(int i=0;i<geneLen;i++)
		genes[i]=new int;

	if(!genes)
	{
		cout<<"no more room for genes in copy Constructure"<<endl;
		exit(0);
	}

	for(int i=0;i<geneLen;i++)

		*genes[i]=*c->genes[i];
}
