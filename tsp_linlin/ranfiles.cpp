#include<iostream>
#include<string>
#include<fstream>
#include<stdlib.h>
#include<vector>
#define TIMES 1000
using namespace std;

void ran(vector<string> & n)
{
	srand(time(NULL));
	string tmp;
	int l,r;
	for(int i=0;i<TIMES;i++)
	{
		l=rand()%n.size();
		r=rand()%n.size();
		tmp=n[l];
		n[l]=n[r];
		n[r]=tmp;
		

	}
}
int main()
{
	vector<string> names;
	
	ofstream out;
	ifstream in;
	in.open("file_list.txt");

	string aname;
	while(!in.eof())
	{
		in>>aname;
		names.push_back(aname);
	}

	in.close();
	ran(names);
	out.open("file_list_ran.txt");
	for(int i=0;i<names.size();i++)
		out<<names[i]<<endl;
	out.close();
	return 0;





	
}
