# NO_UNITTEST
# sample example
import os

# actually, you can just include specifics here
# instead of all of ogre, but this makes it easier
# on me and this example
import ogre.renderer.OGRE as ogre
# OIS is a bit lighter, though.  just include all of it anyways
import ogre.io.OIS as OIS

# for srand()
import random
# for time()
import time

# I really don't suggest you keep so many globals and what-not like this
# but this is a short example, and we'll just use a central "Manager" system that manages EVERYTHING
class Mgr(ogre.WindowEventListener):

    # a config file we'll use to retrieve some settings
    cfgFile = ogre.ConfigFile()

    # our root
    root = None
    
    # simples stuff here
    sceneMgr = None
    cam = None
    vp = None
    window = None
    # note we only used one camera & viewport, simplistic
    timer = None  # we'll just use the root's timer
    # otherwise you can make more complex timer systems
    
    # just incase
    rsys = None
    
    # OIS stuff O_o
    input = None
    keys = None
    mouse = None
    
    # ssao instance
    ssao = None
    
    # local "continue?" variable
    running = True

    # this tells us if the window closed (or alt-f4, for example)
    def windowClosed(self, window):
    
        # stop running.  we don't compare any windows
        # since we only make 1 window anyways
        running = False
        
        
    def __init__(self):
        ogre.WindowEventListener.__init__(self)
        
        
    # stupid constructor.  don't base your code on this code )
    # but since I wanted to make it easy, everything is done
    # in seperate "init" functions.  so this just makes everything
    # default
        
mgr = Mgr()

class shadowListener(ogre.SceneManager.Listener):

    # this is a callback we'll be using to set up our shadow camera
    def shadowTextureCasterPreViewProj(self, light, camera, iteration):
    
        # basically, here we do some forceful camera near/far clip attenuation
        # yeah.  simplistic, but it works nicely.  this is the function I was talking
        # about you ignoring above in the Mgr declaration.
        range = light.getAttenuationRange()
        camera.setNearClipDistance(0.01)
        camera.setFarClipDistance(range)
        # we just use a small near clip so that the light doesn't "miss" anything
        # that can shadow stuff.  and the far clip is equal to the lights' range.
        # (thus, if the light only covers 15 units of objects, it can only
        # shadow 15 units - the rest of it should be attenuated away, and not rendered)
        
        
    # these are pure virtual but we don't need them...  so just make them empty
    # otherwise we get "cannot declare of type Mgr due to missing abstract
    # functions" and so on
    def shadowTexturesUpdated(self, numberOfShadowTextures): pass
    def shadowTextureReceiverPreViewProj(self, light, frustum): pass
    def preFindVisibleObjects(self, source, irs, v): pass
    def postFindVisibleObjects(self, source, irs, v): pass        
    def sortLightsAffectingFrustum(self, lightList): pass
shadowCameraUpdater = shadowListener()

class ssaoListener(ogre.CompositorInstance.Listener):
    # this callback we will use to modify SSAO parameters
    def notifyMaterialRender(self, pass_id, mat):
    
        if (pass_id != 42): # not SSAO, return
            return

        # this is the camera you're using
        cam = mgr.cam

        # calculate the far-top-right corner in view-space
        farCorner = cam.getViewMatrix(True) * cam.getWorldSpaceCorners()[4]

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
            params.setNamedConstant("ptMat", CLIP_SPACE_TO_IMAGE_SPACE * cam.getProjectionMatrixWithRSDepth())
        if (params._findNamedConstantDefinition("far")):
            params.setNamedConstant("far", cam.getFarClipDistance())
            
ssaoParamUpdater = ssaoListener()

