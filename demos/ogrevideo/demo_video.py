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
import sys
sys.path.insert(0,'..')
import PythonOgreConfig
import ctypes
import os

if sys.platform == 'win32': 
    newpath = os.path.abspath(os.path.join ( os.path.abspath(os.path.dirname(__file__)), '../../plugins'))
    print newpath
    os.environ['PATH'] =  newpath +';' + os.environ['PATH']
    print os.environ['PATH']
    
import ogre.renderer.OGRE as ogre
import ogre.addons.ogrevideo as theora
import SampleFramework as sf
import sys

demo = "Single" ## "Multiple" or "Cube"
# demo = "Multiple"
# demo = "Cube"

class VideoApplication(sf.Application):

    def _loadResources( self ):
      if demo == "Single":  # only do audio if it's a single video stream...
         theora.enableOpenALAudio()
# #          self.mAudioFactory= theora.OpenAL_AudioInterfaceFactory()
# #          mgr = ogre.ExternalTextureSourceManager.getSingleton().getExternalTextureSource("ogg_video")
# #          mgr.setAudioInterfaceFactory(self.mAudioFactory)

      sf.Application._loadResources ( self ) #ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()


    def _createScene(self):
         self.Clip=None
         sceneManager = self.sceneManager
         camera = self.camera
         
#          sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
#          sceneManager.setSkyDome (True, 'Examples/CloudySky',4.0,8.0)
         
         light = sceneManager.createLight('MainLight')
         light.setPosition (20, 80, 150)
         
         if demo == "Single":
            ## create one quad, to minimize rendering time inpact on benchmarking
            model = sceneManager.createManualObject("quad")
            model.begin("SimpleVideo")
            model.position( 1,-1,0)
            model.textureCoord(1,1)
            model.position( 1,1,0)
            model.textureCoord(1,0)
            model.position(-1,1,0)
            model.textureCoord(0,0)
            model.position(-1,1,0)
            model.textureCoord(0,0)
            model.position( 1,-1,0)
            model.textureCoord(1,1)
            model.position(-1,-1,0)
            model.textureCoord(0,1)
            model.end()
            model.setUseIdentityProjection(True)
            model.setUseIdentityView(True)
            node = sceneManager.getRootSceneNode().createChildSceneNode()
            node.attachObject(model)

                  
         if demo == "Multiple": ## multiple videos
            ## this is for 4 concurrent videos...
            left=[ -1, 0.01,    -1,  0.01]
            top=[1,    1, -0.01, -0.01]
            right=[-0.01,    1, -0.01,     1]
            bottom=[0.01, 0.01,    -1,    -1]
            
            materials=["konqi","fedora01","fedora02","fedora03"]
            for i in range ( 4):
               model = sceneManager.createManualObject("quad"+str(i))
               model.begin(materials[i])
               model.position( right[i],bottom[i],0)
               model.textureCoord(1,1)
               model.position( right[i],top[i],0)
               model.textureCoord(1,0)
               model.position(left[i],top[i],0)
               model.textureCoord(0,0)
               model.position(left[i],top[i],0)
               model.textureCoord(0,0)
               model.position( right[i],bottom[i],0)
               model.textureCoord(1,1)
               model.position(left[i],bottom[i],0)
               model.textureCoord(0,1)
               model.end()
               model.setUseIdentityProjection(True)
               model.setUseIdentityView(True)
               node = sceneManager.getRootSceneNode().createChildSceneNode()
               node.attachObject(model)
            
         
         ## this is video on the sides of cube
         if demo == "Cube":
            c = sceneManager.createEntity("cube", "cube.mesh")
            c.setMaterialName("SimpleVideo")
   
            n = sceneManager.getRootSceneNode().createChildSceneNode()
            n.attachObject(c)
            scale = 4
            n.setScale(scale, scale, scale)
            n.setPosition(ogre.Vector3(0, 0,10))
            n.yaw(ogre.Degree(30))
            
    def _createFrameListener(self):
        self.frameListener = VideoListener(self.renderWindow, self.camera, self.Clip)
        self.root.addFrameListener(self.frameListener)

        
class VideoListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, video):
        self.video = video
        sf.FrameListener.__init__(self, renderWindow, camera)
 
    def frameStarted(self, frameEvent):
      ret = sf.FrameListener.frameStarted(self, frameEvent)
#       self.mgr = ogre.ExternalTextureSourceManager.getSingleton().getExternalTextureSource("ogg_video")
#       self.clip= self.mgr.getVideoClipByName("konqi.ogg")
#  
#       dur=self.clip.getDuration()
      
      return ret
        
        
        
if __name__ == '__main__':
#     try:
        application = VideoApplication()
        application.go()
#     except OgreException, e:
#         print e
