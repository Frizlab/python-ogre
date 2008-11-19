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
_PreCompiled = True


def log ( instring ):
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
    
##
## set this to True if you compiled Ogre with Threads enabled
##
if isWindows():
    _USE_THREADS = True
else:
    _USE_THREADS = False
    
                    
PythonOgreMajorVersion = "1"
PythonOgreMinorVersion = "6" 
PythonOgrePatchVersion = "0"


##
## these should be fine with auto create - however override them as necessary
##
PATH_Python = os.path.dirname( sys.executable )
## I want a version string 2.4 or 2.5 etc
PythonVersionString = str(sys.version_info[0]) + '.' + str(sys.version_info[1])

if isWindows(): 
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

UseSystem = "--usesystem" in sys.argv or (os.environ.has_key('USESYSTEM') and eval(os.environ['USESYSTEM'].title()))
if "--usesystem" in sys.argv:
    sys.argv.remove("--usesystem")

##
## Now we load the user specific stuff
##

# log ( "Config.PATH_Python: %s, UserName: %s, SystemType: %s, Root_dir: %s" % (Config.PATH_Python, _UserName, _SystemType, root_dir) )
try:
    s = 'PythonOgreConfig_' + _UserName ## + '.py'
    Config = __import__(  s  )
    _ConfigSet = True
    log ( "Loaded Config (based on username) from %s" % (s ))
except ImportError:
    try:
        if UseSystem:
            s= 'PythonOgreConfig_system'
        else:
            s= 'PythonOgreConfig_' + _SystemType ## + '.py'
       
        Config = __import__(  s  )
        _ConfigSet = True
        log ( "Loaded Config (based on systemtype) from %s" % (s))
    except ImportError:
        pass   

if not _ConfigSet:
    log ( "  Going to use internal config setting")
    ## These need to be adjusted based upon your system
    ##
    ## PLEASE use an external PythonOgreConfig_<username>.py to hold these value !!!!
    ## 
    print "\n\n You DO need to create a PythonOgreConfig_%s.py file with config details" % ( _SystemType)
    sys.exit(-1)

_STABLE = os.path.exists(os.path.join(os.path.split(__file__)[0], "STABLE")) # set to true if using specific versions of CVS and SVN checkouts..

######################
downloadPath = os.path.abspath("downloads") 
wget = "wget -c -nc "
tar = "tar "
gzip = "gzip "

if isWindows():
    mv = "move /Y "
    rm = "del /Q "
    cp = "copy /Y "
    unzip = "unzip -o "
    cvs = "cvs -z3 -q  "
    svn = "svn "
    VCBUILD="'c:\\Program Files\\Microsoft Visual Studio 8\\vc\\vcpackages\\vcbuild.exe'  /useenv  "
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
 
if isMac():    
    Config.PATH_Boost = os.path.join(Config.LOCAL_INCLUDE, 'boost-1_34_1')
    Config.LIB_Boost = 'libboost_python-1_34_1'
    Config.RPATH=""
    
## BIG assumption about where you want things put    
if UseSystem:
    ROOT = '/'
    PREFIX = '/usr'
else:
    ROOT = os.path.join(os.getcwd(),'root' )
    PREFIX = os.path.join(os.getcwd(),'root', 'usr' )


def unTarGz ( base, source ):
    """ a complete hack to cope with untar gziping a source file in the downloads directory into it's own directory
    Needed as the GNU tools for windows don't handle tar forking gzip or working on files that are in
    different directories
    """
    ret =[
        [0,cp + os.path.join(downloadPath, base +".tar.gz") + " " + os.getcwd(), '' ],
        [0,gzip + " -f -d " + os.path.join(os.getcwd(), base + ".tar.gz"), ''],
        [0,tar + " xf " + base + ".tar", '' ],
        [0,rm + base + ".tar", '']
        ]
    return ret

    


####################################################
##
## These are the base tools and libraries 
##
####################################################
class gccxml:
    pythonModule = False
    active = True
    base = 'gccxml'
    if _STABLE:
       source_version = "20080522"
       source = [
                [cvs, " -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML co -D 22May2008 "+base, os.getcwd()]
             ]
    else:
       source_version = "20080901"
       source = [
                [cvs, " -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML co  "+base, os.getcwd()]
             ]
    if isLinux() or isMac():
        buildCmds =  [
                    [0,"mkdir -p gccxml-build", ''],
                    [0,"cmake ../gccxml -DCMAKE_INSTALL_PREFIX:PATH="+ PREFIX,os.path.join(os.getcwd(),'gccxml-build')],
                    [0,"make", os.path.join(os.getcwd(),'gccxml-build')],
                    [0,"make install",os.path.join(os.getcwd(),'gccxml-build')]
                    ]   
    else:
        
        buildCmds = [ 
                    [0,'echo Please use CMAKE to create a msvc build projects for gccxml',''],
                    [0,'echo  ie. Run Cmake (from the windows menu)',''],
                    [0,'echo      specific the gccxml for both directories and run configure',''],
                    [0,'echo      ignore any errors - you will need to run configure twice and then OK',''],
                    [0,'echo      Now run MSVC and load the gccxml project (in the gccxml directory) and build it','']
                    ]
        
class install:
    pythonModule = False
    active = True
    source = [
                 ]
                 
    buildCmds =  [
                    [0,"python setup.py install --prefix=%s" % PREFIX, os.path.join(os.getcwd(),'python-ogre')]
                    ]  
                         
class newton:
    pythonModule = False
    active = True
#     if not os.path.exists( os.path.join(os.getcwd(), 'ogreaddons/ogrenewt')):
#         os.makedirs ( os.path.join(os.getcwd(), 'ogreaddons/ogrenewt') )    

    if isLinux():
        base = 'newton'
        source = [
                    [wget, "http://www.newtondynamics.com/downloads/newtonLinux-1.53.tar.gz", downloadPath]
                 ]
                 
        buildCmds =  [
            [0, "tar zxf " + os.path.join(downloadPath, "newtonLinux-1.53.tar.gz"), ''],
            [0,"patch -s -i ./python-ogre/patch/Newton.patch -p0 ", ''],
            [0, "cp newtonSDK/sdk/Newton.h %s/include" % PREFIX, ''],
            [0, "cp newtonSDK/sdk/*.a %s/lib" % PREFIX, ''],
            [0, "cp newtonSDK/sdk/*.a ogreaddons/ogrenewt",'']
            ]
    if isMac():
        base = 'newton'
        source = [
                    [wget, "http://www.newtondynamics.com/downloads/NewtonMac-1.53.zip", downloadPath]
                 ]
                 
        buildCmds =  [
            [0, "unzip -q -o " + os.path.join(downloadPath, "NewtonMac-1.53.zip"), ''],
            [0,"patch -s -i ./python-ogre/patch/Newton.patch -p0 ", ''],
            [0, "cp newtonSDK/sdk/Newton.h %s/include" % PREFIX, ''],
            [0, "cp newtonSDK/sdk/*.a %s/lib" % PREFIX, ''],
            [0, "cp newtonSDK/sdk/*.a ogreaddons/ogrenewt" , '']
            ]
            
    if isWindows():
        base = 'newton'
        source = [
                    [wget, "http://www.newtondynamics.com/downloads/NewtonWin-1.53.zip", downloadPath]
                 ]
                 
        buildCmds =  [
            [0, unzip + os.path.join(downloadPath, "NewtonWin-1.53.zip"), ''],
            [0, "setup.exe", ''],
            ]
            
class pygccxml:
    pythonModule = False
    active = True
    base = 'pygccxml'
    if _STABLE:
        source_version = "1383"
        source = [
                    [svn, " co -r 1383 https://pygccxml.svn.sourceforge.net/svnroot/pygccxml/pygccxml_dev "+base, os.getcwd()]
                 ]
    else:                 
        source_version = "1446"
        source = [
                    [svn, " co -r 1446 https://pygccxml.svn.sourceforge.net/svnroot/pygccxml/pygccxml_dev "+base, os.getcwd()]
                    ]             
    if isLinux() or isMac() :
        buildCmds =  [
# #                     [0, "patch -s -N -i ../python-ogre/patch/pygccxml.patch -p0 ", os.path.join(os.getcwd(), base) ],
                    [0,"python setup.py install  --prefix="+ PREFIX , os.path.join (os.getcwd(), base) ],
                    ]                       
    if isWindows():
        buildCmds =  [
# #                     [0, "patch -s -N -i ../python-ogre/patch/pygccxml.patch -p0 ", os.path.join(os.getcwd(), base) ],
                    [0,"python setup.py install  " , os.path.join (os.getcwd(), base) ],
                    ]                       
                    

