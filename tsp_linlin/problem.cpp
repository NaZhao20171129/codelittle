#include"problem.hpp"
Problem::Problem()
{
	numOfCity=0;
	distance=0;
	feature=NULL;
	point=NULL;
	startSolutions=NULL;
}

Problem::Problem(int n)
{
	numOfCity=n;
	point=new Point[n];
	for(int i=0;i<n;i++)
	{
		double x=myRandom->boundedRandom(MINDOUBLE,MAXDOUBLE );
		double y=myRandom->boundedRandom(MINDOUBLE,MAXDOUBLE );
		point[i].setPoint(x,y,i+1);


	}
	distance=new double* [n];

	for(int i=0;i<n;i++)
		distance[i]=new double [n];

	initIndividualForPro();
	calculateDistance();
	//calculateFeature();
}

int Problem::readProblemFromFile(string filename)
{
	proName=filename;
	ifstream in;
	in.open(filename.c_str());
	if(!in)
	{
		cout<<"cannot open file "<<filename<<" to read the problem"<<endl;
		return 1;
	}

	in>>numOfCity;

	if(numOfCity<=0)
	{
		cout<<"city number less than 1 ,exit!"<<endl;
		return 1;
	}


	point=new Point[numOfCity];
	distance=new double* [numOfCity];

	for(int i=0;i<numOfCity;i++)
		distance[i]=new double [numOfCity];

	double xx,yy;
	for(int i=0;i<numOfCity;i++)
	{
		in>>xx>>yy;
		point[i].setPoint(xx,yy,i+1);
	}
	calculateDistance();

	initIndividualForPro();
	feature=new Feature(this);
//	calculateFeature();

	return 0;


}
Feature * Problem::getFeature()
{
	return feature;
}


int Problem ::calculateDistance()
{
	int i,j;
	for(i=0;i<numOfCity;i++)
		for(j=0;j<numOfCity;j++)
		{
			if(i!=j)
				distance[i][j]=computeTwoPointDistance(point[i],point[j]);
			else
				distance[i][j]=0;
		}
}
//wait to be done
int Problem ::calculateFeature()
{
	feature->computeFeatures();
}

double Problem:: computeTwoPointDistance(Point a,Point b)
{
	double dis,tmp;
	tmp=pow((a.getx()-b.getx()),2)+pow((a.gety()-b.gety()),2);
	dis=sqrt(tmp);
	return dis;
}
double Problem::getDistanceOfTwoCity(int a,int b)
{
	a=a%numOfCity;
	b=b%numOfCity;
	return distance[a][b];
}

Problem ::~Problem()
{
	if(distance !=NULL)
	{
		for(int i=0;i<numOfCity;i++)
			{
				delete distance[i];
				distance[i]=NULL;
			}
		distance =NULL;
	}
	if(point !=NULL)
	{
			delete []  point;
			point =NULL;
	}
	if(feature!=NULL)
	{
		delete feature;
		feature=NULL;
	}
}



void Problem::initIndividualForPro()
{
	int psize=lowSetup->getPopSize();
	if(startSolutions!=NULL)
	{
		for(int i=0;i<psize;i++)
			delete startSolutions[i];
		delete [] startSolutions;
		startSolutions=NULL;
	}
	startSolutions=new Individual* [psize];
	for(int i=0;i<psize;i++)
	{
		startSolutions[i]=new Individual(numOfCity);
		startSolutions[i]->chromosome->initializeChromo();
	
	}

}
