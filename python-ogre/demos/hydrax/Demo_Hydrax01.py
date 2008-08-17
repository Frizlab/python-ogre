# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-ogre
# For the latest info, see http:#python-ogre.org/
#
# It is likely based on original code from OGRE and/or Pyogre
# For the latest info, see http:#www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import sys
sys.path.insert(0,'..')
import PythonOgreConfig
import ogre.renderer.OGRE as ogre
import SampleFramework as sf

import ogre.addons.hydrax as Hydrax

_def_SkyBoxNum = 5
_def_PGComplexity = 256

mSkyBoxes = [ "Sky/ClubTropicana",
             "Sky/Stormy",
             "Sky/EarlyMorning",
             "Sky/Evening",
             "Sky/Clouds"]

mSunPosition = [ogre.Vector3(0,10000,-90000),
                 ogre.Vector3(0,10000,-90000)/2.5,
                 ogre.Vector3(13000,0,120000)/3,
                 ogre.Vector3(-50000,-5000,50000),
                 ogre.Vector3(0,0,0)]
                 
mSunColor = [ogre.Vector3(1, 0.9, 0.6)/5,
              ogre.Vector3(0.75, 0.65, 0.45)/2,
              ogre.Vector3(1,0.6,0.4),
              ogre.Vector3(1,0.4,0.1),
              ogre.Vector3(0,0,0)]

# print dir (Hydrax)
# print dir( Hydrax.Module)  
# print dir ( Hydrax.Noise)            
mPGOptions = [Hydrax.ProjectedGrid.Options(_def_PGComplexity),
              Hydrax.ProjectedGrid.Options(_def_PGComplexity, 45.5, 7.0, False),
              Hydrax.ProjectedGrid.Options(_def_PGComplexity, 32.5, 7.0, False),
              Hydrax.ProjectedGrid.Options(_def_PGComplexity, 32.5, 7.0, False),
              Hydrax.ProjectedGrid.Options(_def_PGComplexity, 20.0, 7.0, False)]

mPerlinOptions = [ Hydrax.Perlin.Options(8, 0.085, 0.49, 1.4, 1.27),
                  Hydrax.Perlin.Options(8, 0.085, 0.49, 1.4, 1.27),
                  Hydrax.Perlin.Options(8, 0.085, 0.49, 1.4, 1.27),
                  Hydrax.Perlin.Options(8, 0.075, 0.49, 1.4, 1.27),
                  Hydrax.Perlin.Options(8, 0.085, 0.49, 1.4, 1.27)]

mCurrentSkyBox = 0

## Event handler to add ability to alter subdivision
class HydraxListener(sf.FrameListener):
    def __init__(self, rw, cam, sm , app):
        sf.FrameListener.__init__(self, rw, cam)
        self.sceneManager=sm
        self.renderWindow =rw
        self.camera=cam
        self.app = app
        self.mKeyBuffer=-1

    def frameStarted(self, e):
        if sf.FrameListener.frameStarted(self, e) == False:
            return False
        self.app.hydrax.update(e.timeSinceLastFrame)
# 
#         mKeyboard->capture()
# 
#         if (mKeyboard->isKeyDown(OIS::KC_M) && mKeyBuffer < 0)
#         {
#           mCurrentSkyBox++
# 
#           if(mCurrentSkyBox > (_def_SkyBoxNum-1))
#           {
#               mCurrentSkyBox = 0
#           }
# 
#           changeSkyBox()
# 
#           mKeyBuffer = 0.5f
#       }
# 
#       mKeyBuffer -= e.timeSinceLastFrame
# 
        return True

    def changeSkyBox( self ):
        # Change skybox
        self.sceneManager.setSkyBox(true, mSkyBoxes[mCurrentSkyBox], 99999*3, true)

        # Update Hydrax sun position and colour
        app.hydrax.setSunPosition(mSunPosition[mCurrentSkyBox])
        app.hydrax.setSunColor(mSunColor[mCurrentSkyBox])

        # Update light 0 light position and colour
        self.sceneManager.getLight("Light0").setPosition(mSunPosition[mCurrentSkyBox])
        self.sceneManager.getLight("Light0").setSpecularColour(mSunColor[mCurrentSkyBox].x,mSunColor[mCurrentSkyBox].y,mSunColor[mCurrentSkyBox].z)

        # Update perlin noise options
        app.hydrax.getModule().getNoise().setOptions(mPerlinOptions[mCurrentSkyBox])

        # Update projected grid options
        app.hydrax.getModule().setOptions(mPGOptions[mCurrentSkyBox])

