#include"globalSetup.hpp"
#include<iostream>
#include<fstream>
using namespace std;
bool GlobalSetup::readConfigFromFile(string fileName)
{
	ifstream in;
	in.open(fileName.c_str());
	if(!in)
	{
		cout<<"can not open file "<<fileName<<endl;
		return 0;
	}
	in>>numOfDecisionVariable;
	in>>populationSize;
	in>>maxGeneration;
	in>>replaceType;
	in>>replaceProportation;
	in>>mutationType;
	in>>mutationRate;
	in>>crossoverType;
	in>>crossOverProportation;
	in>>LSType;
	in>>localSearchProbaility;
	in>>selectionType;
	in>>maxOperatorNum;
	in>>mutationTimes;
	in.close();
	return 1;

}


ostream & operator<<(ostream & out,const GlobalSetup & g)
{
	out<<"numDiceisionVariable: "<<g.numOfDecisionVariable<<" "<<endl;
//	out<<"numOfRawConstraint: "<<g.numOfRawConstraint<<" "<<endl;
	out<<"populationSize: "<<g.populationSize<<" "<<endl;
	out<<"maxGeneration: "<<g.maxGeneration<<" "<<endl;
	out<<"replaceProportation: "<<g.replaceProportation<<" "<<endl;
	out<<"mutationRate: "<<g.mutationRate<<" "<<endl;
	out<<"crossOverProportation: "<<g.crossOverProportation<<" "<<endl;
	out<<"localSearchProbaility: "<<g.localSearchProbaility<<" "<<endl;
//	out<<"clauseNum: "<<g.clauseNum<<" "<<endl;
//	out<<"variableInClause: "<<g.variableInClause<<" "<<endl;
	out<<"selectionType :"<<g.selectionType;

	out<<endl;
	return out;
}


void GlobalSetupForLow::readConfigFromFile(string filename)
{

	ifstream in;
	in.open(filename.c_str());
	if(!in)
	{
		cout<<"can not open file to read Lower Config "<<filename<<endl;
		return ;
	}
	in>>popSize;
	in>>geneNum;
	in>>maxGen;
	in>>operatorLen;
	in>>localSearchNum;
	in>>totalNum;
	in>>selectionType;

}
