import os

SDK=False

## Boost stuff 
## path to the root of your boost dir, should have 'libs', 'boost' subdirectories
PATH_Boost =        r'c:/development/boost'    
## Path to your boost_pythonxxxx lib file
PATH_LIB_Boost =    r'c:/development/boost/bin.v2/libs/python/build/msvc-7.1/release/threading-multi'
## and the name of the boost python library
LIB_Boost =         r'boost_python-vc71-mt-1_35'

## Parent directories of the libraries
PATH_OgreDshow =         r'c:/development/OgreVideo'
PATH_OGRE =           r'c:/development/Ocvs/ogrenew'

# We need to know where to find gccxml......
gccxml_bin =        r'c:/development/gccxml/bin/release/gccxml.exe'
# and the Py++ directory as sometimes we need access to the code repository there
pyplusplus_install_dir = r'c:/development/pyplus'


###        
### NOTE the MSVC projects puts the OgreAR lib on dir level below the base directory
###
PATH_LIB_OgreDshow =    os.path.join(PATH_OgreDshow, 'OgreDshow/Release')
PATH_LIB_Ogre_OgreMain  =  os.path.join( PATH_OGRE, 'lib' )


PATH_INCLUDE_OgreDshow =          os.path.join(PATH_OgreDshow,'include') 
PATH_INCLUDE_OGRE =            os.path.join(PATH_OGRE,'OgreMain/include')
