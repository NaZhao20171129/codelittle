#include"crossover.hpp"
#define CROSSSWAPTIMES 25

/*****************************************************************************************************/
OneParentCrossOver::OneParentCrossOver(Population * pop,int t):CrossOver(pop,t)
{

}



OneParentCrossOver::OneParentCrossOver(Population * pop,Individual * p,int t):CrossOver(pop,t)
{
	parent =p;
}




/*****************************************************************************************************/
TwoParentCrossOver::TwoParentCrossOver(Population * pop,int t):CrossOver(pop,t)
{
//	parent1=new Individual();
//	parent2=new Individual();
//	crossType=t;
}


//TwoParentCrossOver::TwoParentCrossOver(Individual * p1)
//{
//        parent1=p1;
//        parent2=new Individual();
//}

TwoParentCrossOver::TwoParentCrossOver(Population * pop,Individual *p1,int t):CrossOver(pop,t)
{
	parent1=p1;
	parent2=NULL;
	crossType=t;
}

TwoParentCrossOver::TwoParentCrossOver(Population * pop , Individual *p1,Individual *p2,int t):CrossOver(pop,t)
{
	parent1 =p1;
	parent2=p2;
}


/*****************************************************************************************************/


MultiParentCrossOver ::MultiParentCrossOver(Population * pop, int t):CrossOver(pop,t)
{
}



MultiParentCrossOver ::MultiParentCrossOver(Population * pop, Individual * p,int t):CrossOver(pop,t)
{

	parent1=p;
}



MultiParentCrossOver ::MultiParentCrossOver(Population * pop,Individual *p1,Individual *p2,Individual *p3,int t):CrossOver(pop,t)
{

	parent1=p1;
	parent2=p2;
	parent3=p3;
}

/*****************************************************************************************************/
int ProblemOneParentCrossOver::makingCrossover(int type)
{
	int result =0;
	if(type==4)
		result =swap();
	else if(type==5)
		result =inverse();
	else
	{
		cerr<<"making crossover type error in Problem oneP "<<endl;
		cerr<<"the wrong type is "<<type<<endl;
		exit(0);
	}
	return result;
}

int ProblemOneParentCrossOver::swap()
{
	cout<<"swap in crossover ";
	int tmp;
	Individual *result=NULL;	
	int loc1,loc2;
	if(result!=NULL)
	{
		delete result;
		result=NULL;
	}
	result =parent->clone();
  
	for(int i=0;i<CROSSSWAPTIMES;i++)
	{
		loc1=myRandom->boundedIntegerRandom(0,pop->geneLen);
		loc2=myRandom->boundedIntegerRandom(0,pop->geneLen);

		tmp=*result->getIndividualGeneAt(loc1);
		result->setGeneAt(loc1,*result->getIndividualGeneAt(loc2));
		result->setGeneAt(loc2,tmp);
	}

	pop->addToPool(result);
	delete result;
	result=NULL;
	
}
int ProblemOneParentCrossOver::inverse()
{

	cout<<"in inverse ";
	int tmp1,tmp2;
	
	Individual * result=NULL;
	if(result!=NULL)
	{
		delete result;
		result=NULL;
	}
	result =parent->clone();
	int loc1,loc2;
  
	loc1=myRandom->boundedIntegerRandom(0,pop->geneLen);
	loc2=myRandom->boundedIntegerRandom(0,pop->geneLen);
	if(loc1>loc2)
	{
		int a=loc1;
		loc1=loc2;
		loc2=a;
	}

	int i,j;
	i=loc1;j=loc2;
	while(i<j)
	{
		tmp1=*result->getIndividualGeneAt(i);
		tmp2=*result->getIndividualGeneAt(j);
		result->setGeneAt(i,tmp2);
		result->setGeneAt(j,tmp1);
		i++;
		j--;
	}

	pop->addToPool(result);
	delete result;
	result=NULL;
	
}
/*****************************************************************************************************/

