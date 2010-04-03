import os
import sys
import getpass
import subprocess

_LOGGING_ON = False
_PRECOMPILED = True
_DEBUG = False
#_DEBUG = True

PythonOgreMajorVersion = "1"
PythonOgreMinorVersion = "7"
PythonOgrePatchVersion = "0"

_UserName = getpass.getuser()

UseSystem = "--usesystem" in sys.argv or (os.environ.has_key('USESYSTEM') and eval(os.environ['USESYSTEM'].title()))
if "--usesystem" in sys.argv:
    sys.argv.remove("--usesystem")

    
def log(instring):
    if _LOGGING_ON:
        print __file__, "LOG::", instring

def isWindows():     
    if os.name == 'nt':
        return True
    return False

def isLinux():
    if os.name == 'posix' and os.sys.platform != 'darwin':
        return True
    return False

def isDebian():
    return os.path.exists("/etc/debian_version")

def isMac():
    if os.sys.platform == 'darwin':
        return True
    return False

def is64():
    if isLinux():
        if os.uname()[4] == 'x86_64': # TOFIX - can we use sys.platform for this as uname is linux specific..
            return True
    return False

def numCores():
 if hasattr(os, "sysconf"):
     # Linux
     if os.sysconf_names.has_key("SC_NPROCESSORS_ONLN"):
         ncpus = os.sysconf("SC_NPROCESSORS_ONLN")
         if isinstance(ncpus, int) and ncpus > 0:
             return ncpus
     else: # OSX:
         return int(os.popen2("sysctl -n hw.ncpu")[1].read())
 # Windows
 if os.environ.has_key("NUMBER_OF_PROCESSORS"):
         ncpus = int(os.environ["NUMBER_OF_PROCESSORS"]);
         if ncpus > 0:
             return ncpus
 return 1 # Default

##
## set this to True if you compiled Ogre with Threads enabled and setting a static boost link
##
if isWindows():
    _USE_THREADS = True
    BOOST_STATIC = False
else:
    _USE_THREADS = False
    BOOST_STATIC = False

##
## these should be fine with auto create - however override them as necessary
##
## I want a version string 2.4 or 2.5 etc
##
PythonVersionString = str(sys.version_info[0]) + '.' + str(sys.version_info[1])

## and now setup suitable paths
if isWindows():
    ppath = os.path.dirname(sys.executable)
    python_include_dirs = os.path.join(ppath, 'include')
    python_lib_dirs = os.path.join(ppath, 'libs')
elif isLinux():
    ## It's linux of some sort
    python_include_dirs = os.path.join('/usr/include/python' + PythonVersionString, '')
    python_lib_dirs = os.path.join('/usr/lib/python' + PythonVersionString, 'libs')
elif isMac(): ## it's Mac OS X
    pathlist = os.path.normpath(sys.prefix).split('/')
    version = pathlist[pathlist.index('Versions') + 1]
    python_include_dirs = '/'.join(pathlist[:pathlist.index('Versions') + 2]) + '/include/python' + version
    python_lib_dirs = ''
    sdks = []
    base = '/Developer/SDKs'
    for x in os.listdir(base):
         if os.path.isdir(os.path.join(base, x)):
            sdks.append(os.path.join(base, x))
    sdks.sort()
    MAC_SDK = sdks[-1]
    MAC_ARCH = os.uname()[4] # need to force the architecture on the compile and link
    MAC_ARCH_b = 'x86' # FIX -- need to detect if on ppc and change this for boost build
    MAC_ADDR = '32' # default to 32 bit
    if '64' in MAC_ARCH:
        MAC_ADDR = '64' # need to build the right boost libraries FIX = guessing here
else:
    raise (SystemExit, "Don't know what this system is (checked for Windows, Linux and Mac)!")


root_dir = os.path.abspath(os.path.dirname(__file__))## The root directory is where this module is located
_root = root_dir
sys.path.append(os.path.join(root_dir, 'common_utils'))
shared_ptr_dir = os.path.join(root_dir, 'shared_ptr')
include_dir = os.path.join(root_dir, 'include')
_thirdPartyDir = os.path.join(root_dir, 'ThirdParty')
generated_dir_name = 'generated'
package_dir_name = 'packages' + "_" + PythonVersionString
generated_dir = os.path.join(root_dir, generated_dir_name)
declarations_cache_dir = os.path.join(root_dir, 'code_generators', 'cache')


##
## Now we load the user specific stuff
##
_ConfigSet = False
try:
    s = 'PythonOgreConfig_' + _UserName ## + '.py'
    Config = __import__(s)
    _ConfigSet = True
    log("Loaded Config (based on username) from %s" % (s))
except ImportError:
    try:
        if UseSystem:
            s = 'PythonOgreConfig_system'
        else:
            s = 'PythonOgreConfig_' + os.name # 'nt', 'posix' etc

        Config = __import__(s)
        _ConfigSet = True
        log("Loaded Config (based on systemtype) from %s" % (s))
    except ImportError:
        pass

if not _ConfigSet:
    log("  Going to use internal config setting")
    ## These need to be adjusted based upon your system
    ##
    ## PLEASE use an external PythonOgreConfig_<username>.py to hold these value !!!!
    ##
    print "\n\n You DO need to create a PythonOgreConfig_%s.py file with config details" % (os.name)
    sys.exit(-1)

## BIG assumption about where you want things put
if UseSystem and not isWindows():
    ROOT = '/'
    Config.ROOT_DIR = '/'  # need to override this so the code generation stuff works
    PREFIX = '/usr'
    _sudo = 'sudo '
else:
    ROOT = os.path.join(os.getcwd(), 'root')
    PREFIX = os.path.join(os.getcwd(), 'root', 'usr')
    _sudo = ''

# Let the user override the number of cores.
if hasattr(Config, "NUMBER_OF_CORES") and Config.NUMBER_OF_CORES:
    NUMBER_OF_CORES = Config.NUMBER_OF_CORES
else:
    NUMBER_OF_CORES = numCores()


######################
downloadPath = os.path.abspath("downloads")
wget = "wget -c -nc "
if isMac():
    try:
        subprocess.check_call(['curl', '--version'], stdout=file('/dev/null', 'w'))
        wget = "curl -C- -L -O -s "
    except: #check_call will raise this is it can't find curl
        pass # so leave wget alone
tar = "tar "
gzip = "gzip "

if isWindows():
    mv = "move /Y "
    rm = "del /Q "
    cp = "copy /Y "
    unzip = "unzip -o "
    cvs = "cvs -z3 -q  "
    svn = "svn "
    sed_ = "sed --in-place "
    VCBUILD = "'c:\\Program Files\\Microsoft Visual Studio 8\\vc\\vcpackages\\vcbuild.exe'  /useenv  "
    VCBUILD = '"c:/Program Files/Microsoft Visual Studio 8/vc/vcpackages/vcbuild.exe" /useenv '
else:
    mv = "mv "
    rm = "rm -f "
    cp = "cp -f "
    unzip = "unzip -o "
    cvs = "cvs -z3 -q  "
    svn = "svn --non-interactive "
    if isMac():
        sed_ = "sed -i '' "
    else:
        sed_ = "sed --in-place "


# set a default set of symbols for gccxml to use when generating code
# put it here do to the changes that threading makes
defined_symbols = ['OGRE_NONCLIENT_BUILD', 'OGRE_GCC_VISIBILITY', '__PYTHONOGRE_BUILD_CODE' ]
if isMac():
    defined_symbols.append ('__APPLE_CC__')
if _USE_THREADS:
    defined_symbols.append('BOOST_HAS_THREADS')
    if isWindows():
        defined_symbols.append('BOOST_HAS_WINTHREADS')

if int(PythonOgreMajorVersion) >= 1 and int(PythonOgreMinorVersion) >= 7:
    defined_symbols.append ('HAVE_OGRE_BUILDSETTINGS_H') # Ogre 1.7 it uses the cmake buildsettings include


def unTarGz(base, source):
    """ a complete hack to cope with untar gziping a source file in the downloads directory into it's own directory
    Needed as the GNU tools for windows don't handle tar forking gzip or working on files that are in
    different directories
    """
    ret = [
        [0, cp + os.path.join(downloadPath, base + ".tar.gz") + " " + os.getcwd(), '' ],
        [0, gzip + " -f -d " + os.path.join(os.getcwd(), base + ".tar.gz"), ''],
        [0, tar + " xf " + base + ".tar", '' ],
        [0, rm + base + ".tar", '']
    ]
    return ret


class module(object):
    """Base class for all modules."""

    # By default all modules are active
    active = True

    # By default modules are not python based.
    pythonModule = False

    # The commands to download the source code for this module - enacted using
    # "BuildModule.py -r"
    source = []
    # The version of the source we downloaded.
    source_version = "head"

    # The commands to build this module - enacted using "BuildMoulde.py -b"
    buildCmds = []

    # Hack to add properties to the class
    class __metaclass__(type):
        @property
        def base(cls):
            return cls.__name__

        @property
        def name(cls):
            return cls.__name__

class pymodule(module):
    """Base class for all the python modules."""
    pythonModule = True

    # The module name that Python ends up with
    ModuleName = None

    # Extra libraries to link against.
    libs = []
    # Directory to find the libraries in.
    lib_dirs = []

    # Extra directories to search for include files.
    include_dirs = []

    # dlls that need to live in the package directory
    moduleLibs = []

    # Extra flags passed to gccxml compiler when generating XML code base
    cflags = ''

    # FIXME: Why is these uppercase while cflags above is lowercase?
    # flags passed to SCON's when  compiling
    LINKFLAGS = ''
    CCFLAGS = ''



####################################################
##
## These are the base tools and libraries
##
####################################################
class gccxml(module):
    source_version = "20100101"
    source = [
        [cvs, " -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML co -D 01Jan2010 gccxml", os.getcwd()]
    ]

    if isLinux() or isMac():
        buildCmds = [
            [0, "mkdir -p gccxml-build", ''],
            [0, "cmake ../gccxml -DCMAKE_INSTALL_PREFIX:PATH=" + PREFIX, os.path.join(os.getcwd(), 'gccxml-build')],
            [0, "make", os.path.join(os.getcwd(), 'gccxml-build')],
            [0, _sudo + " make install", os.path.join(os.getcwd(), 'gccxml-build')],
        ]
    else:
        buildCmds = [
            [0, 'echo Please use CMAKE to create a msvc build projects for gccxml', ''],
            [0, 'echo  ie. Run Cmake (from the windows menu)', ''],
            [0, 'echo      specific the gccxml for both directories and run configure', ''],
            [0, 'echo      ignore any errors - you will need to run configure twice and then OK', ''],
            [0, 'echo      Now run MSVC and load the gccxml project (in the gccxml directory) and build it', '']
        ]

class install(module):
    source = []
    if UseSystem:
        buildCmds = [
            [0, _sudo + " python setup.py install ", os.path.join(os.getcwd(), 'python-ogre')],
        ]
    else:
        buildCmds = [
            [0, _sudo + " python setup.py install --prefix=%s" % PREFIX, os.path.join(os.getcwd(), 'python-ogre')],
        ]


class pygccxml(module):
    source_version = "head"
    source = [
        [svn, " co http://pygccxml.svn.sourceforge.net/svnroot/pygccxml/pygccxml_dev pygccxml", os.getcwd()]
    ]

    if isLinux() or isMac():
        if UseSystem:
            buildCmds = [
                [0, _sudo + " python setup.py install " , os.path.join(os.getcwd(), "pygccxml") ],
            ]
        else:
            buildCmds = [
                [0, _sudo + " python setup.py install  --prefix=" + PREFIX, os.path.join(os.getcwd(), "pygccxml") ],
            ]

    if isWindows():
        buildCmds = [
            [0, "python setup.py install  ", os.path.join(os.getcwd(), "pygccxml") ],
        ]


