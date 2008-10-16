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

# NOTE:
# This demo includes meshes and shaders by lf3thn4d
# see below for more details:
# http://www.ogre3d.org/phpBB2/viewtopic.php?t=41801&highlight=pssm

# Authors Note:
# This demo seems to perform much better on ATI cards than NVIDIA!

import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS
import SampleFramework as sf

class PSSMDemoFrameListener(sf.FrameListener):
    def __init__(self, rw, cam, light):
        sf.FrameListener.__init__(self, rw, cam)
        self.light = light
        self.angle = ogre.Math.PI / 2
        self.radius = 1000
        
    def frameStarted(self, evt):
        self.angle += evt.timeSinceLastFrame
        xpos = self.radius * ogre.Math.Sin( ogre.Degree( self.angle ) * 5.0 )
        zpos = self.radius * ogre.Math.Cos( ogre.Degree( self.angle ) * 5.0 )
        pos = ogre.Vector3( xpos, 400, zpos )
        self.light.setPosition( pos )
        dirvec = -self.light.getPosition()
        dirvec.normalise()
        self.light.setDirection( dirvec )
        
        return sf.FrameListener.frameStarted(self, evt)



class ApplicationPSSM(sf.Application):
    
    def __del__(self):
        del self.pssmSetup
        
    def _createScene(self):
        self.sceneManager.setShadowTechnique(ogre.SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED)
        
        # 3 textures per directional light
        self.sceneManager.setShadowTextureCountPerLightType(ogre.Light.LT_DIRECTIONAL, 3);
        self.sceneManager.setShadowTextureSettings(1024, 3, ogre.PF_FLOAT32_R);
        self.sceneManager.setShadowTextureSelfShadow(True);
        # Set up caster material - this is just a standard depth/shadow map caster
        self.sceneManager.setShadowTextureCasterMaterial("shadow_caster");
        self.camera.setNearClipDistance(1.0)
        self.camera.setFarClipDistance(1000.0)
        
        # shadow camera setup
        self.pssmSetup = ogre.PSSMShadowCameraSetup()
        self.pssmSetup.calculateSplitPoints(3, self.camera.getNearClipDistance(), self.camera.getFarClipDistance())
        self.pssmSetup.setSplitPadding(5)
        self.pssmSetup.setOptimalAdjustFactor(0, 2)
        self.pssmSetup.setOptimalAdjustFactor(1, 1);
        self.pssmSetup.setOptimalAdjustFactor(2, 0.5)
        
        self.sceneManager.setShadowCameraSetup(self.pssmSetup)
        
        
        # setup the sun light.
        self.sceneManager.setAmbientLight(ogre.ColourValue(0.4, 0.4, 0.5));
        self.mSunLight = self.sceneManager.createLight("_SunLight")
        self.mSunLight.setType(ogre.Light.LT_DIRECTIONAL)
        self.mSunLight.setDirection(ogre.Vector3(-1, -1, -1))
        self.mSunLight.setDiffuseColour(ogre.ColourValue(1, 1, 1))
        self.mSunLight.setSpecularColour(ogre.ColourValue(1,1,1))
        
        # Create the Mesh - 
        # Note Mesh and Shaders courtesy of lf3thn4d
        cityNode = self.sceneManager.getRootSceneNode().createChildSceneNode()
        cityEntity = self.sceneManager.createEntity("City", "city.mesh")
        cityNode.attachObject(cityEntity)
        cityNode.setScale(10,10,10)

        splitPoints = ogre.Vector4(0,0,0,0)
        splitPointList = self.pssmSetup.getSplitPoints();
        mat = ogre.MaterialManager.getSingleton().getByName("city")
        splitPoints.y = splitPointList[1]
        splitPoints.z = splitPointList[2]
        mat.getTechnique(0).getPass(1).getFragmentProgramParameters().setNamedConstant("pssmSplitPoints", splitPoints)
        self._createDebugShadowOverlays()

    def _createDebugShadowOverlays(self):
        baseWhite = ogre.MaterialManager.getSingletonPtr().getByName("BaseWhite")
        DepthShadowTexture = baseWhite.clone("DepthShadowTexture0")
        textureUnit = DepthShadowTexture.getTechnique(0).getPass(0).createTextureUnitState()
        tex = self.sceneManager.getShadowTexture(0)
        textureUnit.setTextureName(tex.getName())
        
        DepthShadowTexture = baseWhite.clone("DepthShadowTexture1")
        textureUnit = DepthShadowTexture.getTechnique(0).getPass(0).createTextureUnitState()
        tex = self.sceneManager.getShadowTexture(1)
        textureUnit.setTextureName(tex.getName())
        
        DepthShadowTexture = baseWhite.clone("DepthShadowTexture2")
        textureUnit = DepthShadowTexture.getTechnique(0).getPass(0).createTextureUnitState()
        tex = self.sceneManager.getShadowTexture(2)
        textureUnit.setTextureName(tex.getName())
        
        overlayManager = ogre.OverlayManager.getSingleton()
        # Create an overlay
        self.mDebugOverlay = overlayManager.create("OverlayName")
        
        # Create a panel
        panel = overlayManager.createOverlayElement("Panel", "PanelName0")
        panel.setMetricsMode(ogre.GMM_PIXELS)
        panel.setPosition(10, 10)
        panel.setDimensions(100, 100)
        panel.setMaterialName("DepthShadowTexture0") # Optional background material
        self.mDebugOverlay.add2D(panel)
        panel = overlayManager.createOverlayElement("Panel", "PanelName1")
        panel.setMetricsMode(ogre.GMM_PIXELS)
        panel.setPosition(120, 10)
        panel.setDimensions(100, 100)
        panel.setMaterialName("DepthShadowTexture1") # Optional background material
        self.mDebugOverlay.add2D(panel)
        panel = overlayManager.createOverlayElement("Panel", "PanelName2")
        panel.setMetricsMode(ogre.GMM_PIXELS)
        panel.setPosition(230, 10)
        panel.setDimensions(100, 100)
        panel.setMaterialName("DepthShadowTexture2") # Optional background material
        self.mDebugOverlay.add2D(panel)
        self.mDebugOverlay.show()
        
    def _createFrameListener(self):
        self.frameListener = PSSMDemoFrameListener(self.renderWindow, self.camera, self.mSunLight)
        self.root.addFrameListener(self.frameListener)
        
        
        
if __name__ == '__main__':
    import exceptions,sys
    try:
        application = ApplicationPSSM()
        application.go()
    except ogre.OgreException, e:
        print e
        
        

