#include <iostream>
#include <string>
#include <fstream>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <ext/hash_map>
#include <sys/time.h>  
#include <stdio.h>
#include<string.h>


//csdn文本总行数
#define TOTAL_LINE_NUMBER 6428632
//线程个数
#define MAX_THREAD_SUM 5
//每个线程处理的行数
#define PER_THREAD_LINE_SUM TOTAL_LINE_NUMBER/MAX_THREAD_SUM+1

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

using namespace std;
using namespace __gnu_cxx;

struct str_hash{
	size_t operator()(const string& str) const
	{
	   unsigned long __h = 0;
	   for (size_t i = 0 ; i < str.size() ; i ++)
		__h = 5*__h + str[i];
		return size_t(__h);
	}
};


hash_map<string,int,str_hash> passwordfreq;
hash_map<string,int,str_hash> mailfreq;


bool less_first(pair<int, string> const& lhs, pair<int, string> const& rhs)
{
    return lhs.first > rhs.first;
}

vector<pair<int, string> > mirror_map(hash_map<string, int, str_hash> const& m)
{
	vector<pair<int, string> > mirror;
	for (hash_map<string, int, str_hash>::const_iterator i = m.begin(); i != m.end(); ++i)
	{
		mirror.push_back(pair<int, string>(i->second, i->first));
	}
    std::sort(mirror.begin(), mirror.end(), less_first);
    return mirror;
}


void *doanalysis(void *avg)
{
	int threadCount=(int)avg;
	
	hash_map<string,int,str_hash> mypasswordfreq;
	hash_map<string,int,str_hash> mymailfreq;
	
	ifstream input;
	input.open("csdn_psaaword.sql");
	
	//目前遍历到的行号
	int lineNum=0;
	
	//是否进入该线程处理的行范围
	int dobegin=0;
	
	int perTLS=PER_THREAD_LINE_SUM;
	string eachline;
	
	while(getline(input,eachline) )
	{	
		if (lineNum/perTLS==threadCount){
			dobegin++;
			string::size_type start = 0;
			string::size_type end = eachline.find_first_of("#");
			string usename = eachline.substr(start, end - start);
			
			start = end + 1;
			end = eachline.find_first_of("#",start);
			string password = eachline.substr(start, end - start);
			mypasswordfreq[password]++;
			
			start = end + 1;
			end = eachline.find_first_of("#",start);
			string email = eachline.substr(start, end - start);
			string emailtype=email.substr(email.find_first_of("@",0)+1);
			mymailfreq[emailtype]++;
			
		}else if (dobegin!=0){
			//离开该线程处理的行范围
			input.close();
			
			pthread_mutex_lock( &count_mutex );
			for(hash_map<string, int, str_hash>::iterator myiter= mypasswordfreq.begin(); myiter != mypasswordfreq.end(); ++myiter){
				passwordfreq[myiter->first]+=myiter->second;
			}
			for(hash_map<string, int, str_hash>::iterator myiter= mymailfreq.begin(); myiter != mymailfreq.end(); ++myiter){
				mailfreq[myiter->first]+=myiter->second;
			}
			pthread_mutex_unlock(&count_mutex );
			
			pthread_exit((void *)1);
		}
		
		lineNum++;
		
	}
}


int main()
{
	cout <<"Begin...."<< endl;
	
    // start timing
    struct timeval tim;  
    gettimeofday(&tim, NULL);  
    double analystart=tim.tv_sec+(tim.tv_usec/1000000.0);
	
    pthread_t MyThreads[MAX_THREAD_SUM];
    
	for(int i=0;i<MAX_THREAD_SUM;i++){
		pthread_t tid;
		int rc;
	
		rc= pthread_create(&MyThreads[i],NULL,doanalysis,(void *)i);
		if(rc!=0)
			printf("%s:%d\n",__func__,strerror(rc));
	}
	
	for(int i = 0;i < MAX_THREAD_SUM;i++)  
	{  
        pthread_join(MyThreads[i],NULL);
	}
	
	//常用密码按使用次数降序排序后，输入至文本（密码 被使用次数） 
	ofstream pwdoutput;
	pwdoutput.open("passwordfrequency.txt",ios::out|ios::app);
	
	vector<pair<int, string> > pwdmirror = mirror_map(passwordfreq);
	for(vector<pair<int, string> >::iterator myiter= pwdmirror.begin(); myiter != pwdmirror.end(); ++myiter){
		pwdoutput<<myiter->second<<"\t"<<myiter->first<<endl;
	}
	
	pwdoutput.close();
	
	//常用邮箱按使用次数降序排序后，输入至文本（邮箱后缀 被使用次数） 
	ofstream emailoutput;
	emailoutput.open("emailfrequency.txt",ios::out|ios::app);
	
	vector<pair<int, string> > edumirror = mirror_map(mailfreq);
	for(vector<pair<int, string> >::iterator myiter= edumirror.begin(); myiter != edumirror.end(); ++myiter){
		emailoutput<<myiter->second<<"\t\t\t"<<myiter->first<<endl;
	}
	
	emailoutput.close();
	
	gettimeofday(&tim, NULL);  
    double analyend=tim.tv_sec+(tim.tv_usec/1000000.0);
	cout <<"Analysis with "<<MAX_THREAD_SUM<<" Threads use:"<<analyend-analystart<<"seconds."<< endl;
	
	return 1;
}
