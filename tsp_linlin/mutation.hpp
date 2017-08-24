#ifndef MUTATION_HPP
#define MUTATION_HPP

#include"individual.hpp"
#include<iostream>
#include<string>
#include<math.h>
#include"random.hpp"
#include"globalSetup.hpp"

#include"population.hpp"
class Population;
extern Random * myrandom;
extern GlobalSetup * myglobalSetup;
extern GlobalSetupForLow * lowSetup;
class Mutation
{
	public:
		Mutation(Population * p)
		{
			if(p!=NULL)
				pop=p;
			else
				cout<<"population p is empty constructing mutation"<<endl;
			parent=NULL;
			result=NULL;
		}
		virtual ~Mutation()
		{
			if(result!=NULL)
			{
				delete result;
				result=NULL;
			}
		}
		
		Population * pop;//保存一个调用该mutation类的population类，为了便于获取种群中的个体，进行突变
		virtual void setParent(Individual * p);
		
		virtual int makingMutation(int type);
	public:
		
		Individual * parent;
		Individual * result;  //result 需要保持为空，因为一直用的都是clone函数给他赋值的
		int SelectiveMutation();
		int GenewiseMutation();

};



class ProblemMutation:public Mutation
{
	public:
		ProblemMutation(Population * p):Mutation(p)
		{}
		~ProblemMutation(){}
		int makingMutation(int type);


		//具体的突变算子
		int swap();
};
#endif
