#include<iostream>
#include<string>
#include<stdexcept>
#include<set>

using namespace std;

class ItemBase
{
        //书的号，以及单价
	private:
		string isbn; 
//                int num;
		double price;
	public:
		friend istream & operator <<(istream & i,ItemBase &);
		friend ostream & operator >>(ostream & o,ItemBase &); 

		ItemBase(){}
		ItemBase(string i,double p=0.0):isbn(i),price(p)
		{

		}

		~ItemBase()
		{}

		void setIsbn(string i)
		{
			isbn=i;
		}

		string getIsbn() const 
		{
			return isbn;
		}
//                void setNum(int n)
//                {
//                        this.num=n;
//                }
//
//                int getNum()
//                {
//                        return this.num;
//
//
//                }

		void setPrice(double p)
		{
			price =p;
		}
		double getPrice() const
		{
			return price;
		}

		virtual ItemBase * clone() const
		{
			return (new ItemBase(*this));
		}

		virtual double netPrice(size_t n) const
		{
			return n*price;
		}


};

 istream & operator >>(istream & i,ItemBase & ib)

{
//        printf("input an ItemBase: string  float \n");
	cout<<"input an ItemBase: string  float \n";
	string isbn; float price;
	cin >>isbn>>price;
	ib.setIsbn(isbn);
	ib.setPrice(price);
	return i;
}
ostream & operator << (ostream & o,ItemBase &ib) 
{
	cout<<"the ItemBase is :\n";
	cout<<"isbn--"<<ib.getIsbn()<<"  "<<"price--"<<ib.getPrice()<<endl;

}

//子类，表示可以打折的书
class BulkItem:public ItemBase
{
	public:
		double netPrice(size_t n) const
		{
			if(n>minQty)
				return n*(1-discount)*getPrice();
			else
				return n*getPrice();

		}
		BulkItem * clone() const
		{
			return (new BulkItem(*this));
		}
		size_t  getMinQty()
		{
			return minQty;
		}

		void setMinQty(size_t m)
		{
			minQty=m;
		}
		double getDiscount()
		{return discount;}
		void setDiscount(double d)
		{
			discount=d;
		}
	private:
//要想打折的最小数量，以及相应的折扣
		size_t minQty;
		double discount;
};
//句柄类，封装了上面两个类的继承关系。
class SaleItem
{
	private :
		ItemBase * p;
		std::size_t * use;

		void decrUse() //再析构的时候调用
		{
			if((--*use)==0)
			{
				delete p;
				delete use;
			}
		}
	public:
		SaleItem():p(0),use(new std::size_t (1))
		{ } 
		SaleItem(const ItemBase& b)
		{
			p=b.clone();//构造时候的重点
			use=new std::size_t(2);
		}
		SaleItem(const SaleItem &i):p(i.p),use(i.use)
		{
			++*use;
		}
		~SaleItem()
		{
			decrUse();
		}

		SaleItem & operator =(const SaleItem &);

		const ItemBase * operator ->() const  //重载，为了返回p指针
		  {
			if(p)
				return p;
			else
				throw std::logic_error("unbound saleitem ");
		}

		const ItemBase & operator * ()const  //重载，解引用，为了返回一个ItemBase对象的引用
		{
			if(p)
				return * p;
			else
				throw std::logic_error("unbound saleitem ");
		}

};

SaleItem & SaleItem::operator =(const SaleItem & i)  //重载 赋值运算符，考虑了自身赋值的情况
{
	++*i.use;

	decrUse();

	p=i.p;
	use=i.use;
	return * this;
}



 bool compare(const SaleItem & sil,const SaleItem & sir);


//购物车类，主要是set
class Basket 
{
	public:
		typedef bool (*Comp) (const SaleItem & ,const SaleItem &);
		typedef std::multiset<SaleItem,Comp> setType;

		typedef setType::size_type size_type;
		typedef setType::const_iterator setItConst;

		Basket():items(compare)
		{}

		void addItem(const SaleItem & item)
		{
			items.insert(item);
		}

		size_type size (const SaleItem & i) const
		{
			return items.count(i);
		}

		double total () const;
	private:
		
		std::multiset<SaleItem ,Comp> items; //在构造时，使用句柄类SaleItem，然后传入一个Comp，用于句柄类之间的比较。因为如果想让SaleItem作为set的关键字，必须能够比较他们。
                                                      //但是给句柄类重载 operator< 不好，幸好容器可以指定函数。可以用函数代替<.详情见compare函数。
}; 

 //该函数与<具有相同的接口，SaleItem sil-> （重载过的函数） 返回ItemBase指针，然后再去寻找getIsbn函数。于是可以返回相应的内容。需要在前面声明一下。
inline bool compare(const SaleItem & sil,const SaleItem & sir)
{
	return sil->getIsbn()<sir->getIsbn();

}

double Basket::total () const
{
	double sum=0.0;

	for(setItConst iter=items.begin();iter!=items.end();iter=items.upper_bound(*iter))  ///这个for循环有些特殊。是针对set的。upper_bound函数返回于当前值相等的下一个位置。
	{
		sum+=(*iter)->netPrice(items.count(*iter));   // （*iter） 解引用得到SaleItem类，然后在 -> 得到ItemBase的基类指针，然后动态调用相应子类的netPrice函数。
		cout<<"the sum is "<<sum<<endl;
	}
		
	return sum;
				
}
 

