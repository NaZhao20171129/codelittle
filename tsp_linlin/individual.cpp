#include"individual.hpp"
#include<stdlib.h>
#include"ga.hpp"
#define PROBLEMNUM 20
//class GA;
//extern Problem  ** problem;
extern GlobalSetup * myglobalSetup;
extern double  globalEvaluate(double objfunction);
Problem **p;

Individual::Individual(int cityNum)
{
	geneLen=cityNum;
//	cout<<"initialize individual with cityNum"<<cityNum<<endl;
	chromosome=new Chromosome(cityNum);
	fitness=0.0;

}

Individual::Individual()
{
//	cout<<"initialize individual with no parameter   High Level"<<endl;
//	chromosome=new Chromosome(50);//默认为解决50个城市的问题
	fitness=0.0;
}


int OperatorIndividual::countP=0;

Individual::Individual(const Chromosome & c)
{
	chromosome=new Chromosome();
	*chromosome=c;
	fitness=evaluateFitness();
}

Individual::Individual(const Individual & ind)
{
////	cout<<" the ind lenght is "<<ind.geneLen<<endl;
	//cout<<"construcing a tsp solution individual"<<endl;
	chromosome=new Chromosome(ind.geneLen);
//	cout<<*chromosome;
//	cout<<*ind.chromosome<<endl;
//
	*chromosome=*ind.chromosome;
//	cout<<*chromosome;
	fitness=ind.fitness;
	geneLen=ind.geneLen;

}

Individual::Individual(Individual * ind)
{
	chromosome=new Chromosome(ind->geneLen);
	*chromosome =*ind->chromosome;
	fitness =ind->fitness;
}
int Individual::evaluateFitness()
//将这个个体，针对一个具体的问题进行评估，得到一个实数值
{
	//if(problem ==NULL)
//	{
//		cout<<"in evaluatefitness function ,problem is empty"<<endl;
//		return -1;
//	}

//	static int Count=1;
//	if(Count==myglobalSetup->populationSize)
//	{
//		Count=1;
//		OperatorIndividual::countP++;
//	
//	}
///	// 这句是为了纠错用的，实际实验时候，应该改，因为我现在手上只有50个 50城市的tsp问题
//	if(OperatorIndividual::countP>=9)
//		OperatorIndividual::countP=0;
	if(chromosome->isEmptyGene()==1)
	{
		cout<<"in the evaluatefitness function ,chromosome genes is NULL"<<endl;
		return -1;
	}

//	cout<<"the problem is "<<OperatorIndividual::countP<<endl;
	//int len =myglobalSetup->numOfDecisionVariable;
	int len=lowSetup->getGeneNum();
	double cost=0.0;
//	cout<<"computing cost on problem "<<OperatorIndividual::countP;//<<" for the solution is:"<<endl<<*chromosome<<endl;
	for(int i=0;i<len;i++)
	{
		//cout<<"computing the distance between citys: "<<*getIndividualGeneAt(i)<<"  "<<*getIndividualGeneAt((i+1)%len)<<endl;
		cost +=problem[OperatorIndividual::countP]->getDistanceOfTwoCity(*getIndividualGeneAt(i),*getIndividualGeneAt((i+1)%len));

	
	}
	//cout<<"cmputing over     "<<"the best solution is "<< cost<<endl;
	fitness=cost;
	return 1;
}

Individual * Individual::clone()
{
	return new Individual (*this);
}


void Individual ::rank()
{
	int min=100000;
	if(chromosome!=NULL)
	{
		for(int i=0;i<geneLen;i++)
		{
			int j=0,loc=0;
			min=*chromosome->genes[i];
			loc=i;
			for(j=i;j<geneLen;j++)
			{
				if(*chromosome->genes[j]<min)
				{
					min=*chromosome->genes[j];
					loc=j;
				}
			}
			*chromosome->genes[loc]=*chromosome->genes[i];
			*chromosome->genes[i]=min;

			
		}
	}
}
//int Individual::mutate(int type)
//{}
//
//int Individual::crossOver(int type,Individual * ind)
//{}
//
//int Individual::localSearch(int type)
//{}
//

//若是相等，返回0，若是不等，返回1，对genes的每一位进行比较
int  individualCompare(Individual * a,Individual * b)
{
	if(a==NULL||b==NULL||a->geneLen!=b->geneLen)
	{
		cout<<"in individualCompare function a or b is NULL"<<endl;
		exit(0);
	}

	for(int i=0;i<a->geneLen;i++)
	{
		if(*a->chromosome->genes[i]!=*b->chromosome->genes[i])
			return 1;
		
	}
	return 0;  // 相等

}

ostream & operator <<(ostream & out,const Individual & b)

