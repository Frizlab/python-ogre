import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS
import SampleFramework as sf
import random
import time
       

class SSAOApp(sf.Application):
    def __init__ ( self):
        sf.Application.__init__(self)
        sf.Application.debugText = "E-Flashlight, F1-Toggle SSAO, L-Place light, SPACE-Place cube"

    # Override resource sources to add SSAO data resources
    def _setUpResources(self):
        sf.Application._setUpResources(self)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation(
            "../media/ssao", "FileSystem", 'SSAODemo') 
            
    def shadowSetup(self):
        # we'll be self shadowing
        self.sceneManager.setShadowTextureSelfShadow(True)

        # our caster material
        self.sceneManager.setShadowTextureCasterMaterial("shadow_caster")
        # note we have no "receiver".  all the "receivers" are integrated.

        # (each light needs a shadow texture)
        self.sceneManager.setShadowTextureCount(4)

        # the size, too (1024 looks good with 3x3 or more filtering)
        self.sceneManager.setShadowTextureSize(256)

        # float 16 here.  we need the R and G channels.
        # float 32 works a lot better with a low/none VSM epsilon (wait till the shaders)
        # but float 16 is good enough and supports bilinear filtering on a lot of cards
        # (we should use _GR, but OpenGL doesn't really like it for some reason)
        self.sceneManager.setShadowTexturePixelFormat(ogre.PF_FLOAT16_RGB)

        # big NONO to render back faces for VSM.  it doesn't need any biasing
        # so it's worthless (and rather problematic) to use the back face hack that
        # works so well for normal depth shadow mapping (you know, so you don't
        # get surface acne)
        self.sceneManager.setShadowCasterRenderBackFaces(False)

        numShadowRTTs = self.sceneManager.getShadowTextureCount()
        for i in range(0, numShadowRTTs):
        
            tex = self.sceneManager.getShadowTexture(i)
            vp = tex.getBuffer().getRenderTarget().getViewport(0)
            vp.setBackgroundColour(ogre.ColourValue(1, 1, 1, 1))
            vp.setClearEveryFrame(True)
            
        # enable integrated additive shadows
        # actually, since we render the shadow map ourselves, it doesn't
        # really matter whether they are additive or modulative
        # as long as they are integrated v(O_o)v
        self.sceneManager.setShadowTechnique(ogre.SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED)

        # and add the shader listener
        self.shadowCameraUpdater = ShadowListener()
        self.sceneManager.addListener(self.shadowCameraUpdater)

        self._createDebugShadowOverlay()
        
    def ssaoSetup(self):
        self.ssaoParamUpdater = SSAOListener(self.camera)
        vp = self.root.getAutoCreatedWindow().getViewport(0)
        self.ssao = ogre.CompositorManager.getSingleton().addCompositor(vp, "ssao")
        self.ssao.setEnabled(True)
        self.ssao.addListener(self.ssaoParamUpdater)
        
    def _createScene(self):
        self.shadowSetup()
        
        self.ssaoSetup()
        
        self.camera.setNearClipDistance(0.01)
        self.camera.setFarClipDistance(100)
        self.camera.setFOVy(ogre.Degree(70))
    
        rootNode = self.sceneManager.getRootSceneNode()
        node = rootNode.createChildSceneNode()

        # create some random objects
        NUM_X = 10
        NUM_Z = NUM_X
        self.createObjects(NUM_X, NUM_Z, "knot.mesh", "blue")#"ogre")
        self.createObjects(NUM_X, NUM_Z, "ogrehead.mesh", "green")#"ogre")
        self.createObjects(NUM_X, NUM_Z, "Prefab_Cube", "orange")#"ogre")
        self.createObjects(NUM_X, NUM_Z, "Prefab_Sphere", "red")#"ogre")

        ent = self.sceneManager.createEntity("level", "level.mesh")
        ent.setMaterialName("metal")
        node = rootNode.createChildSceneNode()
        node.attachObject(ent)
        node.setScale(ogre.Vector3(0.01, 0.01, 0.01)) # from cm to m
        # move it down to the center a bit
        node.setPosition(ogre.Vector3(0, -5, 0))

        # random position I found to be pretty
        self.camera.setPosition(ogre.Vector3(0.5, 2, 4))
        self.camera.setDirection(ogre.Vector3(0, 0, -1))

        # create our flash light
        light = self.sceneManager.createLight("flashLight")
        light.setDiffuseColour(ogre.ColourValue(1, 1, 1))
        light.setSpecularColour(ogre.ColourValue(1, 1, 1))
        light.setType(ogre.Light.LT_SPOTLIGHT)
        light.setSpotlightInnerAngle(ogre.Degree(25))
        light.setSpotlightOuterAngle(ogre.Degree(45))
        light.setAttenuation(30, 1, 1, 1) # meter range.  the others our shader ignores
        light.setDirection(ogre.Vector3(0, 0, -1))
        node = rootNode.createChildSceneNode("flashLightNode")
        node.attachObject(light)


    def createObjects(self, NUM_X, NUM_Z, mesh, material):
        # num_x and num_z are the X/Z segments
        rootNode = self.sceneManager.getRootSceneNode()

        for x in range(0, NUM_X):
            for z in range(0, NUM_Z):
                LIMIT = 10
                p = ogre.Vector3(
                    ogre.Math.RangeRandom(-LIMIT, LIMIT),
                    ogre.Math.RangeRandom(-LIMIT, LIMIT),
                    ogre.Math.RangeRandom(-LIMIT, LIMIT))

                ent = self.sceneManager.createEntity( \
                    mesh + str(x) + "_" + str(z), mesh)
                    
                ent.setMaterialName(material)

                node = rootNode.createChildSceneNode()
                node.attachObject(ent)
                node.setPosition(p)

                # scale from Ogre examples centimeters to meters
                node.setScale(ogre.Vector3(0.01, 0.01, 0.01))

    def _createDebugShadowOverlay(self):
        baseWhite = ogre.MaterialManager.getSingletonPtr().getByName("BaseWhite")
        DepthShadowTexture = baseWhite.clone("DepthShadowTexture0")
        textureUnit = DepthShadowTexture.getTechnique(0).getPass(0).createTextureUnitState()
        tex = self.sceneManager.getShadowTexture(0)
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
        self.mDebugOverlay.show()
        
    def _createFrameListener(self):
        self.frameListener = SSAOFrameListener(self.renderWindow, self.camera, self.sceneManager, self.ssao)
        self.root.addFrameListener(self.frameListener)


