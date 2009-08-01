rem echo off
echo
echo ====== Update DLL's ======
echo 
setlocal
set root=c:\development

rem Support for 2 versions of Python...
set package_2.5=..\packages_2.5\ogre
set package_2.6=..\packages_2.6\ogre


set ogre=%root%\ogre\samples\common\bin\release
set ogre=%root%\ogre\bin\release

set ogretools=%root%\ogre\tools\common\bin\release
set ogretools=%root%\ogre\bin\release

set cegui=%root%\cegui-0.6.2\bin

set quick=%root%\quickgui\bin
set opcode=%root%\OpCode\Release
set nxogre=%root%\NxOgre\NxOgre\lib
set newton=%root%\NewtonSDK\sdk\dll
set ogreode=%root%\ogreaddons\ogreode
set physx="c:\program files\NVIDIA Corporation\NVIDIA PhysX SDK\v2.8.1\bin\win32"
set theora=%root%\ogrevideo\TheoraVideoPlugin\bin\Release
set ffmpeg=%root%\python-ogre\thirdparty\extra
set naviextra=%root%\navi\Dependencies\win32\awesomium\bin\release
set navi=%root%\navi\navi\lib

set boost_2.5=%root%\boost_1_38_0\bin.v2\libs\python_2.5\build\msvc-9.0\release\threading-multi
set boost_2.6=%root%\boost_1_38_0\bin.v2\libs\python_2.6\build\msvc-9.0\release\threading-multi
set boostlib_2.5=%boost_2.5%\boost_python-vc90-mt-1_38.dll
set boostlib_2.6=%boost_2.6%\boost_python-vc90-mt-1_38.dll

set particleuniverse=%root%\ogre\lib

set ogreal=%root%\OgreAL\lib\Release
set freealut=%root%\FreeAlut\lib
set vorbis=%root%\vorbis\win32
set ogg=%root%\ogg\win32\Dynamic_Release

set openal=%root%\openal\redist
set openal=%root%\openal.1.1\redist


rem Ogre
copy /y %ogre%\ogremain.dll %package_2.5%\renderer\OGRE
copy /y %ogre%\cg.dll %package_2.5%\renderer\OGRE
copy /y %ogre%\ogremain.dll %package_2.6%\renderer\OGRE
copy /y %ogre%\cg.dll %package_2.6%\renderer\OGRE

copy /y %ogretools%\* ..\tools

copy /y %boostlib_2.5% %package_2.5%\renderer\OGRE
copy /y %boostlib_2.6% %package_2.6%\renderer\OGRE

copy /y %ogre%\render*.dll ..\plugins
copy /y %ogre%\plug*.dll ..\plugins

rem OIS
rem copy /y %ogre%\ois.dll %package%\io\OIS

rem ParticleUniverse
copy /y %particleuniverse%\ParticleUniverse.dll ..\plugins

rem Physics
copy /y %ogre%\ReferenceAppLayer.dll %package_2.5%\physics\OgreRefApp
copy /y %ogre%\ReferenceAppLayer.dll %package_2.6%\physics\OgreRefApp

REM copy /y %opcode%\opcode.dll %package%\physics\OpCode
copy /y %nxogre%\nxogre.dll %package_2.5%\physics\NxOgre
copy /y %newton%\newton.dll %package_2.5%\physics\OgreNewt
copy /y %boostlib_2.5% %package_2.5%\physics\ode
copy /y %boostlib_2.5% %package_2.5%\physics\bullet
copy /y %boostlib_2.5% %package_2.5%\physics\physx

copy /y %nxogre%\nxogre.dll %package_2.6%\physics\NxOgre
copy /y %newton%\newton.dll %package_2.6%\physics\OgreNewt
copy /y %boostlib_2.6% %package_2.6%\physics\ode
copy /y %boostlib_2.6% %package_2.6%\physics\bullet
copy /y %boostlib_2.6% %package_2.6%\physics\physx

rem OgreODE
copy /y %ogreode%\lib\Release\OgreOde_Core.dll %package_2.5%\physics\OgreOde
copy /y %ogreode%\loader\lib\Release\OgreOde_Loader.dll %package_2.5%\physics\OgreOde
copy /y %ogreode%\prefab\lib\Release\OgreOde_Prefab.dll %package_2.5%\physics\OgreOde

copy /y %ogreode%\lib\Release\OgreOde_Core.dll %package_2.6%\physics\OgreOde
copy /y %ogreode%\loader\lib\Release\OgreOde_Loader.dll %package_2.6%\physics\OgreOde
copy /y %ogreode%\prefab\lib\Release\OgreOde_Prefab.dll %package_2.6%\physics\OgreOde

