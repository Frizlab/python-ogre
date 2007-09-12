rem echo off
echo
echo ====== Python-Ogre Snap Shot ======
echo =
rmdir /s /q ..\..\PythonOgreSnapShot
mkdir ..\..\PythonOgreSnapShot\packages_2.5
mkdir ..\..\PythonOgreSnapShot\plugins
mkdir ..\..\PythonOgreSnapShot\demos
mkdir ..\..\PythonOgreSnapShot\ThirdParty
mkdir ..\..\PythonOgreSnapShot\Tools
mkdir ..\..\PythonOgreSnapShot\docs
xcopy ..\packages_2.5 ..\..\PythonOgreSnapShot\packages_2.5 /s
xcopy ..\plugins ..\..\PythonOgreSnapShot\plugins
copy ..\*.rtf ..\..\PythonOgreSnapShot
copy ..\COPYING ..\..\PythonOgreSnapShot
copy ..\*.gpl ..\..\PythonOgreSnapShot
copy ..\setup.py ..\..\PythonOgreSnapShot

copy ..\changelog.txt ..\..\PythonOgreSnapShot
del ..\..\PythonOgreSnapShot\*.pyc /s
del ..\..\PythonOgreSnapShot\*.log /s
del ..\..\PythonOgreSnapShot\1 /s
del ..\..\PythonOgreSnapShot\2 /s
del ..\..\PythonOgreSnapShot\ogre.cfg /s
for /r ..\..\PythonOgreSnapShot\ %1 in (*) do touch %1
rem rmdir /s /q ..\..\PythonOgreSnapShot\demos\gui\tofix 
rem rmdir /s /q ..\..\PythonOgreSnapShot\demos\ogre\tofix 




