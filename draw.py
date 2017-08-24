#!/usr/bin/python
import numpy as np  

import matplotlib.pyplot as plt  
import sys

plt.figure(1,figsize=(20,14))
#plt.figure(4,figsize=(20,3))

  
def draw(filename,nu):
	s="ax".join(str(nu))
	t = np.arange(0, 96, 1)  
	infile=open(filename)
	lines=infile.readlines();

	for i in range(1,len(lines)):
		y=lines[i].split("\t")
		del y[-1]
		ax1=plt.subplot(610+i)
		try:
			plt.grid(True)  
			plt.plot(t,y)
		except:
				pass
	  
	infile.close()
def main():
#	ax1=plt.subplot(811)
	draw("data.txt",1)
	plt.grid(True)  
	plt.show()  
main()
