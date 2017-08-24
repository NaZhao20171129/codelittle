Void Resolution(ClauseSet *S2,ClauseSet *S2,ClauseSet *S3) 
{ 
   head1指向子句集S1中的第一个子句；
   head2指向子句集S2中的第一个子句；
   if(head1==NULL || head2 ==NULL)
	   exit(0)；
    while(true)
    {
	P指向子句集S1中的第一个子句；
 	do
	{
		q 指向子句集S2中的第一个子句;
		do
		{
			调用两子句间的归结函数 guijie（p,q）//返回指向子句的指针pc，若pc==null
			if(pc是空子句)       //说明两子句不能归结。
			 {
				 输出信息：该子句集S不可满足！
				 exit(0);
			 }
			else
				将pc指向的子句插入到S1中；
					
			q指向S2中的下一个子句；

		}while（q!=head2）
		p指向S2中的下一个子句；

	}while（p!=head1）

	输出信息：该子句集不能证明不可满足；
	exit(0); 
}





//该函数从p和q指向的子句中寻找互补谓词对，并调用具体的归结实现函数生成新
//子句，并返回新子句。如果子句p和q不能归结则返回NULL。
Clause *guijie(Clause *p,*q)
{
	p指向子句C1的第一个谓词；
	while(p!=null)
	{
		q指向子句C2的第一个谓词；
		while（q!=null）
		{
			if（p，q指向的谓词对象的num之和==0）
			{
				运用最一般合一算法对C1和C2进行归结；终止并返回新子句指针；
			}
			else if( (p指向的谓词对象的num的绝对值)>q指向的谓词对象的num的绝对值)
				q指向下一个谓词；
			else 跳出该层循环；
		}
	P指向下一个谓词；
	}
	返回NULL；//代表C1与C2不能归结。
}
