@echo off
echo
echo ====== Python-Ogre Windows Release Build ======
echo =
rmdir /s /q ..\..\PythonOgreRelease
mkdir ..\..\PythonOgreRelease\packages_2.4
mkdir ..\..\PythonOgreRelease\packages_2.5
mkdir ..\..\PythonOgreRelease\plugins
mkdir ..\..\PythonOgreRelease\demos
mkdir ..\..\PythonOgreRelease\ThirdParty
mkdir ..\..\PythonOgreRelease\Tools
mkdir ..\..\PythonOgreRelease\docs
xcopy ..\packages_2.4 ..\..\PythonOgreRelease\packages_2.4 /s
xcopy ..\packages_2.5 ..\..\PythonOgreRelease\packages_2.5 /s
xcopy ..\demos ..\..\PythonOgreRelease\demos /s
xcopy ..\plugins ..\..\PythonOgreRelease\plugins
xcopy ..\docs ..\..\PythonOgreRelease\docs
xcopy ..\tools ..\..\PythonOgreRelease\tools
xcopy ..\ThirdParty ..\..\PythonOgreRelease\ThirdParty
copy ..\*.rtf ..\..\PythonOgreRelease
copy ..\COPYING ..\..\PythonOgreRelease
copy ..\*.gpl ..\..\PythonOgreRelease
copy ..\changelog.txt ..\..\PythonOgreRelease
del ..\..\PythonOgreRelease\*.pyc /s
del ..\..\PythonOgreRelease\*.log /s
del ..\..\PythonOgreRelease\1 /s
del ..\..\PythonOgreRelease\2 /s
del ..\..\PythonOgreRelease\ogre.cfg /s
rmdir /s /q ..\..\PythonOgreRelease\demos\ogre\tofix 




