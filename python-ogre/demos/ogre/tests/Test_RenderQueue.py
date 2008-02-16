import ogre.renderer.OGRE as ogre 
import SampleFramework as sf 

class MyRenderQueueListener(ogre.RenderQueueListener): 
   def __init__ ( self ):
        ogre.RenderQueueListener.__init__(self)
        
   def renderQueueStarted (self, id , invocation, skipThisInvocation): 
      print "In Queue Started", id, invocation, skipThisInvocation  
      print ogre.RENDER_QUEUE_OVERLAY
      print type (id)
      print type (ogre.RENDER_QUEUE_OVERLAY)
      if (id == ogre.RENDER_QUEUE_OVERLAY): 
          print "Queue Start Skip %i %i " % (id,ogre.RENDER_QUEUE_OVERLAY) 
          return True
      print "returning false"
      return False
   def renderQueueEnded (self, id , invocation,repeatThisInvocation): 
      print "In QueueEnded"
      if (id == ogre.RENDER_QUEUE_OVERLAY): 
          print "Queue End Repeat %i %i" % (id, ogre.RENDER_QUEUE_OVERLAY) 
          return False
      return False

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
