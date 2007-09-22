import sys,os

## Boost stuff 
## path to the root of your boost dir, should have 'libs', 'boost' subdirectories
module_dir = os.path.abspath(os.path.dirname(__file__) )## The root directory is where this module is located
## lets assume that the base development directory is one level higher
BASE_DIR,ignore = os.path.split(module_dir) ##  r'/home/andy/development'

ROOT_DIR = os.path.join(BASE_DIR,'root')
root_dir = os.path.abspath(os.path.dirname(__file__) )## The root directory is where this module is located

LOCAL_LIB = os.path.join(ROOT_DIR,'usr/lib')
LOCAL_INCLUDE = os.path.join(ROOT_DIR, 'usr/include')

PATH_Boost = os.path.join(LOCAL_INCLUDE, 'boost-1_34')

#
PATH_LIB_Boost = LOCAL_LIB
## and the name of the boost python library
LIB_Boost = 'libboost_python-gcc41-1_34'

#
# Don't forget we need to call gccxml......
gccxml_bin = os.path.join(BASE_DIR,'gccxml-build/bin')
# and the Py++ directory as sometimes we need access to the code repository there
pyplusplus_install_dir = os.path.join(BASE_DIR,'pygccxml')


# # ## Parent directories of the libraries
PATH_THIRDPARTY =   os.path.join(module_dir, 'ThirdParty' )
PATH_Ogre =         os.path.join(BASE_DIR,'ogrenew')
PATH_OgreAddons =   os.path.join(BASE_DIR, 'OgreAddons')
PATH_CEGUI =        os.path.join(BASE_DIR, 'CEGUI-0.5.0')
PATH_OIS =          os.path.join(BASE_DIR, 'ois-1.0RC1')
PATH_Newton =       os.path.join(BASE_DIR, 'newtonSDK','sdk')
PATH_ODE =          os.path.join(BASE_DIR, 'ode-0.8')
PATH_OGG =          os.path.join(BASE_DIR, 'ogg')
PATH_VORBIS=        os.path.join(BASE_DIR, 'vorbis')
PATH_OPENAL=        os.path.join(BASE_DIR, 'openal')
PATH_ALUT=          os.path.join(BASE_DIR, 'freealut')
PATH_OgreOde=       os.path.join(BASE_DIR, 'OgreAddons','ogreode')
PATH_OGREAL=        os.path.join(PATH_THIRDPARTY,,'ogreal')
PATH_OPCODE=        os.path.join(PATH_THIRDPARTY,'opcode')
PATH_quickgui=      os.path.join(PATH_THIRDPARTY,'quickgui')
PATH_betagui=       os.path.join(PATH_THIRDPARTY,'betagui')
PATH_ogredshow =    os.path.join(PATH_THIRDPARTY,'dshow')
PATH_plib =         os.path.join(PATH_THIRDPARTY, 'plib')
PATH_ogrevideoffmpeg = os.path.join(PATH_THIRDPARTY,'ffmpeg')
PATH_NxOgre=        os.path.join(BASE_DIR, 'nxogre/NxOgre')
PATH_Bullet=        os.path.join(BASE_DIR, 'bullet-2.52') 
PATH_PhysX=         os.path.join(BASE_DIR, 'AGEIA Technologies/SDK/v2.7.2/SDKs')
PATH_Theora=        os.path.join(PATH_OgreAddons,'videoplugin','TheoraVideo')
PATH_ffmpeg=        os.path.join(BASE_DIR, 'ffmpeg')
PATH_navi =         os.path.join(BASE_DIR, 'navi','Navi')


