# This code is in the public domain
import Ogre as ogre
import SampleFramework as sf

class TerrainApplication(sf.Application):
    def _chooseSceneManager(self):
        self.sceneManager = self.root.createSceneManager("TerrainSceneManager")

    def _createCamera(self):
        self.camera = self.sceneManager.createCamera('PlayerCam')
 
        self.camera.setPosition (ogre.Vector3(128, 25, 128))
        self.camera.lookAt(ogre.Vector3(0, 0, -300))
        self.camera.nearClipDistance = 1

        # infinte far clip plane?
        #if self.root.renderSystem.capabilities.hasCapability(ogre.RSC_INFINITE_FAR_PLANE):
            #self.camera.farClipDistance = 0
        #else:
        self.camera.farClipDistance = 1000 

    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        
        fadeColour = ogre.ColourValue(0.93, 0.86, 0.76)
        sceneManager.setFog(ogre.FOG_LINEAR, fadeColour, 0.001, 500.0, 1000.0)
        self.renderWindow.getViewport(0).backgroundColour = fadeColour

        sceneManager.setWorldGeometry('terrain.cfg')

        # setup the sky plane
        plane = ogre.Plane()
        # 5000 world units from the camera
        plane.d = 5000
        # Above the camera, facing down
        plane.normal = -ogre.Vector3.UNIT_Y
        # Create a light
        l = sceneManager.createLight("MainLight")
        l.setPosition(20,80,50)
 
        ## Setup a nice starting position for the camera
        camera.setPosition(707,2500,528)
        camera.setOrientation(ogre.Quaternion(-0.3486, 0.0122, 0.9365, 0.0329))

            
    def _createFrameListener(self):
        self.frameListener = TerrainListener(self.renderWindow, self.camera, self.sceneManager)
        self.root.addFrameListener(self.frameListener)

class TerrainListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, sceneManager):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.sceneManager = sceneManager
        self.moveSpeed = 50.0
        self.raySceneQuery = sceneManager.createRayQuery(ogre.Ray(camera.getPosition(),
                                                                    ogre.Vector3.NEGATIVE_UNIT_Y))
        self.camera = camera
        self.camera.setPosition (self.camera.getPosition() + ogre.Vector3(0.0, 10.0, 0.0))

    def frameStarted(self, frameEvent):
        # clamp to terrain
        updateRay = ogre.Ray()
        updateRay.setOrigin (self.camera.getPosition() + ogre.Vector3(0.0, 10.0, 0.0))
        updateRay.setDirection (ogre.Vector3.NEGATIVE_UNIT_Y)
        self.raySceneQuery.ray = updateRay
        for queryResult in self.raySceneQuery.execute():
            if queryResult.worldFragment is not None:
                pos = self.camera.getPosition()
                self.camera.setPosition (pos.x, pos.y - queryResult.distance + 15.0, pos.z)
                break

        return sf.FrameListener.frameStarted(self, frameEvent)

if __name__ == '__main__':
    try:
        application = TerrainApplication()
        application.go()
    except ogre.Exception, e:
        print e
