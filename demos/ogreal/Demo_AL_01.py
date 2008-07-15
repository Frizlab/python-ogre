import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.sound.OgreAL as OgreAL
import SampleFramework as sf

class RenderToTextureFrameListener(sf.FrameListener):

    def __init__(self, renderWindow, camera, mReflectCam , planeSceneNode, pitchnode):
    
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.camera = camera
        self.mReflectCam = mReflectCam       
        self.planeNode = planeSceneNode
        self.pitchnode = pitchnode

    def frameStarted(self, frameEvent):
        result = sf.FrameListener.frameStarted(self,frameEvent)
        if not result: return result
        # Make sure reflection camera is updated too
        self.mReflectCam.setOrientation ( self.camera.getOrientation() )
        self.mReflectCam.setPosition (self.camera.getPosition())
        self.pitchnode.setOrientation ( self.camera.getOrientation() )
        self.pitchnode.setPosition (self.camera.getPosition())
        # Rotate plane
        dd = ogre.Degree(d=(30.0 * frameEvent.timeSinceLastFrame))
        self.planeNode.yaw(dd, ogre.Node.TS_PARENT)
        
        return result        
        
class test1 (ogre.MovableObject):
    def __init__(self):
        print "Test1 __init__"
        
        
class RenderToTextureApplication(sf.Application,ogre.RenderTargetListener):
    def __init__(self):
        "Init Render Application"
        sf.Application.__init__(self)
        self.mPlane = None
        self.mPlaneEnt = None
        self.mReflectCam= None
        self.mPlaneNode= None
        ogre.RenderTargetListener.__init__(self)
#         self.soundManager  = OgreAL.SoundManager()
        
    def preRenderTargetUpdate(self,evt):
        self.mPlane.setVisible(False)
           
    def postRenderTargetUpdate(self,evt):
        self.mPlane.setVisible(True)
        
    def _createCamera(self):
        """Creates the camera."""        
        self.camera = self.sceneManager.createCamera('PlayerCam')
        self.camera.setPosition(ogre.Vector3(0, 0, 500))
        self.camera.lookAt(ogre.Vector3(0, 0, -300))
        self.camera.NearClipDistance = 5
        
    def __del__ ( self ):
        del self.soundManager
        sf.Application.__del__(self)
        
    def _createScene(self):
        "Override sf create scene"
        self.soundManager  = OgreAL.SoundManager() ## "Generic Software")
        
        
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.setAmbientLight (ogre.ColourValue(0.2, 0.2, 0.2))
        sceneManager.setSkyBox(True, "Examples/MorningSkyBox")

        # Create a light
        l = sceneManager.createLight("MainLight")
        l.setType (ogre.Light.LT_DIRECTIONAL)
        dirn = ogre.Vector3(0.5, -1, 0)
        dirn.normalise()
        l.setDirection(dirn)
        l.setDiffuseColour (ogre.ColourValue(1.0, 1.0, 0.8))
        l.setSpecularColour ( ogre.ColourValue(1.0, 1.0, 1.0))

        # Create a prefab plane
        self.mPlane = ogre.MovablePlane(name="ReflectPlane")
        
        self.mPlane.d = 0
        self.mPlane.normal =ogre.Vector3.UNIT_Y
          
        ogre.MeshManager.getSingleton().createPlane("ReflectionPlane", 
            ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, 
            self.mPlane, 2000.0, 2000.0, 
            1, 1, True, 1, 1.0, 1.0, ogre.Vector3.UNIT_Z,
            ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, 
            True,True
            )
        # Create plane entity
        self.mPlaneEnt = sceneManager.createEntity( "Plane", "ReflectionPlane" )
 
        #Create an entity from a model (will be loaded automatically)
        knotEnt = sceneManager.createEntity("Knot", "knot.mesh")
        knotEnt.setMaterialName("Examples/TextureEffect2")

        ogreHead = sceneManager.createEntity("Head", "ogrehead.mesh")

        # Attach the rtt entity to the root of the scene
        rootNode = sceneManager.getRootSceneNode()
        self.mPlaneNode = rootNode.createChildSceneNode()
        
        # Attach both the plane entity, and the plane definition
        self.mPlaneNode.attachObject(self.mPlaneEnt)
        self.mPlaneNode.attachObject(self.mPlane) 
        
        self.mPlaneNode.translate( ogre.Vector3(0, -10, 0))
        self.mPlaneNode.roll(ogre.Degree(d=5))

               #
        #
        ## SOUND !!!!!!!!!!!!
        #
        #
#         
        
        dl = self.soundManager.getDeviceList()
#        for dev in dl:
#            print "Device available:", dev
            
        node = rootNode.createChildSceneNode( "Head" )
        node.attachObject( ogreHead )
        sound = self.soundManager.createSound("Roar", "roar.wav", True) ## "6chan.ogg", True) ## "roar.wav", True)
        node.attachObject(sound)
        sound.play()
        OgreAL.SoundManager.getSingleton().getSound("Roar").play()
        
        
        bgSound = self.soundManager.createSound("ZeroFactor", "Zero Factor - Untitled.ogg", True, True)
       
        
    	node = sceneManager.getRootSceneNode().createChildSceneNode("CameraNode")
     	node.setPosition(0, 100, 100)
     	
     	node = node.createChildSceneNode("PitchNode")
