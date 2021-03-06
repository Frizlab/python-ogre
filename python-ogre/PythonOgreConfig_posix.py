import sys,os

## The root directory is where this module is located
module_dir = os.path.abspath(os.path.dirname(__file__) )## The root directory is where this module is located
## lets assume that the base development directory is one level higher
BASE_DIR,ignore = os.path.split(module_dir) ##  r'/home/andy/development'

_SDK = False
_SVN = False    # if building from Ogre svn

# the base of the /usr/... directory structure that we are using
ROOT_DIR = os.path.join(BASE_DIR,'root')
LOCAL_LIB = os.path.join(ROOT_DIR,'usr/lib')
LOCAL_INCLUDE = os.path.join(ROOT_DIR, 'usr/include')



## path to the root of your boost dir, should have 'libs', 'boost' subdirectories
PATH_Boost = None
## Path to your boost_pythonxxxx lib file
PATH_LIB_Boost = LOCAL_LIB
PATH_LIB_Thread_STATIC = LOCAL_LIB
PATH_LIB_date_time_STATIC = LOCAL_LIB
## and the name of the boost python library
LIB_Boost = "libboost_python" 

# in Linux we need to code in the Relative path for the library
RPATH=os.path.join('\\$$ORIGIN',os.pardir,os.pardir, 'lib') #+ '\''

# We need to know where to find gccxml......
gccxml_bin = os.path.join(BASE_DIR,'gccxml-build/bin')
gccxml_bin = os.path.join(ROOT_DIR,'usr/bin')
# and the Py++ directory as sometimes we need access to the code repository there
pyplusplus_install_dir = os.path.join(BASE_DIR,'pygccxml')

isMAC = False
if os.sys.platform == 'darwin':  # for the Mac
   isMAC = True
   MAC_SDK_INCLUDE= '' #/Developer/SDKs/MacOSX10.5.sdk/Developer/Headers/FlatCarbon'
   MAC_CCFLAGS = ' -D__ASSERTMACROS__ '
   MAC_cflags = '  -D__APPLE_CC__ -DCF_OPEN_SOURCE  \
             -I. \
             -F/Developer/SDKs/MacOSX10.5.sdk/System/Library/Frameworks \
             -DBOOST_PYTHON_NO_PY_SIGNATURES -DBOOST_PYTHON_MAX_ARITY=19  \
             -I' + os.path.join(module_dir,'code_generators','ogre') + ' '
   MAC_AdditionalFrameWorks=[ os.path.abspath(os.path.join(BASE_DIR,'ogre/Mac/build/Release')),  # this one can be removed as Ogr eis now copied to the user frameworks area
                             os.path.abspath(os.path.join(os.path.expanduser('~'), 'Library','Frameworks'))]


#    _SDK = True
#    FRAMEWORK_DIR = os.path.abspath(os.path.join(BASE_DIR, '..', 'Library','Frameworks'))   ## need this for the Mac
#    MAC_SDK_INCLUDE= '/Developer/SDKs/MacOSX10.4u.sdk/Developer/Headers/CFMCarbon'
#    MAC_CCFLAGS = ' -DBOOST_PYTHON_MAX_ARITY=19 -D_POSIX_C_SOURCE -DCF_OPEN_SOURCE'
#    MAC_cflags = ' --gccxml-cxxflags "-DCF_OPEN_SOURCE -D_POSIX_C_SOURCE -isysroot /Developer/SDKs/MacOSX10.4u.sdk"'



# # ## Parent directories of the libraries
PATH_THIRDPARTY =   os.path.join(module_dir, 'ThirdParty' )
PATH_Ogre =         os.path.join(BASE_DIR,'ogre')
PATH_OgreAddons =   os.path.join(BASE_DIR, 'ogreaddons')
PATH_CEGUI =        os.path.join(BASE_DIR, 'CEGUI-0.7.5' )
PATH_OIS =          os.path.join(BASE_DIR, 'ois')

