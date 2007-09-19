#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh

#
# compile code
#
cd python-ogre 
scons PROJECTS=ogre,ois,quickgui,cegui,ogrerefapp
python setup.py install
cd ..

