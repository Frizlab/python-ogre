import os,sys, ctypes
if sys.platform == 'win32': 
    os.environ['PATH'] += ';' + __file__[0] 

    
    
import ogre.renderer.OGRE as ogre
import SampleFramework 
import math 

class FilePtr ( ogre.DataStream ):
    def __init__ ( self, filename ):
        ogre.DataStream.__init__(self)
        datain = file(filename, 'r').read() # should put error checking etc here
        self.length = len ( datain )
        self.source = ctypes.create_string_buffer( datain ) ## Note it allocates one extra byte
    def read ( self, dest, size ):
        if size <= self.length:
            print ogre.CastInt(dest)
            ctypes.memmove ( ogre.CastInt(dest), self.source, size ) # again should check here for 
    def size ( self ):
        return self.length
        
class TutorialApplication(SampleFramework.Application): 


    def _createScene(self): 
        sm = self.sceneManager 
#         f= file("test.material", 'r')
#         MatString = f.read()
#         RawMemBuffer = ctypes.create_string_buffer( MatString ) ## Note it allocates one extra byte
#         dataptr = ogre.MemoryDataStream ( ctypes.addressof ( RawMemBuffer ), len (MatString) + 1 )
#         ogre.MaterialManager.getSingleton().parseScript( dataptr, "General" )   
#         print "MATERIAL OK"     
        
        fp = FilePtr ( "test.material")
        print "##", fp
        dataptr = ogre.MemoryDataStream ( fp )
        ogre.MaterialManager.getSingleton().parseScript( dataptr, "General" )   
        print "MATERIAL OK"     
        
        
        try:
        
        
        
            entity = self.sceneManager.getEntity("Junk") 
        except ogre.OgreItemIdentityException, e:
            print"\nException OK: OgreItemIdentityException \n",e, "\n"

        try: 
             entity = self.sceneManager.getEntity("Junk") 
             entity.visible = True 

        except ogre.OgreException, e:
            print "Exception OK: OgreException\n", e, "\n"
            
        sm.ambientLight = (1,1,1) 
        ent1 = sm.createEntity("Robot","robot.mesh") 
        node1 = sm.rootSceneNode.createChildSceneNode("RobotNode") 
        node1.lookAt ( ogre.Vector3(0,0,0), ogre.Node.TransformSpace.TS_LOCAL )
        node1.lookAt ( (0,0,0), ogre.Node.TransformSpace.TS_WORLD )
        
        node1.attachObject(ent1) 
        
        try:
            node1.attachObject(ent1) # should fail as you can only attach once
        except ogre.OgreInvalidParametersException, e:
            print "Exception OK\n", e, "\n"
        dummy = node1.getAttachedObject ("doesnt_exist")
        
            
        ent2 = sm.createEntity("Robot2",'robot.mesh') 
        node2 = node1.createChildSceneNode("Robot2Node",(50,0,0)) 
        node2.attachObject(ent2) 
        node1.setScale = (2,2,2) 
        node2.setScale = (.5,.5,.5) 
        node2.roll(math.pi/2.0) 
        bb = ent1.getBoundingBox() 
        print "\n\nThis should now print 8 sets of vectors (Corners)"
        for vect in bb.allCorners:
            print vect.x, vect.y, vect.z  
        print "\n\n"
        sys.exit()

if __name__ == '__main__': 
    ta = TutorialApplication() 
    ta.go() 
