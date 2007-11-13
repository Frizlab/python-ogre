rem echo off
echo
echo ====== Python-Ogre Windows Release Build ======
echo =
rmdir /s /q ..\..\PythonOgreRelease
rem mkdir ..\..\PythonOgreRelease\packages_2.4
mkdir ..\..\PythonOgreRelease\packages_2.5
mkdir ..\..\PythonOgreRelease\plugins
mkdir ..\..\PythonOgreRelease\demos
mkdir ..\..\PythonOgreRelease\ThirdParty
mkdir ..\..\PythonOgreRelease\Tools
mkdir ..\..\PythonOgreRelease\docs
rem xcopy ..\packages_2.4 ..\..\PythonOgreRelease\packages_2.4 /s
xcopy ..\packages_2.5 ..\..\PythonOgreRelease\packages_2.5 /s
xcopy ..\demos ..\..\PythonOgreRelease\demos /s
xcopy ..\plugins ..\..\PythonOgreRelease\plugins
xcopy ..\docs ..\..\PythonOgreRelease\docs
xcopy ..\tools ..\..\PythonOgreRelease\tools
copy ..\ThirdParty ..\..\PythonOgreRelease\ThirdParty
copy ..\*.rtf ..\..\PythonOgreRelease
copy ..\COPYING ..\..\PythonOgreRelease
copy ..\*.gpl ..\..\PythonOgreRelease
copy ..\changelog.txt ..\..\PythonOgreRelease
del ..\..\PythonOgreRelease\*.pyc /s
del ..\..\PythonOgreRelease\*.log /s
del ..\..\PythonOgreRelease\1 /s
del ..\..\PythonOgreRelease\2 /s
del ..\..\PythonOgreRelease\ogre.cfg /s
for /r ..\..\PythonOgreRelease\ %%1 in (*) do touch %%1
rem rmdir /s /q ..\..\PythonOgreRelease\demos\gui\tofix 
rem rmdir /s /q ..\..\PythonOgreRelease\demos\ogre\tofix 




