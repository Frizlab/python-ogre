import ogre.renderer.OGRE as ogre 
import SampleFramework as sf 

class MyRenderQueueListener(ogre.RenderQueueListener): 
   def __init__ ( self ):
        ogre.RenderQueueListener.__init__(self)
        
   def renderQueueStarted (self, id , invocation, skipThisInvocation): 
      if (id == ogre.RENDER_QUEUE_OVERLAY): 
          print "Queue Start Skip %i %i " % (id,ogre.RENDER_QUEUE_OVERLAY) 
          return 1 # True
      return 0 # False
   def renderQueueEnded (self, id , invocation,repeatThisInvocation): 
      if (id == ogre.RENDER_QUEUE_OVERLAY): 
          print "Queue End Repeat %i %i" % (id, ogre.RENDER_QUEUE_OVERLAY) 
          return 0 # False
      return 0 # False

class EnvMapApplication(sf.Application): 
    def _createScene( self ): 
        sceneManager = self.sceneManager 
        camera = self.camera 
        
        sceneManager.ambientLight = ogre.ColourValue (0.5, 0.5, 0.5) 

        light = sceneManager.createLight('MainLight') 
        light.setPosition (20, 80, 50) 

        entity = sceneManager.createEntity('head', 'ogrehead.mesh') 
        entity.setMaterialName("Examples/EnvMappedRustySteel") 
        self.myRenderQueueListener = MyRenderQueueListener() 
        self.renderQueueListener = sceneManager.addRenderQueueListener( self.myRenderQueueListener) 
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(entity) 

if __name__ == '__main__': 
 
    application = EnvMapApplication() 
    application.go() 
