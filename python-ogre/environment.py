import os
import sys
import getpass
import datetime

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

##
## set this to True if you compiled Ogre with Threads enabled
##
_USE_THREADS = False

def log ( instring ):
    if _LOGGING_ON:
        print __file__, "LOG::", instring

PythonOgreMajorVersion = "1"
PythonOgreMinorVersion = "1" # change to 0.7 due to lowercase properties
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
class Default:
    version = ""
    pythonModule = False
    ModuleName = ""
    myHome = ""
    myLibraryPaths = [ ]
    myLibraries = [ ]
    parent = None
    children = []
    codeGenerationFlags=[]
    dependsOn = []
    compileFlags = []
    linkFlags= []
    includePaths = []
    myIncludes = []
    libaries = myLibraries # a list of the libraries I need plus
    mySource = []   # where to get the source - tuples, {'svn|cvs|ftp|http','utl'}
    myBuildCmds = []    # commands to build the library with

######################
class boost:
    version = "3.4"
    pythonModule = False
    ModuleName = ""
    myHome = 'boost'
    myLibraryPaths = [ 'boost/bin.v2/libs/python2.5/build/msvc-8.0/release/threading-multi' ]
    myLibraries = [ 'boost_python-vc80-mt-1_35']
   
    parent = None
    children = []
    codeGenerationFlags=[]
    dependsOn = []
    compileFlags = []
    linkFlags= []
    includePaths = []
    myIncludes = []
    libaries = myLibraries # a list of the libraries I need plus
    mySource = ['http://prdownloads.sourceforge.net/boost/boost-jam-3.1.13-1-linuxx86.tgz',
                'http://downloads.sourceforge.net/boost/boost_1_34_0.tar.bz2']   # where to get the source - tuples, {'svn|cvs|ftp|http','utl'}
    myBuildCmds = []    # commands to build the library with
       
class ogre:
    active = True
    pythonModule = True
    version = "1.4"
    myName='ogre'
    ModuleName='OGRE'
    cflags = ""
    moduleParentLocation = "renderer"
    parent = "ogre/renderer"
    dependsOn = ['boost']
    myLibraryPaths = []
    myLibraries = ['OgreMain']
    libraries = myLibraries
    
#     for mod in dependsOn:
#         libraries += mod.libraries   
#     
    
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
        CCFLAGS =  ' -DBOOST_PYTHON_MAX_ARITY=19'
        LINKFLAGS = ''
        externalFiles=['OgreMain.dll', 'OgreGuiRender.dll', Config.LIB_Boost+'.dll']
    elif os.sys.platform <> 'darwin':
        libs=[Config.LIB_Boost, 'OgreMain' ] #,  'OgreGUIRenderer', 'CEGUIBase']
        lib_dirs = [ Config.LOCAL_LIB
                    ]
        include_dirs = [ Config.PATH_Boost 
                    , Config.PATH_INCLUDE_Ogre 
                    ]
        CCFLAGS =  ' -DBOOST_PYTHON_MAX_ARITY=19'
        LINKFLAGS = ''
    else:
        libs=[Config.LIB_Boost]
        lib_dirs = [ Config.PATH_LIB_Boost ]
        include_dirs = [ Config.PATH_Boost 
                        , Config.PATH_INCLUDE_Ogre
                        , python_include_dirs
                        , '/Developer/SDKs/MacOSX10.4u.sdk/Developer/Headers/CFMCarbon' 
                        ]
                                
        CCFLAGS = ' -DBOOST_PYTHON_MAX_ARITY=19 -D_POSIX_C_SOURCE -DCF_OPEN_SOURCE'
        LINKFLAGS = '-framework Python -framework Ogre'
        cflags += '--gccxml-cxxflags "-DCF_OPEN_SOURCE -D_POSIX_C_SOURCE -isysroot /Developer/SDKs/MacOSX10.4u.sdk"'
        
    CheckIncludes=['boost/python.hpp', 'Ogre.h']

     
class ois:
    active = True
    version= "1.0"
    parent = "ogre/io"
    if os.name=='nt':
        libs=['OIS_Static',Config.LIB_Boost]
    else:
        libs=['OIS',Config.LIB_Boost]
    if os.name=="nt":
        libs.append ( "User32" ) # needed for static linking
    include_dirs = [ Config.PATH_Boost 
            , Config.PATH_INCLUDE_OIS
            ]
    lib_dirs = [ Config.PATH_LIB_Boost 
            , Config.PATH_LIB_OIS
            ]
    ModuleName = 'OIS'
    CheckIncludes=['boost/python.hpp', 'OIS.h']
    #externalFiles = ['OIS.dll']
    if os.sys.platform == 'darwin':
        LINKFLAGS = '-framework Python -framework Carbon'
    
