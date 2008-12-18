rem echo off
echo
echo ====== Update DLL's ======
echo 
setlocal
set root=c:\development
set package=..\packages_2.5\ogre
set ogre=%root%\ogre\samples\common\bin\release
set ogretools=%root%\ogre\tools\common\bin\release
set cegui=%root%\cegui-0.6.2\bin
set quick=%root%\quickgui\bin
set opcode=%root%\OpCode\Release
set nxogre=%root%\NxOgre\NxOgre\lib
set newton=%root%\NewtonSDK\sdk\dll
set ogreode=%root%\ogreaddons\ogreode
set physx="c:\program files\NVIDIA Corporation\NVIDIA PhysX SDK\v2.8.1\bin\win32"
set theora=%root%\ogreaddons\videoplugin\TheoraVideo\bin\Release
set ffmpeg=%root%\ffmpeg
set naviextra=%root%\navi\Dependencies\win32\astral\runtime\Release
set navi=%root%\navi\lib
set boost=%root%\boost_1_37_0\bin.v2\libs\python\build\msvc-9.0\release\threading-multi
set boostlib=%boost%\boost_python-vc90-mt-1_37.dll
set particleuniverse=%root%\ogre\lib

set ogreal=%root%\OgreAL\lib\Release
set freealut=%root%\FreeAlut\lib
set vorbis=%root%\vorbis\win32
set ogg=%root%\ogg\win32\Dynamic_Release
set openal=%root%\openal\samples\bin\win32

rem Ogre
copy /y %ogre%\ogremain.dll %package%\renderer\OGRE
copy /y %ogre%\cg.dll %package%\renderer\OGRE

copy /y %ogretools%\* ..\tools

copy /y %boostlib% %package%\renderer\OGRE
copy /y %ogre%\render*.dll ..\plugins
copy /y %ogre%\plug*.dll ..\plugins

rem ParticleUniverse
copy /y %particleuniverse%\ParticleUniverse.dll ..\plugins

rem Physics
copy /y %ogre%\ReferenceAppLayer.dll %package%\physics\OgreRefApp
REM copy /y %opcode%\opcode.dll %package%\physics\OpCode
copy /y %nxogre%\nxogre.dll %package%\physics\NxOgre
copy /y %newton%\newton.dll %package%\physics\OgreNewt
copy /y %boostlib% %package%\physics\ode
copy /y %boostlib% %package%\physics\bullet
copy /y %boostlib% %package%\physics\physx


rem OgreODE
copy /y %ogreode%\lib\Release\OgreOde_Core.dll %package%\physics\OgreOde
copy /y %ogreode%\loader\lib\Release\OgreOde_Loader.dll %package%\physics\OgreOde
copy /y %ogreode%\prefab\lib\Release\OgreOde_Prefab.dll %package%\physics\OgreOde
rem Physx
copy /y %physx%\Nx*.dll %package%\physics\PhysX
copy /y %physx%\Ph*.dll %package%\physics\PhysX
copy /y %physx%\Nx*.dll %package%\physics\NxOgre_09
rem Theora
copy /y %theora%\Plugin_TheoraVideoSystem.dll ..\plugins
REM copy /y %theora%\Plugin_TheoraVideoSystem.dll %package%\addons\theora

rem FFMpeg
copy /y %ffmpeg%\lib\*.dll %package%\addons\ogrevideoffmpeg

rem  CEGUI
copy /y %cegui%\CEGUIBase.dll %package%\gui\CEGUI
copy /y %cegui%\CEGUIExpatParser.dll %package%\gui\CEGUI
copy /y %cegui%\CEGUIFalagardWRBase.dll %package%\gui\CEGUI
copy /y %cegui%\CEGUITinyXMLParser.dll %package%\gui\CEGUI
copy /y %ogre%\OgreGUIRenderer.dll %package%\gui\CEGUI

rem QuickGUI
REM copy /y %quick%\QuickGUI.dll %package%\gui\QuickGui

rem OgreAL
copy /y %ogreal%\OgreAL.dll %package%\sound\OgreAL
copy /y %vorbis%\VorbisEnc_Dynamic_Release\vorbisenc.dll %package%\sound\OgreAL
copy /y %vorbis%\VorbisFile_Dynamic_Release\vorbisfile.dll %package%\sound\OgreAL
copy /y %vorbis%\Vorbis_Dynamic_Release\vorbis.dll %package%\sound\OgreAL
copy /y %ogg%\ogg.dll %package%\sound\OgreAL
REM copy /y %freealut%\alut.dll %package%\sound\OgreAL
copy /y %openal%\OpenAL32.dll %package%\sound\OgreAL
copy /y %openal%\wrap_oal.dll %package%\sound\OgreAL

rem navi
copy /y %navi%\*.dll %package%\gui\navi

rem plib
copy /y %boostlib% %package%\addons\plib