int ProblemTwoParentCrossOver::makingCrossover(int type)
{
	
	//CX不用了，不好，还不好实现

	int result =0;
	if(type==7||type==6||type==5||type==10||type==11)
		result =PMX();
	else  if(type ==8 || type==9||type==4||type==12||type==13)
		result =OX();
	//else if(type==10 ||type==11||type==12||type==13)
	//	result =EX();
	else
	{	cerr<<"making crossover type error in Problem twoP"<<endl;
		cerr<<"the wrong type is"<<type<<endl;
		exit(0);
	}
	return result;
}

int ProblemTwoParentCrossOver::PMX()
{
	cout<<"in PMX ";
	if(parent1==NULL || parent2==NULL)
	{
		cerr<<"error p1 or p2 is empty in PMX"<<endl;
		exit(0);
	}

	else
	{
		int loc1=0,loc2=0;
		loc1=myRandom->boundedIntegerRandom(1,pop->geneLen-1);
		//cout<<"the problemloc1 is "<<loc1<<endl;
		loc2=myRandom->boundedIntegerRandom(1,pop->geneLen-1);
		while(loc1==loc2)
			loc2=myRandom->boundedIntegerRandom(1,pop->geneLen-1);

		//cout<<"the problemloc2 is "<<loc2<<endl;
		if(loc1>loc2)
		{
			int tmp=loc2;
			loc2=loc1;
			loc1=tmp;
		}

		int i=0,flag=0;
		Individual * child1=parent1->clone();
		Individual * child2 =parent2->clone();
// 构造child1
		for( i=0;i<loc1;i++)
		{
			flag=0;
			for(int j=loc1;j<loc2;j++)
			{
				if(*parent2->getIndividualGeneAt(i)==*parent1->getIndividualGeneAt(j))
					{
						child1->setGeneAt(i,*parent2->getIndividualGeneAt(j));
						flag=1;
						break;
					}
			}
			if(flag==0)
				child1->setGeneAt(i,*parent2->getIndividualGeneAt(i));
		}
		for(i=loc2;i<parent1->geneLen;i++)
		{
			flag=0;
			for(int j=loc1;j<loc2;j++)
			{
				if(*parent2->getIndividualGeneAt(i)==*parent1->getIndividualGeneAt(j))
					{
						child1->setGeneAt(i,*parent2->getIndividualGeneAt(j));
						flag=1;
						break;
					}
			}
			if(flag==0)
				child1->setGeneAt(i,*parent2->getIndividualGeneAt(i));
		}

// 构造child2
		for( i=0;i<loc1;i++)
		{
			flag=0;
			for(int j=loc1;j<loc2;j++)
			{
				if(*parent1->getIndividualGeneAt(i)==*parent2->getIndividualGeneAt(j))
					{
						child2->setGeneAt(i,*parent1->getIndividualGeneAt(j));
						flag=1;
						break;
					}
			}
			if(flag==0)
				child2->setGeneAt(i,*parent1->getIndividualGeneAt(i));
		}			
		for(i=loc2;i<parent1->geneLen;i++)
		
		{
			flag=0;
			for(int j=loc1;j<loc2;j++)
			{
				if(*parent1->getIndividualGeneAt(i)==*parent2->getIndividualGeneAt(j))
					{
						child2->setGeneAt(i,*parent1->getIndividualGeneAt(j));
						flag=1;
						break;
					}
			}
			if(flag==0)
				child2->setGeneAt(i,*parent1->getIndividualGeneAt(i));
		}







		//cout<<"before cross over ,the parent is"<<endl;
	//	cout<<* parent1<<endl;
	//	cout<<*parent2<<endl;
	//	cout<<"the locations are: "<<loc1<<"  "<<loc2<<endl;
	//	cout<<"after cross over ,the child are like"<<endl;
	//	cout<<*child1<<endl;
	//	cout<<*child2<<endl;
		pop->addToPool(child1);
		pop->addToPool(child2);
		delete child1;
		child1=NULL;
		delete child2;
		child2=NULL;

	}
	return 1;
}