class pyplusplus:
    pythonModule = False
    active = True
    base = 'pyplusplus'
    if _STABLE:
        source_version = "1383"
        source = [
                    [svn, " co -r 1383 https://pygccxml.svn.sourceforge.net/svnroot/pygccxml/pyplusplus_dev "+base, os.getcwd()]
                 ]
    else:                 
        source_version = "1446"
        source = [
                    [svn, " co -r 1446 https://pygccxml.svn.sourceforge.net/svnroot/pygccxml/pyplusplus_dev "+base, os.getcwd()]
                 ]             
    if isLinux() or isMac() :
        buildCmds =  [
                [0,"python setup.py install  --prefix=" + PREFIX , os.path.join (os.getcwd(), base) ]
                    ]                       
    if isWindows():
        buildCmds =  [
                [0,"python setup.py install  " , os.path.join (os.getcwd(), base) ]
                    ]                       
                    
class cg:
    ## we don't need this for Windows as it's part of the Ogre dependencies package
    
    pythonModule = False
    active = True
    if isLinux():
        base = 'Cg-2.0_May2008_x86'
        source = [
                    [wget, " http://developer.download.nvidia.com/cg/Cg_2.0/2.0.0015/"+base+".tgz",downloadPath]
                 ]
                 
        buildCmds =  [
                    [0,tar + " xvzf "+ os.path.join(downloadPath,base) + ".tgz --overwrite", ROOT],   # unpack it directly into 'our' root
                    ]    
    if isMac():
        source = [
                    [wget, "http://developer.download.nvidia.com/cg/Cg_2.0/2.0.0015/Cg-2.0_May2008.dmg", downloadPath ]
                    
                 ]                
        buildCmds = [
                    ]
## "http://downloads.sourceforge.net/ogre/OgreDependenciesOSX_20070929.zip"  

                    
class zziplib:
    ## The 'install' "fails" on the mac however it's actually OK
    pythonModule = False
    active = True
    if isLinux() or isMac():
        base = 'zziplib-0.13.49'
        source = [
                    [wget, " http://downloads.sourceforge.net/zziplib/"+base+".tar.bz2",downloadPath],
                 ]
                 
        buildCmds =  [
                    [0,tar + " xjf "+ os.path.join(downloadPath,base) + ".tar.bz2 --overwrite", ''],  
                    [0,"aclocal", os.path.join(os.getcwd(), base) ],
                    [0,"./configure --prefix=%s --datadir=%s/share --includedir=%s/include " % (PREFIX,PREFIX,PREFIX), os.path.join(os.getcwd(), base) ],
                    [0,"make" ,os.path.join(os.getcwd(), base) ],
                    [0,"make install",os.path.join(os.getcwd(), base) ]
                    ]    
                    
                        
class freeimage:
    pythonModule = False
    active = True
    if isLinux() or isMac():
        base = 'FreeImage'
        source = [
                    [wget, " http://downloads.sourceforge.net/freeimage/FreeImage3110.zip",downloadPath],
                 ]
                 
        if isLinux():
            buildCmds =  [
                    [0, unzip + " -q -o " + os.path.join(downloadPath,base) + "3110.zip", ''],
                    ##
                    # Note the use of a single command that does multiple sed's 
                    ##
                    [0, """sed --in-place -s 's|/usr/include|\$(DESTINC)|' Makefile.*\n\
                        sed --in-place -s 's|/usr/lib|\$(DESTLIB)|' Makefile.*\n\
                        sed --in-place -s 's|-o root -g root||' Makefile.*\n\
                        sed --in-place -s 's|ldconfig||' Makefile.*""", os.path.join(os.getcwd(), base) ],
                    [0, "make", os.path.join(os.getcwd(), base) ],
                    [0, "make DESTINC=%s/include DESTLIB=%s/lib install " % (PREFIX, PREFIX) ,os.path.join(os.getcwd(), base) ]
                    ]    
        if isMac(): ## mac has a crap sed command implementation and a different shell interaction, plus make file is a little different (or course)
            buildCmds =  [
                    [0, unzip + " -q -o " + os.path.join(downloadPath,base) + "3110.zip", ''],
                    ##
                    # Note the use of a single command that does multiple sed's 
                    ##
                    [0, """sed -i '' 's|/usr/local|$(DEST)|' Makefile.*\n\
                        sed -i '' 's|-o root -g wheel||' Makefile.*\n\
                        sed -i '' 's|ldconfig||' Makefile.*""", os.path.join(os.getcwd(), base) ],
                    [0, "make", os.path.join(os.getcwd(), base) ],
                    [0, "make DEST=%s install " % PREFIX ,os.path.join(os.getcwd(), base) ]
                    ]    
                    
                              
class cmake:
    pythonModule = False
    active = True
    if isLinux() or isMac():
        base = 'cmake-2.6.2-Linux-i386'
        
        if isMac():
            base = 'cmake-2.6.2-Darwin-universal'
            
        source = [
                    [wget, "http://www.cmake.org/files/v2.6/"+base+".tar.gz",downloadPath],
                 ]
    
                 
        buildCmds =  [
                    [0,tar + " xzf "+ os.path.join(downloadPath,base) + ".tar.gz --overwrite", ''],   # unpack it
                    [0,cp + "-R  * " + PREFIX, os.path.join (os.getcwd(), base) ]   # copy to our bin area
                    ]
    if isWindows():
        source = [ [wget, "http://www.cmake.org/files/v2.6/cmake-2.6.2-win32-x86.exe", downloadPath ]]
        buildCmds = [
                [0, os.path.join(downloadPath, "cmake-2.6.2-win32-x86.exe"), '' ]
                ]
                
                                
class scons:
    pythonModule = False
    active = True
    base = 'scons-1.0.1'
    source = [
                [wget, "http://prdownloads.sourceforge.net/scons/"+base+".tar.gz",downloadPath],
             ]
             
    # the utils in Windows don't handle paths or tar spawing gzip hence the work arounds             
    if isLinux() or isMac():
        buildCmds =  [
                [0, tar + " zxf " + os.path.join(downloadPath,base)+".tar.gz --overwrite",'' ],
                [0,"python setup.py install  --prefix=%s" % PREFIX , os.path.join (os.getcwd(), base) ]
                ]
    
    else:
        buildCmds =  unTarGz( base, downloadPath ) +\
                [
                [0,"python setup.py install ", os.path.join (os.getcwd(), base) ]
                ]
             
