#include<iostream>
#include<cstdlib>
#include<fstream>
#include<vector>
#include<cmath>

using namespace std;

const int Time=96;
const int NUM=4;
const int N=7;
const int C=9;

const int duanshu=3;
const int duanbiao[duanshu+1]={0,29,79,96};
//const int duanbiao[duanshu+1]={0,31,69,96};


double comMinMaxCha(vector<double> &need,int start,int end)
{
	double min,max;
	min=need[start];
	max=need[start];
	for(int i=start;i<end;i++)
	{
		if (need[i]>max)
			max=need[i];
		if(need[i]<min)
			min=need[i];

	}

	return max-min;

}

double comAllan(vector<double> & need, int start,int end)
{
	double allen=0;
	for(int i=start+1;i<end;i++)
	{
		allen+=pow(need[i]-need[i-1],2);
		//cout<<need[i]<<"\t"<<need[i-1]<<"\t"<<need[i]-need[i-1]<<endl;
	}
	return allen/(end-start);
}

int main()
{
	ifstream in;
	in.open("leftload.txt");
	if(!in)
	{
		cout<<"open file error"<<endl;
		exit(0);
	}

	ofstream out;
	out.open("statics_result.txt");
	if(!out)
	{
		cout<<"cannot open file to write"<<endl;
		exit(0);
	}
	vector< double > need;

	double sum=0;
	double tem;
	double fenggucha;
	double allancha;
	for(int x=0;x<NUM;x++)
	{
		need.clear();
		for(int i=0;i<Time;i++)
		{
			in>>tem;

			need.push_back(tem);

		}

		for(int i=0;i<duanshu;i++)
		{
			
			//out<<"duanshu"<<i<<endl;
			fenggucha=comMinMaxCha(need,duanbiao[i],duanbiao[i+1]);
			//out<<"fenggucha:\t"<<fenggucha<<"\t";
			out<<fenggucha<<"\t";
			allancha=comAllan(need,duanbiao[i],duanbiao[i+1]);
			//out<<"allancha:\t"<<allancha<<endl;
			out<<allancha<<"\t";
		}
		out<<endl;

	}
	in.close();
	out.close();
}
