#include<iostream>
#include<fstream>
#include<vector>
#include<bitset>
#include<cstdlib>
#include<cmath>
#include<algorithm>

using namespace std;
/********************************variable declear******************************/
const int Lenx=18; 
const int Leny=15; 
typedef struct gene
{
	bitset<Lenx> x;
	bitset<Leny> y;
	double fit;

	bool operator ==( gene & r)
	{
		return fit==r.fit && x==r.x && y==r.y;
	}
};
const double PI = 3.1415926535897932384626433832795;
const int Basex=262143;
const int Basey=32767;

int popSize;
double mup;
double crossp;
int Stop;
double Time;

vector<gene> pop;
vector<gene> pool;

/********************************funciton declear******************************/
int com(const gene &a,const gene &b)
{
	return a.fit>b.fit;
}
int initProblem();  // init the problem .read the praameters from file
int domutation();  // make mutation on one individual

int allcross();  // let the pop to corss freely
int docross(int cur); // use too parent to cross
int dols(); // add a local search in ga. but I did't do it
int doevaluate(gene & g); // evaluate each individual on the prolem given
int doselect();  //just select the best N ones
int gaProcess(); // the main code for GA
int display(); // output the result , I used a python script to show the process of the best solution
int initGene(gene & a); // random init a individual
/*******************************************the main funtion****************************/
int main()
{
	cout<<"useage"<<endl;
	cout<<"need a gaconfig.ini file to config, and the ./ga to run "<<endl;
	initProblem();
	gaProcess();
	display();
}

/********************************funtion completion******************************************/

int initGene(gene & ge)
{
	int ranTime;
	//srand(time(NULL));
	ranTime=rand()%Lenx;
	for(int j=0;j<ranTime;j++)
		ge.x.set(rand()%Lenx,1);
	ranTime=rand()%Leny;
	for(int j=0;j<ranTime;j++)
		ge.y.set(rand()%Leny,1);
}

int disGene(gene & ge)
{
	cout<<ge.x.to_string()<<"\t"<<ge.y.to_string()<<"\t"<<ge.fit<<endl;
}
int initProblem()
{
	ifstream in;
	in.open("gaconfig.ini");
	if(!in)
	{
		cout<<"please prepare gaconfig.ini file"<<endl;
		exit(0);
	}

	in>>popSize;
	in>>mup;
	in>>crossp;
	in>>Time;
	in>>Stop;
	pop.resize(popSize);

	srand(time(NULL));
	for(int i=0;i<pop.size();i++)
	{
		initGene(pop[i]);
		doevaluate(pop[i]);
	}
	 for(int i=0;i<pop.size();i++)
	{
		disGene(pop[i]);
	}
	
	 in.close();
}

int docross(int cur)
{
	int pos;
	gene son1,son2;
	gene p1,p2;

	int pd1,pd2;


	//pd1=rand()%pop.size();
	pd1=cur;
	pd2=rand()%pop.size();
	
	while(pd2==pd1)
		pd2=rand()%pop.size();
	cout<<"pd1= "<<pd1<<" pd2= "<<pd2<<endl;
	p1=pop[pd1];
	p2=pop[pd2];


	pos=1+rand()%(Lenx+Leny-2);
	if(pos<Lenx)  //交叉点落在前段
	{
		for(int i=0;i<pos;i++)
		{
			son1.x.set(i,p1.x[i]);
			son2.x.set(i,p2.x[i]);
			
		}
		for(int i=pos;i<Lenx;i++)
		{

			son1.x.set(i,p2.x[i]);
			son2.x.set(i,p1.x[i]);
		}
		for(int i=0;i<Leny;i++)
		{
			son1.y.set(i,p2.y[i]);
			son2.y.set(i,p1.y[i]);
		}
	}

	else   //交叉点落在hou段
	{
		for(int i=0;i<Lenx;i++)
		{
			son1.x.set(i,p1.x[i]);
			son2.x.set(i,p2.x[i]);
		}

		for(int i=0;i<pos-Lenx;i++)
		{

			son1.y.set(i,p1.y[i]);
			son2.y.set(i,p2.y[i]);
		}

		for(int i=0;i<Leny-pos;i++)
		{
			son1.y.set(i,p2.y[pos+i]);
			son2.y.set(i,p1.y[pos+i]);
		}
	}

	doevaluate(son1);
	doevaluate(son2);
	pool.push_back(son1);
	pool.push_back(son2);

	//disGene(son1);
	//disGene(son2);
}


int allcross()
{
	double p=0;
	for(int i=0;i<pop.size();i++)
	{
		p=rand()%100*1.0/100;
		if(p<crossp)
		{
			docross(i);
		}
	}
}
int domutation()  //only mutate one parent each time
{
	int pd =rand()%pop.size();
	gene p=pop[pd];

	cout<<"mutation parent : pd = "<<pd<<endl;
	gene son;

	for(int i=0;i<Lenx;i++)
	{
		if(rand()%100*1.0/100<mup)
			son.x.set(i,~p.x[i]);
		else
			son.x.set(i,p.x[i]);

	}
	for(int i=0;i<Leny;i++)
	{
		if(rand()%100*1.0/100<mup)
			son.y.set(i,~p.y[i]);
		else
			son.y.set(i,p.y[i]);
	}

	doevaluate(son);
	pool.push_back(son);
	disGene(son);
}
int dols()
{
	//for(int i=0;i<pop.size();i++)
//	{
		
//	}
}
int doselect()
{

//	cout<<"hee"<<endl;
	sort(pool.begin(),pool.end(),com);

//	cout<<"here is the pool"<<endl;
//	for(int i=0;i<pool.size();i++)
//		disGene(pool[i]);

	gene cur;
	pop.clear();
	for(int i=0;i<pool.size();i++)
	{

		int flag=1;
		cur=pool[i];
		for(int j=0;j<pop.size();j++)
		{
			if(pop[j]==cur)
			{
				cout<<"has exit"<<endl;
				flag=0;
				break;
			}
			
		}

		if(flag==1)
			pop.push_back(cur);
		if(pop.size()==popSize)
			break;
	
	}

	cout<<"here is the pop select"<<endl;
	for(int i=0;i<pop.size();i++)
		disGene(pop[i]);

}

int doevaluate(gene & g)
{
	double x=-3.0+1.0*g.x.to_ulong()*(12.1+3.0)/Basex;
	double y=4.1+1.0*g.y.to_ulong()*(5.8-4.1)/Basey;
	g.fit=21.5+x* sin(4* PI * x) + y * sin(20 * PI * y);

}
int display()
{}
int gaProcess()
{
	int count=0;
	time_t start,end;
	start=time(NULL);
	double t=0;

	vector<double> allfit;
	while(count++<Stop && t<Time)
	{

		pool.clear();
		pool=pop;
		allcross();
		domutation();
		dols();
		//doevaluate(gene);
		doselect();
//display the pool
//	cout<<"here is the pool"<<endl;
//	for(int i=0;i<pool.size();i++)
//		disGene(pool[i]);
		end=time(NULL);
		t=difftime(start,end);

		allfit.push_back(pop[0].fit);
	}

	ofstream out;
	out.open("data.txt");
	if(!out)
	{
		cout<<"cannot open file to write"<<endl;
		exit(0);
	}

	for(int i=0;i<allfit.size();i++)
		out<<allfit[i]<<"\t";
	out<<endl;
	out.close();
}