class boost:    ## also included bjam
    active = True
    version = "3.5"
    pythonModule = False
    ModuleName = ""

    if isWindows():
        if _STABLE:
            base = 'boost_1_36_0'
            lib = 'boost_python-vc90-mt-1_36'
            versionBase = '1_36' ## the version used on the library name
        else:        
            base = 'boost_1_36_0'
            lib = 'boost_python-vc90-mt-1_36'
            versionBase = '1_36' ## the version used on the library name
    else:
        if _STABLE:
            base = 'boost_1_34_1'
            versionBase = '1_34' ## the version used on the library name
        else:
            base = 'boost_1_36_0'
            versionBase = '1_36' ## the version used on the library name

    if isLinux() or isMac():
        bjambase = 'boost-jam-3.1.16'
        if isMac():
            bjambuilddir = 'bin.macosxx86'
            bjambuildset = 'darwin'
        else:
            bjambuilddir = 'bin.linuxx86'
            bjambuildset = 'gcc'
                    
        source = [
             [wget,'http://downloads.sourceforge.net/boost/boost-jam-3.1.16.tgz', downloadPath],
             [wget,'http://downloads.sourceforge.net/boost/'+base+'.tar.gz',downloadPath]
             ]   
        buildCmds  = [
                ## first handle bjam
                [0, tar + ' zxf ' + os.path.join(downloadPath, bjambase) + '.tgz --overwrite', ''],
                [0,"./build.sh " + bjambuildset, os.path.join(os.getcwd(), bjambase )],
                [0,"mkdir -p %s/bin/" % PREFIX, os.path.join(os.getcwd(), bjambase )],
                [0,cp + " bjam %s/bin/" % PREFIX, os.path.join(os.getcwd(), bjambase, bjambuilddir )], ## may need to change on 64 bit systems
                
                ## and now boost
                [0, tar + ' zxf ' + os.path.join(downloadPath, base) + '.tar.gz', ''],
                [0,'chmod -R +rw *', os.path.join(os.getcwd(), base ) ],
                [0, sed_ + " 's/BJAM_CONFIG=\"\"/BJAM_CONFIG=release/' "+base+"/configure", '' ],
                [0, sed_ + " s/'BOOST_PYTHON_MAX_ARITY 15'/'BOOST_PYTHON_MAX_ARITY 19'/ "+base+"/boost/python/detail/preprocessor.hpp", ''],
                [0, sed_ + ' s/"# include <boost\/preprocessor\/cat.hpp>"/"\\n#define BOOST_PYTHON_NO_PY_SIGNATURES\\n# include <boost\/preprocessor\/cat.hpp>"/ '+base+'/boost/python/detail/preprocessor.hpp', '' ],
                [0,"./configure --with-libraries=python,thread,date_time --prefix=%s --without-icu --with-bjam=../root/usr/bin/bjam"  % PREFIX, os.path.join(os.getcwd(), base )],
                [0,'make', os.path.join(os.getcwd(), base )],
                [0,'make install', os.path.join(os.getcwd(), base )],
                ]
                
    if isWindows():
        bjambase = 'boost-jam-3.1.16-1-ntx86'
        source = [
             [wget,'http://downloads.sourceforge.net/boost/'+bjambase+'.zip',downloadPath] ,
             [wget,'http://downloads.sourceforge.net/boost/'+base+'.zip',downloadPath]
             ] 
                         
        buildCmds =  [
                [0,unzip + os.path.join(downloadPath, bjambase+".zip"), ''],
                [0,unzip + os.path.join(downloadPath, base + ".zip"), ''],
                [0,'xcopy /s /i /y '+os.path.join('python-ogre','boost') + ' ' + base , ''],  # need to overwrite the boost with our files
                [0,'sed -i s/BJAM_CONFIG=\"\"/BJAM_CONFIG=release/ '+base+'/configure', '' ],
                [0,'sed -i s/"BOOST_PYTHON_MAX_ARITY 15"/"BOOST_PYTHON_MAX_ARITY 19"/ '+base+'/boost/python/detail/preprocessor.hpp', ''],
                [0,'sed -i s/"# include <boost\/preprocessor\/cat.hpp>"/"\\n#define BOOST_PYTHON_NO_PY_SIGNATURES\\n# include <boost\/preprocessor\/cat.hpp>"/ '+base+'/boost/python/detail/preprocessor.hpp', '' ],
                [0,'sed -i s/BJAM_CONFIG=\"\"/BJAM_CONFIG=release/ '+base+'/boost/python/detail/preprocessor.hpp', '' ],
                [0,os.path.join(os.getcwd(), bjambase, "bjam.exe") + ' release --with-python ',os.path.join(os.getcwd(),base)] # --toolset=msvc-8
                ]
    
    if not isWindows():
        # Figure out the gcc version we are running - this is needed by Boost
        # FIXME: This assumes that the gcc we are building with now was what we built boost with
        import subprocess
        gccp = subprocess.Popen (["gcc", "--version"], stdout=subprocess.PIPE)
        import re
        gcc_version = re.search(r'([0-9]+?\.[0-9]+?\.[0-9]+?)', gccp.communicate()[0]).groups()[0].split('.')
        # FIXME: Should this be the multithreaded version!? 
        lib = "boost_python-gcc%s%s-mt-%s" % (gcc_version[0], gcc_version[1], versionBase)

class boost_python_index:
    """ only used for Linx
    """
    active = True
    source_version = boost.versionBase.replace("_",".")
    pythonModule = False
    ModuleName = ""
    base = "libboost-python%s-index" % source_version #.replace(".", "_")
    if isLinux() or isMac():
        FLAGS = "PREFIX=%s BOOST_VERSION=%s BOOST_VERSION_PATH=%s MT=True" % (PREFIX, boost.versionBase,boost.versionBase)
        source = [
             ["rm","-rf %s" % (base,), os.getcwd()],
             ["cp",'-rvf %s/ %s' % (os.path.join('python-ogre','boost'), base), os.getcwd()],
             ]
        buildCmds  = [
            [0,'make all %s' % FLAGS, os.path.join(os.getcwd(), base)],
            [0,'make install %s' % FLAGS, os.path.join(os.getcwd(), base)],
        ]
   
    lib = boost.lib.replace("python", "python_index")

####################################################
##
## Now for the actual Python-Ogre Modules
##
####################################################
    
class ogre:
    active = True
    pythonModule = True
    name='ogre'
    ModuleName='OGRE'
    CCFLAGS = " "
    cflags=''
    moduleParentLocation = "renderer"
    parent = "ogre/renderer"
    dependsOn = ['boost']
    myLibraryPaths = []
    myLibraries = ['OgreMain']
    libraries = myLibraries
    
    if isWindows(): 
        
        if _STABLE or not _STABLE:   ## temp force to this version...
            version="1.6.0"
            source = [
                [ wget, "http://downloads.sourceforge.net/ogre/OgreDependencies_VC9_Eihort_20080203.zip", downloadPath],
                [ wget, "http://downloads.sourceforge.net/ogre/ogre-v1-6-0.zip", downloadPath],
                ]
            buildCmds  = [
                [0, unzip + os.path.join(downloadPath,"ogre-v1-6-0.zip"),os.getcwd() ],
                [0, unzip + os.path.join(downloadPath,"OgreDependencies_VC9_Eihort_20080203.zip"),
                                            os.path.join(os.getcwd(), 'ogre') ],
                [0, "patch -s -N -i ./python-ogre/patch/ogre_1.6.0.patch -p0 ", os.getcwd()],
                [0,'echo Please use MSVC Express Edition to build Ogre Release.','']
                ]                      
