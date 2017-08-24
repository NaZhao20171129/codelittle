#include"QueryBase.h"
//#include"textQuery.h"
#include<set>
#include<iostream>
#include<string>
#include<fstream>


void printResult(const set<TextQuery::line_no> & res,string & w,TextQuery & tq)
{
	set<TextQuery::line_no>::size_type size=res.size();
	cout<<"\n"<<w<<" occurs"<<size <<" times"<<endl;
	set<TextQuery::line_no>::const_iterator  it =res.begin();

	for(;it!=res.end();++it)
	{
		cout<<"\t (line "<<(*it)+1<<" )"<<endl;
		cout<<tq.textOfLineN(*it)<<endl;
	}
}

int main(int argc , char * argv[])
{
	if(argc<2)
	{
		cout<<"usage "<<argv[0]<<" file to index"<<endl;
		return 1;
	}

	ifstream infile;
	infile.open(argv[1]);
	if(!infile)
	{
		cerr<<"the file "<<argv[1]<<" cannot be open"<<endl;
		return 1;
	}

	TextQuery tq;
	tq.readFile(infile);

	while(true)
	{
		cout<<"input two  word to look for"<<endl;
		string word1,word2;
		cin>>word1>>word2;

		if(!cin||word1=="q"|| word1=="Q")
			break;

//		set<TextQuery::line_no> result=tq.runQuery(word);
		Query q=Query(word1)&Query(word2);
		const set<TextQuery::line_no> &result =q.eval(tq);
		printResult(result,word1,tq);
	}

	return 0;
}
