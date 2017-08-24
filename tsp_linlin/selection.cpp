#include"selection.hpp"

#include<math.h>


Selection::Selection(Population *p)
{
	if(p!=NULL)
		pop=p;
	else
		cout<<"empty p in constructing Selection"<<endl;

}

Selection::~Selection()
{

}
int Selection::rankingAndComputing()
{
	int i=0;
	double value=0;
	int counter=0;
	int problemId;
	int maxPPPLen=0;
	double  tmpValue=0.0;
	int sumRankingNum=0;
	int sumRanking=0;
	double ranking=0.0;

	for(i=0;i<pop->poolPtr;i++)
		if(maxPPPLen<pop->pool[i]->getPPPLen())
			maxPPPLen=pop->pool[i]->getPPPLen();
	for(problemId=0;problemId<maxPPPLen;problemId++)
	{		
		for(i=0;i<pop->poolPtr;i++)
		{
			value=pop->pool[i]->getFitnessWithProblemId(problemId);
			if(fabs(value-0)<0.01)
				break;
			counter=0;
			for(int j=0;j<pop->poolPtr;j++)
			{
				tmpValue=pop->pool[j]->getFitnessWithProblemId(problemId);
				if(fabs(tmpValue-0)>0.01)
					sumRankingNum++;
				if(pop->pool[j]->getFitnessWithProblemId(problemId)<value)

					counter++;
			}
			//ranking=counter*1.0/sumRankingNum;
			//ranking=counter*1.0/sumRankingNum*1.0;
			ranking=counter*1.0/sqrt(sumRankingNum*1.0);
			pop->pool[i]->setRankingWithProblemId(problemId,ranking);
		}
	}
	

//	ofstream out;
//	out.open("inseTsp.xls");
	double sum =0.0;
	int n=0;
	for(i=0;i<pop->poolPtr;i++)
	{
		for(int j=0;j<pop->pool[i]->getPPPLen();j++)
			{
				sum+=dynamic_cast<OperatorIndividual *>(pop->pool[i])->ppp[j].ranking;
				n++;
			}


		pop->pool[i]->fitness=sum/n;
		//pop->pool[i]->printTspSolu(out);
	}
//	out.close();
}




int Selection::tournamentSelection()
{}


int Selection::rankingSelection()
{

//	pop->outPutPoolStatus();
	rankingAndComputing();
	pop->rankingQuickSort(pop->pool,0,pop->poolPtr-1);
//	cout<<"after computing and ranking ,the pools are like:"<<endl;
	//pop->outPutPoolStatus();
//	cout<<"the rankings for each individual"<<endl;
	//for(int i=0;i<pop->poolPtr;i++)
//	{
//		pop->pool[i]->outPutRankings();
//	}
	for(int i=0;i<pop->getPopSize();i++)
	{
		if(pop->guys[i]!=NULL)
		{
			delete pop->guys[i];
			pop->guys[i]=NULL;
		}
	}
	pop->guys[0]=pop->pool[0]->clone();
	int flag=0;
	int ppool=1;
	int i;
	for(i=1;i<pop->popSize&& ppool<pop->poolPtr;)
	{
		flag=0;
		for(int j=0;j<i;j++)    //解决重复问题，将当前要插入到guys中的个体与已经插入进去的对比，若不相等，就插入，否则，不插入
			if(individualCompare(pop->pool[ppool],pop->guys[j])==0)
			{
				flag=1;
				break;
			}
		if(flag==0)
		{
				pop->guys[i]=pop->pool[ppool]->clone();
				i++;
		}
		ppool++;
	//	pop->guys[i]=new OperatorIndividual(pop->pool[i]);
	
	}
	while(i<pop->popSize)
	{

		int rand=myRandom->boundedIntegerRandom(0,pop->poolPtr-1);
		pop->guys[i]=pop->pool[rand]->clone();
		i++;
	}

	if(pop->bestIndividual!=0)
	{
		delete pop->bestIndividual;
		pop->bestIndividual=NULL;
	}
	pop->bestIndividual=pop->guys[0]->clone();

	pop->printTspSolutions(cout);
	return 1;
}

int Selection::makingSelection(int t)
{
	//cout<<"making selection for LLH"<<endl;
	int result=0;
	if(t==1)
	{
		result =rankingSelection();

	}
	else if(t==2)
	{
		result =tournamentSelection();
	}
	else
	{
		cout<<"selection type error"<<endl;

	}

//	pop->poolPtr=0;

	return result;
}
SolutionSelection::SolutionSelection(Population * p)
{
	if(p!=NULL)
		pop=p;
	else
		cout<<"constructing solutionSelection error ,p is empty"<<endl;
}
SolutionSelection::~SolutionSelection()
{}
int  SolutionSelection::tournamentSelection()
{}
int SolutionSelection::rankingSelection()
{
	//pop->outPutPoolStatus();
	pop->rankingQuickSort(pop->pool,0,pop->poolPtr-1);
	//pop->outPutPoolStatus();
	for(int i=0;i<pop->getPopSize();i++)
	{

		if(pop->guys[i]!=NULL)
		{
			delete pop->guys[i];
			pop->guys[i]=NULL;
		}
	}

	pop->guys[0]=pop->pool[0]->clone();
	int ppool=1;
	int i;
	for(i=1;i<pop->getPopSize()&&ppool<pop->poolPtr;)
	{
		int flag=0;
		for(int j=0;j<i;j++)    //解决重复问题，将当前要插入到guys中的个体与已经插入进去的对比，若不相等，就插入，否则，不插入
			if(individualCompare(pop->pool[ppool],pop->guys[j])==0)
			{
				flag=1;
				break;
			}
		if(flag==0)
		{
				pop->guys[i]=pop->pool[ppool]->clone();
				i++;
		}
		ppool++;
	}

	while(i<pop->getPopSize())
	{
		int rand=myRandom->boundedIntegerRandom(0,pop->poolPtr-1);
		
		pop->guys[i]=pop->pool[rand]->clone();
		i++;
	}
	if(pop->bestIndividual!=NULL)
	{
		delete pop->bestIndividual;
		pop->bestIndividual=NULL;
	}
	pop->bestIndividual =pop->pool[0]->clone();
	return 1;

}
int SolutionSelection::ranking()
{}
int SolutionSelection::makingSelection(int type)
{
	//cout<<"making selection for TSP"<<endl;
	int result =0;
	if(type==1)
	{
		result=rankingSelection();
	}
	else if(type==2)
	{
		result=tournamentSelection();
	}
	else 
	{
		result =0;
		cout<<"selection error in making selection"<<endl;
	}
//	pop->poolPtr=0;
	return result;
}
