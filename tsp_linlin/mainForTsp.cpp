#ifndef MAINFORTSP
#define MAINFORTST

#include"globalSetup.hpp"
#include"individual.hpp"
#include"population.hpp"
#include"crossover.hpp"

#include"chromosome.hpp"
#include"random.hpp"
#include"problem.hpp"
#include"feature.hpp"

#include"ga.hpp"
#define MINCITYNUMBER 20
#define MAXCITYNUMBER 1500
#define DEFAULTCITYNUMBER 100
#define PROBLEMNUM 50

#include<sstream>
#include<vector>
#include<boost/timer.hpp>


using boost::timer;
using namespace std;
Random *myRandom;
GlobalSetup * myglobalSetup;
GlobalSetupForLow * lowSetup;
Problem ** problem ;

int problemCounter=0;
//int OperatorIndividual::countP=0;
int main(int argc, char ** argv)
{

/*	if(argc==1)
	{
		cout<<"will random generate problem instance ,saved in problem.txt"<<endl;
		problem *p =new Problem (DEFAULTCITYNUMBER);

	}
	else if(argc==2)
	{

		Problem *p=new Problem();
		p->readProblemFromFile(argv[1]);
	}
	else
	{
		cout<<"erorr, the usage should be "<< argv[0]<<" or "<< argv[0]

	}*/



	timer t;
	myRandom =new Random();
	myglobalSetup=new GlobalSetup();
	myglobalSetup->readConfigFromFile("setupForHyper.txt");

	lowSetup =new GlobalSetupForLow();
	lowSetup->readConfigFromFile("setupForLowGA.txt");
	lowSetup->setOpeartorLen(myglobalSetup->numOfDecisionVariable);
	vector<string> problemNames;

	string pName;
	ifstream inName;
	inName.open("file_list.txt");
	if(!inName)
	{
		cout<<"open file_list.txt  error"<<endl;
		return 0;
	}
	vector<string>::iterator iter=problemNames.begin();
	while(inName>>pName)
	{
	//	inName>>pName;
		problemNames.push_back(pName);
//		*iter=pName;
//		iter++;
	}
//        Problem ** problem ;
	
	problem =new Problem * [PROBLEMNUM];
	iter=problemNames.begin();
	for(int i=0;i<PROBLEMNUM&&iter!=problemNames.end();i++,iter++)
	{
		problem [i]=new Problem();
		problem[i]->readProblemFromFile(*iter);
	}
	GA * ga =new GA(myglobalSetup->numOfDecisionVariable); //2表示算子种群，即高层GA
	ga->evolute();

	inName.close();


	cout<<endl;
	cout<<endl;
	cout<<"time:::"<<endl;

	cout<<t.elapsed();

	return 0;

	
}
#endif
