/***************************************************************
/* Single & Multi-Objective Real-Coded Genetic Algorithms Code */
/* Author: Kumara Sastry                                       */
/* Illinois Genetic Algorithms Laboratory (IlliGAL)            */
/* Deparment of General Engineering                            */
/* University of Illinois at Urbana-Champaign                  */
/* 104 S. Mathews Ave, Urbana, IL 61801                        */
/***************************************************************/

#include "ga.hpp"
#include<sstream>
extern int  problemCounter;
GA::GA(int gl)
{
	  genID = 0;
//	  noOfGlobalEvals = (long)(myglobalSetup->populationSize);
  	  noOfLocalEvals = 0;
	  noOfGlobalEvals=0;
   	  population = new Population(gl);
	  successiveNoChangeInBestFitness=0;
	  successiveNoChangeInBestObjective=0;
	  successiveNoChangeInNoOfFronts=0;
	  successiveNoChangeInAvgObjective=0;
	  successiveNoChangeInAvgFitness=0;
	  successiveNoChangeInFitnessVar=0;
}
int  GA::EvalIndex=0;
GA::GA(Problem *p)
{
	  genID = 0;
//	  noOfGlobalEvals = (long)(myglobalSetup->populationSize);
  	  noOfLocalEvals = 0;
	  noOfGlobalEvals=0;
   	  population = new Population(p);
	  problemInGA=p;

}

GA::~GA(void)
{
	if(population!=NULL)
  	{
		delete population;
		population=NULL;
	}
}

void GA::reinitialize(int gl)  //默认为高层而写
{
  delete population;
  genID = 0;
  popType=2;
//  noOfGlobalEvals = (long)(myglobalSetup->populationSize);
  noOfGlobalEvals=0;
  noOfLocalEvals = 0;
  population = new Population(gl);
}

