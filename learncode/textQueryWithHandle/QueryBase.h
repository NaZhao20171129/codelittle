#ifndef QUERYBASE_H
#define QUERYBASE_H
#include<iostream>
#include<string>
#include<stdexcept>
#include<set>
#include<map>
#include<algorithm>
#include<iterator>

using namespace std;

#include"textQuery.h"

class QueryBase
{
	friend class Query;

	protected:
	typedef TextQuery::line_no line_no;

	virtual ~QueryBase(){}
	private:

	virtual set<line_no > eval (const TextQuery & )const =0;
	virtual ostream & display (ostream & ) const =0;
};


class Query
{
	private:
		QueryBase * q;
		std::size_t * use;
		void decr_use()
		{
			if(--* use==0)
			{
				delete q;
				delete use;

			}
		}

		Query(QueryBase * qb):q(qb),use(new std::size_t(1))
		{
			cout<<"using the private of Query"<<endl;
			
		}
	public:
		Query (const string & );
		~Query() 
		{
			decr_use();
		}
		Query (const Query & query):q(query.q),use(query.use)
		{
			++*use;
			cout<<"using Querrry copy"<<endl;
		}

		
		Query & operator = (const Query & que);

		set<TextQuery::line_no> eval (const TextQuery & t) const
		{
			return q->eval(t);
			
		}

		std::ostream & display (std::ostream &os) const 
		{
			return q->display(os);
		}



	friend Query operator ~ (const Query & que);
	friend Query operator | (const Query & que1,const Query & que2);
	friend Query operator & (const Query & que1,const Query & que2);

};

inline  std::ostream & operator<<(std::ostream & os,const Query & q) 
{
	return q.display(os);
}


class WordQuery :public QueryBase

{
	friend class Query;
	string queryWord;
	
	WordQuery(const string & s):queryWord(s)
	{
		cout<<"using wordQuery constructor with String s"<<endl;
	}

	set<line_no> eval(const TextQuery & t) const
	{
		return t.runQuery(queryWord);
	}

	ostream & display(ostream & os) const
	{
		return os<<queryWord;
	}
};





class NotQuery : public QueryBase
{
	friend Query operator ~ (const Query & q);
	const Query query;

	NotQuery( Query q):query(q){cout<<"using NotQuery constructor with a Query q"<<endl;}
	std::set<line_no> eval(const TextQuery &tq) const;
	ostream & display (ostream &os) const
	{
		return os<<"~( "<<query<<" )"<<endl;
	}

};

inline  Query operator ~ (const Query & q)
{
	return new NotQuery(q);
}

class BinaryQuery:public QueryBase
{
	protected:
	const Query lhs,rhs;
	const string oper;

	BinaryQuery(Query left,Query right,string op):lhs(left),rhs(right),oper(op)
	{
		cout<<"using BinarayQuery constructor with Query left ,Query right string op"<<endl;
	}

	ostream & display (ostream &os)const
	{
		return os<<"( "<<lhs<<" "<<oper<<" "<<rhs<<" "<<endl;
	}

};



class AndQuery:public BinaryQuery
{
	friend Query operator & (const Query & ,const Query & );
	AndQuery(Query left,Query right):BinaryQuery(left,right,"&")
	{
		cout<<"using AndQuery with Query left,Query right"<<endl;
	}

	std::set<line_no> eval(const TextQuery & tq) const ;
};

inline Query operator & (const Query & l ,const Query & r)
{
	return new AndQuery(l,r);
}

class OrQuery:public BinaryQuery
{
	friend Query operator | (const Query & ,const Query & );
	OrQuery(Query left,Query right):BinaryQuery(left,right,"&")
	{
		cout<<"using OrQuery with Query left,Query right"<<endl;
	}

	std::set<line_no> eval(const TextQuery & tq) const ;
};

inline Query operator | (const Query & l,const Query & r)
{
	return new OrQuery(l,r);
}






#endif
