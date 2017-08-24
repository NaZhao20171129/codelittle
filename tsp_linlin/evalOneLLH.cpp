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
#define PROBLEMNUM 1

#define EVATIMES 20
#include<sstream>
#include<vector>
//#include<boost/timer.hpp>


//using boost::timer;
using namespace std;
Random *myRandom;
GlobalSetup * myglobalSetup;
GlobalSetupForLow * lowSetup;
Problem ** problem ;

typedef struct llhqualforp
{
	int id;
	double fit;
};


int compare(const llhqualforp  &l1,const llhqualforp &l2)
{
	if(l1.fit<l2.fit)
		return 1;
	else
		return 0;
}
int problemCounter=0;
//int OperatorIndividual::countP=0;
int main(int argc, char ** argv)
{

	cout<<"usage: ./a.out  llhfile problemfile"<<endl;
//        timer t;
	myRandom =new Random();
	myglobalSetup=new GlobalSetup();
	myglobalSetup->readConfigFromFile("setupForHyper.txt");

	lowSetup =new GlobalSetupForLow();
	lowSetup->readConfigFromFile("setupForLowGA.txt");
	lowSetup->setOpeartorLen(myglobalSetup->numOfDecisionVariable);
	vector<string> problemNames;

	//Problem *testPro;
	//testPro=new Problem();
/*	string pfile;
	string llhfile;
	if(argc==2)
	{
		llhfile=argv[1];
		pfile="problem_50_11";
	}
	else if (argc==3)
	{
		llhfile=argv[1];
		pfile=argv[2];
	}
	else
	{
		llhfile="llh.txt";
		pfile="problem_50_11";
	}*/

	ifstream inName;
	inName.open("file_list.txt");
	if(!inName)
	{
		cout<<"cannot open file file_list.txt"<<endl;
		exit(0);
	}
	string pname;
	int pnum=0;
	while(!inName.eof() &&pnum++<PROBLEMNUM )
	{
		inName>>pname;
		problemNames.push_back(pname);

	}
	inName.close();

	string llhfile="llh.txt";
	ifstream inllh;
	inllh.open(llhfile.c_str());
	if(!inllh)
	{
		cout<<"cannot open file"<<llhfile<<endl;
		exit(0);
	}

	vector<vector<int> >  llhs;
	vector<int > tmpv;
	int tmp;
	while(!inllh.eof())
	{
		for(int i=0;i<lowSetup->operatorLen;i++)
		{	
			inllh>>tmp;
			tmpv.push_back(tmp);
		}

		llhs.push_back(tmpv);

		for(int i=0;i<tmpv.size();i++)
			cout<<tmpv[i]<<" ";
		cout<<endl;
		tmpv.clear();
	}

	inllh.close();

	double bestfit=10000000.0;
	double cur=0.0;
	int index=0;

//        system("cd storeProblems");
//        system("pwd");
	problem =new Problem *[PROBLEMNUM];
	string profile;
	for(int i=0;i<PROBLEMNUM;i++)
	{
		problem[i]=new Problem();
		profile=problemNames[i];
		cout<< profile<<endl;
		problem[i]->readProblemFromFile(profile.c_str());
	}


	//cout<<"the problem 160-64 start is"<<endl;
//	cout<<*problem[1]->startSolutions[0]<<endl;
	
	ofstream outres;
	outres.open("restoevalclassifi.txt");
	if(!outres)
	{
		cout<<"cannot open file restoevalclasifi.txt";
		exit(0);
	}


	OperatorIndividual * oillh=NULL;
	for(int k=0;k<PROBLEMNUM;k++)
	{

		int bestllh=0;
		bestfit=10000000.0;
		index=0;
		problem[k]->initIndividualForPro();
	//	cout<<"the problem 160-64 start is"<<endl;
	//	cout<<*problem[1]->startSolutions[0]<<endl;
	//	for(int j=0;j<EVATIMES;j++)
	//	{
		vector<llhqualforp> llhsort;
		llhqualforp tmpllhqualforp;
		for(int i =0;i<llhs.size();i++)
		{
			tmpllhqualforp.id=i;

			GA::EvalIndex=0;
			if(oillh!=NULL)
			{
				delete oillh;
				oillh=NULL;
			}
			oillh=new OperatorIndividual(llhs[i]);

			double fitavg=0.0;
			for(int j=0;j<EVATIMES;j++)
			{
				GA * ga=NULL;
				if(ga!=NULL)
				{
					delete ga;
					ga=NULL;
				}
				ga =new GA(problem[k]);
				cout<<problem[k]->proName<<endl;
				ga->evolute(oillh);
				cur=ga->population->guys[0]->fitness;
				if(ga!=NULL)
				{
					delete ga;
					ga=NULL;
				}
				if(bestfit>cur)
				{
					bestfit=cur;
				//	index=j;
				//	bestllh=i;
					cout<<"best solution is  "<<bestfit<<endl;
				}

				fitavg+=cur;
				GA::EvalIndex++;
			}

			fitavg=fitavg/(EVATIMES)*1.0;
			tmpllhqualforp.fit=fitavg;

			llhsort.push_back(tmpllhqualforp);
		}

		sort(llhsort.begin(),llhsort.end(),compare);
		for(int i=0;i<llhsort.size();i++)
		{
			outres<<llhsort[i].id<<" "<<llhsort[i].fit<<endl;
		}
		
		outres<<problem[k]->proName<<"\t"<<" bestLLH "<<"\t"<<llhsort[0].id<<endl;
		OperatorIndividual::countP++;
	}

	outres.close();
	cout<<endl;
	cout<<"time:::"<<endl;

//        cout<<t.elapsed();
	cout<<endl;
	cout<<"best fitness is "<<bestfit<<endl;
	cout<<"best fit is get at "<<index<<endl;

	return 0;

	
}
#endif
