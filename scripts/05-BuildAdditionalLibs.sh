#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh

echo " == Building additional libraries == "
#
# Install and build OgreOde
#
echo "  -- Building Ogreode"
pushd ogreaddons/ogreode
chmod +x autogen.sh
./autogen.sh
./configure  --prefix=$PREFIX
make
make install
popd 

#
# Install and build OgreNewt
#
echo " -- Building OgreNewt"
pushd ogreaddons/ogrenewt
scons prefix=$PREFIX boost=$PREFIX/include/boost-1_34 install
popd 	

#
# OgreAl
#
echo " -- Building OgreAL - libogg"
pushd libogg-1.1.3
./configure --prefix=$PREFIX
make
make install
popd

echo " -- Building OgreAL - libvorbis"
pushd libvorbis-1.2.0
./configure --prefix=$PREFIX
make
make install
popd

echo " -- Building OgreAL - openal"
pushd openal-0.0.8
./autogen.sh
./configure  --prefix=$PREFIX
make
make install
popd 

echo " -- Building OgreAL - freealut"
pushd freealut-1.1.0
./autogen.sh
./configure  --prefix=$PREFIX
## ugly hack as the make files in freealut don't really handle a non standard prefix
cp $PREFIX/include/AL/* ./include/AL
make
make install
popd 

#
# Bullet
# 
echo " -- Building Bullet --"
pushd bullet-2.63final
cmake . -DCMAKE_INSTALL_PREFIX:PATH=$PREFIX
make
find . -name *.a -execdir cp {} $PREFIX/lib \;
popd

