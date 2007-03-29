import sys, os 

if sys.platform == 'win32': 
    os.environ['PATH'] += ';' + __path__[0] 

from _ogre_ import *

OgreException = _ogre_.Exception


## Note the work around to ensure the Radians and Degree types work correctly
## the problem is the constructors are overloaded in each class and passing a real to 
## Radian actually causes boost to try it as a Degree first, and as Degreee has a 'Real' overload
## it create the Degree and passes it back to Radian -- 
##
## and as it's a lot of work and  overly confusing to fix this in generate_code.py we fix it here
##
def Radian( r=None, d=None ):
    if isinstance( r, _ogre_.Degree ):
        return _ogre_.Radian( r )
    elif d:
        return _ogre_.Radian( d=d )
    else:
        return _ogre_.Radian( r=r )
       
def Degree( d=None, r=None ):
    if isinstance( d, _ogre_.Radian ):
        return _ogre_.Degree( d )
    elif r: 
        return _ogre.Degree (r=r)
    else:
        return _ogre_.Degree( d=d )
        
class Vector3 ( _ogre_.Vector3 ):
    def __str__ ( self ):
        
        return "Ogre.Vector3: x=%(x)f y=%(y)f y=%(y)f" % { 'x':self.x, 'y':self.y, 'z':self.z}