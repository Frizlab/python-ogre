import os
import sys
import getpass

_UserName = getpass.getuser()

_SDK = False        # Changes Ogre include path settings
_SVN = False        # if building from Ogre svn

## The root directory is where this module is located
module_dir = os.path.abspath(os.path.dirname(__file__) )
BASE_DIR, ignore = os.path.split(module_dir)

# the base of the /usr/... dircetory structure that we are using
ROOT_DIR = os.path.join(BASE_DIR,'root')
LOCAL_LIB = os.path.join(ROOT_DIR,'usr/lib')
LOCAL_INCLUDE = os.path.join(ROOT_DIR, 'usr/include')

## path to the root of your boost dir, should have 'libs', 'boost' subdirectories
PATH_Boost = os.path.join(BASE_DIR, 'boost_1_45_0' )
#PATH_Boost = os.path.join(BASE_DIR, 'boost_svn')

## Path to your boost_pythonxxxx lib file
PATH_LIB_Boost = os.path.join(PATH_Boost, 'stage/lib' )
PATH_LIB_Thread_STATIC = PATH_LIB_Boost
PATH_LIB_date_time_STATIC = PATH_LIB_Boost

if (getpass.getuser() == 'amiller' ):  # special for Andy's machine as I tend to build against multiple python versions
   PythonVersionString = str(sys.version_info[0]) + '.' + str(sys.version_info[1])
   PATH_LIB_Boost =    os.path.join(PATH_Boost, 'bin.v2/libs/python_' +PythonVersionString +'/build/msvc-9.0/release/threading-multi')
#    _SVN = True

# in Linux we need to code in the Relative path for the library
RPATH=""

# We need to know where to find gccxml......
gccxml_bin = os.path.join(BASE_DIR, 'gccxml/bin/release/gccxml.exe')

# and the Py++ directory as sometimes we need access to the code repository there
pyplusplus_install_dir = os.path.join(BASE_DIR, 'pyplusplus')

## Ogre Itself
PATH_Ogre = os.path.join(BASE_DIR, 'ogre')
if _SDK:
    PATH_Ogre = "C:\\development\\ogre"
    
PATH_LIB_Ogre_OgreMain = os.path.join( PATH_Ogre, 'lib', 'Release' )
PATH_LIB_Ogre_OgreMain_debug = os.path.join( PATH_Ogre, 'lib' )
if _SVN:
    PATH_LIB_Ogre_OgreMain = os.path.join( PATH_Ogre, 'lib', 'Release' )
    
PATH_OgreAddons =   os.path.join(BASE_DIR, 'OgreAddons')
PATH_THIRDPARTY =   os.path.join(module_dir, 'ThirdParty' )    

PATH_INCLUDE_Ogre = os.path.join(PATH_Ogre,'OgreMain\\include') 
PATH_INCLUDE_ogrepaging = os.path.join(PATH_Ogre,'Components','Paging', 'include')
PATH_INCLUDE_ogrertshadersystem =   os.path.join(PATH_Ogre,'Components','RTShaderSystem', 'include')
PATH_INCLUDE_ogreterrain =   os.path.join(PATH_Ogre,'Components','Terrain', 'include')
PATH_INCLUDE_ogreproperty = os.path.join(PATH_Ogre,'Components','Property', 'include')
PATH_INCLUDE_ogreoctreezone = os.path.join(PATH_Ogre,'Plugins', 'OctreeZone', 'include' )
PATH_INCLUDE_ogreoctreesm = os.path.join(PATH_Ogre,'Plugins', 'OctreeSceneManager', 'include' )
PATH_INCLUDE_sdksample = os.path.join(PATH_Ogre, "Samples", "Common", "include")
PATH_INCLUDE_sdksample = os.path.join(PATH_THIRDPARTY, "sdksample")

PATH_LIB_Ogre_Dependencies =    os.path.join( PATH_Ogre, 'Dependencies/lib/Release')
PATH_INCLUDE_Ogre_Dependencies = os.path.join( PATH_Ogre, 'Dependencies/include')

## Ogre Plugins
PATH_plugins = os.path.join(PATH_Ogre, 'PlugIns')
PATH_ogrepcz = os.path.join(PATH_plugins, 'PCZSceneManager' )
PATH_LIB_pcz = os.path.join(PATH_LIB_Ogre_OgreMain, 'OGRE')
PATH_particleuniverse = os.path.join(PATH_plugins, 'ParticleUniverse' )
PATH_LIB_particleuniverse =     os.path.join(PATH_particleuniverse, 'bin', 'release')

