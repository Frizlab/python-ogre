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
import ogre.video as OgreDshow 
import SampleFramework as sf

class VideoApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera
        
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)

        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 150)
        
        self.dshowMovieTextureSystem =  OgreDshow.DirectShowMovieTexture(304, 240)
        movieName = "c:/windows/clock.avi"
        self.dshowMovieTextureSystem.loadMovie(movieName)
        self.dshowMovieTextureSystem.playMovie()
        
        
        self._createScalingPlane()
        self._createScrollingKnot()
        #self._createWateryPlane()
        self._createVideoPlane()
        skyMaterial = ogre.MaterialManager.getSingleton()   #get the material manager pointer
        skyMaterial =  skyMaterial.create('SkyMat', ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME )
        skyMaterial.lightingEnabled = False
        
        textureUnitState = skyMaterial.getTechnique(0).getPass(0).createTextureUnitState('clouds.jpg', 0)
        textureUnitState.setScrollAnimation(0.15, 0)

        sceneManager.setSkyDome(True, 'SkyMat', -5.0, 2.0)
        
     

    def _createScalingPlane(self):
        entity = self.sceneManager.createEntity('Plane', ogre.SceneManager.PT_PLANE)
        entity.setMaterialName('Examples/TextureEffect1')
        node = self.sceneManager.getRootSceneNode().createChildSceneNode(ogre.Vector3(-250, -40, -100))
        node.attachObject(entity)

    def _createScrollingKnot(self):
        entity = self.sceneManager.createEntity('Knot', 'knot.mesh')
        entity.setMaterialName('Examples/TextureEffect2')
        node = self.sceneManager.getRootSceneNode().createChildSceneNode(ogre.Vector3(200, 50, 150))
        node.attachObject(entity)

    def _createWateryPlane(self):
        entity = self.sceneManager.createEntity('WaterPlane', ogre.SceneManager.PT_PLANE)
        entity.setMaterialName('Examples/TextureEffect3')
        node = self.sceneManager.getRootSceneNode().attachObject(entity)
        
    def _createVideoPlane(self):
        entity = self.sceneManager.createEntity('WaterPlane', ogre.SceneManager.PT_PLANE)
        entity.setMaterialName('Examples/Rockwall')
        node = self.sceneManager.getRootSceneNode().attachObject(entity)
        
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
        self.video.updateMovieTexture()
        return sf.FrameListener.frameStarted(self, frameEvent)
        
        
        
if __name__ == '__main__':
    try:
        application = VideoApplication()
        application.go()
    except ogre.OgreException, e:
        print e
