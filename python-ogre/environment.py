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

_LOGGING_ON = False

def log ( instring ):
    if _LOGGING_ON:
        print __file__, "LOG::", instring

PythonOgreMajorVersion = "1"
PythonOgreMinorVersion = "0" # change to 0.7 due to lowercase properties
PythonOgrePatchVersion = "0"


##
## these should be fine with auto create - however override them as necessary
##
PATH_Python = os.path.dirname( sys.executable )
## I want a version string 2.4 or 2.5 etc
PythonVersionString = str(sys.version_info[0]) + '.' + str(sys.version_info[1])

if os.name =='nt': 
	python_include_dirs = os.path.join ( PATH_Python, 'include')
	python_lib_dirs = os.path.join ( PATH_Python, 'libs' )
##
## for linux or darwin (MAC OS X)
else:
    if os.sys.platform <> 'darwin':
        ## It's linux of some sort
    	python_include_dirs = os.path.join ( '/usr/include/python'+PythonVersionString,'')
    	python_lib_dirs = os.path.join ( '/usr/lib/python'+PythonVersionString, 'libs' )
    else:
        ## it's Mac OS X
        pathlist = PATH_Python.split('/')
        version = pathlist[pathlist.index('Versions')+1]
        python_include_dirs = '/'.join(pathlist[:pathlist.index('Versions')+2]) + '/include/python' + version
        python_lib_dirs = ''

root_dir = os.path.abspath(os.path.dirname(__file__) )## The root directory is where this module is located

sys.path.append( os.path.join( root_dir, 'common_utils' ) )
shared_ptr_dir = os.path.join( root_dir, 'shared_ptr' )
include_dir = os.path.join( root_dir, 'include' )
generated_dir_name = 'generated' 
package_dir_name = 'packages'+ "_" + PythonVersionString 
generated_dir = os.path.join( root_dir, generated_dir_name )
declarations_cache_dir = os.path.join( root_dir, 'code_generators', 'cache' )

_UserName = getpass.getuser()
_ConfigSet = False
_SystemType = os.name ## nt or posix or mac
_PlatformType = sys.platform ## win32, ??

##
## Now we load the user specific stuff
##

# log ( "Config.PATH_Python: %s, UserName: %s, SystemType: %s, Root_dir: %s" % (Config.PATH_Python, _UserName, _SystemType, root_dir) )
try:
    s = 'PythonOgreConfig_' + _UserName ## + '.py'
    Config = __import__(  s  )
    _ConfigSet = True
    log ( "Loaded Config (based on username) from %s" % (s ))
except:
    try:
        s= 'PythonOgreConfig_' + _SystemType ## + '.py'
        
        Config = __import__(  s  )
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
    print "\n\n You DO need to create a PythonOgreConfig_%s.py file with config details" % ( _SystemType)
    sys.exit(-1)
    
        
######################

##  Hopefully we can make the rest of this 'automagic'  ##

######################
                    
class ogre:
    active = True
    version = "1.4"   # "1.2"
    cflags = ""
    parent = "ogre/renderer"
    ModuleName = 'OGRE'
    if os.name =='nt': 
        libs=[Config.LIB_Boost, 'OgreMain' ] #,  'OgreGUIRenderer', 'CEGUIBase']
        lib_dirs = [ Config.PATH_LIB_Boost
                    ,  Config.PATH_LIB_Ogre_CEGUIRenderer
                    , Config.PATH_LIB_Ogre_OgreMain
                    , Config.PATH_LIB_Ogre_Dependencies #needed for ceguibase.lib etc
                    , Config.PATH_LIB_CEGUI
                     ]
        include_dirs = [ Config.PATH_Boost 
                    , Config.PATH_INCLUDE_Ogre 
                    ]
        CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
        LINKFLAGS = ''
        externalFiles=['OgreMain.dll', 'OgreGuiRender.dll', Config.LIB_Boost+'.dll']
    elif os.sys.platform <> 'darwin':
        libs=[Config.LIB_Boost, 'OgreMain' ] #,  'OgreGUIRenderer', 'CEGUIBase']
        lib_dirs = [ Config.LOCAL_LIB
                    ]
        include_dirs = [ Config.PATH_Boost 
                    , Config.PATH_INCLUDE_Ogre 
                    ]
        CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
        LINKFLAGS = ''
    else:
        libs=[Config.LIB_Boost]
        lib_dirs = [ Config.PATH_LIB_Boost ]
        include_dirs = [ Config.PATH_Boost 
                        , Config.PATH_INCLUDE_Ogre
                        , python_include_dirs
                        , '/Developer/SDKs/MacOSX10.4u.sdk/Developer/Headers/CFMCarbon' 
                        ]
                                
        CCFLAGS = ' -D"BOOST_PYTHON_MAX_ARITY=19" -D_POSIX_C_SOURCE -DCF_OPEN_SOURCE'
        LINKFLAGS = '-framework Python -framework Ogre'
        cflags += '--gccxml-cxxflags "-DCF_OPEN_SOURCE -D_POSIX_C_SOURCE -isysroot /Developer/SDKs/MacOSX10.4u.sdk"'
        
    CheckIncludes=['boost/python.hpp', 'Ogre.h']

     
