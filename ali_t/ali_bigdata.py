#!/usr/bin/python
#coding:utf-8
import random
import math


raw_data=[] # train data
test_data=[]
data=[]
userList=[]
itemList=[]
userMap={}
itemMap={}

similar=[]
K=6

weight=[1,8,4,5]
throld=5

userNumber=0
itemNumber=0


def random_get_data():
	f=open("data.csv","r");
	for line in f:
		raw_data.append(line)
		test_data.append(line)

#def random_get_data():
#	f=open("data.csv","r");
#	for line in f:
#		if ( random.random()<0.9):
#			raw_data.append(line)
#		else:
#			test_data.append(line)
		 	

def index_user_item():
	for line in raw_data:
		words=line.split(',')
		if(not userMap.has_key(words[0])):
			userMap[words[0]]=len(userList)
			userList.append(words[0])
		if(not itemMap.has_key(words[1]) ):
			itemMap[words[1]]=len(itemList)
			itemList.append(words[1])
	global userNumber
	userNumber=len(userList)
	print 'userNumber=', userNumber
	global itemNumber
	itemNumber=len(itemList)
	print  'itemNumber=', itemNumber

def genMatrix(rows,cols):
	global data
	global similar
	data=[[0 for col in range(cols)] for row in range(rows)]  
	similar=[[0 for col in range(rows)] for row in range(rows)]  
def get_data():
	f=open("data.csv","r");
	for line in f:
		words=line.split(',')
		for t in words:
			print t.decode("gbk").encode("utf-8")

def getUserIndex(id):
	index=(int) (userMap[id])
	return index
def getItemIndex(id):
	index=(int) (itemMap[id])
	return index
def getItemID(index):
	return itemList[index]
def getUserID(index):
	return userList[index]
	
def computeScore():

	global weight
	global data
	genMatrix(userNumber,itemNumber)

	for line in raw_data:
		words=line.split(',')
	#	print words
	#	print  userMap[words[0]] 
	#	print itemMap[ words[1]]
	#	print data[ getUserIndex(words[0]) ] [ getItemIndex( words[1]) ]
		data[ getUserIndex(words[0]) ] [ getItemIndex( words[1]) ]+=int (weight[int (words[2]) ])


def computeCos(v,u):
	sum_son=0.0
	sum_x=0.0
	sum_y=0.0
	for x,y in zip(v,u):
		sum_son+= float(x)*float(y)
		sum_x+=pow(float(x),2)	
		sum_y+=pow(float(y),2)	
	res=sum_son/math.sqrt(sum_x*sum_y)
	return res
		

def computeSimilarity():
	for i in range(0,userNumber):
		print "computing between user ",i 
		similar[i][i]=1
		for j in range(i+1,userNumber):
			res=computeCos(data[i],data[j])
			similar[i][j]=res
			similar[j][i]=res
def qsort(L):  
     if not L:
	return L  
     pivot = random.choice(L)  
     def lt(x): return x<pivot 
     def gt(x): return x>pivot  
     return qsort(filter(lt, L))+[pivot]*L.count(pivot)+qsort(filter(gt, L))	

def get_k_neighbor(u):
	tmp_sim=[]
	for i in range(0,len(similar[u])):
		w=(similar[u][i],i)
		tmp_sim.append(w)
	kn=qsort(tmp_sim)
	return kn[len(kn)-K:]


def get_top_k(vector,k):
	tmp_sim=[]
	for i in range(0,len(vector)):
		w=(vector[i],i)
		tmp_sim.append(w)
	kn=qsort(tmp_sim)
	return kn[len(kn)-k:]


def recommend(uid):
	u_index=getUserIndex(uid)
	kn=get_k_neighbor(u_index)

	pre_item_score=[]

	#####

	#print itemNumber
	for j in range(0,itemNumber):
		pre_score=0
		for i in range(0,K):
			pre_score+=kn[i][0] * data[kn[i][1]][j]
		pre_item_score.append(pre_score)
	return pre_item_score


def main():
	#get_data();
	out=open("res.txt","w")
	print "getting data..."
	random_get_data()
	print "indexing..."
	index_user_item()
	print "computing score..."
	computeScore()
	print "computing similarity..."
	computeSimilarity()
	print "recommending..."
	user_to_pre={}


	for line in test_data:
		recom_list=""
		words=line.split(',')
		if(not user_to_pre.has_key(words[0]) ):
			user_to_pre[words[0] ]=1
			recom_list+=(words[0]+'\t')
			rem=recommend(words[0])
			better_items=get_top_k(rem,5)
			print better_items
			for w in better_items:
				if( float(w[0])>throld):
					recom_list+=(","+getItemID(w[1]) )
			recom_list+='\n'
			out.write(recom_list)
	out.close()
	print "over..."



main()

