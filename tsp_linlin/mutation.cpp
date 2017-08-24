#include"mutation.hpp"
#define SWAPTIMES 10
int Mutation:: makingMutation(int type)
{

	Individual * tmppar;
	double p=0.0;
	if(type==1||type==6  ||type==7 ||type==8)             //表示第一种突变方式
	//这里的条件是为了让低层的mutation都走这条线
	{
		//对于给定种群中的个体，都用mutation
		for(int i=0;i<pop->getPopSize();i++)
			{

				tmppar=pop->getIndividualAt(i);
//				cout<<"the tmpparent is "<<*tmppar<<endl;

		//		setParent(tmppar);
				if(result!=NULL)
				{
					delete result;
					result=0;
				}
				result=tmppar->clone();
				if(!GenewiseMutation())
				{
					cout<<"makingMutation error"<<endl;
					return 0;
				}
//				return 0;
			}
	}
	else if(type==2)
	{
		for(int i=0;i<pop->getPopSize();i++)
		{
			tmppar=pop->getIndividualAt(i);
			setParent(tmppar);
			//setParent(pop->getIndividualAt(i));
			SelectiveMutation();
		}
	}
	else
	{
		cout<<"mutation type error"<<endl;
	}
	return 0;

}
//对于每个个体，选定几位进行突变。位数固定
int Mutation::SelectiveMutation()
{
	int times=myglobalSetup->mutationTimes;
	int loc=0,newopt=0;
	for(int i=0;i<times;i++)
	{
		loc=myRandom->boundedIntegerRandom(0,pop->getGeneLen());
		if(loc>pop->getGeneLen())
		{
			cout<<"error,location > geneLen"<<endl;
			return 0;
		}
		newopt=myRandom->boundedIntegerRandom(1,myglobalSetup->maxOperatorNum);
		result->setGeneAt(loc,newopt);

	}
	pop->addToPool(result);
	result=0;
	return 0;

}
//对与每个基因位，都以一定的概率进行突变
int Mutation::GenewiseMutation()
{
	double p=0.0;
	int newopt=0;//突变后新的operator


//	cout<<"before genewisemutation ,the result is "<<*result<<endl;
	for(int i=0;i<pop->getGeneLen();i++)
	{
		p=myRandom->random01();
		if(p<myglobalSetup->mutationRate)
		{
			newopt=myRandom->boundedIntegerRandom(1,myglobalSetup->maxOperatorNum);
			result->setGeneAt(i,newopt);
		}
	}

//	cout<<"after genewisemution ,the result is "<<*result<<endl;
	
	if(!pop->addToPool(result))
	{
		cout<<"mutation failed,adding to pool error"<<endl;
		return 0;
	}

//	cout<<"pool changed**************************************************"<<endl;
//	pop->outPutPoolStatus();
	return 1;
}

void Mutation:: setParent(Individual * p)

{
	if(p!=NULL)
		parent =p;
	else
		cout<<" P is empty in setParent"<<endl;

//	if(pop->poolPtr!=0)
//	{
//		cout<<"in setting parrent ,before result deleted,the pool[0] is"<<endl;
//		cout<<*pop->pool[0];
//	}
//	cout<<"cloning a individual"<<endl;
//	if(result!=NULL)
//	{
//		cout<<"the result is "<<*result<<endl;
//		delete result;
//		result=0;
//	}
//	result=parent->clone();
//	if(pop->poolPtr!=0)
//	{
//		cout<<"in setting parrent ,after result deleted,the pool[0] is"<<endl;
//		cout<<*pop->pool[0];
//	}
}



int ProblemMutation::makingMutation(int type)
{
	switch (type)
	{
		case 1:
			swap();
			break;
		default:
			swap();
			break;
	}
}


int ProblemMutation::swap()
{
	cout<<"in swap mutation";
	int loc1,loc2;
	int tmp;

//	for(int i=0;i<parent->geneLen;i++)
//		parent->setGeneAt(i,i);
	if(result!=NULL)
	{
		delete result;
		result=NULL;
	}
//	cout<<"the parent is "<<*parent<<endl;
	result =parent->clone();

  
	for(int i=0;i<SWAPTIMES;i++)
	{
		loc1=myRandom->boundedIntegerRandom(0,pop->geneLen-1);
		loc2=myRandom->boundedIntegerRandom(0,pop->geneLen-1);
		
	//	cout<<"loc1= "<<loc1;
	//	cout<<"  loc2= "<<loc2<<endl;
//
//		cout<<"result is "<<*result<<endl;

		tmp=*result->getIndividualGeneAt(loc1);
		result->setGeneAt(loc1,*result->getIndividualGeneAt(loc2));
		result->setGeneAt(loc2,tmp);
	}

//		pop->pool[pop->poolPtr]=result->clone();
//
//		pop->poolPtr++;
		pop->addToPool(result);
	//	result->rank();
	//	cout<<*result<<endl;
		delete result;
		result=NULL;

		
}
