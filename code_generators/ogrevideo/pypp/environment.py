import os
import sys
import getpass


_LOGGING_ON = False

def log ( instring ):
    if _LOGGING_ON:
        print __file__, "LOG::", instring

PythonOgreARMajorVersion = "0"
PythonOgreARMinorVersion = "1" 
PythonOgreARPatchVersion = "0"

##
## these should be fine with auto create - however override them as necessary
##
PATH_Python = os.path.dirname( sys.executable )
if os.name =='nt': 
	python_include_dirs = os.path.join ( PATH_Python, 'include')
	python_lib_dirs = os.path.join ( PATH_Python, 'libs' )
##
## for linux or darwin (MAC OS X)

else:
    if os.sys.platform <> 'darwin':
        ## It's linux of some sort
    	python_include_dirs = os.path.join ( '/usr/include/python2.4', '')
    	python_lib_dirs = os.path.join ( '/usr/lib/python2.4', 'libs' )
    else:
        ## it's Mac OS X
        python_include_dirs = '/Library/Frameworks/Python.framework/Versions/2.5/include/python2.5'
        python_lib_dirs = ''

root_dir = os.path.abspath(os.path.dirname(__file__) )## The root directory is where this module is located

generated_dir_name = 'generated'
package_dir_name = 'packages'
generated_dir = os.path.join( root_dir, generated_dir_name )
declarations_cache_dir = os.path.join( root_dir, 'cache' )

_UserName = getpass.getuser()
_ConfigSet = False
_SystemType = os.name ## nt or posix or mac
_PlatformType = sys.platform ## win32, ??

##
## Now we load the user specific stuff
##

# log ( "Config.PATH_Python: %s, UserName: %s, SystemType: %s, Root_dir: %s" % (Config.PATH_Python, _UserName, _SystemType, root_dir) )
try:
    s = 'PythonOgreDshowConfig_' + _UserName ## + '.py'
    Config = __import__(  s  )
    _ConfigSet = True
    log ( "Loaded Config (based on username) from %s" % (s ))
except:
    try:
        s= 'PythonOgreDshowConfig_' + _SystemType ## + '.py'
        
        Config = __import__(  s  )
        _ConfigSet = True
        log ( "Loaded Config (based on systemtype) from %s" % (s))
    except:
        pass   

if not _ConfigSet:
    log ( "  Going to use internal config setting")
    ## These need to be adjusted based upon your system
    ##
    ## PLEASE use an external PythonOgreARConfig_<username>.py to hold these value !!!!
    ## 
    print "\n\n You DO need to create a PythonOgreDshowConfig_%s.py file with config details" % ( _SystemType)
    sys.exit(-1)
    
        
######################

##  Hopefully we can make the rest of this 'automagic'  ##

######################
                    
class ogredshow:
    active = True
    version = "0.0"   # "1.2"
    cflags = ""
    if os.sys.platform <> 'darwin':
        libs=[Config.LIB_Boost,  'OgreMain', 'Strmiids' , 'ole32']
        lib_dirs = [ Config.PATH_LIB_Boost
                    ,  Config.PATH_LIB_OgreDshow
                    , Config.PATH_LIB_Ogre_OgreMain
                     ]
        include_dirs = [ Config.PATH_Boost 
                    , Config.PATH_INCLUDE_OgreDshow
                    , Config.PATH_INCLUDE_OGRE 
                    ]
        CCFLAGS =  ' -D"BOOST_PYTHON_MAX_ARITY=19"'
        LINKFLAGS = ''
        
    CheckIncludes=['boost/python.hpp']

     

############################################################################################

## Here is the master list....
## Keep eveything here in lowercase so the over rides work :)
    
projects = {
    'ogredshow' : ogredshow

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
    cls.root_dir = os.path.join( root_dir )
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