if _SDK:
    PATH_CEGUI =        os.path.join(BASE_DIR, 'ogre','Dependencies')
    PATH_OIS =          os.path.join(BASE_DIR, 'ogre','Dependencies')
if isMAC:
    PATH_CEGUI =        os.path.join(BASE_DIR, 'ogre','Dependencies')
    #PATH_OIS = os.path.join(BASE_DIR, 'ogre','Dependencies')
    
## Raknet
PATH_raknet = os.path.join(BASE_DIR, 'raknet')
PATH_LIB_raknet = os.path.join ( PATH_raknet, 'Lib/LibStatic')
PATH_INCLUDE_raknet = os.path.join ( PATH_raknet, 'Source')

## Hydrax
PATH_hydrax = os.path.join(BASE_DIR, 'Hydrax-v0.5.1')
PATH_LIB_hydrax = os.path.join ( PATH_hydrax, 'Hydrax/bin')
PATH_INCLUDE_hydrax = [ os.path.join(PATH_hydrax,'Hydrax/include'),
                        os.path.join(PATH_hydrax,'Hydrax/include/Modules/ProjectedGrid'),
                        os.path.join(PATH_hydrax,'Hydrax/include/Modules/SimpleGrid'),
                        os.path.join(PATH_hydrax,'Hydrax/include/Noise/Perlin') ]    


PATH_Newton =       os.path.join(BASE_DIR, 'newtonSDK','sdk')
PATH_Newton2 =       os.path.join(BASE_DIR, 'newtonSDK','sdk')
PATH_ODE =          os.path.join(BASE_DIR, 'ode-0.11')
PATH_OGG =          os.path.join(BASE_DIR, 'libogg-1.1.3')
PATH_VORBIS=        os.path.join(BASE_DIR, 'libvorbis-1.2.0')
PATH_OPENAL=        os.path.join(BASE_DIR, 'openal-soft-1.5.304')
PATH_OgreOde=       os.path.join(PATH_OgreAddons,'ogreode')

PATH_OgreBullet=    os.path.join(PATH_THIRDPARTY,'ogrebullet')
PATH_ogreforests=   os.path.join(PATH_THIRDPARTY,'forests')
PATH_OGREAL=        os.path.join(PATH_THIRDPARTY,'ogreal')
PATH_OPCODE=        os.path.join(PATH_THIRDPARTY,'opcode')
PATH_quickgui=      os.path.join(PATH_THIRDPARTY,'quickgui')
PATH_betagui=       os.path.join(PATH_THIRDPARTY,'betagui')
PATH_ogredshow =    os.path.join(PATH_THIRDPARTY,'dshow')
PATH_plib =         os.path.join(PATH_THIRDPARTY, 'plib')
PATH_et =           os.path.join(PATH_THIRDPARTY, 'et')
PATH_caelum =       os.path.join(PATH_THIRDPARTY, 'caelum')
PATH_noise=         os.path.join(PATH_THIRDPARTY, 'noise')
PATH_watermesh=     os.path.join(PATH_THIRDPARTY, 'watermesh')
PATH_ofusion=       os.path.join(PATH_THIRDPARTY, 'ofusion')
PATH_ogrevideoffmpeg = os.path.join(PATH_THIRDPARTY,'ffmpeg')
PATH_NxOgre=        os.path.join(PATH_THIRDPARTY, 'nxogre')
PATH_NxOgre_09=        os.path.join(PATH_THIRDPARTY, 'nxogre_0.9')
# PATH_NxOgre=        os.path.join(BASE_DIR, 'nxogre/NxOgre')
PATH_Bullet=        os.path.join(BASE_DIR, 'bullet-svn-r2243')
###PATH_PhysX=         os.path.join(BASE_DIR, 'Physx/v2.7.3/SDKs')
PATH_Theora=        os.path.join(PATH_OgreAddons,'videoplugin','TheoraVideo')
PATH_ffmpeg=        os.path.join(BASE_DIR, 'ffmpeg')
PATH_navi =         os.path.join(BASE_DIR, 'navi','Navi')
PATH_particleuniverse = os.path.join(PATH_Ogre, 'PlugIns', 'ParticleUniverse' )
PATH_cadunetree=        os.path.join(PATH_THIRDPARTY, 'cadunetree')
PATH_opensteer =        os.path.join(BASE_DIR, 'opensteer', 'trunk')
PATH_ogrepcz = os.path.join(PATH_Ogre, 'PlugIns', 'PCZSceneManager' )
PATH_hikari = os.path.join(PATH_THIRDPARTY, 'Hikari' ) # BASE_DIR, 'hikari', 'hikari')
PATH_mygui = os.path.join(BASE_DIR, 'MyGUI_2.2.0_RC1_source' )
PATH_canvas = os.path.join(PATH_THIRDPARTY, 'canvas')
PATH_plsm2 =        os.path.join(PATH_THIRDPARTY, 'plsm2' )

