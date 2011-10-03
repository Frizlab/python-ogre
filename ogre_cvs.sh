#!/bin/bash

################################################################################
#
# Installs all packages and libraries needed to build Python-Ogre
#
# Note this script should be run from a home/subdirectory (ie ./development)
# 
# All output lis are places into ./local
#
#
# This script has been written and tested on for Ubuntu linux version 7.04 (x86, AI 32)s
# 
# PLEASE NOTE: You might need to interact with this program by entering
#              the root password and things like that.
#
#
# Based upon original script from Konrad Kleine
#
################################################################################
echo
echo  "      Python-Ogre Retrieve script"
echo
echo "   This script will retrieve and build all the necessary software for Ogre and Python-Ogre"

echo "  *** Add <this dir./local/lib to the /etc/ld.so.conf file ***  "
echo "  *** and run sudo ldconfig  BEFORE running this script "
echo

if [ $UID -eq 0 ]
then
	echo
	echo "It's not good practice to start this script as root!"
	echo 
	exit 1
fi

echo 
echo
echo "Do you want to proceed? (Y/n)" 

read answer

if [ "$answer" == "n" ]
then
	exit 0
fi

#
# Defines !!
#
#
INSTALL_DIR=$PWD
PREFIX=$INSTALL_DIR/local
DOWNLOADS=$INSTALL_DIR/downloads
#
# set these to 'y' depending on what you want to run
#
CONTROLapt="n"
CONTROLretrieve="n"
CONTROLpatchogre="n"
CONTROLbuildogre="y"
CONTROLbuildutils="y"
CONTROLgeneratecode="y"
CONTROLbuildpythonogre="y"

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$PREFIX/lib/pkgconfig
mkdir -p $PREFIX
mkdir -p $PREFIX/lib
mkdir -p $PREFIX/include
mkdir -p $DOWNLOADS

#
# Install required packages
#
echo $CONTROLapt
if [ "$CONTROLapt" == "y" ]
then
{ echo "getting packages" &&\
echo && \
echo "Enter your (root) password." &&\
echo &&\
sudo apt-get install g++ libfreetype6-dev libpcre3-dev autoconf automake1.9 \
libgl1-mesa-dev libx11-dev libglut3-dev libtool libxaw7-dev libzzip-dev \
zziplib-bin libxxf86vm-dev libxrandr-dev cvs subversion libxt-dev libexpat1-dev \
python python-dev python-celementtree libglew-dev; }
fi


