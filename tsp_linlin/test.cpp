#include"random.hpp"
#include"globalSetup.hpp"
#include"chromosome.hpp"
#include"individual.hpp"
#include<iostream>
#include<string>
#include<fstream>

using namespace std;
	
Random *  myrandom=new Random();
GlobalSetup * myglobalSetup=new GlobalSetup();
int ** problem;


bool funbool(int x,int y)
{
	if(x<y)
		return 1;
	else
		return 0;
}


int readProblem(string fileName)
{
	if(problem!=NULL)
	{
		for(int i=0;i<myglobalSetup->clauseNum;i++)
			if(problem[i]!=NULL)
			{
				delete problem[i];
				problem[i]=NULL;
			}

		delete problem ;
		problem =NULL;
	}

	ifstream in;
	in.open(fileName.c_str());
	if(!in)
	{
		cout<<"cannot open file "<<fileName <<"in function readProblem"<<endl;
		return 0;

	}
	problem=new int* [myglobalSetup->clauseNum];
	for(int i=0;i<myglobalSetup->clauseNum;i++)
	{
		problem [i]=new int [myglobalSetup->variableInClause];
		for(int j=0;j<myglobalSetup->variableInClause;j++)
			in>>problem[i][j];
	}

	in.close();
	return 1;

}

void displayProlem(int ** p)
{
	if(p==NULL)
	{
		cout<<"the problem is empty"<<endl;
		return ;
	}
	for(int i=0;i<myglobalSetup->clauseNum;i++)
	{
		for(int j=0;j<myglobalSetup->variableInClause;j++)
		{
			cout<<problem[i][j]<<" ";
		}

		cout<<endl;
	}
}
int main(int argc, char * * argv)
{
	if(argc<2)
	{
		cout<<"usage:"<<argv[0]<<" problemFile "<<endl;
		return 1;
	}


	string fileName="setup.txt";
	if(!myglobalSetup->readConfigFromFile(fileName))
	{
		cout<<"reading from setup.txt errors"<<endl;
		exit(0);
	}

	string problemFile=argv[1];

	cout<<*myglobalSetup;


//	Chromosome c;
//	cout<<c;

	readProblem(problemFile);
//	displayProlem(problem);
//
 	Individual ind;
	ind.evaluateFitness();
	cout<<ind;
	return 0;
	
	
	
//	cout<<r.random01()<<endl;

//        int x=3,y =5;
//        int res=funbool(x,y);
//        cout<<res;
//        return 0;
//        
//
	

	
}
