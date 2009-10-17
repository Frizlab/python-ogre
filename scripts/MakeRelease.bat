rem echo off
echo
echo ====== Python-Ogre Windows Release Build ======
echo =
rmdir /s /q ..\..\PythonOgreRelease
mkdir ..\..\PythonOgreRelease
mkdir ..\..\PythonOgreRelease\packages_2.6
mkdir ..\..\PythonOgreRelease\plugins
rem mkdir ..\..\PythonOgreRelease\demos
mkdir ..\..\PythonOgreRelease\ThirdParty
mkdir ..\..\PythonOgreRelease\Tools
mkdir ..\..\PythonOgreRelease\docs
xcopy ..\packages_2.6 ..\..\PythonOgreRelease\packages_2.6 /s
rem xcopy ..\demos ..\..\PythonOgreRelease\demos /s
xcopy ..\plugins ..\..\PythonOgreRelease\plugins
xcopy ..\docs ..\..\PythonOgreRelease\docs
xcopy ..\tools ..\..\PythonOgreRelease\tools
copy ..\ThirdParty ..\..\PythonOgreRelease\ThirdParty
copy ..\*.rtf ..\..\PythonOgreRelease
copy ..\ReportVersion.py ..\..\PythonOgreRelease
copy ..\Setup.py ..\..\PythonOgreRelease
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