class ois:
    active = True
    version= "1.0"
    parent = "ogre/io"
    libs=['OIS',Config.LIB_Boost]
    include_dirs = [ Config.PATH_Boost 
            , Config.PATH_INCLUDE_OIS
            ]
    lib_dirs = [ Config.PATH_LIB_Boost 
            , Config.PATH_LIB_OIS
            ]
    ModuleName = 'OIS'
    CheckIncludes=['boost/python.hpp', 'OIS.h']
    externalFiles = ['OIS.dll']
    if os.sys.platform == 'darwin':
        LINKFLAGS = '-framework Python -framework Carbon'
    
class ogrerefapp:
    active = True
    version = "1.4"
    parent = "ogre/physics"
    libs=[Config.LIB_Boost, 'OgreMain', 'ode', 'ReferenceAppLayer']
    lib_dirs = [ Config.PATH_LIB_Boost
                , Config.PATH_LIB_Ogre_OgreMain
                , Config.PATH_LIB_ODE
                , Config.PATH_LIB_OgreRefApp
                ]
    include_dirs = [ Config.PATH_Boost 
                    ,Config.PATH_INCLUDE_Ogre
                    ,Config.PATH_INCLUDE_OgreRefApp
                    ,Config.PATH_INCLUDE_ODE
                    ]
    CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
    ModuleName = 'OgreRefApp'
    CheckIncludes = ['boost/python.hpp', 'Ogre.h', 'OgreReferenceAppLayer.h', 'ode/ode.h']
    
class ogrenewt:
    active=True
    version = "1.0"
    parent = "ogre/physics"
    libs = ['newton', Config.LIB_Boost, 'OgreNewt_Main', 'OgreMain']
    include_dirs = [Config.PATH_Boost
                    , Config.PATH_INCLUDE_Newton   # only one path for Newton
                    , Config.PATH_INCLUDE_Ogre 
                    , Config.PATH_INCLUDE_OgreNewt
                    , Config.PATH_INCLUDE_Ogre_Dependencies  #needed for OIS/OIS.h
                    ]
    lib_dirs = [ Config.PATH_LIB_Boost
                ,Config.PATH_LIB_Newton
                ,Config.PATH_LIB_OgreNewt
                , Config.PATH_LIB_Ogre_OgreMain
                
                ]
    CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
    ModuleName = 'OgreNewt'
    CheckIncludes=['boost/python.hpp', 'Ogre.h', 'OgreNewt.h', 'Newton.h']
    
class cegui:
    active = True
    version = "0.5.0b" 
    parent = "ogre/gui"
    libs=[Config.LIB_Boost, 'CEGUIBase', 'OgreMain', 'OgreGUIRenderer' ]
    include_dirs = [Config.PATH_Boost
                    ,Config.PATH_INCLUDE_CEGUI
                    #,Config.PATH_CEGUI
                    , Config.PATH_INCLUDE_Ogre_CEGUIRenderer
                    , Config.PATH_INCLUDE_Ogre
                    , Config.PATH_INCLUDE_Ogre_Dependencies ## needed as OgreCEGUI uses CEGUI/.. in #includes
                    ]
                  
    lib_dirs = [ Config.PATH_LIB_Boost
                ##,  Config.PATH_LIB_Ogre_CEGUIRenderer
                , Config.PATH_LIB_Ogre_OgreMain
                , Config.PATH_LIB_CEGUI
                ,  Config.PATH_LIB_Ogre_Dependencies  
                ]
    CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
    ModuleName = 'CEGUI'
    CheckIncludes = ['boost/python.hpp', 'Ogre.h', 'CEGUI.h', 'OgreCEGUIRenderer.h'] 

                    
#############  these are under construction and DO NOT WORK (Yet) #####################  
  
class ode:
    version= "0.7"
    include_dirs = [r'c:/development/ode/include']
    parent = "ogre/physics"
    libs=[Config.LIB_Boost,  'ode']
    lib_dirs = [ Config.PATH_LIB_Boost
                ,  Config.PATH_LIB_ODE
                ]
    include_dirs = [ Config.PATH_Boost 
                    ,  Config.PATH_INCLUDE_ODE
                    , Config.PATH_INCLUDE_ODESOURCE ## some header files are in the source tree ??
                    ]

    ModuleName = 'ODE'
    CheckIncludes = ['boost/python.hpp',  'ode/ode.h'] 
    active=True

