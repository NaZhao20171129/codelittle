#include"salse.h"

int main()
{
	Basket b;
	ItemBase ib1,ib2;
	BulkItem bi1;
	ib1.setPrice (10.5);
	ib1.setIsbn("abc1001");

	bi1.setIsbn("abc1003");
	bi1.setPrice(2.5);
	bi1.setMinQty(4);
	bi1.setDiscount(0.5);

	SaleItem si1(ib1);
	ItemBase * base=&bi1;
	SaleItem si2(*base);


	b.addItem(si1);
	b.addItem(si1);
	b.addItem(si1);

	cin>>ib2;
	//si1(ib1);
	SaleItem si3(ib2);
	b.addItem(si3);


	b.addItem(si1);

	b.addItem(si2);
	b.addItem(si2);
	b.addItem(si2);
	b.addItem(si2);
	b.addItem(si2);
	b.addItem(si2);

	cout<<"the total money is :"<<b.total()<<endl;

	return 0;

}
