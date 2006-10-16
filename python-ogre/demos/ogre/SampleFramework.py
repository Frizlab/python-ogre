# This code is in the Public Domain
# it has been changed to a "wrapper" to handle both the 1.2.3 version of Ogre (current SDK) and the CVS version
# which is different in that it uses OIS for all input handling.
#

import Ogre as ogre
if ogre.version[:3] == "1.2":
    from sf import *
else:
    from sf_OIS import *
    
