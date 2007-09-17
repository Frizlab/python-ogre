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
echo " -- installing SCONs" 
rm -rf scons-0.96.96 
tar -xvzf $DOWNLOADS/scons-0.96.96.tar.gz --overwrite 
cd scons-0.96.96 
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
