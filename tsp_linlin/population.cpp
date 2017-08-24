#include"population.hpp"


Population::Population(int gl)
//两个任务：初始化种群，申请空间，并填充内容（貌似是由相应的别的类的构造函数完成
//另外，计算初始种群的各项统计数据。完成一些类内部的变量的赋值。
{
//	cout<<"initialize population with type "<<type<<endl;
	geneLen=gl;
	initializationPop(2);
	initlizeLLHs();
	bestIndividual=guys[0]->clone();
	computeObjStatistics();
	mapObjectiveToFitness();

	computeFitnessStatistics();


}

Population::Population(Problem *p)
{
	geneLen=p->numOfCity;
	initializationPop(1);
	initializeSolution(p);
	bestIndividual=guys[0]->clone();
	computeObjStatistics();
	mapObjectiveToFitness();

	computeFitnessStatistics();

}

int Population::initializationPop(int type )
//两个任务：根据类型需要，初始化种群，申请空间，并填充内容（貌似是由相应的别的类的构造函数完成
//另外，计算初始种群的各项统计数据。完成一些类内部的变量的赋值。
{
	maxFitness=new double(0);
	minFitness=new double(0);
	avgFitness=new double (0);	
	varFitness=new double(0);
	selection=NULL;
	mutation=NULL;
	crossover=NULL;
	localsearch=NULL;
	solutionselection=NULL;
	int i;

	popType=type;
//	guys=new Individual * [myglobalSetup->populationSize];
//	newguys=new Individual *[myglobalSetup->populationSize];

	poolLen =popSize * 5;
	poolPtr=0;
}
void Population::initlizeLLHs()

{

		popSize=myglobalSetup->populationSize;
		poolLen =popSize * 5;
		int i=0;
//		geneLen=myglobalSetup->numOfDecisionVariable;
		guys=new Individual *[popSize];
		newguys=new Individual *[popSize];
		pool =new Individual * [poolLen];
		for(i=0;i<poolLen;i++)
			pool[i]=NULL;
		for(i=0;i<popSize;i++)
		{
			guys[i]=new OperatorIndividual(geneLen);
			newguys[i]=new OperatorIndividual(geneLen);
			

		}
		//bestIndividual=new OperatorIndividual(myglobalSetup->numOfDecisionVariable);
//
//		for(i=0;i<poolLen;i++)
//			pool[i]=new OperatorIndividual(myglobalSetup->numOfDecisionVariable);
	
}
void Population::initializeSolution(Problem *p)
//低层GA，普通的种群，用于求解具体问题
{
		popSize=lowSetup->getPopSize();
		poolLen =popSize * 5;
		guys=new Individual *[popSize];
		newguys=new Individual *[popSize];
		pool =new Individual * [poolLen];
		for(int i=0;i<poolLen;i++)
			pool[i]=NULL;
		for(int i=0;i<popSize;i++)
		{
			guys[i]=new Individual(* problem[OperatorIndividual::countP]->startSolutions[i]);
			newguys[i]=new Individual(* problem[OperatorIndividual::countP]->startSolutions[i]);

		}
}


//ostream & operator >> (std::ostream & outfile, Population & pop)

int Population::genid=0;
ostream & operator <<(std::ostream & outfile, const Population & pop)
{
//	outfile.open("generation.txt",ofstream ::app);
	//outfile<< "the best individual is: "<<endl<<*(pop.getBestIndividual())<<endl;
	//outfile<<" the other individual are as fellows "<<endl;
	for(int i=0;i<pop.popSize;i++)
	{
//		if(pop!=0)

			outfile<<*(pop.getIndividualAt(i))<<"\t"<<Population::genid<<endl;
//		else
//			cout<<"the population is empty when outputing to file"<<endl;
	
	}

//	outfile.close();

	return outfile;


}

