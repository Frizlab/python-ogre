rem echo off
echo
echo ====== Update DLL's ======
echo 
setlocal
set root=c:\development
set package=..\packages_2.5\ogre
set ogre=%root%\ogrenew\samples\common\bin\release
set cegui=%root%\cegui-0.5.0\bin
set quick=%root%\quickgui\bin
set opcode=%root%\OpCode\Release
set nxogre=%root%\NxOgre\NxOgre\lib
set newton=%root%\NewtonSDK\sdk\dll
set ogreode=%root%\ogreaddons\ogreode
set physx="c:\program files\AGEIA Technologies\SDK\v2.7.2\bin\win32"
set theora=%root%\ogreaddons\videoplugin\TheoraVideo\bin\Release
set ffmpeg=%root%\ffmpeg
set navi=%root%\navi\Dependencies\win32\llmozlib\runtime\Release

set ogreal=%root%\OgreAL\lib\Release
set freealut=%root%\FreeAlut\lib
set vorbis=%root%\vorbis\win32
set ogg=%root%\ogg\win32\Dynamic_Release
set openal=%root%\openal\samples\bin\win32

rem Ogre
copy /y %ogre%\ogremain.dll %package%\renderer\OGRE
copy /y %ogre%\render*.dll ..\plugins
copy /y %ogre%\plug*.dll ..\plugins

rem Physics
copy /y %ogre%\ReferenceAppLayer.dll %package%\physics\OgreRefApp
copy /y %opcode%\opcode.dll %package%\physics\OpCode
copy /y %nxogre%\nxogre.dll %package%\physics\NxOgre
copy /y %newton%\newton.dll %package%\physics\OgreNewt
rem OgreODE
copy /y %ogreode%\lib\Release\OgreOde_Core.dll %package%\physics\OgreOde
copy /y %ogreode%\loader\lib\Release\OgreOde_Loader.dll %package%\physics\OgreOde
copy /y %ogreode%\prefab\lib\Release\OgreOde_Prefab.dll %package%\physics\OgreOde
rem Physx
copy /y %physx%\Nx*.dll %package%\physics\PhysX
copy /y %physx%\Ph*.dll %package%\physics\PhysX
rem Theora
copy /y %theora%\Plugin_TheoraVideoSystem.dll ..\plugin
rem FFMpeg
copy /y %ffmpeg%\*.dll %package%\addons\ogrevideoffmpeg

rem  CEGUI
copy /y %cegui%\CEGUIBase.dll %package%\gui\CEGUI
copy /y %cegui%\CEGUIExpatParser.dll %package%\gui\CEGUI
copy /y %cegui%\CEGUIFalagardWRBase.dll %package%\gui\CEGUI
copy /y %cegui%\CEGUITinyXMLParser.dll %package%\gui\CEGUI
copy /y %ogre%\OgreGUIRenderer.dll %package%\gui\CEGUI

rem QuickGUI
copy /y %quick%\QuickGUI.dll %package%\gui\QuickGui

rem OgreAL
copy /y %ogreal%\OgreAL.dll %package%\sound\OgreAL
copy /y %vorbis%\VorbisEnc_Dynamic_Release\vorbisenc.dll %package%\sound\OgreAL
copy /y %vorbis%\VorbisFile_Dynamic_Release\vorbisfile.dll %package%\sound\OgreAL
copy /y %vorbis%\Vorbis_Dynamic_Release\vorbis.dll %package%\sound\OgreAL
copy /y %ogg%\ogg.dll %package%\sound\OgreAL
copy /y %freealut%\alut.dll %package%\sound\OgreAL
copy /y %openal%\OpenAL32.dll %package%\sound\OgreAL
copy /y %openal%\wrap_oal.dll %package%\sound\OgreAL

rem navi
copy /y %navi%\*.dll %package%\gui\navi

