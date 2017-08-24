#include<iostream>
#include<fstream>
#include<vector>
#include<iomanip>
#include<algorithm>
#include<cmath>
#include<cstdlib>

#include<iomanip>

const int Time=96;
const int NETNUMBER=4;
const int PLANTNUMBER=10;
const double STEPMIN=5;
const int T=20;
int Nerghber=2;
const int firstN=8;
const int secondN=5;

const int duanshu=7;
//const int duanbiao[duanshu+1]={0,7,20,34,43,49,69,77,92,96};  //good 2
const int duanbiao[duanshu+1]={0,25,45,55,69,81,92,96};  //good 2
const double STEP=100;
const double walk=1.2;
//const double Cheng=1.025;
const double Cheng=1.025;
const int MIDDLE=95;
int a[NETNUMBER][duanshu]={1,1,1,
			   1,1,1,
			   1,1,1,
			   1,1,1};
const int Para=0;
//const int duanbiao[duanshu+1]={0,11,26,36,52,69,90};
//const int duanbiao[duanshu+1]={0,35,67,78,96};
//const int duanbiao[duanshu+1]={0,26,35,69,80,96};  //good
//const int duanbiao[duanshu+1]={0,31,69,83,96};
//const int duanbiao[duanshu+1]={0,31,69,96};
//const int duanbiao[duanshu+1]={0,31,69,88,96};
//const int duanbiao[duanshu+1]={0,26,35,67,78,96};
//const int duanbiao[duanshu+1]={0,26,35,67,78,96};
using namespace std;
double comAvg(vector<double> stdl)
{

	double stds=0;
	for(int i=0;i<Time;i++)
		stds+=stdl[i];
	return stds/Time;
	

}

double  comStd(vector<double> stdl)
{
	int avg=comAvg(stdl);
	double stds=0;
	for(int i=0;i<Time;i++)
		stds+=pow(stdl[i]-avg,2);
	return sqrt(stds);

}



double comAllen(vector<double> stdl)
{
	double  stds=0;
	int stdc=0;
	for(int i=1;i<Time;i++)
	{

		stds+=pow((stdl[i]-stdl[i-1]),2);
	}

	return stds/Time;
}

double mmin(vector<double> res)
{
	double min;
	min=res[0];
	for(int i=0;i<Time;i++)
	{
		if(min>res[i])
			min=res[i];
	}
	return min;

}

double mmax(vector<double> res)
{
	double max;
	max=res[0];
	for(int i=0;i<Time;i++)
	{
		if(max<res[i])
			max=res[i];
	}
	return max;
}

int commetric(vector< double> stdl,ofstream & out)
{

	double min,max,avg,cha,fangcha,allen;

	min=mmin(stdl);
	max=mmax(stdl);
	cha=max-min;
	avg=comAvg(stdl);
	fangcha=comStd(stdl);
	allen=comAllen(stdl);

	out<<min<<"\t"<<max<<"\t"<<cha<<"\t"<<avg<<"\t"<<fangcha<<"\t"<<allen<<endl;
	cout<<"yes"<<endl;


}
int main()
{

	ifstream in;
	in.open("need2.txt");
	vector<double> stdl;
	double tem;

	ofstream out;
	out.open("resstatics.txt");
	for(int t=0;t<4;t++)
	{
		stdl.clear();
		for(int i=0;i<Time;i++)
		{

			in>>tem;
			stdl.push_back(tem);
		}
		commetric(stdl,out);
	}



}
