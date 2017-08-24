#include<iostream>
#include<string>
#include<sstream>
#include<stdexcept>
using namespace std;
#include"textQuery.h"

 void TextQuery::storeInputFile(istream & is)
{
	string textline;
	while(getline(is,textline))
		lineOfText.push_back(textline);


}

void TextQuery::buildMap()
{
	for (TextQuery::line_no lineNum =0;lineNum<lineOfText.size();++lineNum)
	{
		istringstream line(this->lineOfText[lineNum]);
		string word;
		while(line>>word)
		{
			wordMap[word].insert(lineNum);
		}
	}
}

void TextQuery::displayMap()
{
	std::map<string,set<TextQuery::line_no> >::iterator mapIt=wordMap.begin();
	
	cout<<"word\t\t"<<"line set\n"<<endl;

	while(mapIt!=wordMap.end())
	{
		cout<<mapIt->first<<" ";
		set<TextQuery::line_no>::iterator setIt=mapIt->second.begin();
		while(setIt!=mapIt->second.end())
		{
			cout<<*setIt<<" ";
			setIt++;
		}
		cout<<endl;
		mapIt++;
	}
}


string TextQuery::textOfLineN(TextQuery::line_no n)
{
	if(n<lineOfText.size())
		return lineOfText[n];
	throw(std::out_of_range("line out of range"));
}


set<TextQuery::line_no> TextQuery::runQuery(const string & word) const
{
	map< string, set<TextQuery::line_no> > ::const_iterator loc=wordMap.find(word);
	if(loc!=wordMap.end())
		return loc->second;
	else
		return set<TextQuery::line_no>();
}
