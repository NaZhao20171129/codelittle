#ifndef GLOBALSETUP_H
#define GLOBALSETUP_H
#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>



enum SelectionType{ TournamentSelection,RankingSelection};
enum MutateType { Selective,Genewise};
enum CrossOverType{ OnePointCrossover, TwoPointCrossover, UniformCrossover };
enum LocalSearchType{ NonLocalSearch, TwoOPTLocalSearch,ThreeOPTLocalSearch};
enum StoppingCriterias
{
	NumOfEvolution,
	BestFines,
	AvgFitness,
	ChangeInBestFitness,
	ChangeInAvgfitness
};
using namespace std;


class GlobalSetup
{
	public:
		int numOfDecisionVariable;
//		int numOfRawConstraint;
		int populationSize;
//		int geneLen;
		int maxGeneration;

		double replaceProportation;
		int replaceType;

		double mutationRate;
		int mutationType;

		double crossOverProportation;
		int crossoverType;

		double localSearchProbaility;
		int LSType;
		
		int selectionType;

		int maxOperatorNum;

		int mutationTimes;  //选择突变的话，变几位呢		
//		int clauseNum;

//		int variableInClause;
	public:

//		GlobalSetup():numOfDecisionVariable(0),numOfRawConstraint(0),populationSize(0),maxGeneration(0),replaceProportation(0.9),mutationRate(0.5),crossOverProportation(0.5),localSearchProbaility(0.5)
//		{}
//
		GlobalSetup(){}

		GlobalSetup(int nd,int p=10,int max=100,double rp=0.9,double mu=0.5,double cp=0.5,double lsp=0.5,int rtype=1,int mtype=1,int ctype=1,int lstype=1,int stype=1,int mon=14):
			numOfDecisionVariable(nd),populationSize(p), maxGeneration(max),replaceProportation(rp),mutationRate(mu),
			crossOverProportation(cp),localSearchProbaility(lsp)
			,replaceType(rtype),mutationType(mtype),crossoverType(ctype),LSType(lstype),selectionType(stype),maxOperatorNum(mon)
		{}


		int getPopulationSize()
		{
			return populationSize;
		}

		int getMaxGeneration()
		{
			return maxGeneration;
		}

		double getReplaceProportation()
		{
			return replaceProportation;
		}

		double getMutationRate ()
		{
			return mutationRate;
		}

		double getLocalSearchProbaility()
		{
			return localSearchProbaility;
		}
		double getCrossOverProportation()
		{
			return crossOverProportation;
		}

		int getReplaceType()
		{
			return replaceType;
		}

		int getMutationType()
		{
			return mutationRate;
		}

		int getCrossoverType()
		{
			return crossoverType;
		}

		int getLSType()
		{
			return LSType;
		}

		int getSelectionType()
		{
			return selectionType;
		}
		bool readConfigFromFile(string fileName);
		friend ostream & operator << (ostream & o,const GlobalSetup & g);		

};

class GlobalSetupForLow
{
	public:
		int popSize;
		int geneNum;
		int maxGen;
		int operatorLen;
		int localSearchNum;
		int totalNum;
		int selectionType;

	public:
		void readConfigFromFile(string filename);

		void setOpeartorLen(int len){operatorLen=len;}
		void setGeneNum(int len){geneNum =len;}
		int getOperatorLen(){return operatorLen;}
		int getSelectionType(){return selectionType;}
		int getPopSize(){return popSize;}
		int getGeneNum(){return geneNum;}
		int getLocalSearchNum(){return operatorLen;}
		int getTotalNum(){return totalNum;}
		
};
#endif