#         else:
#             
#             version="trunk"
#             source = [
#                 [ wget, "http://downloads.sourceforge.net/ogre/OgreDependencies_VC9_Eihort_20080203.zip", downloadPath],
#                 [ svn, "https://svn.ogre3d.org/svnroot/ogre/trunk", os.path.join(os.getcwd(), 'ogre')]
#                 ]
#             buildCmds  = [
#                     [0, unzip + os.path.join(downloadPath,"OgreDependencies_VC9_Eihort_20080203.zip"),
#                                                 os.path.join(os.getcwd(), 'ogre') ],
#                     [0, "patch -s -N -i ./python-ogre/patch/ogre_1.6.patch -p0 ", os.getcwd()],
#                     [0,'echo Please use MSVC Express Edition to build Ogre Release.','']
#                     ]                                             
            
        # requirements to build a precompiled header on the fly
        if _PreCompiled:
            pchstop = 'python_ogre_precompiled.h'
            pchbuild = 'buildpch.cpp'
            pchincludes = ['python_ogre_precompiled.h']

        libs=[boost.lib, 'OgreMain']
        lib_dirs = [ Config.PATH_LIB_Boost
                    ,  Config.PATH_LIB_Ogre_CEGUIRenderer
                    , Config.PATH_LIB_Ogre_OgreMain
                    , Config.PATH_LIB_Ogre_Dependencies #needed for ceguibase.lib etc
                    , Config.PATH_LIB_CEGUI
                     ]
        include_dirs = [ Config.PATH_Boost 
                    , Config.PATH_INCLUDE_Ogre 
                    , Config.PATH_INCLUDE_NEDMALLOC
                    ]
        CCFLAGS =  '  -DBOOST_PYTHON_MAX_ARITY=19 '
        LINKFLAGS = ''
        externalFiles=['OgreMain.dll', 'OgreGuiRender.dll', boost.lib+'.dll']
    elif isLinux():
        if _STABLE:
            version = "1.4"
            base = "ogre-v1-4-9"
            buildCmds  = [
                [0, tar + " jxf " + os.path.join(downloadPath,base)+".tar.bz2 --overwrite",os.getcwd() ],
                [0, "patch -s -N -i ./python-ogre/patch/ogre.patch -p0 ", os.getcwd()],
                [0, "aclocal", os.path.join(os.getcwd(), 'ogre')],
                [0, "./bootstrap", os.path.join(os.getcwd(), 'ogre')],
                [0, "./configure --prefix=%s --with-gui=Xt --disable-devil" % PREFIX, os.path.join(os.getcwd(), 'ogre')],
                [0, "make", os.path.join(os.getcwd(), 'ogre')],
                [0, "make install", os.path.join(os.getcwd(), 'ogre')],
                ]
        else:
            version = "1.6.0"
            base = "ogre-v1-6-0"
            buildCmds  = [
                [0, tar + " jxf " + os.path.join(downloadPath,base)+".tar.bz2 --overwrite",os.getcwd() ],
                [0, "patch -s -N -i ./python-ogre/patch/ogre_1.6.0.patch -p0 ", os.getcwd()],
                [0, "sed --in-place -s 's|#define OGRE_THREAD_SUPPORT 1|#define OGRE_THREAD_SUPPORT 0|' OgreConfig.h",os.path.join(os.getcwd(),"ogre","OgreMain", "include")],
                [0, "aclocal", os.path.join(os.getcwd(), 'ogre')],
                [0, "./bootstrap", os.path.join(os.getcwd(), 'ogre')],
                [0, "./configure --prefix=%s --with-gui=Xt --disable-devil" % PREFIX, os.path.join(os.getcwd(), 'ogre')],
                [0, "make", os.path.join(os.getcwd(), 'ogre')],
                [0, "make install", os.path.join(os.getcwd(), 'ogre')],
                ]
        libs=[boost.lib,  'OgreMain']
        libs.append ( boost_python_index.lib )
        lib_dirs=[Config.LOCAL_LIB]
        include_dirs=[Config.PATH_Boost, Config.PATH_INCLUDE_Ogre]
        CCFLAGS = ' -DBOOST_PYTHON_MAX_ARITY=19 -D__PYTHONOGRE_BUILD_CODE '
        
        source = [
            [wget, "http://downloads.sourceforge.net/ogre/"+base+".tar.bz2",downloadPath],
        ]
        

    elif isMac():
        version = "1.6.0RC1"
        base = "ogre-linux_osx-v1-4-9"
        basedep = "OgreDependenciesOSX_20070929"
        source = [
            [wget, "http://prdownloads.sourceforge.net/ogre/"+base+".tar.bz2",downloadPath],
            [wget, "http://downloads.sourceforge.net/ogre/" +basedep +".zip",downloadPath], 
        ]
        buildCmds  = [
                [0, tar + " jxf " + os.path.join(downloadPath,base)+".tar.bz2 --overwrite",os.getcwd() ],
                [0, unzip + os.path.join(downloadPath,basedep)+".zip ",os.path.join(os.getcwd(), 'ogre') ],
                [0, "mkdir ~/Library/Frameworks", ''], ## Note that this will fail if the directory exists !!!
                [0,cp + " -R "+os.path.join(os.getcwd(), 'ogre', '__MACOSX','Dependencies')+'/*.framework ' + Config.FRAMEWORK_DIR,''],
                [0, "patch -s -N -i ./python-ogre/patch/ogre.patch -p0 ", os.getcwd()],
                [0, "mkdir Ogre",os.path.join(os.getcwd() ,'ogre','OgreMain', 'include') ],
                # need copies of these in an 'Ogre/..' directory due to includes in the OSX directory -- or get the framework right
                [0, "cp OgreRoot.h Ogre",os.path.join(os.getcwd() ,'ogre','OgreMain', 'include') ],
                [0, "cp OgreRenderSystem.h Ogre",os.path.join(os.getcwd() ,'ogre','OgreMain', 'include') ],
                [0, "cp OgrePrerequisites.h Ogre",os.path.join(os.getcwd() ,'ogre','OgreMain', 'include') ],
                [0, "xcodebuild -project ogre/Mac/Ogre/Ogre.xcodeproj -configuration Release", ''],
                [0, "xcodebuild -project ogre/Mac/Samples/Samples.xcodeproj -configuration Release", ''],
                [0, "cp -R *.framework " + Config.FRAMEWORK_DIR, os.path.join(os.getcwd() ,'ogre',"Mac", "build", "Release") ]
                ]
                
        libs=[boost.lib]
        lib_dirs = [ Config.PATH_LIB_Boost ]
        include_dirs = [ Config.PATH_Boost 
                        , Config.PATH_INCLUDE_Ogre
                        , python_include_dirs
                        ]
                                
        CCFLAGS = ''
        LINKFLAGS = ''
        cflags += ''
        
    CheckIncludes=['boost/python.hpp', 'Ogre.h']
    
     
class ois:
    active = True
    pythonModule = True
    version= "1.2"
    name = 'ois'
    cflags=''
    parent = "ogre/io"
    if isMac():
        source=[]
    if isLinux():
        base = "ois"
        source=[
            [wget, "http://downloads.sourceforge.net/wgois/ois_1.2.0.tar.gz", downloadPath]
            ]
#        buildCmds  = [
#               [0, tar + " zxf " + os.path.join(downloadPath,'ois_1.2.0')+".tar.gz --overwrite",os.getcwd() ],
#               [0, "rm -rf autom4te.cache", os.path.join(os.getcwd(), base )],
#               [0, "libtoolize --force && aclocal $ACLOCAL_FLAGS && autoheader &&\
#                             automake --include-deps --add-missing --foreign && autoconf",
#                             os.path.join(os.getcwd(), base )],
#               [0,"./configure --prefix=%s --includedir=%s/include" % (PREFIX,PREFIX) ,os.path.join(os.getcwd(), base )],
#               [0,'make', os.path.join(os.getcwd(), base )],
#               [0,'make install', os.path.join(os.getcwd(), base )]
#               ]
            
        buildCmds  = [
                [0, tar + " zxf " + os.path.join(downloadPath,'ois_1.2.0')+".tar.gz --overwrite",os.getcwd() ],
                [0, "./bootstrap" ,os.path.join(os.getcwd(), base )],
                [0,"./configure --prefix=%s --includedir=%s/include" %(PREFIX,PREFIX) ,os.path.join(os.getcwd(), base )],
                [0,'make', os.path.join(os.getcwd(), base )],
                [0,'make install', os.path.join(os.getcwd(), base )]
                ]
    if isWindows():
        base = "ois"
        source = [ [wget,"http://downloads.sourceforge.net/wgois/ois_1.2.0.zip", downloadPath] ]
        buildCmds = [
            [0, unzip + downloadPath + "/" + "ois_1.2.0.zip"  ,os.getcwd() ]
#             [0, '"c:/Program Files/Microsoft Visual Studio 8/vc/vcpackages/vcbuild.exe" /useenv ois_VC8.sln ', os.path.join(os.getcwd(), base, 'Win32' )],
#             [0, VCBUILD + " ois_vc8.sln " + "\"Release|Win32\"", os.path.join(os.getcwd(), base, 'Win32' )]
            ]
                         
    if os.name=='nt':
        if _PreCompiled:
            pchstop = 'OIS.h'
            pchbuild = 'buildpch.cpp'
            pchincludes = ['boost/python.hpp', 'OIS.h']
        
        libs=['OIS_Static',boost.lib]
    else:
        libs=['OIS',boost.lib]
        libs.append ( boost_python_index.lib )
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
    else:
        LINKFLAGS = "-l%s" % boost_python_index.lib
    
