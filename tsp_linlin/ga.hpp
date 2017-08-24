
#ifndef _GA_H
#define _GA_H

#include <iostream>
#include <fstream>
#include<string>

#include "globalSetup.hpp"
#include "population.hpp"
#include "random.hpp"

#define GAP 1 //表示在用算子串对种群演化时候，几个算子以后，进行一次选择操作。
class Population;
extern GlobalSetup *globalSetup;
extern Random * myRandom;
extern GlobalSetupForLow * lowSetup;

class GA 
{
public:
  int genID;
  long noOfGlobalEvals;
  long noOfLocalEvals;
  int popType; //表示种群的种类，为了后面便于评估每个个体

  Problem * problemInGA;
  static int EvalIndex;

  bool stoppingCriteria(void);
  int successiveNoChangeInBestFitness;
  int successiveNoChangeInBestObjective;
  int successiveNoChangeInNoOfFronts;
  int successiveNoChangeInAvgObjective;
  int successiveNoChangeInAvgFitness;
  int successiveNoChangeInFitnessVar;
 public:
  Population *population;
  GA(int geneL); //构造高层的
  GA(Problem * p);// 构造低层的，针对某个具体的问题
  ~GA();

  bool evolute();//高层ga，用于演化算子串，用固定的opeartor进行演化，默认算子都是1.

  //低层ga，用特定的llh对某个问题进行求解
   bool evolute(Individual * operatorStrings );

  int setType(int t){popType=t;}
  void reinitialize(int );
//  bool generate();
// Individual* getBestGuy() {return population->getBestIndividual();}
 // void freeze(int locus) {population->freeze(locus);}
 // void flood(int locus) {population->flood(locus);}
  //void gaOutput(std::ostream &out) {out << *population;}

  Population * getPopulation() {return population;}

  inline int getGenID () const {return genID;}
  inline long getNoOfGlobalEvals(void) const {return noOfGlobalEvals;}
  inline long getNoOfLocalEvals(void) const {return noOfLocalEvals;}
  inline int getSuccessiveNoChangeInBestFitness(void) const {return successiveNoChangeInBestFitness;}
  inline int getSuccessiveNoChangeInBestObjective(void) const {return successiveNoChangeInBestObjective;}
  inline int getSuccessiveNoChangeInNoOfFronts(void) const {return successiveNoChangeInNoOfFronts;}
  inline int getSuccessiveNoChangeInAvgObjective(void) const {return successiveNoChangeInAvgObjective;}
  inline int getSuccessiveNoChangeInAvgFitness(void) const {return successiveNoChangeInAvgFitness;}
  inline int getSuccessiveNoChangeInFitnessVar(void) const {return successiveNoChangeInFitnessVar;}

  inline void setGenID (int iValue) {genID = iValue;}
  inline void setNoOfGlobalEvals(long lValue) {noOfGlobalEvals = lValue;}
  inline void setNoOfLocalEvals(long lValue) {noOfLocalEvals = lValue;}
  inline void setSuccessiveNoChangeInBestFitness(int iValue) {successiveNoChangeInBestFitness = iValue;}
  inline void setSuccessiveNoChangeInBestObjective(int iValue) {successiveNoChangeInBestObjective = iValue;}
  inline void setSuccessiveNoChangeInNoOfFronts(int iValue) {successiveNoChangeInNoOfFronts = iValue;}
  inline void setSuccessiveNoChangeInAvgObjective(int iValue) {successiveNoChangeInAvgObjective = iValue;}
  inline void setSuccessiveNoChangeInAvgFitness(int iValue) {successiveNoChangeInAvgFitness = iValue;}
  inline void setSuccessiveNoChangeInFitnessVar(int iValue) {successiveNoChangeInFitnessVar = iValue;}
};

#endif
