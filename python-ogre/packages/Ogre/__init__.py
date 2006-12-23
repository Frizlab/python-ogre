import sys, os 

if sys.platform == 'win32': 
    print "*******", __path__, __path__[0]
    
    os.environ['PATH'] += ';' + __path__[0] 

from _ogre_ import *
OgreException = _ogre_.Exception


    