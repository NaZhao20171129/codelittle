#include"feature.hpp"
#include<vector>
#include<map>
#include<algorithm>
#include<stdlib.h>
using namespace std;
class Point;
typedef struct edgeWithId
{
	int start;
	int end;
	double length;
}edgeWithId;


bool operator < (const edgeWithId &e1,const edgeWithId & e2)
{
	return e1.length<e2.length;

}
int Feature::computeFeatures()
{

	if(pro==NULL)
	{
		cout<<"problem is empty in computing features"<<endl;
		exit(0);
	}
	
	numOfCity=pro->numOfCity;

	numOfEdge=numOfCity *(numOfCity-1)/2;

	int count=0;
	double sum=0.0;

	struct edgeWithId tmpewi;
	vector<struct edgeWithId> edges;

	for(int i=0;i<numOfCity;i++)   // 对problem里面的distance的上三角阵进行操作
	{
		for(int j=0;j<i;j++)
		{
			count++;
			sum+=pro->distance[i][j];

			tmpewi.start=i;
			tmpewi.end=j;
			tmpewi.length=pro->distance[i][j];

			edges.push_back(tmpewi);
		}
	}


	

	avgLengthOfEdge=sum/count;

	sort(edges.begin(),edges.end());
//	vector<edgeWithId> ::iterator iter;
	//for(iter=edges.begin();iter!=edges.end();iter++)
	//	cout<<"("<<iter->start<<","<<iter->end<<")---"<<iter->length<<endl;
	medianEdgeLength=edges[int (edges.size()/2)].length;

	minLength=edges.front().length;
	maxLength=edges.back().length;
//compute deviation
   	double sumdev=0.0;
	for(int i=0;i<edges.size();i++)
		sumdev+=pow(edges[i].length-avgLengthOfEdge,2);

	deviation=sqrt(sumdev);

	minSolution=0.0;
	for(int i=0;i<numOfCity;i++)
		minSolution+=edges[i].length;


//compute point center
 	double sumx=0.0,sumy=0.0;
	for(int i=0;i<numOfCity;i++)
	{

		sumx+=pro->point[i].x;
		sumy+=pro->point[i].y;
	}
	centerPoint=new Point();
	centerPoint->x=sumx/numOfCity;
	centerPoint->y=sumy/numOfCity;



//compute modes
	computeModes(edges);
}



double computeDistance(Point & p1,Point & p2)
{
	return pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2);
}
int Feature::kmeans(int np)
{
	Point * centers;
	centers=new Point[numOfCity];//此时point中的id用于保存这个簇中共有多少个点

//初始化中心点
	int ranCenId=0;
	for(int i=0;i<np;i++)
	{
		int flag=0;
		while(flag==0)
		{	
			ranCenId=myRandom->boundedIntegerRandom(0,numOfCity-1);
			for(int j=0;j<i;j++)
			{
				if(ranCenId==centers[i].id)
				{
					break;
				}

				flag=1;            //如果没有重复，就置1
			}
		}

		centers[i]=pro->point[ranCenId];
		centers[i].id=1; //此簇中有一个点
	}

//end for chushihua 

	double distance=0;
	double mindis=1000000.0;
	int mindisCenter=0;
	int j=0;
	for(int i=0;i<numOfCity;i++)
	{
		for(j=0;j<np;j++)
		{	
			distance =computeDistance(pro->point[i],centers[j]);
			if(mindis>distance)
			{
				mindis=distance;
				mindisCenter=j;
			}
		}

		pro->point[i].clusterId=mindisCenter; //确定当前点属于哪个簇

		//更新簇心位置
		centers[j].x=(centers[j].id*centers[j].x+ pro->point[i].x)/centers[j].id;
		centers[j].y=(centers[j].id*centers[j].y+ pro->point[i].y)/centers[j].id;
		centers[j].id++;
	
	}
}


int Feature::computeModes(vector<struct edgeWithId> &edges)
{
	map<int, int> mym;  //for mode statistical
	int cur;
	int yu;
	vector<struct edgeWithId > ::iterator iter;
	for(iter=edges.begin();iter!=edges.end();iter++)
	{
		cur=(int) iter->length;
		yu=cur%10;
		switch(yu)
		{
			case 0:
			case 1:
			case 2:
				cur=cur-yu;
				break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				cur=cur-yu+5;
				break;
			case 8:
			case 9:
				cur=cur-yu+10;
				break;
			default:
				cur=cur-yu;
		}

		if(mym[cur]==0)
			mym[cur]=1;
		else
			mym[cur]++;
	}

	//构造好了用于统计的map，下面开始进行统计map
	int count=0;
	int maxoccur=0;
	int maxoccurLen=0;
	map<int,int>::iterator miter;
	for(miter=mym.begin();miter!=mym.end();miter++)
	{
		if(miter->second>15)
			count++;
		if(maxoccur<miter->second)
		{
			maxoccur=miter->second;
			maxoccurLen=miter->first;
		}

	}
	//统计结束，赋值

	eqmode=count;
	efmode =maxoccur;
	emode=(double) maxoccurLen;
}

//to be continue
int Feature::outputFeatures()
{
	ofstream out;
	out.open("feature.txt",ios::app);
	if(!out)
	{
		cout<<"open feature.txt error"<<endl;
		exit(0);
	}

	out<<numOfCity<<"\t"<<numOfEdge<<"\t"<<avgLengthOfEdge<<"\t";
	out<<medianEdgeLength<<"\t"<<minLength<<"\t"<<maxLength<<"\t";
	out<<deviation<<"\t"<<minSolution<<"\t"<<eqmode<<"\t";
	out<<efmode<<"\t"<<emode<<"\t";
	// to do 
}
