# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http://www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.addons.ogredshow as OgreDshow 
import SampleFramework as sf

class VideoApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera
        
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)

        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 150)
        
        self.dshowMovieTextureSystem =  OgreDshow.DirectShowMovieTexture(160,120) ## get this right for scaling
        movieName = "../media_extra/fish.avi"
        
        self.dshowMovieTextureSystem.loadMovie(movieName)
        self.dshowMovieTextureSystem.playMovie()
        
        skyMaterial = ogre.MaterialManager.getSingleton()   #get the material manager pointer
        skyMaterial =  skyMaterial.create('SkyMat', ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME )
        skyMaterial.lightingEnabled = False
        
        textureUnitState = skyMaterial.getTechnique(0).getPass(0).createTextureUnitState('clouds.jpg', 0)
        textureUnitState.setScrollAnimation(0.15, 0)

        sceneManager.setSkyDome(True, 'SkyMat', -5.0, 2.0)
         
        c = sceneManager.createEntity("cubo", "cube.mesh")
        n = sceneManager.getRootSceneNode().createChildSceneNode()
        n.attachObject(c)
        n.setScale(2,2,2)
        n.setPosition(ogre.Vector3(250, 0, 0)) 
        
        c.setMaterialName('Examples/Rockwall')
        
        mat=ogre.MaterialManager.getSingleton().getByName('Examples/Rockwall')
        tex=mat.getTechnique(0).getPass(0).getTextureUnitState(0)
        tex.setTextureName(
            self.dshowMovieTextureSystem.getMovieTexture().getName())

    def _createFrameListener(self):
        self.frameListener = VideoListener(self.renderWindow, self.camera, self.dshowMovieTextureSystem)
        self.root.addFrameListener(self.frameListener)

    def __del__(self):
        self.dshowMovieTextureSystem.stopMovie()
        del ( self.dshowMovieTextureSystem )
        sf.Application.__del__(self)    
        
class VideoListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, video):
        self.video = video
        sf.FrameListener.__init__(self, renderWindow, camera)
 
    def frameStarted(self, frameEvent):
        if not self.video.isPlayingMovie():
            self.video.rewindMovie()
            self.video.playMovie()
        else:
            self.video.updateMovieTexture()
        return sf.FrameListener.frameStarted(self, frameEvent)
        
if __name__ == '__main__':
    try:
        application = VideoApplication()
        application.go()
    except ogre.OgreException, e:
        print e
