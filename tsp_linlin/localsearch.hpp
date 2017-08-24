#ifndef LOCALSEARCH_HPP
#define LOCALSEARCH_HPP

#include"individual.hpp"
#include<iostream>
#include<string>
#include<math.h>
#include"random.hpp"
#include"globalSetup.hpp"
#include"population.hpp"
#include"ga.hpp"

class Population;

extern Random * myrandom;
extern GlobalSetup * myglobalSetup;
extern int problemCounter;
class LocalSearch
{
	public:
		LocalSearch(Population * p);

		LocalSearch(Population * p,int t);
		~LocalSearch();
		
		int makingLocalSearch(int type);
		int setStart(Individual * st);
	//	int setGeneAtLocation(int loc,int gene);
		int setBest(Individual *b);
	public:
		int type;
		Population *pop;
		Individual * start;
		Individual * changed;
		Individual * best;

		int firstImproveLocalSearch();
		int bestImproveLocalSearch();
		int two_opt();
		int three_opt();
		void release();

};

#endif
