#!/bin/bash
# Part of the Python-Ogre installation
if [ $DARWIN == 1 ] then
    pushd $DOWNLOADS
    curl -o wget-1.10.2.tar.gz http://ftp.gnu.org/pub/gnu/wget/wget-1.10.2.tar.gz
    curl -o subversion-client-1.3.1.dmg http://metissian.com/downloads/macosx/subversion/subversion-client-1.3.1.dmg
    curl -o python-2.5-macosx.dmg http://www.pythonmac.org/packages/py25-fat/dmg/python-2.5-macosx.dmg
    
    popd
    tar zxf $DOWNLOADS/wget-1.10.2.tar.gz
    pushd wget-1.10.2
    ./configure --prefix='/usr'
    make
    sudo make install
    popd
    sudo open $DOWNLOADS/subversion-client-1.3.1.dmg
    sudo open $DOWNLOADS/python-2.5-macosx.dmg
else 

sudo apt-get install g++ libfreetype6-dev libpcre3-dev autoconf automake1.9 libgl1-mesa-dev libx11-dev \
libglut3-dev libtool libxaw7-dev libxxf86vm-dev patch \
libxrandr-dev cvs subversion libxt-dev libexpat1-dev python python-dev \
python-celementtree libglew-dev 

if