class ShadowListener(ogre.SceneManager.Listener):
    # this is a callback we'll be using to set up our shadow camera
    def shadowTextureCasterPreViewProj(self, light, camera, iteration):
    
        # basically, here we do some forceful camera near/far clip attenuation
        # yeah.  simplistic, but it works nicely.
        range = light.getAttenuationRange()
        camera.setNearClipDistance(0.01)
        camera.setFarClipDistance(range)
        # we just use a small near clip so that the light doesn't "miss" anything
        # that can shadow stuff.  and the far clip is equal to the lights' range.
        # (thus, if the light only covers 15 units of objects, it can only
        # shadow 15 units - the rest of it should be attenuated away, and not rendered)
        
    # these are pure virtual but we don't need them...  so just make them empty
    def shadowTexturesUpdated(self, numberOfShadowTextures): pass
    def shadowTextureReceiverPreViewProj(self, light, frustum): pass
    def preFindVisibleObjects(self, source, irs, v): pass
    def postFindVisibleObjects(self, source, irs, v): pass        
    def sortLightsAffectingFrustum(self, lightList): pass

    
class SSAOListener(ogre.CompositorInstance.Listener):
    def __init__(self, cam):
        ogre.CompositorInstance.Listener.__init__(self)
        self.cam = cam
        
    # this callback we will use to modify SSAO parameters
    def notifyMaterialRender(self, pass_id, mat):
        if (pass_id != 42): # not SSAO, return
            return

        # calculate the far-top-right corner in view-space
        farCorner = self.cam.getViewMatrix(True) * self.cam.getWorldSpaceCorners()[4]

        # get the pass
        matPass = mat.getBestTechnique().getPass(0)

        # get the vertex shader parameters
        params = matPass.getVertexProgramParameters()
        # set the camera's far-top-right corner
        if (params._findNamedConstantDefinition("farCorner")):
            params.setNamedConstant("farCorner", farCorner)

        # get the fragment shader parameters
        params = matPass.getFragmentProgramParameters()
        # set the projection matrix we need
        CLIP_SPACE_TO_IMAGE_SPACE = ogre.Matrix4(
            0.5,    0,    0,  0.5,
            0,   -0.5,    0,  0.5,
            0,      0,    1,    0,
            0,      0,    0,    1)
        if (params._findNamedConstantDefinition("ptMat")):
            params.setNamedConstant("ptMat", CLIP_SPACE_TO_IMAGE_SPACE * self.cam.getProjectionMatrixWithRSDepth())
        if (params._findNamedConstantDefinition("far")):
            params.setNamedConstant("far", self.cam.getFarClipDistance())

        