## OIS
PATH_OIS =            os.path.join( PATH_Ogre, 'Dependencies' ) ## ois-1.2.0
PATH_LIB_OIS =        os.path.join( PATH_OIS, 'lib/Release') ## NOTE Posix platforms this lives in'lib' not 'dll'
PATH_LIB_OIS_debug =  os.path.join( PATH_OIS, 'lib/Debug') ## NOTE Posix platforms this lives in'lib' not 'dll'
PATH_INCLUDE_OIS =    os.path.join( PATH_OIS, 'include/OIS') ## Note the plural include's

## CEGUI
PATH_CEGUI = os.path.join(BASE_DIR, 'CEGUI-0.7.5')
PATH_LIB_CEGUI = os.path.join( PATH_CEGUI, 'lib' )
PATH_INCLUDE_CEGUI = os.path.join(PATH_CEGUI, 'cegui','include')
PATH_LIB_Ogre_CEGUIRenderer =   os.path.join( PATH_CEGUI, 'lib')
PATH_INCLUDE_Ogre_CEGUIRenderer = os.path.join( PATH_INCLUDE_CEGUI, 'RendererModules','Ogre')

## OgreOggSound
PATH_OGG = os.path.join(BASE_DIR, 'ogreoggsound')
PATH_VORBIS = os.path.join(BASE_DIR, 'ogreoggsound')
PATH_OPENAL = os.path.join(BASE_DIR, 'OpenAL.1.1' )
PATH_ogreoggsound = os.path.join(BASE_DIR,'ogreoggsound')
PATH_INCLUDE_OGG = os.path.join( PATH_OGG, 'include/ogg' )
PATH_INCLUDE_VORBIS = os.path.join( PATH_VORBIS, 'include/vorbis' )
PATH_INCLUDE_OPENAL = os.path.join( PATH_OPENAL, 'include')
PATH_INCLUDE_ogreoggsound = os.path.join(PATH_ogreoggsound,'include')
PATH_LIB_ogreoggsound = os.path.join(PATH_ogreoggsound,'lib')

## Raknet
PATH_raknet = os.path.join(BASE_DIR, 'raknet')
PATH_LIB_raknet = os.path.join ( PATH_raknet, 'Lib/LibStatic/Release')
PATH_INCLUDE_raknet = os.path.join ( PATH_raknet, 'Source')

## Hydrax
PATH_hydrax = os.path.join(BASE_DIR, 'hydrax')
PATH_LIB_hydrax = os.path.join ( PATH_hydrax, 'Hydrax/lib')
PATH_INCLUDE_hydrax= os.path.join(PATH_hydrax,'Hydrax/include')

## Bullet
# # PATH_OgreBullet=    os.path.join(PATH_OgreAddons,'ogrebullet')
PATH_OgreBullet=    os.path.join(PATH_THIRDPARTY,'ogrebullet')
PATH_Bullet=        os.path.join(BASE_DIR, 'bullet-2.76')
#PATH_LIB_OgreBullet =              os.path.join( PATH_OgreBullet, 'lib/Release') 
#PATH_LIB_Bullet =               os.path.join(PATH_Bullet, 'out/release8/libs' )
#PATH_LIB_Bullet =               os.path.join(PATH_Bullet, 'out/release_dll8/libs' )
PATH_LIB_Bullet =               os.path.join(PATH_Bullet, 'lib', 'Release' )
#PATH_LIB_Bullet =               os.path.join(PATH_Bullet, 'out/debug8/libs' )
# # PATH_INCLUDE_OgreBullet =   [ 
# #                         os.path.join( PATH_OgreBullet,'Collisions','include')
# #                         ,os.path.join( PATH_OgreBullet,'Dynamics','include')
# #                         ]
##PATH_INCLUDE_OgreBullet =   PATH_OgreBullet
PATH_INCLUDE_Bullet=        os.path.join(PATH_Bullet, 'src')


PATH_Newton =       os.path.join(BASE_DIR, 'newtonsdk','sdk')
PATH_Newton2 =       os.path.join(BASE_DIR, 'newtonsdk2','sdk')
PATH_FMOD =         os.path.join(BASE_DIR, 'fmod')
PATH_ODE =          os.path.join(BASE_DIR, 'ode-0.11.1')
PATH_OgreOde=       os.path.join(BASE_DIR, PATH_OgreAddons,'ogreode')
# # PATH_ogreforests=    os.path.join(PATH_OgreAddons,'forests')