PATH_ogreoggsound = os.path.join(BASE_DIR,'ogreoggsound')
PATH_INCLUDE_ogreoggsound = os.path.join(PATH_ogreoggsound,'include')
PATH_LIB_ogreoggsound = os.path.join(PATH_ogreoggsound)

###
### these paths assume you've left all the directory structure as standard
### Override anything that is different
###
PATH_LIB_Ogre_CEGUIRenderer =    os.path.join( LOCAL_LIB ) #, 'Samples/Common/CEGUIRenderer/lib')
PATH_LIB_Ogre_OgreMain=         os.path.join( LOCAL_LIB ) # PATH_Ogre, 'OgreMain/lib/Release' )
PATH_LIB_Ogre_Dependencies =    os.path.join( LOCAL_LIB ) # , 'Dependencies/lib/Release')
PATH_LIB_OgreRefApp =           os.path.join( LOCAL_LIB ) # PATH_Ogre, 'ReferenceApplication/ReferenceAppLayer/lib/Release')
PATH_LIB_OgreNewt =             os.path.join( LOCAL_LIB ) #PATH_OgreAddons, r'ogrenewt/OgreNewt_Main/lib/Release')
PATH_LIB_OgreNewt2 =             os.path.join( LOCAL_LIB ) #PATH_OgreAddons, r'ogrenewt/OgreNewt_Main/lib/Release')
PATH_LIB_Newton =               os.path.join( LOCAL_LIB ) # PATH_Newton ,'dll')  ##NOTE Posix platforms this lives in 'lib-mt'
PATH_LIB_Newton2 =               os.path.join( LOCAL_LIB ) # PATH_Newton ,'dll')  ##NOTE Posix platforms this lives in 'lib-mt'
PATH_LIB_Ogre_OgreMain_debug =   os.path.join( LOCAL_LIB )

PATH_LIB_OIS =                  os.path.join( LOCAL_LIB ) #PATH_OIS, 'dll') ## NOTE Posix platforms this lives in'lib' not 'dll'
PATH_LIB_OIS_debug  =                  os.path.join( LOCAL_LIB )
PATH_LIB_CEGUI =                os.path.join( LOCAL_LIB ) #PATH_Ogre, r'Dependencies/lib/Release' )
if _SDK:
    PATH_LIB_OIS =                  os.path.join( PATH_OIS,'lib', 'Release' ) #PATH_OIS, 'dll') ## NOTE Posix platforms this lives in'lib' not 'dll'
    PATH_LIB_CEGUI =                os.path.join( PATH_CEGUI,'lib', 'Release' ) #PATH_Ogre, r'Dependencies/lib/Release' )
    PATH_LIB_Ogre_OgreMain=         os.path.join( PATH_Ogre, 'OgreMain/lib/Release' )

if isMAC:
    PATH_LIB_OIS =                  os.path.join( PATH_OIS,'Mac', 'XCode-2.2', 'build', 'Release' ) #PATH_OIS, 'dll') ## NOTE Posix platforms this lives in'lib' not 'dll'
    #PATH_LIB_OIS = 				os.path.join(PATH_OIS,'lib','Release')
    PATH_LIB_CEGUI =                os.path.join( PATH_CEGUI,'lib', 'Release' ) #PATH_Ogre, r'Dependencies/lib/Release' )