class SSAOFrameListener(sf.FrameListener):
    def __init__(self, rw, cam, sm, ssao):
        sf.FrameListener.__init__(self, rw, cam)
        self.sceneManager = sm
        self.ssao = ssao
        self.flashLight = self.sceneManager.getLight("flashLight")
        self.moveSpeed = 2.5
        self.objectIndex = 0
        self.toggleTimer = 0
    
    def _processUnbufferedKeyInput(self, evt):
        sf.FrameListener._processUnbufferedKeyInput(self, evt)
        self.toggleTimer -= evt.timeSinceLastFrame
        
        # place a light if we pressed space
        if self.Keyboard.isKeyDown(OIS.KC_L) and self.toggleTimer <= 0:
            self.objectIndex += 1
            name = "light" + str(self.objectIndex)
            light = self.sceneManager.createLight(name)
            # random colour
            r = ogre.Math.UnitRandom()
            g = ogre.Math.UnitRandom()
            b = ogre.Math.UnitRandom()
            # make sure the light is not too dark
            if (ogre.Vector3(r, g, b).length() < 0.75):
                # if so, multiply by 2
                r *= 2
                g *= 2
                b *= 2
                
            light.setDiffuseColour(ogre.ColourValue().White) #ogre.ColourValue(r, g, b))
            light.setSpecularColour(ogre.ColourValue().White) #ogre.ColourValue(r, g, b))

            light.setDirection(ogre.Vector3(0, 0, -1))

            light.setType(ogre.Light.LT_SPOTLIGHT)
            light.setSpotlightInnerAngle(ogre.Degree(70))
            light.setSpotlightOuterAngle(ogre.Degree(90))

            light.setAttenuation(30, 1, 1, 1) # meter range.  the others our shader ignores

            node = self.sceneManager.getRootSceneNode().createChildSceneNode(name + "Node")
            node.setPosition(self.camera.getDerivedPosition())
            node.setOrientation(self.camera.getDerivedOrientation())
            node.attachObject(light)
            
            self.toggleTimer = 0.5

        # toggle the flash light if we press E
        if (self.Keyboard.isKeyDown(OIS.KC_E)) and self.toggleTimer <= 0:
            if self.flashLight.getVisible():
                self.flashLight.setVisible(False)
            else:
                self.flashLight.setVisible(True)
            
            self.toggleTimer = 0.5

        if (self.Keyboard.isKeyDown(OIS.KC_SPACE)) and self.toggleTimer <= 0:
            self.objectIndex += 1
            mesh = "Prefab_Cube"
            ent = self.sceneManager.createEntity(mesh + str(self.objectIndex), mesh)

            material = "orange" #"ogre"
            ent.setMaterialName(material)

            node = self.sceneManager.getRootSceneNode().createChildSceneNode()
            node.attachObject(ent)
            node.setPosition(self.camera.getDerivedPosition())

            # scale from Ogre examples centimeters to meters
            node.setScale(ogre.Vector3(0.01, 0.01, 0.01))
            self.toggleTimer = 0.5

            
        if (self.Keyboard.isKeyDown(OIS.KC_F1)) and self.toggleTimer <= 0:
            if self.ssao.getEnabled():
                self.ssao.setEnabled(False)
            else:
                self.ssao.setEnabled(True)
                
            self.toggleTimer = 0.5
        
        if self.Keyboard.isKeyDown(OIS.KC_ESCAPE) or self.Keyboard.isKeyDown(OIS.KC_Q):
            return False
        else:
            return True
    
    def frameStarted(self, evt):
        self.syncFlashLight()
        return sf.FrameListener.frameStarted(self, evt)

    def syncFlashLight(self):
        node = self.sceneManager.getSceneNode("flashLightNode")
        node.setPosition(self.camera.getDerivedPosition())
        node.setOrientation(self.camera.getDerivedOrientation())
        # this is a simply "offset", to the bottom right, as if
        # the camera is holding the light in its right hand
        offset = ogre.Vector3(0.225, -0.3, -0.3)
        # local space means that it's relative to the node
        # itself, not in world units
        node.translate(offset, ogre.Node.TS_LOCAL)
        
if __name__ == '__main__':
    import exceptions,sys
    try:
        application = SSAOApp()
        application.go()
    except ogre.OgreException, e:
        print e
        
        