class pyplusplus(module):
    source_version = "head"
    source = [
        [svn, " co http://pygccxml.svn.sourceforge.net/svnroot/pygccxml/pyplusplus_dev pyplusplus", os.getcwd()]
    ]

    if isLinux() or isMac():
        if UseSystem:
            buildCmds = [
                [0, _sudo + " python setup.py install " , os.path.join(os.getcwd(), "pyplusplus") ],
            ]
        else:
            buildCmds = [
                [0, _sudo + " python setup.py install  --prefix=" + PREFIX, os.path.join(os.getcwd(), "pyplusplus") ],
            ]

    if isWindows():
        buildCmds = [
            [0, "python setup.py install  ", os.path.join(os.getcwd(), "pyplusplus") ],
        ]

class cg(module):
    """Not needed on Windows or Mac as it's part of the Ogre dependencies package."""

    if isLinux():
        if is64():
            base = 'Cg-2.2_October2009_x86_64'
        else:
            base = 'Cg-2.2_October2009_x86'
        source = [
#            [wget, " http://developer.download.nvidia.com/cg/Cg_2.0/2.0.0015/" + base + ".tgz", downloadPath]
             [wget, " http://developer.download.nvidia.com/cg/Cg_2.2/" + base + ".tgz", downloadPath]
        ]
        buildCmds = [
            [0, _sudo + ' ' + tar + " xvzf " + os.path.join(downloadPath, base) + ".tgz --overwrite", ROOT], # unpack it directly into 'our' root
        ]

    elif isMac():
        ## "http://downloads.sourceforge.net/ogre/OgreDependenciesOSX_20070929.zip"
        source = [
            [wget, "http://developer.download.nvidia.com/cg/Cg_2.2/Cg-2.2_October2009.dmg", downloadPath ]
        ]
        buildCmds = [
            [0, 'open ' + os.path.join(downloadPath, 'Cg-2.2_October2009.dmg'), os.getcwd()]
        ]


class zziplib(module):
    ## The 'install' "fails" on the mac however it's actually OK
    if isLinux() or isMac():
        base = 'zziplib-0.13.49'
        source = [
            [wget, " http://downloads.sourceforge.net/zziplib/" + base + ".tar.bz2", downloadPath],
        ]

        buildCmds = [
            [0, tar + " xjf " + os.path.join(downloadPath, base) + ".tar.bz2 --overwrite", ''],
            [0, "aclocal", os.path.join(os.getcwd(), base) ],
            [0, "./configure --prefix=%s --datadir=%s/share --includedir=%s/include " % (PREFIX, PREFIX, PREFIX), os.path.join(os.getcwd(), base) ],
            [0, "make", os.path.join(os.getcwd(), base) ],
            [0, "make install", os.path.join(os.getcwd(), base) ]
        ]


class freeimage(module):
    if isLinux() or isMac():
        base = 'FreeImage'
        source = [
            [wget, " http://downloads.sourceforge.net/freeimage/FreeImage3110.zip", downloadPath],
        ]

        if isLinux():
            buildCmds = [
                [0, unzip + " -q -o " + os.path.join(downloadPath, base) + "3110.zip", ''],
                ##
                # Note the use of a single command that does multiple sed's
                ##
                [0, """sed --in-place -s 's|/usr/include|\$(DESTINC)|' Makefile.*\n\
                       sed --in-place -s 's|/usr/lib|\$(DESTLIB)|' Makefile.*\n\
                       sed --in-place -s 's|-o root -g root||' Makefile.*\n\
                       sed --in-place -s 's|ldconfig||' Makefile.*""", os.path.join(os.getcwd(), base)],
                [0, "make", os.path.join(os.getcwd(), base) ],
                [0, "make DESTINC=%s/include DESTLIB=%s/lib install " % (PREFIX, PREFIX), os.path.join(os.getcwd(), base)],
            ]

        if isMac():
            ## mac has a crap sed command implementation and a different shell interaction, plus make file is a little different (or course)
            buildCmds = [
                [0, unzip + " -q -o " + os.path.join(downloadPath, base) + "3110.zip", ''],
                ##
                # Note the use of a single command that does multiple sed's
                ##
                [0, """sed -i '' 's|/usr/local|$(DEST)|' Makefile.*\n\
                       sed -i '' 's|-o root -g wheel||' Makefile.*\n\
                       sed -i '' 's|ldconfig||' Makefile.*""", os.path.join(os.getcwd(), base) ],
                [0, "make", os.path.join(os.getcwd(), base) ],
                [0, "make DEST=%s install " % PREFIX, os.path.join(os.getcwd(), base) ]
            ]

class cmake(module):
    if isLinux() :
        base = 'cmake-2.8.0-Linux-i386'
        buildCmds = [
            [0, tar + " xzf " + os.path.join(downloadPath, base) + ".tar.gz --overwrite", ''],
            [0, cp + "-R  * " + PREFIX, os.path.join(os.getcwd(), base) ],
        ]
        source = [
            [wget, "http://www.cmake.org/files/v2.8/" + base + ".tar.gz", downloadPath],
        ]

    elif isMac():
        base = 'cmake-2.8.0-Darwin-universal'
        buildCmds = [
            [0, tar + " xzf " + os.path.join(downloadPath, base) + ".tar.gz", ''],
            [0, cp + r"-R  CMake\ 2.8-0.app/Contents/* " + PREFIX, os.path.join(os.getcwd(), base) ],
        ]
        source = [
            [wget, "http://www.cmake.org/files/v2.8/" + base + ".tar.gz", downloadPath],
        ]

    elif isWindows():
        source = [
            [wget, "http://www.cmake.org/files/v2.8/cmake-2.8.0-win32-x86.exe", downloadPath]
        ]

        buildCmds = [
            [0, os.path.join(downloadPath, "cmake-2.6.4-win32-x86.exe"), '' ]
        ]


class scons(module):
    base = 'scons-1.2.0'
    source = [
        [wget, "http://prdownloads.sourceforge.net/scons/" + base + ".tar.gz", downloadPath],
    ]

    # the utils in Windows don't handle paths or tar spawing gzip hence the work arounds
    if isLinux():
        buildCmds = [
            [0, tar + " zxf " + os.path.join(downloadPath, base) + ".tar.gz --overwrite", '' ],
            # note fix here as scons defaults to adding bundle to command line which stops us building dynamiclibs!!
            #[0, 'sed -i s/-bundle// applelink.py', os.path.join(os.getcwd(), base, 'engine', 'SCons', 'Tool')   ],
            [0, "python setup.py install  --prefix=%s" % PREFIX, os.path.join(os.getcwd(), base) ]
        ]
    elif isMac():
        buildCmds = [
            [0, tar + " zxf " + os.path.join(downloadPath, base) + ".tar.gz ", '' ],
            # note fix here as scons defaults to adding bundle to command line which stops us building dynamiclibs!!
            [0, 'sed -i bak "s/-bundle//" applelink.py', os.path.join(os.getcwd(), base, 'engine', 'SCons', 'Tool')   ],
            [0, "python setup.py install  --prefix=%s" % PREFIX, os.path.join(os.getcwd(), base) ]
        ]

    else:
        buildCmds = unTarGz(base, downloadPath) + [
            [0, "python setup.py install ", os.path.join(os.getcwd(), base) ]
        ]

class boost_python(module):
    """ build versionof boost that we include with the SVN code -- builds python, thread and date-time
    only installs boost_python on Linux
    """
    version = "1.42.0"
    source = []
    boostDir = os.path.join(os.getcwd(), "boost_python")
    buildCmds = [
        [0, unzip + ' ' + os.path.join(_thirdPartyDir, "boost_python.zip"), '' ],
        [0, 'bjam -j%i release --with-python --with-thread --with-date_time --prefix=%s' % (NUMBER_OF_CORES, PREFIX), boostDir ],
        [0, 'echo This may take a while -- copying each include file individually!!', ''],
        [0, _sudo + ' bjam  install --with-python  --prefix=%s' % (PREFIX), boostDir ],
        ]
        
        
        
class boost(module):
    """Boost module, also included bjam."""
    #if Config._SVN:
    #    version = "1.41r57399"
    #else:
    version = "1.42.0"
    ModuleName = ""

    if isWindows():
        PATH_LIB_THREAD = Config.PATH_LIB_Thread_STATIC
        PATH_LIB_DATETIME = Config.PATH_LIB_date_time_STATIC
        base = 'boost_1_42_0'
        if _DEBUG:
            lib = 'boost_python-vc90-mt-gd-1_42'
        else:
            lib = 'boost_python-vc90-mt-1_42'
        versionBase = '1_42' ## the version used on the library name
    else:
        base = 'boost_1_41_0'
        versionBase = '1_41' ## the version used on the library name

    if isLinux():
        bjambuildset = 'gcc'

        if is64():
            bjambase = 'boost-jam-3.1.17'
            bjambuilddir = 'bin.linuxx86_64'
        else:
            bjambase = 'boost-jam-3.1.17-1-linuxx86'
            bjambuilddir = ''

        source = [
            [wget, 'http://downloads.sourceforge.net/boost/boost-jam-3.1.17-1-linuxx86.tgz', downloadPath],
            # this isn't used unless building from source -- however small download :)
            [wget, 'http://downloads.sourceforge.net/boost/boost-jam-3.1.17.tgz', downloadPath],
            [wget, 'http://downloads.sourceforge.net/boost/' + base + '.tar.gz', downloadPath],
            ## only doing an export not a co to save bandwidth
            ##[svn, ' export http://svn.boost.org/svn/boost/trunk boost-trunk',os.getcwd()]
        ]

        buildCmds = [
            ## first handle bjam
            [0, tar + ' zxf ' + os.path.join(downloadPath, bjambase) + '.tgz --overwrite', ''],
        ]

        # No prebuilt binaries for amd64
        if is64():
            buildCmds += [
                [0, "./build.sh " + bjambuildset, os.path.join(os.getcwd(), bjambase)],
                [0, "mkdir -p %s/bin/" % PREFIX, os.path.join(os.getcwd(), bjambase)],
            ]

        buildCmds += [
            [0, cp + " bjam %s/bin/" % PREFIX, os.path.join(os.getcwd(), bjambase, bjambuilddir)],

            ## and now boost
            [0, tar + ' zxf ' + os.path.join(downloadPath, base) + '.tar.gz', ''],
           ## [0, 'chmod -R +rw *', os.path.join(os.getcwd(), base)],
            ##[0, PREFIX + '/bin/bjam release install --with-python --with-thread --with-date_time --prefix='+PREFIX, os.path.join(os.getcwd(), base) ],
            ##[0, PREFIX + '/bin/bjam release install --with-python --with-thread --with-date_time --prefix='+PREFIX, os.path.join(os.getcwd(), 'boost-trunk') ],
            [0, PREFIX + '/bin/bjam -j%i release install --with-python --with-thread --with-date_time --prefix=%s' % (NUMBER_OF_CORES, PREFIX), os.path.join(os.getcwd(), base) ],
           # [0, 'make', os.path.join(os.getcwd(), base)],
           # [0, 'make install', os.path.join(os.getcwd(), base)],
        ]

    if isMac():
        bjambase = 'boost-jam-3.1.17-1-macosxx86'
        bjambuilddir = 'bin.macosxx86'
        bjambuildset = 'darwin'

        source = [
            #[wget, 'http://downloads.sourceforge.net/boost/boost-jam-3.1.17-1-macosxx86.tgz', downloadPath],
            [wget, 'http://downloads.sourceforge.net/boost/' + base + '.tar.gz', downloadPath]
        ]

        buildCmds = [
            ## first handle bjam ## REMOVED as using bootstrap instead
            ##[0, tar + ' zxf ' + os.path.join(downloadPath, bjambase) + '.tgz', ''],
            ##[0, cp + " bjam %s/bin/" % PREFIX, os.path.join(os.getcwd(), bjambase)], ## may need to change on 64 bit systems

            ## and now boost
            [0, tar + ' zxf ' + os.path.join(downloadPath, base) + '.tar.gz', ''],
            [0, 'chmod -R +rw *', os.path.join(os.getcwd(), base) ],
            [0, './bootstrap.sh  --without-icu --with-libraries=python,thread,date_time --prefix=%s' % (PREFIX), os.path.join(os.getcwd(), base) ],
            [0, './bjam -j%i release install architecture=%s address-model=%s --with-python --with-thread --with-date_time --prefix=%s' % (NUMBER_OF_CORES, MAC_ARCH_b, MAC_ADDR, PREFIX), os.path.join(os.getcwd(), base) ],
#            [0, "./configure --with-toolset=darwin --with-libraries=python,thread,date_time --prefix=%s --without-icu --with-bjam=../root/usr/bin/bjam" % PREFIX, os.path.join(os.getcwd(), base)],
#            [0, 'make', os.path.join(os.getcwd(), base)],
#            [0, 'make install', os.path.join(os.getcwd(), base)],
        ]

    if isWindows():
        bjambase = 'boost-jam-3.1.17-1-ntx86'
        source = [
            [wget, 'http://downloads.sourceforge.net/boost/' + bjambase + '.zip', downloadPath],
            [wget, 'http://downloads.sourceforge.net/boost/' + base + '.zip', downloadPath]
        ]

        if BOOST_STATIC:
            _link = "static"
        else:
            _link = "shared"
        buildCmds = [
            [0, unzip + os.path.join(downloadPath, bjambase + ".zip"), ''],
            [0, unzip + os.path.join(downloadPath, base + ".zip"), ''],
            [0, os.path.join(os.getcwd(), bjambase, "bjam.exe") + ' release --with-python --with-thread --with-date_time link=' + _link, os.path.join(os.getcwd(), base)] # --toolset=msvc-8
        ]

    # If we are not given a LIB path, use the version we are building.
    if Config.PATH_Boost is None:
        PATH = os.path.join(Config.LOCAL_INCLUDE, "boost") ## -%s" % versionBase)
    else:
        PATH = Config.PATH_Boost

    # If we are not given a LIB path, use the version we are building.
    if Config.PATH_LIB_Boost is None:
        PATH_LIB = Config.LOCAL_LIB
    else:
        PATH_LIB = Config.PATH_LIB_Boost

    if not isWindows():
        #print "*****", Config.LIB_Boost, Config
        # If we are not given a LIB_Boost, use the version we are building.
        if Config.LIB_Boost is None:
            if isMac():  # latest version of boost build on mac generate a 'nice' boost lib name
                lib = "libboost_python"
            else:
                import subprocess
                gccp = subprocess.Popen(["gcc", "--version"], stdout=subprocess.PIPE)
                import re
                gcc_version = re.search(r'([0-9]+?\.[0-9]+?\.[0-9]+?)', gccp.communicate()[0]).groups()[0].split('.')
                lib = "boost_python-gcc%s%s-mt" % (gcc_version[0], gcc_version[1])
        else:
            lib = Config.LIB_Boost
    #lib = "libboost_python"
