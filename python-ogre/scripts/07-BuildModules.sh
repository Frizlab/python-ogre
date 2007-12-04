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
    for PROJ in ogre ois cegui quickgui; do scons PROJECTS=$PROJ ; done ;
    for PROJ in plib ogrenewt ogreode caelum et ogreforests bullet ogrebulletc ogrebulletd ; do scons PROJECTS=$PROJ ; done ;
#ogreal,ogrebulletc,ogrebulletd,nxogre -i
fi
#opcode -- not really suited to a linux build -- lots of work needed ? 
#
python setup.py install --prefix=$PREFIX 
cd ..

