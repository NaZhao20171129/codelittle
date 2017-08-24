#include"random.hpp"
#include<utility>
#include<map>
#include<sstream>


#define MIN 1
#define MAX 300

using namespace std;

struct point 
{
	int x, y;
};

bool operator < (const point & p1, const point & p2)
{
	if(p1.x<p2.x || p1.x==p2.x&& p1.y<p2.y)
		return 1;
	else
		return 0;
}
int writeProblemToFile(string filename,map<point,int> * po,int cityN)
{
	ofstream outfile(filename.c_str());
	if(!outfile)
	{
		cout<<"cannot open file to write"<<endl;
		return 0;
	}
	else
	{
		cout<<"writting to "<<filename<<endl;
		map<point,int>::iterator iter=po->begin();
		outfile<<cityN<<endl;
		for(;iter!=po->end();iter++)
		{
			outfile<<(iter->first).x<<" "<<(iter->first).y<<endl;
		}
		outfile.close();
		return 1;
	}
}


int main()
{
	Random * r =new Random;

	int cityNum;
	int problemNum=50;
	int diff;
	int i,j;
	int x,y;
	map<point,int> city;
	point p;
	string problemfileName;
	string prefix("problem_");
	ostringstream ss(problemfileName);

	cityNum=50;
	for( diff=0;diff<5;diff++)
	{
		for(i=0;i<problemNum;i++)
			{
				for(j=0;j<cityNum;)
				{
					x=r->boundedRandom(MIN,MAX);			
					y=r->boundedRandom(MIN,MAX);	
					p.x=x;
					p.y=y;
					if(city.count(p)==0)
					{
						//city.insert(make_pair(p,1));
						city[p]=1;
						j++;
					}
				}
				//问题编号 problem_50_1
				ss<<prefix<<cityNum<<"_"<<i+1;

			if(!writeProblemToFile(ss.str(),&city,cityNum))
			{
				cout<<"writing "<<ss.str()<<" error"<<endl;
				return 0;
			}
			ss.str("");
		}
		cityNum+=10;
	}
	return 0;
}