####################################################
##
## Now for the actual Python-Ogre Modules
##
####################################################

class ogre(pymodule):
    ModuleName = 'OGRE'

    moduleParentLocation = "renderer"
    parent = "ogre/renderer"
    dependsOn = ['boost']
    myLibraryPaths = []
    myLibraries = ['OgreMain']
    libraries = myLibraries
    if isWindows():
        version = "1.7.0"
        source = [
            [ wget, "http://downloads.sourceforge.net/ogre/ogre-v1-7-0.zip", downloadPath],
            [ wget, "http://downloads.sourceforge.net/ogre/OgreDependencies_VC9_Eihort_20080203.zip", downloadPath],
        ]

        buildCmds = [
            [0, unzip + os.path.join(downloadPath, "ogre-v1-7-0.zip"), os.getcwd()  ],
            [0, unzip + os.path.join(downloadPath, "OgreDependencies_VC9_Eihort_20080203.zip"), Config.PATH_Ogre  ],
            [0, 'echo Please use MSVC Express Edition to build Ogre Release.', '']
        ]
        try:
            if Config._SVN:
                version = "1.7.x-SVN"
        except:
            pass
        # requirements to build a precompiled header on the fly
        if _PRECOMPILED:
            pchstop = 'python_ogre_precompiled.h'
            pchbuild = 'buildpch.cpp'
            pchincludes = ['python_ogre_precompiled.h']

        if _DEBUG:
            libs = [boost.lib, 'OgreMain_d', 'OgreProperty_d']
        else:
            libs = [boost.lib, 'OgreMain', 'OgreProperty']
#         if not Config._SVN:
#             moduleLibs = [os.path.join(boost.PATH_LIB, boost.lib),
#                           os.path.join(Config.PATH_LIB_Ogre_OgreMain, 'OgreMain'),
#                           os.path.join(Config.PATH_LIB_Ogre_OgreMain, 'cg'),
#                           ]
#         else:
        if _DEBUG:
            moduleLibs = [os.path.join(boost.PATH_LIB, boost.lib),
                          os.path.join(Config.PATH_Ogre, 'bin', 'Debug', 'OgreMain_d'),
                          os.path.join(Config.PATH_Ogre, 'bin', 'Debug', 'OgreProperty_d'),
                          os.path.join(Config.PATH_Ogre, 'bin', 'Release', 'cg'),
                          ]
        else:
            moduleLibs = [os.path.join(boost.PATH_LIB, boost.lib),
                          os.path.join(Config.PATH_Ogre, 'bin', 'Release', 'OgreMain'),
                          os.path.join(Config.PATH_Ogre, 'bin', 'Release', 'OgreProperty'),
                          os.path.join(Config.PATH_Ogre, 'bin', 'Release', 'cg'),
                          ]
        lib_dirs = [
            boost.PATH_LIB,
            Config.PATH_LIB_Ogre_OgreMain,
            Config.PATH_LIB_Ogre_OgreMain_debug,

        ]
        include_dirs = [
            boost.PATH,
            Config.PATH_INCLUDE_Ogre,
            python_include_dirs,
        ]
        LINKFLAGS = ''

    elif isLinux():
        version = "1.7.0"
        base = "ogre-v1-7-0p1"
        baseDir = 'ogre'
        ##https://sourceforge.net/projects/ogre/files/ogre/1.7/ogre-v1-7-0RC1.tar.bz2/download

        source = [
            [wget, "http://downloads.sourceforge.net/ogre/" + base + ".tar.bz2", downloadPath],
        ]

#         buildCmds = [
#             [0, tar + " jxf " + os.path.join(downloadPath, base) + ".tar.bz2 --overwrite", os.getcwd() ],
#             [0, "patch -s -N -i ./python-ogre/patch/ogre_%s.patch -p0 " % version, os.getcwd()],
#             [0, "aclocal", os.path.join(os.getcwd(), 'ogre')],
#             [0, "./bootstrap", os.path.join(os.getcwd(), 'ogre')],
#             [0, "./configure --prefix=%s --disable-devil" % PREFIX, os.path.join(os.getcwd(), 'ogre')], #--with-gui=Xt
#             [0, "make -j%i" % NUMBER_OF_CORES, os.path.join(os.getcwd(), 'ogre')],
#             [0, _sudo + " make install", os.path.join(os.getcwd(), 'ogre')],
#         ]

        buildCmds = [
            [0, tar + " jxf " + os.path.join(downloadPath, base) + ".tar.bz2 --overwrite", os.getcwd() ],
            [0, "cmake . -DOGRE_INSTALL_PLUGINS_HEADERS=TRUE -DCMAKE_INSTALL_PREFIX:PATH=%s" % PREFIX, baseDir],
            [0, "make", baseDir],
            [0, _sudo + " make install", baseDir]
            ]
        libs = [boost.lib, 'OgreMain', 'OgreProperty']
        lib_dirs = [Config.LOCAL_LIB]
        include_dirs = [boost.PATH,
                        Config.PATH_INCLUDE_Ogre,
                        python_include_dirs,
                        ]
        CCFLAGS = ' -D__PYTHONOGRE_BUILD_CODE '


    elif isMac():
        version = "1.6.4"
        base = "ogre-v1-6-4"
        basedep = "OgreDependencies_OSX_Eihort_20080115"
        source = [
            [wget, "http://downloads.sourceforge.net/ogre/" + base + ".tar.bz2", downloadPath],
            [wget, "http://downloads.sourceforge.net/ogre/" + basedep + ".zip", downloadPath],
        ]
        buildCmds = [
            [0, tar + " jxf " + os.path.join(downloadPath, base) + ".tar.bz2 ", os.getcwd() ],
            [0, unzip + os.path.join(downloadPath, basedep) + ".zip ", os.path.join(os.getcwd(), 'ogre') ],
            [0, "mkdir ~/Library/Frameworks", ''], ## Note that this will fail if the directory exists !!!
            [0, 'mv OgreDependencies_OSX_Eihort_20080115/Dependencies .', os.path.join(os.getcwd(), 'ogre')],
            [0, "patch -s -N -i ./python-ogre/patch/ogre_1.6.4.patch -p0 ", os.getcwd()],
            [0, "mkdir Ogre", os.path.join(os.getcwd(), 'ogre', 'OgreMain', 'include') ],
            # need copies of these in an 'Ogre/..' directory due to includes in the OSX directory -- or get the framework right
            [0, "cp OgreRoot.h Ogre", os.path.join(os.getcwd(), 'ogre', 'OgreMain', 'include') ],
            [0, "cp OgreRenderSystem.h Ogre", os.path.join(os.getcwd(), 'ogre', 'OgreMain', 'include') ],
            [0, "cp OgrePrerequisites.h Ogre", os.path.join(os.getcwd(), 'ogre', 'OgreMain', 'include') ],
            [0, "xcodebuild -project ogre/Mac/Ogre/Ogre.xcodeproj -configuration Release -sdk %s" % (MAC_SDK), ''],
            [0, "xcodebuild -project ogre/Mac/Samples/Samples.xcodeproj -configuration Release -sdk %s" % (MAC_SDK), ''],
            [0, "cp -R *.framework ~/Library/Frameworks", os.path.join(os.getcwd(), 'ogre', "Mac", "build", "Release") ],
            [0, "cp -R *.framework ~/Library/Frameworks", os.path.join(os.getcwd(), 'ogre', "Dependencies") ]
        ]

        libs = [boost.lib]
        lib_dirs = [ boost.PATH_LIB ]
        include_dirs = [
            boost.PATH,
            Config.PATH_INCLUDE_Ogre,
            python_include_dirs,
        ]

        LINKFLAGS = ''

class ois(pymodule):
    version = "1.0"

    parent = "ogre/io"
    if isMac():
        base = "ois"
        source = [
            [cvs, "-z3 -d:pserver:anonymous@wgois.cvs.sourceforge.net:/cvsroot/wgois co -D 01Jan2009 -P ois", os.getcwd()]
        ]
        buildCmds = [
            [0, "xcodebuild -project ois/Mac/XCode-2.2/OIS.xcodeproj -configuration Release -sdk %s " % (MAC_SDK), ''],
            [0, 'cp -R ois/Mac/XCode-2.2/build/Release/OIS.Framework ~/Library/Frameworks ', '']
        ]

    if isLinux():
        base = "ois"
        source = [
            [cvs, "-z3 -d:pserver:anonymous@wgois.cvs.sourceforge.net:/cvsroot/wgois co -D 01Jan2009 -P ois", os.getcwd()]
        ]

        buildCmds = [
            [0, "./bootstrap", os.path.join(os.getcwd(), base)],
            [0, "./configure --prefix=%s --includedir=%s/include" % (PREFIX, PREFIX), os.path.join(os.getcwd(), base)],
            [0, 'make', os.path.join(os.getcwd(), base)],
            [0, _sudo + ' make install', os.path.join(os.getcwd(), base)]
        ]

    if isWindows():
        base = "ois"
        source = [
            [cvs, "-z3 -d:pserver:anonymous@wgois.cvs.sourceforge.net:/cvsroot/wgois co -D 01Jan2009 -P ois", os.getcwd()]
        ]

        buildCmds = [
            [0, 'echo Use MSVC to build OIS', '']
        ]

    if isWindows():
        if _PRECOMPILED:
            pchstop = 'OIS.h'
            pchbuild = 'buildpch.cpp'
            pchincludes = ['boost/python.hpp', 'OIS.h']

        if Config.SDK:
            if _DEBUG:
                libs = ['OIS_d', boost.lib]
            else:
                libs = ['OIS', boost.lib]
        else:
            libs = ['OIS_static', boost.lib]
        libs.append("User32") # needed for static linking
    elif isLinux():
        libs = ['OIS', boost.lib]
    else: # MAC
        libs = ['OIS.dylib', boost.lib]

    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_OIS,
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_OIS,
    ]
    if _DEBUG:
        lib_dirs.append(Config.PATH_LIB_OIS_debug,
        )
    ModuleName = 'OIS'
    if os.sys.platform == 'darwin':
        LINKFLAGS = '-framework IOKit '


