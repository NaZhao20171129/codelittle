#!/usr/bin/python

import threading

def loop_dead(str):
	while True:
		print str


t=threading.Thread(target=loop_dead,args=("abbbbbbb",))
t.start()


loop_dead("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
t.join()
