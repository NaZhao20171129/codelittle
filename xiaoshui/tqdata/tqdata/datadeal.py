#!/usr/bin/python
#coding:utf8

import sys


reload(sys)
sys.setdefaultencoding('utf-8')


def dealfile(dt):
	fin=open(dt+".txt")
	count=0
	sum=0
	fin.readline()
	while 1:
		line=fin.readline()
		if not line:
			break
		count+=1
		a=line.split("\t")
		day=(int(a[1])+int(a[2]))/2
		print day
		sum+=day
	
	print count
	print sum
	return sum*1.0/count

		


if __name__=="__main__":
        targetDates=[]
        for year in ('2011','2012'):
                for mon in ("01","02","03","04","05","06","07","08","09","10","11","12",):
                        targetDates.append(year+mon)

	result=[]
        for dt in targetDates:
            res=dealfile(dt)
	    result.append(res)


	fout=open("result.txt","w")
	for re in result:
		fout.write(str(re)+"\n")

	print result
        print "finished all"

