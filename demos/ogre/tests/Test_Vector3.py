import exceptions as e
import ogre.renderer.OGRE as ogre

class NewVector3 ( ogre.Vector3 ):

    def __init__( self, value ):
        ogre.Vector3.__init__( self, *value )
        
    def __getitem__ ( self, index ):
        if not type ( index ) == type ( 1 ):
            raise e.TypeError
        if index < 0 or index > 3:
            raise e.IndexError
        return ogre.Vector3.__getitem__( self, index )
                   
    def __getslice__ ( self, start, end ):
        if not type ( start ) == type ( end ) == type ( 1 ) :
            raise e.TypeError
        if (start < 0 or start > 3) or ( end < 0 ):
            raise e.IndexError
        if end > 3: end = 3             
        retValues=[]            
        for x in range ( end - start ):
            retValues.append ( ogre.Vector3.__getitem__( self, start + x ) )                
        return retValues            
            
TestValues=[1.223,5.676,98.43]
               
v = NewVector3( TestValues )
print v
print v.x, v.y, v.z
print v[0], v[1], v[2]
print v[0:1]
print v[1:2]
print v[1:1]
print v[:]
print v[0:3]



    