
import sys
sys.path.insert(0,'..')
#import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as ois
#import ogre.addons.caelum as caelum
import tile_Manager as tiles
import SampleFramework as sf
import ctypes
import random
from tile_Config import *
import tile_interface

class TerrainApplication(sf.Application):

    def __init__(self):
        sf.Application.__init__(self)


    def __del__(self):
        del self.camera
        del self.sceneManager
        del self.frameListener
        del self.root
        del self.renderWindow


    def _chooseSceneManager(self):
        self.sceneManager = self.root.createSceneManager("TerrainSceneManager")

    def _createCamera(self):
        self.camera = self.sceneManager.createCamera('PlayerCam')
        self.camera.setPosition (ogre.Vector3(1100,1000,1000))
        self.raySceneQuery = self.sceneManager.createRayQuery(ogre.Ray(self.camera.getPosition(), ogre.Vector3.NEGATIVE_UNIT_Y))


    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera
        sceneManager.setAmbientLight(ogre.ColourValue(0.5,0.5,0.5,1.0))
        sceneManager.setSkyDome(True, "Examples/CloudySky")
        self.landNode = self.sceneManager.getRootSceneNode().createChildSceneNode("LandNode")
        self.tileManager = tiles.TileManager("test", self.landNode)
        print dir(self.tileManager)
        self.tileManager.setMapSize(512, 512)
        td = []
        for x in range(512):
            for y in range(512):
                u = float(x) / 512.0
                v = float(y) / 512.0
                td.append(TileData(TexCoords(u,v), x+y, True))
                
        self.tileManager.loadTileData(td)
        hm = [ random.uniform(0.0, 1.0) ] * (513*513)
        self.tileManager.loadHeightmap(hm)
        self.tileManager.createChunks()
        self.tileInterface = tile_interface.TileInterface(self.tileManager, self.camera)
        
    def _createFrameListener(self):
        self.frameListener = tileTerrainListener(self.renderWindow, self.camera, self.tileInterface)
        self.root.addFrameListener(self.frameListener)
        
        
class tileTerrainListener( sf.FrameListener ):
    def __init__(self, rw, camera, tileInterface):
        sf.FrameListener.__init__(self, rw, camera)
        self.tileInterface = tileInterface
        self.moveSpeed = 2000.0
        
    def frameStarted(self, evt):
        self.tileInterface.controlChunks(evt.timeSinceLastFrame)
        return sf.FrameListener.frameStarted(self, evt)


if __name__ == '__main__':
    try:
        application = TerrainApplication()
        application.go()

    except ogre.OgreException, e:
        print e
