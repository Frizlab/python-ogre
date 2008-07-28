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
    newpath = os.path.join ( os.path.abspath(os.path.dirname(__file__)), '../../plugins')
    os.environ['PATH'] =  newpath +';' + os.environ['PATH']
    
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
        
        
        self.Control =  theora.TheoraVideoController() ##InitVideoController()  getVideoController() ## 

        if not self.Control:
            print "\n\nClip not found\n"
            sys.exit()
        self.Control.setInputName( '../media_extra/fish.ogg' )
        self.Control.setPlayMode(theora.TextureEffectPause )

        self.Control.setTextureTecPassStateLevel( 0, 0, 0 )
        self.Control.setSeekEnabled( False )
#         self.Control.setAutoAudioUpdate ( True )
        
        mat  = ogre.MaterialManager.getSingleton().getByName("Example/TheoraVideoPlayer/Play")
        #Create the material the first time through this method
        if not mat :
            mat = ogre.MaterialManager.getSingleton().create("Example/TheoraVideoPlayer/Play", "General")
    
        mat.getTechnique(0).getPass(0).createTextureUnitState()
        self.Control.createDefinedTexture( "Example/TheoraVideoPlayer/Play", "General" )
        
        
        sm = theora.SoundManager()
        self.SoundSystem=None
        mods=sm.loadSoundModules()
        if len(mods) > 0:
            self.SoundSystem = sm.startUpSoundManager( 'FMOD_MOD' ) #OAL
        
        self.Clip =  self.Control.getMaterialNameClip("Example/TheoraVideoPlayer/Play")
        
        if self.SoundSystem:
            self.Audio = self.SoundSystem.createAudio()
            self.Clip.setAudioDriver( self.Audio )

        
        self.playmode = theora.TextureEffectPlay_ASAP
        self.Clip.changePlayMode ( self.playmode )
         
        c = sceneManager.createEntity("cubo", "cube.mesh")
        n = sceneManager.getRootSceneNode().createChildSceneNode()
        n.attachObject(c)
        n.setScale(2,2,2)
        n.setPosition(ogre.Vector3(250, 0, 0)) 
        c.setMaterialName("Example/TheoraVideoPlayer/Play")
        
            
            
    def _createFrameListener(self):
        self.frameListener = VideoListener(self.renderWindow, self.camera, self.Clip)
        self.root.addFrameListener(self.frameListener)

        
class VideoListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, video):
        self.video = video
        sf.FrameListener.__init__(self, renderWindow, camera)
 
    def frameStarted(self, frameEvent):
       ret = sf.FrameListener.frameStarted(self, frameEvent)
       if self.video and ret:
            self.video.blitFrameCheck()
            self.video.getAudioDriver().autoUpdate()
       return ret
        
        
        
if __name__ == '__main__':
#     try:
        application = VideoApplication()
        application.go()
#     except OgreException, e:
#         print e
