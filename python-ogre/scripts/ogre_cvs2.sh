#!/bin/bash

# May 17th -- extend to pathc ogreode and ogrenewt, build code for both, compile ode, ogreode (not ogrenewt yet)
# Changes late May 15th - Corrected the environment variables definitions on the setup.sh file.
# Changes May 15th - added Ode, Newton, Ogreode - adjusted for Boost 1.34 instead of CVS version
#
# Copyright (C) 2007
# Renato Cunha <renato@tampycross.org>
# Andy Miller
# Konrad Kleine
#
# http://www.python-ogre.org
#
# This script is based on Andy Miller's script, which was based on Konrad Kleine's
# one. Since there wasn't a license associated, I decided to apply this script
# to the GPL terms.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program on the LICENSE file; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02

# sudo ldconfig ~/development/root/usr/lib

##
## This script installs all packages and libraries needed to build Python-Ogre
## It can be run as root, but you'll be on your own. You really should
## run it as a normal user.
##
## PLEASE NOTE: You might need to interact with this program by entering
## the root password and things like that.
##
## Another note: Unlike this script's predecessors I am going to assume you have
## a working build environment (with gcc, g++, svn etc.) If this is not the case,
## please install the following packages:
## 
# sudo apt-get install g++ libfreetype6-dev libpcre3-dev autoconf automake1.9 libgl1-mesa-dev libx11-dev \
# libglut3-dev libtool libxaw7-dev libxxf86vm-dev \
# libxrandr-dev cvs subversion libxt-dev libexpat1-dev python python-dev \
# python-celementtree libglew-dev libzzip-dev scons
##

echo -e "Python-Ogre Retrieve script\n"
echo -e "This script will retrieve and build all the necessary software for\
 Ogre and Python-Ogre\n"

if [ $UID -eq 0 ]
then
	echo
	echo "It's not good practice to start this script as root!"
	echo 
	exit 1
fi

echo "Do you want to proceed? (Y/n)" 

read answer

if [ "$answer" == "n" ]
then
	exit 0
fi

INSTALL_DIR=`pwd`
ROOT=$INSTALL_DIR/root
PREFIX=$ROOT/usr
DOWNLOADS=$INSTALL_DIR/downloads
WGETARGS="-c -nc --tries=3 --random-wait"
WGET="`which wget` $WGETARGS"

CONTROLretrieve="n"
CONTROLextract="n"
CONTROLpatchogre="n"
CONTROLbuildfreeimage="n"
CONTROLbuildutils="n"
CONTROLbuildogre="n"
CONTROLgeneratecode="y"
CONTROLbuildpythonogre="n"

#
# Please don't edit below this point unless you know what you are doing
#

TEMPFILE=`mktemp`
cat << EOF > $TEMPFILE
#!/usr/bin/python
import sys
version = sys.version.split('.')
print "%s.%s" % (version[0], version[1])
EOF
PYTHONVERSION=`python $TEMPFILE`
rm $TEMPFILE

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$PREFIX/lib/pkgconfig
export LD_LIBRARY_PATH=$PREFIX/lib
export CFLAGS="-I$PREFIX/include -L$PREFIX/lib"
export CXXFLAGS=$CFLAGS
export PATH=$PATH:$PREFIX/bin
export PYTHONPATH=$PREFIX/lib/python$PYTHONVERSION/site-packages

cat << EOF > setup.sh
export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:$PREFIX/lib/pkgconfig"
export LD_LIBRARY_PATH="$PREFIX/lib"
export CFLAGS="-I$PREFIX/include -L$PREFIX/lib"
export CXXFLAGS="$CFLAGS"
export PATH="$PATH:$PREFIX/bin"
export PYTHONPATH="$PREFIX/lib/python$PYTHONVERSION/site-packages"
EOF

mkdir -p $PREFIX
mkdir -p $PREFIX/lib
mkdir -p $PREFIX/include
mkdir -p $DOWNLOADS