class ogrerefapp:
    active = True
    pythonModule = True
    version = ogre.version # same as the Ogre version
    name = 'ogrerefapp'
    parent = "ogre/physics"
    baseDir = os.path.join(os.getcwd(),'ogrenew', 'ReferenceApplication')
    source = []
    buildCmds=[
                [0, "aclocal", baseDir],
                [0, "./bootstrap", baseDir],
                [0, "./configure --prefix=%s " % PREFIX, baseDir],
                [0, "make", baseDir],
                [0, "make install", baseDir],
                ]
    if isWindows():
        libs=[boost.lib, 'OgreMain', 'ode_single', 'ReferenceAppLayer']
    else:
        libs=[boost.lib, 'OgreMain', 'ode', 'ReferenceAppLayer']
        libs.append ( boost_python_index.lib )
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
    active = True
    pythonModule = True
    version = "r2429"
    name = 'ogrenewt'
    parent = "ogre/physics"
    base = 'ogreaddons/ogrenewt'
    if isWindows():
        libs = ['Newton', boost.lib, 'OgreNewt_Main', 'OgreMain']
    else:
        libs = ['Newton', boost.lib, 'OgreNewt', 'OgreMain']
        libs.append ( boost_python_index.lib )
    if _STABLE:
        source = [
                 [cvs, " -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -D 01052008 -P "+base, os.getcwd()]
                 ]
    else:
        source = [
                [svn, " co https://ogreaddons.svn.sourceforge.net/svnroot/ogreaddons/trunk/ogrenewt " + base, os.getcwd()]
                 ]
    baseDir = os.path.join(os.getcwd(), base )
    buildCmds = [
            [0, "patch -s -N -i ../../python-ogre/patch/ogrenewt.patch -p0", baseDir], 
            #[0, "cp SConscript OgreNewt_Main", baseDir],
            #[0, "rm SConscript", baseDir],
            [0, rm + " -rf ./OgreNewt_Main/inc/boost", baseDir],
            [0, "scons prefix=%s boost=%s/include/boost-1_34_1 install" % (PREFIX, PREFIX), baseDir],  ##WARNING -- boost include dir name is different than  build name (dash not underscore)
            ]
    if isWindows():
        buildCmds = [
            [0, "patch -s -N -i ../../python-ogre/patch/ogrenewt.patch -p0", baseDir], 
            [0, "rm -r ./OgreNewt_Main/inc/boost", baseDir],
            ]

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
    pythonModule = True
    parent = "ogre/gui"
    name = 'cegui'
    if _STABLE:
        version = "0.5.0b" 
    else:
        version = "0.6.1"        
    if isWindows():
        if _PreCompiled:
            pchstop = 'cegui.h'
            pchbuild = 'buildpch.cpp'
            pchincludes = ['boost/python.hpp', 'cegui.h']
        libs=[boost.lib, 'CEGUIBase', 'OgreMain', 'OgreGUIRenderer' ]
    else:
        libs=[boost.lib, 'CEGUIBase', 'OgreMain', 'CEGUIOgreRenderer' ]
        libs.append ( boost_python_index.lib )

    if isLinux():
        if _STABLE:
            base = "CEGUI-0.5.0"
            source=[
                [wget, "http://downloads.sourceforge.net/crayzedsgui/CEGUI-0.5.0b.tar.gz", downloadPath]
                ]
        else:
            base = "CEGUI-0.6.1"
            source=[
                [wget, "http://prdownloads.sourceforge.net/crayzedsgui/CEGUI-0.6.1.tar.gz?download", downloadPath]
                ]
                   
        buildCmds  = [
                [0, tar + " zxf " + os.path.join(downloadPath,base)+".tar.gz --overwrite",os.getcwd() ],
#                [0, "patch -s -N -i ../python-ogre/patch/cegui.patch -p0", os.path.join(os.getcwd(),base)],
                [0, "echo 'EMPTY' >>./INSTALL", os.path.join(os.getcwd(),base)],
                [0, "echo 'EMPTY' >>./NEWS", os.path.join(os.getcwd(),base)],
#                [0, "aclocal", os.path.join(os.getcwd(),base)],
#cl                [0, "automake" ,os.path.join(os.getcwd(), base )],
                [0, "./bootstrap" ,os.path.join(os.getcwd(), base )],
                [0,"./configure --prefix=%s --enable-freeimage=yes --with-default-xml-parser=TinyXMLParser --disable-samples --without-ogre-renderer --includedir=%s/include" %(PREFIX,PREFIX) ,os.path.join(os.getcwd(), base )],
                [0,'make', os.path.join(os.getcwd(), base )],
                [0,'make install', os.path.join(os.getcwd(), base )]
                ]
    if isWindows():      
        version = "0.6.1"                  
        base = "CEGUI-0.6.1"
        source=[
            [wget, "http://prdownloads.sourceforge.net/crayzedsgui/CEGUI-0.6.1.zip?download", downloadPath]
            ]
        buildCmds  = [
            [0, unzip + " " +  os.path.join(downloadPath,base)+".zip", os.getcwd()],
            [0, 'echo Now run build_vs2008.bat from the CEGUI makefiles\premake directory',''], 
            [0, 'echo then use MSVC to build the CEGUI Release project',''],
            [0, 'echo WARNING - Check that RTTI is enabled in the compiler options',''],
            [0, 'echo Then ensure that OgreGUIRender is compiled with this version of CEGUI','']
            ]         
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
    if isLinux():
        LINKFLAGS = "-l%s" % boost_python_index.lib


class ode:
    active = True
    pythonModule = True
    version= "0.10.1"
    name ='ode'
    parent = "ogre/physics"
    odeLibraryName = 'ode'
    libs=[boost.lib,  odeLibraryName]
    
    if isWindows():
        odeLibraryName = 'ode_single'
        libs= [boost.lib, odeLibraryName, 'User32'] # needed for MessageBox function
    else:
        libs.append ( boost_python_index.lib )    
    lib_dirs = [ Config.PATH_LIB_Boost
                ,  Config.PATH_LIB_ODE
                ]
    include_dirs = [ Config.PATH_Boost 
                    ,  Config.PATH_INCLUDE_ODE
                    , os.path.join(Config.PATH_ODE, 'OPCODE')
                    ]

    ModuleName = 'ODE'
    CheckIncludes = ['boost/python.hpp',  'ode/ode.h'] 
    baseDir = os.path.join(os.getcwd(),"ode-0.10.1")
    if not isWindows():
        source = [
        ["wget", "http://downloads.sourceforge.net/opende/ode-0.10.1.tar.gz",downloadPath]
        ]
        buildCmds = [
            # FIXME: Should this untar not use the unTarGz method mentioned above?
            [0, 'tar  zxf ' + os.path.join(downloadPath,"ode-0.10.1.tar.gz"), ''],
            [0, "chmod +x autogen.sh", baseDir],
            [0, "./autogen.sh", baseDir],
            [0, "./configure --prefix=%s --includedir=%s/include" %(PREFIX, PREFIX), baseDir],
            [0, "make", baseDir],
            [0, "make install", baseDir]
    
            ]
    else:
        source = [
            ["wget", "http://downloads.sourceforge.net/opende/ode-0.10.1.zip",downloadPath]
        ]
        buildCmds = [
            [0, unzip + os.path.join(downloadPath,"ode-src-0.10.1.zip"), ''],
            ]   
            
                     
class opcode:
    active = True
    pythonModule = True
    version= "1.3"
    name = 'opcode'
    parent = "ogre/physics"
    libs=[boost.lib]
    if isLinux():
        libs.append ( boost_python_index.lib )
    lib_dirs = [ Config.PATH_LIB_Boost
                ]
    include_dirs = [ Config.PATH_Boost 
                    ,  Config.PATH_INCLUDE_OPCODE
                    ,  os.path.join(Config.PATH_INCLUDE_OPCODE, 'Ice' )

                    ]
    CCFLAGS =  ' -DBAN_OPCODE_AUTOLINK -DICE_NO_DLL '
    ModuleName = 'Opcode'
    CheckIncludes = ['boost/python.hpp',  'Opcode.h'] 
       
class caelum:
    active = True
    pythonModule = True
    name = 'caelum'
    version="r331"
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
    if isLinux():
        libs=[  boost.lib, boost_python_index.lib, 'OgreMain' ]
    else:        
        libs=[  boost.lib, 'OgreMain' ]
        
    ModuleName="caelum"  
     
class ogreode:
    active = True
    pythonModule = True
    version= "r2497"
    cflags = ""
    name='ogreode'
    parent = "ogre/physics"
    base = 'ogreaddons/ogreode'
    baseDir = os.path.join(os.getcwd(), base )

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
    if _STABLE:
        source = [
             [cvs, " -d :pserver:anonymous@cvs.ogre3d.org:/cvsroot/ogre co -D 01052008 -P "+base, os.getcwd()]
             ]
    else:
        source = [
             [svn, " co https://ogreaddons.svn.sourceforge.net/svnroot/ogreaddons/trunk/ogreode " + baseDir, os.getcwd()]
             ]
    buildCmds = [
            [0, "patch -s -N -i ../../python-ogre/patch/ogreode.patch -p0", baseDir], 
            [0, "chmod +x autogen.sh", baseDir],
            [0, "./autogen.sh", baseDir],
            [0, "./configure --without-samples --prefix=%s" % PREFIX, baseDir],
            [0, "make", baseDir],
            [0, "make install", baseDir],
            ]

    libs=[boost.lib, 'OgreMain',  'OgreOde_Core', 'OgreOde_Prefab', 'OgreOde_Loader' ]
    if isLinux():
        libs.append('ode')
        libs.append ( boost_python_index.lib )
    else:
        libs.append('ode_single')        
    CCFLAGS =  ' -DBOOST_PYTHON_MAX_ARITY=19'
    CheckIncludes = ['boost/python.hpp', 'Ogre.h', 'ode/ode.h', 'ode/odecpp.h', 'OgreOde_Core.h', 'OgreOde_Loader.h', 
                    'Ogreode_Prefab.h'] 
                     
    ModuleName='OgreOde'
    
class quickgui:
    active = True
    pythonModule = True
    version="r80"
    name ='quickgui'
    parent="ogre/gui"
    ## note the defined for _QuickGUIExport forces non dll usage
    if os.name=='nt': 
        ## note the defined for _QuickGUIExport forces non dll usage 
        CCFLAGS = ' -DWIN32 -DNDEBUG -DWINDOWS -D_QuickGUIExport=""  -D_PRECOMP' 