class cegui(pymodule):
    parent = "ogre/gui"
    version = "0.7.1"

    if isWindows():
        version = "0.7.1"
        if _PRECOMPILED:
            pchstop = 'cegui.h'
            pchbuild = 'buildpch.cpp'
            pchincludes = ['boost/python.hpp', 'cegui.h']
        bp = os.path.join(Config.PATH_LIB_CEGUI, '..', 'bin')
        if _DEBUG:
            libs = [boost.lib, 'CEGUIBase_d', 'OgreMain_d', 'msvcrtd']
            moduleLibs = [os.path.join(bp, 'CEGUIBase_d'),
                os.path.join(bp, 'CEGUIExpatParser_d'),
                os.path.join(bp, 'CEGUIFalagardWRBase_d'),
                ]
        else:
            libs = [boost.lib, 'CEGUIBase', 'OgreMain']
            moduleLibs = [os.path.join(bp, 'CEGUIBase'),
                os.path.join(bp, 'CEGUIExpatParser'),
                os.path.join(bp, 'CEGUIFalagardWRBase'),
                ]
        if PythonOgreMajorVersion == "1" and int(PythonOgreMinorVersion) < 7 and not Config._SVN:
            libs.append('OgreGUIRenderer')
            moduleLibs.append(os.path.join(Config.PATH_LIB_Ogre_OgreMain, 'OgreGUIRenderer'))
        else:
            if _DEBUG:
                libs.append('CEGUIOgreRenderer_d')
                moduleLibs.append(os.path.join(bp, 'CEGUIOgreRenderer_d'))
            else:
                libs.append('CEGUIOgreRenderer')
                moduleLibs.append(os.path.join(bp, 'CEGUIOgreRenderer'))

    elif isLinux():
        libs = [boost.lib, 'CEGUIBase', 'OgreMain', 'CEGUIOgreRenderer' ]
    else:
        libs = [boost.lib, 'OgreMain' ]

    if isLinux() or isMac():
        base = "CEGUI-0.7.1"
        source = [
            [wget, "http://prdownloads.sourceforge.net/crayzedsgui/CEGUI-0.7.1.tar.gz?download", downloadPath]
        ]

    if isLinux() :
        buildCmds = [
            [0, tar + " zxf " + os.path.join(downloadPath, base) + ".tar.gz --overwrite", os.getcwd() ],
            #[0, "echo 'EMPTY' >>./INSTALL", os.path.join(os.getcwd(), base)],
            #[0, "echo 'EMPTY' >>./NEWS", os.path.join(os.getcwd(), base)],
            [0, "./bootstrap", os.path.join(os.getcwd(), base)],
            [0, "./configure --prefix=%s --enable-freeimage=yes --enable-ogre-renderer=yes --with-default-xml-parser=TinyXMLParser  --includedir=%s/include" % (PREFIX, PREFIX), os.path.join(os.getcwd(), base)],
            [0, 'make', os.path.join(os.getcwd(), base)],
            [0, _sudo + ' make install', os.path.join(os.getcwd(), base)]
        ]

    if isMac():
        buildCmds = [
            [0, tar + " zxf " + os.path.join(downloadPath, base) + "b.tar.gz --overwrite", os.getcwd() ],
            [0, 'echo YOU DO NOT NEED TO BUILD CEGUI - YOU CAN USE THE OGRE DEPENCIES ', '']
        ]

    if isWindows():
        version = "0.7.1"
        base = "CEGUI-0.7.1"
        source = [
            [wget, "http://prdownloads.sourceforge.net/crayzedsgui/CEGUI-0.7.1.zip?download", downloadPath],
            [wget, "http://prdownloads.sourceforge.net/crayzedsgui/CEGUI-DEPS-0.7.x-r1-vc9.zip?download", downloadPath]
        ]
        buildCmds = [
            [0, unzip + " " + os.path.join(downloadPath, "CEGUI-0.7.1.zip"), os.getcwd()],
            [0, unzip + " " + os.path.join(downloadPath, "CEGUI-DEPS-0.7.x-r1-vc9.zip"), os.path.join(os.getcwd(), base) ],
            #[0, 'move CEGUI-0.6.0\dependencies .', os.path.join(os.getcwd(), base) ],
            [0, sed_ + ' -s "s/OGRE_RENDERER = false/OGRE_RENDERER = true/" config.lua ', os.path.join(os.getcwd(), base, 'projects', 'premake')],
            [0, sed_ + ' -s "s/SAMPLES_OGRE = false/SAMPLES_OGRE = true/" config.lua ', os.path.join(os.getcwd(), base, 'projects', 'premake')],
            [0, 'echo Now run build_vs2008.bat from the CEGUI projects\premake directory', ''],
            [0, 'echo then use MSVC to build the CEGUI Release project', ''],
            [0, 'echo WARNING - Check that RTTI is enabled in the compiler options', ''],
            [0, 'echo Then ensure that OgreGUIRender is compiled with this version of CEGUI', '']
        ]
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_CEGUI,
        Config.PATH_CEGUI,
        Config.PATH_INCLUDE_Ogre_CEGUIRenderer,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_Ogre_Dependencies, ## needed as OgreCEGUI uses CEGUI/.. in #includes
    ]

    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_CEGUI,
        Config.PATH_LIB_Ogre_Dependencies,
    ]
    if _DEBUG:
        lib_dirs.append(Config.PATH_LIB_Ogre_OgreMain_debug)
    ModuleName = 'CEGUI'
    if isMac():
        LINKFLAGS = ' -framework CEGUI -framework OgreCEGUIRenderer '



class ode(pymodule):
    version = "0.11.1"
    parent = "ogre/physics"
    odeLibraryName = 'ode'
    libs = [boost.lib, odeLibraryName]

    if isWindows():
        odeLibraryName = 'ode_single'
        libs = [boost.lib, odeLibraryName, 'User32'] # needed for MessageBox function
        moduleLibs = [os.path.join(boost.PATH_LIB, boost.lib)]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_ODE,
    ]
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_ODE,
        os.path.join(Config.PATH_ODE, 'OPCODE'),
    ]

    ModuleName = 'ODE'
    baseDir = os.path.join(os.getcwd(), "ode-0.11")
    if not isWindows():
        source = [
            [wget, "http://downloads.sourceforge.net/opende/ode-0.11.tar.gz", downloadPath]
        ]

        buildCmds = [
            # FIXME: Should this untar not use the unTarGz method mentioned above?
            [0, 'tar  zxf ' + os.path.join(downloadPath, "ode-0.11.tar.gz"), ''],
            [0, "chmod +x autogen.sh", baseDir],
            [0, "./autogen.sh", baseDir],
            [0, "./configure --prefix=%s --includedir=%s/include" % (PREFIX, PREFIX), baseDir],
            [0, "make", baseDir],
            [0, "make install", baseDir]
        ]
    else:
        source = [
            [wget, "http://downloads.sourceforge.net/opende/ode-0.11.zip", downloadPath]
        ]
        buildCmds = [
            [0, unzip + os.path.join(downloadPath, "ode-0.11.zip"), ''],
        ]

    CCFGLAGS = ' -DBAN_OPCODE_AUTOLINK -DICE_NO_DLL '
    if isMac():
        noFrameWorks = True;

class opcode(pymodule):
    version = "1.3"
    name = 'opcode'
    parent = "ogre/physics"
    libs = [boost.lib]
    lib_dirs = [boost.PATH_LIB]
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_OPCODE,
        os.path.join(Config.PATH_INCLUDE_OPCODE, 'Ice'),
    ]
    CCFLAGS = ' -DBAN_OPCODE_AUTOLINK -DICE_NO_DLL '
    ModuleName = 'Opcode'

class caelum(pymodule):
    version = "r451"
    parent = "ogre/addons"
    cflags = ""
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_caelum,
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
    ]
    if isLinux():
        libs = [boost.lib, 'OgreMain' ]
    else:
        libs = [boost.lib, 'OgreMain' ]

    ModuleName = "caelum"

class ogreode(pymodule):
    version = "r2684"
    cflags = ""
    parent = "ogre/physics"
    base = 'ogreaddons/ogreode'
    baseDir = os.path.join(os.getcwd(), base)

    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_OgreOde,
        Config.PATH_LIB_OgreOdePrefab,
        Config.PATH_LIB_OgreOdeLoader,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_ODE,
    ]
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_ODE,
        Config.PATH_INCLUDE_OgreOde,
        Config.PATH_INCLUDE_OgreOdePrefab,
        Config.PATH_INCLUDE_OgreOdeLoader,
        Config.PATH_INCLUDE_Ogre,
    ]
    source = [
       [svn, " co http://ogreaddons.svn.sourceforge.net/svnroot/ogreaddons/trunk/ogreode " + baseDir, os.getcwd()]
    ]
    buildCmds = [
       [0, "patch -s -N -i ../../python-ogre/patch/ogreode.patch -p0", baseDir],
       [0, "chmod +x autogen.sh", baseDir],
       [0, "./autogen.sh", baseDir],
       [0, "./configure --without-SAMPLES --prefix=%s" % PREFIX, baseDir],
       [0, "make", baseDir],
       [0, "make install", baseDir],
    ]
    if isWindows():
        buildCmds = [
            [0, "patch -s -N -i ../../python-ogre/patch/ogreode.patch -p0", baseDir],
        ]
    libs = [boost.lib, 'OgreMain', 'OgreOde_Core', 'OgreOde_Prefab', 'OgreOde_Loader' ]
    if isLinux():
        libs.append('ode')
    else:
        libs.append('ode_single')

    ModuleName = 'OgreOde'
    moduleLibs = [os.path.join(Config.PATH_OgreOde,'bin','Release', 'OgreOde_Core'),
                    os.path.join(Config.PATH_OgreOde,'bin','Release', 'OgreOde_Prefab'),
                    os.path.join(Config.PATH_OgreOde, 'bin','Release','OgreOde_Loader')
                    ]

class quickgui(pymodule):
    version = "10.1"
    name = 'quickgui'
    parent = "ogre/gui"

    ## note the defined for _QuickGUIExport forces non dll usage
    if os.name == 'nt':
        ## note the defined for _QuickGUIExport forces non dll usage
        CCFLAGS = ' -DWIN32 -DNDEBUG -DWINDOWS -D_QuickGUIExport=""  -D_PRECOMP '
    else:
        CCFLAGS = ' -D_QuickGUIExport="" '
    cflags = ""
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_quickgui
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain
    ]
    libs = [ boost.lib, 'OgreMain' ]
    ModuleName = "QuickGUI"

class navi(pymodule):
    active = False
    version = "head"
    parent = "ogre/gui"
    CCFLAGS = '-D"WIN32" -D"NDEBUG", -D"WINDOWS", -D"_WIN32", -D"NAVI_DYNAMIC_LIB" '
    cflags = ""
    base = 'navi'

    source = [
        [svn, " checkout http://navi.googlecode.com/svn/trunk/ " + base, os.getcwd()]
    ]

    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_navi,
        os.path.join(Config.PATH_navi, '..', 'Dependencies', 'win32', 'awesomium', 'include'),
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_navi,
        os.path.join(Config.PATH_navi, '..', 'Dependencies', 'win32', 'awesomium', 'lib', 'release')
    ]
    libs = [  boost.lib, 'Navi_DLL', 'OgreMain', 'Awesomium', 'user32',
            'kernel32.lib', 'gdi32.lib', 'winspool.lib', 'comdlg32.lib', 'advapi32.lib',
            'shell32.lib', 'ole32.lib', 'oleaut32.lib', 'uuid.lib' ]
    ModuleName = "navi"


