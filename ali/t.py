
m={1:'a',2:'b',3:'d',4:'e'}
def pdict(m):
		for key in m.keys():
				print key
#pdict(m)
def readfile():
		f=open("uudata.csv")
		line = f.readline()
		print line
		words=line.split(",")
		print len(words[3])
		print words[3][0]
		count=0
		for i in words[3]:
			count+=1
			print count,"  ",i
readfile()

