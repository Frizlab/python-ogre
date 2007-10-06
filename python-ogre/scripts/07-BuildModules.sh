#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh

#
# compile code
#
cd python-ogre 
if [ $1 != "" ]
then 
    scons PROJECTS=$1
else
    scons PROJECTS=ogre,ois,cegui,plib,ogreode,ogrenewt,ogreal,quickgui,et,caelum
fi
#opcode -- not really suited to a linux build -- lots of work needed ? 
#
python setup.py install --prefix=$PREFIX 
cd ..

