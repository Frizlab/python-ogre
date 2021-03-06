#!/bin/python
# Part of the Python-Ogre installation 

import os,sys,urllib
import logging
import logger
print sys.argv

log = logging.getLogger('PO.' + sys.argv[0].split('.')[0])
log.info ("Retrieving Sources")

root = os.getcwd() # our base location

RetrieveUrls = [
["wget", "http://developer.download.nvidia.com/cg/Cg_1.5/1.5.0/0022/Cg-1.5_Aug2007_x86.tar.gz",''], 
["wget", "http://www.cmake.org/files/v2.4/cmake-2.4.6-Linux-i386.tar.gz",''], 
["wget", "http://prdownloads.sourceforge.net/freeimage/FreeImage393.zip",''], 
["wget", "http://prdownloads.sourceforge.net/crayzedsgui/CEGUI-0.5.0b.tar.gz",''],  
["wget", "http://prdownloads.sourceforge.net/wgois/ois-1.0RC1.tar.gz",''],  
["wget", "http://prdownloads.sourceforge.net/boost/boost-jam-3.1.13-1-linuxx86.tgz",''], 
["wget", "http://prdownloads.sourceforge.net/scons/scons-0.96.96.tar.gz",''], 
["wget", "http://downloads.sourceforge.net/scons/scons-0.97.0d20070918.tar.gz",''],
["wget", "http://prdownloads.sourceforge.net/ogre/ogre-linux_osx-v1-4-5.tar.bz2",''], 
["wget", "http://prdownloads.sourceforge.net/opende/ode-src-0.8.zip",''], 
["wget", "http://downloads.sourceforge.net/boost/boost_1_34_0.tar.bz2",''], 
["wget", "http://www.newtondynamics.com/downloads/newtonLinux-1.53.tar.gz",''], 
["wget", "http://www.openal.org/openal_webstf/downloads/openal-0.0.8.tar.gz",''],
["wget", "http://www.openal.org/openal_webstf/downloads/freealut-1.1.0.tar.gz", ''],
["wget", "http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz",''],
["wget", "http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz",''],
["wget", "http://downloads.sourceforge.net/zziplib/zziplib-0.13.49.tar.bz2",''],
['svn', "https://pygccxml.svn.sourceforge.net/svnroot/pygccxml pygccxml",""],
['svn', "https://python-ogre.svn.sourceforge.net/svnroot/python-ogre/trunk/python-ogre python-ogre",""],
['cvs', "-z3 -q -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML co gccxml",""],
['cvs', "-z3 -q -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/ogreode",""],
['cvs', "-z3 -q -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/ogrenewt",""] 
]

try:
    os.mkdir( os.path.join(root, 'downloads') )
except:
    pass

for mode, url, extra in RetrieveUrls:

    if mode == "wget": ## do a standard http get on the file
        os.chdir ( os.path.join(root, 'downloads') )
        destfile = url.split('/')[-1]
        log.info ("Retrieving " + destfile + " from " + url )
        (filename, headers) = urllib.urlretrieve (url, destfilename)
        os.chdir ( root )

RetrieveSvns = [
"https://pygccxml.svn.sourceforge.net/svnroot/pygccxml pygccxml",
"https://python-ogre.svn.sourceforge.net/svnroot/python-ogre/trunk/python-ogre python-ogre",

cvs -z3 -q -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML co gccxml 
echo " -- getting ogreode"
cvs -z3 -q -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/ogreode
echo " -- getting ogrenewt"
cvs -z3 -q -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/ogrenewt 
echo ""
echo "======= Sources Retrieved ======="
echo ""


