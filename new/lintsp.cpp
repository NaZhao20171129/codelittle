#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<fstream>
#include<algorithm>
#include<cmath>
#include<map>



using namespace std;

const int CityNum=9847;
const double MAX =100000000000000.0;
const double undone= -1;
const int initTime =100000;
double mutationRate;
double crossoverRate;
int  popSize;
int iterSize;
int LsTime;

typedef struct point
{
	double x,y;
};

vector<point> problem;

double comdis(point &a, point &b);
int readProblem()
{
	ifstream in;
	in.open("ja9847.tsp");
	if(!in)
	{
		cout<<"cannot open file ja9847.tsp.txt to read problem"<<endl;
		exit(0);
	}

	int num;
//	in>>num;
//	if(num!=CityNum)
//	{
//		cout<<"city number not the same"<<endl;
//		exit(0);

//	}

	double x,y;
	point p;
	for(int i=0;i<CityNum;i++)
	{
		in>>x>>y;
		p.x=x;
		p.y=y;
		problem.push_back(p);
		
	}

	cout<<"load problem over"<<endl;
}
class individual
{
	public:
		individual();
		individual(int x);
		vector<int> gene;
		double fitness;


		individual(const  individual&);
		int crossover( individual& ,individual& son1, individual&  son2);
		int crossoverOX(  individual& par ,individual& son1, individual&  son2);
		int mutation(individual & son);
		int ils(individual &bestson);

		int swapCity(int x,int y);
		int check();
		int display(ofstream & a);
};

class popTSP
{
	public:
		vector< individual> inds;
		vector< individual> pool; 
		popTSP();
		popTSP(const  popTSP &);
		int popEvolution();
		int crossover();
//		int mutation(individual & x);
		int mutation();
		int ls();
		int printBest();
		int evaluate(int x);
		int select(int x);


		int selectGreedy();
		int selectWheelRoute();

		ofstream outop;
		ofstream poplog;
		ofstream outbest;
		int pcur;
};
int main()
{
	ifstream in;
	in.open("linconfig.ini");
	if(!in)
	{
		cout<<"cannot open linconfig.ini to read parameters"<<endl;
		exit(0);
	}

	in>>mutationRate>>crossoverRate>>popSize>>iterSize>>LsTime;


	readProblem();
	popTSP s;

	s.evaluate(1);

//	for(int i=0;i<s.inds.size();i++)
//		cout<<s.inds[i].fitness<<"\t";
	cout<<endl;
	s.popEvolution();
	cout<<"over********************************************************"<<endl;
	return 0;

}



popTSP::popTSP()
{

	inds.resize(popSize);
	pool.resize(popSize*10);

	for(int i=0;i<inds.size();i++)
	{
		individual * a=new individual(1);
		inds[i]=*a;
		delete a;
	}
	pcur=0;
	outop.open("oplog.txt");
	if(!outop)
	{
		cout<<"open oplog.txt failed,exit"<<endl;
		exit(0);
	}

	poplog.open("poplog.txt");
	if(!poplog)
	{
		cout<<"open poplog.txt error,exit"<<endl;
		exit(0);
	}

	outbest.open("best.txt");
	if(!outbest)
	{
		cout<<"open best.txt error,exit"<<endl;
		exit(0);
	}

}
int popTSP::popEvolution()
{


	srand(time(NULL));
	for(int i=0;i<iterSize;i++)
	{
		if(i>2000 & i%3==0)
			crossover();
		mutation();
		ls();
		evaluate(0);
		select(1);
		printBest();
	}
}

int popTSP::mutation()
{
	for(int i=0;i<inds.size();i++)
		inds[i].mutation(pool[pcur++]);

	outop<<"muation new "<<inds.size()<<endl;

}


int popTSP::crossover()
{

	int parentid;
	for(int i=0;i<inds.size();i++)
	{

		parentid=rand()%inds.size();
		while(parentid==i)
			parentid=rand()%inds.size();
	//	inds[i].crossover(inds[parentid],pool[pcur++],pool[pcur++]);
		inds[i].crossoverOX(inds[parentid],pool[pcur++],pool[pcur++]);
	}
	cout<<"crossover new "<<inds.size()*2<<" sons"<<endl;
		
}

int popTSP::ls()
{

	for(int i=0;i<inds.size();i++)
	{
		inds[i].ils(pool[pcur++]);
	}
}

