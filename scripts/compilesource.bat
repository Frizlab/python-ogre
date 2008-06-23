setlocal
pushd
set _VCBUILD="c:\\Program Files\\Microsoft Visual Studio 9.0\\vc\\vcpackages\\vcbuild.exe"
set _ROOT=c:\\development
call vcvars32
cd %_ROOT%\\ogre
%_VCBUILD% /useenv Ogre_vc8.sln "Release|Win32"
rem cd %_ROOT%\\NxOgre\\NxOgre
rem %_VCBUILD% /useenv NxOgre.VC8.Windows.sln "Release|Win32"
cd %_ROOT%\\ogreaddons\\ogreode\\scripts\\vc8
%_VCBUILD% /useenv OgreOde_CVS.sln "Release|Win32"
cd %_ROOT%\\ogreaddons\\ogrenewt
%_VCBUILD% /useenv OgreNewt.sln "Release|Win32"
cd %_ROOT%\\navi
%_VCBUILD% /useenv navi.sln "Release|Win32"
cd %_ROOT%\\ogreaddons\\videoplugin\theoravideo\scripts
%_VCBUILD% /useenv VC8_TheoraVideo.sln "Release|Win32"
cd %_ROOT%\\OgreNew
%_VCBUILD% /useenv ParticleUniverse.sln "Release|Win32"

popd
endlocal