#         if _PreCompiled:
#             pchstop = 'boost/python.hpp'
#             pchbuild = 'buildpch.cpp'
#             pchincludes = ['boost/python.hpp']
    else:
        CCFLAGS = ' -D_QuickGUIExport="" '
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
    libs=[  boost.lib, 'OgreMain' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="QuickGUI"   

class navi:
    active = False
    pythonModule = True
    version="1.6"
    parent="ogre/gui"
    name ='navi'
    CCFLAGS = '-D"WIN32" -D"NDEBUG", -D"WINDOWS", -D"_WIN32", -D"NAVI_DYNAMIC_LIB" ' 
    cflags=""
    base = 'navi'
    include_dirs = [ Config.PATH_Boost
                    ,Config.PATH_INCLUDE_Ogre
                    ,Config.PATH_INCLUDE_navi
                    ,os.path.join(Config.PATH_navi,'..','Dependencies','win32','astral','include')
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ,Config.PATH_LIB_navi
                ,os.path.join(Config.PATH_navi,'..','Dependencies','win32','astral','lib')
                ]
    CheckIncludes=[]
    libs=[  boost.lib, 'Navi_DLL', 'OgreMain','Astral', 'user32',
            'kernel32.lib', 'gdi32.lib', 'winspool.lib', 'comdlg32.lib', 'advapi32.lib',
            'shell32.lib','ole32.lib','oleaut32.lib','uuid.lib' ]
    ModuleName="navi"   
    source = [
             [svn, " checkout http://navi.googlecode.com/svn/trunk/ "+base, os.getcwd()]
             ]
    
    
class betagui:
    active = True
    pythonModule = True
    version="2"
    name='betagui'
    parent="ogre/gui"
    CCFLAGS = ' ' # -D"FT2_BUILD_LIBRARY"
    cflags=""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_OIS,
                    Config.PATH_INCLUDE_betagui
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                Config.PATH_LIB_betagui,
                Config.PATH_LIB_OIS
                ]
    CheckIncludes=[]
    libs=[  boost.lib, 'OgreMain', 'OIS_Static' ]
    if os.name=="nt":
        libs.append ( "User32" ) # needed for static linking
    else:
        libs.append ( boost_python_index.lib )
    
    ModuleName="betagui"   

class ogreforests:
    active = True
    pythonModule = True
    version="r2504"
    name='ogreforests'
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
    libs=[  boost.lib, 'OgreMain' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="ogreforests"   

    
class particleuniverse:
    active = True
    pythonModule = True
    version="0.81"
    name='particleuniverse'
    parent="ogre/addons"
    CCFLAGS = ' ' 
    cflags=""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_particleuniverse
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                Config.PATH_LIB_particleuniverse
                ]
    CheckIncludes=[]
    libs=[  boost.lib, 'OgreMain', 'ParticleUniverse' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="particleuniverse"   
            
class nxogre:
    active = True
    pythonModule = True
    version="0.22T5"
    name='nxogre'
    parent="ogre/physics"
    cflags=""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_NxOgre,
                    ]
    for d in Config.PATH_INCLUDE_PhysX:
        include_dirs.append( d )
    if os.name == 'nt':
        CCFLAGS = ' -DWIN32 '
#         if _PreCompiled:
#             pchstop = 'nxogre.h'
#             pchbuild = 'buildpch.cpp'
#             pchincludes = ['boost/python.hpp', 'nxogre.h']

    else:
        CCFLAGS = ' -DNX_LINUX -DLINUX -DNX_DISABLE_FLUIDS '                
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                Config.PATH_LIB_NxOgre,
                Config.PATH_LIB_PhysX
                ]
    CheckIncludes=[]
#     libs=[  boost.lib, 'NxOgre','NxCharacter', 'NxCooking', 'NxExtensions', 'PhysXLoader','OgreMain' ]
    if os.name =='nt':
        libs=[  boost.lib, 'NxCharacter', 'NxCooking',  'PhysXLoader','OgreMain','NxOgre' ] #, 'NxOgreStatic'] #'NxExtensions',
    else:
        libs=[  boost.lib, 'NxCharacter', 'NxCooking', 'PhysXCore','PhysXLoader','OgreMain', 'NxOgre' ]
        libs.append ( boost_python_index.lib )
    ModuleName="NxOgre"   
    
    
        
class theora:
    active = True
    pythonModule = True
    version="r2455"
    parent="ogre/addons"
    cflags=""
    name='theora'
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_Theora,
                    Config.PATH_INCLUDE_TheoraDemo
                    
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
    libs=[  boost.lib, 'Plugin_TheoraVideoSystem','OgreMain' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="theora"   
 
class plib:
    active = True
    pythonModule = True
    version="1.8.4"
    name='plib'
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
        libs = [ boost.lib, 'winmm', 'User32','wsock32', 'opengl32.lib','glu32.lib' ]
    else:
        libs = [ boost.lib, "GL" ] # Note sure what linux libraries are needed
        libs.append ( boost_python_index.lib )
    
class physx:
    active = True
    pythonModule = True
    version="2.8.1"
    name='physx'
    parent="ogre/physics"
    cflags=""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    ]
    for d in Config.PATH_INCLUDE_PhysX:
        include_dirs.append( d )
    if os.name == 'nt':
        CCFLAGS = ' -D"WIN32" '
        ## CCFLAGS = ' ' ## try to not define WIN 32 to remove inline assembly code that GCCxml doesn't like
    else:
        CCFLAGS = ' -D"LINUX" -D"NX_USE_SDK_DLLS" -D"_USRDLL" -D"NX_DISABLE_FLUIDS" '                
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                Config.PATH_LIB_PhysX
                ]
    CheckIncludes=[]
    if isWindows():
        libs=[  boost.lib, 'NxCharacter', 'NxCooking', 'PhysXLoader' ]
    elif isLinux():
        libs=[  boost.lib, 'NxCharacter', 'NxCooking',  'PhysXCore', 'PhysXLoader' ]
        libs.append ( boost_python_index.lib )
        source = [
                    [wget, "http://developer.download.nvidia.com/PhysX/2.8.1/PhysX_2.8.1_SDK_CoreLinux_deb.tar.gz", downloadPath]
                 ]
                 
        buildCmds =  [
            [0, "tar zxf " + os.path.join(downloadPath, "PhysX_2.8.1_SDK_CoreLinux_deb.tar.gz"), ''],
            [0, "sudo dpkg -i  libphysx-common_2.8.1-4_i386.deb", ''],
            [0, "sudo dpkg -i  libphysx-2.8.1_4_i386.deb", ''],
            [0, "sudo dpkg -i  libphysx-dev-2.8.1_4_i386.deb", ''],
            [0, "sudo dpkg -i  libphysx-extras-2.8.1_4_i386.deb", '']
            ]

    ModuleName="PhysX"   

# class nxogre_09:
#     active = False  # True
#     pythonModule = True
#     version="0.9"
#     name='nxogre_09'
#     parent="ogre/physics"
#     cflags=""
#     include_dirs = [ Config.PATH_Boost,
#                     Config.PATH_INCLUDE_Ogre,
#                     Config.PATH_INCLUDE_NxOgre_09,
#                     ]
#     for d in Config.PATH_INCLUDE_PhysX:
#         include_dirs.append( d )
#     if os.name == 'nt':
#         CCFLAGS = ' -DWIN32 -DNxExport="" '
# 
#     else:
#         CCFLAGS = ' -DNX_LINUX -DLINUX -DNX_DISABLE_FLUIDS '                
#     lib_dirs = [Config.PATH_LIB_Boost,
#                 Config.PATH_LIB_Ogre_OgreMain,
#                 Config.PATH_LIB_PhysX
#                 ]
#     CheckIncludes=[]
#     if os.name =='nt':
#         libs=[  boost.lib, 'NxCharacter', 'NxCooking', 'NxExtensions', 'PhysXLoader','OgreMain' ]
#     else:
#         libs=[  boost.lib, 'NxCharacter', 'NxCooking', 'PhysXCore','PhysXLoader','OgreMain' ]
#     ModuleName="NxOgre_09"   