/*============================================================
** Function Name: GA::nsgaGenerate()                         
** Function Task: Has the main NSGA loop in which it does the following computation:
**   1. Perform the Non-Dominated Sort on Initial generation and assign ranks to respective individuals
**   2. Compute the Crowding Distance of each individual      
**   3. Compute this for a specified number of generations    
**      4. Select individuals using a user specified selection procedure and instead of fitness use the ranks and crowding distance
**      5. Perform Crossover and Mutation to create new individuals
**      6. Evaluate the new individuals
**      7. Combine the parents and the new individuals into one population
**      8. Perform Non-Dominated Sort on the combined population and assign ranks and generate Pareto fronts.
**      9. Compute Crowding Distance of each individual in the combined population
**     10. Select higher ranked individuals to create the new population
**     11. If the number of individuals in the new population exceeds population size, transfer only those individuals who have higher crowding distance.
** Input Parameters: None
** Output : None
** Functions Invoked : NsgaPopulation::doNonDominatedSort(int)
**                     NsgaPopulation::computeCrowdingDistance(int)
**                     NsgaPopulation::doSelect()
**                     NsgaPopulation::doCrossover()
**                     NsgaPopulation::doMutate()
**                     NsgaPopulation::doEvaluate()
**                     NsgaPopulation::computeStatistics()
**                     NsgaPopulation::regQSort(double*, int*, int, int)
**============================================================*/
/*bool GA::nsgaGenerate()
{
  double *crowdDist;
  int ii, np = 2*(globalSetup->populationSize), fid = 0;
  int numNextGen = 0, indID, *index, *sortedIndex;
  int numEvals;

  //
  // Output the population to the history file -- Begin
  //
  //
  // Output the population to the history file -- End
  //

  NsgaPopulation *pop = (NsgaPopulation *)population;

  std::cout << "% Generation: " << genID << std::endl;
  std::cout << (*population) << std::endl;
 
  // While the generation counter is less than maximum number of generations
  if(!(stoppingCriteria())) {
    pop->doSelect();
    //Selection returns the index of selected individuals. Use this index to copy the selected individuals to create new pop
    for(ii = 0; ii < globalSetup->populationSize; ii++) 
      *((NsgaIndividual *)(pop->newGuys[ii])) = *(pop->guys[pop->mpool[ii]]);
    pop->doCrossover();
    pop->doMutate();
    numEvals = pop->doEvaluate();
    noOfGlobalEvals += (long)numEvals;
    pop->computeObjStatistics(NEWGUYS);
    pop->mapObjectiveToFitness(NEWGUYS);
    // Combine the current population and new population into one population of size 2n
    for(ii = 0; ii < globalSetup->populationSize; ii++) {
      *(pop->combinedGuys[ii]) = *(pop->guys[ii]);
    }
    for(ii = 0; ii < globalSetup->populationSize; ii++)
      *(pop->combinedGuys[ii+(globalSetup->populationSize)]) = *(pop->newGuys[ii]);
    
    pop->computeFitnessStatistics(COMBINEDGUYS);
    pop->doNonDominatedSort(COMBINEDGUYS);
    pop->computeCrowdingDistance(COMBINEDGUYS);
    
    //Start with indiviudals with rank 0. While the number of individuals in the next generation plus the number of individuals in the current front is less than the population size then copy all the individuals in the current front and increase the front counter.
    numNextGen = fid = 0;
    while(numNextGen + (pop->numIndsFront[fid]) <= globalSetup->populationSize) {
      for(ii = 0; ii < pop->numIndsFront[fid]; ii++) {
	indID = pop->paretoFront[fid][ii];
	*((NsgaIndividual *)(pop->guys[numNextGen+ii])) = *(pop->combinedGuys[indID]);
      }
      numNextGen += pop->numIndsFront[fid++];
    }
    //    cout << fid << " " << numNextGen << endl;
    
    //If the number of individuals in the next generation plus the number of individuals in the current front is greater than the population size then copy the crowding distances of all the individuals in the current front to an array CrowdDist and also store the index of individuals in Index.
    if(numNextGen < globalSetup->populationSize) {
      crowdDist = new double[pop->numIndsFront[fid]];
      index = new int[pop->numIndsFront[fid]];
      sortedIndex = new int[pop->numIndsFront[fid]];
      
      for(ii = 0; ii < pop->numIndsFront[fid]; ii++) {
	indID = pop->paretoFront[fid][ii];
	crowdDist[ii] = pop->getCrowdingDistance(indID);
	index[ii] = indID;
	sortedIndex[ii] = ii;
      }
      
    //Sort the individuals in Index according to ascending order of Crowding Distance
      pop->regQSort(crowdDist, sortedIndex, 0, pop->numIndsFront[fid]);

    // Copy the guys with higher crowding distance into the new population
      for(ii = numNextGen; ii < globalSetup->populationSize; ii++) {
	indID = index[sortedIndex[pop->numIndsFront[fid] + numNextGen - ii - 1]];
	*((NsgaIndividual *)(pop->guys[ii])) = (*(pop->combinedGuys[indID]));
      }
      delete []index;
      delete []crowdDist;
      delete []sortedIndex;
    }
    ++genID;
	return 1;
  }
  else return 0;
}
*/

/*============================================================
** Function Name: GA:generate()                         
** Function Task: This method has the main GA loop which performs the following
** While the number of generations maxGenerations we do
**   Selection
**   CrossOver
**   Mutation
**   Evaluate
**   Niching (RTS, Sharing)
**   Local search
**   Objective function statistics generation
**   Objective mapped to fitness
**   Fitness Statistics generation
**   Fitness Scaling if required
**   Population Replacement
**   Fitness Statistics generation
** Output : None
** Functions Invoked : Population::doSelect()
**                     Population::doCrossover()
**                     Population::doMutate()
**                     Population::doEvaluate()
**                     Population::doRTS()
**                     Population::doLocalSearch()
**                     Population::computeObjStatistics()
**                     Population::mapObjectiveToFitness()
**                     Population::computeFitnessStatistics()
**                     Population::scaleFitness();
**                     Population::shareFitness()
**                     Population::replacePopulation()
**============================================================*/

