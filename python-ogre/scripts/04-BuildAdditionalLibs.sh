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
