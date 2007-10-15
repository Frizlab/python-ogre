#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh

echo " === Building Tools ==="

# 
# build GCC_XML
#
echo " -- building GCC_XML" 
mkdir -p gccxml-build  
cd gccxml-build 
cmake ../gccxml -DCMAKE_INSTALL_PREFIX:PATH=$PREFIX 
make 
make install 
cd .. 


# 
# build and install boost
#
echo " -- building boost" 
cd boost_1_34_0
./configure --with-libraries=python --prefix=$PREFIX
make
make install
cd $INSTALL_DIR

## Scons
#echo " -- installing SCONs" 
#rm -rf scons-0.96.96 
#tar -xvzf $DOWNLOADS/scons-0.96.96.tar.gz --overwrite 
#cd scons-0.96.96 
#python setup.py install --prefix=$PREFIX 
#cd .. 

echo " -- installing SCONs" 
rm -rf scons-0.97.0d20070918 
tar -xvzf $DOWNLOADS/scons-0.97.0d20070918.tar.gz --overwrite 
cd scons-0.97.0d20070918 
python setup.py install --prefix=$PREFIX 
cd .. 


# py++
echo " -- installing PY++" 
cd pygccxml/pygccxml_dev 
python setup.py install --prefix=$PREFIX 
cd ../pyplusplus_dev 
python setup.py install --prefix=$PREFIX 
cd $INSTALL_DIR 

echo
echo " ==== Done building tools ===="
