import os
import sys
import getpass


##
##  Jan 2007 modification
##
##  This will now look for external config files.
## 
## first it will look for PythonOgreConfig_<username>.py
##
## if that fails it will look for PythonOgreConfig_<platform>.py
##
## otherwise it will use the values you enter below...
##

_LOGGING_ON = True

def log ( instring ):
    if _LOGGING_ON:
        print instring

PythonOgreMajorVersion = "0"
PythonOgreMinorVersion = "7" # change to 0.7 due to lowercase properties
PythonOgrePatchVersion = "2"

##
## these should be fine with auto create - however override them as necessary
##
PATH_Python = os.path.dirname( sys.executable )
python_include_dirs = os.path.join ( PATH_Python, 'include')
Python_lib_dirs = os.path.join ( PATH_Python, 'libs' )
root_dir = os.path.abspath(os.path.dirname(__file__) )## The root directory is where this module is located

sys.path.append( os.path.join( root_dir, 'common_utils' ) )
shared_ptr_dir = os.path.join( root_dir, 'shared_ptr' )
generated_dir = os.path.join( root_dir, 'generated' )
declarations_cache_dir = os.path.join( root_dir, 'code_generators', 'cache' )

_UserName = getpass.getuser()
_ConfigSet = False
_SystemType = os.name ## nt or posix
_PlatformType = sys.platform ## win32, ??

##
## Now we load the user specific stuff
##

log ( "Using PATH_Python: %s\nUserName: %s\nSystemType: %s\nRoot_dir: %s" % (PATH_Python, _UserName, _SystemType, root_dir) )
try:
    s = 'PythonOgreConfig_' + _UserName + '.py'
    log ( "  Trying to load Config based upon UserName: %s " % (s) )
    execfile( os.path.join(root_dir, s ) )
    _ConfigSet = True
    log ( "  Success with UserName load")
except:
    try:
        s= 'PythonOgreConfig_' + _SystemType + '.py'
        log ( "  Trying to load Config based upon SystemType: %s" % (s))
        execfile( os.path.join(root_dir, s ) )
        _ConfigSet = True
        log ( "  Success with SystemType load")
    except:
        pass   

if not _ConfigSet:
    log ( "  Going to use internal config setting")
    ## These need to be adjusted based upon your system
    ##
    ## PLEASE use an external PythonOgreConfig_<username>.py to hold these value !!!!
    ## 
    print "\n\n You REALLY need to create a PythonOgreConfig_%s.py file with config details", _UserName
    PATH_Boost = r'c:/development/boost'
    PATH_LIB_Boost = r'c:/development/boost/boost/bin.v2/libs/python/build/msvc-7.1/release/threading-multi'
    LIB_Boost = 'boost_python-vc71-mt-1_35'
    PATH_Newton = r'c:/development/newtonsdk/sdk'
    PATH_Ogre = r'c:/development/Ocvs/Ogrenew'
    PATH_OgreAddons = r'c:/development/Ocvs/OgreAddons'
    PATH_CEGUI = r'c:/development/cegui-0.5.0'
    PATH_OIS = r'c:/development/ois'
    if "roman" in getpass.getuser():
        gccxml_bin = r'd:/gccxml_cvs/gccxml-build/bin/release/gccxml.exe'
    else:
        gccxml_bin = r'c:/development/gccxml/bin/release/gccxml.exe'


        
######################

##  Hopefully we can make the rest of this 'automagic'  ##

######################
                    
class ogre:
    active = True
    version = "1.4"   # "1.2"
    libs=[LIB_Boost, 'OgreMain', 'ode', 'OgreGUIRenderer', 'CEGUIBase']
    lib_dirs = [ PATH_LIB_Boost
                , os.path.join( PATH_Ogre, 'Samples/Common/CEGUIRenderer/lib')
                , os.path.join( PATH_Ogre, 'OgreMain/lib/Release' )
                , os.path.join( PATH_Ogre, 'Dependencies/lib/Release')
                ]
    include_dirs = [ PATH_Boost 
                , os.path.join(PATH_Ogre,'OgreMain/include') 
                ]
    CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
    LINKFLAGS = ''
     
