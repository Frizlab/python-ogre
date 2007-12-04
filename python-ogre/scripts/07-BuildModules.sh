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
    scons PROJECTS=ogre,ois,caelum,cegui,plib,ogrenewt,ogreode,quickgui,et,bullet,ogreforests -i
#ogreal,ogrebulletc,ogrebulletd,nxogre -i
fi
#opcode -- not really suited to a linux build -- lots of work needed ? 
#
python setup.py install --prefix=$PREFIX 
cd ..