if [ "$CONTROLretrieve" == "y" ]
then
{
	echo "Retrieving sources" &&\
#
# Get all sources
#
pushd $DOWNLOADS && \
$WGET http://developer.download.nvidia.com/cg/Cg_1.5/1.5.0/0022/Cg-1.5_Aug2007_x86.tar.gz && \
$WGET http://www.cmake.org/files/v2.4/cmake-2.4.6-Linux-i386.tar.gz && \
$WGET http://prdownloads.sourceforge.net/freeimage/FreeImage393.zip  && \
$WGET http://prdownloads.sourceforge.net/crayzedsgui/CEGUI-0.5.0b.tar.gz  && \
$WGET http://prdownloads.sourceforge.net/wgois/ois-1.0RC1.tar.gz  && \
$WGET http://prdownloads.sourceforge.net/boost/boost-jam-3.1.13-1-linuxx86.tgz && \
$WGET http://prdownloads.sourceforge.net/scons/scons-0.96.96.tar.gz && \
$WGET http://prdownloads.sourceforge.net/ogre/ogre-linux_osx-v1-4-4.tar.bz2 && \
$WGET http://prdownloads.sourceforge.net/opende/ode-src-0.8.zip && \
$WGET http://downloads.sourceforge.net/boost/boost_1_34_0.tar.bz2 && \
$WGET http://www.newtondynamics.com/downloads/newtonLinux-1.53.tar.gz && \
# $WGET http://www.openal.org/openal_webstf/downloads/openal-0.0.8-1.i586.rpm && 
$WGET http://www.openal.org/openal_webstf/downloads/openal-0.0.8.tar.gz
# $WGET http://www.openal.org/openal_webstf/downloads/freealut-1.1.0-1.i586.rpm && 
$WGET http://www.openal.org/openal_webstf/downloads/freealut-1.1.0.tar.gz &&\
popd && \
echo && \
echo "Please just press Enter when you are asked for a password" && \
echo && \
svn co https://ogreal.svn.sourceforge.net/svnroot/ogreal/trunk/OgreAL-Eihort OgreAL 
svn co https://pygccxml.svn.sourceforge.net/svnroot/pygccxml pygccxml 
svn co https://python-ogre.svn.sourceforge.net/svnroot/python-ogre/trunk/python-ogre python-ogre
svn co --username anyone --password nopassword svn://www.stellarcommand.com/QuickGUI/trunk/src/ quickgui
#cvs -z3 -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML co gccxml 
cvs -z3 -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/ogreode
cd ogreaddons/ogreode
patch -i ../../python-ogre/patch/ogreode.patch -p0 
cd ../..
cvs -z3 -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/ogrenewt 
cd ogreaddons/ogrenewt 
patch -i ../../python-ogre/patch/ogrenewt.patch -p0 
cp SConscript OgreNewt_Main 
rm -r ./OgreNewt_Main/inc/boost
cd ../..
cvs -z3 -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/videoplugin; } 
fi

