#include<iostream>
#include<bitset>
#include<vector>
#include<cmath>
#include<string>
#include<list>

using namespace std;
const double PI = 3.1415926535897932384626433832795;
int main()
{
	bitset<8> te;
	//te.set();
	te.set(1,1);
	te.set(7,1);
	cout<<~te[2]<<endl;
	cout<<te.to_ulong()<<endl;
	
	string b=te.to_string();
	cout<<b<<endl;

	cout<<sin(PI)<<endl;

	vector<int> a;
	cout<<a.size()<<endl;
	a.resize(10);
	cout<<a.size()<<endl;
	cout<<pow(2,15)<<endl;

	vector<int> bb;
	bb.push_back(3);
	bb.push_back(7);
	bb.push_back(5);

	cout<<bb.size()<<endl;
	vector<int> c;
	c.resize(15);
	cout<<c.size()<<endl;
	c=bb;
	cout<<c.size()<<endl;



}