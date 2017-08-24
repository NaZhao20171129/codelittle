#ifndef CHROMOSOME_H
#define CHROMOSOME_H
#include<iostream>
#include<string>
#include"random.hpp"
#include"globalSetup.hpp"
using namespace std;

extern GlobalSetup * myglobalSetup;
extern Random  *myRandom;
class Chromosome
{
	public:
		int ** genes;
		int geneLen;
	public:
		Chromosome(int cityNum);
		Chromosome();
		Chromosome(const Chromosome& chrom);
		~Chromosome();

		 Chromosome (Chromosome * c);

		 void initializeChromo();

	inline void  SetValue(const int loc,const double   val)

	{
		*genes[loc]=val;
	}

	inline void  SetValue(const int loc,const int   val)

	{
		*genes[loc]=val;
	}
	inline int * operator [] (const int & loc)
	{
		return genes[loc];
	}

	Chromosome & operator =(const Chromosome & c);
	friend ostream & operator << (ostream & o,const Chromosome &c );
	void displayChromosome();
	int copyChromosome(const Chromosome & c);
	int isEmptyGene()
	{
		if(genes==NULL)
			return 1;
		else
			return 0;
	}
};
#endif