PATH_LIB_ODE =                  os.path.join( LOCAL_LIB ) #PATH_ODE, 'lib/releasedll')## probable releaselib for posix
PATH_LIB_OPCODE =               os.path.join( PATH_OPCODE )
PATH_LIB_OgreOde =              os.path.join( LOCAL_LIB ) #PATH_OgreOde, 'lib/Release')
# PATH_LIB_OgreBullet =              os.path.join( PATH_OgreBullet, 'lib/Release')
PATH_LIB_OgreOdePrefab =        os.path.join( LOCAL_LIB ) #PATH_OgreOde, 'prefab/lib/Release' )
PATH_LIB_OgreOdeLoader =        os.path.join( LOCAL_LIB ) #PATH_OgreOde, 'loader/lib/Release' )
PATH_LIB_OgreAL =               os.path.join( LOCAL_LIB ) #PATH_OGREAL, 'lib/Release' )
PATH_LIB_betagui =              PATH_betagui
PATH_LIB_quickgui =             PATH_quickgui
PATH_LIB_NxOgre=                os.path.join(LOCAL_LIB )
PATH_LIB_PhysX =                os.path.join('/usr/lib', 'PhysX','v2.8.1')
PATH_LIB_Bullet = 		        os.path.join( LOCAL_LIB )
PATH_LIB_Theora=                os.path.join(PATH_Theora, 'bin', 'Release')
PATH_LIB_ogrevideoffmpeg =      PATH_ogrevideoffmpeg
PATH_LIB_ffmpeg=                os.path.join(PATH_ffmpeg, 'lib')
PATH_LIB_plib =                 PATH_plib
PATH_LIB_navi=                  os.path.join(PATH_navi, 'lib')
PATH_LIB_particleuniverse =     os.path.join(PATH_particleuniverse, 'bin', 'release')
PATH_LIB_opensteer = os.path.join(LOCAL_LIB)
PATH_LIB_hikari = os.path.join(PATH_hikari ) #, 'lib')
PATH_LIB_mygui = os.path.join(PATH_mygui, 'MyGUIEngine','lib','Release')
PATH_LIB_canvas = os.path.join(PATH_canvas )
PATH_LIB_pcz = os.path.join(PATH_LIB_Ogre_OgreMain, 'OGRE')
PATH_LIB_plsm2 = PATH_plsm2
PATH_LIB_OPENAL=  PATH_OPENAL ## ????????????????


PATH_INCLUDE_Ogre=          os.path.join(LOCAL_INCLUDE,'OGRE')  # os.path.join(PATH_Ogre,'OgreMain/include')
PATH_INCLUDE_Ogre_Dependencies =    PATH_INCLUDE_Ogre #      os.path.join( PATH_Ogre, 'Dependencies/include')

PATH_INCLUDE_OIS =          os.path.join(LOCAL_INCLUDE,'OIS') #os.path.join(PATH_OIS,'includes')    ## Note the plural include's
PATH_INCLUDE_CEGUI =        os.path.join(LOCAL_INCLUDE,'CEGUI') #os.path.join(PATH_CEGUI, r'include/CEGUI')

PATH_INCLUDE_ogrepaging =   os.path.join(PATH_Ogre,'Components','Paging', 'include')
PATH_INCLUDE_ogrertshadersystem =   os.path.join(PATH_Ogre,'Components','RTShaderSystem', 'include')
PATH_INCLUDE_ogreterrain =   os.path.join(PATH_Ogre,'Components','Terrain', 'include')
PATH_INCLUDE_ogreoctreezone = os.path.join(PATH_Ogre,'Plugins', 'OctreeZone', 'include' )
PATH_INCLUDE_ogreoctreesm = os.path.join(PATH_Ogre,'Plugins', 'OctreeSceneManager', 'include' )
PATH_INCLUDE_sdksample = os.path.join(PATH_Ogre, "Samples", "Common", "include"),
PATH_INCLUDE_ogreproperty = os.path.join(PATH_Ogre,'Components','Property', 'include')