PATH_ogreforests=   os.path.join(PATH_THIRDPARTY,'forests')
PATH_OGREAL=        os.path.join(PATH_THIRDPARTY,'ogreal')

# PATH_OGREAL=        os.path.join(BASE_DIR,'ogreal')

PATH_OPCODE=        os.path.join(PATH_THIRDPARTY,'opcode')
PATH_quickgui=      os.path.join(PATH_THIRDPARTY,'quickgui')
PATH_betagui=       os.path.join(PATH_THIRDPARTY,'betagui')
PATH_ogredshow =    os.path.join(PATH_THIRDPARTY,'dshow')
PATH_plib =         os.path.join(PATH_THIRDPARTY, 'plib')
PATH_et =           os.path.join(PATH_THIRDPARTY, 'et')
PATH_caelum =       os.path.join(PATH_THIRDPARTY, 'caelum')
PATH_NxOgre=        os.path.join(BASE_DIR, 'nxogre_0.22')
PATH_plsm2 =        os.path.join(PATH_THIRDPARTY, 'plsm2' )

# # # PATH_NxOgre=        os.path.join(PATH_THIRDPARTY, 'nxogre')
PATH_NxOgre_09=        os.path.join(PATH_THIRDPARTY, 'nxogre_0.9')
PATH_watermesh=        os.path.join(PATH_THIRDPARTY, 'watermesh')
PATH_noise=        os.path.join(PATH_THIRDPARTY, 'noise')
PATH_ofusion=        os.path.join(PATH_THIRDPARTY, 'ofusion')
PATH_cadunetree=        os.path.join(PATH_THIRDPARTY, 'cadunetree')
PATH_opensteer =        os.path.join(BASE_DIR, 'opensteer', 'trunk')

PATH_ogrevideoffmpeg = os.path.join(PATH_THIRDPARTY,'ffmpeg')
PATH_PhysX=         "c:/program files/NVIDIA Corporation/NVIDIA Physx SDK/v2.8.1/SDKs"
PATH_Theora=        os.path.join(BASE_DIR,'ogrevideo','trunk','TheoraVideoPlugin')

PATH_ffmpeg=        os.path.join(PATH_THIRDPARTY,'extra')
PATH_navi =         os.path.join(BASE_DIR, 'navi', 'navi')

PATH_hikari = os.path.join(PATH_THIRDPARTY, 'Hikari' ) #BASE_DIR, 'hikari', 'hikari')
PATH_mygui = os.path.join(BASE_DIR, 'MyGUI2.2.3_source' ) # \development\MyGUI2.2.2_source\MyGUIEngine\include
PATH_canvas = os.path.join(PATH_THIRDPARTY, 'canvas')
 
PATH_ogrevideo =  os.path.join(BASE_DIR,'ogrevideo','trunk')
PATH_INCLUDE_ogrevideo  =  os.path.join(PATH_ogrevideo,'include')

PATH_LIB_ogrevideo      =  os.path.join(PATH_ogrevideo,'lib') # 'bin','Release_static')
PATH_DEMO_ogrevideo   =  os.path.join(PATH_ogrevideo, 'demos', 'audio_openal')

PATH_libtheoraplayer = os.path.join(BASE_DIR, "libtheoraplayer")

###
### these paths assume you've left all the directory structure as standard
### Override anything that is different
###        

PATH_LIB_OgreRefApp =           os.path.join( PATH_Ogre, 'lib')
PATH_LIB_OgreNewt =             os.path.join( PATH_OgreAddons, 'ogrenewt/OgreNewt_Main/lib/Release') 
PATH_LIB_OgreNewt2 =             os.path.join( PATH_OgreAddons, 'ogrenewt2/lib') 
PATH_LIB_Newton =               os.path.join( PATH_Newton ,'dll')  ##NOTE Posix platforms this lives in 'lib-mt'
PATH_LIB_Newton2 =               os.path.join( PATH_Newton2 ,'x32','lib_vs9_mt')  ##NOTE Posix platforms this lives in 'lib-mt'

PATH_LIB_ODE =                  os.path.join( PATH_ODE, 'lib/ReleaseSingleLib')## probable releaselib for posix
PATH_LIB_OPCODE =               os.path.join( PATH_OPCODE ) 
PATH_LIB_OgreOde =              os.path.join( PATH_OgreOde, 'lib/Release') 
PATH_LIB_OgreOdePrefab =        os.path.join( PATH_OgreOde, 'prefab/lib/Release' )
PATH_LIB_OgreOdeLoader =        os.path.join( PATH_OgreOde, 'loader/lib/Release' )