# simple "choosing" of the renderer
D3D = 1
OGL = 0
# initialize Ogre and the renderer
def initRenderer(renderer = D3D):   # default to D3D

    # create our Root
    # note the ""s.  we supply our own plugins and config options
    mgr.root = ogre.Root("", "") # default "Ogre.log"

    # load the D3D or OGL render system based on the renderer
    # note how this doesn't require an installation of both OGL
    # *and* D3D - only the ones to be used.  The demoes, on the other
    # hand, seem to load both at once -.-
    
    _DEBUG = False
    if renderer == D3D:
        if _DEBUG:
            mgr.root.loadPlugin(os.path.abspath("../../plugins/RenderSystem_Direct3D9_d"))
        else:
            mgr.root.loadPlugin(os.path.abspath( "../../plugins/RenderSystem_Direct3D9"))
        
        
        
    elif renderer == OGL:
        if _DEBUG:
            mgr.root.loadPlugin(os.path.abspath("../../plugins/RenderSystem_GL_d"))
        else:
            mgr.root.loadPlugin(os.path.abspath("../../plugins/RenderSystem_GL"))

        
        
        
    # technically these are all the plugins we *need*.
    # but since we're going to write the soft shadowing shaders
    # in CG, might as well use the CG program manager plugin
    # here you can also stick in stuff like the particleFX plugin and what-not
    if _DEBUG:
        mgr.root.loadPlugin(os.path.abspath("../../plugins/Plugin_CgProgramManager_d"))
    else:
        mgr.root.loadPlugin(os.path.abspath("../../plugins/Plugin_CgProgramManager"))

        
    # get the available render systems
    # technically, should be only one since we only load one plugin
    # much better than comparing the string names like some people do it
    renderers = mgr.root.getAvailableRenderers()

    # use the first one
    # (the '*' is because begin() returns an iterator, like the STL)
    mgr.rsys = renderers[0]
    # tell the root to use this render system
    mgr.root.setRenderSystem(mgr.rsys)

    # now we can initialise the root
    # though pass False since we want our own window creation
    mgr.root.initialise(False)

    # OK, this is really badly hard coded.  but its an example
    # the disorder engine, for example, reads a lua script for these values
    # (the disorder engine is what's under Portalized)
    # (*cough*don'tworryifyou'veneverheardofit,it'smyproject*cough*)
    miscParams = {}
    # retrive anti aliasing from the config file
    miscParams["FSAA"] = mgr.cfgFile.getSetting("FSAA", "renderer")
    miscParams["vsync"] = mgr.cfgFile.getSetting("vsync", "renderer")

    # get these settings from the cfg file, too
    width = int( mgr.cfgFile.getSetting("width", "renderer") )
    height = int( mgr.cfgFile.getSetting("height", "renderer") )
    fullscreen = int( mgr.cfgFile.getSetting("fullscreen", "renderer") )
    if fullscreen == 1:
        fullscreen = True
    else:
        fullscreen = False

        
        
    # now we can just make a window, it's that easy
    mgr.window = mgr.root.createRenderWindow("Python-Ogre SSAO Compositor and Soft Shadows", width, height, fullscreen) #, miscParams)

    # now, we can, for example, set the default texture filtering
    ogre.MaterialManager.getSingleton().setDefaultTextureFiltering(ogre.TFO_ANISOTROPIC)

    # tell the window it's active (duh, rofl)
    mgr.window.setActive(True)
    # make it NOT auto update on renderOneFrame()
    #mgr.window.setAutoUpdated(False)

    # time to prepare the scene
    mgr.sceneMgr = mgr.root.createSceneManager(ogre.ST_GENERIC) # just generic for the example

    # "some" ambient light
    mgr.sceneMgr.setAmbientLight(ogre.ColourValue(0.8, 0.8, 0.8))

    # make the camera
    mgr.cam = mgr.sceneMgr.createCamera("Mgr.cam")
    mgr.cam.setNearClipDistance(0.01)
    mgr.cam.setFarClipDistance(100)

    # add a viewport to the window and this camera
    # (covers up the whole window)
    mgr.vp = mgr.window.addViewport(mgr.cam)
    # black background
    mgr.vp.setBackgroundColour(ogre.ColourValue(0.0, 0.0, 0.0))
    # we'll manually clear every frame
    mgr.vp.setClearEveryFrame(False)

    # aspect ratio, etc.
    mgr.cam.setAspectRatio(mgr.window.getWidth() / float(mgr.window.getHeight()))
    # 60 degree FOV
    mgr.cam.setFOVy(ogre.Degree(70))

    # set the timer
    mgr.timer = mgr.root.getTimer()

    
def stopRenderer():

    # deleting the root will take care of everything
    del mgr.root

    
def initResources():

    # just some hard coded stuff
    rgm = ogre.ResourceGroupManager.getSingletonPtr()

    # add our data directory (we sort of stuff everything in there
    # just to make it easy on ourselves.  organize better otherwise!)
    rgm.addResourceLocation("../media/ssao", "FileSystem", "Media")
    rgm.addResourceLocation("../media/models", "FileSystem", "Media")
    # init this group
    rgm.initialiseResourceGroup("Media") # "Media" is the group from above