PATH_ogrevideo =  os.path.join(BASE_DIR,'ogrevideo','trunk')
PATH_INCLUDE_ogrevideo  =  os.path.join(PATH_ogrevideo,'include')

PATH_LIB_ogrevideo      =  os.path.join(PATH_ogrevideo,'lib') # 'bin','Release_static')
PATH_DEMO_ogrevideo   =  os.path.join(PATH_ogrevideo, 'demos', 'audio_openal')

if _SDK :
    PATH_INCLUDE_OIS =          os.path.join(PATH_OIS,'include','OIS') #os.path.join(PATH_OIS,'includes')    ## Note the plural include's
    PATH_INCLUDE_CEGUI =        os.path.join(PATH_CEGUI,'include','CEGUI') #os.path.join(PATH_CEGUI, r'include/CEGUI')
    PATH_INCLUDE_Ogre=          os.path.join(PATH_Ogre,'OgreMain/include')
    PATH_INCLUDE_Ogre_Dependencies =    os.path.join( PATH_Ogre, 'Dependencies/include')

if isMAC:
    PATH_INCLUDE_OIS =          os.path.join(PATH_OIS,'includes') #os.path.join(PATH_OIS,'includes')    ## Note the plural include's
    PATH_INCLUDE_CEGUI =        os.path.join(PATH_Ogre, 'Dependencies','include', 'CEGUI' ) #PATH_CEGUI,'include','CEGUI') #os.path.join(PATH_CEGUI, r'include/CEGUI')
    PATH_INCLUDE_Ogre=          os.path.join(PATH_Ogre,'OgreMain','include')
    PATH_INCLUDE_Ogre_Dependencies =    os.path.join( PATH_Ogre, 'Dependencies','include')


PATH_INCLUDE_OgreRefApp =   os.path.join(PATH_Ogre,'ReferenceApplication/ReferenceAppLayer/include')
PATH_INCLUDE_OgreNewt =     os.path.join(LOCAL_INCLUDE,'OgreNewt') #os.path.join(PATH_OgreAddons,'ogrenewt/OgreNewt_Main/inc')
PATH_INCLUDE_OgreNewt2 =     os.path.join(LOCAL_INCLUDE,'OgreNewt') #os.path.join(PATH_OgreAddons,'ogrenewt/OgreNewt_Main/inc')
PATH_INCLUDE_ODE =          os.path.join(LOCAL_INCLUDE,'ode') #os.path.join( PATH_ODE, 'include')
PATH_INCLUDE_OPCODE =       PATH_OPCODE
PATH_INCLUDE_ODESOURCE =    '' #      os.path.join( PATH_ODE, 'ode/src')
PATH_INCLUDE_OgreAL =       PATH_OGREAL
PATH_INCLUDE_OPENAL =       '/usr/include' #os.path.join( LOCAL_INCLUDE ) #os.path.join( PATH_OPENAL, 'include')
PATH_INCLUDE_OGG=           '/usr/include/ogg' #os.path.join( LOCAL_INCLUDE,'ogg' ) #os.path.join( PATH_OGG, 'include' )
PATH_INCLUDE_VORBIS=        '/usr/include/vorbis' #os.path.join( LOCAL_INCLUDE,'vorbis' ) #os.path.join( PATH_VORBIS, 'include' )

# PATH_INCLUDE_OgreBullet =   [
#                         os.path.join( PATH_OgreBullet,'Collisions','include')
#                         ,os.path.join( PATH_OgreBullet,'Dynamics','include')
#                         ]


