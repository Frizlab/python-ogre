echo off
echo =========================================
echo Python-Ogre batch file to generate HTML help 
echo edit "epydoc_config" if you want to change anything
echo NOTE: import ogre.renderer.OGRE must be added to \python25\scripts\epydoc for this to work
echo =========================================
del \temp\html\*
\python25\python \python25\scripts\epydoc --config epydoc_config
echo .
echo =========================================
echo now to build the compiled help
echo first we try with chmgenerator
python ..\..\chmgenerator\gen_epydoc.py c:\temp\html -o c:\temp\doc -n Python-Ogre -c c:\apps\hc\hcp.exe
echo .
echo Now it's likely that this fails, however it's created the Python-Ogre.hpp config file so you can
echo run the microsfot htmlhtml studio and compile the chm
rem
rem https://frogpie.svn.sourceforge.net/svnroot/frogpie/trunk/sandbox/chm (chm generator)
rem http://www.microsoft.com/downloads/details.aspx?FamilyID=00535334-c8a6-452f-9aa0-d597d16580cc&DisplayLang=en
rem https://svn.sourceforge.net/svnroot/epydoc/trunk/epydoc
rem Checkout a new gen_epydoc.py in the patch directory if you have issues

