# this code is in the public domain
# http://www.ogre3d.org/wiki/index.php/PyOgre_Beginner_Tutorial_2
import Ogre as ogre
import SampleFramework

def AndyTest():
    mm=ogre.ResourceManager()

class TutorialApplication(SampleFramework.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        sceneManager.ambientLight = (0, 0, 0)
        sceneManager.shadowTechnique = ogre.SHADOWTYPE_STENCIL_ADDITIVE

        ent = sceneManager.createEntity("Ninja", "ninja.mesh")
        ent.castShadows = True     # this is not actually needed
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(ent)

        plane = ogre.Plane(ogre.Vector3(0, 1, 0), 0)    #vector 
        
        ogre.MeshManager.getSingleton().createPlane('ground', ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME,
                      plane, 1500.0, 1500.0 
                      ,20, 20, True, 1, 
                      5.0, 5.0, ogre.Vector3(0, 0, 1),
                      ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, 
                      True,True
                      )
                       

        ent = sceneManager.createEntity("GroundEntity", "ground")
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(ent)
        
        ent.setMaterialName("Examples/Rockwall")
        ent.castShadows = False

        # point light
        light = self.sceneManager.createLight("Light1")
        light.type = ogre.Light.LT_POINT
        light.position = (0, 150, 250)
        light.diffuseColour = (1.0, 0.0, 0.0)
        light.specularColour = (1.0, 0.0, 0.0)

        # directional light
        light = sceneManager.createLight("Light3")
        light.type = ogre.Light.LT_DIRECTIONAL
        light.diffuseColour = (.25, .25, 0)
        light.specularColour = (.25, .25, 0)
        light.direction = (0, -1, 1)

        # spotlight
        light = sceneManager.createLight("Light2")
        light.type = ogre.Light.LT_SPOTLIGHT
        light.diffuseColour = (0, 0, 1.0)
        light.specularColour = (0, 0, 1.0)
        light.direction = (-1, -1, 0)
        light.position = (300, 300, 0)
        dd=ogre.Radian(50.0)
        dd1=ogre.Radian(35.0)
        print dd, dd1
##        light.setSpotlightRange(dd, dd1, 1.0)
    
#    def _createCamera(self):
#        self.camera = self.sceneManager.createCamera("PlayerCam")
#        print self.camera
#        self.camera.setPosition (0, 10, 500)
#        print self.camera.getPosition
#        self.camera.lookAt(0.0, 0.0, 0.0)

#        self.camera.setNearClipDistance (5)

    
#    def _createViewports(self):
#        vp = self.renderWindow.addViewport(self.camera)
#        vp.backgroundColour = (0, 0, 0)
        
#        self.camera.setAspectRatio( vp.getActualWidth() / vp.getActualHeight() )

if __name__ == '__main__':
#    AndyTest()
    ta = TutorialApplication()
    ta.go()
