#!/usr/bin/python
import numpy as np  

import matplotlib.pyplot as plt  
import sys,os

plt.figure(1,figsize=(20,14))

  
def draw(filename,nu):
	infile=open(filename)
	lines=infile.readline();

	y=lines.split("\t")
	t = np.arange(0, len(y)-1, 1)  
	del y[-1]
	try:
		plt.grid(True)  
		plt.plot(t,y)
	except:
			pass
	  
	infile.close()
def main():

	os.system("./ga");
	draw("data.txt",1)
	plt.grid(True)  
	plt.show()  
main()
