#ifndef POPULATION_H
#define POPULATION_H
#include<iostream>
#include<string>
#include<stdlib.h>
#include<math.h>
#include"individual.hpp"
#include"mutation.hpp"
#include"localsearch.hpp"
#include"selection.hpp"
#include"crossover.hpp"
#include"random.hpp"
#include"problem.hpp"
using namespace std;


class Mutation;
class CrossOver;
class LocalSearch;
class Selection;
class SolutionSelection;

class GA;
extern GlobalSetup * myglobalSetup;
extern GlobalSetupForLow * lowSetup;
extern Random * myRandom;

class Population
{
	public:
		Individual **guys;
		Individual **newguys;

		Individual * bestIndividual;

		Individual **pool;
		int poolLen; //个体池的容量，通常为individualguys的5倍
		int poolPtr;// 表明pool中共有多少个个体，指向最后一个个体

		int popSize;//表是种群中个体的数量
		int geneLen;//表示个体的基因个数
//		int numOfFeasible;
		
		static int genid;


		int popType;//2表示算子串种群，1表示解种群
		double *maxFitness;
		double *minFitness;
		double *avgFitness;
		double *varFitness;
		
		LocalSearch * localsearch;
		Selection * selection;
		CrossOver * crossover;
		Mutation * mutation;
		SolutionSelection * solutionselection;
		double bestFitnessChange;
		double avgFitnessChange;

//		ostream outfile;
	public:
		Population(int gelen);
		Population(Problem *p);
		~Population();

		int initializationPop(int type);

		void initlizeLLHs();
		void initializeSolution(Problem *p);
		int clearPool();
		int copyGuysToPool();
		//interferes
//		double  getNumOfFeasible(){return numOfFeasible;}

		double  getMaxFitness(){ return *maxFitness;}

		double  getMinFitness() {  return * minFitness;}

		double getBestFitnessChange() { return bestFitnessChange; }

		double getAvgFitnessChange() {  return avgFitnessChange; }

		//void setNumOfFeasible(double value) {numOfFeasible=value;}

		void setMaxFitness (double value) { *maxFitness=value; }

		void setMinFitness (double value) { *minFitness=value;}

		void setBestFitnessChange(double value){  bestFitnessChange=value;}
		void setAvgFitnessChange(double value){ avgFitnessChange=value; }

		int  getGeneLen(){return geneLen;}
		int getPopSize(){return popSize;}
		Individual * getBestIndividual()const  {return bestIndividual;}
		
		Individual * getIndividualAt(int t)const {return guys[t];}
		void setIndividualAt(int i,Individual * p);
		Individual * operator [] (int index)
		{
			guys[index];
		}

		friend ostream & operator << (ostream & o,const Population & p);

		int  doEvaluate(int  );
		void doCrossOver(int );
		void doMutate(int );
		int doSelect(int );
		int  doLocalSearch(int );
		void replacePopulation();
		void computeFitnessStatics();
		void computeObjStatistics();
		void mapObjectToFitness();
//		void scaleFitness();
		void rankingQuickSort(Individual ** i,int left,int right);

		void swap(int &ii,int &jj);
		
		double getFitnessAt(int index);


		void mapObjectiveToFitness();
		void computeFitnessStatistics();

		int  addToPool(Individual * ind);
		void outPutPoolStatus();
		void outPutPoolStatus(ostream & out);
		int printTspSolutions(ostream &out);
		friend class GA;

		friend class Mutation;
		friend class LocalSearch;
		friend class CrossOver;
		friend class Selection;
		friend class SolutionSelection;
		
};

#endif
