import os,sys, ctypes
if sys.platform == 'win32': 
    os.environ['PATH'] += ';' + __file__[0] 

    
    
import ogre.renderer.OGRE as ogre
import SampleFramework 
import math 



class UD ( ogre.UserDefinedObject ):
    def __init__ ( self, id=99, name='Default' ):
        ogre.UserDefinedObject.__init__(self)
        self.id = id
        self.name = name
        
    def getTypeName ( self ):
        return self.name  
        
    def getTypeID ( self ):
        return self.id      
       
class TutorialApplication(SampleFramework.Application): 


    def _createScene(self): 
        sm = self.sceneManager
         
        ent1 = sm.createEntity("Robot","robot.mesh") 
        ud = UD( )
        ent1.setUserObject ( ud )
        
        nud = ent1.getUserObject()
        print nud
        print nud.id, nud.name
        print nud.getTypeName()
        print nud.getTypeID()   
        
        meshMgr = ogre.MeshManager.getSingleton() 
        mesh = meshMgr.createManual("test", ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        print mesh
        print dir(mesh)
        ser = ogre.MeshSerializer()
        ser.exportMesh(mesh,".cube.mesh");
       
        sys.exit()

if __name__ == '__main__': 
    ta = TutorialApplication() 
    ta.go() 