#         ogre.LogManager.getSingleton().logMessage("Skybox " + mSkyBoxes[mCurrentSkyBox] + " selected. ("+Ogre::StringConverter::toString(mCurrentSkyBox+1)+"/"+Ogre::StringConverter::toString(_def_SkyBoxNum)+")")
                


class HydraxApplication(sf.Application):
    
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        # Create the SkyBox
        sceneManager.setSkyBox(True, mSkyBoxes[mCurrentSkyBox], 99999*3, True)

        # Set some camera params
        camera.setFarClipDistance(99999*6)
        camera.setPosition(4897.61,76.8614,4709.9)
        camera.setOrientation(ogre.Quaternion(0.487431, -0.0391184, 0.869485, 0.0697797))

        # Create Hydrax object
        self.hydrax = Hydrax.Hydrax(sceneManager, camera)

        # Set RTT textures quality
        self.hydrax.setRttOptions(
                   Hydrax.RttOptions(# Reflection tex quality
                                      Hydrax.TEX_QUA_1024,
                                      # Refraction tex quality
                                      Hydrax.TEX_QUA_1024,
                                      # Depth tex quality
                                      Hydrax.TEX_QUA_1024))

        # Set components
        self.hydrax.setComponents(  Hydrax.HydraxComponent( Hydrax.HYDRAX_COMPONENT_SUN    |
                                     Hydrax.HYDRAX_COMPONENT_FOAM   |
                                     Hydrax.HYDRAX_COMPONENT_DEPTH  |
                                     Hydrax.HYDRAX_COMPONENT_SMOOTH |
                                     Hydrax.HYDRAX_COMPONENT_CAUSTICS ))

        # Create our projected grid module  
        self.noise=  Hydrax.Perlin()  ## Default options (8, 0.085, 0.49, 1.4, 1.27f) 
        self.Module = Hydrax.ProjectedGrid(# Hydrax parent pointer
                                                self.hydrax,
                                                # Noise module
                                                self.noise,
                                                # Base plane
                                                ogre.Plane(ogre.Vector3(0,1,0), ogre.Vector3(0,0,0)),
                                                # Projected grid options (Can be updated each frame . setOptions(...))
                                                Hydrax.ProjectedGrid.Options(_def_PGComplexity ))

        # Set our module
        self.hydrax.setModule(self.Module)

        # Set our shader mode
        self.hydrax.setShaderMode(Hydrax.MaterialManager.SM_HLSL)

        # Create water
        self.hydrax.create()

        # Adjust some options
        self.hydrax.setPosition(ogre.Vector3(0,0,0))
        self.hydrax.setPlanesError(37.5)
        self.hydrax.setDepthLimit(110)
        self.hydrax.setSunPosition(mSunPosition[mCurrentSkyBox])
        self.hydrax.setSunColor(mSunColor[mCurrentSkyBox])
        self.hydrax.setNormalDistortion(0.025)
        self.hydrax.setDepthColor(ogre.Vector3(0.04,0.135,0.185))
        self.hydrax.setSmoothPower(5)
        self.hydrax.setCausticsScale(12)
        self.hydrax.setGlobalTransparency(0.1)
        self.hydrax.setFullReflectionDistance(99999997952.0)
        self.hydrax.setGlobalTransparency(0)
        self.hydrax.setPolygonMode(0)

        # Lights
        sceneManager.setAmbientLight(ogre.ColourValue(1, 1, 1))
        Light = sceneManager.createLight("Light0")
        Light.setPosition(mSunPosition[mCurrentSkyBox])
        Light.setDiffuseColour(1, 1, 1)
        Light.setSpecularColour(mSunColor[mCurrentSkyBox].x,mSunColor[mCurrentSkyBox].y,mSunColor[mCurrentSkyBox].z)

        # Island
        IslandEntity = sceneManager.createEntity("Island", "Island.mesh")
        IslandEntity.setMaterialName("Examples/OffsetMapping/Specular")
        IslandSceneNode = sceneManager.getRootSceneNode().createChildSceneNode(ogre.Vector3(5000, -175, 5000))
        IslandSceneNode.setScale(23, 13.5, 23)
        IslandSceneNode.attachObject(IslandEntity)

    def _createFrameListener(self):
        self.frameListener = HydraxListener(self.renderWindow, self.camera, self.sceneManager, self)
        self.root.addFrameListener(self.frameListener)
        self.frameListener.showDebugOverlay( True )
       
                
if __name__ == '__main__':
    try:
        application = HydraxApplication()
        application.go()
    except ogre.OgreException, e:
        print e
