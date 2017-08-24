#!/usr/bin/python
#coding:utf-8
import random
import math
import time
import datetime


all_data=[] 
train_data=[] 
test_data=[]
data=[]
data_true_buy=[]
userList=[]
itemList=[]
userMap={}
itemMap={}

pre_result={}
true_result={}
similar=[]
K=2
K_item=80
throld=60

BEGINDAY = datetime.date(2013, 4, 13)
weight=[1,8,4,5]

userNumber=0
itemNumber=0


def get_data_train_test():
	global test_data
	global train_data
	f=open("udata.csv","r");
	for line in f:
		words=line.split(",")
		all_data.append(line)
		if(int (words[3][0])>=8 or (int (words[3][0])>=7 and int(words[3][4])>15)):
				test_data.append(line)
		else:
				train_data.append(line)
	out_train=open("train.txt","w")
	for line in train_data:
		out_train.write("%s\n"%line)
	out_train.close()
	out_test=open("test.txt","w")
	for line in test_data:
		out_test.write("%s\n"%line)
def random_get_data():
	f=open("data.csv","r");
	for line in f:
		if ( random.random()<0.9):
			raw_data.append(line)
		else:
			test_data.append(line)
		 	

def index_user_item():
	for line in all_data:
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
	global data_true_buy
	data=[[0 for col in range(cols)] for row in range(rows)]  
	data_true_buy=[[0 for col in range(cols)] for row in range(rows)]  
	similar=[[0 for col in range(rows)] for row in range(rows)]  
def get_data():
	f=open("data.csv","r");
	for line in f:
		words=line.split(',')
		for t in words:
			print t.decode("gbk").encode("utf-8")

def getUserIndex(id):
	index=-1
	if(userMap.has_key(id) ):
		index=(int) (userMap[id])
	return index
def getItemIndex(id):
	index=-1
	if(itemMap.has_key(id) ):
		index=(int) (itemMap[id])
	return index
def getItemID(index):
	return itemList[index]
def getUserID(index):
	return userList[index]
def parse_date(raw_date):
	#entry_date = raw_date.decode("gbk")
	entry_date = raw_date
	month = int(entry_date[0])
	if len(entry_date) == 11:
		day = 10 * int(entry_date[4]) + int(entry_date[5])
	else:
		day = int(entry_date[4])
	return (datetime.date(2013, month, day)-BEGINDAY).days

def computeScore():

	global weight
	global data
	global data_true_buy
	genMatrix(userNumber,itemNumber)

	for line in all_data:
		words=line.split(',')
	#	print words
	#	print  userMap[words[0]] 
	#	print itemMap[ words[1]]
	#	print data[ getUserIndex(words[0]) ] [ getItemIndex( words[1]) ]
		twei=math.log(1+parse_date(words[3]) )

		data[ getUserIndex(words[0]) ] [ getItemIndex( words[1]) ]+=int (weight[ int (words[2] )] ) * 1.0 *twei
		if(words[2]=='1'):
			data_true_buy[ getUserIndex(words[0]) ] [ getItemIndex( words[1]) ]=1

	out_true_buy=open("true_buy.txt","w")
	
	for i in range(0,len(data_true_buy)):
		out_true_buy.write(getUserID(i))
		out_true_buy.write("\t")
		for j in range(0,len(data_true_buy[i]) ):
				if (data_true_buy[i][j]==1) :
					out_true_buy.write(getItemID(j))
					out_true_buy.write(",")
		out_true_buy.write("\n")
					
	out_true_buy.close()


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
		

def getSimilarity():
	fin=open("similar_all.txt")
	for i in range(0,userNumber):
		print "getting similar for user",i
		tmp=fin.readline()
		tmp_list=tmp.split(',')
		for j in range(0,userNumber):
			similar[i][j]=float (tmp_list[j])
		#similar[i][j]=1

	fin.close()


def computeSimilarity():
	curTime=time.strftime("%Y_%m_%d_%H_%M_%S",time.localtime( time.time()))
	for i in range(0,userNumber):
		print "computing between user ",i 
		similar[i][i]=1
		for j in range(i+1,userNumber):
			res=computeCos(data[i],data[j])
			similar[i][j]=res
			similar[j][i]=res
	
	outSimi=open("similar.txt"+curTime,"w")
	for i in range(0,len(similar)):
		for j in range(0,len(similar[i])):
			outSimi.write ("%3f," %similar[i][j])
		outSimi.write ("\n")
	outSimi.close()


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
	pre_item_score=[]
	for j in range(0,itemNumber):
		pre_item_score.append(0.0)
	if (getUserIndex(uid)<0):
		return pre_item_score
	u_index=getUserIndex(uid)
	kn=get_k_neighbor(u_index)

	for j in range(0,itemNumber):
		pre_score=0
		for i in range(0,K):
			pre_score+=kn[i][0] * data[kn[i][1]][j]
		pre_item_score[j]=pre_score
		pre_score=0
	return pre_item_score


def computeR()	:
		recall=0
		hit=0
		total=0
		for key in pre_result.keys():
				total+=len(true_result[key])
				for item in pre_result[key]:
					if item in true_result[key]:
						hit+=1
					
		recall=hit*1.0/total
		return recall

def computeP()	:
		precision=0
		hit=0
		total=0
		for key in pre_result.keys():
				total+=len(pre_result[key])
				for item in pre_result[key]:
					if item in true_result[key]:
						hit+=1
					
		precision=hit*1.0/total
		return precision


def computePR()	:
	pre=computeP()
	recall=computeR()
	print "precision= ",pre
	print "recall= ",recall

	fscore=2.0*pre*recall/(pre+recall)
	print "fscore= ",fscore

def test():
		#get the true result
		for line in test_data:
			words=line.split(",")
			if (not true_result.has_key(words[0])):
				true_result[words[0] ]=[]
			if( words[2]=='1'):
				true_result[words[0] ].append(words[1])
#		for line in raw_data:
#			words=line.split(",")
#			if (not true_result.has_key(words[0])):
#				true_result[words[0] ]=[]
#			if( words[2]=='1'):
#				true_result[words[0] ].append(words[1])
		print "ture result dict"
		print true_result


		computePR()	

def main():
	#file_after="_"+K+"_"+K_item+"_"+throld
	#file_after="_".join(K)+"_".join(K_item)+"_".join(throld)
	file_after="_%d_%d_%d.txt"%(throld,K_item,K)
	#get_data();
	#out=open("res"+file_after,"w")
	out=open("predict.txt","w")
	print "getting data..."
	get_data_train_test()
	print "indexing..."
	index_user_item()
	print "getting similarity..."
	computeScore()
	#genMatrix(userNumber,itemNumber)
	getSimilarity()
	print "recommending..."
	user_to_pre={}

	outRecomScore=open("recom"+file_after,"w")

	count=0
	for line in all_data:
		words=line.split(',')
		if(not user_to_pre.has_key(words[0]) ):
			user_to_pre[words[0] ]=1


	for keys in user_to_pre.keys():
		item_pre_list_for_u=[]
		count+=1
		print "recommending for user",count,keys
		rem=recommend(keys)
		better_items=get_top_k(rem,K_item)
		for item,index in better_items:
			outRecomScore.write("%3f,"%item)
		outRecomScore.write("\n")
		for w in better_items:
			if( float(w[0])>throld):
				item_pre_list_for_u.append(getItemID(w[1]) )

		if(len(item_pre_list_for_u)>0):
			out.write(keys+'\t'+','.join(item_pre_list_for_u)+"\n")

	out.close()
	outRecomScore.close()

#	test()
	print "over..."



main()

