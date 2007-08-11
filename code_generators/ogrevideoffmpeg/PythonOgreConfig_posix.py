import sys,os
import commands





## Boost stuff 
## path to the root of your boost dir, should have 'libs', 'boost' subdirectories
BASE_DIR = r'/home/dermont/PythonOgre'
LOCAL_LIB = r'/usr/local/lib'
PATH_Boost = os.path.join(BASE_DIR, 'boost_1_34_0')

#
PATH_LIB_Boost = LOCAL_LIB
## and the name of the boost python library
LIB_Boost = 'libboost_python-gcc41-mt-1_34'

## Ogre Home, should 'Samples', 'OgreMain' subdirectories
PATH_Ogre = os.path.join('/home/dermont/Development/ogre-v1-4-2','ogrenew')
#
# Don't forget we need to call gccxml......
gccxml_bin = os.path.join(BASE_DIR,'gccxml-build/bin')


# and the Py++ directory as sometimes we need access to the code repository there
pyplusplus_install_dir = os.path.join(BASE_DIR,'pygccxml')

### =====
### OGRE
### =====
PATH_LIB_Ogre_OgreMain=         r'/usr/local/lib'
PATH_LIB_Ogre_Dependencies =    os.path.join( PATH_Ogre, 'lib')
PATH_INCLUDE_Ogre=          os.path.join(PATH_Ogre,'OgreMain/include') 
PATH_INCLUDE_Ogre_Dependencies =          os.path.join( PATH_Ogre, 'OgreMain/include')


### ================
### OGREVIDEOFFMPEG
### ================
PATH_ogrevideoffmpeg = r'/home/dermont/PythonOgre/python-ogre/code_generators/ogrevideoffmpeg'
PATH_INCLUDE_ogrevideoffmpeg= os.path.join(PATH_ogrevideoffmpeg,'include')
PATH_LIB_ogrevideoffmpeg= os.path.join(PATH_ogrevideoffmpeg,'source')

## we need this otherwise the sourcefiles will not be copied accross
PATH_SRC_ogrevideoffmpeg= os.path.join(PATH_ogrevideoffmpeg,'source')