int ProblemTwoParentCrossOver::OX()
{
	cout<<"in ox"<<endl;
	if(parent1==NULL || parent2==NULL)
	{
		cerr<<"error p1 or p2 is empty in OX"<<endl;
		exit(0);
	}

	else
	{
		int loc1=0,loc2=0;
		loc1=myRandom->boundedIntegerRandom(2,pop->geneLen-5);
		//cout<<"the LLHloc1 is "<<loc1<<endl;
		loc2=myRandom->boundedIntegerRandom(2,pop->geneLen-5);
		while(loc1==loc2)
			loc2=myRandom->boundedIntegerRandom(2,pop->geneLen-5);

		//cout<<"the LLHloc2 is "<<loc2<<endl;
		if(loc1>loc2)
		{
			int tmp=loc2;
			loc2=loc1;
			loc1=tmp;
		}

		int i=0,flag=0,j=0;
		Individual * child1=parent1->clone();
		Individual * child2 =parent2->clone();
// 构造child1
		int curchild=loc2,curp=loc2;
		for( i=0;i<loc1+parent1->geneLen-loc2;)  //共需要重新填充geneLen-(loc2-loc1)个位置的gene
		{

			flag=0;
			for(j=loc1;j<loc2;j++)
			{
				if(*parent2->getIndividualGeneAt(curp)==*parent1->getIndividualGeneAt(j))
				{
					flag=1;
					break;
				}
			}

			if(flag==0)
			{
				child1->setGeneAt(curchild,*parent2->getIndividualGeneAt(curp));
				curchild=(curchild+1)%parent1->geneLen;
				i++;
			}
			curp=(curp+1)%parent1->geneLen;
		}

		curchild=loc2,curp=loc2;
		for( i=0;i<loc1+parent1->geneLen-loc2;)  //共需要重新填充geneLen-(loc2-loc1)个位置的gene
		{

			flag=0;
			for(j=loc1;j<loc2;j++)
			{
				if(*parent1->getIndividualGeneAt(curp)==*parent2->getIndividualGeneAt(j))
				{
					flag=1;
					break;
				}
			}

			if(flag==0)
			{
				child2->setGeneAt(curchild,*parent1->getIndividualGeneAt(curp));
				curchild=(curchild+1)%parent1->geneLen;
				i++;
			}
			curp=(curp+1)%parent1->geneLen;
		}
		//cout<<"in ox"<<endl;
//                cout<<"before cross over ,the parent is"<<endl;
//                cout<<* parent1<<endl;
//                cout<<*parent2<<endl;
//                cout<<"the locations are: "<<loc1<<"  "<<loc2<<endl;
//                cout<<"after cross over ,the child are like"<<endl;
//                cout<<*child1<<endl;
//                cout<<*child2<<endl;
		pop->addToPool(child1);
		pop->addToPool(child2);
		delete child1;
		child1=NULL;
		delete child2;
		child2=NULL;
	}
}

int ProblemTwoParentCrossOver::CX()
{
}

bool veccom(const vector<int> &a,const vector<int> & b)
{
	return a.size()<b.size();
}


