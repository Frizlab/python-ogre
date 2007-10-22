#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh

echo " == extracting ==" 
echo " -- Unpacking OGRE"
tar jxf $DOWNLOADS/ogre-linux_osx-v1-4-5.tar.bz2
tar jxf $DOWNLOADS/zziplib-0.13.49.tar.bz2
echo " -- Unpacking Boost"
tar jxf $DOWNLOADS/boost_1_34_0.tar.bz2
echo " -- Unpacking CEGUI"
tar zxf $DOWNLOADS/CEGUI-0.5.0b.tar.gz
echo " -- Unpacking OIS"
tar zxf $DOWNLOADS/ois-1.0RC1.tar.gz
echo " -- Unpacking FreeImage"
unzip -q -o $DOWNLOADS/FreeImage393.zip
echo " -- Unpacking Newton"
tar zxf $DOWNLOADS/newtonLinux-1.53.tar.gz
echo " -- Unpacking ODE"
unzip -q -o $DOWNLOADS/ode-src-0.9.zip
echo " -- Unpacking OgreAL Support"
tar zxf $DOWNLOADS/libogg-1.1.3.tar.gz
tar zxf $DOWNLOADS/libvorbis-1.2.0.tar.gz
tar zxf $DOWNLOADS/freealut-1.1.0.tar.gz
tar zxf $DOWNLOADS/openal-0.0.8.tar.gz

echo " -- Unpacking CG"
pushd $ROOT
echo "Installing Cg"
tar zxf $DOWNLOADS/Cg-1.5_Aug2007_x86.tar.gz
popd
#
# install bjam
#
echo " -- Unpacking bjam"
rm -r -f boost-jam-3.1.13-1-linuxx86
tar -xvzf $DOWNLOADS/boost-jam-3.1.13-1-linuxx86.tgz --overwrite 
mv boost-jam-3.1.13-1-linuxx86/bjam $PREFIX/bin

# 
# install cmake
#
echo " -- Unpacking cmake"
tar -xvzf $DOWNLOADS/cmake-2.4.6-Linux-i386.tar.gz --overwrite 
cd cmake-2.4.6-Linux-i386 
cp * $PREFIX -r 
cd .. 


echo " -- Patching OgreOde"
cd ogreaddons/ogreode
patch -s -N -i ../../python-ogre/patch/ogreode.patch -p0 
cd $INSTALL_DIR
echo " -- Patching OgreNewt"
cd ogreaddons/ogrenewt 
patch -s -N -i ../../python-ogre/patch/ogrenewt.patch -p0 
cp SConscript OgreNewt_Main 
rm -r ./OgreNewt_Main/inc/boost
cd $INSTALL_DIR

# 
# Install Newton 
#
echo " -- Patching and Installing Newton "
# patch to include stddef.h as size_t isn't defined in Newton.h
patch -s -i ./python-ogre/patch/Newton.patch -p0 
cp newtonSDK/sdk/Newton.h $PREFIX/include
cp newtonSDK/sdk/*.a $PREFIX/lib
cp newtonSDK/sdk/*.a ogreaddons/ogrenewt

echo " -- Patching Boost"
chmod -R +rw boost_1_34_0
cp -r python-ogre/boost/* boost_1_34_0
sed -i s/BJAM_CONFIG=\"\"/BJAM_CONFIG=release/ boost_1_34_0/configure
sed -i s/'BOOST_PYTHON_MAX_ARITY 15'/'BOOST_PYTHON_MAX_ARITY 19'/ boost_1_34_0/boost/python/detail/preprocessor.hpp

echo " -- Patching Ogre"
patch -s -N -i ./python-ogre/patch/ogre.patch -p0 

echo " -- Patching CEGUI"
pushd CEGUI-0.5.0
patch -s -N -i ../python-ogre/patch/cegui.patch -p0
echo "EMPTY" >>./INSTALL
echo "EMPTY" >>./NEWS
popd
echo " -- Unpacking Bullet"
tar zxf $DOWNLOADS/bullet-2.63final.tgz
echo
echo "== Finished Extraction and Patching =="
echo


