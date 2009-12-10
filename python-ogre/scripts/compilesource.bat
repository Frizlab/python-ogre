setlocal
pushd
set _VCBUILD="c:\\Program Files\\Microsoft Visual Studio 9.0\\vc\\vcpackages\\vcbuild.exe"
set _ROOT=c:\\development
call "c:\\Program Files\\Microsoft Visual Studio 9.0\\vc\\bin\vcvars32.bat"
cd %_ROOT%\\ogre_svn
%_VCBUILD% /useenv Ogre.sln "Release|Win32"

cd %_ROOT%\\NxOgre\\build\\msvc
%_VCBUILD% /useenv NxOgre.VC9.sln "Release|Win32"

cd %_ROOT%\\ogreaddons\\ogreode\\scripts\\vc8
%_VCBUILD% /useenv OgreOde_SDK.sln "Release|Win32"

cd %_ROOT%\\ogreaddons\\ogrenewt
%_VCBUILD% /useenv OgreNewt_vc71.sln "Release|Win32"

cd %_ROOT%\\ogreaddons\\ogrenewt2
%_VCBUILD% /useenv OgreNewt_vc2008.sln "Release|Win32"

cd %_ROOT%\\MyGUI2.2.3_source
%_VCBUILD% /useenv MyGUI_v8.sln "Release|Win32"

cd %_ROOT%\\Ogreoggsound
%_VCBUILD% /useenv ogreoggsoundlib_vc9.sln "Release|Win32"

cd %_ROOT%\\SkyX\\SkyX
%_VCBUILD% /useenv SkyX.sln "Release|Win32"

rem cd %_ROOT%\\Ogre
rem %_VCBUILD% /useenv ParticleUniverse.sln "Release|Win32"


popd
endlocal
