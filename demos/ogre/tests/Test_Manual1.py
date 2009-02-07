import ogre.renderer.OGRE as ogre 
import ogre.io.OIS as ois 
import SampleFramework as sf 

class TestListener(sf.FrameListener): 

    def __init__(self, renderWindow, camera, sceneManager): 
        sf.FrameListener.__init__(self, renderWindow, camera) 
        self.sceneManager = sceneManager 
         ## setup 3D line node. 
        self.DragLineNode = self.sceneManager.getRootSceneNode().createChildSceneNode() 
        self.DragLine = ogre.ManualObject( "__DRAG_LINES__" ) 

    def frameStarted(self, evt): 
        # draw manual object 
        start = ogre.Vector3(-100,0,0) 
        end = ogre.Vector3 ( 10,20,30 )        
        self.DragLineNode.detachAllObjects() 
        self.DragLine.clear() 
        self.DragLine.begin("BaseWhiteNoLighting", ogre.RenderOperation.OT_LINE_LIST ) 
        self.DragLine.position( start ) 
        self.DragLine.position( end ) 
        self.DragLine.end() 
        self.DragLineNode.attachObject( self.DragLine ) 
        return sf.FrameListener.frameStarted(self,evt)


class Application(sf.Application): 

    def _createScene(self): 
        sceneManager = self.sceneManager
        camera = self.camera
       
        sceneManager.AmbientLight = ogre.ColourValue(0.5, 0.5, 0.5)

        self.camNode = self.sceneManager.getRootSceneNode().createChildSceneNode("CamNode", (-400, 200, 400)) 
        self.camNode.attachObject(self.camera) 

    def _createFrameListener(self): 
        self.frameListener = TestListener(self.renderWindow, self.camera,self.sceneManager) 
        self.root.addFrameListener(self.frameListener) 


if __name__ == "__main__": 
    try: 
        a = Application() 
        a.go() 
    except ogre.OgreException, e: 
        print e 