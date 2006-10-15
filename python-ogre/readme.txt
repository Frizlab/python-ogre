##
## Python-Ogre Version 0.5
##
## This package contains python binding for the Ogre3D graphics library
## and was built using Py++ and Boost.
##
## Direct all comments etc to mailto://python-ogre-developers@googlegroups.com
##
## http://python-ogre.python-hosting.com/
##
## Python-Ogre SVN:  https://svn.python-ogre.python-hosting.com/trunk/python-ogre
## Py++ SVN:         https://svn.sourceforge.net/svnroot/pygccxml
##

Installing the prebuilt Windows Binary:
=======================================
From this directory run:
    python setup.py install
This will install the necessary module to your lib/site-packages directory

Run the demos:
==============
Ensure you have a path to the ogreSDK bin/release directory
    path=c:\OgreSDK\bin\release;%path%
Change directory to demos/ogre
Run demos with:
    python demofilename.py
Note: Included in the demo directory is the boot_python.dll. It may be senisble to copy this to the OgreSDK/bin/release 
    directory....

              
Prerequisits if buiilding from source:
======================================
Latest CVS version of Boost (www.boost.org) - I suggest you modify the ./boost/python/detail/preprocessor.hpp and change BOOST_PYTHON_MAX_ARITY to 19 (instead of the default 15).  Although this 'should' be done at the command line by making this change before building boost it's something else you don;t have to worry about
BJAM - this is needed to build boost so you should have it anyway
OgreCVS - latest code, or you can use the SDK 1.2.3 for MSVC 7 (www.ogre3d.org)
Python - tested with python version 2.5
MSVC 7 - :)
gccxml - CVS
Py++ - Always get the latest SVN version
-

Building:
=========
Edit the environment.py file to suit your environment - comment out the appropiate OGRE_VERSION line
Copy OgreSharedPtr.h from ./ogre_wrappers directory to the appropiate ogre include directory 
                    - note this is NOT used to build ogre, simply to ensure the wrappers are created correctly
Run the "BuildSource" batch file
Run the "Compile" batch file
You will now have an Ogre.pyd under the bin directory that should work :)
Check out the demos - ./demos/ogre for ogre 1.2.3 and ./demos/ogreCVS for the CVS version :)

You will also need to build a python wrapper for OIS if you are using the CVS version of Ogre
Follow the above directions (edit environment.py, Buildsource etc) from the PYOIS directory.