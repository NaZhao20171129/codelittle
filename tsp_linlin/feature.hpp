#ifndef FEATURE_H
#define FEATURE_H

#include<iostream>
#include<math.h>
#include"problem.hpp"
#include<vector>

#include<map>

using namespace std;

class Problem;
class Point;
class Feature
{
	public:

//////////////////第一部分特征，11个//////////////////
		int numOfCity;
		int numOfEdge;
		double avgLengthOfEdge;
		double medianEdgeLength;
		double minLength;
		double maxLength;
		double deviation;//边长标准差
		double minSolution;

		//modes
		int eqmode;//边长的值出现次数最多的个数,大于8次就算出现次数较多
		int efmode;//出现次数最多的边长的出现的辨率
		double emode;//出现次数最多的边长的值
	



/////////////////////////第二部分特征,8个//////////////////////////////
		Point *centerPoint;

		Problem *pro; //保存一个指向对应问题的指针


	public:
		Feature(){}
		Feature(Problem * p){pro =p;}
		~Feature(){}
	/*	Feature (const Feature & f)
		{
			numOfNode =f.getNodeNum();
			numOfEdge=f.getEdgeNum();
			avgLengthOfEdge=f.getAvgLen();
		};
	
		int getNodeNum() const {return numOfNode;}
		int getEdgeNum()const {return numOfEdge;}
		double getAvgLen()const {return avgLengthOfEdge;}
*/


		void printFeature(ostream &out);
		double compateFeature(Feature * f);
		int computeFeatures();
		int kmeans(int np);
		int computeModes(vector<struct edgeWithId> & a);



		//display feature to file
		int outputFeatures();
};
#endif