{
	int i=0;
	if(&b!=NULL)
	{	
		if(b.chromosome!=NULL)
		{

			//out<<"chromosome:  "<<*(b.chromosome);
			//out<<"fitness:  :"<<b.fitness<<endl;
			out<<*(b.chromosome)<<"\t"<<b.fitness;
		}
	}
	else
		cout<<"the chromosome is empty in outputing Individual"<<endl;
	return out;
}


int OperatorIndividual :: evaluateFitness()
{


	int flag=0;
	double minfit=10000000.0;
	cout<< " this is "<<*this<<endl;
//	Problem * problems;
//	vector<ProblemPerformancePair> ::iterator iter=ppp.begin();
//	for(;iter!=ppp.end();iter++)
///		if(iter->problemId==OperatorIndividual::countP)
///			{
///				flag=1;
//				break;
//			}
			//return 0 ;

	GA  * ga=new GA(problem[OperatorIndividual::countP]);

	//多次运行，取最大。
//	if(fabs(fitness-0.0)<1.0)
//		fitness =1000000.0;
	for(int i=0;i<15;i++)
	{
		ga->evolute(this);
		if(ga->population->getBestIndividual()==NULL)
			cout<<"best ind is null"<<endl;
		double fit=ga->population->getBestIndividual()->getFitness();
		if(minfit>fit)
			minfit=fit;
		
	}
	delete ga;
	ga=NULL;
	//建立低层ga，进行演化，算子为this OperatorIndividual所指，进行评估，然后将低层ga的整个种群中的
	//最好的一个解作为该llh算子串的适应度。
//	OperatorIndividual * oi=new OperatorIndividual(*this);
	//cout<<"current operator is "<<*this<<endl;



	vector<ProblemPerformancePair> ::iterator iter=ppp.begin();
	for(;iter!=ppp.end();iter++)
		if(iter->problemId==OperatorIndividual::countP)
			{
				if((iter->solutionQuality-0.0)>2.0)
				{
					if(iter->solutionQuality>minfit)
						iter->solutionQuality=minfit;
				}
				else
					iter->solutionQuality=minfit;
	
				flag=1;
			}
	if(flag==0)
	{
		ProblemPerformancePair *p=new ProblemPerformancePair(OperatorIndividual::countP,fitness,0);
		ppp.push_back(*p);
	}
//	vector<ProblemPerformancePair>::iterator iter=ppp.begin();
//	for(i=0;i<OperatorIndividual::countP;i++)
//		iter++;
//	ppp.insert(iter,p);
	return 1;
// 后面的参数是表示算子的长度

}
OperatorIndividual::OperatorIndividual( OperatorIndividual &p)
{
	//cout<<"constructing a operator individual"<<endl;
//	OperatorIndividual * oi=dynamic_cast<OperatorIndividual *> (p);
	chromosome=new Chromosome(p.chromosome);
//	* chromosome =*( p.getChromosome());
	geneLen=p.geneLen;
	fitness =p.getFitness();
	for(int i=0;i<p.ppp.size();i++)
		ppp.push_back(p.ppp[i]);

	cout<<"cloning vectors"<<endl;
	printTspSolu(cout);
}
Individual * OperatorIndividual::clone()
{
	return new OperatorIndividual(*this);
}
double OperatorIndividual:: getFitnessWithProblemId(int id)
{
	vector<ProblemPerformancePair>::iterator iter=ppp.begin();
	for(;iter!=ppp.end();iter++)
	{
		if(iter->problemId==id)
			return iter->solutionQuality;
	}

	return 0.0;
}


int OperatorIndividual:: setRankingWithProblemId(int p,double r)
{
	vector<ProblemPerformancePair>::iterator iter=ppp.begin();
	for(;iter!=ppp.end();iter++)
	{
		if(iter->problemId==p)
			iter->ranking=r;
	}
}
int OperatorIndividual:: outPutRankings()
{
	vector<ProblemPerformancePair>::iterator iter=ppp.begin();
	for(;iter!=ppp.end();iter++)
		cout<<* iter <<"  ";
	cout<<endl;
}
 ostream  & operator << (ostream & out,const ProblemPerformancePair & p)
{
	out <<p.problemId<<"----"<<p.solutionQuality<<"----"<<p.ranking;
}


int OperatorIndividual::printTspSolu(ostream & out)
{
	vector<ProblemPerformancePair>::iterator iter;
	for(iter=ppp.begin();iter!=ppp.end();iter++)
		out<<iter->problemId<<"\t"<<iter->solutionQuality<<"\t"<<iter->ranking<<"\t";
}
OperatorIndividual::OperatorIndividual(vector<int> & v):Individual()
{

	geneLen=v.size();
	chromosome=new Chromosome(geneLen);
	for(int i=0;i<geneLen;i++)
	{
		*(chromosome->genes[i])=v[i];
	}
}