Population::~Population()
{
	int i=0;
	if(guys!=NULL)
	{
		for(i=0;i<popSize;i++)
		{
			delete guys[i];
			guys[i]=NULL;
		}

		delete [] guys;
		guys=NULL;
	}
	
	if(newguys!=NULL)
	{
		for(i=0;i<popSize;i++)
		{
			delete newguys[i];
			newguys[i]=NULL;
		}

		delete [] newguys;
		newguys=NULL;
	}
	if(bestIndividual!=NULL)

	{
		delete bestIndividual;
		bestIndividual=NULL;
	}

	if(pool!=NULL)
	{
		for(i=0;i<poolLen;i++)
		{
			delete pool[i];
			pool[i]=NULL;
		}
		delete [] pool;
		pool=NULL;
	}


	if(maxFitness!=NULL)
	{
		delete maxFitness;
		maxFitness=NULL;
	}

	if(minFitness!=NULL)
	{
		delete minFitness;
		minFitness=NULL;
	}

	if(avgFitness!=NULL)
	{
		delete avgFitness;
		avgFitness=NULL;
	}

	if(varFitness!=NULL)
	{
		delete varFitness;
		varFitness=NULL;
	}


	if(selection!=NULL)
	{
		delete selection;
		selection=NULL;

	}

	if(crossover!=NULL)
	{
		delete crossover;
		crossover=NULL;
	}

	if(mutation!=NULL)
	{
		delete mutation;
		mutation =NULL;
	}

	if(solutionselection!=NULL)
	{
		delete solutionselection;
		solutionselection=NULL;
	}

	if(localsearch!=NULL)
	{
		delete localsearch;
		localsearch=NULL;
	}
	

}


void Population::computeFitnessStatistics()
{
	double oneOfThePop=1.0/(1.0*myglobalSetup->populationSize);

	int popSize=myglobalSetup->populationSize;
	int i=0,bestIndividualId,worstIndividualId;

	double oldMaxFit,oldAvgFit,oldVarFit;


	oldMaxFit=(*maxFitness);
	oldAvgFit=(*avgFitness);
	oldVarFit=(*avgFitness);



	//maxFitness=0.0;
//	minFitness=1000000.0;

	double sumFitness=0.0;
	for(i=0;i<popSize;i++)
	{
		if(*maxFitness<guys[i]->getFitness())
		{
			*maxFitness=guys[i]->getFitness();
			if(bestIndividual!=NULL)
			{
				delete bestIndividual ;
				bestIndividual=NULL;
			}
			bestIndividual=guys[i]->clone();

			bestIndividualId=i;
		}
		if(*minFitness>guys[i]->getFitness())
			*minFitness=guys[i]->getFitness();
		sumFitness+=guys[i]->getFitness();
	}
	*avgFitness=sumFitness*oneOfThePop;

	*varFitness=oldAvgFit-(*avgFitness);

	
}



int Population::doEvaluate(int type) //type 是表示后面选择的方式
{

	
	int stop=0;
//在评估之前，先把guys中的个体，拷贝到pool中
	for(int i=0;i<popSize;i++)
	{
		//cout<<"the guys "<<i<< " is "<<*guys[i]<<endl;
		pool[poolPtr]=guys[i]->clone();
		//cout<<"after cloning,the pool "<<poolPtr<<" is "<<*pool[poolPtr]<<endl;
		poolPtr++;

	}


	if(popType==2)  //是对一个搞成算子串的评估
	{	//OperatorIndividual::countP=0;
		for(int i=0;i<poolPtr;i++)
		{
			for(int j=0;j<20;j++)
			{
		//		cout<<"this was "<<*pool[i]<<endl;
				pool[i]->evaluateFitness();
				OperatorIndividual::countP++;
			
				OperatorIndividual::countP=OperatorIndividual::countP%20;
			}
	//		OperatorIndividual::countP=(OperatorIndividual::countP++)%9;
		//	cout<<"the pool for higher leval (LLH )    after opeartor  "<<endl;
		//	outPutPoolStatus();
		}
		stop=doSelect(type);
	}
	else if(popType==1)
	{
//		cout<<"the pool for lowleval after opeartor  "<<endl;
		//cout<<" for tsp solution ,after mutation ,the guys and pools are like:"<<endl;
		//outPutPoolStatus();
		for(int i=0;i<poolPtr;i++)
			pool[i]->evaluateFitness();

		doSelect(type);
	}
	else
	{
		cout<<"error in doEvaluate ,poptype error"<<endl;
		return 0;
	}

	return 1;  //返回1，表示成功
	//本来是为了扩展停止条件用的，返回具体的评估次数，现在先不写。
}

