#include<iostream>
#include<vector>
using namespace std;

int disv(vector<int> x)
{
	for(int i=0;i<x.size();i++)
		cout<<x[i]<<"\t";
	cout<<endl;
}
int main()
{
	vector< int> x;

	x.resize(15);
	cout<<x.size()<<endl;

	disv(x);

	x.clear();
	disv(x);

	cout<<x.size()<<endl;



}
