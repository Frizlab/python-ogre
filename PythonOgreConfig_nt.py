import os
## Boost stuff 
## path to the root of your boost dir, should have 'libs', 'boost' subdirectories
PATH_Boost =        r'c:/development/boost'    
## Path to your boost_pythonxxxx lib file
PATH_LIB_Boost =    r'c:/development/boost/bin.v2/libs/python/build/msvc-7.1/release/threading-multi'
## and the name of the boost python library
LIB_Boost =         r'boost_python-vc71-mt-1_35'

## Parent directories of the libraries
PATH_Ogre =         r'c:/development/Ocvs/Ogrenew'
PATH_OgreAddons =   r'c:/development/Ocvs/OgreAddons'
PATH_CEGUI =        r'c:/development/cegui-0.5.0'
PATH_OIS =          r'c:/development/ois'
PATH_Newton =       r'c:/development/newtonsdk/sdk'
PATH_FMOD =         r'c:/development/fmod'
PATH_ODE =          r'c:/development/ode'

# We need to know where to find gccxml......
gccxml_bin =        r'c:/development/gccxml/bin/release/gccxml.exe'
# and the Py++ directory as sometimes we need access to the code repository there
pyplusplus_install_dir = r'c:/development/pyplus'

## Overrides - this is needed as libs isn't defined in enirvonment for fmod..
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
PATH_LIB_OgreOde =              os.path.join( PATH_OgreAddons, 'ogreode/lib/Release') 
PATH_LIB_OgreOdePrefab =        os.path.join( PATH_Ogre, 'OgreMain/lib/Release' )
PATH_LIB_FMOD =                 os.path.join(PATH_FMOD, 'api/lib') 

PATH_INCLUDE_Ogre=          os.path.join(PATH_Ogre,'OgreMain/include') 
PATH_INCLUDE_Ogre_Dependencies =          os.path.join( PATH_Ogre, 'Dependencies/include')
PATH_INCLUDE_OIS =          os.path.join(PATH_OIS,'includes')    ## Note the plural include's
PATH_INCLUDE_OgreRefApp =   os.path.join(PATH_Ogre,'ReferenceApplication/ReferenceAppLayer/include') 
PATH_INCLUDE_OgreNewt =     os.path.join(PATH_OgreAddons,'ogrenewt/OgreNewt_Main/inc')
PATH_INCLUDE_CEGUI =        os.path.join(PATH_CEGUI, r'include')
PATH_INCLUDE_ODE =          os.path.join( PATH_ODE, 'include')
PATH_INCLUDE_ODESOURCE =          os.path.join( PATH_ODE, 'ode/src')

PATH_INCLUDE_OgreOde =      os.path.join( PATH_OgreAddons,'ogreode/include')
PATH_INCLUDE_OgreOdePrefab= os.path.join( PATH_OgreAddons,'ogreode/prefab/include')
PATH_INCLUDE_FMOD =          os.path.join(PATH_FMOD, 'api/inc')
PATH_INCLUDE_Ogre_CEGUIRenderer = os.path.join( PATH_Ogre, 'samples/Common/CEGUIRenderer/include')