###
### these paths assume you've left all the directory structure as standard
### Override anything that is different
###        
PATH_LIB_Ogre_CEGUIRenderer =    os.path.join( LOCAL_LIB ) #, 'Samples/Common/CEGUIRenderer/lib')
PATH_LIB_Ogre_OgreMain=         os.path.join( LOCAL_LIB ) # PATH_Ogre, 'OgreMain/lib/Release' )
PATH_LIB_Ogre_Dependencies =    os.path.join( LOCAL_LIB ) # , 'Dependencies/lib/Release')
PATH_LIB_OgreRefApp =           os.path.join( LOCAL_LIB ) # PATH_Ogre, 'ReferenceApplication/ReferenceAppLayer/lib/Release')
PATH_LIB_OgreNewt =             os.path.join( LOCAL_LIB ) #PATH_OgreAddons, r'ogrenewt/OgreNewt_Main/lib/Release') 
PATH_LIB_Newton =               os.path.join( LOCAL_LIB ) # PATH_Newton ,'dll')  ##NOTE Posix platforms this lives in 'lib-mt'
PATH_LIB_OIS =                  os.path.join( LOCAL_LIB ) #PATH_OIS, 'dll') ## NOTE Posix platforms this lives in'lib' not 'dll'
PATH_LIB_CEGUI =                os.path.join( LOCAL_LIB ) #PATH_Ogre, r'Dependencies/lib/Release' )
PATH_LIB_ODE =                  os.path.join( LOCAL_LIB ) #PATH_ODE, 'lib/releasedll')## probable releaselib for posix
PATH_LIB_OPCODE =               os.path.join( PATH_OPCODE ) 
PATH_LIB_OgreOde =              os.path.join( LOCAL_LIB ) #PATH_OgreOde, 'lib/Release') 
PATH_LIB_OgreOdePrefab =        os.path.join( LOCAL_LIB ) #PATH_OgreOde, 'prefab/lib/Release' )
PATH_LIB_OgreOdeLoader =        os.path.join( LOCAL_LIB ) #PATH_OgreOde, 'loader/lib/Release' )
PATH_LIB_OgreAL =               os.path.join( LOCAL_LIB ) #PATH_OGREAL, 'lib/Release' )
PATH_LIB_betagui =              PATH_betagui
PATH_LIB_quickgui =             PATH_quickgui
PATH_LIB_Bullet = 		        os.path.join( LOCAL_LIB )
PATH_LIB_plib =                 PATH_plib
PATH_LIB_navi=                  os.path.join(PATH_navi, 'lib')
PATH_LIB_NxOgre=                os.path.join(LOCAL_LIB )
PATH_LIB_PhysX =                os.path.join(LOCAL_LIB)
PATH_LIB_Theora=                os.path.join(PATH_Theora, 'bin', 'Release')
PATH_LIB_ogrevideoffmpeg =      PATH_ogrevideoffmpeg
PATH_LIB_ffmpeg=                os.path.join(PATH_ffmpeg, 'lib')


PATH_INCLUDE_Ogre=          os.path.join(LOCAL_INCLUDE,'OGRE')  # os.path.join(PATH_Ogre,'OgreMain/include') 
PATH_INCLUDE_Ogre_Dependencies =    PATH_INCLUDE_Ogre #      os.path.join( PATH_Ogre, 'Dependencies/include')
PATH_INCLUDE_OIS =          os.path.join(LOCAL_INCLUDE,'OIS') #os.path.join(PATH_OIS,'includes')    ## Note the plural include's
PATH_INCLUDE_OgreRefApp =   os.path.join(PATH_Ogre,'ReferenceApplication/ReferenceAppLayer/include') 
PATH_INCLUDE_OgreNewt =     os.path.join(LOCAL_INCLUDE,'OgreNewt') #os.path.join(PATH_OgreAddons,'ogrenewt/OgreNewt_Main/inc')
PATH_INCLUDE_CEGUI =        os.path.join(LOCAL_INCLUDE,'CEGUI') #os.path.join(PATH_CEGUI, r'include/CEGUI')
PATH_INCLUDE_ODE =          os.path.join(LOCAL_INCLUDE,'ode') #os.path.join( PATH_ODE, 'include')
PATH_INCLUDE_ODESOURCE =    '' #      os.path.join( PATH_ODE, 'ode/src')
PATH_INCLUDE_quickgui =     PATH_quickgui
PATH_INCLUDE_OgreAL =       PATH_OGREAL
PATH_INCLUDE_OPENAL =       os.path.join( PATH_OPENAL, 'include')
PATH_INCLUDE_OGG=           os.path.join( PATH_OGG, 'include' )
PATH_INCLUDE_VORBIS=        os.path.join( PATH_VORBIS, 'include' )
PATH_INCLUDE_ALUT=          os.path.join( PATH_ALUT, 'include' )
PATH_INCLUDE_OgreOde =      os.path.join( PATH_OgreOde,'include')
PATH_INCLUDE_OgreOdePrefab= os.path.join( PATH_OgreOde,'prefab/include')
PATH_INCLUDE_OgreOdeLoader= os.path.join( PATH_OgreOde,'loader/include')
PATH_INCLUDE_NxOgre=        os.path.join(PATH_NxOgre, 'include')
PATH_INCLUDE_betagui =       PATH_betagui
PATH_INCLUDE_Ogre_CEGUIRenderer = os.path.join( PATH_Ogre, 'Samples/Common/CEGUIRenderer/include')PATH_INCLUDE_OPCODE = 		os.path.join( LOCAL_INCLUDE )
PATH_INCLUDE_Bullet = 		os.path.join( LOCAL_INCLUDE )
PATH_INCLUDE_navi =         os.path.join (PATH_navi,'include')
PATH_INCLUDE_plib =         PATH_plib
PATH_INCLUDE_Theora =       os.path.join (PATH_Theora,'include')
PATH_INCLUDE_ogrevideoffmpeg =  PATH_ogrevideoffmpeg
PATH_INCLUDE_ffmpeg =       os.path.join (PATH_ffmpeg,'include')
PATH_INCLUDE_plib =         PATH_plib
PATH_INCLUDE_navi =     os.path.join (PATH_navi,'include')

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
