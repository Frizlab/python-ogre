setlocal
pushd
set _SVN="c:\apps\svn-win32-1.4.5\bin\svn.exe"
set _CVS="c:\Program Files\cvsnt\cvs.exe"
set _VCBUILD="c:\Program Files\Microsoft Visual Studio 8\vc\vcpackages\vcbuild.exe"
set _ROOT=c:\development
set _TP=c:\development\python-ogre\ThirdParty

cd %_ROOT%\ogrenew
%_CVS% up
cd %_ROOT%\ogreaddons\ogreode
%_CVS% up

cd %_ROOT%\NxOgre
%_SVN% up
del /q %_TP%\nxogre
xcopy /s /y include %_TP%\nxogre
xcopy /s /y source %_TP%\nxogre

cd %_ROOT%\ogreal
%_SVN% up
del /q %_TP%\ogreal
copy include\*.h %_TP%\ogreal
copy src\*.cpp %_TP%\ogreal

cd %_ROOT%\QuickGui
%_SVN% up
del /q %_TP%\quickgui
copy QuickGui\include\*.h %_TP%\quickgui
copy QuickGui\src\*.cpp %_TP%\quickgui

cd %_ROOT%\caelum
%_SVN% up
del /q %_TP%\caelum
copy trunk\Caelum\main\include\*.h %_TP%\caelum
copy trunk\Caelum\main\src\*.cpp %_TP%\caelum

cd %_ROOT%\ogreaddons\ogrebullet
%_CVS% up
del /q /s %_TP%\ogrebullet\Collisions
del /q /s %_TP%\ogrebullet\Dynamics
xcopy /s Collisions\include %_TP%\ogrebullet\Collisions
xcopy /s Collisions\src %_TP%\ogrebullet\Collisions
xcopy /s Dynamics\include %_TP%\ogrebullet\Dynamics
xcopy /s Dynamics\src %_TP%\ogrebullet\Dynamics

cd %_ROOT%\ogreaddons\forests
%_CVS% up
del /q %_TP%\forests
copy include\*.h %_TP%\forests
copy source\*.cpp %_TP%\forests

cd %_ROOT%\opensteer
%_SVN% up
cd trunk
xcopy /s include\* %_TP%\opensteer 
copy src\*.cpp %_TP%\opensteer
copy include\OpenSteer\*.h %_TP%\opensteer


popd
endlocal
