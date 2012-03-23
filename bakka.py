#!/usr/bin/python
#-* encoding: utf8 *-

def read(url):                                             
    import urllib2
    con = urllib2.urlopen(url)
    return con.read()

def writefile(path,content):
    f=open(path, 'w')
    f.write(content)
    f.close()
    return True

def backupfile(path,backuppath):
    import shutil
    shutil.copyfile(path, backuppath)
    return nil

def get_hostsfile():
    import os
    hostsfile={'posix':"/etc/hosts",'nt':'c:\\windows\\system32\\drivers\\etc\hosts'}[os.name]
    return hostsfile


import re, sys
BAKKA_START="## BAKKA_START: DO NOT MODIFY"
BAKKA_END="## BAKKA_END: DO NOT MODIFY"

print '''
--------------------------------------------------------------------
 Welcome to Bakka - Console Edition
--------------------------------------------------------------------
[[ Choose the type of service ]]

  1) Test
  2) RC
  3) Production
  4) Local
  5) Beta
--------------------------------------------------------------------
'''
m = raw_input("Menu Number : ")
url = {
    "1": "http://dev.media.daum.net/wiki/index.php/%EC%95%84%EA%B3%A0%EB%9D%BC/hosts/%ED%85%8C%EC%8A%A4%ED%8A%B8?action=hosts",
    "2": "http://dev.media.daum.net/wiki/index.php/%EC%95%84%EA%B3%A0%EB%9D%BC/hosts/RC?action=hosts",
    "3": "http://dev.media.daum.net/wiki/index.php/%EC%95%84%EA%B3%A0%EB%9D%BC/hosts/%EC%8B%A4%EC%84%9C%EB%B9%84%EC%8A%A4?action=hosts",
    "4": "http://dev.media.daum.net/wiki/index.php/%EC%95%84%EA%B3%A0%EB%9D%BC/hosts/%EB%A1%9C%EC%BB%AC?action=hosts",
    "5": "http://dev.media.daum.net/wiki/index.php/%EC%95%84%EA%B3%A0%EB%9D%BC/hosts/%EB%B2%A0%ED%83%80?action=hosts"
}[m]

webcont = read(url)
webcont = re.sub(r'#.*', '', webcont)
webcont.strip()

hostsfile = get_hostsfile()

hosts = open(hostsfile, 'r')
flag = 1
hh = ""
for h in hosts.readlines():
    if h.startswith(BAKKA_START):
        flag = 0
    if h.startswith(BAKKA_END):
        flag = 1
    if flag and not h.startswith(BAKKA_END):
        hh = hh + h

writefile(hostsfile,hh+"\n"+BAKKA_START+"\n"+webcont+"\n"+BAKKA_END)

print BAKKA_START
print webcont
print BAKKA_END
print ""
print "Done.\n"