class betagui(pymodule):
    version = "2"
    name = 'betagui'
    parent = "ogre/gui"
    cflags = ""
    active = False # no one seems to be using this so removing support (11/09)
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_OIS,
        Config.PATH_INCLUDE_betagui
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_betagui,
        Config.PATH_LIB_OIS
    ]
    if Config.SDK:
        libs = [  boost.lib, 'OgreMain', 'OIS' ]
    else:
        libs = [  boost.lib, 'OgreMain', 'OIS_static' ]
    if os.name == "nt":
        libs.append("User32") # needed for static linking

    ModuleName = "betagui"

class ogreforests(pymodule):
    version = "r2719"
    name = 'ogreforests'
    parent = "ogre/addons"
    cflags = ""
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_ogreforests
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain
    ]
    libs = [  boost.lib, 'OgreMain' ]
    ModuleName = "ogreforests"


class particleuniverse(pymodule):
    version = "0.81"
    active = False
    name = 'particleuniverse'
    parent = "ogre/addons"
    cflags = ""
    source = [
        [wget, "http://www.fxpression.com/files/download/ParticleUniverseV0.81.zip", downloadPath]
    ]

    buildCmds = [
        [0, unzip + os.path.join(downloadPath, "ParticleUniverseV0.81.zip"), Config.PATH_Ogre],
        [0, "patch -s -N -i " + os.path.join(os.getcwd(), "python-ogre", "patch", "ParticleUniverse_0.8.patch") + " -p0 ", os.path.join(Config.PATH_Ogre, 'Plugins', 'ParticleUniverse') ],
        [0, "echo *** NOW build ParticleUniverse with MSVC from " + Config.PATH_Ogre, '']
    ]
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_particleuniverse
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_particleuniverse
    ]
    libs = [  boost.lib, 'OgreMain', 'ParticleUniverse' ]
    ModuleName = "particleuniverse"


class nxogre(pymodule):
    active = False
    version = "0.22T5"
    parent = "ogre/physics"
    cflags = ""
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_NxOgre,
    ]

    for d in Config.PATH_INCLUDE_PhysX:
        include_dirs.append(d)

    if os.name == 'nt':
        CCFLAGS = ' -DWIN32 '
    else:
        CCFLAGS = ' -DNX_LINUX -DLINUX -DNX_DISABLE_FLUIDS '

    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_NxOgre,
        Config.PATH_LIB_PhysX
    ]

    if os.name == 'nt':
        libs = [boost.lib, 'NxCharacter', 'NxCooking', 'PhysXLoader', 'OgreMain', 'NxOgre' ]
    else:
        libs = [boost.lib, 'NxCharacter', 'NxCooking', 'PhysXCore', 'PhysXLoader', 'OgreMain', 'NxOgre' ]
    ModuleName = "NxOgre"


class ogrevideo(pymodule):
    version = "r109"
    parent = "ogre/addons"
    base = 'ogrevideo'
    ##
    ## you will need to retreive and build ptypes for this modules
    ##http://www.melikyan.com/ptypes/ptypes-2.1.1.tar.gz
    ##svn co http://ogrevideo.svn.sf.net/svnroot/ogrevideo/trunk ogrevideo
    baseDir = os.path.join(os.getcwd(), base)

    cflags = ""
    name = 'ogrevideo'
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_ogrevideo,
        Config.PATH_DEMO_ogrevideo,
        Config.PATH_INCLUDE_libtheoraplayer,
    ]

    for d in Config.PATH_INCLUDE_OggVorbisTheora:
        include_dirs.append(d)

    source = [
        [svn, " co http://ogrevideo.svn.sf.net/svnroot/ogrevideo/trunk " + baseDir, os.getcwd()],
        [wget, "http://www.melikyan.com/ptypes/ptypes-2.1.1.tar.gz", downloadPath]
    ]
    buildCmds = [
         [0, "tar zxf " + os.path.join(downloadPath, "ptypes-2.1.1.tar.gz"), ''],
    ]

    if os.name == 'nt':
        CCFLAGS = ' -D"WIN32" '

    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_ogrevideo,
        Config.PATH_LIB_OPENAL
    ]
    libs = [boost.lib, 'Plugin_OgreVideo', 'OgreMain', 'openal32' ]
    moduleLibs = [ os.path.join(Config.PATH_OPENAL, 'redist', 'OpenAL32'),
                os.path.join(Config.PATH_OPENAL, 'redist', 'wrap_oal')
                ]
    ModuleName = "ogrevideo"

class plib(pymodule):
    version = "1.8.4"
    parent = "ogre/addons"
    cflags = ""
    CCFLAGS = ' -D"_CRT_SECURE_NO_WARNINGS" '
    include_dirs = [
        Config.PATH_INCLUDE_plib,
        Config.PATH_THIRDPARTY, # needed as net.h includes via plib/xxxx
    ]
    lib_dirs = [
        boost.PATH_LIB,
    ]
    ModuleName = "plib"

    if os.name == "nt":
        libs = [ boost.lib, 'winmm', 'User32', 'wsock32', 'opengl32.lib', 'glu32.lib' ]
    else:
        libs = [ boost.lib, "GL" ] # Note sure what linux libraries are needed

class physx(pymodule):
    active=False
    version = "2.8.1"
    parent = "ogre/physics"
    cflags = ""
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
    ]
    for d in Config.PATH_INCLUDE_PhysX:
        include_dirs.append(d)
    if os.name == 'nt':
        CCFLAGS = ' -D"WIN32" '
    else:
        CCFLAGS = ' -D"LINUX" -D"NX_USE_SDK_DLLS" -D"_USRDLL" -D"NX_DISABLE_FLUIDS" '
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_PhysX
    ]

    if isWindows():
        libs = [boost.lib, 'NxCharacter', 'NxCooking', 'PhysXLoader' ]
        source = [
            [wget, "http://developer.download.nvidia.com/PhysX/2.8.1/PhysX_2.8.1_SDK_Core.msi", downloadPath]
        ]
        buildCmds = [
            [0, os.path.join(downloadPath, "PhysX_2.8.1_SDK_Core.msi"), ''  ],
            [0, 'sed -i s/"#ifdef WIN32"/"#if (defined WIN32) \&\& !(defined __PYTHONOGRE_BUILD_CODE)"/ NxMath.h',
                            'c:\\program files\\NVIDIA Corporation\\NVIDIA PhysX SDK\\v2.8.1\\SDKs\\Foundation\\include']
        ]
        moduleLibs = [os.path.join(boost.PATH_LIB, boost.lib)]
    elif isLinux():
        libs = [boost.lib, 'NxCharacter', 'NxCooking', 'PhysXCore', 'PhysXLoader' ]
        source = [
            [wget, "http://developer.download.nvidia.com/PhysX/2.8.1/PhysX_2.8.1_SDK_CoreLinux_deb.tar.gz", downloadPath]
        ]

        buildCmds = [
            [0, "tar zxf " + os.path.join(downloadPath, "PhysX_2.8.1_SDK_CoreLinux_deb.tar.gz"), ''],
            [0, "sudo dpkg -i  libphysx-common_2.8.1-4_i386.deb", ''],
            [0, "sudo dpkg -i  libphysx-2.8.1_4_i386.deb", ''],
            [0, "sudo dpkg -i  libphysx-dev-2.8.1_4_i386.deb", ''],
            [0, "sudo dpkg -i  libphysx-extras-2.8.1_4_i386.deb", '']
        ]

    ModuleName = "PhysX"
    moduleLibs = [os.path.join(Config.PATH_LIB_PhysX, '../../../bin/win32', 'NxCharacter'),
                    os.path.join(Config.PATH_LIB_PhysX, '../../../bin/win32', 'NxCooking'),
                    os.path.join(Config.PATH_LIB_PhysX, '../../../bin/win32', 'PhysXLoader')
                    ]

class ogreal(pymodule):
    version = "r130"
    cflags = ''

    parent = "ogre/sound"

    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_OgreAL,
        Config.PATH_INCLUDE_OGG,
        Config.PATH_INCLUDE_VORBIS,
        Config.PATH_INCLUDE_OPENAL,
    ]

    if isWindows():
        lib_dirs = [ boost.PATH_LIB
                , Config.PATH_LIB_Ogre_OgreMain
                # change with latest ogg -- 
                , os.path.join(Config.PATH_OGG, 'win32', 'VS2008', 'Win32', 'Release')
                , os.path.join(Config.PATH_VORBIS, 'win32', 'VS2008', 'Win32', 'Release') #'Vorbis_Static_Release')
                #, os.path.join(Config.PATH_VORBIS, 'win32', 'VorbisEnc_Static_Release')
                #, os.path.join(Config.PATH_VORBIS, 'win32', 'VorbisFile_Static_Release')
                , os.path.join(Config.PATH_OPENAL, 'Release') #'libs','Win32')
                , Config.PATH_LIB_OgreAL
                , Config.PATH_LIB_OPENAL
                  ]
        CCFLAGS = ' -DWIN32 -DNDEBUG -D_LIB -D_WIN32 -D_WINDOWS ' #-DVORBIS_IEEE_FLOAT32 -D_USE_NON_INTEL_COMPILER '
        libs = [
            boost.lib,
            'OgreMain',
            'libogg_static',
            'libvorbis_static',
            'libvorbisfile_static',
            'OpenAL32',
        ] # -- going to compile OgreAL ourselves
        source = [
            ["wget", "http://connect.creativelabs.com/openal/Downloads/OpenAL11CoreSDK.zip", downloadPath],
            ["wget", "http://downloads.xiph.org/releases/ogg/libogg-1.1.4.zip", downloadPath],
            ["wget", "http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.3.zip", downloadPath],
        ]
        buildCmds = [
            [0, "unzip -q -o  " + os.path.join(downloadPath, "OpenAL11CoreSDK.zip"), ''],
            [0, "OpenAL11CoreSDK.exe", ''],
            [0, "unzip -q -o  " + os.path.join(downloadPath, "libogg-1.1.4.zip"), ''],
            [0, "ren libogg-1.1.4 ogg", ''],
            [0, "unzip -q -o  " + os.path.join(downloadPath, "libvorbis-1.2.3.zip"), ''],
            [0, "ren libvorbis-1.2.3 vorbis", ''],
        ]
        moduleLibs = [os.path.join(Config.PATH_OPENAL, 'redist', 'OpenAL32'),
                    os.path.join(Config.PATH_OPENAL, 'redist', 'wrap_oal')
                    ]
    else:
        lib_dirs = [
            boost.PATH_LIB,
            Config.PATH_LIB_Ogre_OgreMain,
        ]
        source = [
            ["wget", "http://connect.creativelabs.com/openal/Downloads/openal-soft-1.6.372.tar.bz2", downloadPath],
            ["wget", "http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz", downloadPath],
            ["wget", "http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz", downloadPath],
        ]
        buildCmds = [
            [0, "tar jxf " + os.path.join(downloadPath, "openal-soft-1.6.372.tar.bz2"), ''],
            [0, "tar zxf " + os.path.join(downloadPath, "libogg-1.1.3.tar.gz"), ''],
            [0, "tar zxf " + os.path.join(downloadPath, "libvorbis-1.2.0.tar.gz"), ''],
            [0, "./configure --prefix=%s\nmake\nmake install" % PREFIX, os.path.join(os.getcwd(), "libogg-1.1.3")],
            [0, "./configure --prefix=%s\nmake\nmake install" % PREFIX, os.path.join(os.getcwd(), "libvorbis-1.2.0")],
            [0, "cmake -DCMAKE_INSTALL_PREFIX:PATH=" + PREFIX, os.path.join(os.getcwd(), 'openal-soft-1.6.372')],
            [0, "make ", os.path.join(os.getcwd(), 'openal-soft-1.6.372')],
            [0, "cp -p libopenal* %s/lib " % PREFIX, os.path.join(os.getcwd(), 'openal-soft-1.6.372')],
            [0, "cp -p * %s/include " % PREFIX, os.path.join(os.getcwd(), 'openal-soft-1.6.372', 'include', 'AL')]
        ]
        libs = [
            boost.lib,
            'OgreMain',
            'vorbisfile',
            'openal'
        ]  ##  'OgreAL' -- going to compile OgreAL ourselves
    ModuleName = 'OgreAL'