int popTSP::evaluate(int flag)
{

	if(flag==0)  // the pool
	{
		int first=0,second=0;
		for(int i=0;i<pool.size();i++)
		{
			double fit=0;
			pool[i].fitness=0;
			for(int j=1;j<pool[i].gene.size();j++)
			{
				first=pool[i].gene[j-1];
				second=pool[i].gene[j];

				fit =pow(problem[first].x-problem[second].x,2) + pow(problem[first].y-problem[second].y,2);

				pool[i].fitness+=sqrt(fit);
			}

		}
	}

	
	else  //the inds
	{
		int first=0,second=0;
		for(int i=0;i<inds.size();i++)
		{
			double fit=0;
			inds[i].fitness=0;
			for(int j=1;j<inds[i].gene.size();j++)
			{
				first=inds[i].gene[j-1];
				second=inds[i].gene[j];

				
				fit =pow(problem[first].x-problem[second].x,2) + pow(problem[first].y-problem[second].y,2);

				inds[i].fitness+=sqrt(fit);
			}

		}
	}
}

int popTSP::select(int type)
{

	if(type==1)
		selectGreedy();
	else if (type==2)
		selectWheelRoute();

}


int com(const  individual &x,const  individual &y)
{
	return x.fitness<y.fitness;
}
int popTSP::selectGreedy()
{
	for(int i =0;i<inds.size();i++)
		pool[pcur+i]=inds[i];
	pcur+=inds.size();
	sort(pool.begin(),pool.begin()+pcur,com);
	inds.clear();
	for(int i=0;i<popSize;i++)
		inds.push_back(pool[i]);
//	cout<<"after selected ,popSize= "<<inds.size()<<endl;

	pool.clear();
	pool.resize(popSize*10);
	pcur=0;
}

int popTSP::selectWheelRoute()
{}


int popTSP::printBest()
{
	cout<<"best= "<<inds[0].fitness<<endl;
	outbest<<inds[0].fitness<<endl;
	
	for(int i=0;i<inds.size();i++)
		poplog<<inds[i].fitness<<"\t";
	poplog<<endl;
	poplog<<endl;

	poplog<<"best= "<<inds[0].fitness<<endl;
	for(int i=0;i<inds[0].gene.size();i++)
		poplog<<inds[0].gene[i]<<"\t";
	poplog<<endl;
	poplog<<endl;
}


/*******************************************************************/
individual::individual(int a)
{
	gene.resize(CityNum);

	for(int i=0;i<gene.size();i++)
		gene[i]=i;
	for(int i=0;i<initTime;i++)
	{
		swapCity(rand()%CityNum,rand()%CityNum);
	}

	fitness=undone;
}

individual::individual()
{
	gene.resize(CityNum);
	fitness=undone;
	for(int i=0;i<gene.size();i++)
		gene[i]=i;
}

int individual::swapCity(int x,int y)
{
	int w;
	w=gene[x];
	gene[x]=gene[y];
	gene[y]=w;
}

int individual::mutation(individual & son)
{
	son=*this;
	int f, s;
	f=rand()%CityNum;
	s=rand()%CityNum;

	son.swapCity(f,s);
	
}


int individual::crossoverOX(  individual& par ,individual& son1, individual&  son2)
{
	//cout<<"in ox"<<endl;
	int loc1=rand()%CityNum;
	int loc2=rand()%CityNum;
	while(loc2==loc1)
		loc2=rand()%CityNum;

	if(loc1>loc2)
	{
		int tem=loc1;
		loc1=loc2;
		loc2=tem;
	}

	int i=0,flag=0,j=0;

	son1=*this;
	son2=par;
// 构造child1
	int curchild=loc2,curp=loc2;
	for( i=0;i<loc1+CityNum-loc2;)  //共需要重新填充geneLen-(loc2-loc1)个位置的gene
	{

		flag=0;
		for(j=loc1;j<loc2;j++)
		{
			if(par.gene[curp]==this->gene[j])
			{
				flag=1;
				break;
			}
		}

		if(flag==0)
		{
			son1.gene[curchild]=par.gene[curp];
			curchild=(curchild+1)%CityNum;
			i++;
		}
		curp=(curp+1)%CityNum;
	}

	curchild=loc2,curp=loc2;
	for( i=0;i<loc1+CityNum-loc2;)  //共需要重新填充geneLen-(loc2-loc1)个位置的gene
	{

		flag=0;
		for(j=loc1;j<loc2;j++)
		{
			if(this->gene[curp]==par.gene[j])
			{
				flag=1;
				break;
			}
		}

		if(flag==0)
		{
			son2.gene[curchild]=this->gene[curp];
			curchild=(curchild+1)%CityNum;
			i++;
		}
		curp=(curp+1)%CityNum;
	}

	/*if(son1.check()<0)
	{
		cout<<"cross over son1 error"<<endl;
		exit(0);
	}
	if(son2.check()<0)
	{
		cout<<"cross over son2 error"<<endl;
		exit(0);
	}*/
}
int individual::crossover(  individual& par ,individual& son1, individual&  son2)

