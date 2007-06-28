#-----------------------------------------------------------------------------#
#                                                                             #
#   This source code is part of the python-ogre techdemo project.             #
#                                                                             #
#   This program is released as public domain                                 #
#                                                                             #
#-----------------------------------------------------------------------------#
#   
#   TITLE: Ocean Listener
#   DESCRIPTION: Cut and paste from the PO Render to texture demo


import ogre.renderer.OGRE as ogre
import logging

class oceanRttManager(ogre.FrameListener, ogre.RenderTargetListener):
    def __init__(self, mainCamera, renderWindow):
        ogre.FrameListener.__init__(self)
        self.mPlane = None
        self.mPlaneEnt = None
        self.camera = mainCamera
        self.mReflectCam= None
        self.mPlaneNode= None
        self.events = []
        ogre.RenderTargetListener.__init__(self)
        
    def __del__(self):
        del self.camera
        del self.mReflectCam
        print 'OceanListener Destroyed'
        
    def _createScene(self, sceneManager, renderWindow):
        "Override sf create scene"
        # Create a prefab plane
        self.mPlane = ogre.MovablePlane("ReflectPlane")
        self.mPlane.d = 0
        self.mPlane.normal =ogre.Vector3.UNIT_Y
          
#         ogre.MeshManager.getSingleton().createPlane("ReflectionPlane", 
#             ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, 
#             self.mPlane._getDerivedPlane(), 2000.0, 2000.0, 
#             1, 1, True, 1, 1.0, 1.0, ogre.Vector3.UNIT_Z,
#             ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, 
#             True,True
#             )
        ogre.MeshManager.getSingleton().createPlane("ReflectionPlane", 
            ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, 
            self.mPlane, 20000.0, 20000.0, 
            1, 1, True, 1, 1.0, 1.0, ogre.Vector3.UNIT_Z,
            ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, 
            True,True
            )
        # Create plane entity
        self.mPlaneEnt = sceneManager.createEntity( "WaterRefPlane", "ReflectionPlane" )

        # Attach the rtt entity to the root of the scene
        rootNode = sceneManager.getRootSceneNode()
        self.mPlaneNode = rootNode.createChildSceneNode()
        
        # Attach both the plane entity, and the plane definition
        self.mPlaneNode.attachObject(self.mPlaneEnt)
        self.mPlaneNode.attachObject(self.mPlane) 
        
        #self.mPlaneNode.translate( ogre.Vector3(0, -10, 0))
        #self.mPlaneNode.roll(ogre.Degree(d=5))
        
        
        ## Either of these techniques works...
        # create RenderTexture
        rttTex = ogre.Root.getSingleton().getRenderSystem().createRenderTexture( "RttTex", 512, 512, 
                                                                 ogre.TEX_TYPE_2D,ogre.PixelFormat.PF_R8G8B8 )
#         texture = ogre.TextureManager.getSingleton().createManual( "RttTex", 
#                     ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, ogre.TEX_TYPE_2D, 
#                     512, 512, 0, ogre.PixelFormat.PF_R8G8B8, ogre.TU_RENDERTARGET )
        
        self.mReflectCam = sceneManager.createCamera("ReflectCam")
        self.mReflectCam.setNearClipDistance(self.camera.getNearClipDistance())
        self.mReflectCam.setFarClipDistance(self.camera.getFarClipDistance())
        
        w = renderWindow.getViewport(0).getActualWidth() 
        h = renderWindow.getViewport(0).getActualHeight ()
        self.mReflectCam.setAspectRatio ( float(w)/float(h) )

        # create ViewPort        
        v = rttTex.addViewport( self.mReflectCam )
        v.setClearEveryFrame ( True  )
        v.setBackgroundColour (ogre.ColourValue.White)
       
        mat = ogre.MaterialManager.getSingleton().create("RttMat",ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME)
        t = mat.getTechnique(0).getPass(0).createTextureUnitState('Water02.png')
        #mat.getTechnique(0).getPass(0).setSceneBlending(ogre.SceneBlendType.SBT_TRANSPARENT_ALPHA)
        rt = mat.getTechnique(0).getPass(0).createTextureUnitState('RttTex') # creates the reflection...

        # Blend with base texture
        rt.setColourOperationEx( ogre.LBX_BLEND_MANUAL, ogre.LBS_TEXTURE, 
                                ogre.LBS_CURRENT, ogre.ColourValue.White,
                                ogre.ColourValue.White, 0.5)
                                
        rt.setTextureAddressingMode (ogre.TextureUnitState.TAM_CLAMP)
        rt.setProjectiveTexturing(True, self.mReflectCam)
         
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
        
        self.renderWindow = renderWindow
        
    def frameStarted(self, frameEvent):
        if(self.renderWindow.isClosed()):
            return False
        # Make sure reflection camera is updated too
        self.mReflectCam.setOrientation ( self.camera.getOrientation() )
        self.mReflectCam.setPosition (self.camera.getPosition())
        return True 
    
    def preRenderTargetUpdate(self,evt):
        self.mPlane.setVisible(False)
           
    def postRenderTargetUpdate(self,evt):
        self.mPlane.setVisible(True)
        