class ogrerefapp:
    active = True
    version = "1.4"
    parent = "ogre/physics"
    if os.name=="nt":
        libs=[Config.LIB_Boost, 'OgreMain', 'ode', 'ReferenceAppLayer']
    else:
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
    CCFLAGS =  ' -DBOOST_PYTHON_MAX_ARITY=19'
    ModuleName = 'OgreRefApp'
    CheckIncludes = ['boost/python.hpp', 'Ogre.h', 'OgreReferenceAppLayer.h', 'ode/ode.h']
    
class ogrenewt:
    active=True
    version = "1.0"
    parent = "ogre/physics"
    if os.name =="nt":
        libs = ['Newton', Config.LIB_Boost, 'OgreNewt_Main', 'OgreMain']
    else:
        libs = ['Newton', Config.LIB_Boost, 'OgreNewt', 'OgreMain']
    include_dirs = [Config.PATH_Boost
                    , Config.PATH_Newton   # only one path for Newton
                    , Config.PATH_INCLUDE_Ogre 
                    , Config.PATH_INCLUDE_OgreNewt
                    , Config.PATH_INCLUDE_Ogre_Dependencies  #needed for OIS/OIS.h
                    ]
    lib_dirs = [ Config.PATH_LIB_Boost
                ,Config.PATH_LIB_Newton
                ,Config.PATH_LIB_OgreNewt
                , Config.PATH_LIB_Ogre_OgreMain
                
                ]
    CCFLAGS =  ' -DBOOST_PYTHON_MAX_ARITY=19'
    ModuleName = 'OgreNewt'
    CheckIncludes=['boost/python.hpp', 'Ogre.h', 'OgreNewt.h', 'Newton.h']
    
class cegui:
    active = True
    version = "0.5.0b" 
    parent = "ogre/gui"
    if os.name=='nt':
        libs=[Config.LIB_Boost, 'CEGUIBase', 'OgreMain', 'OgreGUIRenderer' ]
    else:
        libs=[Config.LIB_Boost, 'CEGUIBase', 'OgreMain', 'CEGUIOgreRenderer' ]

    include_dirs = [Config.PATH_Boost
                    ,Config.PATH_INCLUDE_CEGUI
                    ,Config.PATH_CEGUI
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
    CCFLAGS =  ' -DBOOST_PYTHON_MAX_ARITY=19'
    ModuleName = 'CEGUI'
    CheckIncludes = ['boost/python.hpp', 'Ogre.h', 'CEGUI.h', 'OgreCEGUIRenderer.h'] 

                    
#############  these are under construction and DO NOT WORK (Yet) #####################  
  
class ode:
    version= "0.8"
    parent = "ogre/physics"
    libs=[Config.LIB_Boost,  'ode']
    if os.name=="nt":
        libs.append ( "User32" ) # needed for MessageBox function
    lib_dirs = [ Config.PATH_LIB_Boost
                ,  Config.PATH_LIB_ODE
                ]
    include_dirs = [ Config.PATH_Boost 
                    ,  Config.PATH_INCLUDE_ODE
                    ]

    ModuleName = 'ODE'
    CheckIncludes = ['boost/python.hpp',  'ode/ode.h'] 
    active=True
    
class opcode:
    version= "1.3"
    parent = "ogre/physics"
    libs=[Config.LIB_Boost]
    lib_dirs = [ Config.PATH_LIB_Boost
                ]
    include_dirs = [ Config.PATH_Boost 
                    ,  Config.PATH_INCLUDE_OPCODE
                    ,  os.path.join(Config.PATH_INCLUDE_OPCODE, 'Ice' )

                    ]
    CCFLAGS =  ' -DBAN_OPCODE_AUTOLINK -DICE_NO_DLL '
    ModuleName = 'Opcode'
    CheckIncludes = ['boost/python.hpp',  'Opcode.h'] 
    active=True 
       
class caelum:
    version="0.2.1"
    parent="ogre/addons"
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_caelum
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ]
    CheckIncludes=[]
    libs=[  Config.LIB_Boost, 'OgreMain' ]
    ModuleName="caelum"   
    active=True