bool GA::evolute()
{
  int ii, numEvals;

  // Output the population to the history file -- Begin
  

   ofstream out;
   out.open("populations.xls");
   ofstream fllhout;
   fllhout.open("llhQuatlites.xls");
   ofstream outsolu;
   outsolu.open("solutionQualities.xls");

 while (!(stoppingCriteria()))
  {

//	out<<"*****************High Level GA Generations : "<<genID<<"****************************"<<std::endl;
	out<<(*population);
//	for(i=0;i<popSize;i++)
//	{
	//	cout<<"before anyting ,the pool is:"<<endl;
	//	population->outPutPoolStatus();
		cout<<"*****************High Level GA Generations : "<<genID<<"************************************************"<<std::endl;
		population->doCrossOver(5);
		population->doMutate(1); //表示用第一种突变算子
	//	population->outPutPoolStatus();
//		cout<<"after mutation in ga"<<endl;

		population->doEvaluate(1);  //表示用第一种选择算子


		fllhout<<genID<<"###############################################"<<endl;
		population->outPutPoolStatus(fllhout);
		population->clearPool();
		population->printTspSolutions(outsolu);
//}
	genID++;
	Population::genid++;
  }
 out.close();
 fllhout.close();

// 	problemCounter++;

 	 return 1;

}

/*    // Selection
    if (globalSetup->nichingType != DeterministicCrowding)
    {
      population->doSelect();
      for(ii = 0; ii < globalSetup->populationSize; ii++) 
	*(population->newGuys[ii]) = *(population->guys[population->mpool[ii]]);
    }
    // Crossover
    population->doCrossover(UniformCrossover);

    
    // Mutation
    population->doMutate(Selective);
    
    // Evaluate
 //   numEvals = population->doEvaluate();     
 	population->doEvaluate();
        noOfGlobalEvals += (long)numEvals;
    
    // Niching (RTS)
  //  if(globalSetup->nichingType == RTS) population->doRTS();
    
    // Local search
    if(globalSetup->localSearchMethod != NoLocalSearch)
    {
      numEvals = population->doLocalSearch();
      noOfLocalEvals += (long)numEvals;
    }
    
    // Objective function statistics generation
    population->computeObjStatistics();
    
    // Map objective function to fitness function
    population->mapObjectiveToFitness();
    
    // Fitness Statistics Generation
    population->computeFitnessStatistics();
    if((globalSetup->selectionType == SUS)||
       (globalSetup->selectionType == RouletteWheel)||
       (globalSetup->nichingType == Sharing))
    {
      
      //Scale Fitness
      population->scaleFitness();
      
      // If Niching(sharing) share the fitness values
      if(globalSetup->nichingType == Sharing) population->shareFitness();
      
      // Compute Fitness Statistics because sharing changes the fitness of the individuals
      population->computeFitnessStatistics();
    }
    
    // Replace the Population
    population->replacePopulation();
    ++genID;
 
  }  //end for while
*/
//}

