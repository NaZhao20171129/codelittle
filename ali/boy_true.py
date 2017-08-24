#!/usr/bin/python

userMap={}


def get_true():
	f=open("data.csv","r");
	for line in f:
		words=line.split(',')
		if( not userMap.has_key(words[i])
