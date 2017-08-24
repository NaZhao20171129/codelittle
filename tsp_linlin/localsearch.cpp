#include"localsearch.hpp"
#define LSTIMES 580    // 对tsp解的局部搜索次数，1000

//0 表示不局部搜索，1表示firstImprove，2表示bestImprove
LocalSearch::LocalSearch(Population * p)
{
	if(p!=NULL)
		pop=p;
	else
		cout<<"error ,pop is empty in localsearch "<<endl;
	start=NULL;
	changed=NULL;
	best=NULL;
}

LocalSearch::LocalSearch(Population * p,int t)
{
	if(p!=NULL)
		pop=p;
	else
		cout<<"error ,pop is empty in localsearch "<<endl;
	start=NULL;
	changed=NULL;
	best=NULL;
	type=t;
}

LocalSearch::~LocalSearch()
{
	if(changed!=NULL)
	{
		delete changed;
		changed=NULL;
	}
	if(best!=NULL)
	{
		delete best;
		best=NULL;
	}
}
void LocalSearch::release()
{

	if(changed!=NULL)
	{
		delete changed;
		changed=NULL;
	}
	if(best!=NULL)
	{
		delete best;
		best=NULL;
	}
}
int LocalSearch::makingLocalSearch(int t)
{
	for(int i=0;i<pop->getPopSize();i++)
	{
		setStart(pop->getIndividualAt(i));
				
		switch(t)
		{
			case 0:
				break;
			case 1:
				firstImproveLocalSearch();
				break;
			case 2:
				bestImproveLocalSearch();
				break;
			case 14:
				cout<<"doing 2opt"<<endl;
				for(int j=0;j<LSTIMES;j++)
					two_opt();
				pop->addToPool(best);
				delete best;
				best=NULL;
				break;
			case 15:
				cout<<"doing 3opt"<<endl;
				for(int j=0;j<LSTIMES;j++)
					three_opt();
				pop->addToPool(best);
				delete best;
				best=NULL;
				break;

			default:
				cout<<"local search type error"<<endl;
		}
	//	pop->setIndividualAt(i,best);
	//pop->addToPool(best);
	}
}

//主要针对高层的算子串，进行局部搜索
int LocalSearch:: bestImproveLocalSearch()
{
	for(int i=0;i<myglobalSetup->numOfDecisionVariable;i++)
	{
		cout<<"get gene at this bit"<<endl;
		if(start==NULL)
			cout<<"error ,start still empty"<<endl;
		int gene=* (start->getIndividualGeneAt(i));
		
		for(int j=0;j<myglobalSetup->maxOperatorNum;j++)
		{
			if(j!=gene)
			{
				changed->setGeneAt(i,j);//把个体的第i位改成基因j（即第j个算子）
				GA * ga=new GA(problem[OperatorIndividual::countP]);
				ga->evolute(changed);
				if(changed->getFitness()>best->getFitness())
					setBest(changed);
			}
		}
	}

}
int LocalSearch:: firstImproveLocalSearch()
{
	for(int i=0;i<myglobalSetup->numOfDecisionVariable;i++)
	{
		cout<<"get gene at this bit"<<endl;
		if(start==NULL)
			cout<<"error ,start still empty"<<endl;
		int gene=* (start->getIndividualGeneAt(i));
		
		for(int j=0;j<myglobalSetup->maxOperatorNum;j++)
		{
			if(j!=gene)
			{
				changed->setGeneAt(i,j);//把个体的第i位改成基因j（即第j个算子）
				GA * lga=new GA(problem[OperatorIndividual::countP]);
				cout<<"evaluating the local new opeartors"<<endl;
				lga->evolute(changed);
				if(changed->getFitness()>best->getFitness())
				{
					setBest(changed);
					setStart(changed);
					i=0;
					break;
				}
			}
		}
	}

}
int LocalSearch::two_opt()
{
	//cout<<"doing 2opt"<<endl;
	int result =0;
	int loc1=0,loc2=0;
	loc1=myRandom->boundedIntegerRandom(1,changed->geneLen-1);
	loc2=myRandom->boundedIntegerRandom(1,changed->geneLen-1);
	while(loc2==loc1)
		loc2=myRandom->boundedIntegerRandom(1,changed->geneLen-1);
	if(loc2<loc1)
	{
		int tmp=loc2;
		loc2=loc1;
		loc1=tmp;
	}

	int i=loc1+1,j=loc2,tmp=0;
	while(i<j)
	{
		tmp=*changed->getIndividualGeneAt(i);
		changed->setGeneAt(i,*changed->getIndividualGeneAt(j));
		changed->setGeneAt(j,tmp);
		++i;
		--j;

	}

	changed->evaluateFitness();

	if(changed->fitness>start->fitness)
	{
		setBest(changed);
		result=1;
	//	cout<<"fitness improve in 2opt"<<endl;
	}
	return result;
}
void rank(int & loc1,int & loc2)
{

	if(loc2<loc1)
	{
		int tmp=loc2;
		loc2=loc1;
		loc1=tmp;
	}
}
int LocalSearch::three_opt()
{

	//cout<<"doing 3opt"<<endl;
	int result =0;
	int loc1=0,loc2=0,loc3=0;

	loc1=myRandom->boundedIntegerRandom(1,changed->geneLen-1);
	loc2=myRandom->boundedIntegerRandom(1,changed->geneLen-1);
	loc3=myRandom->boundedIntegerRandom(1,changed->geneLen-1);
	while(loc2==loc1)
		loc2=myRandom->boundedIntegerRandom(1,changed->geneLen-1);

	while(loc3==loc1)
		loc3=myRandom->boundedIntegerRandom(1,changed->geneLen-1);

	rank(loc1,loc2);
	rank(loc1,loc3);
	rank(loc2,loc3);
	int i=loc1+1,j=loc2,tmp=0;
	while(i<j)
	{
		tmp=*changed->getIndividualGeneAt(i);
		changed->setGeneAt(i,*changed->getIndividualGeneAt(j));
		changed->setGeneAt(j,tmp);
		++i;
		--j;


	}
	i=loc2+1;
	j=loc3;
	
	while(i<j)
	{
		tmp=*changed->getIndividualGeneAt(i);
		changed->setGeneAt(i,*changed->getIndividualGeneAt(j));
		changed->setGeneAt(j,tmp);
		++i;
		--j;

	}

	changed->evaluateFitness();

	if(changed->fitness>start->fitness)
	{
		setBest(changed);
//		cout<<"fitness improve in 3opt"<<endl;
		result=1;
	}

	return result;
}


int LocalSearch:: setStart(Individual * st)
{
	if(st!=NULL)
		start=st;
	else
	{
		cout<<"the start is empty in setStart of localsearch"<<endl;
		return 0;
	}
	if(changed!=NULL)
	{
		delete changed;
		changed=NULL;
	}
	changed=st->clone();
	if(best!=NULL)
	{
		delete best;
		best=NULL;
	}
	best=st->clone();
}


int LocalSearch::setBest(Individual * b)
{
	if(best!=NULL)
	{
		delete best;
		best=NULL;
	}
	best =b->clone();
}


//int LocalSearch::setChanged(Individual *c)
//{
//}