class ois:
    active = True
    version= "1.0"
    libs=['OIS']
    include_dirs = [ PATH_Boost 
            , os.path.join(PATH_OIS,'includes')    ## Note the plural include's
            ]
    if os.name =='nt':
        _lpath='dll'
    else:
        _lpath='lib'
    lib_dirs = [ PATH_LIB_Boost 
            , os.path.join(PATH_OIS, _lpath)
            ]
    
class ogrerefapp:
    active = True
    version = "1.4"
    libs=[LIB_Boost, 'OgreMain', 'ode', 'OgreGUIRenderer', 'CEGUIBase', 'ReferenceAppLayer']
    lib_dirs = [ PATH_LIB_Boost
                , os.path.join( PATH_Ogre, 'Samples/Common/CEGUIRenderer/lib')
                , os.path.join( PATH_Ogre, 'OgreMain/lib/Release' )
                , os.path.join( PATH_Ogre, 'Dependencies/lib/Release')
                , os.path.join( PATH_Ogre, 'ReferenceApplication/ReferenceAppLayer/lib/Release')
                ]
    include_dirs = [ PATH_Boost 
                    , os.path.join(PATH_Ogre,'OgreMain/include') 
                    , os.path.join(PATH_Ogre,'ReferenceApplication/ReferenceAppLayer/include') 
                    , os.path.join( PATH_Ogre, 'Dependencies/include')
                    ]
    CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
    
class ogrenewt:
    active=True
    version = "1.0"
    libs = ['newton', LIB_Boost, 'OgreNewt_Main']
    if os.name =='nt':
        _lpath='dll'
    else:
        _lpath='lib-mt'  
    include_dirs = [PATH_Boost
                    , PATH_Newton   # only one path for Newton
                , os.path.join(PATH_Ogre,'OgreMain/include') 
                    ]
    lib_dirs = [ PATH_LIB_Boost
                , os.path.join(PATH_Newton ,_lpath)
                ,os.path.join(PATH_OgreAddons, r'ogrenewt/OgreNewt_Main/lib/Release') 
                , os.path.join( PATH_Ogre, 'OgreMain/lib/Release' )
                ]
    CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
                    
class CEGUI:
    active = True
    version = "0.5.0b" 
    libs=[LIB_Boost, 'CEGUIBase.lib', 'OgreMain', 'OgreGUIRenderer' ]
    include_dirs = [PATH_Boost
                    ,os.path.join(PATH_CEGUI, r'include')
                    ,PATH_CEGUI
                    ,os.path.join ( PATH_Ogre, r'Samples/Common/CEGUIRenderer/include' )
                    ]
                  
    lib_dirs = [ PATH_LIB_Boost
                , os.path.join ( PATH_Ogre, r'Samples/Common/CEGUIRenderer/lib' )
                , os.path.join ( PATH_Ogre, r'OgreMain/lib/Release' )
                , os.path.join ( PATH_Ogre, r'Dependencies/lib/Release' )
                ]
    CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'

                    
#############  these are under construction and DO NOT WORK (Yet) #####################  
  
class ode:
    version= "0.7"
    include_dirs = [r'c:/development/ode/include']
class newton:
    version= "1.0"
    include_dirs = [r'c:/development/newtonsdk/sdk']
class ogreode:
    version= "1.0"
    include_dirs = [r'c:/development/ocvs/ogreaddons/ogreode/include',
                    r'c:/development/ocvs/ogreaddons/ogreode/prefab/include',
                    r'c:\development\ocvs\ogrenew\ogremain\include',
                    r'c:/development/ode/include']
class FMOD:
    version= "1.0"
    include_dirs=[r'c:\development\fmod\api\inc']
############################################################################################

## Here is the master list....
    
projects = {
    'ois' : ois
    , 'ogre' : ogre
    , 'CEGUI' : CEGUI
    , 'ode' : ode
    , 'newton' : newton
    , 'ogrerefapp' : ogrerefapp
    , 'ogrenewt' : ogrenewt
    , 'FMOD' : FMOD
    , 'ogreode' : ogreode
}        

    

for name, cls in projects.items():
    cls.root_dir = os.path.join( root_dir, 'code_generators', name )
    cls.generated_dir = os.path.join( generated_dir, name + '_' + str(cls.version) )
    cls.cache_file = os.path.join( declarations_cache_dir, name + '_' + str(cls.version) + '_cache.xml' )