if [ "$CONTROLextract" == "y" ]
then
{
echo "Unpacking OGRE"
tar jxf $DOWNLOADS/ogre-linux_osx-v1-4-4.tar.bz2
echo "Unpacking (and Patching) Boost"
tar jxf $DOWNLOADS/boost_1_34_0.tar.bz2
cp -r python-ogre/boost/* boost_1_34_0
echo "Unpacking CEGUI"
tar zxf $DOWNLOADS/CEGUI-0.5.0b.tar.gz
echo "Unpacking OIS"
tar zxf $DOWNLOADS/ois-1.0RC1.tar.gz
echo "Unpacking FreeImage"
unzip -o $DOWNLOADS/FreeImage393.zip
echo "Unpacking Newton"
tar zxf $DOWNLOADS/newtonLinux-1.53.tar.gz
echo "Unpacking ODE"
unzip -o $DOWNLOADS/ode-src-0.8.zip ; }
fi

if [ "$CONTROLpatchogre" == "y" ]
then
#
# patch ogre etc
#
{ echo "Patching Ogre" &&\
patch -i ./python-ogre/patch/ogre.patch -p0 ; }
fi



if [ "$CONTROLbuildfreeimage" == "y" ]
then
{

#
# Install FreeImage (For image loading in Ogre)
#
	pushd FreeImage
	make
cat << EOF > freeimage.patch
--- Makefile.gnu	2006-12-20 15:22:08.000000000 -0200
+++ Makefile.gnu.new	2007-05-12 11:11:54.000000000 -0300
@@ -58,11 +58,11 @@
 	\$(CC) -s -shared -Wl,-soname,\$(VERLIBNAME) -o \$@ \$(MODULES) \$(LIBRARIES)
 
 install:
-	install -m 644 -o root -g root \$(HEADER) \$(INCDIR)
-	install -m 644 -o root -g root \$(STATICLIB) \$(INSTALLDIR)
-	install -m 755 -o root -g root \$(SHAREDLIB) \$(INSTALLDIR)
-	ln -sf \$(SHAREDLIB) \$(INSTALLDIR)/\$(VERLIBNAME)
-	ln -sf \$(VERLIBNAME) \$(INSTALLDIR)/\$(LIBNAME)	
+	install -m 644 \$(HEADER) \$(DESTDIR)\$(INCDIR)
+	install -m 644 \$(STATICLIB) \$(DESTDIR)/\$(INSTALLDIR)
+	install -m 755 \$(SHAREDLIB) \$(DESTDIR)/\$(INSTALLDIR)
+	ln -sf \$(DESTDIR)/\$(SHAREDLIB) \$(DESTDIR)/\$(INSTALLDIR)/\$(VERLIBNAME)
+	ln -sf \$(DESTDIR)/\$(VERLIBNAME) \$(DESTDIR)/\$(INSTALLDIR)/\$(LIBNAME)	
 	ldconfig
 
 clean:
EOF
	patch -p0 < freeimage.patch
	make DESTDIR=$ROOT install # On Freeimage's case, this is the right path
	popd; }
fi


#  cegui gets patched further down

if [ "$CONTROLbuildogre" == "y" ]
then
#
# Install Nvidia's Cg-Toolkit (Needed for shader support in Ogre)
#
{
	pushd $ROOT
	echo "Installing Cg"
	tar zxvf $DOWNLOADS/Cg-1.5_Aug2007_x86.tar.gz
	popd


	#
	# Install CEGUI (Crazy Eddie's GUI)
	#
	pushd CEGUI-0.5.0
        aclocal
	patch -i ../python-ogre/patch/cegui.patch -p0
        echo "EMPTY" >>./INSTALL
        echo "EMPTY" >>./NEWS
        automake
	./configure --prefix=$PREFIX --enable-freeimage=yes --disable-samples --without-ogre-renderer
	make
	make install
	popd

	#
	# Install OIS (Object Oriented Input System)
	#
	pushd ois-1.0RC1
	./bootstrap
	./configure --prefix=$PREFIX
	make
	make install
	popd

	# 
	# Install Newton 
	#
        cp newtonSDK/sdk/Newton.h ogreaddons/ogrenewt/OgreNewt_Main/inc
        cp newtonSDK/sdk/*.a ogreaddons/ogrenewt

	# 
	# Install and build ODE
	#
	echo "Building ODE"
	pushd ode-0.8
	chmod +x autogen.sh
 	./autogen.sh
 	./configure --prefix=$PREFIX
	make
	make install
	popd

	#
	# Install Ogre (Object Oriented Graphics Rendering Engine)
	#
	pushd ogrenew 
	./bootstrap
	./configure --prefix=$PREFIX
	make
	make install
	popd 

	#
	# Install and build OgreOde
	#
	pushd ogreaddons/ogreode
 	chmod +x autogen.sh
 	./autogen.sh
 	./configure  --prefix=$PREFIX
 	make
 	make install
 	popd ; }

	#
	# Install and build OgreNewt
	#
	pushd ogreaddons/ogrenewt/OgreNewt_Main/inc
cat << EOF > newt.patch
--- Newton.h
+++ Newton.h
//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// One and only header file.
//********************************************************************

#ifndef NEWTON_H__736495_6495_076__INCLUDED_
#define NEWTON_H__736495_6495_076__INCLUDED_
+#include "Ogre.h"

// force all not visual studio platform to be in library form
EOF
	patch -p0 < newt.patch
        popd

        pushd ogreaddons/ogrenewt
	scons prefix=$PREFIX boost=$PREFIX/include/boost-1_34 install
	popd 	
	
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
	cp * $PREFIX -r && \
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
	mv boost-jam-3.1.13-1-linuxx86/bjam $PREFIX/bin

	# 
	# build and install boost
	#
	cd boost_1_34_0
	./configure --with-libraries=python --prefix=$PREFIX
cat << EOF > boost.patch
--- Makefile	2007-05-12 12:10:17.000000000 -0300
+++ Makefile.new	2007-05-12 12:10:52.000000000 -0300
@@ -1,6 +1,4 @@
-BJAM=./tools/jam/src/bin.linuxx86
-don't know how to make trie.c
-...skipped bjam for lack of trie.c.../bjam
+BJAM=bjam
 BJAM_CONFIG=release
 PREFIX=/usr/local
 EPREFIX=/usr/local
EOF
	patch -p0 < boost.patch
	sed -i s/'BOOST_PYTHON_MAX_ARITY 15'/'BOOST_PYTHON_MAX_ARITY 19'/ boost/python/detail/preprocessor.hpp
	make
	make install
	cd $INSTALL_DIR

	## Scons
	rm -rf scons-0.96.96 &&\
	tar -xvzf $DOWNLOADS/scons-0.96.96.tar.gz --overwrite && \
	cd scons-0.96.96 && \
	python setup.py install --prefix=$PREFIX && \
	cd .. &&\

	# py++
	cd pygccxml/pygccxml_dev &&\
	python setup.py install --prefix=$PREFIX &&\
	cd ../pyplusplus_dev &&\
	python setup.py install --prefix=$PREFIX &&\
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
cd quickgui && \
python generate_code.py > build.out && \
cd .. &&\
cd ogreode && \
python generate_code.py > build.out && \
cd .. &&\
#cd ogrenewt && \
#python generate_code.py > build.out && \
#cd .. &&\
cd ode && \
python generate_code.py > build.out && \
cd .. &&\
cd cegui && \
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
scons PROJECTS=ogre,ois,quickgui,cegui,ogrerefapp&&\
cd .. ;}
fi

# 
# install
#

# 
# check/update  ldconfig
#
