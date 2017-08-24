#/usr/bin/python
import random
import time
a =[1,3,5]
b=[2,4,6,9,2,1,5,3,7,4]


def qsort(L):  
     if not L:
	return L  
     pivot = random.choice(L)  
     def lt(x): return x<pivot 
     def gt(x): return x>pivot  
     return qsort(filter(lt, L))+[pivot]*L.count(pivot)+qsort(filter(gt, L))
#b=[(2,0),(4,1),(6,2),(9,3),(2,4),(1,5),(5,6),(3,7),(7,8),(4,9)]
#print qsort(b)
#print b[1]
#print b[1][0]
#print b[1][1]
#ta=(13,10)
#b.append(ta)
#print b
#c=b[len(b)-3:]

c=[]
for i in range(0,len(b)):
	w=(b[i],i)
	c.append(w)
#print c

def fc():
	fin=open("tsim.txt")
	for i in range(0,3):
		a=fin.readline()
		al=a.split(',')
		print al
		for j in range(0,5):
			print al[j]
def fd():
	print time.time()
	print time.localtime( time.time())
	print time.strftime("%Y_%m_%d_%H_%M_%S",time.localtime( time.time()))
fd()