if [ "$CONTROLretrieve" == "y" ]
then
{ echo "Retrieveing source" &&\
#
# Get all sources
#
cd $DOWNLOADS && \
#wget http://developer.download.nvidia.com/cg/Cg_1.5/1.5.0/Cg-1.5_x86.tar.gz && 
wget http://developer.download.nvidia.com/cg/Cg_1.5/1.5.0/0019/Cg-1.5_Feb2007_x86.tar.gz && \
wget http://heanet.dl.sourceforge.net/sourceforge/freeimage/FreeImage393.zip  && \
wget http://heanet.dl.sourceforge.net/sourceforge/crayzedsgui/CEGUI-0.5.0b.tar.gz  && \
wget http://heanet.dl.sourceforge.net/sourceforge/wgois/ois-1.0RC1.tar.gz  && \
wget http://www.cmake.org/files/v2.4/cmake-2.4.6-Linux-i386.tar.gz && \
wget http://downloads.sourceforge.net/boost/boost-jam-3.1.13-1-linuxx86.tgz && \
wget http://prdownloads.sourceforge.net/scons/scons-0.96.96.tar.gz && \
cd .. && \
echo && \
echo "JUST HIT ENTER WHEN YOUR'RE BEING ASKED FOR A PASSWORD!" && \
echo && \
cvs -d:pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre login && \
cvs -z3 -d:pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogrenew && \
svn co https://pygccxml.svn.sourceforge.net/svnroot/pygccxml pygccxml && \
svn co https://svn.python-ogre.org/trunk/python-ogre/ python-ogre && \
cvs -d:pserver:anonymous@boost.cvs.sourceforge.net:/cvsroot/boost login && \
cvs -z3 -d:pserver:anonymous@boost.cvs.sourceforge.net:/cvsroot/boost co -P boost && \
chmod +rwX -R boost/* &&\
cvs -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML login && \
cvs -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML co gccxml && \
svn co https://opende.svn.sourceforge.net/svnroot/opende/trunk && \
mv trunk ODE &&\
cvs -z3 -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/ogreode && \
cvs -z3 -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/ogrenewt ; } 
fi


if [ "$CONTROLpatchogre" == "y" ]
then
#
# patch ogre etc
#
{ echo "Patching Boost and Ogre" &&\
cp -r python-ogre/boost/* boost &&\
cd ogrenew &&\
patch -i ../python-ogre/patch/ogre.patch -p0 &&\
cd .. ; }
fi

#  cegui gets patched further down

if [ "$CONTROLbuildogre" == "y" ]
then
#
# Install Nvidia's Cg-Toolkit (Needed for shader support in Ogre)
#
{ echo && \
echo "Enter your (root) password." && \
echo && \
sudo cp $DOWNLOADS/Cg-1.5_Feb2007_x86.tar.gz / && \
cd / && \
sudo tar -xvzf Cg-1.5_Feb2007_x86.tar.gz && \
sudo rm Cg-1.5_Feb2007_x86.tar.gz && \
cd $INSTALL_DIR && \

#
# Install FreeImage (For image loading in Ogre)
#
unzip -o $DOWNLOADS/FreeImage393.zip && \
cd FreeImage && \
make -j 4 && \
echo && \
echo "Enter your (root) password." && \
echo && \
sudo make install && \
cd .. && \

#
# Install CEGUI (Crazy Eddie's GUI)
#
tar -xvzf $DOWNLOADS/CEGUI-0.5.0b.tar.gz --overwrite && \
cd CEGUI-0.5.0 && 
patch -i ../python-ogre/patch/cegui.patch -p0 &&\
./configure --prefix=$PREFIX --enable-freeimage=yes --disable-samples --without-ogre-renderer && \
# --prefix=$PREFIX 
make -j 4 && \
echo && \
echo "Enter your (root) password." && \
echo && \
make install && \
cd .. && \

#
# Install OIS (Object Oriented Input System)
#
tar -xvzf $DOWNLOADS/ois-1.0RC1.tar.gz && \
cd ois-1.0RC1 && \
./bootstrap && \
./configure  --prefix=$PREFIX&& \
# --prefix=$PREFIX
#make -j 4 && 
make && \
echo && \
echo "Enter your (root) password." && \
echo && \
make install && \
cd .. && \

#
# Install Ogre (Object Oriented Graphics Rendering Engine)
#
cd ogrenew && \
./bootstrap && \
./configure  --prefix=$PREFIX && \   
# --prefix=$PREFIX   --with-platform=GLX --enable-debug=full
#make -j 4 && 
make  && \
echo && \
echo "Enter your (root) password." && \
echo && \
make install && \
cd .. ; }
fi


if [ "$CONTROLbuildutils" == "y" ]
then
# 
# install cmake
#
{ echo && \
echo "Building tools" &&\
tar -xvzf $DOWNLOADS/cmake-2.4.6-Linux-i386.tar.gz --overwrite && \
cd cmake-2.4.6-Linux-i386 && \
sudo cp * /usr -r && \
cd .. && \

# 
# build GCC_XML
#
mkdir -p gccxml-build  && \
cd gccxml-build && \
cmake ../gccxml -DCMAKE_INSTALL_PREFIX:PATH=$PREFIX && \
make && \
make install && \
cd .. &&\

#
# install bjam
#
rm -r -f boost-jam-3.1.13-1-linuxx86
tar -xvzf $DOWNLOADS/boost-jam-3.1.13-1-linuxx86.tgz --overwrite && \

# 
# build and install boost
#
cd boost/libs/python/build &&\
../../../../boost-jam-3.1.13-1-linuxx86/bjam release --v2 &&\
cd $INSTALL_DIR &&\
cp boost/bin.v2/libs/python/build/gcc-4.1.2/release/libboost_python-gcc41-1_35.so.1.35.0 $PREFIX/lib/libboost_python-gcc41-1_35.so &&\

## Scons
sudo rm -rf scons-0.96.96 &&\
tar -xvzf $DOWNLOADS/scons-0.96.96.tar.gz --overwrite && \
cd scons-0.96.96 && \
python setup.py build &&\
sudo python setup.py install &&\
cd .. &&\

# py++
cd pygccxml/pygccxml_dev &&\
sudo python setup.py install &&\
cd ../pyplusplus_dev &&\
python setup.py build &&\
sudo python setup.py install &&\
cd $INSTALL_DIR ; }
fi


#
# create code
#
if [ "$CONTROLgeneratecode" == "y" ]
then
{ echo "generating code" && \
cd python-ogre/code_generators/ogre && \
python generate_code.py > build.out && \
cd .. &&\
cd ogrerefapp && \
python generate_code.py > build.out && \
cd .. &&\
cd ois && \
python generate_code.py > build.out && \
cd .. &&\
cd CEGUI && \
python generate_code.py > build.out && \
cd $INSTALL_DIR ; }
fi
#
# compile code
#
if [ "$CONTROLbuildpythonogre" == "y" ]
then
{ echo "building python-ogre" && \
cd python-ogre &&\
scons PROJECTS=ogre,ois,ogrerefapp,cegui &&\
# ,ois,cegui
cd .. ;}
fi

# 
# install
#

# 
# check/update  ldconfig
#