def initShadows():

    # we'll be self shadowing
    mgr.sceneMgr.setShadowTextureSelfShadow(True)

    # our caster material
    mgr.sceneMgr.setShadowTextureCasterMaterial("shadow_caster")
    # note we have no "receiver".  all the "receivers" are integrated.

    # get the shadow texture count from the cfg file
    tempData = mgr.cfgFile.getSetting("shadowTextureCount", "renderer")
    # (each light needs a shadow texture)
    mgr.sceneMgr.setShadowTextureCount(int(tempData))

    # the size, too (1024 looks good with 3x3 or more filtering)
    tempData = mgr.cfgFile.getSetting("shadowTextureRes", "renderer")
    mgr.sceneMgr.setShadowTextureSize(int(tempData))

    # float 16 here.  we need the R and G channels.
    # float 32 works a lot better with a low/none VSM epsilon (wait till the shaders)
    # but float 16 is good enough and supports bilinear filtering on a lot of cards
    # (we should use _GR, but OpenGL doesn't really like it for some reason)
    mgr.sceneMgr.setShadowTexturePixelFormat(ogre.PF_FLOAT16_RGB)

    # big NONO to render back faces for VSM.  it doesn't need any biasing
    # so it's worthless (and rather problematic) to use the back face hack that
    # works so well for normal depth shadow mapping (you know, so you don't
    # get surface acne)
    mgr.sceneMgr.setShadowCasterRenderBackFaces(False)

    numShadowRTTs = mgr.sceneMgr.getShadowTextureCount()
    numShadowRTTs = mgr.sceneMgr.getShadowTextureCount()
    for i in range(0, numShadowRTTs):
    
        tex = mgr.sceneMgr.getShadowTexture(i)
        vp = tex.getBuffer().getRenderTarget().getViewport(0)
        vp.setBackgroundColour(ogre.ColourValue(1, 1, 1, 1))
        vp.setClearEveryFrame(True)

        
    # enable integrated additive shadows
    # actually, since we render the shadow map ourselves, it doesn't
    # really matter whether they are additive or modulative
    # as long as they are integrated v(O_o)v
    mgr.sceneMgr.setShadowTechnique(ogre.SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED)

    # and add the shader listener
    mgr.sceneMgr.addListener(shadowCameraUpdater)

    
def initInput():

    # window handle
    winHandle = mgr.window.getCustomAttributeInt("WINDOW")
    paramList = [("WINDOW", str(winHandle))]

    # create input "manager" or "hook", as I call it
    mgr.input = OIS.createPythonInputSystem(paramList)

    mgr.keys = mgr.input.createInputObjectKeyboard(OIS.OISKeyboard, True)
    mgr.mouse = mgr.input.createInputObjectMouse(OIS.OISMouse, True)

    
def stopInput():

    # destroy the keyboard object, the mouse object
    # and the input "hook"
    mgr.input.destroyInputObject(mgr.keys)
    mgr.input.destroyInputObject(mgr.mouse)
    OIS.InputManager.destroyInputSystem(mgr.input)

    
def captureInput():

    # we have to "capture" the current "state" of the input devices
    # every single frame.  (think of it as an "input update()")
    mgr.keys.capture()
    mgr.mouse.capture()

    
# handling the camera every frame based on a delta time and mouse/keys
def handleCamera(dt):
    # this is actually a bad way to do a first person camera
    # however, I'm trying to keep this relatively simple.
    pitch = 0
    yaw = 0

    # get the current mouse state
    ms = mgr.mouse.getMouseState()
    # fix pitch and yaw
    pitch += -ms.Y.rel * 0.5 # add the relative mouse movement (up/down for pitching)
    yaw += -ms.X.rel * 0.5 # add the relative mouse movement (left/right for yawing)
    # constrain pitch to +/- 90 degrees
    if (pitch < -90): pitch = -90
    if (pitch >  90): pitch =  90

    # set the camera to a default orientation and then get
    # it into our own rotation
    #mgr.cam.setOrientation(ogre.Quaternion().IDENTITY)
    # yaw first, pitch second (all in world space)
    mgr.cam.yaw(ogre.Degree(yaw))
    mgr.cam.pitch(ogre.Degree(pitch))

    # define a local movement vector (+X == right, +Y == yp, -Z == forward)
    move = ogre.Vector3(0, 0, 0)
    # basic WASD.  for your own entertainment, don't hardcode like me )
    if (mgr.keys.isKeyDown(OIS.KC_W)):
        move.z += -1
    if (mgr.keys.isKeyDown(OIS.KC_S)):
        move.z += 1
    if (mgr.keys.isKeyDown(OIS.KC_A)):
        move.x += -1
    if (mgr.keys.isKeyDown(OIS.KC_D)):
        move.x += 1
    # move the camera based on where it's looking at a speed
    CAM_SPEED = 10 # meters per second
    mgr.cam.move(mgr.cam.getOrientation() * (move * CAM_SPEED * dt))