int Population::doLocalSearch(int type)
{
	localsearch=new LocalSearch(this,type);
	localsearch->makingLocalSearch(type);
	return 1;  //这个应该返回一个具体的数，表示评估的次数。
}


void Population::doMutate(int type)
{	
	if(popType==2)//高层的GA，用Mutation类进行突变
	{
		mutation=new Mutation(this);
		mutation->makingMutation(type);
	}
	else if(popType==1)
	{
		mutation =new ProblemMutation(this);
		for(int i=0;i<popSize;i++)
		{
			mutation->setParent(guys[i]);
		//	cout<<"the guys[i] is "<<endl;
		//	cout<<*guys[i]<<endl;
			mutation->makingMutation(type);
		}
	}
	else
		cout<<"newing mutation ,poptype error"<<endl;
	//outPutPoolStatus();
}

// 说明，根据传来的参数，生成对应的crossver类。
// 1 2 3 表示单亲杂交
// 4---10 表示双亲杂交
// 11 12 表示3亲杂交
// 其他均视为单亲杂交。
// 具体的每个算子对应的数字应该有变化，这个等程序基本稳定以后再改
void Population::doCrossOver(int type)
{
	int pnum=0;
	int p2,p3;
	if(popType==1)
	{
		if(type==4|| type==5)
		{
			crossover=new ProblemOneParentCrossOver(this,type);
			pnum=1;
		}
		else if(type>=6&& type <=12)
		{
			crossover=new ProblemTwoParentCrossOver(this,type);
			pnum=2;
		}

		else if( type==13)
		{
			crossover=new ProblemMultiParentCrossOver(this,type);
			pnum=3;
		}
		else
			crossover=new ProblemOneParentCrossOver(this,type);
		for(int i=0;i<popSize;i++)
		{
			p2=0;
			p3=0;
			switch (pnum)
			{
				case 1:
					crossover->setParent(guys[i]);
					break;
				case 2:
					while(p2==i)
						p2=myRandom->boundedIntegerRandom(0,popSize);
					crossover->setParent1(guys[i]);
					crossover->setParent2(guys[p2]);
					break;
				case 3:
					while(p2==i)
						p2=myRandom->boundedIntegerRandom(0,popSize);
					while(p3==i)
						p3=myRandom->boundedIntegerRandom(0,popSize);
					crossover->setParent1(guys[i]);
					crossover->setParent1(guys[p2]);
					crossover->setParent1(guys[p3]);
					break;
				default:
					cout<<"error in do crossover"<<endl;
		
			}
		crossover->makingCrossover(type);

		}
	}
        else if(popType==2)
	{
		if(type==1||type==2)
		{
			crossover=new LLHOneParentCrossOver(this,type);
			pnum=1;
		}
		else if(type==6||type==4|| type==5)
		{
			crossover=new LLHTwoParentCrossOver(this,type);
			pnum=2;
		}
		else if(type==7 )
		{
			crossover=new LLHMultiParentCrossOver(this,type);
			pnum=3;
		}
		else
		{
			crossover=new LLHOneParentCrossOver(this,type);
			pnum =1;
		}

		for(int i=0;i<popSize;i++)
		{
			p2=0;
			p3=0;
			switch (pnum)
			{
				case 1:
					crossover->setParent(guys[i]);
					break;
				case 2:
					while(p2==i)
						p2=myRandom->boundedIntegerRandom(0,popSize);
					crossover->setParent1(guys[i]);
					crossover->setParent2(guys[p2]);
					break;
				case 3:
					while(p2==i)
						p2=myRandom->boundedIntegerRandom(0,popSize);
					while(p3==i)
						p3=myRandom->boundedIntegerRandom(0,popSize);
					crossover->setParent1(guys[i]);
					crossover->setParent1(guys[p2]);
					crossover->setParent1(guys[p3]);
					break;
				default:
					cout<<"error in do crossover"<<endl;
		
			}
		crossover->makingCrossover(type);

	
		}
	}
	else
		cerr<<"newing crossover ,poptype error"<<endl;
}
void Population::computeObjStatistics()
{}

void Population::mapObjectiveToFitness()
{}