PATH_INCLUDE_OgreOde =      os.path.join( PATH_OgreOde,'include')
PATH_INCLUDE_OgreOdePrefab= os.path.join( PATH_OgreOde,'prefab/include')
PATH_INCLUDE_OgreOdeLoader= os.path.join( PATH_OgreOde,'loader/include')
PATH_INCLUDE_FMOD =  ""
#PATH_INCLUDE_Ogre_CEGUIRenderer = os.path.join( PATH_Ogre, 'Samples/Common/CEGUIRenderer/include')
PATH_INCLUDE_Ogre_CEGUIRenderer = os.path.join( PATH_INCLUDE_CEGUI, 'RendererModules', 'Ogre')
PATH_INCLUDE_quickgui =     PATH_quickgui
PATH_INCLUDE_NxOgre=        PATH_NxOgre
PATH_INCLUDE_NxOgre_09=        PATH_NxOgre_09
# PATH_INCLUDE_NxOgre=        os.path.join(PATH_NxOgre, 'include')
PATH_INCLUDE_Bullet = 		os.path.join( PATH_Bullet, 'src')
PATH_INCLUDE_betagui =       PATH_betagui
PATH_INCLUDE_Theora =       os.path.join (PATH_Theora,'include')
PATH_INCLUDE_TheoraDemo =       os.path.join (PATH_Theora,'..', 'CEGUI_TheoraDemo','include')

PATH_INCLUDE_ogrevideoffmpeg =  PATH_ogrevideoffmpeg
PATH_INCLUDE_ffmpeg =       os.path.join (PATH_ffmpeg,'include')
PATH_INCLUDE_plib =         PATH_plib
PATH_INCLUDE_navi =     os.path.join (PATH_navi,'include')
PATH_INCLUDE_noise = PATH_noise
PATH_INCLUDE_ogredshow =    PATH_ogredshow
PATH_INCLUDE_ogreforests =         PATH_ogreforests
PATH_INCLUDE_particleuniverse = os.path.join(PATH_particleuniverse, 'include' )

PATH_INCLUDE_OggVorbisTheora = [ os.path.join(BASE_DIR,'ogg','include')
                        ,os.path.join(BASE_DIR, 'vorbis', 'include')
                        ,os.path.join(PATH_OgreAddons,'videoplugin','theora','include')
                        ##,os.path.join(PATH_OgreAddons,'videoplugin','ptypes-1.8.3','include')
                        ,os.path.join(PATH_THIRDPARTY,'ptypes','include')
                        ]
PATH_INCLUDE_ogrevideo = LOCAL_INCLUDE
PATH_DEMO_ogrevideo = LOCAL_INCLUDE
PATH_INCLUDE_libtheoraplayer = LOCAL_INCLUDE
PATH_LIB_ogrevideo = LOCAL_LIB
PATH_libtheoraplayer =  LOCAL_LIB


physxbase = os.path.join('/usr/include/', 'PhysX','v2.8.1', 'SDKs')
PATH_INCLUDE_PhysX= [ physxbase
                    ,os.path.join(physxbase, 'Cooking','include')
                    ,os.path.join(physxbase, 'Foundation','include')
                    ,os.path.join(physxbase, 'PhysXLoader','include')
                    ,os.path.join(physxbase, 'NxCharacter','include')
                    ##,os.path.join(physxbase, 'NxExtensions','include')
                    ,os.path.join(physxbase, 'Physics','include')
                    ]
PATH_INCLUDE_opensteer = os.path.join(LOCAL_INCLUDE, 'OpenSteer')
PATH_INCLUDE_opensteer_opensteer = os.path.join(LOCAL_INCLUDE, 'OpenSteer')
PATH_INCLUDE_ogrepcz = os.path.join(PATH_ogrepcz,'include')
PATH_INCLUDE_hikari = os.path.join(PATH_hikari ) #, 'include')
PATH_INCLUDE_mygui = os.path.join(PATH_mygui,'MyGUIEngine','include')
PATH_INCLUDE_canvas=        PATH_canvas
PATH_INCLUDE_plsm2 = PATH_plsm2

if os.sys.platform=='darwin':
   PATH_INCLUDE_ODE =          LOCAL_INCLUDE
   pass