PATH_LIB_OgreAL =               os.path.join( PATH_OGREAL ) ##, 'lib/Release' )
PATH_LIB_betagui =              PATH_betagui
PATH_LIB_quickgui =             PATH_quickgui
PATH_LIB_NxOgre=                os.path.join(PATH_NxOgre, 'lib')
PATH_LIB_PhysX =                os.path.join(PATH_PhysX,'lib/win32')

PATH_LIB_Theora=                os.path.join(PATH_Theora, 'bin', 'Release')
PATH_LIB_ogrevideoffmpeg =      PATH_ogrevideoffmpeg
PATH_LIB_ffmpeg=                os.path.join(PATH_ffmpeg, 'lib')
PATH_LIB_plib =                 PATH_plib
PATH_LIB_navi=                  os.path.join(PATH_navi, 'lib')
PATH_LIB_ogredshow =            PATH_ogredshow
PATH_LIB_opensteer = os.path.join(PATH_opensteer, 'win32','release')
PATH_LIB_hikari = os.path.join(PATH_hikari ) # , 'lib')
PATH_LIB_mygui = os.path.join(PATH_mygui, 'MyGUIEngine','lib','Release')
PATH_LIB_canvas = os.path.join(PATH_canvas )
PATH_LIB_plsm2 = PATH_plsm2
PATH_LIB_OPENAL=        os.path.join(PATH_OPENAL, 'libs','win32' ) ##'release') #'openal')                
    
PATH_INCLUDE_NEDMALLOC=      "" #os.path.join(PATH_Ogre,'OgreMain', 'src', 'nedmalloc')
PATH_INCLUDE_OgreRefApp =   os.path.join(PATH_Ogre,'ReferenceApplication/ReferenceAppLayer/include') 
PATH_INCLUDE_OgreNewt =     os.path.join(PATH_OgreAddons,'ogrenewt/OgreNewt_Main/inc')
PATH_INCLUDE_OgreNewt2 =     os.path.join(PATH_OgreAddons,'ogrenewt2/inc')
PATH_INCLUDE_ODE =          os.path.join( PATH_ODE, 'include')
PATH_INCLUDE_OPCODE =       PATH_OPCODE
PATH_INCLUDE_ODESOURCE =    os.path.join( PATH_ODE, 'ode/src')

PATH_INCLUDE_OgreAL =       os.path.join( PATH_OGREAL ) #, 'include' )                        
PATH_INCLUDE_OgreOde =      os.path.join( PATH_OgreOde,'Core','include')
PATH_INCLUDE_OgreOdePrefab= os.path.join( PATH_OgreOde,'prefab','include')
PATH_INCLUDE_OgreOdeLoader= os.path.join( PATH_OgreOde,'loader','include')
PATH_INCLUDE_FMOD =          os.path.join(PATH_FMOD, 'api/inc')
PATH_INCLUDE_quickgui =     PATH_quickgui
PATH_INCLUDE_NxOgre_09=        PATH_NxOgre_09
PATH_INCLUDE_NxOgre=        os.path.join(PATH_NxOgre, 'include')
# # # PATH_INCLUDE_NxOgre=        PATH_NxOgre
PATH_INCLUDE_freetype=      os.path.join(PATH_INCLUDE_quickgui,'FreeType2.3.5')    
PATH_INCLUDE_betagui =      PATH_betagui
PATH_INCLUDE_Theora =       os.path.join (PATH_Theora,'include')
# need the audio files
PATH_INCLUDE_TheoraDemo =       os.path.join (PATH_Theora,'..', 'demos','audio_openal')

# # PATH_INCLUDE_TheoraDemo =   PATH_Theora
PATH_INCLUDE_plsm2 = PATH_plsm2

PATH_INCLUDE_ogrevideoffmpeg =  PATH_ogrevideoffmpeg
PATH_INCLUDE_ffmpeg =       os.path.join (PATH_ffmpeg,'include')
PATH_INCLUDE_ogredshow =    PATH_ogredshow
PATH_INCLUDE_plib =         PATH_plib
PATH_INCLUDE_navi =         os.path.join (PATH_navi,'include')
PATH_INCLUDE_noise = PATH_noise 
##PATH_INCLUDE_ogreforests =         os.path.join (PATH_ogreforests,'include')
PATH_INCLUDE_ogreforests =         PATH_ogreforests
PATH_INCLUDE_particleuniverse = os.path.join(PATH_particleuniverse, 'include' )
PATH_INCLUDE_ogrepcz = os.path.join(PATH_ogrepcz,'include')
PATH_INCLUDE_hikari = os.path.join(PATH_hikari ) #, 'include')
PATH_INCLUDE_mygui = os.path.join(PATH_mygui,'MyGUIEngine','include') 
PATH_INCLUDE_canvas=        PATH_canvas

