#!/usr/bin/python
#coding:utf-8
import codecs


def get_data():
		f=open("data.csv","r");
		for i in xrange(0,5):
				print "hello"
				str_line=f.readline()
				words=str_line.split(',')
				for t in words:
					print t.decode("gbk").encode("utf-8")

def main():
		get_data();

main()
