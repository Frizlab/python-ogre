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
import random as random

import ogre.addons.hydrax as Hydrax
import ogre.io.OIS as OIS

_def_SkyBoxNum = 3
_def_PGComplexity = 256

mSkyBoxes = [ "Sky/ClubTropicana",
             "Sky/EarlyMorning",
             "Sky/Clouds"]

mSunPosition = [ ogre.Vector3(0,10000,0),
                 ogre.Vector3(0,10000,90000),
                 ogre.Vector3(0,10000,0)
                 ]
                 
mSunColor = [ogre.Vector3(1, 0.9, 0.6),
              ogre.Vector3(1,0.6,0.4),
              ogre.Vector3(0.45,0.45,0.45)]

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
        self.raySceneQuery = self.sceneManager.createRayQuery(ogre.Ray(self.camera.getPosition(),
                                                                    ogre.Vector3().NEGATIVE_UNIT_Y))

    def frameStarted(self, e):
        global mCurrentSkyBox
        if sf.FrameListener.frameStarted(self, e) == False:
            return False
            
        # Check camera height
        # this is the code from the Hydrax demo -- stops you going too low...
        raySceneQuery = self.sceneManager.createRayQuery(ogre.Ray(self.camera.getPosition() + ogre.Vector3(0,1000000,0),
                              ogre.Vector3().NEGATIVE_UNIT_Y));
        qryResult = raySceneQuery.execute()
        for i in qryResult:
            if (i.worldFragment):
              if (self.camera.getPosition().y < i.worldFragment.singleIntersection.y + 30):
                    self.camera.setPosition(self.camera.getPosition().x, 
                                     i.worldFragment.singleIntersection.y + 30, 
                                     self.camera.getPosition().z);

        ## this code will clamp the camera at a fixed position above the ground
#         updateRay = ogre.Ray()
#         updateRay.setOrigin (self.camera.getPosition() + ogre.Vector3(0.0, 10.0, 0.0))
#         updateRay.setDirection (ogre.Vector3().NEGATIVE_UNIT_Y)
#         self.raySceneQuery.Ray = updateRay
#         for queryResult in self.raySceneQuery.execute():
#             if queryResult.worldFragment:
#                 pos = self.camera.getPosition()
#                 self.camera.setPosition (pos.x, pos.y - queryResult.distance + 30.0, pos.z)
#                 break
                                     
                                                 
        self.app.hydrax.update(e.timeSinceLastFrame)
        if self._isToggleKeyDown(OIS.KC_M, 0.5):
            mCurrentSkyBox+=1

            if mCurrentSkyBox > (_def_SkyBoxNum-1):
                mCurrentSkyBox = 0

            self.changeSkyBox()
          
        return True

    def changeSkyBox( self ):
        # Change skybox
        self.sceneManager.setSkyBox(True, mSkyBoxes[mCurrentSkyBox], 99999*3, True)

        # Update Hydrax sun position and colour
        self.app.hydrax.setSunPosition(mSunPosition[mCurrentSkyBox])
        self.app.hydrax.setSunColor(mSunColor[mCurrentSkyBox])

        # Update light 0 light position and colour
        self.sceneManager.getLight("Light0").setPosition(mSunPosition[mCurrentSkyBox])
        self.sceneManager.getLight("Light0").setSpecularColour(mSunColor[mCurrentSkyBox].x,mSunColor[mCurrentSkyBox].y,mSunColor[mCurrentSkyBox].z)


        
### Just to locate palmiers with a pseudo-random algoritm

seed_ = 801;
def rnd_1(min,  max):
    global seed_
    seed_ += ogre.Math.PI*2.8574 + seed_*(0.3424 - 0.12434 + 0.452345)
    if (seed_ > 10000000000): seed_ -= 10000000000
    i= float(ogre.Math.Sin(ogre.Radian(seed_)))
    k = ((max-min)*abs(i) + min)    # this is a float !!!
    return (k)
    
def rnd_(min,  max):
    return random.uniform (min, max )
    