class ogrevideoffmpeg(pymodule):
    version = "0.2.1"
    name = 'ogrevideoffmpeg'
    parent = "ogre/addons"
    CCFLAGS = ""
    ## "http://ffmpeg.arrozcru.org/builds/shared/ffmpeg-r16537-gpl-lshared-win32.tar.bz2"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_ogrevideoffmpeg,
        Config.PATH_INCLUDE_ffmpeg,
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_ogrevideoffmpeg,
        Config.PATH_LIB_ffmpeg
    ]
    if os.name == 'nt':
        libs = [boost.lib, 'OgreMain', 'libavformat', 'libavcodec', 'libavutil',
            'libavdevice', 'libswscale', 'liba52', 'libmingwex', 'libfaac', 'libfaad',
            'libz', 'libxvidcore', 'libx264', 'libpthreadGC2', 'libwsock32', 'libvorbis', 'libmp3lame', 'libogg',
            'libvorbisenc', 'libvorbisfile', 'libtheora', 'libgsm', 'libvfw32', 'libgcc' ]
        libs = [boost.lib, 'OgreMain', 'avcodec-52', 'avdevice-52', 'avformat-52', 'avutil-49', 'swscale-0' ]
    else:
        libs = [boost.lib, 'OgreMain', 'avformat', 'avcodec', 'avutil', 'z', 'GL', 'GLU', 'Xxf86vm', 'Xext', 'X11' ]
    ModuleName = "ogrevideoffmpeg"
    moduleLibs = [ os.path.join(Config.PATH_THIRDPARTY, 'extra', 'bin', 'avcodec-52'),
                os.path.join(Config.PATH_THIRDPARTY, 'extra', 'bin', 'avdevice-52'),
                os.path.join(Config.PATH_THIRDPARTY, 'extra', 'bin', 'avformat-52'),
                os.path.join(Config.PATH_THIRDPARTY, 'extra', 'bin', 'avutil-49'),
                os.path.join(Config.PATH_THIRDPARTY, 'extra', 'bin', 'libogg-0'),
                os.path.join(Config.PATH_THIRDPARTY, 'extra', 'bin', 'swscale-0')
                ]

class ogredshow(pymodule):
    active = False
    version = "0.1"
    parent = "ogre/addons"
    if os.name == 'nt':
        ## note the defined for _ETManagerExport forces non dll usage
        CCFLAGS = ' -DWIN32 -DNDEBUG -DWINDOWS -D_ETManagerExport="" '
    else:
        CCFLAGS = ''
    cflags = ""
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_ogredshow
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
    ]
    libs = [boost.lib, 'OgreMain', 'Strmiids', 'ole32']
    ModuleName = "ogredshow"

class et(pymodule):  ## editable terrain
    active=False
    version = "2.3.1"
    parent = "ogre/addons"
    if isLinux():
        libs = [ boost.lib, 'OgreMain' ]
    else:
        libs = [ boost.lib, 'OgreMain' ]

    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain
    ]
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_et
    ]
    ModuleName = 'et'

class bullet(pymodule):
    """ WIndows Build Notes: -- 
    patch src/LineraMath/btScaler.h -- around line 39 (removes the align 16 issues) to look like:
        #ifdef WIN32

            #if 1 || defined(__MINGW32__) || defined(__CYGWIN__) || (defined (_MSC_VER) && _MSC_VER < 1300)

    build ReleaseDLL and copy generated libs into a single directory ie (PATH_Bullet, 'out/release_dll8/libs')
    """
    version = "2.76"
    base = "bullet-" + version
    baseDir = os.path.join(os.getcwd(), base)
    parent = "ogre/physics"
    libs = [boost.lib, 'libBulletCollision', 'libBulletDynamics', 'libBulletSoftBody', 'libBulletMultiThreaded', 'libGIMPACTUtils']
    if isWindows():
        libs.append('libbulletMath')
    else:
        libs.append('libLinearMath')

    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Bullet,
    ]
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Bullet
    ]

    if not isWindows():
        source = [
            [wget, "http://bullet.googlecode.com/files/" + base + ".tgz", downloadPath]
        ]
        buildCmds = [
            [0, "tar zxf " + os.path.join(downloadPath, base) + ".tgz", ''],
            [0, "cmake . -DCMAKE_INSTALL_PREFIX:PATH=%s" % PREFIX, baseDir],
            [0, "make", baseDir],
            [0, "find . -name *.a -execdir cp {} %s/lib \;" % PREFIX, baseDir]
        ]
        CCFLAGS = ' -D"NDEBUG" -D"_CONSOLE" -D"_MBCS"  '
    else:
        libs = [boost.lib, 'BulletCollision', 'BulletDynamics',
                'BulletSoftBody', 'BulletMultiThreaded', 'GIMPACTUtils',
                'LinearMath', 'BulletFileLoader']
        source = [
            [wget, "http://bullet.googlecode.com/files/" + base + ".zip", downloadPath]
        ]
        buildCmds = [
            [0, unzip + os.path.join(downloadPath, base) + ".zip", ''],
        ]
        CCFLAGS = ' -D"NDEBUG" -D"_CONSOLE"  -D"WIN32" -D"_MBCS" /fp:fast /Gy /GF '

    ModuleName = 'bullet'

class ogrebulletc(pymodule):
    version = "r2684"
    parent = "ogre/physics"
    libs = [boost.lib, 'OgreMain',
        'libBulletCollision', 'libBulletDynamics', 'libBulletMultiThreaded',
        'libBulletSoftBody',
        'libConvexDecomposition'
        ]
    if isLinux():
       libs.append('libLinearMath')
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Bullet,
        os.path.join(Config.PATH_INCLUDE_Bullet, 'BulletCollision'),
        os.path.join(Config.PATH_OgreBullet, 'Collisions'),
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_Ogre_Dependencies,
        os.path.join(Config.PATH_Bullet, "Extras", "ConvexDecomposition"),
    ]

    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Bullet,
        Config.PATH_LIB_Ogre_OgreMain,
    ]
    if isWindows():
        CCFLAGS = ' -DWIN32 -DNDEBUG -D_WINDOWS -D_PRECOMP '
        LINKFLAGS = '  /NODEFAULTLIB:LIBCMT.lib  '
        libs = [boost.lib, 'OgreMain','BulletCollision', 'BulletDynamics',
                'BulletSoftBody', 'BulletMultiThreaded', 'GIMPACTUtils',
                'LinearMath', 'BulletFileLoader','ConvexDecomposition']
    else:
        CCFLAGS = ' -D_PRECOMP -fno-inline '
    ModuleName = 'OgreBulletC'


class ogrebulletd(pymodule):
    version = "r2684"
    name = 'ogrebulletd'
    parent = "ogre/physics"

    libs = [boost.lib, 'OgreMain',
        'libBulletCollision', 'libBulletDynamics', 'libBulletSoftBody',
        'libBulletMultiThreaded', 'libConvexDecomposition'
        ]
    if isWindows():
        libs = [boost.lib, 'OgreMain','BulletCollision', 'BulletDynamics',
                'BulletSoftBody', 'BulletMultiThreaded', 'GIMPACTUtils',
                'LinearMath', 'BulletFileLoader','ConvexDecomposition']
    else:
        libs.append('libLinearMath')

    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Bullet,
        os.path.join(Config.PATH_OgreBullet, 'Collisions'),
        os.path.join(Config.PATH_OgreBullet, 'Dynamics'),
        os.path.join(Config.PATH_INCLUDE_Bullet, 'BulletCollision'),
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_Ogre_Dependencies,
        os.path.join(Config.PATH_Bullet, "Extras", "ConvexDecomposition"),
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Bullet,
        Config.PATH_LIB_Ogre_OgreMain,
    ]
    if isWindows():
        CCFLAGS = ' -DWIN32 -DNDEBUG -D_WINDOWS -D_PRECOMP '
        #LINKFLAGS = '  /NODEFAULTLIB:LIBCMT.lib '
    else:
        CCFLAGS = ' -D_PRECOMP -fno-inline '
    ModuleName = 'OgreBulletD'
    descText = "Ogre Bullet Dynamics (physics) implementation"

class noise(pymodule):
    version = "r2486"
    parent = "ogre/addons"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_noise,
        os.path.join(Config.PATH_INCLUDE_noise, 'model'),
        os.path.join(Config.PATH_INCLUDE_noise, 'module')
    ]
    lib_dirs = [
        boost.PATH_LIB
    ]
    libs = [boost.lib]
    ModuleName = "noise"
    descText = "Generate 'noise'"

class watermesh(pymodule):
    active = False
    version = "1.0"
    parent = "ogre/addons"
    include_dirs = [
        boost.PATH,
        Config.PATH_watermesh,
        Config.PATH_INCLUDE_Ogre,
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain
    ]
    libs = [boost.lib, 'OgreMain']
    ModuleName = "watermesh"
    descText = "C++ code from Ogre Water demo -- use Hydrax instead"

class ofusion(pymodule):
    version = "1.0"
    name = 'ofusion'
    parent = "ogre/addons"
    include_dirs = [
        boost.PATH,
        Config.PATH_ofusion,
        Config.PATH_INCLUDE_Ogre
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
    ]
    libs = [boost.lib, 'OgreMain' ]
    ModuleName = "ofusion"
    descText = "Import Ogre 'Scenes' from 3dMax using the ofusion exporter"

class cadunetree(pymodule):
    version = "0.6"
    name = 'cadunetree'
    parent = "ogre/addons"
    include_dirs = [
        boost.PATH,
        Config.PATH_cadunetree,
        Config.PATH_INCLUDE_Ogre
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
    ]
    libs = [  boost.lib, 'OgreMain' ]
    ModuleName = "cadunetree"
    descText = "Generate realistic trees"

class ogrepcz(pymodule):
    version = ogre.version
    name = 'ogrepcz'
    parent = "ogre/renderer"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_ogrepcz
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_pcz
    ]
    libs = [boost.lib, 'Plugin_PCZSceneManager', 'OgreMain' ]
    ModuleName = "ogrepcz"
    descText = "Portal SceneManager - seamlessly move from interior to exterior scene management"

class opensteer(pymodule):
    active = False
    version = "1.0"
    parent = "ogre/addons"
    cflags = ""
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_opensteer,
        Config.PATH_INCLUDE_opensteer_opensteer
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_opensteer
    ]
    libs = [  boost.lib, 'opensteer' ]
    ModuleName = "opensteer"

class hydrax(pymodule):
    version = "0.5.1"
    parent = "ogre/addons"
    cflags = ""
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_hydrax,
        Config.PATH_INCLUDE_Ogre,
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
    ]
    libs = [  boost.lib, 'OgreMain' ]
    ModuleName = "hydrax"
    descText = "Fantastic water/ocean effects"

    if isWindows():
        CCFLAGS = " /fp:fast "
        LINKFLAGS = "  /LTCG /DYNAMICBASE:NO /MACHINE:X86 "

    # testing dll linkage..
    if False:
        libs.append('Hydrax')
        lib_dirs.append(Config.PATH_LIB_hydrax)
        for p in Config.PATH_INCLUDE_hydrax_modules:
            include_dirs.insert(0, p)

class hikari(pymodule):
    active=True
    version = "r23"
    name = 'hikari'
    parent = "ogre/gui"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_hikari,
        Config.PATH_INCLUDE_Ogre,
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_hikari
    ]
    libs = [  boost.lib, 'OgreMain', 'comsuppw', 'Gdi32' ] ##, 'hikari' ]
    ModuleName = "hikari"
    descText = "Use Flash controls within Ogre"
    descLink = "http://hikari-library.googlecode.com/"

class mygui(pymodule):
    active = False
    version = "3.0"
    parent = "ogre/gui"
    source = [
        ["wget", "http://downloads.sourceforge.net/my-gui/MyGUI2.2.3_source.zip", downloadPath],
    ]
    buildCmds = [
        [0, unzip + os.path.join(downloadPath, "MyGUI2.2.3_source.zip"), '']
    ]
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_mygui,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_Ogre_Dependencies,
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_mygui,
    ]
    libs = [  boost.lib, 'OgreMain', 'MyGUI' ] ##, 'hikari' ]
    ModuleName = "mygui"
    descText = "MyGUI Interface System"
    descLink = "http://my-gui.sourceforge.net/"
    moduleLibs = [os.path.join(Config.PATH_mygui, 'Shared', 'release', 'MyGUI')]