class newton:
    version= "1.0"
    active=False
    include_dirs = [r'c:/development/newtonsdk/sdk']
    ModuleName = 'NEWTON'
    parent = ""    
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
    CCFLAGS =  ' -DBOOST_PYTHON_MAX_ARITY=19'
    CheckIncludes = ['boost/python.hpp', 'Ogre.h', 'ode/ode.h', 'ode/odecpp.h', 'OgreOde_Core.h', 'OgreOde_Loader.h', 
                    'Ogreode_Prefab.h'] 
                     
    ModuleName='OgreOde'
    active=True
    
class quickgui:
    version="0.9.7"
    parent="ogre/gui"
    ## note the defined for _QuickGUIExport forces non dll usage
    if os.name=='nt': 
        ## note the defined for _QuickGUIExport forces non dll usage 
        CCFLAGS = ' -DWIN32 -DNDEBUG -DWINDOWS -D_QuickGUIExport="" ' 
    else:
        CCFLAGS = ''
    cflags=""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_quickgui
                    #,Config.PATH_INCLUDE_freetype
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ##,Config.PATH_LIB_quickgui
                ]
    CheckIncludes=[]
    libs=[  Config.LIB_Boost, 'OgreMain' ]
    ModuleName="QuickGUI"   
    active=True

class navi:
    version="1.4"
    parent="ogre/gui"
    CCFLAGS = '-D"WIN32" -D"NDEBUG", -D"WINDOWS"' 
    cflags=""
    include_dirs = [ Config.PATH_Boost
                    ,Config.PATH_INCLUDE_Ogre
                    ,Config.PATH_INCLUDE_navi
                    ,os.path.join(Config.PATH_navi,'..','Dependencies','win32','llmozlib','include')
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ,Config.PATH_LIB_navi
                ,os.path.join(Config.PATH_navi,'..','Dependencies','win32','llmozlib','lib')
                ]
    CheckIncludes=[]
    libs=[  Config.LIB_Boost, 'Navi', 'OgreMain','llmozlib', 'user32',
            'kernel32.lib', 'gdi32.lib', 'winspool.lib', 'comdlg32.lib', 'advapi32.lib',
            'shell32.lib','ole32.lib','oleaut32.lib','uuid.lib' ]
    ModuleName="navi"   
    active=True

class betagui:
    version="0.16"
    parent="ogre/gui"
    CCFLAGS = ' ' # -D"FT2_BUILD_LIBRARY"
    cflags=""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_betagui
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                Config.PATH_LIB_betagui
                ]
    CheckIncludes=[]
    libs=[  Config.LIB_Boost, 'OgreMain' ]
    ModuleName="betagui"   
    active=True

class ogreforests:
    version="0.1"
    parent="ogre/addons"
    CCFLAGS = ' ' # -D"FT2_BUILD_LIBRARY"
    cflags=""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_ogreforests
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ]
    CheckIncludes=[]
    libs=[  Config.LIB_Boost, 'OgreMain' ]
    ModuleName="ogreforests"   
    active=True
    
class nxogre:
    version="0.9"
    parent="ogre/physics"
    cflags=""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_NxOgre,
                    ]
    for d in Config.PATH_INCLUDE_PhysX:
        include_dirs.append( d )
    if os.name == 'nt':
        CCFLAGS = ' -D"WIN32" '
    else:
        CCFLAGS = ' -DNX_LINUX -DLINUX -DNX_DISABLE_FLUIDS '                
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
#                 Config.PATH_LIB_NxOgre,
                Config.PATH_LIB_PhysX
                ]
    CheckIncludes=[]
#     libs=[  Config.LIB_Boost, 'NxOgre','NxCharacter', 'NxCooking', 'NxExtensions', 'PhysXLoader','OgreMain' ]
    libs=[  Config.LIB_Boost, 'NxCharacter', 'NxCooking', 'NxExtensions', 'PhysXLoader','OgreMain' ]
    ModuleName="NxOgre"   
    active=True
    
    
class theora:
    version="0.5.0"
    parent="ogre/addons"
    cflags=""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_Theora,
                    ]
    for d in Config.PATH_INCLUDE_OggVorbisTheora:
        include_dirs.append( d )
        
    if os.name == 'nt':
        CCFLAGS = ' -D"WIN32" '
                    
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                Config.PATH_LIB_Theora,
                ]
    CheckIncludes=[]
    libs=[  Config.LIB_Boost, 'Plugin_TheoraVideoSystem','OgreMain' ]
    ModuleName="theora"   
    active=True
 
