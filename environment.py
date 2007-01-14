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
        print __file__, "LOG::", instring

PythonOgreMajorVersion = "0"
PythonOgreMinorVersion = "7" # change to 0.7 due to lowercase properties
PythonOgrePatchVersion = "2"

##
## these should be fine with auto create - however override them as necessary
##
PATH_Python = os.path.dirname( sys.executable )
python_include_dirs = os.path.join ( PATH_Python, 'include')
python_lib_dirs = os.path.join ( PATH_Python, 'libs' )
root_dir = os.path.abspath(os.path.dirname(__file__) )## The root directory is where this module is located

sys.path.append( os.path.join( root_dir, 'common_utils' ) )
shared_ptr_dir = os.path.join( root_dir, 'shared_ptr' )
generated_dir_name = 'generated'
package_dir_name = 'packages'
generated_dir = os.path.join( root_dir, generated_dir_name )
declarations_cache_dir = os.path.join( root_dir, 'code_generators', 'cache' )

_UserName = getpass.getuser()
_ConfigSet = False
_SystemType = os.name ## nt or posix
_PlatformType = sys.platform ## win32, ??

##
## Now we load the user specific stuff
##

# log ( "PATH_Python: %s, UserName: %s, SystemType: %s, Root_dir: %s" % (PATH_Python, _UserName, _SystemType, root_dir) )
try:
    s = 'PythonOgreConfig_' + _UserName + '.py'
    execfile( os.path.join(root_dir, s ) )
    _ConfigSet = True
    log ( "Loaded Config (based on username) from %s" % (s))
except:
    try:
        s= 'PythonOgreConfig_' + _SystemType + '.py'
        execfile( os.path.join(root_dir, s ) )
        _ConfigSet = True
        log ( "Loaded Config (based on systemtype) from %s" % (s))
    except:
        pass   

if not _ConfigSet:
    log ( "  Going to use internal config setting")
    ## These need to be adjusted based upon your system
    ##
    ## PLEASE use an external PythonOgreConfig_<username>.py to hold these value !!!!
    ## 
    print "\n\n You DO need to create a PythonOgreConfig_%s.py file with config details", _SystemType
    sys.exit(-1)
    
        
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
    CheckIncludes=['boost/python.hpp', 'Ogre.h']

     
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
    ModuleName = 'OIS'
    CheckIncludes=['boost/python.hpp', 'OIS.h']
        
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
    ModuleName = 'OgreRefApp'
    CheckIncludes = ['boost/python.hpp', 'Ogre.h', 'OgreReferenceAppLayer.h', 'OIS/OIS.h']
    
class ogrenewt:
    active=True
    version = "1.0"
    libs = ['newton', LIB_Boost, 'OgreNewt_Main', 'OgreMain']
    if os.name =='nt':
        _lpath='dll'
    else:
        _lpath='lib-mt'  
    include_dirs = [PATH_Boost
                    , PATH_Newton   # only one path for Newton
                , os.path.join(PATH_Ogre,'OgreMain/include') 
                , os.path.join(PATH_OgreAddons,'ogrenewt/OgreNewt_Main/inc')
                    ]
    lib_dirs = [ PATH_LIB_Boost
                , os.path.join(PATH_Newton ,_lpath)
                ,os.path.join(PATH_OgreAddons, r'ogrenewt/OgreNewt_Main/lib/Release') 
                , os.path.join( PATH_Ogre, 'OgreMain/lib/Release' )
                ]
    CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
    ModuleName = 'OgreNewt'
    CheckIncludes=['boost/python.hpp', 'Ogre.h', 'OgreNewt.h', 'Newton.h']
    
