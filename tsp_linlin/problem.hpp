#ifndef PROBLEM_H
#define PROBLEM_H
#include<iostream>
#include<string>
#include<math.h>
#include"feature.hpp"
#include"random.hpp"

#include"individual.hpp"

using namespace std;

class Individual;
extern Random * myRandom;
extern GlobalSetupForLow * lowSetup;
class Feature;
class Point
{
	public:
		 int id;
		double x,y;

		int clusterId;
	public:
		Point(){x=0.0;y=0.0;id=-1;}
		Point(double  xx,double yy):x(xx),y(yy){}
		
		Point (const Point & p):x(p.getx()),y(p.gety()),id(p.getid()){}
		

		~Point(){}	
		double getx() const {return x;}
		double gety() const {return y;}
		int getid()const {return id;}


		int setPoint(double xx,double  yy,int  idd)
		{
			id=idd;
			x=xx;
			y=yy;
			return 0;
		}


};


class Problem 
{
	public:
		int numOfCity;
		string proName;
		double ** distance;
		Point *point;
		Feature *feature;


		Individual ** startSolutions;
		int calculateFeature();
		int calculateDistance();
		double computeTwoPointDistance(Point a,Point b); 
		void initIndividualForPro();
	public:
		Problem();
		Problem(int n);
		
		~Problem();
		//从文件读取n个点的坐标，返回0表示成功，其他表示失败。
		int readProblemFromFile(string fileName);

		Feature * getFeature();
		double getDistanceOfTwoCity(int a,int b);
	
};
#endif
