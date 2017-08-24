#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include"globalSetup.hpp"
#include"chromosome.hpp"
#include"random.hpp"
#include<fstream>
#include"problem.hpp"
#include<vector>
#include<algorithm>


using namespace std;
class ProblemPerformancePair ;
class GA;
class Problem;
class Selection;
extern GlobalSetup * myglobalSetup;
extern Problem ** problem ;
class Individual
{
//	protected:
	public:
		Chromosome *chromosome;
		double fitness;
	public:
		int geneLen;

		double objectiveFunction; //目标函数值，然后通过它求fitness
		//int numOfViolation;
	public:
		Individual();
		Individual(const Individual & i);
		Individual(const Chromosome & c);
		Individual(int cityNum);
		Individual(Individual *);
		void rank();
      		virtual ~Individual()
		{
			if(chromosome!=NULL)
			{
				delete chromosome;
				chromosome=NULL;
			}
		}

		double getFitness()
		{ return fitness;}


		Chromosome * getChromosome()
		{
			return chromosome;
		}
	        virtual int evaluateFitness();

		virtual void setGeneAt(int i,int val)
		{
			if(chromosome!=NULL & chromosome->genes!=NULL)
				chromosome->SetValue(i,val);

			else
				cout<<"can not set individual value at bit "<<i<<endl;
		}
                virtual Individual * clone();
		void setFitness(double fit)
		{ fitness=fit ; }


		int * getIndividualGeneAt(int tt)
		{
			return (*chromosome)[tt];
		}
//		int mutate(int type);
//		int crossOver(int type,Individual * i);
//		int localSearch(int type);


		virtual int getPPPLen()  	{}

		virtual	double  getFitnessWithProblemId(int id){}

		virtual int outPutRankings(){}  // 父类不用这个函数，纯粹是为子类流接口的
		virtual int setRankingWithProblemId(int p,double r){}
		virtual int printTspSolu(ostream &out){}
		friend int  individualCompare(Individual *a,Individual * b);
		friend ostream & operator<<  (ostream & s,const Individual & i);
		friend class Selection;		
		
};

class ProblemPerformancePair;

class OperatorIndividual:public Individual
{
	public:
		vector <ProblemPerformancePair> ppp;
//		int pppLen;//ppp中共有多少个pair对
	public:

		OperatorIndividual(int len):Individual()
		{
			geneLen=len;
			chromosome=new Chromosome();
			//cout<<"initlize a opeatorIndividual now ."<<endl;
			fitness=0.0;
		};

		OperatorIndividual (OperatorIndividual & p);
		OperatorIndividual (vector<int> &v);
		~OperatorIndividual()
		{
		}
		int  evaluateFitness();
		Individual * clone();

		static int countP;
		void setGeneAt(int i,int val)
		{
			if(chromosome!=NULL)
				chromosome->SetValue(i,val);
			else
				cout<<"cannot set value,the chromosome is NULL"<<endl;
		}
		int getPPPLen()  	{return ppp.size();}

		double  getFitnessWithProblemId(int id);
		int setRankingWithProblemId(int p,double r);
		int outPutRankings();
		int printTspSolu(ostream &out);
};
// int OperatorIndividual::countP=0;
class ProblemPerformancePair
{
	public:

		int problemId;
		double solutionQuality;
		double ranking;
		ProblemPerformancePair()
		{
			problemId =0;
			solutionQuality=0.0;
			ranking =0;
		}
		ProblemPerformancePair(const ProblemPerformancePair & pp)
		{
			problemId=pp.problemId;
			solutionQuality=pp.solutionQuality;
			ranking=pp.ranking;
		}

		ProblemPerformancePair(int pi,double f,int r)
		{
			problemId=pi;
			solutionQuality=f;
			ranking=r;
		}

		~ProblemPerformancePair()
		{}
		friend ostream  & operator << (ostream & out,const ProblemPerformancePair & p);

};

#endif

