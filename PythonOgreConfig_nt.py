import os

SDK=True ## set to true if buiding from 1.4 release source...

## The root directory is where this module is located
module_dir = os.path.abspath(os.path.dirname(__file__) )
BASE_DIR, ignore = os.path.split(module_dir)

## path to the root of your boost dir, should have 'libs', 'boost' subdirectories
PATH_Boost =        os.path.join(BASE_DIR, 'boost')
## Path to your boost_pythonxxxx lib file
PATH_LIB_Boost =    os.path.join(BASE_DIR, 'boost/bin.v2/libs/python2.5/build/msvc-8.0/release/threading-multi')
## and the name of the boost python library
LIB_Boost =         r'boost_python-vc80-mt-1_35'

# We need to know where to find gccxml......
gccxml_bin =        os.path.join(BASE_DIR, 'gccxml/bin/release/gccxml.exe')
# and the Py++ directory as sometimes we need access to the code repository there
pyplusplus_install_dir = os.path.join(BASE_DIR, 'pygccxml')

## Parent directories of the libraries
PATH_THIRDPARTY =   os.path.join(module_dir, 'ThirdParty' )
PATH_Ogre =         os.path.join(BASE_DIR, 'ogrenew')
PATH_OgreAddons =   os.path.join(BASE_DIR, 'OgreAddons')
PATH_CEGUI =        os.path.join(BASE_DIR, 'CEGUI-0.5.0')
PATH_OIS =          os.path.join(BASE_DIR, 'ois-1.0RC1')
PATH_Newton =       os.path.join(BASE_DIR, 'newtonsdk','sdk')
PATH_FMOD =         os.path.join(BASE_DIR, 'fmod')
PATH_ODE =          os.path.join(BASE_DIR, 'ode-0.9')
PATH_OGG =          os.path.join(BASE_DIR, 'ogg')
PATH_VORBIS=        os.path.join(BASE_DIR, 'vorbis')
PATH_OPENAL=        os.path.join(BASE_DIR, 'openal')
PATH_ALUT=          os.path.join(BASE_DIR, 'freealut')
PATH_OgreOde=       os.path.join(BASE_DIR, PATH_OgreAddons,'ogreode')
# # PATH_OgreBullet=    os.path.join(PATH_OgreAddons,'ogrebullet')
# # PATH_ogreforests=    os.path.join(PATH_OgreAddons,'forests')

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
PATH_NxOgre=        os.path.join(PATH_THIRDPARTY, 'nxogre')
# PATH_NxOgre=        os.path.join(BASE_DIR, 'nxogre/NxOgre')
PATH_noise=        os.path.join(PATH_THIRDPARTY, 'noise')


PATH_ogrevideoffmpeg = os.path.join(PATH_THIRDPARTY,'ffmpeg')
PATH_Bullet=        os.path.join(BASE_DIR, 'bullet-2.62') 
PATH_PhysX=         "c:/program files/AGEIA Technologies/SDK/v2.7.2/SDKs"
PATH_Theora=        os.path.join(PATH_OgreAddons,'videoplugin','TheoraVideo')
PATH_ffmpeg=        os.path.join(BASE_DIR, 'ffmpeg')
PATH_navi =         os.path.join(BASE_DIR, 'navi','Navi')

# it's time for the SDK version
if SDK:
    PATH_CEGUI =        os.path.join(BASE_DIR, 'Ogrenew')
    PATH_OIS =          os.path.join(BASE_DIR, 'Ogrenew')
    
###
### these paths assume you've left all the directory structure as standard
### Override anything that is different
###        
PATH_LIB_Ogre_CEGUIRenderer =   os.path.join( PATH_Ogre, 'Samples/Common/CEGUIRenderer/lib')
PATH_LIB_Ogre_OgreMain=         os.path.join( PATH_Ogre, 'lib' )
PATH_LIB_Ogre_Dependencies =    os.path.join( PATH_Ogre, 'Dependencies/lib/Release')
PATH_LIB_OgreRefApp =           os.path.join( PATH_Ogre, 'lib')
PATH_LIB_OgreNewt =             os.path.join( PATH_OgreAddons, 'ogrenewt/OgreNewt_Main/lib/Release') 
PATH_LIB_Newton =               os.path.join( PATH_Newton ,'dll')  ##NOTE Posix platforms this lives in 'lib-mt'
PATH_LIB_OIS =                  os.path.join( PATH_OIS, 'dll') ## NOTE Posix platforms this lives in'lib' not 'dll'
PATH_LIB_CEGUI =                os.path.join( PATH_CEGUI, 'lib' )
PATH_LIB_ODE =                  os.path.join( PATH_ODE, 'lib/releaselib')## probable releaselib for posix
PATH_LIB_OPCODE =               os.path.join( PATH_OPCODE ) 
PATH_LIB_OgreOde =              os.path.join( PATH_OgreOde, 'lib/Release') 
# # PATH_LIB_OgreBullet =              os.path.join( PATH_OgreBullet, 'lib/Release') 
PATH_LIB_OgreOdePrefab =        os.path.join( PATH_OgreOde, 'prefab/lib/Release' )
PATH_LIB_OgreOdeLoader =        os.path.join( PATH_OgreOde, 'loader/lib/Release' )
PATH_LIB_OgreAL =               os.path.join( PATH_OGREAL)#, 'lib/Release' )
PATH_LIB_betagui =              PATH_betagui
PATH_LIB_quickgui =             PATH_quickgui
# PATH_LIB_NxOgre=                os.path.join(PATH_NxOgre, 'lib')
PATH_LIB_PhysX =                os.path.join(PATH_PhysX,'lib/win32')
PATH_LIB_Bullet =               os.path.join(PATH_Bullet, 'out/release8/libs' )
PATH_LIB_Theora=                os.path.join(PATH_Theora, 'bin', 'Release')
PATH_LIB_ogrevideoffmpeg =      PATH_ogrevideoffmpeg
PATH_LIB_ffmpeg=                os.path.join(PATH_ffmpeg, 'lib')
PATH_LIB_plib =                 PATH_plib
PATH_LIB_navi=                  os.path.join(PATH_navi, 'lib')
PATH_LIB_ogredshow =            PATH_ogredshow

