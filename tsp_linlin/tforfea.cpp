#include"feature.hpp"
Random * myRandom;
int main()
{
	Problem *p=new Problem();
	myRandom =new Random();
	p->readProblemFromFile("problem_50_1");

	return 0;
}