class CEGUI:
    active = True
    version = "0.5.0b" 
    libs=[LIB_Boost, 'CEGUIBase', 'OgreMain', 'OgreGUIRenderer' ]
    include_dirs = [PATH_Boost
                    ,os.path.join(PATH_CEGUI, r'include')
                    ,PATH_CEGUI
                    ,os.path.join ( PATH_Ogre, r'Samples/Common/CEGUIRenderer/include' )
                    , os.path.join(PATH_Ogre,'OgreMain/include')
                    ]
                  
    lib_dirs = [ PATH_LIB_Boost
                , os.path.join ( PATH_Ogre, r'Samples/Common/CEGUIRenderer/lib' )
                , os.path.join ( PATH_Ogre, r'OgreMain/lib/Release' )
                , os.path.join ( PATH_Ogre, r'Dependencies/lib/Release' )
                ]
    CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
    ModuleName = 'CEGUI'
    CheckIncludes = ['boost/python.hpp', 'Ogre.h', 'CEGUI.h', 'OgreCEGUIRenderer.h'] 

                    
#############  these are under construction and DO NOT WORK (Yet) #####################  
  
class ode:
    version= "0.7"
    include_dirs = [r'c:/development/ode/include']
    ModuleName = 'ODE'
    active=False
class newton:
    version= "1.0"
    active=False
    include_dirs = [r'c:/development/newtonsdk/sdk']
    ModuleName = 'NEWTON'
class ogreode:
    version= "1.0"
    include_dirs = [r'c:/development/ocvs/ogreaddons/ogreode/include',
                    r'c:/development/ocvs/ogreaddons/ogreode/prefab/include',
                    r'c:\development\ocvs\ogrenew\ogremain\include',
                    r'c:/development/ode/include']
    ModuleName='OgreOde'
    active=False
class FMOD:
    version= "1.0"
    include_dirs=[r'c:\development\fmod\api\inc']
    ModuleName = 'FMOD'
    active=False
############################################################################################

## Here is the master list....
    
projects = {
    'ois' : ois
    , 'ogre' : ogre
    , 'cegui' : CEGUI
    , 'ode' : ode
    , 'newton' : newton
    , 'ogrerefapp' : ogrerefapp
    , 'ogrenewt' : ogrenewt
    , 'fmod' : FMOD
    , 'ogreode' : ogreode
}        

#
# let's setup some defaults
#    
def CheckPaths ( cls , name):
    """ lets check we can find files listed in the CheckIncludes list somewhere in the include_dirs directories
    also look for libs in the lib_dirs.
    This way we can warn people early if they have a configuration problem"""
    if cls.active:  ## only check active classes
        for incfile in cls.CheckIncludes:
            found = False
            log ( "Checking for %s include file (%s class) in include_dirs" % (incfile, name) )
            for lookdir in cls.include_dirs:
                if os.path.isfile ( os.path.join ( lookdir, incfile ) ):
                    found = True
                    break
            if not found:
                print "WARNING: Unable to find %s include file (%s class) in include_dirs" % (incfile, name)
        for libfile in cls.libs:
            if os.name =='nt':
                libfile += '.lib'
            found = False
            log ( "Checking for %s library (%s class) in lib_dirs" % (libfile, name) )
            for lookdir in cls.lib_dirs:
                if os.path.isfile ( os.path.join ( lookdir, libfile ) ):
                    found = True
                    break
            if not found:
                print "WARNING: Unable to find %s library (%s class) in lib_dirs" % (libfile, name)
    
        
for name, cls in projects.items():
    CheckPaths( cls, name )
    cls.root_dir = os.path.join( root_dir, 'code_generators', name )
    cls.dir_name = name + '_' + str(cls.version)
    cls.generated_dir = os.path.join( generated_dir, cls.dir_name )
    cls.cache_file = os.path.join( declarations_cache_dir, cls.dir_name + '_cache.xml' )
    if not hasattr (cls, 'ModuleName'):
        cls.ModuleName = name[0].upper() + name[1:]
    if not hasattr (cls, 'PydName'):
        cls.PydName = '_' + name.lower() + '_'
        if os.name == 'nt':
            cls.PydName = cls.PydName + '.pyd'
        else:
            print "WARNING - check the last line of environment.py!!"

