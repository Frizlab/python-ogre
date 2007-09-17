#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh

echo "=== generating code ===" 
echo ' -- creating OGRE code'
cd python-ogre/code_generators/ogre 
python generate_code.py > build.out 
cd .. 
echo ' -- creating OGRErefapp code'
cd ogrerefapp 
python generate_code.py > build.out 
cd .. 
#echo ' -- creating OgreAL code'
#cd ogreal 
#python generate_code.py > build.out 
#cd .. 
echo ' -- creating quickgui code'
cd quickgui 
python generate_code.py > build.out 
cd .. 
echo ' -- creating OIS code'
cd ois 
python generate_code.py > build.out 
cd .. 
echo ' -- creating OGREode code'
cd ogreode 
python generate_code.py > build.out 
cd .. 
#echo ' -- creating OGREnewt code'
#cd ogrenewt 
#python generate_code.py > build.out 
#cd .. 
echo ' -- creating ode code'
cd ode 
python generate_code.py > build.out 
cd ..
echo ' -- creating CEGUI code'
cd cegui 
python generate_code.py > build.out 
cd $INSTALL_DIR
echo
echo "=== done building code ==="
