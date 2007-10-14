#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh
pushd $INSTALL_DIR/python-ogre/code_generators/$1
python generate_code.py > build.out
popd
