import os
import sys
import getpass


root_dir = None #python-ogre root directory
gccxml_bin = None #path to gccxml binary
python_include_dir = None #path to python include files directory
pyplusplus_install_dir = None #path to pygccxml & Py++ packages

generated = None #path to generated files dir
shared_ptr_dir = None #path to shared_ptr directory

class ogre:
    root_dir = None
    include_dir = None
    libs_dir = None
    dlls_dir = None
    generated_dir = None
    version = "1.2"
    cache_file = None

class ois:
    pass


if 'roman' in getpass.getuser():
    root_dir = r'D:\python-ogre'
    gccxml_bin = r'd:/gccxml_cvs/gccxml-build/bin/release/gccxml.exe'
    python_include_dir = 'c:/python/include'
    pyplusplus_install_dir = r'D:\pygccxml_sources\sources'

    ogre.include_dir = r'D:\OgreSDK\include'

else:
    gccxml_path = r'c:/development/gccxml/bin/release/gccxml.exe'
    pygccxml_base_dir = r'c:\development\pyplus'
    if OGRE_VERSION == "CVS":
        headers_dir = r'c:\development\ocvs\ogrenew\ogremain\include'
        working_dir = r'c:\development\pywork'
    else:
        headers_dir = r'c:\development\ogrenew\include'
        working_dir = r'c:\development\pyworkRel'
    python_headers_dir = 'c:/python25/include'

sys.path.append( os.path.join( root_dir, 'common_utils' ) )
sys.path.append( pyplusplus_install_dir + r'/pygccxml_dev' )
sys.path.append( pyplusplus_install_dir + r'/pyplusplus_dev' )


ogre.root_dir = os.path.join( root_dir, 'ogre_dev' )

shared_ptr_dir = os.path.join( root_dir, 'shared_ptr' )

generated_dir = os.path.join( root_dir, 'generated' )
ogre.generated_dir = os.path.join( generated_dir, 'ogre' )

declarations_cache_dir = os.path.join( root_dir, 'declarations_cache' )
ogre.cache_file = os.path.join( declarations_cache_dir, 'ogre_cache.xml' )