void removeCur(vector<vector<int> > & v ,int cur);
int ProblemTwoParentCrossOver::EX()
{

	cout<< "in ex ";
	if(parent1==NULL || parent2==NULL)
		
	{
		cout<<"error in EX ,parent is empty"<<endl;
		exit(0);
	}
	vector< vector<int> > cityGra(parent1->geneLen);
	int cur=0,before=0,after=0;
	int flagl=0,flagr=0;

//	cout<<"parents"<<endl;
//	cout<<*parent1<<endl;
//	cout<<*parent2<<endl;
	for(int i=0;i<parent1->geneLen;i++)
	{
		flagl=0,flagr=0;

		cur=*parent1->getIndividualGeneAt(i);
		cityGra[i].push_back(cur);
		before=*parent1->getIndividualGeneAt((i+1+parent1->geneLen)%parent1->geneLen);
		after=*parent1->getIndividualGeneAt((i-1+parent1->geneLen)%parent1->geneLen);
		//cout <<after<<"--"<<cur<<"--"<<before<<endl;
			cityGra[i].push_back(before);
			cityGra[i].push_back(after);
	}
/*	cout<<"the first"<<endl;
	for(int i=0;i<cityGra.size();i++)
	{
		cout<<i+1<<" : ";
		for(int j=0;j<cityGra[i].size();j++)
			cout<<cityGra[i][j]<<" ";
		cout<<endl;
	}
	cout<<"print over"<<endl;*/
/*****************************对parent2处理**********************************/
	int iindex=0;
	int count=0;
	for(int i=0;i<parent2->geneLen;i++)
	{
		flagl=0,flagr=0;

		cur=*parent2->getIndividualGeneAt(i); //取出一个城市
		for(int k=0;k<cityGra.size();k++) //找到该城市在之前map中的位置
			if(cityGra[k][0]==cur)
			{
				iindex=k;
				break;
			}
		before=*parent2->getIndividualGeneAt((i+1+parent2->geneLen)%parent2->geneLen);
		for(int j=1;j<cityGra[iindex].size();j++) 
		{
			if(before==cityGra[iindex][j])
			{	
				flagl=1;
				break;
			}
		}
		if(flagl==0)
		{
			cityGra[iindex].push_back(before);
	//		cout<<"inserting "<<++count<<endl;
		}
		after=*parent2->getIndividualGeneAt((i-1+parent2->geneLen)%parent2->geneLen);
		for(int j=1;j<cityGra[cur].size();j++)
		{
			if(after==cityGra[iindex][j])
			{	
				flagr=1;
				break;
			}
		}
		if(flagr==0)
		{
			cityGra[iindex].push_back(after);
	//		cout<<"inserting "<<++count<<endl;
		}
	}
//out put the map
/*	for(int i=0;i<cityGra.size();i++)
	{
		cout<<i+1<<" : ";
		for(int j=0;j<cityGra[i].size();j++)
			cout<<cityGra[i][j]<<" ";
		cout<<endl;
	}*/
//	cout<<"print over"<<endl;


///////////产生children
 

	Individual * child1,*child2;
	child1=new Individual (parent1->geneLen);
	//child2=new Individual (parent1->geneLen);

	child1->setGeneAt(0,*parent1->getIndividualGeneAt(0));
	//removeCur(cityGra,*parent1->getIndividualGeneAt(0));
	//sort(cityGra.begin(),cityGra.end(),veccom);
	//排序后，vector的第一个元素就是最少的vector
	int index=0;
	int min=0;
	for(int i=1;i<parent1->geneLen;)
	{
		cur=*child1->getIndividualGeneAt(i-1);
		for(int j=0;j<cityGra.size();j++)
			if(cityGra[j][0]==cur)
			{
				index =j;
				break;
			}
		if(cityGra[index].size()<2)
		{
			int ran=cityGra[myRandom->boundedIntegerRandom(0,cityGra.size())][0];
			while(ran==cityGra[index][0])
				 ran=cityGra[myRandom->boundedIntegerRandom(0,cityGra.size())][0];
			child1->setGeneAt(i,ran);
			i++;
			removeCur(cityGra,cityGra[index][0]);
		}
		else
		{	
			min=cityGra[index][1];
			for(int k=2;k<cityGra[index].size() ;k++)
			{
				if(cityGra[ cityGra[index][k] ].size()<cityGra[min].size())
					min=cityGra[index][k];
			
			}
			
			child1->setGeneAt(i,min);
			i++;
			removeCur(cityGra,cityGra[index][0]);
			sort(cityGra.begin(),cityGra.end(),veccom);
			//排序后，vector的第一个元素就是最少的vector
		
		}
	}

	pop->addToPool(child1);

//	cout<<"before ex ,the parents are"<<endl;
//	cout<<*parent1<<endl;
//	cout<<*parent2<<endl;
//	cout<<"after ex, the child is "<<endl;
//	cout<<*child1;
//	child1->rank();
//	cout<<*child1<<endl;

	delete child1;
	child1=NULL;
	return 1;
}


void removeCur(vector< vector<int> > &v, int cur)
{
	vector< vector<int> > ::iterator  iter=v.begin();
	for(;iter!=v.end();iter++)
		if(iter->front()==cur)
		{
			//cout<<"iter ===cur"<<endl;
			v.erase(iter);
			break;
		}
	vector<int >::iterator it;
	for(int i=0;i<v.size();i++)
	{
		for(it=v[i].begin();it!=v[i].end();it++)
		{
		//	cout<<*it<<endl;
		//	cout<<cur<<endl;
			if(*it==cur)
			{
				v[i].erase(it);
				break;
			}
		}
	}


}