def makeQuat(forward, wantedUp=ogre.Vector3.UNIT_Y):


    # just basically makes a full orientation based
    # on a desired "up" vector and a desired "forward" vector
    right = wantedUp.crossProduct(forward).normalisedCopy()
    up = forward.crossProduct(right).normalisedCopy()
    return ogre.Quaternion(right, up, forward)


def createObjects( 
    NUM_X, NUM_Z, # X/Z segments
    mesh, material):
    

    rootNode = mgr.sceneMgr.getRootSceneNode()

    for x in range(0, NUM_X):

        for z in range(0, NUM_Z):
        
            LIMIT = 10
            p = ogre.Vector3(
                ogre.Math.RangeRandom(-LIMIT, LIMIT),
                ogre.Math.RangeRandom(-LIMIT, LIMIT),
                ogre.Math.RangeRandom(-LIMIT, LIMIT))

            ent = mgr.sceneMgr.createEntity( \
                mesh + str(x) + "_" + str(z), mesh)

                
            ent.setMaterialName(material)

            node = rootNode.createChildSceneNode()
            node.attachObject(ent)
            node.setPosition(p)

            # scale from Ogre examples centimeters to meters
            node.setScale(ogre.Vector3(0.01, 0.01, 0.01))

            


def setupScene():

    rootNode = mgr.sceneMgr.getRootSceneNode()
    node = rootNode.createChildSceneNode()

    # create some random objects
    NUM_X = 10
    NUM_Z = NUM_X
    createObjects(NUM_X, NUM_Z, "knot.mesh", "blue")#"ogre")
    createObjects(NUM_X, NUM_Z, "ogrehead.mesh", "green")#"ogre")
    createObjects(NUM_X, NUM_Z, "Prefab_Cube", "orange")#"ogre")
    createObjects(NUM_X, NUM_Z, "Prefab_Sphere", "red")#"ogre")

    #print "*** MESH: " + mgr.cfgFile.getSetting("scene", "levelMesh")
    ent = mgr.sceneMgr.createEntity("level", "level.mesh")
    ent.setMaterialName("metal")
    node = rootNode.createChildSceneNode()
    node.attachObject(ent)
    node.setScale(ogre.Vector3(0.01, 0.01, 0.01)) # from cm to m
    # move it down to the center a bit
    node.setPosition(ogre.Vector3(0, -5, 0))

    # random position I found to be pretty
    mgr.cam.setPosition(ogre.Vector3(0.5, 2, 4))
    mgr.cam.setDirection(ogre.Vector3(0, 0, -1))

    # create our flash light
    light = mgr.sceneMgr.createLight("flashLight")
    light.setDiffuseColour(ogre.ColourValue(1, 1, 1))
    light.setSpecularColour(ogre.ColourValue(1, 1, 1))
    light.setType(ogre.Light.LT_SPOTLIGHT)
    light.setSpotlightInnerAngle(ogre.Degree(25))
    light.setSpotlightOuterAngle(ogre.Degree(45))
    light.setAttenuation(30, 1, 1, 1) # meter range.  the others our shader ignores
    light.setDirection(ogre.Vector3(0, 0, -1))
    node = rootNode.createChildSceneNode("flashLightNode")
    node.attachObject(light)

    
def handleScene(dt):

    # sync our "flashlight" with the camera
    node = mgr.sceneMgr.getSceneNode("flashLightNode")
    node.setPosition(mgr.cam.getDerivedPosition())
    node.setOrientation(mgr.cam.getDerivedOrientation())
    # this is a simply "offset", to the bottom right, as if
    # the camera is holding the light in its right hand
    offset = ogre.Vector3(0.225, -0.3, -0.3)
    # local space means that it's relative to the node
    # itself, not in world units
    node.translate(offset, ogre.Node.TS_LOCAL)

    
def initSSAO():

    mgr.ssao = ogre.CompositorManager.getSingleton().addCompositor(mgr.vp, "ssao")
    mgr.ssao.setEnabled(True)
    mgr.ssao.addListener(ssaoParamUpdater)

    
def stopSSAO():
    pass

    