if SDK:
    PATH_LIB_Ogre_CEGUIRenderer =   os.path.join( PATH_Ogre, 'lib')
    PATH_LIB_OIS =                  os.path.join( PATH_Ogre, 'Dependencies/lib/Release') ## NOTE Posix platforms this lives in'lib' not 'dll'
    PATH_LIB_CEGUI =                os.path.join( PATH_Ogre, 'lib' )



PATH_INCLUDE_Ogre=          os.path.join(PATH_Ogre,'OgreMain/include') 
PATH_INCLUDE_Ogre_Dependencies =  os.path.join( PATH_Ogre, 'Dependencies/include')
PATH_INCLUDE_OIS =          os.path.join(PATH_OIS,'includes')    ## Note the plural include's
PATH_INCLUDE_OgreRefApp =   os.path.join(PATH_Ogre,'ReferenceApplication/ReferenceAppLayer/include') 
PATH_INCLUDE_OgreNewt =     os.path.join(PATH_OgreAddons,'ogrenewt/OgreNewt_Main/inc')
PATH_INCLUDE_CEGUI =        os.path.join(PATH_CEGUI, 'include/CEGUI')
PATH_INCLUDE_ODE =          os.path.join( PATH_ODE, 'include')
PATH_INCLUDE_OPCODE =       PATH_OPCODE
PATH_INCLUDE_ODESOURCE =    os.path.join( PATH_ODE, 'ode/src')
PATH_INCLUDE_OgreAL =       os.path.join( PATH_OGREAL)#, 'include' )
PATH_INCLUDE_OPENAL =       os.path.join( PATH_OPENAL, 'include')
PATH_INCLUDE_OGG=           os.path.join( PATH_OGG, 'include' )
PATH_INCLUDE_VORBIS=        os.path.join( PATH_VORBIS, 'include' )
PATH_INCLUDE_ALUT=          os.path.join( PATH_ALUT, 'include' )

# # PATH_INCLUDE_OgreBullet =   [ 
# #                         os.path.join( PATH_OgreBullet,'Collisions','include')
# #                         ,os.path.join( PATH_OgreBullet,'Dynamics','include')
# #                         ]
##PATH_INCLUDE_OgreBullet =   PATH_OgreBullet
                        
PATH_INCLUDE_OgreOde =      os.path.join( PATH_OgreOde,'include')
PATH_INCLUDE_OgreOdePrefab= os.path.join( PATH_OgreOde,'prefab/include')
PATH_INCLUDE_OgreOdeLoader= os.path.join( PATH_OgreOde,'loader/include')
PATH_INCLUDE_FMOD =          os.path.join(PATH_FMOD, 'api/inc')
PATH_INCLUDE_Ogre_CEGUIRenderer = os.path.join( PATH_Ogre, 'samples/Common/CEGUIRenderer/include')
PATH_INCLUDE_quickgui =     PATH_quickgui
PATH_INCLUDE_NxOgre=        PATH_NxOgre
# PATH_INCLUDE_NxOgre=        os.path.join(PATH_NxOgre, 'include')
PATH_INCLUDE_Bullet=        os.path.join(PATH_Bullet, 'src')
PATH_INCLUDE_freetype=      os.path.join(PATH_INCLUDE_quickgui,'FreeType2.3.5')    
PATH_INCLUDE_betagui =      PATH_betagui
PATH_INCLUDE_Theora =       os.path.join (PATH_Theora,'include')
PATH_INCLUDE_ogrevideoffmpeg =  PATH_ogrevideoffmpeg
PATH_INCLUDE_ffmpeg =       os.path.join (PATH_ffmpeg,'include')
PATH_INCLUDE_ogredshow =    PATH_ogredshow
PATH_INCLUDE_plib =         PATH_plib
PATH_INCLUDE_navi =         os.path.join (PATH_navi,'include')
PATH_INCLUDE_noise = PATH_noise 
##PATH_INCLUDE_ogreforests =         os.path.join (PATH_ogreforests,'include')
PATH_INCLUDE_ogreforests =         PATH_ogreforests

PATH_INCLUDE_OggVorbisTheora = [ os.path.join(BASE_DIR,'ogg','include')
                        ,os.path.join(BASE_DIR, 'vorbis', 'include')
                        ,os.path.join(PATH_OgreAddons,'videoplugin','theora','include')
                        ,os.path.join(PATH_OgreAddons,'videoplugin','ptypes-1.8.3','include')
                        ]

PATH_INCLUDE_PhysX= [ os.path.join(PATH_PhysX, 'Physics','include')
                    ,os.path.join(PATH_PhysX, 'Cooking','include')
                    ,os.path.join(PATH_PhysX, 'Foundation','include')
                    ,os.path.join(PATH_PhysX, 'PhysXLoader','include')
                    ,os.path.join(PATH_PhysX, 'NxCharacter','include')
                    ,os.path.join(PATH_PhysX, 'NxExtensions','include')
                    ]

if SDK:
    PATH_INCLUDE_OIS =          os.path.join(PATH_Ogre,'Dependencies/include/OIS')    ## Note the plural include's
    PATH_INCLUDE_CEGUI =        os.path.join(PATH_Ogre, 'Dependencies/include/CEGUI')

    