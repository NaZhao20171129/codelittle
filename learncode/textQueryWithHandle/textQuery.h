#ifndef TEXTQUERY_H
#define TEXTQUERY_H

#include<iostream>
#include<set>
#include<string.h>
#include<vector>
#include<map>
using namespace std;

class TextQuery
{
	public:
		typedef std::vector<std::string>::size_type line_no;
	private:
		//std::vector<std::string> linesOfText;
		//vector<string> lineOfText;
		map<string,set<line_no> >  wordMap;

		void storeInputFile(istream & is);

		void buildMap();
		void displayMap();

	public:

		vector <string> lineOfText;
		void readFile(istream & is)
		{
			storeInputFile(is);
			buildMap();
			displayMap();

		}
		set<line_no> runQuery(const string &strWord ) const;

		string textOfLineN(line_no  num);

		line_no size() const
		{
			return (lineOfText.end()-lineOfText.begin());
		}

};

#endif

