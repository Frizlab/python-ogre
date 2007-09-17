#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh

echo " == Building base libraries == "
#
# Install CEGUI (Crazy Eddie's GUI)
#
echo " -- Building CEGUI"
pushd CEGUI-0.5.0
aclocal
#patch -q -i ../python-ogre/patch/cegui.patch -p0
#echo "EMPTY" >>./INSTALL
#echo "EMPTY" >>./NEWS
automake
./configure --prefix=$PREFIX --enable-freeimage=yes --disable-samples --without-ogre-renderer
make
make install
popd

#
# Install OIS (Object Oriented Input System)
#
echo " -- Builing OIS"
pushd ois-1.0RC1
./bootstrap
./configure --prefix=$PREFIX
make
make install
popd


# 
# Install and build ODE
#
echo " -- Building ODE"
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
echo " -- Building Ogre"
pushd ogrenew 
./bootstrap
./configure --prefix=$PREFIX
make
make install
popd 

echo
echo " ==== Finished building base libraries ===="
