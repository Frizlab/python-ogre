# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-ogre
# For the latest info, see http://python-worg/
#
# It is likely based on original code from OGRE and/or Pyogre
# For the latest info, see http://www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import ogre.renderer.OGRE as ogre
import ogre.addons.theora as theora
import SampleFramework as sf
import sys

class VideoApplication(sf.Application):
    def _createScene(self):
        self.Clip=None
        sceneManager = self.sceneManager
        camera = self.camera
        
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.setSkyDome (True, 'Examples/CloudySky',4.0,8.0)

        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 150)
        
        ## Grab Our material, then add a new texture unit
# # #         material = ogre.MaterialManager.getSingleton().getByName("Examples/TheoraVideoPlayer/Play")

# # #         #Create the material the first time through this method
# # #         if not material :
# # #             print "CREATEING MATERIAL"
# # #             material = ogre.MaterialManager.getSingleton().create("Examples/TheoraVideoPlayer/Play", "General")
# # #     
# # #         material.getTechnique(0).getPass(0).createTextureUnitState()
        
#         print "Material OK"
#         self.VideoControl =  theora.InitVideoController() ##ogre.ExternalTextureSourceManager.getSingleton().getExternalTextureSource("ogg_video")
#         print "Returned from C++"
# #         if not self.VideoControl:
# #             print "\n\nError loading video control\n"
# #             sys.exit()
#         self.playmode = ogre.TextureEffectPause
# #         self.VideoControl.setInputName( "test.ogg" )
# #         self.VideoControl.setPlayMode(  self.playmode )
# #         
# #         self.VideoControl.setTextureTecPassStateLevel( 0, 0, 0 )
# #         self.VideoControl.setSeekEnabled( False )
# #         ##This is mainly for OpenAL - but applies to other audio libs which
# #         ##use pooling instead of callbacks for updating...
# #         ##Let TheoraMovieClip update the audioclip.
# #         self.VideoControl.setAutoAudioUpdate( False )
#         
#         
#         print dir (self.VideoControl)
#         print self.VideoControl.getInputName()
#         
# #         self.VideoControl.createDefinedTexture( "Examples/TheoraVideoPlayer/Play", "General" )
#         self.Clip = self.VideoControl.getMaterialNameClip('Examples/TheoraVideoPlayer/Play' )
        mat  = ogre.MaterialManager.getSingleton().getByName("Example/TheoraVideoPlayer/Play")
        print mat
        self.Clip =  theora.InitVideoController()
        if not self.Clip:
            print "\n\nClip not found\n"
            sys.exit()
        else:
            print "\n CLIP OK \n"
        self.playmode = ogre.TextureEffectPlay_ASAP
        self.Clip.changePlayMode ( self.playmode )
            
# #         self.text1 = self.Clip.getVideoDriver().getTexture()  
              
        # create head entity
        headNode = sceneManager.getRootSceneNode().createChildSceneNode()
        entity = sceneManager.createEntity('head', 'cube.mesh')
#         print dir(entity)
        headNode.attachObject(entity)
        entity.setMaterialName("Example/TheoraVideoPlayer/Play")
        headNode.setScale(2,2,2)
#         self.e = entity
#         self.m = material
#         self.h = headNode
       
# # #         ## Create background rectangle covering the whole screen
#         self.rect = ogre.Rectangle2D(True)
#         self.rect.setCorners(-0.5, 0.5, 0.5, -0.5)
#         self.rect.setMaterial("Examples/TheoraVideoPlayer/Play")
#         
# #         ## Render the background before everything else
#         self.rect.setRenderQueueGroup(ogre.RENDER_QUEUE_BACKGROUND)
# #         
#         ## Use infinite AAB to always stay visible
#         aabInf = ogre.AxisAlignedBox()
#         aabInf.setInfinite()
#         self.rect.setBoundingBox(aabInf)
# #         
#         ## Attach background to the scene
#         node = self.sceneManager.getRootSceneNode().createChildSceneNode("videoMaterial")
#         node.attachObject(self.rect)
# #         node.setPosition(0,0,-100)


# # #         self._createScalingPlane()
# # #         self._createScrollingKnot()
# # #         #self._createWateryPlane()
# # #         self._createVideoPlane()
# # #         skyMaterial = MaterialManager.getSingleton()   #get the material manager pointer
# # #         skyMaterial =  skyMaterial.create('SkyMat', ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME )
# # #         skyMaterial.lightingEnabled = False
# # #         
# # #         textureUnitState = skyMaterial.getTechnique(0).getPass(0).createTextureUnitState('clouds.jpg', 0)
# # #         textureUnitState.setScrollAnimation(0.15, 0)

# # #         sceneManager.setSkyDome(True, 'SkyMat', -5.0, 2.0)
# # #         
     

    def _createScalingPlane(self):
        entity = self.sceneManager.createEntity('Plane', ogre.SceneManager.PT_PLANE)
        entity.setMaterialName('Examples/TextureEffect1')
        node = self.sceneManager.getRootSceneNode().createChildSceneNode(Vector3(-250, -40, -100))
        node.attachObject(entity)

    def _createScrollingKnot(self):
        entity = self.sceneManager.createEntity('Knot', 'knot.mesh')
        entity.setMaterialName('Examples/TextureEffect2')
        node = self.sceneManager.getRootSceneNode().createChildSceneNode(Vector3(200, 50, 150))
        node.attachObject(entity)

    def _createWateryPlane(self):
        entity = self.sceneManager.createEntity('WaterPlane', ogre.SceneManager.PT_PLANE)
        entity.setMaterialName('Examples/TextureEffect3')
        node = self.sceneManager.getRootSceneNode().attachObject(entity)
        
    def _createVideoPlane(self):
        entity = self.sceneManager.createEntity('WaterPlane', ogre.SceneManager.PT_PLANE)
        entity.setMaterialName('Examples/Rockwall')
        node = self.sceneManager.getRootSceneNode().attachObject(entity)
        
        mat=MaterialManager.getSingleton().getByName('Examples/Rockwall')
        tex=mat.getTechnique(0).getPass(0).getTextureUnitState(0)
        tex.setTextureName(
            self.dshowMovieTextureSystem.getMovieTexture().getName())

      

    def _createFrameListener(self):
        self.frameListener = VideoListener(self.renderWindow, self.camera, self.Clip)
        self.root.addFrameListener(self.frameListener)
        

    def __del__(self):
        sf.Application.__del__(self)    
        
class VideoListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, video):
        self.video = video
        sf.FrameListener.__init__(self, renderWindow, camera)
 
    def frameStarted(self, frameEvent):
       ret = sf.FrameListener.frameStarted(self, frameEvent)
       if self.video and ret:
            self.video.blitFrameCheck()
            
       return ret
        
        
        
if __name__ == '__main__':
#     try:
        application = VideoApplication()
        application.go()
#     except OgreException, e:
#         print e
