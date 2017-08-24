#include "QueryBase.h"

Query::Query(const string & s):q(new WordQuery(s)),use(new std::size_t(1))
{
	cout<<"using Query with a string "<<endl;	
}

Query & Query::operator =(const Query & que)
{
	++ *que.use;
	decr_use();
	q=que.q;
	use=que.use;
	return * this;
}


set<TextQuery ::line_no > NotQuery :: eval(const TextQuery & tq) const


{
	set<line_no> hasVal=query.eval(tq);
	set<line_no > ret;
	line_no  total =tq.size();
	for(line_no n=0;n!=total;++n)
		if(hasVal.find(n)==hasVal.end())
			ret.insert(n);


        return ret;
}


std::set<TextQuery::line_no> OrQuery:: eval (const TextQuery & tq) const 
{
	set<line_no> right=rhs.eval(tq), left =lhs.eval(tq);
	left.insert(right.begin(),right.end());
	return left;
}


std::set<TextQuery ::line_no > AndQuery::eval(const TextQuery & tq) const 
{
	set<line_no> left =lhs.eval(tq),
		     right=rhs.eval(tq);

	set<line_no > ret;

	set_intersection(left.begin(),left.end (),right.begin(),right.end(),
			 inserter(ret,ret.begin()));
	return ret;
}

