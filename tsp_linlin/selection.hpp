#ifndef SELECTION_H
#define SELECTION_H

#include<iostream>
#include<string>
#include<math.h>
#include"individual.hpp"
#include"globalSetup.hpp"
#include"population.hpp"

class Population;
extern Random * myrandom;
extern GlobalSetup * myglobalSetup;

class Selection
{
	public:
		Selection(Population *p);
		virtual ~Selection();

		int tournamentSelection();
		int rankingSelection();
		int raceSelection(){}
	
		int makingSelection(int type);

		int rankingAndComputing();
	private:
		Population *pop;
};


class SolutionSelection
{
	public:
		SolutionSelection(Population * p);
		~SolutionSelection();
		int tournamentSelection();
		int rankingSelection();
		int ranking();
		int makingSelection(int type);
	private:
		Population * pop;
};
#endif