def createPalms(mSceneMgr):
    NumberOfPalms = 14;

    mPalmsSceneNode = mSceneMgr.getRootSceneNode().createChildSceneNode();
    for k in range (NumberOfPalms):
        RandomPos = ogre.Vector3(rnd_(500,2500),0, rnd_(500,2500))
        raySceneQuery = mSceneMgr.createRayQuery(ogre.Ray(RandomPos + ogre.Vector3(0,1000000,0), ogre.Vector3().NEGATIVE_UNIT_Y));
        qryResult = raySceneQuery.execute()
        for i in qryResult:
            if (i.worldFragment):
                if (i.worldFragment.singleIntersection.y>105 or i.worldFragment.singleIntersection.y<20):
                    k-=1
                    continue
                RandomPos.y = i.worldFragment.singleIntersection.y
            else:
                k-=1
                continue

            mPalmEnt = mSceneMgr.createEntity("Palm"+str(k), "Palm.mesh")
            mPalmSN = mPalmsSceneNode.createChildSceneNode()
    
            mPalmSN.rotate(ogre.Vector3(-1,0,rnd_(-0.3,0.3)), ogre.Degree(90));
            mPalmSN.attachObject(mPalmEnt)
            Scale = rnd_(50,75)
            mPalmSN.scale(Scale,Scale,Scale)
            mPalmSN.setPosition(RandomPos)
        
        
class HydraxApplication(sf.Application):
    def _chooseSceneManager(self):
        # Create the SceneManager
        self.sceneManager = self.root.createSceneManager("TerrainSceneManager")
        
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.setAmbientLight ( ogre.ColourValue(1.0,1.0,1.0 ))
        
        # Create the SkyBox
        sceneManager.setSkyBox(True, mSkyBoxes[mCurrentSkyBox], 99999*3, True)

        # Set some camera params
        camera.setFarClipDistance(99999*6)
        camera.setPosition(312.902,206.419,1524.02)
        camera.setOrientation(ogre.Quaternion(0.998, -0.0121, -0.0608, -0.00074))

        # Light
        mLight = sceneManager.createLight("Light0")
        mLight.setPosition(mSunPosition[mCurrentSkyBox])
        mLight.setDiffuseColour(1, 1, 1)
        mLight.setSpecularColour(mSunColor[mCurrentSkyBox].x,
                                  mSunColor[mCurrentSkyBox].y,
                                  mSunColor[mCurrentSkyBox].z)
        
        
        # Create Hydrax object
        self.hydrax = Hydrax.Hydrax(sceneManager, camera, self.renderWindow.getViewport(0))

        # Create our projected grid module  
        self.noise=  Hydrax.Perlin()  ## Default options (8, 0.085, 0.49, 1.4, 1.27f) 
        self.Module = Hydrax.ProjectedGrid(# Hydrax parent pointer
                                                self.hydrax,
                                                # Noise module
                                                self.noise,
                                                # Base plane
                                                ogre.Plane(ogre.Vector3(0,1,0), ogre.Vector3(0,0,0)),
                                                Hydrax.MaterialManager.NM_VERTEX,
                                                # Projected grid options (Can be updated each frame . setOptions(...))
                                                Hydrax.ProjectedGrid.Options(264 ))

        # Set our module
        self.hydrax.setModule(self.Module)

        # Load all parameters from config file
        # Remarks: The config file must be in Hydrax resource group.
        # All parameters can be set/updated directly by code(Like previous versions),
        # but due to the high number of customizable parameters, Hydrax 0.4 allows save/load config files.
        self.hydrax.loadCfg("HydraxDemo.hdx");

        # Create water
        self.hydrax.create()
        
        # Load island
        sceneManager.setWorldGeometry("Island.cfg")
        
        # Add our hydrax depth technique to island material
        # (Because the terrain mesh is not an ogre.Entity)
        self.hydrax.getMaterialManager().addDepthTechnique(
            ogre.MaterialManager.getSingleton().getByName("Island").createTechnique())

        # Create palmiers
        createPalms(sceneManager );


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