{
	son1=*this;
	son2=par;

	if(son1.check()<0)
	{
		cout<<"parent1 illegal"<<endl;
		exit(0);
	}
	if(son2.check()<0)
	{
		cout<<"parent2 illegal"<<endl;
		exit(0);
	}


	int loc1=rand()%CityNum;
	int loc2=rand()%CityNum;
	while(loc2==loc1)
		loc2=rand()%CityNum;

	if(loc1>loc2)
	{
		int tem=loc1;
		loc1=loc2;
		loc2=tem;
	}
	int flag=0,i=0;
	int count;
	for( i=0;i<loc1;i++)
	{

		int cur;
		cur=par.gene[i];
		flag=0;
		count=0;
		while(1)
		{
			for(int j=loc1;j<loc2;j++)
			{
				if(cur==son1.gene[j])
				{
					cur=par.gene[j];
					count=0;
					break;
				}
				else
				{
					count++;
					
				}
				if(count==loc2-loc1)
				{
					flag=1;
					break;
				}
				
			}
			if(flag==1)
				break;
		}
	}


	for(i=loc2;i<CityNum;i++)
	{

		int cur;
		cur=par.gene[i];
		flag=0;
		while(1)
		{
			for(int j=loc1;j<loc2;j++)
			{
				if(cur==son1.gene[j])
				{
					cur=par.gene[j];
					count=0;
					break;
				}
				else
				{
					count++;
					
				}
				if(count==loc2-loc1)
				{
					flag=1;
					break;
				}
				
			}
			if(flag==1)
				break;
		}
	}


	cout<<"son1"<<endl;
	if(son1.check()<0)
	{
		ofstream out;
		out.open("err.txt");
		this->display(out);
		par.display(out);
		son1.display(out);
		exit(0);
	}

	

//for son2



	for( i=0;i<loc1;i++)
	{
		flag=0;
		for(int j=loc1;j<loc2;j++)
		{
			if(this->gene[i]==par.gene[j])
				{
					son2.gene[i]=this->gene[j];
					flag=1;
					break;
				}
		}
		if(flag==0)
			son2.gene[i]=this->gene[i];
	}			
	for(i=loc2;i<CityNum;i++)

	{
		flag=0;
		for(int j=loc1;j<loc2;j++)
		{
			if(this->gene[i]==par.gene[j])
				{
					son2.gene[i]=this->gene[j];
					flag=1;
					break;
				}
		}
		if(flag==0)
			son2.gene[i]=this->gene[i];
	}

	cout<<"son2"<<endl;
	//son2.check();
}
individual::individual(const  individual& ind)
{
	this->gene=ind.gene;
	this->fitness=ind.fitness;
}


int individual::ils(individual & best)
{

	//cout<<"doing 2opt"<<endl;
	//

	individual cur=*this;
	int loc1=0,loc2=0;
	int t=0;
	int count=0;
	while( t++<LsTime && count<100)
	{
		loc1=rand()%(CityNum-2)+1;
		loc2=rand()%(CityNum-2)+1;
		while(loc2==loc1)
			loc2=rand()%(CityNum-2)+1;
		if(loc2<loc1)
		{
			int tmp=loc2;
			loc2=loc1;
			loc1=tmp;
		}
		double before=0;
		double after=0;

		//cout<<"loc1= "<<loc1<<"  loc2= "<<loc2<<endl;
		before+=comdis(problem[ cur.gene[loc1] ],problem[cur.gene[loc1-1] ]);
		before+=comdis(problem[ cur.gene[loc2] ],problem[cur.gene[loc2+1] ]);

		after+=comdis(problem[ cur.gene[loc2] ],problem[cur.gene[loc1-1] ]);
		after+=comdis(problem[ cur.gene[loc1] ],problem[cur.gene[loc2+1] ]);

		if(before>after)
		{
			int i=loc1,j=loc2,tmp=0;
			while(i<j)
			{
				tmp=cur.gene[i];
				cur.gene[i]=cur.gene[j];
				cur.gene[j]=tmp;
				++i;
				--j;
			
			}
			count=0;
		}
		else
			count++;

	}

	best=cur;
//	cout<<"lsson"<<endl;
//	best.check();
}

double comdis(point &a, point &b)
{

	return pow(a.x-b.x,2) + pow(a.y-b.y,2);
}


int individual::check()
{
	map<int,int> che;
	for(int i=0;i<CityNum;i++)
	{
		if(che[gene[i] ]!=1)
			che[gene[i] ]=1;
		else
		{
//			cout<<"not eagle individual at gene "<<gene[i]<<endl;

//			sort(gene.begin(),gene.end());
//			ofstream out;
//			out.open("err.txt");
//			display(out);
//			exit(0);
			return -1;
		}

	}
	return 1;
}

int individual::display(ofstream & out)
{
	for(int i=0;i<CityNum;i++)
		out<<gene[i]<<endl;
	out<<"fitness= "<<fitness<<endl;
}