class plib:
    version="1.8.4"
    parent="ogre/addons"
    cflags=""
    CCFLAGS = ' -D"_CRT_SECURE_NO_WARNINGS" ' 
    include_dirs= [ Config.PATH_INCLUDE_plib,
            Config.PATH_THIRDPARTY   # needed as net.h includes via plib/xxxx
            ]
    lib_dirs = [ Config.PATH_LIB_Boost,
            ]           
    ModuleName = "plib"
    CheckIncludes=[]
    if os.name == "nt":
        libs = [ Config.LIB_Boost, 'winmm', 'User32','wsock32', 'opengl32.lib','glu32.lib' ]
    else:
        libs = [ Config.LIB_Boost ] # Note sure what linux libraries are needed
    
    active = True
    
class physx:
    version="2.7.2"
    parent="ogre/physics"
    cflags=""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    ]
    for d in Config.PATH_INCLUDE_PhysX:
        include_dirs.append( d )
    if os.name == 'nt':
        CCFLAGS = ' -D"WIN32" '
    else:
        CCFLAGS = ' -D"LINUX" '                
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                Config.PATH_LIB_PhysX
                ]
    CheckIncludes=[]
    if os.name == 'nt':
        libs=[  Config.LIB_Boost, 'NxCharacter', 'NxCooking', 'NxExtensions', 'PhysXLoader' ]
    else:
        libs=[  Config.LIB_Boost, 'NxCharacter', 'NxCooking',  'PhysXCore' ]
    ModuleName="PhysX"   
    active=True 
           
class ogreal:
    ## changes to compile ogreal as part of Python-Ogre to make it easier under linux
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
    if os.name=='nt':                  
        lib_dirs = [ Config.PATH_LIB_Boost
                ,Config.PATH_LIB_Ogre_OgreMain    
                ,os.path.join(Config.PATH_OGG, 'win32', 'Static_Release')
                ,os.path.join(Config.PATH_VORBIS, 'win32','Vorbis_Static_Release')
                ,os.path.join(Config.PATH_VORBIS, 'win32','VorbisEnc_Static_Release')
                ,os.path.join(Config.PATH_VORBIS, 'win32','VorbisFile_Static_Release')
                ,os.path.join(Config.PATH_OPENAL, 'libs','Win32')
                ,os.path.join(Config.PATH_ALUT, 'lib')
                  ] 
    else:
        lib_dirs = [ Config.PATH_LIB_Boost
                ,Config.PATH_LIB_Ogre_OgreMain    
                  ] 
                  
    if os.name =='nt':
        CCFLAGS = ' -DOgreAL_Export="" -DWIN32 -DNDEBUG -D_LIB -D_WIN32 -D_WINDOWS -DVORBIS_IEEE_FLOAT32 -D_USE_NON_INTEL_COMPILER '              
        libs=[Config.LIB_Boost, 'OgreMain', 
                    'ogg_static', 
                    'alut', 
                    'vorbis_static','vorbisfile_static','vorbisenc_static',
                    'OpenAL32', 'EFX-Util']  ##  'OgreAL' -- going to compile OgreAL ourselves
    else:                    
        libs=[Config.LIB_Boost, 'OgreMain', 
                    'ogg', 
                    'alut', 
                    'vorbis','vorbisfile','vorbisenc',
                    'openal']  ##  'OgreAL' -- going to compile OgreAL ourselves
    ModuleName = 'OgreAL'
    CheckIncludes = ['OgreAL.h']
    active=True
    
class ogrevideoffmpeg:
    version="0.2.1"
    parent="ogre/addons"
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_ogrevideoffmpeg,
                    Config.PATH_INCLUDE_ffmpeg
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                Config.PATH_LIB_ogrevideoffmpeg,
                Config.PATH_LIB_ffmpeg
                ]
    CheckIncludes=[]
    if os.name == 'nt':
        libs=[  Config.LIB_Boost, 'OgreMain', 'avformat-51', 'avcodec-51', 'avutil-49' ]
    else:
        libs=[  Config.LIB_Boost, 'OgreMain', 'avformat', 'avcodec', 'avutil', 'z', 'GL', 'GLU', 'Xxf86vm', 'Xext', 'X11' ]
    ModuleName="ogrevideoffmpeg"   
    active=True 

class ogredshow:
    version="0.1"
    parent="ogre/addons"
    if os.name=='nt': 
        ## note the defined for _ETManagerExport forces non dll usage 
        CCFLAGS = ' -DWIN32 -DNDEBUG -DWINDOWS -D_ETManagerExport="" ' 
    else:
        CCFLAGS = ''
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_ogredshow
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                
                ]
    CheckIncludes=[]
    libs=[  Config.LIB_Boost, 'OgreMain','Strmiids' , 'ole32']
    ModuleName="ogredshow"   
    active=True 
    
