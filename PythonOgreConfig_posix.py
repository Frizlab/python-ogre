import sys,os

## Boost stuff 
## path to the root of your boost dir, should have 'libs', 'boost' subdirectories
PATH_Boost = r'/home/andy/development/boost'    
## Path to your boost_pythonxxxx lib file
## Path to your boost_pythonxxxx lib file

## and the name of the boost python library
LIB_Boost =         r'boost_python-vc71-mt-1_35'



PATH_LIB_Boost = r'/usr/boost/bin.v2/libs/python/build/'
## and the name of the boost python library
LIB_Boost = 'boost_python??????????'

## Ogre Home, should 'Samples', 'OgreMain' subdirectories
PATH_Ogre = r'/home/andy/development/ogrenew'
PATH_OgreAddons = r'/home/andy/development/OgreAddons'

PATH_CEGUI = r'/home/andy/development/CEGUI-0.5.0'
PATH_OIS = r'/home/andy/development/ois-1.0RC1'
PATH_Newton = r'/usr/development/newtonsdk/sdk'

#
# Don't forget we need to call gccxml......
gccxml_bin = r'/home/andy/development/gccxml/GCC_XML/'


## Parent directories of the libraries
PATH_FMOD =         r'/home/andy/development/'
PATH_ODE =          r'/home/andy/development/'
PATH_OGREAL=        r'/home/andy/development/'
PATH_OGG =          r'/home/andy/development/'
PATH_VORBIS=        r'/home/andy/development/'
PATH_OPENAL=        r'/home/andy/development/'
PATH_ALUT=          r'/home/andy/development/'
PATH_OgreOde=       r'/home/andy/development/'


# and the Py++ directory as sometimes we need access to the code repository there
pyplusplus_install_dir = r'/home/andy/development/pygccxml'

# Overrides - this is needed as libs isn't defined in enirvonment for fmod..
class fmod:
    version= "4.06"
    libs=[LIB_Boost, 'fmodexL_vc']
    ##libs=[Config.LIB_Boost, 'libfmodex']  ##posix

###
### these paths assume you've left all the directory structure as standard
### Override anything that is different
###        
PATH_LIB_Ogre_CEGUIRenderer =    os.path.join( PATH_Ogre, 'Samples/Common/CEGUIRenderer/lib')
PATH_LIB_Ogre_OgreMain=         os.path.join( PATH_Ogre, 'OgreMain/lib/Release' )
PATH_LIB_Ogre_Dependencies =    os.path.join( PATH_Ogre, 'Dependencies/lib/Release')
PATH_LIB_OgreRefApp =           os.path.join( PATH_Ogre, 'ReferenceApplication/ReferenceAppLayer/lib/Release')
PATH_LIB_OgreNewt =             os.path.join(PATH_OgreAddons, r'ogrenewt/OgreNewt_Main/lib/Release') 
PATH_LIB_Newton =               os.path.join(PATH_Newton ,'dll')  ##NOTE Posix platforms this lives in 'lib-mt'
PATH_LIB_OIS =                  os.path.join(PATH_OIS, 'dll') ## NOTE Posix platforms this lives in'lib' not 'dll'
PATH_LIB_CEGUI =                os.path.join ( PATH_Ogre, r'Dependencies/lib/Release' )
PATH_LIB_ODE =                  os.path.join( PATH_ODE, 'lib/releasedll')## probable releaselib for posix
PATH_LIB_OgreOde =              os.path.join( PATH_OgreOde, 'lib/Release') 
PATH_LIB_OgreOdePrefab =        os.path.join( PATH_OgreOde, 'prefab/lib/Release' )
PATH_LIB_OgreOdeLoader =        os.path.join( PATH_OgreOde, 'loader/lib/Release' )

PATH_LIB_FMOD =                 os.path.join(PATH_FMOD, 'api/lib') 
PATH_LIB_OgreAL =               os.path.join(PATH_OGREAL, 'lib/Release' )


PATH_INCLUDE_Ogre=          os.path.join(PATH_Ogre,'OgreMain/include') 
PATH_INCLUDE_Ogre_Dependencies =          os.path.join( PATH_Ogre, 'Dependencies/include')
PATH_INCLUDE_OIS =          os.path.join(PATH_OIS,'includes')    ## Note the plural include's
PATH_INCLUDE_OgreRefApp =   os.path.join(PATH_Ogre,'ReferenceApplication/ReferenceAppLayer/include') 
PATH_INCLUDE_OgreNewt =     os.path.join(PATH_OgreAddons,'ogrenewt/OgreNewt_Main/inc')
PATH_INCLUDE_CEGUI =        os.path.join(PATH_CEGUI, r'include/CEGUI')
PATH_INCLUDE_ODE =          os.path.join( PATH_ODE, 'include')
PATH_INCLUDE_ODESOURCE =          os.path.join( PATH_ODE, 'ode/src')
PATH_INCLUDE_OgreAL =       os.path.join( PATH_OGREAL, 'include' )
PATH_INCLUDE_OPENAL =       os.path.join( PATH_OPENAL, 'include')
PATH_INCLUDE_OGG=           os.path.join( PATH_OGG, r'include' )
PATH_INCLUDE_VORBIS=        os.path.join( PATH_VORBIS, r'include' )
PATH_INCLUDE_ALUT=          os.path.join( PATH_ALUT, 'include' )

PATH_INCLUDE_OgreOde =      os.path.join( PATH_OgreOde,'include')
PATH_INCLUDE_OgreOdePrefab= os.path.join( PATH_OgreOde,'prefab/include')
PATH_INCLUDE_OgreOdeLoader= os.path.join( PATH_OgreOde,'loader/include')

PATH_INCLUDE_FMOD =          os.path.join(PATH_FMOD, 'api/inc')
PATH_INCLUDE_Ogre_CEGUIRenderer = os.path.join( PATH_Ogre, 'samples/Common/CEGUIRenderer/include')
