#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh

echo "Retrieving sources" 
#
# Get all sources
#
pushd $DOWNLOADS 
echo " -- Getting bundles"
$WGET http://developer.download.nvidia.com/cg/Cg_1.5/1.5.0/0022/Cg-1.5_Aug2007_x86.tar.gz 
$WGET http://www.cmake.org/files/v2.4/cmake-2.4.6-Linux-i386.tar.gz 
$WGET http://prdownloads.sourceforge.net/freeimage/FreeImage393.zip  
$WGET http://prdownloads.sourceforge.net/crayzedsgui/CEGUI-0.5.0b.tar.gz  
$WGET http://prdownloads.sourceforge.net/wgois/ois-1.0RC1.tar.gz  
$WGET http://prdownloads.sourceforge.net/boost/boost-jam-3.1.13-1-linuxx86.tgz 
##$WGET http://prdownloads.sourceforge.net/scons/scons-0.96.96.tar.gz 
$WGET http://downloads.sourceforge.net/scons/scons-0.97.0d20070918.tar.gz
$WGET http://prdownloads.sourceforge.net/ogre/ogre-linux_osx-v1-4-5.tar.bz2 
$WGET http://prdownloads.sourceforge.net/opende/ode-src-0.9.zip 
$WGET http://downloads.sourceforge.net/boost/boost_1_34_0.tar.bz2 
$WGET http://www.newtondynamics.com/downloads/newtonLinux-1.53.tar.gz 
$WGET http://www.openal.org/openal_webstf/downloads/openal-0.0.8.tar.gz
$WGET http://www.openal.org/openal_webstf/downloads/freealut-1.1.0.tar.gz 
$WGET http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz
$WGET http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz
$WGET http://downloads.sourceforge.net/zziplib/zziplib-0.13.49.tar.bz2
##$WGET http://devfiles.ageia.com/PhysX_2.7.2_SDK_CoreLinux_deb.tar.gz?
$WGET http://downloads.sourceforge.net/bullet/bullet-2.63final.tgz
popd 
cd $INSTALL_DIR
echo " -- getting pygccxml"
svn co https://pygccxml.svn.sourceforge.net/svnroot/pygccxml pygccxml 
echo " -- getting python-ogre"
svn co https://python-ogre.svn.sourceforge.net/svnroot/python-ogre/trunk/python-ogre python-ogre
echo " -- getting gccxml"
cvs -z3 -q  -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML  co -D "1 Oct 2007" gccxml 
echo " -- getting ogreode"
cvs -z3 -q -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/ogreode
echo " -- getting ogrenewt"
cvs -z3 -q -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -P ogreaddons/ogrenewt 
echo " -- getting nxogre"
svn co  http://latest.nxogre.org/nxogre3/ nxogre
echo ""
echo "======= Sources Retrieved ======="
echo ""


