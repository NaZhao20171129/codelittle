#ifndef CROSSOVER_H
#define CROSSOVER_H

#include"individual.hpp"
#include<iostream>
#include<math.h>
#include"population.hpp"
using namespace std;
//class Population;

class CrossOver
{

	public:

		int crossType;
		Population * pop;
	public:
		
		CrossOver(Population * p,int type=1){pop =p;crossType=type;}

		virtual int makingCrossover(int t)=0;
		virtual ~CrossOver(){}
		virtual void setParent(Individual * p){}
		virtual void setParent1(Individual *p){}
		virtual void setParent2(Individual *p){}

};

class OneParentCrossOver:public CrossOver
{
	public:
		Individual * parent;

	public:
		OneParentCrossOver( Population * pop, Individual *p,int t=1);
		OneParentCrossOver(Population *p,int t=1);

		~OneParentCrossOver()
		{
			if(parent!=NULL)
				delete parent;
		}
		Individual * getParent(){return parent;}
		void setParent(Individual *p){parent =p;}

		int makingCrossover(int type)=0;

	
};

class TwoParentCrossOver:public CrossOver
{
	public:
		Individual *parent1,*parent2;
	public:

		TwoParentCrossOver(Population * pop,int t=1);
		TwoParentCrossOver(Population * pop,Individual *p1,int t=1);
		TwoParentCrossOver(Population * pop, Individual *p1,Individual *p2,int t=1);

		~TwoParentCrossOver()
		{
			if(parent1!=NULL)
				delete parent1;
			if(parent2!=NULL)
				delete parent2;
		}
		Individual * getParent1(){return parent1;}
		Individual * getParent2(){return parent2;}

		void setParent1(Individual *p1){parent1 =p1;}
		void setParent2(Individual *p2){parent2 =p2;}
		
		int makingCrossover(int type)=0;
};
class MultiParentCrossOver:public CrossOver
{
	public:
		Individual *parent1,*parent2,*parent3;
	public:
		MultiParentCrossOver(Population *pop,int t=1);
		MultiParentCrossOver(Population * pop,Individual *p,int t=1);
		MultiParentCrossOver(Population * pop,Individual *p1,Individual *p2,Individual *p3,int t=1);
		~MultiParentCrossOver()
		{
			if(parent1!=NULL)
				delete parent1;
			if(parent2!=NULL)
				delete parent2;
			if(parent3!=NULL)
				delete parent3;
		}

		Individual *getParent1(){return parent1;}
		Individual *getParent2(){return parent2;}
		Individual *getParent3(){return parent2;}
		void setParent1(Individual *p1){parent1=p1;}
		void setParent2(Individual *p2){parent2=p2;}
		void setParent3(Individual *p3){parent3=p3;}

		int makingCrossover(int type)=0;
};






class ProblemOneParentCrossOver:public OneParentCrossOver
{
	public:
		ProblemOneParentCrossOver(Population * pop,int type):OneParentCrossOver(pop,type){}
		int  swap();
		int inverse();

		int makingCrossover(int type);
};


class ProblemTwoParentCrossOver:public TwoParentCrossOver
{
	public:
		ProblemTwoParentCrossOver(Population *pop,int type):TwoParentCrossOver(pop,type){}

		int PMX();
		int OX();
		int CX();
		int EX();
		int makingCrossover(int type);
};

class ProblemMultiParentCrossOver:public MultiParentCrossOver
{
	public:
		ProblemMultiParentCrossOver(Population *pop,int type):MultiParentCrossOver(pop,type){}
		int threeParent();
		int makingCrossover(int type);
};



//接下来为LLH串定义交叉所用的类，继承自最上面的三个类

class LLHOneParentCrossOver:public OneParentCrossOver
{
	public:
		LLHOneParentCrossOver(Population *pop,int type):OneParentCrossOver(pop,type){}
		int twoPointAndCross();

		int makingCrossover(int type);
};


class LLHTwoParentCrossOver:public TwoParentCrossOver
{
	public:
		LLHTwoParentCrossOver(Population * pop,int type):TwoParentCrossOver(pop,type){}
		int onePoint();
		int twoPoint();

		//todo other crossover operator
		int makingCrossover(int type);
};


class LLHMultiParentCrossOver:public MultiParentCrossOver
{
	public:

		LLHMultiParentCrossOver(Population * pop,int type):MultiParentCrossOver(pop,type){}
		int threeParent();
		int makingCrossover(int type);
};
#endif