class ogreode:
    version= "1.0"
    parent = "ogre/physics"

    lib_dirs = [ Config.PATH_LIB_Boost
                , Config.PATH_LIB_OgreOde
                , Config.PATH_LIB_OgreOdePrefab
                , Config.PATH_LIB_OgreOdeLoader
                , Config.PATH_LIB_Ogre_OgreMain
                ,  Config.PATH_LIB_ODE
                ]
    include_dirs = [ Config.PATH_Boost 
                , Config.PATH_INCLUDE_ODE
                , Config.PATH_INCLUDE_OgreOde
                , Config.PATH_INCLUDE_OgreOdePrefab
                , Config.PATH_INCLUDE_OgreOdeLoader
                , Config.PATH_INCLUDE_Ogre
                ]

    libs=[Config.LIB_Boost, 'OgreMain', 'ode', 'OgreOde_Core', 'OgreOde_Prefab', 'OgreOde_Loader' ]
    CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
    CheckIncludes = ['boost/python.hpp', 'Ogre.h', 'ode/ode.h', 'ode/odecpp.h', 'OgreOde_Core.h', 'OgreOde_Loader.h', 
                    'Ogreode_Prefab.h'] 
                     
    ModuleName='OgreOde'
    active=True
    

    
# class betagui:
#     version="1.0"
#     parent="ogre/gui"
#     cflags = ""
#     include_dirs = [ Config.PATH_Boost,
#                     Config.PATH_INCLUDE_Ogre,
#                     Config.PATH_INCLUDE_betagui
#                     ]
#     lib_dirs = [Config.PATH_LIB_Boost,
#                 Config.PATH_LIB_Ogre_OgreMain,
#                 Config.PATH_LIB_betagui
#                 ]
#     CheckIncludes=[]
#     libs=[  Config.LIB_Boost, 'OgreMain' ]
#     ModuleName="BetaGui"   
#     active=True

class simplegui:
    version="1.0"
    parent="ogre/gui"
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_simplegui
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                Config.PATH_LIB_simplegui
                ]
    CheckIncludes=[]
    libs=[  Config.LIB_Boost, 'OgreMain' ]
    ModuleName="simplegui"   
    active=True
 
# class raknet:
#     version="1.0"
#     parent="ogre/network"
#     cflags = ""
#     include_dirs = [ Config.PATH_Boost,
#                     Config.PATH_INCLUDE_raknet
#                     ]
#     lib_dirs = [Config.PATH_LIB_Boost,
#                 Config.PATH_LIB_raknet
#                 ]
#     CheckIncludes=[]
#     libs=[  Config.LIB_Boost, 'RakNetLibStatic' ]  # could use RakNetDll
#     ModuleName="raknet"   
#     active=True    
           
class ogreal:
    version="0.3"
    parent = "ogre/sound"
    
    include_dirs = [ Config.PATH_Boost
                , Config.PATH_INCLUDE_Ogre
                , Config.PATH_INCLUDE_OgreAL
                , Config.PATH_INCLUDE_OGG
                , Config.PATH_INCLUDE_VORBIS
                , Config.PATH_INCLUDE_OPENAL
                , Config.PATH_INCLUDE_ALUT
                ]    
    lib_dirs = [ Config.PATH_LIB_Boost
                  ,Config.PATH_LIB_Ogre_OgreMain
                  , Config.PATH_LIB_OgreAL
                  ] 
    libs=[Config.LIB_Boost, 'OgreMain', 'OgreAL']
    ModuleName = 'OgreAL'
    CheckIncludes = ['OgreAL.h']
    active=True
                
############################################################################################

## Here is the master list....
## Keep eveything here in lowercase so the over rides work :)
    
projects = {
    'ois' : ois
    , 'ogre' : ogre
    , 'cegui' : cegui
    , 'ode' : ode
    , 'ogrerefapp' : ogrerefapp
    , 'ogrenewt' : ogrenewt
    , 'ogreode' : ogreode
    , 'ogreal' : ogreal
#     , 'betagui' : betagui
    , 'simplegui' : simplegui
#     , 'raknet' : raknet
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
        if os.name =='nt': 
            for libfile in cls.libs :
                libfile += '.lib'
                found = False
                log ( "Checking for %s library (%s class) in lib_dirs" % (libfile, name) )
                for lookdir in cls.lib_dirs:
                    if os.path.isfile ( os.path.join ( lookdir, libfile ) ):
                        found = True
                        break
                if not found:
                    print "WARNING: Unable to find %s library (%s class) in lib_dirs" % (libfile, name)
    
#
# a couple of specials that should be done differently
gccxml_bin = Config.gccxml_bin
pyplusplus_install_dir = Config.pyplusplus_install_dir                    
                            
for name, cls in projects.items():

    # little hack to allow overriding of settings from the config file
    if hasattr(Config, name):   # look to see if the class exists in the config file
        print "Using Override for class ", name
        _class = Config.__dict__[name]  # get the class
        for key, value in _class.__dict__.items():
            if not key.startswith('_'):
                cls.__dict__[key] = value   
                print "Set %s.%s to %s" % (name, key, value) 
                
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
        elif os.name == 'posix':
            cls.PydName = cls.PydName + '.so'
        elif os.name =='mac':
            print "WARNING - check the last line of environment.py!!"

