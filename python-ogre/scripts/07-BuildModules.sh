#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh

#
# compile code
#
cd python-ogre 
scons PROJECTS=ogre,ois,quickgui,cegui,plib,ogreode 
#,ogrerefapp
python setup.py install --prefix=$PREFIX 
cd ..