class ogreal:
    active = True
    pythonModule = True
    version="r115"
    name='ogreal'
    cflags = ''
    
    parent = "ogre/sound"
    
    include_dirs = [ Config.PATH_Boost
                , Config.PATH_INCLUDE_Ogre
                , Config.PATH_INCLUDE_OgreAL
                , Config.PATH_INCLUDE_OGG
                , Config.PATH_INCLUDE_VORBIS
                , Config.PATH_INCLUDE_OPENAL
                ]  
                
    if isWindows():                  
        lib_dirs = [ Config.PATH_LIB_Boost
                ,Config.PATH_LIB_Ogre_OgreMain    
                ,os.path.join(Config.PATH_OGG, 'win32', 'Static_Release')
                ,os.path.join(Config.PATH_VORBIS, 'win32','Vorbis_Static_Release')
                ,os.path.join(Config.PATH_VORBIS, 'win32','VorbisEnc_Static_Release')
                ,os.path.join(Config.PATH_VORBIS, 'win32','VorbisFile_Static_Release')
                ,os.path.join(Config.PATH_OPENAL, 'Release' ) #'libs','Win32')
                ,Config.PATH_LIB_OgreAL
                  ] 
        CCFLAGS = ' -DWIN32 -DNDEBUG -D_LIB -D_WIN32 -D_WINDOWS ' #-DVORBIS_IEEE_FLOAT32 -D_USE_NON_INTEL_COMPILER '              
        libs=[boost.lib, 'OgreMain', 
                    'ogg_static', 
                    'vorbis_static','vorbisfile_static',
                    #'vorbisenc_static',
                    'OpenAL32', 
                    #'EFX-Util'

                    ] # -- going to compile OgreAL ourselves
        source = [
            ["wget", "http://connect.creativelabs.com/openal/Downloads/OpenAL11CoreSDK.zip",downloadPath],
            ["wget", "http://downloads.xiph.org/releases/ogg/libogg-1.1.3.zip",downloadPath],
            ["wget", "http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.zip",downloadPath],
            ]
        buildCmds = [
            [0, "unzip -q -o  " + os.path.join(downloadPath, "OpenAL11CoreSDK.zip"), ''],
            [0, "OpenAL11CoreSDK.exe", ''],
            [0, "unzip -q -o  " + os.path.join(downloadPath, "libogg-1.1.3.zip"), ''],
            [0, "ren libogg-1.1.3 ogg",''],
            [0, "unzip -q -o  " + os.path.join(downloadPath, "libvorbis-1.2.0.zip"), ''],
            [0, "ren libvorbis-1.2.0 vorbis", ''],
            ]
    else:
        lib_dirs = [ Config.PATH_LIB_Boost
                ,Config.PATH_LIB_Ogre_OgreMain    
                  ] 
        source = [
#             ["wget", "http://www.openal.org/openal_webstf/downloads/openal-0.0.8.tar.gz",downloadPath],
            ["wget", "http://connect.creativelabs.com/openal/Downloads/openal-soft-1.5.304.tar.bz2",downloadPath],
            ["wget", "http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz",downloadPath],
            ["wget", "http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz",downloadPath],
            ]
        buildCmds = [
#             [0, "tar zxf " + os.path.join(downloadPath, "openal-0.0.8.tar.gz"), ''],

            [0, "tar jxf " + os.path.join(downloadPath, "openal-soft-1.5.304.tar.bz2"), ''],
            [0, "tar zxf " + os.path.join(downloadPath, "libogg-1.1.3.tar.gz"), ''],
            [0, "tar zxf " + os.path.join(downloadPath, "libvorbis-1.2.0.tar.gz"), ''],
            [0, "./configure --prefix=%s\nmake\nmake install" % PREFIX, os.path.join(os.getcwd(), "libogg-1.1.3")],

            [0, "./configure --prefix=%s\nmake\nmake install" % PREFIX, os.path.join(os.getcwd(), "libvorbis-1.2.0")],

            [0,"cmake -DCMAKE_INSTALL_PREFIX:PATH="+ PREFIX, os.path.join(os.getcwd(),'openal-soft-1.5.304')], 
            [0,"make ", os.path.join(os.getcwd(),'openal-soft-1.5.304')], 
            [0,"cp -p libopenal* %s/lib " % PREFIX, os.path.join(os.getcwd(),'openal-soft-1.5.304')],
            [0,"cp -p * %s/include " % PREFIX, os.path.join(os.getcwd(),'openal-soft-1.5.304', 'include','AL')]
            
#            [0, "sed --in-place -s 's|( ALCvoid )|()|' alc.h",os.path.join(os.getcwd(),"openal-0.0.8","common", "include", "AL")],
#            [0, "aclocal\n./autogen.sh", os.path.join(os.getcwd(),"openal-0.0.8")],
#            [0, "./configure --prefix=%s\nmake\nmake install" % PREFIX, os.path.join(os.getcwd(), "openal-0.0.8")]
            ]
        libs=[boost.lib, boost_python_index.lib, 'OgreMain', 'vorbisfile',
                    #'ogg', 
                    #'vorbis','vorbisfile','vorbisenc',
                    'openal']  ##  'OgreAL' -- going to compile OgreAL ourselves
        libs.append ( boost_python_index.lib )         
    ModuleName = 'OgreAL'
    CheckIncludes = ['OgreAL.h']

      
class ogrevideoffmpeg:
    active = False
    pythonModule = True
    version="0.2.1"
    name='ogrevideoffmpeg'
    parent="ogre/addons"
    CCFLAGS = ""
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
        libs=[  boost.lib, 'OgreMain', 'libavformat', 'libavcodec', 'libavutil',
            'libavdevice','libswscale','liba52','libmingwex','libfaac','libfaad',
            'libz','libxvidcore','libx264','libpthreadGC2','libwsock32','libvorbis','libmp3lame','libogg',
            'libvorbisenc','libvorbisfile','libtheora','libgsm','libvfw32','libgcc' ]
    else:
        libs=[  boost.lib, 'OgreMain', 'avformat', 'avcodec', 'avutil', 'z', 'GL', 'GLU', 'Xxf86vm', 'Xext', 'X11' ]
    ModuleName="ogrevideoffmpeg"   

class ogredshow:
    active = False
    pythonModule = True
    version="0.1"
    name='ogredshow'
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
    libs=[  boost.lib, 'OgreMain','Strmiids' , 'ole32']
    ModuleName="ogredshow"   
    
class et:  ## editable terrain
    active = True
    pythonModule = True
    version= "2.3.1"
    name='et'
    parent = "ogre/addons"
    if isLinux():
        libs=[ boost.lib, boost_python_index.lib, 'OgreMain' ]
    else:
        libs=[ boost.lib, 'OgreMain' ]
        
    lib_dirs = [ Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ]
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_et
                    ]
    ModuleName = 'et'
    CheckIncludes = ['boost/python.hpp'] 
    CCFLAGS = "  "
    cflags=''
    
class bullet:
    active = True
    pythonModule = True
    version= "2.70"
    name='bullet'
    base = "bullet-" + version
    baseDir = os.path.join(os.getcwd(), base)
    parent = "ogre/physics"
    libs=[boost.lib,  'libbulletcollision', 'libbulletdynamics']
    if isWindows():
        libs.append('libbulletmath')
    else:
        libs.append('libbulletmath')
        libs.append('libbulletsoftbody')
        #libs.append('libbulletcolladaconverter')
        libs.append('libbulletmultithreaded')
        libs.append ( boost_python_index.lib )

    lib_dirs = [ Config.PATH_LIB_Boost
                ,  Config.PATH_LIB_Bullet
                ]
    include_dirs = [ Config.PATH_Boost 
                    ,  Config.PATH_INCLUDE_Bullet
                    ]
    if not isWindows():
        source=[
            [wget, "http://bullet.googlecode.com/files/"+base+".tgz", downloadPath]
            ]
        buildCmds = [
            [0, "tar zxf " +os.path.join(downloadPath, base)+".tgz", ''],
            [0, "./autogen.sh", baseDir],
            [0, "./configure --prefix=%s " %(PREFIX), baseDir],
##            [0, "cmake . -DCMAKE_INSTALL_PREFIX:PATH=%s" % PREFIX, baseDir],
##            [0, "make", baseDir],
            [0, "jam", baseDir],
            [0, "jam install", baseDir]

##            [0, "find . -name *.a -execdir cp {} %s/lib \;" % PREFIX, baseDir]
            ]
    else:
        source=[
        [wget, "http://bullet.googlecode.com/files/"+base+".zip", downloadPath]
        ]
        buildCmds = [
            [0, unzip +os.path.join(downloadPath, base)+".zip", ''],
            ]
            
    ModuleName = 'bullet'
    CheckIncludes = ['boost/python.hpp'] 
    cflags = ""
    