/// Returns 1 if any one of the selected set of stopping criterias are met.
bool GA::stoppingCriteria(void) 
{
  bool stopFlag = 0;
  int ii,jj;
  if(popType==2)
  {
	  if(genID >= myglobalSetup->maxGeneration)
  	{
    		stopFlag = 1;
   		// return stopFlag;
  	}
  }
  else
  {
	  if(genID>=lowSetup->maxGen )
	  {
		  stopFlag=1;
	  }
  }
  return stopFlag;

  /*for(ii = 0; ii < globalSetup->noOfStoppingCriterias; ii++)
  {
    switch(globalSetup->otherStoppingCriteria[ii]) 
    {
    case NoOfEvaluations:

      if(noOfGlobalEvals + noOfLocalEvals >= 
	 globalSetup->stoppingParameter[ii])
	stopFlag = 1;
      break;

    case AverageFitness:
      if(population->getAvgFit() >= globalSetup->stoppingParameter[ii])
	stopFlag = 1;
      break;

    case AverageObjective:
      if(population->getAvgObj() >= globalSetup->stoppingParameter[ii])
	stopFlag = 1;
      break;

    case BestFitness:
      if((*(globalSetup->typeOfOptimizations) == Minimization) && 
	 (population->getMaxObj() <= globalSetup->stoppingParameter[ii]))
	stopFlag = 1;

      else if((*(globalSetup->typeOfOptimizations) == Maximization) && 
	      (population->getMaxObj() >= globalSetup->stoppingParameter[ii]))
	stopFlag = 1;
      break;


    case ChangeInBestFitness:
      if(genID > globalSetup->genNumWindow) 
	if(successiveNoChangeInBestFitness == globalSetup->genNumWindow) stopFlag = 1;
      if(population->getBestFitChange() <= globalSetup->stoppingParameter[ii])
	++successiveNoChangeInBestFitness;
      else successiveNoChangeInBestFitness = 0;
      break;


    case ChangeInAvgFitness:
      if(genID > globalSetup->genNumWindow)
      {
        if(successiveNoChangeInAvgFitness == globalSetup->genNumWindow) stopFlag = 1;
        if(population->getAvgFitChange() <= globalSetup->stoppingParameter[ii])
	  ++successiveNoChangeInAvgFitness;
        else successiveNoChangeInAvgFitness = 0;
      }
      break;

    case ChangeInFitnessVar:
       if(genID > globalSetup->genNumWindow)
       {
         if(successiveNoChangeInFitnessVar == globalSetup->genNumWindow) stopFlag = 1;
         if(population->getFitVarChange() <= globalSetup->stoppingParameter[ii]) 
	    ++successiveNoChangeInFitnessVar;
         else successiveNoChangeInFitnessVar = 0;
       }
      break;
    case ChangeInBestObjective:
      if(genID > globalSetup->genNumWindow){
         if(successiveNoChangeInBestObjective == globalSetup->genNumWindow) stopFlag = 1;
         if(population->getBestObjChange() <= globalSetup->stoppingParameter[ii])
           ++successiveNoChangeInBestObjective;
         else successiveNoChangeInBestObjective = 0;
      }
      break;
    case ChangeInAvgObjective:
      if(genID > globalSetup->genNumWindow){
         if(successiveNoChangeInBestObjective == globalSetup->genNumWindow) stopFlag = 1;
         if(population->getAvgObjChange() <= globalSetup->stoppingParameter[ii])
	  ++successiveNoChangeInAvgObjective;
         else successiveNoChangeInAvgObjective = 0;
      }
      break;
    case NoOfFronts:
      if(((NsgaPopulation*)population)->getNoOfFronts() <= globalSetup->stoppingParameter[ii])
	stopFlag = 1;
      break;
    case NoOfGuysInFirstFront:
      if(((NsgaPopulation*)population)->getNoOfIndsBestFront() <= globalSetup->stoppingParameter[ii])
	stopFlag = 1;
      break;
    case ChangeInNoOfFronts:
       if(genID > globalSetup->genNumWindow){
         if(successiveNoChangeInBestObjective == globalSetup->genNumWindow) stopFlag = 1;
         if(((NsgaPopulation*)population)->getNoOfFrontChange() <= globalSetup->stoppingParameter[ii])
	  ++successiveNoChangeInNoOfFronts;
         else successiveNoChangeInNoOfFronts = 0;


      }
      break;
    default: exit(0);
    }
    if(stopFlag == 1) return stopFlag;
  }*/
 // return stopFlag;
}

bool GA::evolute(Individual * operatorStrings )
{
	int ii,noOfEvals;
	cout<<problemInGA->proName<<endl;
	ostringstream ossFileName;
	ossFileName<<problemInGA->proName<<"_tspop_"<<GA::EvalIndex<<".txt";
	ofstream outpop;
	string fileWrite=ossFileName.str();
	outpop.open(fileWrite.c_str());

	if(!outpop)
	{
		cout<<"cannot open file  tsppop.txt"<<endl;
		exit(0);
	}

	int count=0;
	Population::genid=0;
	while(!stoppingCriteria())
	{
		
	//	outpop<<"########   LowLeval GA  Generations : "<<genID<<"####"<<std::endl;
		outpop<<(*population);
		int tmpOper=*(operatorStrings->getIndividualGeneAt(count%(lowSetup->getOperatorLen())));//取出一个算子，若是大于num，则取余回滚

		count++;
		if(tmpOper>0&&tmpOper<4)   //为了测试这个功能，所以写这个条件，以后得改条件
			population->doMutate(tmpOper);
		else if(tmpOper>=4&&tmpOper<=13)
			population->doCrossOver(tmpOper);
		else if(tmpOper==14||tmpOper==15)
			population->doLocalSearch(tmpOper);
		population->doEvaluate(1); // 1表示用第一种算择算法进行选择


		population->computeObjStatistics();
		population->mapObjectiveToFitness();

		population->computeFitnessStatistics();

		++genID;
		Population::genid++;
		
		
		//count++;
		//if(count%GAP==0)
		//	population->doSelect(lowSetup->getSelectionType());

		population->clearPool();

	}
	
	operatorStrings->setFitness(population->getBestIndividual()->getFitness());
}
