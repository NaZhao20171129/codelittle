#!/usr/bin/python

a={'1':"meng",'2':"qing",'3':'xi'}
print a
a=a.items()

sa=""
s=["%s-%s" %(i[0],i[1]) for i in a]
print 'sa'
sa+="****".join(s)
print sa
sa=""
sa+="%s" %"-".join(s)
print sa
print s
print a
print a[0][1]
print a[1]
