#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh
echo "=== generating code ===" 
echo "=== generating code ===" >> $INSTALL_DIR/log.out
cd python-ogre/code_generators 
if [ $1 != "" ]
then 
    cd $1 
    python generate_code.py > build.out 
    cd .. 
else

echo ' -- creating OGRE code'
echo ' -- creating OGRE code' >> $INSTALL_DIR/log.out
cd ogre 
python generate_code.py > build.out 
cd .. 
echo ' -- creating OGRErefapp code'
echo ' -- creating OGRErefapp code'>> $INSTALL_DIR/log.out
cd ogrerefapp 
python generate_code.py > build.out 
cd .. 
echo ' -- creating quickgui code'
echo ' -- creating quickgui code'>> $INSTALL_DIR/log.out
cd quickgui 
python generate_code.py > build.out 
cd .. 
echo ' -- creating OIS code'
echo ' -- creating OIS code'>> $INSTALL_DIR/log.out
cd ois 
python generate_code.py > build.out 
cd .. 
echo ' -- creating CEGUI code'
echo ' -- creating CEGUI code'>> $INSTALL_DIR/log.out
cd cegui 
python generate_code.py > build.out 
cd ..
echo ' -- creating OgreAL code'
echo ' -- creating OgreAL code'>> $INSTALL_DIR/log.out
cd ogreal 
python generate_code.py > build.out 
cd .. 
echo ' -- creating OGREode code'
echo ' -- creating OGREode code'>> $INSTALL_DIR/log.out
cd ogreode 
python generate_code.py > build.out 
cd .. 
echo ' -- creating OpCode code'
echo ' -- creating OpCode code'>> $INSTALL_DIR/log.out
cd opcode 
python generate_code.py > build.out 
cd .. 
echo ' -- creating PLIB code'
echo ' -- creating PLIB code'>> $INSTALL_DIR/log.out
cd plib 
python generate_code.py > build.out 
cd .. 
echo ' -- creating Theora code'
echo ' -- creating Theora code'>> $INSTALL_DIR/log.out
cd theora 
python generate_code.py > build.out 
cd .. 
echo ' -- creating OGREnewt code'
echo ' -- creating OGREnewt code' >> $INSTALL_DIR/log.out
cd ogrenewt 
python generate_code.py > build.out 
cd .. 
#echo ' -- creating ode code'
#cd ode 
#python generate_code.py > build.out 

fi
cd $INSTALL_DIR
echo
echo "=== done building code ==="
echo "=== done building code ===">> $INSTALL_DIR/log.out

