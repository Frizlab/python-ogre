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
    include_dirs = None
    libs_dir = None
    dlls_dir = None
    generated_dir = None
    version = "CVS"
    cache_file = None
    libs_files = ["ogremain", "ogreguirenderer", "ceguibase", "ode", "newton"]

class ois:
    version= "0.7"
    pass

class CEGUI:
    version = "0.5" 
    
class ode:
    version= "0.7"
    
class newton:
    version= "1.0"

class ogrerefapp:
    version = "1.2.3"

class ogrenewt:
    version = "1.0"
        
       
projects = {
    'ois' : ois
    , 'ogre' : ogre
    , 'CEGUI' : CEGUI
    , 'ode' : ode
    , 'newton' : newton
    , 'ogrerefapp' : ogrerefapp
    , 'ogrenewt' : ogrenewt
}

if 'roman' in getpass.getuser():
    root_dir = r'd:\dev\python-ogre\bindings-dev'
    gccxml_bin = r'd:/dev/gccxml_cvs/gccxml-bin/bin/release/gccxml.exe'
    python_include_dir = 'e:/python25/include'
    pyplusplus_install_dir = r'D:\dev\language-binding\sources'

    ogre.include_dirs = [ r'd:\dev\python-ogre\sources\ogre_cvs\ogrenew\OgreMain\include' ]
    ois.include_dir = r'D:\OisSDK\includes'
else:
    root_dir = r'C:\development\pywork'
    gccxml_bin = r'c:/development/gccxml/bin/release/gccxml.exe'
    python_include_dir = 'c:/python25/include'
    pyplusplus_install_dir = r'c:\development\pyplus'

    if ogre.version == "CVS":
        ogre.include_dir  = [r'c:\development\ocvs\ogrenew\ogremain\include']
        ogrerefapp.include_dir = [r'c:\development\ocvs\ogrenew\ogremain\include',
                           r'c:\development\ocvs\ogrenew\referenceapplication\referenceapplayer\include',
                            r'c:\development\ocvs\ogrenew\dependencies\include']
        ogrerefapp.version="CVS"
#         ogre.libs_dir = [r'c:/development/ocvs/ogrenew/ogremain/lib/release', 
#                         r'c:/development/ocvs/ogrenew/samples/common/ceguirenderer/lib',
#                         r'c:/development/ocvs/ogrenew/dependencies/lib/release',
#                         r'c:/development/boost/boost' ]
        CEGUI.include_dir = [r'c:/development/cegui-0.5.0/include',
                           ## r'c:/development/ocvs/ogrenew/dependencies/include',
                           ## NOTE: ogre headers expect cegui to live in ./CEGUI so I've copied the cegui include
                           ## tree to ./CEGUI in the followind directory..
                            r'c:/development/ocvs/ogrenew/dependencies/include/',
                            r'c:/development/ocvs/ogrenew/samples/common/CEGUIRenderer/include',
                            r'c:/development/ocvs/ogrenew/ogremain/include',
                            r'c:/development/boost'
                            ]
                            
        ogrenewt.include_dir = [r'c:/development/ocvs/ogreaddons/ogrenewt/OgreNewt_Main/inc',
                            r'c:/development/ocvs/ogrenew/ogremain/include',
                            r'c:/development/newtonsdk/sdk'
                            ]

    else:
        ogre.include_dir  = [r'c:\devold\ogreSDK\include',
                                r'c:\devold\ogreSDK\samples\refapp\include'
                                ]
        CEGUI.include_dir = [r'c:/devold/cegui-0.5.0/include',
                            r'c:/devold/ogresdk/samples/include',
                            r'c:/devold/ogresdk/include',
                            r'c:/development/boost'
                            ]  
        CEGUI.version = "0.4"                              
                                    
        ogrerefapp.include_dir = ogre.include_dir
        
#    CEGUI.include_dir = r'c:/ogresdk/include/CEGUI'
    ode.include_dir = r'c:/development/ocvs/ogrenew/dependencies/include'
    ode.include_dir = r'c:/development/ode-0.7/include'
    newton.include_dir = (r'c:/development/newtonsdk/sdk',)
    ois.include_dir = r'c:\development\ois\includes'
    


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

