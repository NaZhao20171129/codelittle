#!/usr/bin/python
#coding:utf-8

targetDates=[]
for year in ('2011','2012'):
        for mon in ("01","02","03","04","05","06","07","08","09","10","11","12",):
                targetDates.append(year+mon)
print targetDates


for thedate in targetDates:
        url="http://lishi.tianqi.com/heqing/"+thedate+".html"
        print url