PATH_INCLUDE_OggVorbisTheora = [ os.path.join(BASE_DIR,'ogg','include')
                        ,os.path.join(BASE_DIR, 'vorbis', 'include')
                        ,os.path.join(BASE_DIR, 'theora', 'include')
                        ,os.path.join(BASE_DIR,'ptypes','include')
                        ,os.path.join(PATH_INCLUDE_OPENAL, 'AL') 
                        ]
                        
PATH_INCLUDE_PhysX_Samples = os.path.join(PATH_PhysX, '..', 'Samples','SampleCommonCode','src')

PATH_INCLUDE_PhysX= [ os.path.join(PATH_PhysX, 'Physics','include')
                    ,os.path.join(PATH_PhysX, 'Cooking','include')
                    ,os.path.join(PATH_PhysX, 'Foundation','include')
                    ,os.path.join(PATH_PhysX, 'PhysXLoader','include')
                    ,os.path.join(PATH_PhysX, 'NxCharacter','include')
                    ,PATH_INCLUDE_PhysX_Samples
# #                     ,os.path.join(PATH_PhysX, 'NxExtensions','include')
                    ]
PATH_INCLUDE_opensteer = os.path.join(PATH_opensteer, 'include')
PATH_INCLUDE_opensteer_opensteer = os.path.join(PATH_opensteer, 'include', 'OpenSteer')

if _SDK:
    PATH_Ogre =         "C:\\development\\ogre"
    PATH_INCLUDE_Ogre=          os.path.join(PATH_Ogre,'include','OGRE')
    PATH_INCLUDE_Ogre_Dependencies = os.path.join(PATH_Ogre,'include')
    PATH_LIB_Ogre_OgreMain =        os.path.join( PATH_Ogre, 'lib', 'release' )
    PATH_LIB_Ogre_OgreMain_debug =        os.path.join( PATH_Ogre, 'lib', 'debug' )

    PATH_OIS =          PATH_Ogre
    PATH_INCLUDE_OIS =          os.path.join(PATH_Ogre,'include','OIS')
    PATH_LIB_OIS =                  os.path.join( PATH_Ogre, 'lib', 'release')
    PATH_LIB_OIS_debug  =                  os.path.join( PATH_Ogre, 'lib', 'debug')

#     PATH_CEGUI =        PATH_Ogre
#     PATH_INCLUDE_CEGUI =        os.path.join(PATH_Ogre, 'include/CEGUI')
#     PATH_LIB_CEGUI =                os.path.join( PATH_Ogre, 'lib', 'release' )
#     PATH_INCLUDE_Ogre_CEGUIRenderer = os.path.join( PATH_Ogre, 'Samples','Common','include')
#     PATH_LIB_Ogre_CEGUIRenderer =   os.path.join( PATH_Ogre, 'lib', 'release')

    PATH_LIB_pcz =                  os.path.join( PATH_Ogre, 'lib', 'release','opt' )
    PATH_INCLUDE_ogrepcz = os.path.join(PATH_INCLUDE_Ogre,'Plugins','PCZSceneManager')
    PATH_INCLUDE_ogrepaging =   os.path.join(PATH_INCLUDE_Ogre,'Paging')
    PATH_INCLUDE_ogrertshadersystem =   os.path.join(PATH_INCLUDE_Ogre,'RTShaderSystem')
    PATH_INCLUDE_ogreterrain =   os.path.join(PATH_INCLUDE_Ogre,'Terrain')
    PATH_INCLUDE_ogreoctreezone = os.path.join(PATH_INCLUDE_Ogre,'Plugins', 'OctreeZone' )
    PATH_INCLUDE_ogreoctreesm = os.path.join(PATH_INCLUDE_Ogre,'Plugins', 'OctreeSceneManager' )
    
#     PATH_INCLUDE_sdksample = os.path.join(PATH_Ogre, "Samples", "Common", "include")
    PATH_INCLUDE_sdksample = os.path.join(PATH_THIRDPARTY, "sdksample")