#       	node.attachObject(self.camera)
     	node.attachObject(self.soundManager.getListener())
     	node.pitch(ogre.Degree(-30))
     	self.pitchnode = node
        OgreAL.SoundManager.getSingleton().getSound("Roar").play()
        
        
        ## Either of these techniques works...
#         # create RenderTexture
#         rttTex = self.root.getRenderSystem().createRenderTexture( "RttTex", 512, 512, 
#                                                                  ogre.TEX_TYPE_2D,ogre.PixelFormat.PF_R8G8B8 )
#         texture = ogre.TextureManager.getSingleton().createManual( "RttTex", 
#                     ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, ogre.TEX_TYPE_2D, 
#                     512, 512, 0, ogre.PixelFormat.PF_R8G8B8, ogre.TU_RENDERTARGET )
  
        ## Setup Render To Texture for preview window
        texture = ogre.TextureManager.getSingleton().createManual( "RttTex", 
                        ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, ogre.TEX_TYPE_2D, 
                        512, 512, 0, ogre.PixelFormat.PF_R8G8B8, ogre.TU_RENDERTARGET )
        rttTex = texture.getBuffer().getRenderTarget()


      
        self.mReflectCam = sceneManager.createCamera("ReflectCam")
        self.mReflectCam.setNearClipDistance(camera.getNearClipDistance())
        self.mReflectCam.setFarClipDistance(camera.getFarClipDistance())
        
        w = self.renderWindow.getViewport(0).getActualWidth() 
        h = self.renderWindow.getViewport(0).getActualHeight ()
        self.mReflectCam.setAspectRatio ( float(w)/float(h) )

        # create ViewPort        
        v = rttTex.addViewport( self.mReflectCam )
        v.setClearEveryFrame ( True  )
        v.setBackgroundColour (ogre.ColourValue.White)
       
        mat = ogre.MaterialManager.getSingleton().create("RttMat",ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME)
        t = mat.getTechnique(0).getPass(0).createTextureUnitState('RustedMetal.jpg')
        t = mat.getTechnique(0).getPass(0).createTextureUnitState('RttTex') # creates the reflection...
        # Blend with base texture
        t.setColourOperationEx( ogre.LBX_BLEND_MANUAL, ogre.LBS_TEXTURE, 
                                ogre.LBS_CURRENT, ogre.ColourValue.White,
                                ogre.ColourValue.White, 0.25)
                                
        t.setTextureAddressingMode (ogre.TextureUnitState.TAM_CLAMP)
        t.setProjectiveTexturing(True, self.mReflectCam)
         
        rttTex.addListener(self)

        # set up linked reflection
        self.mReflectCam.enableReflection(self.mPlane)
        # Also clip
        
        # NOTE..  a difference in Python implementation
        ## this doesn't work as mPlane is treated as a Plane instead of a MoveablePlane 
        #self.mReflectCam.enableCustomNearClipPlane(self.mPlane) 
        ## So we have an override :)
        
        self.mReflectCam.enableCustomNearClipPlaneMP(self.mPlane) 

        #Give the plane a texture
        self.mPlaneEnt.setMaterialName("RttMat")

        for  n in range (10):
            #Create a new node under the root
            node = sceneManager.createSceneNode()
            #Random translate
            nodePos = ogre.Vector3()
            nodePos.x = ogre.Math.SymmetricRandom() * 750.0
            nodePos.y = ogre.Math.SymmetricRandom() * 100.0 + 25
            nodePos.z = ogre.Math.SymmetricRandom() * 750.0
            node.setPosition(nodePos)
            rootNode.addChild(node)
            # Clone knot
            cloneEnt = knotEnt.clone("Knot" + str (n))
            # Attach to new node
            node.attachObject(cloneEnt)
            
        camera.setPosition (ogre.Vector3(-50, 100, 500))
        camera.lookAt (0,0,0)
#         camera.attachObject(OgreAL.SoundManager.getListener())
# node = sceneMgr.getRootSceneNode().createChildSceneNode("CameraNode");
# node.setPosition(0, 100, 100);
# node = node.createChildSceneNode("PitchNode");
# node.attachObject(camera);
# node.attachObject(soundManager.getListener());
# node.pitch(Ogre::Degree(-30));
      
    def _createFrameListener(self):
        # "create FrameListener"
        self.fL = RenderToTextureFrameListener(self.renderWindow, self.camera,
                                                                   self.mReflectCam, self.mPlaneNode, self.pitchnode)
        self.root.addFrameListener(self.fL)

if __name__ == '__main__':
   try:
        application = RenderToTextureApplication()
        application.go()
   except ogre.OgreException, e:
        print e        

