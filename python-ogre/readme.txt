
Notes on building the python-ogre code for Windows

13 Aug 2006:  Initial Version from Andy
13 Aug 2006:  Roman - fix to Ogre_Generate_code.py to simplify build process (removed 'non exposed'? ENUMS)
                    * Needed to remove cmdXXXX functions etc from ParticleSystem in order to compile correctly
              Change to Ogre_Customization_data.py to add 'NamedValuePairList' mapping
15 Aug 2006:  Andy - addition to Ogre_Generate_code.py to expose additional template classes (initially Controller<float>)
              Fixed ParticleSystem class to remove CmdXXXX functions
              Update to Ogre_Customization_data.py as some mappings had been lost :)
29 Sept 2006: Andy - Update to support both the Ogre CVS and Ogre 1.2.3 code bases.  
              PYPOS project added as a sub driectory (initially)
              
Prerequisits:
=============
Latest CVS version of Boost (www.boost.org) - I suggest you modify the ./boost/python/detail/preprocessor.hpp and change BOOST_PYTHON_MAX_ARITY to 19 (instead of the default 15).  Although this 'should' be done at the command line by making this change before building boost it's something else you don;t have to worry about
BJAM - this is needed to build boost so you should have it anyway
OgreCVS - latest code, or you can use the SDK 1.2.3 for MSVC 7 (www.ogre3d.org)
Python - tested with python version 2.5b3 (earlier betas have XMLbug)  (www.python.org)
MSVC 7 - :)
gccxml - CVS
PYPLUS - Always get the latest SVN version


Building:
=========
Edit the environment.py file to suit your environment - comment out the appropiate OGRE_VERSION line
Edit python_ogre.h to define (or not) the OGRE_VERSION_CVS
Copy OgreSharedPtr.h from ./ogre_wrappers directory to the appropiate ogre include directory - note this is NOT used to build ogre, simply to ensure the wrappers are created correctly
Run the "BuildSource" batch file
Run the "Compile" batch file
You will now have an Ogre.pyd under the bin directory that should work :)
Check out the demos - ./demos/ogre for ogre 1.2.3 and ./demos/ogreCVS for the CVS version :)

You will also need to build a python wrapper for OIS if you are using the CVS version of Ogre
Follow the above directions (edit environment.py, Buildsource etc) from the PYOIS directory.