rem Physx
copy /y %physx%\Nx*.dll %package_2.5%\physics\PhysX
copy /y %physx%\Ph*.dll %package_2.5%\physics\PhysX
copy /y %physx%\Nx*.dll %package_2.5%\physics\NxOgre_09

copy /y %physx%\Nx*.dll %package_2.6%\physics\PhysX
copy /y %physx%\Ph*.dll %package_2.6%\physics\PhysX
copy /y %physx%\Nx*.dll %package_2.6%\physics\NxOgre_09

rem Theora/OgreVideo
copy /y %theora%\Plugin_TheoraVideoSystem.dll ..\plugins


REM copy /y %theora%\Plugin_TheoraVideoSystem.dll %package%\addons\theora

rem FFMpeg
copy /y %ffmpeg%\bin\*.dll %package_2.5%\addons\ogrevideoffmpeg
copy /y %ffmpeg%\bin\*.dll %package_2.6%\addons\ogrevideoffmpeg

rem  CEGUI
copy /y %cegui%\CEGUIBase.dll %package_2.5%\gui\CEGUI
copy /y %cegui%\CEGUIExpatParser.dll %package_2.5%\gui\CEGUI
copy /y %cegui%\CEGUIFalagardWRBase.dll %package_2.5%\gui\CEGUI
copy /y %cegui%\CEGUITinyXMLParser.dll %package_2.5%\gui\CEGUI
copy /y %ogre%\OgreGUIRenderer.dll %package_2.5%\gui\CEGUI

copy /y %cegui%\CEGUIBase.dll %package_2.6%\gui\CEGUI
copy /y %cegui%\CEGUIExpatParser.dll %package_2.6%\gui\CEGUI
copy /y %cegui%\CEGUIFalagardWRBase.dll %package_2.6%\gui\CEGUI
copy /y %cegui%\CEGUITinyXMLParser.dll %package_2.6%\gui\CEGUI
copy /y %ogre%\OgreGUIRenderer.dll %package_2.6%\gui\CEGUI


rem QuickGUI
REM copy /y %quick%\QuickGUI.dll %package%\gui\QuickGui

rem OgreAL
copy /y %ogreal%\OgreAL.dll %package_2.5%\sound\OgreAL
copy /y %vorbis%\VorbisEnc_Dynamic_Release\vorbisenc.dll %package_2.5%\sound\OgreAL
copy /y %vorbis%\VorbisFile_Dynamic_Release\vorbisfile.dll %package_2.5%\sound\OgreAL
copy /y %vorbis%\Vorbis_Dynamic_Release\vorbis.dll %package_2.5%\sound\OgreAL
copy /y %ogg%\ogg.dll %package_2.5%\sound\OgreAL

copy /y %ogreal%\OgreAL.dll %package_2.6%\sound\OgreAL
copy /y %vorbis%\VorbisEnc_Dynamic_Release\vorbisenc.dll %package_2.6%\sound\OgreAL
copy /y %vorbis%\VorbisFile_Dynamic_Release\vorbisfile.dll %package_2.6%\sound\OgreAL
copy /y %vorbis%\Vorbis_Dynamic_Release\vorbis.dll %package_2.6%\sound\OgreAL
copy /y %ogg%\ogg.dll %package_2.6%\sound\OgreAL


REM copy /y %freealut%\alut.dll %package%\sound\OgreAL
copy /y %openal%\OpenAL32.dll %package_2.5%\sound\OgreAL
copy /y %openal%\wrap_oal.dll %package_2.5%\sound\OgreAL
copy /y %openal%\OpenAL32.dll %package_2.6%\sound\OgreAL
copy /y %openal%\wrap_oal.dll %package_2.6%\sound\OgreAL
copy /y %openal%\OpenAL32.dll %package_2.5%\addons\ogrevideo
copy /y %openal%\wrap_oal.dll %package_2.5%\addons\ogrevideo
copy /y %openal%\OpenAL32.dll %package_2.6%\addons\ogrevideo
copy /y %openal%\wrap_oal.dll %package_2.6%\addons\ogrevideo

rem navi
copy /y %navi%\*.dll %package_2.5%\gui\navi
copy /y %naviextra%\*.dll %package_2.5%\gui\navi
copy /y %navi%\*.dll %package_2.6%\gui\navi
copy /y %naviextra%\*.dll %package_2.6%\gui\navi


rem plib
copy /y %boostlib_2.5% %package_2.5%\addons\plib
copy /y %boostlib_2.6% %package_2.6%\addons\plib