class et:  ## editable terrain
    version= "2.2"
    parent = "ogre/addons"
    libs=[ Config.LIB_Boost, 'OgreMain' ]
    lib_dirs = [ Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ]
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_et
                    ]
    ModuleName = 'et'
    CheckIncludes = ['boost/python.hpp'] 
    active=True 
    cflags = ""
    
class bullet:
    version= "2.62"
    parent = "ogre/physics"
    libs=[Config.LIB_Boost,  'LibBulletCollision', 'LibBulletDynamics'] #,'libbulletmath']
    lib_dirs = [ Config.PATH_LIB_Boost
                ,  Config.PATH_LIB_Bullet
                ]
    include_dirs = [ Config.PATH_Boost 
                    ,  Config.PATH_INCLUDE_Bullet
                    ]

    ModuleName = 'bullet'
    CheckIncludes = ['boost/python.hpp'] 
    active=True 
    cflags = ""
    
class ogrebulletc:  # 
    active=True
    version = "1.0"
    cflags = ""
    parent = "ogre/physics"
    libs = [Config.LIB_Boost,  'OgreMain', 
        'LibBulletCollision', 'LibBulletDynamics'
        ]
    include_dirs = [Config.PATH_Boost
                    , Config.PATH_INCLUDE_Bullet
                    , os.path.join(Config.PATH_OgreBullet, 'Collisions' )
                    , Config.PATH_INCLUDE_Ogre 
                    , Config.PATH_INCLUDE_Ogre_Dependencies  
                    ]
    lib_dirs = [ Config.PATH_LIB_Boost
                ,Config.PATH_LIB_Bullet
                , Config.PATH_LIB_Ogre_OgreMain
                ]
    if os.name=="nt":
        CCFLAGS =  ' -DWIN32 -DNDEBUG -D_WINDOWS -D_PRECOMP '
    else:
        CCFLAGS = ' -D_PRECOMP '
    ModuleName = 'OgreBulletC'
    CheckIncludes=['boost/python.hpp', 'Ogre.h']    

class ogrebulletd:  # 
    active=True
    version = "1.0"
    cflags = ""
    parent = "ogre/physics"
    libs = [Config.LIB_Boost,  'OgreMain', 
        'LibBulletCollision', 'LibBulletDynamics'
        ]
    include_dirs = [Config.PATH_Boost
                    , Config.PATH_INCLUDE_Bullet
                    , os.path.join(Config.PATH_OgreBullet, 'Collisions' )
                    , os.path.join(Config.PATH_OgreBullet, 'Dynamics' )
                    , Config.PATH_INCLUDE_Ogre 
                    , Config.PATH_INCLUDE_Ogre_Dependencies  
                    ]
    lib_dirs = [ Config.PATH_LIB_Boost
                ,Config.PATH_LIB_Bullet
                , Config.PATH_LIB_Ogre_OgreMain
                ]
    if os.name=="nt":
        CCFLAGS =  ' -DWIN32 -DNDEBUG -D_WINDOWS -D_PRECOMP '
    else:
        CCFLAGS = ' -D_PRECOMP '
    ModuleName = 'OgreBulletD'
    CheckIncludes=['boost/python.hpp', 'Ogre.h']        
    
############################################################################################

## Here is the master list....
## Keep eveything here in lowercase so the over rides work :)
    
projects = {
    'ois' : ois
    , 'ogre' : ogre
    , 'cegui' : cegui
    , 'ode' : ode
    , 'newton' : newton
    , 'ogrerefapp' : ogrerefapp
    , 'ogrenewt' : ogrenewt
    , 'ogreode' : ogreode
    , 'ogreal' : ogreal
    , 'quickgui' : quickgui
    , 'opcode' : opcode
    , 'nxogre' : nxogre
    , 'bullet' : bullet
    , 'physx' : physx
    , 'betagui': betagui
    , 'theora' : theora
    , 'ogrevideoffmpeg' : ogrevideoffmpeg
    , 'ogredshow' : ogredshow
    , 'plib' : plib
    , 'navi': navi
    , 'ogrebulletc' : ogrebulletc
    , 'ogrebulletd' : ogrebulletd
    , 'ogreforests' : ogreforests
    , 'et' : et
    , 'caelum' : caelum
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