class canvas(pymodule):
    version = "1.0"
    parent = "ogre/gui"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_canvas,
        Config.PATH_INCLUDE_Ogre,
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
    ]

    if isWindows(): # have put freetype into the SVN tree to make it easier on windows
        include_dirs.append (os.path.join(Config.PATH_THIRDPARTY, 'freetype', 'include'))
        lib_dirs.append (os.path.join(Config.PATH_THIRDPARTY, 'freetype', 'lib'))

        libs = [boost.lib, 'OgreMain', 'freetype235']
    else:
        libs = [boost.lib, 'OgreMain', 'freetype']
        include_dirs.append ('/usr/include/freetype2')

    ModuleName = "canvas"
    descText = "Canvas GUI System"
    descLink = "http://www.ogre3d.org/phpBB2/viewtopic.php?t=41365&postdays=0&postorder=asc&start=0&sid=6578000180a935734beb03d548b900a4"

class raknet(pymodule):
    """ windows build notes:--
    patch source/NativeTypes.h ro read:
            #if !defined(__PYTHONOGRE_BUILD_CODE) and (defined(__GNUC__)  || defined(__GCCXML__) || defined(__SNC__))
            #include <stdint.h>
            #endif
    """
    version = "3.7171"
    name = 'raknet'
    parent = "ogre/addons"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_raknet,
        Config.PATH_INCLUDE_Ogre,
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_Ogre_Dependencies,
        Config.PATH_LIB_raknet,
    ]
    libs = [boost.lib, 'OgreMain', 'RakNetLibStatic', 'Ws2_32' ]
    ModuleName = "raknet"
    descText = "RakNet MultiPlayer Gaming System"
    descLink = "http://www.jenkinssoftware.com/"
    CCFLAGS = ' -D_RELEASE -D_RAKNET_DLL -D_CRT_NONSTDC_NO_DEPRECATE -D_WIN32'
    LINKFLAGS = '  /NODEFAULTLIB:LIBCMT.lib '


class doctester(pymodule):
    active = False
    version = "1"
    name = 'doctester'
    parent = "ogre/addons"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
    ]
    lib_dirs = [
        boost.PATH_LIB
    ]
    libs = [  boost.lib, 'OgreMain']
    ModuleName = "doctester"
    descText = "A test environment for documentation stringa"

class plsm2(pymodule):
    active = False
    version = ogre.version
    parent = "ogre/renderer"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_plsm2
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_plsm2
    ]
    CCFLAGS = ' -D_PLSM_OCTREE -DNDEBUG -D_WINDOWS -D__PYTHONOGRE_BUILD_CODE -DPLUGIN_PAGINGLANDSCAPE2_EXPORTS'
    libs = [  boost.lib, 'Plugin_OctreeSceneManager', 'OgreMain' ]
    ModuleName = "plsm2"
    descText = "Paging Landscape SceneManager"

class ogreoctreesm(pymodule):
    version = ogre.version
    if not Config._SVN:
        active = False
    name = 'ogreoctreesm'
    parent = "ogre/renderer"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_ogreoctreesm
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
    ]
    libs = [boost.lib, 'Plugin_OctreeSceneManager', 'OgreMain' ]
    ModuleName = "ogreoctreesm"
    descText = "Octree Zone Namangement - Expose all functions in OctreeZone incase required"

class ogreoctreezone(pymodule):
    version = ogre.version
    if not Config._SVN:
        active = False
    active = False # need to write it !!
    name = 'ogreoctreezone'
    parent = "ogre/renderer"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_ogreoctreezone
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
    ]
    libs = [boost.lib, 'Plugin_OctreeZone', 'OgreMain' ]
    ModuleName = "ogreoctreezone"
    descText = "Octree Zone Management - Expose all functions in OctreeZone incase required"

class ogreoggsound(pymodule):
    version = "r274"
    active = True
    name = 'ogreoggsound'
    parent = "ogre/sound"
    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_ogreoggsound,
        Config.PATH_INCLUDE_OGG,
        Config.PATH_INCLUDE_VORBIS,
        Config.PATH_INCLUDE_OPENAL,
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_ogreoggsound
    ]
    moduleLibs = [os.path.join(Config.PATH_LIB_ogreoggsound, 'OgreOggSound')]
    libs = [boost.lib, 'OgreMain', 'OgreOggSound' ]
    ModuleName = "ogreoggsound"
    descText = "Ogre OGG Sound Module"

if ogre.version.startswith ("1.7"):
    class ogreterrain(pymodule):
        version = ogre.version
        name = 'ogreterrain'
        parent = "ogre/renderer"
        include_dirs = [
            boost.PATH,
            Config.PATH_INCLUDE_Ogre,
            Config.PATH_INCLUDE_ogreterrain,
            Config.PATH_INCLUDE_ogrepaging
        ]
        lib_dirs = [
            boost.PATH_LIB,
            Config.PATH_LIB_Ogre_OgreMain,
            Config.PATH_LIB_Ogre_OgreMain_debug,
        ]
        if _DEBUG:
            libs = [boost.lib, 'OgreTerrain_d','OgrePaging_d','OgreMain_d']
        else:
            libs = [boost.lib, 'OgreTerrain', 'OgrePaging', 'OgreMain' ]

        moduleLibs = [os.path.join(Config.PATH_Ogre, 'bin', 'Release', 'OgreTerrain'),
             ]

        ModuleName = "ogreterrain"
        descText = "OgreTerrain: New Terrain Manager in Ogre"

    class ogrepaging(pymodule):
        version = ogre.version
        name = 'ogrepaging'
        parent = "ogre/renderer"
        include_dirs = [
            boost.PATH,
            Config.PATH_INCLUDE_Ogre,
            Config.PATH_INCLUDE_ogrepaging
        ]
        lib_dirs = [
            boost.PATH_LIB,
            Config.PATH_LIB_Ogre_OgreMain,
            Config.PATH_LIB_Ogre_OgreMain_debug,
        ]
        if _DEBUG:
            libs = [boost.lib,'OgrePaging_d','OgreMain_d']
        else:
            libs = [boost.lib, 'OgrePaging', 'OgreMain' ]
        ModuleName = "ogrepaging"
        descText = "OgrePaging: New Paging Manager in Ogre"
        moduleLibs = [os.path.join(Config.PATH_Ogre, 'bin', 'Release', 'OgrePaging'),
             ]

    class ogrertshadersystem(pymodule):
        version = ogre.version
        name = 'ogrertshadersystem'
        parent = "ogre/renderer"
        include_dirs = [
            boost.PATH,
            Config.PATH_INCLUDE_Ogre,
            Config.PATH_INCLUDE_ogrertshadersystem
        ]
        lib_dirs = [
            boost.PATH_LIB,
            Config.PATH_LIB_Ogre_OgreMain,
        ]
        libs = [boost.lib, 'OgreRTShaderSystem', 'OgreMain', 'OgreProperty' ]
        ModuleName = "ogrertshadersystem"
        descText = "OgreRTShaderSystem: New Real Time Shader System in Ogre"
        moduleLibs = [os.path.join(Config.PATH_Ogre, 'bin', 'Release', 'OgreRTShaderSystem'),
             ]

    class ogresdksample(pymodule):
        version = ogre.version
        name = 'ogresdksample'
        parent = "ogre/renderer"
        include_dirs = [
            boost.PATH,
            Config.PATH_INCLUDE_sdksample,
            Config.PATH_INCLUDE_Ogre,
            #os.path.join(Config.PATH_INCLUDE_sdksample,'..','..','Browser','include'), #needed for filesystemimpl
            Config.PATH_INCLUDE_OIS
        ]
        lib_dirs = [
            boost.PATH_LIB,
            Config.PATH_LIB_Ogre_OgreMain,
            Config.PATH_LIB_Ogre_OgreMain_debug,
            Config.PATH_LIB_OIS
        ]
        if _DEBUG:
            libs = [boost.lib, 'OgreMain_d', 'OIS_d', 'User32']
        else:
            libs = [boost.lib, 'OgreMain', 'OIS', 'User32']
        ModuleName = "ogresdksample"
        descText = "OgreSDKSample: New SDK Sample Framework in Ogre"

    class skyx(pymodule):
        version = "0.1"
        name = 'skyx'
        parent = "ogre/addons"
        include_dirs = [
            boost.PATH,
            Config.PATH_INCLUDE_Ogre,
            os.path.join(Config.BASE_DIR, "SkyX", "SkyX", "SkyX")
        ]
        lib_dirs = [
            boost.PATH_LIB,
            Config.PATH_LIB_Ogre_OgreMain,
            os.path.join(Config.BASE_DIR, "SkyX", "SkyX", "SkyX", "bin")
        ]
        libs = [boost.lib, 'OgreMain', 'SkyX' ]
        moduleLibs = [os.path.join(Config.BASE_DIR, "SkyX", "SkyX", "SkyX", "bin", "SkyX")]
        ModuleName = "skyx"
        descText = "SkyX: Great sky"

    class awesomium(pymodule):
        active = False
        version = "1.5"
        name = 'awesomium'
        parent = "ogre/addons"
        include_dirs = [
            boost.PATH,
            Config.PATH_INCLUDE_Ogre,
            os.path.join(Config.BASE_DIR, "awesomium")
        ]
        lib_dirs = [
            boost.PATH_LIB,
            Config.PATH_LIB_Ogre_OgreMain,
            os.path.join(Config.BASE_DIR, "awesomium")
        ]
        libs = [boost.lib, 'OgreMain', 'Awesomium' ]
        moduleLibs = [os.path.join(Config.BASE_DIR, "awesomium")]
        ModuleName = "awesomium"
        descText = "Awesomium: WebGUI in Ogre "

        
class newton2 (module):
    source_version = "2.16"
    if isLinux():
        source_version = "2.13"
        source = [
            [wget, "http://www.newtondynamics.com/downloads/NewtonLinux-32-2.13.tar.gz", downloadPath],
        ]

        buildCmds = [
            [0, "tar zxf " + os.path.join(downloadPath, "NewtonLinux-32-2.13.tar.gz"), ''],
            #[0, "patch -s -i ./python-ogre/patch/Newton.patch -p0 ", ''],
            [0, "cp newtonSDK/sdk/Newton.h %s/include" % PREFIX, ''],
            [0, "cp newtonSDK/sdk/*.a %s/lib" % PREFIX, ''],
            [0, "cp newtonSDK/sdk/*.a ogreaddons/ogrenewt", ''],
        ]

class newton(module):
    source_version = "1.53"
    if isLinux():
        source = [
            [wget, "http://www.newtondynamics.com/downloads/newtonLinux-1.53.tar.gz", downloadPath],
        ]

        buildCmds = [
            [0, "tar zxf " + os.path.join(downloadPath, "newtonLinux-1.53.tar.gz"), ''],
            [0, "patch -s -i ./python-ogre/patch/Newton.patch -p0 ", ''],
            [0, "cp newtonSDK/sdk/Newton.h %s/include" % PREFIX, ''],
            [0, "cp newtonSDK/sdk/*.a %s/lib" % PREFIX, ''],
            [0, "cp newtonSDK/sdk/*.a ogreaddons/ogrenewt", ''],
        ]

    if isMac():
        source = [
            [wget, "http://www.newtondynamics.com/downloads/NewtonMac-1.53.zip", downloadPath]
        ]

        buildCmds = [
            [0, "unzip -q -o " + os.path.join(downloadPath, "NewtonMac-1.53.zip"), ''],
            [0, "patch -s -i ./python-ogre/patch/Newton.patch -p0 ", ''],
            [0, "cp newtonSDK/sdk/Newton.h %s/include" % PREFIX, ''],
            [0, "cp newtonSDK/sdk/*.a %s/lib" % PREFIX, ''],
            [0, "cp newtonSDK/sdk/*.a ogreaddons/ogrenewt", '']
        ]

    if isWindows():
        source = [
            [wget, "http://www.newtondynamics.com/downloads/NewtonWin-1.53.zip", downloadPath],
        ]


        buildCmds = [
            [0, unzip + os.path.join(downloadPath, "NewtonWin-1.53.zip"), ''],
            [0, "setup.exe", ''],
        ]
        
