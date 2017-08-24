#!/usr/bin/python

import threading
from ctypes import *

lib=cdll.LoadLibrary("a.so")


def loop_dead(str):
	while True:
		print str


#t=threading.Thread(target=loop_dead,args=("abbbbbbb",))
t=threading.Thread(target = lib.Deadloop)
t.start()


lib.Deadloop()
t.join()