void Population::replacePopulation()
{}
int  Population::doSelect(int type)
{
	int result;
	if(popType==2) //高层
	{
		selection=new Selection(this);
		result =selection->makingSelection(type);
	}
	else if(popType==1)  //低层GA
	{
		solutionselection=new SolutionSelection(this);
		result=solutionselection->makingSelection(type);
	}
	else
	{
		cout<<"error in doSelect"<<endl;
		result=0;
	}
	return result;
}

int  Population::addToPool(Individual * ind)
{
	if(ind !=NULL)
	{
	//	cout<<"the waited to added ind is "<<ind <<" its value is "<<*ind;
		pool [poolPtr++]= ind->clone();
	//	cout<<"the pool ["<<poolPtr<<"] is address"<< pool[poolPtr]<<" its value is"<<*pool[poolPtr]<<endl;
	//	cout<<"actutually in the pool is ###############################################"<<endl;
	//	for(int i=0;i<10;i++)
	//		cout<<*pool[poolPtr]->chromosome->genes[i]<<"  ";
	//	cout<<endl;

	//	cout<<"the ind de address is "<<ind<<" is going to be delted"<<endl;
	//	delete ind;

	}
	else
	{
		cout<<"error ,adding a empty to the pool."<<endl;
		return 0;
	}
//	poolPtr++;
	if(poolPtr>=poolLen)
	{
		cout<<" the pool is full"<<endl;
		return 0;
	}
	return  1;
}

void Population:: outPutPoolStatus()
{
	//cout<<"the pool status **********************************"<<endl;
	if(poolPtr>0)
		cout<<"there are "<<poolPtr<<" individuals in the pool,"<<"the pool size is "<<poolLen<<", and they are as fellows :"<<endl;
	//cout<<"the individual in the pool are as fellows:"<<endl;
	if(pool!=NULL && poolPtr>0)
		{
			for(int i=0;i<poolPtr;i++)
				cout<<*pool[i]<<endl;
		}
	else
		cout<<"empty in the pool"<<endl;
}
void   Population:: outPutPoolStatus(ostream & out)
{
	//cout<<"the pool status **********************************"<<endl;
	if(poolPtr>0)
	if(pool!=NULL && poolPtr>0)
		{
			for(int i=0;i<poolPtr;i++)
				//out<<*(pool[i]->chromosome)<<"\t"<<pool[i]->fitness<<endl;
				out<<pool[i]->fitness<<endl;
		}
}
void Population:: setIndividualAt(int i,Individual * p)
{
	Individual * pind=getIndividualAt(i);
	if(pind!=NULL)
		delete pind;
	pind=p->clone();
}
//ostream & operator << (ostream & o,const Population & p)
//
//{
//
//}
int Population:: copyGuysToPool()
{
	int i=0;
	for(;i<popSize;i++)
	{
		addToPool(guys[i]);
	}
	return 1;
}

int partition (Individual ** ind,int left,int right)
{
	int low,high;
	Individual * i,* pivot;
	pivot =ind[left];
	low=left-1;
	high=right+1;
	while(low+1!=high)
	{
		if(ind[low+1]->getFitness()<=pivot->getFitness())
			low++;
		else if(ind[high-1]->getFitness()>pivot->getFitness())
			high--;
		else
		{
			i=ind[low+1];
			ind[++low]=ind[high-1];
			ind[--high]=i;
		}
	}
	ind[left]=ind[low];
	ind[low]=pivot;
	return low;
}
void Population::rankingQuickSort(Individual ** i,int left,int right)
{
	int dp ;
	if(left<right)
		{
			dp=partition(i,left,right);
			rankingQuickSort(i,left,dp-1);
			rankingQuickSort(i,dp+1,right);
		}

}

int Population::clearPool()
{
	if(pool!=NULL && poolPtr>0)
	{
		for(int i=0;i<poolPtr;i++)
		{
			delete pool[i];
			pool[i]=NULL;
		}
		poolPtr=0;
	}
}

int Population::printTspSolutions(ostream & out)   /// for the guys;
{
	for(int i=0;i<popSize;i++)
	{
		out<<"llH "<<i<<"\t";
		guys[i]->printTspSolu(out);
		out<<endl;
	}
	return 1;
}
