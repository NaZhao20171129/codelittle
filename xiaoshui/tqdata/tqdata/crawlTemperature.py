#!/usr/bin/python
#coding:utf8

import urllib,urllib2
import BeautifulSoup 
import socket
import time,requests
from time import sleep
import sys


reload(sys)
sys.setdefaultencoding('utf-8')

def getdate(thedate):

        url="http://lishi.tianqi.com/heqing/"+thedate+".html"
        print "getting the "+ thedate
        r = requests.get(url)
        html_1= urllib.urlopen(url).read()

        html =html_1.decode('gb2312','ignore').encode('utf-8')
        #besoup=BeautifulSoup(html.decode("utf-8","ignore") )
        soup=BeautifulSoup.BeautifulStoneSoup(html)

        name=thedate+".txt"
        fw=open(name,"w")
        for ul in soup.find("div",{"class":"tqtongji2"}).findAll("ul"):
             #   curday=[]
                for li in ul.findAll("li"):
                        try:
                                data=li.a.contents[0].decode("utf-8")
                                #curday.append(data)
                        except:
                                data=li.contents[0].decode("utf-8")
                                #curday.append(data)
                        fw.write(data)
                        fw.write("\t")
                        
                       # curmonth.append(curday)
                fw.write("\n")



        fw.close()
        print "finished the "+ thedate



if __name__=="__main__":
        targetDates=[]
        for year in ('2011','2012'):
                for mon in ("01","02","03","04","05","06","07","08","09","10","11","12",):
                        targetDates.append(year+mon)

        for dt in targetDates:
            getdate(dt)

        print "finished all"


