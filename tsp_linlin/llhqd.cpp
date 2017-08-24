#include<iostream>
#include<fstream>
#include<vector>
#include<stdlib.h>

#include<math.h>

#define LLHLEN 15
using namespace std;
typedef struct llhq
{
	vector<int> llh;
	double fitness;
	int generation;
}llhq;

int compareLLHQ(llhq & l1,llhq & l2)
{
	vector<int> v1;
	vector<int> v2;
	for(int i=0;i<LLHLEN;i++)
	{
		switch(l1.llh[i])
		{
			case 1:
			case 2:
			case 3:
				v1.push_back(1);
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
				v1.push_back(2);
				break;
			case 14:
			case 15:
				v1.push_back(3);
				break;
			default:
				v1.push_back(1);

		}
		switch(l2.llh[i])
		{
			case 1:
			case 2:
			case 3:
				v2.push_back(1);
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
				v2.push_back(2);
				break;
			case 14:
			case 15:
				v2.push_back(3);
				break;
			default:
				v2.push_back(1);
		}
	}
		int distance=0;
		int cha=0;
		cout<<"yuan1  ";
		for(int i=0;i<l1.llh.size();i++)
			cout<<l1.llh[i]<<" ";
		cout<<endl;

		cout<<"L1    ";
		for(int i=0;i<v1.size();i++)
		{
			cout<<v1[i]<<" ";
			
		}
		cout<<endl;
		cout<<"yuan2  ";
		for(int i=0;i<l1.llh.size();i++)
			cout<<l1.llh[i]<<" ";
		cout<<endl;

		cout<<"L2    ";
		for(int i=0;i<v2.size();i++)
		{
			cout<<v2[i]<<" ";
			
		}
		cout<<endl;
		for(int i=0;i<v1.size();i++)
		{
			cha =abs(v1[i]-v2[i])+1;
			distance +=cha;
		}

		return distance;


}
int main()
{
	vector< llhq > llhs;
	vector<llhq>::iterator iter;
	ifstream in;
	in.open("populations.xls");
	if(!in)
	{
		cout<<"cannot open file ,llhQuatlites"<<endl;
		exit(0);
	}

	llhq t;
	int aint;
	while(!in.eof())
	{
		t.llh.clear();
		for(int i=0;i<LLHLEN;i++)
			{
				in>>aint;
				t.llh.push_back(aint);
			}
		in>>t.fitness;
		in>>t.generation;

		llhs.push_back(t);
	}

	int dis=0;
	double qua=0.0;
	ofstream out;
	out.open("quadis.xls");
	if(!out)
	{
		cout<<"cannot open file quadis.xls"<<endl;
		exit(0);
	}
	for(int i=0;i<llhs.size();i++)
	{
		for(int j=0;j<llhs.size();j++)
		{
			dis=compareLLHQ(llhs[i],llhs[j]);
			qua=fabs(llhs[i].fitness-llhs[j].fitness);
			out<<dis<<"\t"<<qua<<endl;
		}
	}

	in.close();
	out.close();



}
