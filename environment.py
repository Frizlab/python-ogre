import os
import sys
import getpass

root_dir = None #python-ogre root directory
gccxml_bin = None #path to gccxml binary
python_include_dir = None #path to python include files directory
pyplusplus_install_dir = None #path to pygccxml & Py++ packages

generated = None #path to generated files dir
shared_ptr_dir = None #path to shared_ptr directory
PythonOgreMajorVersion = "0"
PythonOgreMinorVersion = "6" # change to 0.6 due to lowercase properties
PythonOgrePatchVersion = "0"


class ogre:
    root_dir = None
    include_dir = None
    libs_dir = None
    dlls_dir = None
    generated_dir = None
    version = "CVS"
    #version = "1.2.3"
    cache_file = None
    libs_files = ["ogremain", "ogreguirenderer", "ceguibase", "ode"]

class ois:
    version = "0.7"

class CEGUI:
    version = "0.5" 
    
class ode:
    version = "0.7"

projects = {
    'ois' : ois
    , 'ogre' : ogre
    , 'CEGUI' : CEGUI
    , 'ODE' : ode
}

if 'roman' in getpass.getuser():
    root_dir = r'D:\python-ogre'
    gccxml_bin = r'd:/gccxml_cvs/gccxml-build/bin/release/gccxml.exe'
    python_include_dir = 'e:/python/include'
    pyplusplus_install_dir = r'D:\pygccxml_sources\sources'

    ogre.include_dir = r'D:\OgreSDK1.2.3\include'
    ois.include_dir = r'D:\OisSDK\includes'
    CEGUI.include_dir = r'D:/CEGUI-0.5-0-RC2/include'
    
    ode.include_dir = r'D:\ode-win32-0.7\ode-0.7\include'
    
else:
    root_dir = r'C:\development\pywork'
    gccxml_bin = r'c:/development/gccxml/bin/release/gccxml.exe'
    python_include_dir = 'c:/python25/include'
    pyplusplus_install_dir = r'c:\development\pyplus'

    if ogre.version == "CVS":
        ogre.include_dir  = r'c:\development\ocvs\ogrenew\ogremain\include'
        ois.include_dir = r'c:\development\ois\includes'
        ogre.libs_dir = (r'c:/development/ocvs/ogrenew/ogremain/lib/release', 
                        r'c:/development/ocvs/ogrenew/samples/common/ceguirenderer/lib',
                        r'c:/development/ocvs/ogrenew/dependencies/lib/release',
                        r'c:/development/boost/boost' )
    else:
        ogre.include_dir  = r'c:\development\ogreSDK\include'
    CEGUI.include_dir = r'c:/development/ocvs/ogrenew/dependencies/include/CEGUI'

sys.path.append( os.path.join( root_dir, 'common_utils' ) )
sys.path.append( pyplusplus_install_dir + r'/pygccxml_dev' )
sys.path.append( pyplusplus_install_dir + r'/pyplusplus_dev' )

shared_ptr_dir = os.path.join( root_dir, 'shared_ptr' )
generated_dir = os.path.join( root_dir, 'generated' )
declarations_cache_dir = os.path.join( root_dir, 'code_generators', 'cache' )

for name, cls in projects.items():
    cls.root_dir = os.path.join( root_dir, 'code_generators', name )
    cls.generated_dir = os.path.join( generated_dir, name + '_' + str(cls.version) )
    cls.cache_file = os.path.join( declarations_cache_dir, name + '_' + str(cls.version) + '_cache.xml' )

