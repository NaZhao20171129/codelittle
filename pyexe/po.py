#coding:utf-8
import urllib2
import urllib
import httplib 
test_data = {'ServiceCode':'aaaa','b':'bbbbb','account':'cf_mmmm', 'password':'123456789','mobile':'15940811425','content':'您的验证码是：4526。请不要把验证码泄露给其他人。'}
test_data_urlencode = urllib.urlencode(test_data)

#target = "http://106.ihuyi.com/webservice/sms.php?method=Submit";
#requrl = "http://192.168.81.16/cgi-bin/python_test/test.py"
requrl= "http://106.ihuyi.com/webservice/sms.php?method=Submit";
#headerdata = {"Host":"192.168.81.16"}

#conn = httplib.HTTPConnection("192.168.81.16")

#conn.request(method="POST",url=requrl,body=test_data_urlencode)#,headers = headerdata) 
#req = urllib2.Request("http://xxoo.com",urllib.urlencode({"username":"root","password":"ROOTXXOO"}))

req=urllib2.Request(url=requrl,data=test_data_urlencode)#,headers = headerdata) 
resp = urllib2.urlopen(req)  
print resp.read()  

