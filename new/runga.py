#!/usr/bin/python
import numpy as np  

import matplotlib.pyplot as plt  
import sys,os

plt.figure(1,figsize=(16,11))

  
def draw(filename,nu):
	infile=open(filename)
	lines=infile.readlines();

	y=lines
	#y=lines.split("\n")
	t = np.arange(0, len(y)-1, 1)  
	del y[-1]
	try:
		plt.grid(True)  
		plt.plot(t,y)
	except:
			pass
	  
	infile.close()
def main():

#	os.system("./ga");
	if(len(sys.argv)<2):
		f="best.txt"
	else:
		f=sys.argv[1]

	
	draw(f,1)
	plt.grid(True)  
	plt.show()  
main()