class ogrebulletc:  # 
    active = True
    pythonModule = True
    name='ogrebulletc'
    version = "r2493"
    cflags = ""
    parent = "ogre/physics"
    libs = [boost.lib,  'OgreMain', 
        'libbulletcollision', 'libbulletdynamics', 'libbulletmath'
        ]
#    if isWindows():
#        libs.append('libbulletmath')
#    else:
#        libs.append('liblinearmath')
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
    if isWindows():
        CCFLAGS =  ' -DWIN32 -DNDEBUG -D_WINDOWS -D_PRECOMP '
    else:
        CCFLAGS = ' -D_PRECOMP -fno-inline '
        libs.append ( boost_python_index.lib )
    ModuleName = 'OgreBulletC'
    CheckIncludes=['boost/python.hpp', 'Ogre.h']    


class ogrebulletd:  # 
    active = True
    pythonModule = True
    version = "r2493"
    name='ogrebulletd'
    cflags = ""
    parent = "ogre/physics"
    libs = [boost.lib,  'OgreMain', 
        'libbulletcollision', 'libbulletdynamics', 'libbulletmath'
        ]
#    if isWindows():
#        libs.append('libbulletmath')
#    else:
#        libs.append('liblinearmath')
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
    if isWindows():
        CCFLAGS =  ' -DWIN32 -DNDEBUG -D_WINDOWS -D_PRECOMP '
    else:
        CCFLAGS = ' -D_PRECOMP '
        libs.append ( boost_python_index.lib )
    ModuleName = 'OgreBulletD'
    CheckIncludes=['boost/python.hpp', 'Ogre.h']        
    descText = "Ogre Bullet Dynamics (physics) implementation"
    
class noise:
    active = True
    pythonModule = True
    version="r2486"
    parent="ogre/addons"
    name='noise'
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_noise,
                    os.path.join(Config.PATH_INCLUDE_noise, 'model'),
                    os.path.join(Config.PATH_INCLUDE_noise, 'module')
                    ]
    lib_dirs = [Config.PATH_LIB_Boost
                ]
    CheckIncludes=[]
    libs=[  boost.lib]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="noise"   
    descText = "Generate 'noise'"
    
class watermesh:
    active = True
    pythonModule = True
    version="1.0"
    parent="ogre/addons"
    name='watermesh'
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_watermesh
                    , Config.PATH_INCLUDE_Ogre
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ]
    CheckIncludes=[]
    libs=[  boost.lib, 'OgreMain' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="watermesh"   
    descText = "C++ code from Ogre Water demo -- use Hydrax instead"
    
class ofusion:
    active = True
    pythonModule = True
    version="1.0"
    name='ofusion'
    parent="ogre/addons"
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_ofusion
                    , Config.PATH_INCLUDE_Ogre
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ]
    CheckIncludes=[]
    libs=[  boost.lib, 'OgreMain' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="ofusion"   
    descText = "Import Ogre 'Scenes' from 3dMax using the ofusion exporter"
 
class cadunetree:
    active = True
    pythonModule = True
    version="0.6"
    name='cadunetree'
    parent="ogre/addons"
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_cadunetree
                    , Config.PATH_INCLUDE_Ogre
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ]
    CheckIncludes=[]
    libs=[  boost.lib, 'OgreMain' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="cadunetree"   
    descText = "Generate realistic trees"       

class ogrepcz:
    active = True
    pythonModule = True
    version=ogre.version
    name='ogrepcz'
    parent="ogre/renderer"
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_Ogre,
                    Config.PATH_INCLUDE_ogrepcz
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                os.path.join(Config.PATH_LIB_Ogre_OgreMain, 'OGRE')
                ]
    CheckIncludes=[]
    libs=[  boost.lib, 'Plugin_PCZSceneManager', 'OgreMain' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="ogrepcz" 
    descText = "Portal SceneManager - seamlessly move from interior to exterior scene management"     
        
class opensteer:
    active = False
    pythonModule = True
    version="1.0"
    name='opensteer'
    parent="ogre/addons"
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_opensteer,
                    Config.PATH_INCLUDE_opensteer_opensteer
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_opensteer
                ]
    CheckIncludes=[]
    libs=[  boost.lib, 'opensteer' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="opensteer"          

class hydrax:
    active = True
    pythonModule = True
    version="0.4"
    name='hydrax'
    parent="ogre/addons"
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_hydrax
                    , Config.PATH_INCLUDE_Ogre
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain
                ]
    CheckIncludes=[]
    libs=[  boost.lib, 'OgreMain' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="hydrax"  
    descText = "Fantastic water/ocean effects"   

class hikari:
    active = True
    pythonModule = True
    version="r23"
    name='hikari'
    parent="ogre/gui"
    cflags = ""
    include_dirs = [ Config.PATH_Boost,
                    Config.PATH_INCLUDE_hikari
                    , Config.PATH_INCLUDE_Ogre
                    ]
    lib_dirs = [Config.PATH_LIB_Boost,
                Config.PATH_LIB_Ogre_OgreMain,
                Config.PATH_LIB_hikari
                ]
    CheckIncludes=[]
    libs=[  boost.lib, 'OgreMain', 'comsuppw', 'Gdi32' ] ##, 'hikari' ]
    if isLinux():
        libs.append ( boost_python_index.lib )
    ModuleName="hikari"  
    descText = "Use Flash controls within Ogre"
    descLink = "http://hikari-library.googlecode.com/"
            
############################################################################################

## Here is the master list....
## Keep eveything here in lowercase so the over rides work :)
    
projects = {
    'ois' : ois
    , 'ogre' : ogre
    , 'cegui' : cegui
    , 'ode' : ode
#     , 'newton' : newton
    , 'ogrerefapp' : ogrerefapp
    , 'ogrenewt' : ogrenewt
    , 'ogreode' : ogreode
    , 'ogreal' : ogreal
    , 'quickgui' : quickgui
    , 'opcode' : opcode
    , 'nxogre' : nxogre
    , 'physx' : physx
#     , 'nxogre_09' : nxogre_09
    , 'bullet' : bullet
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
    , 'noise' : noise
    , 'watermesh' : watermesh
    , 'ofusion' : ofusion
    , 'particleuniverse' : particleuniverse
    , 'cadunetree' : cadunetree
    , 'opensteer' : opensteer
    , 'ogrepcz' : ogrepcz
    , 'hydrax' : hydrax
    , 'hikari' : hikari
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
                pass
#                 print "WARNING: Unable to find %s include file (%s class) in include_dirs" % (incfile, name)
# #         if os.name =='nt': 
# #             for libfile in cls.libs :
# #                 libfile += '.lib'
# #                 found = False
# #                 log ( "Checking for %s library (%s class) in lib_dirs" % (libfile, name) )
# #                 for lookdir in cls.lib_dirs:
# #                     if os.path.isfile ( os.path.join ( lookdir, libfile ) ):
# #                         found = True
# #                         break
# #                 if not found:
# #                     print "WARNING: Unable to find %s library (%s class) in lib_dirs" % (libfile, name)
    
#
# a couple of specials that should be done differently
gccxml_bin = Config.gccxml_bin
pyplusplus_install_dir = Config.pyplusplus_install_dir      
         
rpath= Config.RPATH  
                          
for name, cls in projects.items():

    # little hack to allow overriding of settings from the PythonOgreConfig_xxxxx.py file
    if hasattr(Config, name):   # look to see if the class exists in the config file
        print "Using Override for class ", name
        _class = Config.__dict__[name]  # get the class
        for key, value in _class.__dict__.items():
            if not key.startswith('_'):
                cls.__dict__[key] = value   
                print "Set %s.%s to %s" % (name, key, value) 
    
                    
    ##CheckPaths( cls, name )
    cls.root_dir = os.path.join( root_dir, 'code_generators', name )
    cls.dir_name = name + '_' + str(cls.version)
    cls.generated_dir = os.path.join( generated_dir, cls.dir_name )
    cls.cache_file = os.path.join( declarations_cache_dir, cls.dir_name + '_cache.xml' )

    if isMac():  # On the mac the Ogre library is lined in with the -framework command in scons
       try: 
          cls.libs.remove('OgreMain') 
       except:
          pass
       ## and we have a commond set of flags that I will set here...
       cls.include_dirs.append(Config.MAC_SDK_INCLUDE)
       if not hasattr(cls, 'CCFLAGS'):
           cls.CCFLAGS=''
       cls.CCFLAGS += Config.MAC_CCFLAGS
       if not hasattr(cls, 'cflags'):
           cls.cflags=''
       cls.cflags += Config.MAC_cflags

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