/*****************************************************************************************************/
int ProblemMultiParentCrossOver::makingCrossover(int type)
{
	int result =0;
	if(type==13)
		result =threeParent();
	else
		cerr<<"making crossover type error in Problem threeP"<<endl;
	return result;

	
}


int ProblemMultiParentCrossOver::threeParent()
{
	return 1;
}
/*****************************************************************************************************/

int LLHOneParentCrossOver::twoPointAndCross()
{
	int loc1=0,loc2=0;
	loc1=myRandom->boundedIntegerRandom(1,pop->geneLen-1);
	loc2=myRandom->boundedIntegerRandom(1,pop->geneLen-1);
	while(loc2==loc1)
		loc2=myRandom->boundedIntegerRandom(1,pop->geneLen-1);
	
	if(loc1>loc2)
	{
		int tmp=loc2;
		loc2=loc1;
		loc1=tmp;
	}
	
	if(parent==NULL)
	{
		cout<<"parent in crossover of oneP for LLH"<<endl;
		exit(0);
	}
	
	Individual * child=parent->clone();
	int tmp=0;
	for(int i=loc1,j=loc2-1;i<j;i++,j--)
		{
			tmp=*child->getIndividualGeneAt(i);
			child->setGeneAt(i,*child->getIndividualGeneAt(j));
			child->setGeneAt(j,tmp);
		}


//        cout<<"before crossover ,the llh is "<<*parent<<endl;
//        cout<<"the locations are "<<loc1<<"   "<<loc2<<endl;
//        cout<<"after crossover, the llh is "<<*child<<endl;
	pop->addToPool(child);
	delete child;
	child=NULL;
	return 1;	
}

int LLHOneParentCrossOver::makingCrossover(int type)
{
	int result =0;
	if(type ==1 || type==2)
		result =twoPointAndCross();
	else 
		cerr<<"making crossover type error in LLH oneP"<<endl;
	return result;

}
/*****************************************************************************************************/



/*****************************************************************************************************/

int LLHTwoParentCrossOver::onePoint()
{
	int loc=myRandom->boundedIntegerRandom(2,pop->geneLen-3);
	Individual * child1=parent1->clone();
	Individual * child2 =parent2->clone();

	for(int i=0;i<loc;i++)
		child2->setGeneAt(i,*parent1->getIndividualGeneAt(i));
	for(int j=loc;j<pop->geneLen;j++)
		child1->setGeneAt(j,*parent2->getIndividualGeneAt(j));
	pop->addToPool(child1);
	pop->addToPool(child2);
	delete child1;
	child1=NULL;
	delete child2;
	child2=NULL;
}

int LLHTwoParentCrossOver::twoPoint()
{
	int loc1=myRandom->boundedIntegerRandom(2,pop->geneLen-3);
	int loc2=myRandom->boundedIntegerRandom(2,pop->geneLen-3);
	
	if(loc1>loc2)
	{
		int tmp=loc1;
		loc1=loc2;
		loc2=tmp;
	}
	Individual * child1=parent1->clone();
	Individual * child2 =parent2->clone();


	for(int i=loc1;i<loc2;i++)
	{
		child1->setGeneAt(i,*parent2->getIndividualGeneAt(i));
		child2->setGeneAt(i,*parent1->getIndividualGeneAt(i));

	}
	pop->addToPool(child1);
	pop->addToPool(child2);
	delete child1;
	child1=NULL;
	delete child2;
	child2=NULL;
}

int LLHTwoParentCrossOver::makingCrossover(int type)
{
	int result =0;
	if(type==4)
		result =onePoint();
	else if(type==5||type==6)
		result =twoPoint();
	else
		cerr<<"making crossover type error in LLH twoP"<<endl;
	return result;
}

/****************************************************************************************************/

int LLHMultiParentCrossOver::makingCrossover (int type)
{
	int result =0;
	if(type==7)
		result=threeParent();
	else

		cerr<<"making crossover type error in LLH threeP"<<endl;
		
	return result;
}


int LLHMultiParentCrossOver::threeParent()
{}
