# This code is in the Public Domain
# it has been changed to a "wrapper" to handle both the 1.2.3 version of Ogre (current SDK) and the CVS version
# which is different in that it uses OIS for all input handling.
#

import ogre.renderer.OGRE as ogre

ogre.OgreVersion = ogre.GetOgreVersion()
ogre.OgreVersionString = ogre.OgreVersion[0] + ogre.OgreVersion[1] + ogre.OgreVersion[2]
ogre.PythonOgreVersion = ogre.GetPythonOgreVersion()
from ogre.renderer.OGRE.sf_OIS import * 
           
 

