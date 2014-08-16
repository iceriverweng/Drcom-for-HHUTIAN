#Test if not auth
import sys,os,urllib2,socket

try:
	check_online = urllib2.urlopen('http://www.qq.com')
	foo=check_online.read()
except:
	print "Network line Err,check the line...Hit the return"
	raw_input()
else:
	if '10.1.1.10' in foo:
		print "Not Online,try to auth"
		raw_input()
	else:
		print "Already online,Hit the return"
		raw_input()
	sys.exit(0)