def renderScene():

    # set the window's viewport as the active viewport
    mgr.rsys._setViewport(mgr.vp)
    # clear colour & depth
    mgr.rsys.clearFrameBuffer(ogre.FBT_COLOUR | ogre.FBT_DEPTH)

    # update the window
    mgr.window.update(False)
        
    # swap the window buffers
    mgr.window.swapBuffers(True)


# example of a key listener
class InputManager(OIS.KeyListener):

    def __init__(self):
        OIS.KeyListener.__init__(self)
        self.index = 0

    def keyPressed(self, e):
    
        # place a light if we pressed space
        if (e.key == OIS.KC_L):
            self.index += 1
            name = "light" + str(self.index)
            light = mgr.sceneMgr.createLight(name)
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

            node = \
                mgr.sceneMgr.getRootSceneNode().createChildSceneNode(name + "Node")
            node.setPosition(mgr.cam.getDerivedPosition())
            node.setOrientation(mgr.cam.getDerivedOrientation())
            node.attachObject(light)
        

        # toggle the flash light if we press F
        if (e.key == OIS.KC_F):
            flashLight = mgr.sceneMgr.getLight("flashLight")
            if flashLight.getVisible(): flashLight.setVisible(False)
            else: flashLight.setVisible(True)

            
        if (e.key == OIS.KC_SPACE):
            self.index += 1
            mesh = "Prefab_Cube"
            ent = mgr.sceneMgr.createEntity( \
                mesh + str(self.index), mesh)

            material = "orange" #"ogre"
            ent.setMaterialName(material)

            node = mgr.sceneMgr.getRootSceneNode().createChildSceneNode()
            node.attachObject(ent)
            node.setPosition(mgr.cam.getDerivedPosition())

            # scale from Ogre examples centimeters to meters
            node.setScale(ogre.Vector3(0.01, 0.01, 0.01))

            
        if (e.key == OIS.KC_F1):
            if mgr.ssao.getEnabled(): mgr.ssao.setEnabled(False)
            else: mgr.ssao.setEnabled(True)

            
        return True

        
    def keyReleased(self, e):
    
        # do nothing
        return True

inputMgr = InputManager()
# start off with a light index of 0 (we have a 0th light, but it's named "flashLight")

def main():

    # seed the random number generator
    random.seed()
    
    # load our config file
    mgr.cfgFile.loadDirect("../media/ssao/config.ini")
    # get our renderer (OpenGL or Direct3D)
    renderer = OGL # default to OGL
    rendererAsString = mgr.cfgFile.getSetting("library", "renderer")
    if (rendererAsString == "D3D" or rendererAsString == "Direct3D"):
        renderer = D3D

    # start up the renderer and a window (OGL for the example)
    initRenderer(renderer)
    # load all of our shaders and models and stuff
    initResources()
    # init the shadow setup we'll be using
    initShadows()
    # grab the window's input stuff with OIS
    initInput()
    # grab the keyboard input with our light placer
    mgr.keys.setEventCallback(inputMgr)

    # enable the SSAO
    initSSAO()

    # set up our basic scene
    setupScene()

    # this keeps track of time (seconds to keep it easy)
    currentTime = float(mgr.timer.getMilliseconds()) * 0.001
    running = True
    # keep running until something stops us
    # (frame listener, escape key, window closed (the mgr gets a message for this), etc.)
    while running and mgr.running:
    
        # note this
        # when we render manually, we need to pump
        # the window events ourselves
        # (or we won't get window events, like closing of the window, input, etc.)
        ogre.WindowEventUtilities.messagePump()

        # get the delta time and advance currentTime
        deltaTime = (float(mgr.timer.getMilliseconds()) * 0.001) - currentTime
        currentTime += deltaTime

        # capture our input state
        captureInput()
        
        # handle the camera movement
        handleCamera(deltaTime)

        # handle the scene movement (like animations for the robot)
        handleScene(deltaTime)

        # render to the window and stuff
        renderScene()

        running = mgr.root.renderOneFrame()
        # BTW, renderOneFrame() returns True if everything went alright
        # (such as frame listeners, which we actually don't use here)
        
        # simply "escape" key exit
        if (mgr.keys.isKeyDown(OIS.KC_ESCAPE)):
            running = False

        if (mgr.keys.isKeyDown(OIS.KC_P)):
            mgr.window.writeContentsToTimestampedFile("./", ".png")
        

        
    stopSSAO()

    # destroy input
    stopInput()
    # stop and deinitialise Ogre
    stopRenderer()

if __name__ == "__main__":
    main()