class ogrerefapp(pymodule):
    ## making this false as replaced by OgreODE etc..
    active = False

    version = ogre.version # same as the Ogre version
    name = 'ogrerefapp'
    parent = "ogre/physics"
    baseDir = os.path.join(os.getcwd(), 'ogrenew', 'ReferenceApplication')
    source = []
    buildCmds = [
        [0, "aclocal", baseDir],
        [0, "./bootstrap", baseDir],
        [0, "./configure --prefix=%s " % PREFIX, baseDir],
        [0, "make", baseDir],
        [0, "make install", baseDir],
    ]
    if isWindows():
        libs = [boost.lib, 'OgreMain', 'ode_single', 'ReferenceAppLayer']
    else:
        libs = [boost.lib, 'OgreMain', 'ode', 'ReferenceAppLayer']

    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Ogre_OgreMain,
        Config.PATH_LIB_ODE,
        Config.PATH_LIB_OgreRefApp,
    ]

    include_dirs = [
        boost.PATH,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_OgreRefApp,
        Config.PATH_INCLUDE_ODE,
    ]

    ModuleName = 'OgreRefApp'

class ogrenewt(pymodule):
    active=False
    version = "r2429"
    parent = "ogre/physics"
    base = 'ogreaddons/ogrenewt'
    if isWindows():
        libs = ['Newton', boost.lib, 'OgreNewt_Main', 'OgreMain']
        moduleLibs = [os.path.join(Config.PATH_LIB_Newton, 'Newton')]
    elif isLinux():
        libs = ['Newton', boost.lib, 'OgreNewt', 'OgreMain']
    else:
        libs = ['Newton32', boost.lib, 'OgreMain']

    source = [
        [svn, " co http://ogreaddons.svn.sourceforge.net/svnroot/ogreaddons/trunk/ogrenewt " + base, os.getcwd()]
    ]
    baseDir = os.path.join(os.getcwd(), base)
    buildCmds = [
        [0, "patch -s -N -i ../../python-ogre/patch/ogrenewt.patch -p0", baseDir],
        [0, rm + " -rf ./OgreNewt_Main/inc/boost", baseDir],
        [0, 'cp SConscript OgreNewt_Main', baseDir],
        [0, "scons prefix=%s boost=%s/include/boost-1_37 build" % (PREFIX, PREFIX), baseDir], ##WARNING -- boost include dir name is different than  build name (dash not underscore)
        [0, "scons prefix=%s boost=%s/include/boost-1_37 install" % (PREFIX, PREFIX), baseDir],
    ]

    if isWindows():
        buildCmds = [
            [0, "patch -s -N -i ../../python-ogre/patch/ogrenewt.patch -p0", baseDir],
            [0, "rmdir  /s /q .\\OgreNewt_Main\\inc\\boost", baseDir],
            [0, "echo   Now use MSVC to compile OgreNewt -- OgreNewt_vc71.sln", baseDir]
        ]

    include_dirs = [
        boost.PATH,
        Config.PATH_Newton,
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_OgreNewt,
        Config.PATH_INCLUDE_Ogre_Dependencies, #needed for OIS/OIS.h
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Newton,
        Config.PATH_LIB_OgreNewt,
        Config.PATH_LIB_Ogre_OgreMain,
    ]

    if isMac():
        include_dirs = [
            boost.PATH,
            Config.PATH_Newton,
            Config.PATH_INCLUDE_Ogre,
            Config.PATH_INCLUDE_Ogre_Dependencies, #needed for OIS/OIS.h
            Config.PATH_INCLUDE_OgreNewt
        ]
        lib_dirs = [
            boost.PATH_LIB,
            Config.PATH_LIB_Newton,
            Config.PATH_LIB_Ogre_OgreMain,
        ]

    if isMac():
        LINKFLAGS = ' -framework OIS '

    ModuleName = 'OgreNewt'

class ogrenewt2(pymodule):
    active=False
    version = "r2764_2.11"
    parent = "ogre/physics"
    base = 'ogreaddons/ogrenewt2'
    baseDir = os.path.join(os.getcwd(), base)

    if isWindows():
        libs = ['Newton', boost.lib, 'OgreNewt', 'OgreMain']
    elif isLinux():
        libs = ['Newton', boost.lib, 'OgreNewt', 'OgreMain']
        buildCmds = [
            [0, "cmake . -DCMAKE_INSTALL_PREFIX:PATH=%s" % PREFIX, baseDir],
            [0, "make", baseDir],
            [0, "make install", baseDir]
            ]
        CCFLAGS = ' -D_OGRENEWT_DYNAMIC -DOIS_NONCLIENT_BUILD '
    else:
        libs = ['Newton32', boost.lib, 'OgreMain']

    source = [
        [svn, " co http://svn.ogre3d.org/svnroot/ogreaddons/branches/ogrenewt/newton20 " + base, os.getcwd()]
    ]


    if isWindows():
        buildCmds = [
     #       [0, "patch -s -N -i ../../python-ogre/patch/ogrenewt.patch -p0", baseDir],
            [0, "echo   Now use MSVC to compile OgreNewt -- OgreNewt_vc71.sln", baseDir]
        ]
        CCFLAGS = ' -DWIN32 -D_OGRENEWT_DYNAMIC -DOIS_NONCLIENT_BUILD '


    include_dirs = [
        boost.PATH,
        Config.PATH_Newton2,
        os.path.join(Config.PATH_Newton2, 'dMath'),
        os.path.join(Config.PATH_Newton2, 'dAnimation'),
        os.path.join(Config.PATH_Newton2, 'dContainers'),
        os.path.join(Config.PATH_Newton2, 'dCustomJoints'),
        Config.PATH_INCLUDE_Ogre,
        Config.PATH_INCLUDE_OgreNewt2,
        Config.PATH_INCLUDE_Ogre_Dependencies, #needed for OIS/OIS.h
    ]
    lib_dirs = [
        boost.PATH_LIB,
        Config.PATH_LIB_Newton2,
        Config.PATH_LIB_OgreNewt2,
        Config.PATH_LIB_Ogre_OgreMain,
    ]
    moduleLibs = [os.path.join(Config.PATH_Newton2, 'x32', 'dll_vs9', 'newton'),
                os.path.join(Config.PATH_LIB_OgreNewt2, 'OgreNewt')
                ]
    if isMac():
        include_dirs = [
            boost.PATH,
            Config.PATH_Newton2,
            Config.PATH_INCLUDE_Ogre,
            Config.PATH_INCLUDE_Ogre_Dependencies, #needed for OIS/OIS.h
            Config.PATH_INCLUDE_OgreNewt2
        ]
        lib_dirs = [
            boost.PATH_LIB,
            Config.PATH_LIB_Newton2,
            Config.PATH_LIB_Ogre_OgreMain,
        ]

        LINKFLAGS = ' -framework OIS '

    ModuleName = 'ogrenewt2'
        
############################################################################################

## Here is the master list....
## Keep eveything here in lowercase so the over rides work :)

projects = {
    'ois' : ois,
    'ogre' : ogre,
    'cegui' : cegui,
    'ode' : ode,
#    'newton' : newton,
    'ogrerefapp' : ogrerefapp,
    'ogrenewt' : ogrenewt,
    'ogrenewt2' : ogrenewt2,
    'ogreode' : ogreode,
    'ogreal' : ogreal,
    'quickgui' : quickgui,
    'opcode' : opcode,
    'nxogre' : nxogre,
    'physx' : physx,
#    'nxogre_09' : nxogre_09,
    'bullet' : bullet,
    'betagui': betagui,
    'ogrevideo' : ogrevideo,
    'ogrevideoffmpeg' : ogrevideoffmpeg,
    'ogredshow' : ogredshow,
    'plib' : plib,
    'navi': navi,
    'ogrebulletc' : ogrebulletc,
    'ogrebulletd' : ogrebulletd,
    'ogreforests' : ogreforests,
    'et' : et,
    'caelum' : caelum,
    'noise' : noise,
    'watermesh' : watermesh,
    'ofusion' : ofusion,
    'particleuniverse' : particleuniverse,
    'cadunetree' : cadunetree,
    'opensteer' : opensteer,
    'ogrepcz' : ogrepcz,
    'hydrax' : hydrax,
    'hikari' : hikari,
    'mygui' : mygui,
    'canvas' : canvas,
    'raknet' : raknet,
    'doctester' : doctester,
    'plsm2' : plsm2,
    'ogreoctreezone' : ogreoctreezone,
    'ogreoctreesm' : ogreoctreesm,
    'ogreoggsound' : ogreoggsound,

}
if ogre.version.startswith ("1.7"):
    projects['ogrepaging'] = ogrepaging
    projects['ogreterrain'] = ogreterrain
    projects['ogrertshadersystem'] = ogrertshadersystem
    projects['ogresdksample'] = ogresdksample
    projects['skyx'] = skyx
    projects['awesomium'] = awesomium

#
# a couple of specials that should be done differently
gccxml_bin = Config.gccxml_bin
pyplusplus_install_dir = Config.pyplusplus_install_dir

rpath = Config.RPATH

# FIXME: The stuff below is kinda hacky, we should rewrite/rework it.
for name, cls in projects.items():
    # little hack to allow overriding of settings from the PythonOgreConfig_xxxxx.py file
    if hasattr(Config, name):   # look to see if the class exists in the config file
        print "Using Override for class ", name
        _class = Config.__dict__[name]  # get the class
        for key, value in _class.__dict__.items():
            if not key.startswith('_'):
                cls.__dict__[key] = value
                print "Set %s.%s to %s" % (name, key, value)


    cls.root_dir = os.path.join(root_dir, 'code_generators', name)
    cls.dir_name = name + '_' + str(cls.version)
    cls.generated_dir = os.path.join(generated_dir, cls.dir_name)
    cls.cache_file = os.path.join(declarations_cache_dir, cls.dir_name + '_cache.xml')

    if not hasattr(cls, 'CCFLAGS'):
        cls.CCFLAGS = ''
    if BOOST_STATIC:
        cls.CCFLAGS += " /DBOOST_PYTHON_STATIC_LIB "
    if isWindows():
        if not hasattr(cls, 'lib_dirs'):
            cls.lib_dirs = []
        cls.lib_dirs.append(boost.PATH_LIB)
        cls.lib_dirs.append(boost.PATH_LIB_THREAD)
        cls.lib_dirs.append(boost.PATH_LIB_DATETIME)

    if not hasattr(cls, 'cflags'):
        cls.cflags = ''
    if not hasattr(cls, 'LINKFLAGS'):
        cls.LINKFLAGS = ''

    if isMac():  # On the mac the Ogre library is lined in with the -framework command in scons
        try:
            cls.libs.remove('OgreMain')
        except:
            pass
        ## and we have a commond set of flags that I will set here...
        if Config.MAC_SDK_INCLUDE:
            cls.include_dirs.append(Config.MAC_SDK_INCLUDE)
        cls.CCFLAGS += Config.MAC_CCFLAGS
        cls.cflags += Config.MAC_cflags
        if not hasattr(cls, 'noFrameWorks'):
            for f in Config.MAC_AdditionalFrameWorks:
                cls.cflags += ' -F' + f + ' '
                cls.CCFLAGS += ' -F' + f + ' '
                cls.LINKFLAGS += ' -F' + f + ' '
    elif isWindows():
        cls.cflags += ' -D"_HAS_TR1=0" ' # to solve an issue with <map> include with MSVC 9 and advanced feature pack and gccxml ?????
        if Config._SVN: # building Ogre 1.7
            cls.include_dirs.insert (0, os.path.join (Config.PATH_Ogre, 'include')) # puts buildsettings.h in the path
            cls.CCFLAGS += " -DHAVE_OGRE_BUILDSETTINGS_H "

    if not hasattr(cls, 'ModuleName'):
        cls.ModuleName = name[0].upper() + name[1:]
    if not hasattr(cls, 'PydName'):
        cls.PydName = '_' + name.lower() + '_'
        if os.name == 'nt':
            cls.PydName = cls.PydName + '.pyd'
        elif os.name == 'posix':
            cls.PydName = cls.PydName + '.so'
        elif os.name == 'mac':
            print "WARNING - check the last line of environment.py!!"

