import os, sys
import ogre.renderer.OGRE as ogre
import ogre.renderer.ogrepaging as ogrepaging
import ogre.io.OIS as OIS
import math


class GameCamera(object): # camera update
    def __init__(self, scene, camera, cameraNode, defNodeOrient):
        self.scene = scene
        self.viewPort = scene.ogreRoot.getAutoCreatedWindow().addViewport(camera)
        self.camera = camera
        self.cameraNode = cameraNode
        self.defNodeOrient = defNodeOrient

        self.camHeight = 40
        self.translateVector = ogre.Vector3(0.0, 0.0, 0.0)
        self.moveScale = 0
        self.moveSpeed = 400
        self.rotationScale = 0.1
        self.rotationSpeed = 0.5
        self.key = None

        self.ResetRotation()
        self.MoveCamera(500, self.camHeight, 500)
        self.RotateCamera(0, 0)

        listener = scene.gameRoot.GetEventListener()
        listener.BindMouseMoved(self._OnMouseMoved)
        listener.BindKeyPressed(self._OnKeyPressed)
        listener.BindKeyReleased(self._OnKeyReleased)
        listener.BindTick(self._OnTick)

    def _OnTick(self, frameEvent, lastMouseState, lastKeyState):
        if frameEvent.timeSinceLastFrame == 0:
            self.moveScale = 1
            self.rotationScale = 0.1

        else:
            self.moveScale = self.moveSpeed * frameEvent.timeSinceLastFrame
            self.rotationScale = self.rotationSpeed * frameEvent.timeSinceLastFrame

        if self.key == OIS.KC_A:
            self.translateVector.x = -self.moveScale

        if self.key == OIS.KC_D:
            self.translateVector.x = self.moveScale

        if self.key == OIS.KC_W:
            self.translateVector.z = -self.moveScale

        if self.key == OIS.KC_S:
            self.translateVector.z = self.moveScale

        if self.key == OIS.KC_PGUP:
            self.translateVector.y = self.moveScale

        if self.key == OIS.KC_PGDOWN:
            self.translateVector.y = -self.moveScale

        if self.key in [OIS.KC_A, OIS.KC_D, OIS.KC_W, OIS.KC_S, OIS.KC_PGDOWN, OIS.KC_PGUP]:
            self.TranslateCamera(self.translateVector)
    def _OnKeyReleased(self, evt):
        self.key = None
    def _OnKeyPressed(self, evt):
        self.key = evt.key

    def _OnMouseMoved(self, lastMouseState):
        self.RotateCamera(-lastMouseState.relX*self.rotationSpeed, -lastMouseState.relY*self.rotationSpeed)


    def MoveCamera(self, x, y, z):
        self.cameraNode.setPosition(x, y, z)

    def TranslateCamera(self, translateVector):
        try:
            self.camera.translate(translateVector) # for using OgreRefApp
        except AttributeError:
            self.camera.moveRelative(translateVector)

    def RotateCamera(self, yaw, pitch):
        self.camera.getParentSceneNode().yaw(ogre.Degree(yaw),
                ogre.Node.TS_WORLD)
        self.camera.getParentSceneNode().pitch(ogre.Degree(pitch))
        self.cameraNode._updateBounds()

    def ResetRotation(self):
        self.cameraNode.setOrientation(self.defNodeOrient)
        self.camera.lookAt(
            self.cameraNode._getDerivedPosition() + ogre.Vector3().NEGATIVE_UNIT_Z)

    def GetViewport(self):
        return self.viewPort

    def GetDefaultNodeOrient(self):
        return self.defNodeOrient
    def GetOgreCamera(self):
        return self.camera


class GameRoot(object): # Facade
    def __init__(self, ogreRoot, eventListener):
        self.ogreRoot = ogreRoot
        self.eventListener = eventListener
        self.scene = Scene(self)
        self.scene.OnGameRootInit()

    def GetOgreRoot(self):
        return self.ogreRoot
    def GetScene(self):
        return self.scene
    def GetEventListener(self):
        return self.eventListener

class MouseState(object):
    def __init__(self):
        self.x = 0
        self.y = 0
        self.w = 0
        self.h = 0
        self.relX = 0
        self.relY = 0
        self.relZ = 0
        self.pressedButtons = {}

    def OnMouseMoved(self, x, y, w, h, relX, relY, relZ):
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.relX = relX
        self.relY = relY
        self.relZ = relZ
    def OnMousePressed(self, x, y, w, h, id):
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.pressedButtons[id] = 0
    def OnMouseReleased(self, x, y, w, h, id):
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        try:
            del self.pressedButtons[id]
        except:
            pass
    def UpdateWithMouseState(self, mouseState):
        self.x, self.y, self.w, self.h = mouseState.GetValues()
        self.relX, self.relY = self.GetRelativeMovements()
        for key in mouseState.GetPressedButtons().iterkeys():
            self.pressedButtons[key] = mouseState.GetPressedButtons()[key]
    def OnTick(self, time):
        for key in self.pressedButtons.iterkeys():
            self.pressedButtons[key] += time

    def GetValues(self):
        return self.x, self.y, self.w, self.h
    def GetRelativeMovements(self):
        return self.relX, self.relY
    def GetWheelMovement(self):
        return self.relZ
    def GetPressedButtons(self):
        return self.pressedButtons
    def _GetScreenVector(self, x, y, w, h):
        if w and h:
            mx = float(x) - float(w)/2.0
            my = float(y) - float(h)/2.0
            vectorX, vectorY = mx/(float(w)/2.0), -my/(float(h)/2.0)
            return vectorX, vectorY
        else:
            return 0, 0
    def GetScreenVector(self):
        return self._GetScreenVector(*self.GetValues())

    def GetScreenVectorDegree(self):
        vector = self.GetScreenVector()
        return Vector2ToAngle(*vector)

def Vector2ToAngle(x, y):
    vecOrg = ogre.Vector2(1.0, 0.0)
    vecPos = ogre.Vector2(x, y)
    vecPos.normalise()
    dotted = vecOrg.dotProduct(vecPos)
    if dotted == 0.0:
        dotted = 0.0001
    convert = 360.0/(2*math.pi)

    angle = math.acos(dotted)*convert
    if y < 0:
        angle = -angle
    angle %= 360
    return angle

class KeyState(object):
    def __init__(self):
        self.pressedKey = None
        self.pressedChar = None
        self.timePressedFor = None

    def OnKeyPressed(self, evt):
        self.pressedKey = evt.key
        self.pressedChar = evt.text
        self.timePressedFor = 0
    def OnTick(self, time):
        if self.timePressedFor:
            self.timePressedFor += time
    def OnKeyReleased(self):
        self.pressedKey = None
        self.pressedChar = None
        self.timePressedFor = None

    def GetPressedKey(self):
        return self.pressedKey
    def GetPressedTime(self):
        return self.timePressedFor

class OgreEventListener(ogre.FrameListener, ogre.WindowEventListener,
        OIS.MouseListener, OIS.KeyListener, OIS.JoyStickListener):
    mouse = None
    keyboard = None
    joy = None

    def __init__(self, app, renderWindow, bufferedMouse, bufferedKeys, bufferedJoy):
        ogre.FrameListener.__init__(self)
        ogre.WindowEventListener.__init__(self)
        OIS.MouseListener.__init__(self)
        OIS.KeyListener.__init__(self)
        OIS.JoyStickListener.__init__(self)

        self.app = app
        self.renderWindow = renderWindow

        # Init input system
        import platform
        int64 = False
        for bit in platform.architecture():
            if '64' in bit:
                int64 = True
        # Create the inputManager using the supplied renderWindow
        if int64:
            windowHnd = self.renderWindow.getCustomAttributeUnsignedLong("WINDOW")
        else:
            windowHnd = self.renderWindow.getCustomAttributeInt("WINDOW")
        t = self._inputSystemParameters()
        params = [("WINDOW",str(windowHnd))]
        params.extend(t)
        self.inputManager = OIS.createPythonInputSystem(params)

        try:
            if bufferedMouse:
                self.mouse = self.inputManager.createInputObjectMouse(OIS.OISMouse, bufferedMouse)
                self.mouse.setEventCallback(self)

            if bufferedKeys:
                self.keyboard = self.inputManager.createInputObjectKeyboard(OIS.OISKeyboard, bufferedKeys)
                self.keyboard.setEventCallback(self)

            if bufferedJoy:
                self.joy = self.inputManager.createInputObjectJoyStick(OIS.OISJoyStick, bufferedJoy)
                self.joy.setEventCallback(self)

        except Exception, e: # Unable to obtain mouse/keyboard/joy input
            raise e


        self.windowResized(self.renderWindow)

        # Listen for any events directed to the window manager's close button
        ogre.WindowEventUtilities.addWindowEventListener(self.renderWindow, self)

        self.quitApplication = False
        self.lastMouseState = MouseState()
        self.lastKeyState = KeyState()

        self.bindsMouseMoved = []
        self.bindsMousePressed = []
        self.bindsMouseReleased = []
        self.bindsKeyPressed = []
        self.bindsKeyReleased = []
        self.bindsOnTick = []
        self.bindsAppClosed = []

        self.bindsMouseMovedReturn = None
        self.bindsMousePressedReturn = None
        self.bindsMouseReleasedReturn = None



    def __del__ (self ):
        # Clean up OIS
        self.delInputObjects()

        OIS.InputManager.destroyInputSystem(self.inputManager)
        self.inputManager = None

        ogre.WindowEventUtilities.removeWindowEventListener(self.renderWindow, self)
        self.windowClosed(self.renderWindow)

    def delInputObjects(self):
        # Clean up the initialized input objects
        if self.keyboard:
            self.inputManager.destroyInputObjectKeyboard(self.keyboard)
        if self.mouse:
            self.inputManager.destroyInputObjectMouse(self.mouse)
        if self.joy:
            self.inputManager.destroyInputObjectJoy(self.joy)


### Window Event Listener callbacks ###

    def windowResized(self, rw):
         dummyint = 0
         width, height, depth, left, top= rw.getMetrics(dummyint,dummyint,dummyint, dummyint, dummyint)
         # Note the wrapped function as default needs unsigned int's
         ms = self.mouse.getMouseState()
         ms.width = width
         ms.height = height

    def windowClosed(self, renderWindow):
        # Only close for window that created OIS
        for func in self.bindsAppClosed:
            func()
        if(renderWindow == self.renderWindow):
            del self

        import sys
        sys.exit()

### Mouse Listener callbacks ###
    def mouseMoved(self,arg):
        state = arg.get_state()
        self.lastMouseState.OnMouseMoved(state.X.abs, state.Y.abs, state.width, state.height, state.X.rel, state.Y.rel, state.Z.rel)
        for func in self.bindsMouseMoved:
            func(self.lastMouseState)

        return True

    def mousePressed(self, arg, id):
        state = arg.get_state()
        self.lastMouseState.OnMousePressed(state.X.abs, state.Y.abs, state.width, state.height, id)
        for func in self.bindsMousePressed:
            func(self.lastMouseState, id)

        return True

    def mouseReleased(self, arg, id):
        state = arg.get_state()
        self.lastMouseState.OnMouseReleased(state.X.abs, state.Y.abs, state.width, state.height, id)

        for func in self.bindsMouseReleased:
            func(self.lastMouseState, id)
        return True

### Key Listener callbacks ###
    def keyPressed(self, evt):
        self.lastKeyState.OnKeyPressed(evt)

        for func in self.bindsKeyPressed:
            func(evt)
        return True

    def keyReleased(self, evt):
        self.lastKeyState.OnKeyReleased()

        for func in self.bindsKeyReleased:
            func(evt)
        return True

### Joystick Listener callbacks ###
    def buttonPressed(self, evt, id):
        return True

    def buttonReleased(self, evt, id):
        return True

    def axisMoved(self, evt, id):
        return True

### For mouse pointer ###
    def _inputSystemParameters (self ):
        if os.name == 'nt':
            return [("w32_mouse","DISCL_FOREGROUND"), ("w32_mouse", "DISCL_NONEXCLUSIVE")]

### Tick ###
    def frameStarted(self, frameEvent):
        if self.keyboard:
            self.keyboard.capture()
        if self.mouse:
            self.mouse.capture()
        if self.joy:
            self.joy.capture()
            # joystick test
            axes_int = self.joy.getJoyStickState().mAxes
            axes = []
            for i in axes_int:
                axes.append(i.abs)

        self.lastMouseState.OnTick(frameEvent.timeSinceLastFrame)
        self.lastKeyState.OnTick(frameEvent.timeSinceLastFrame)

        for func in self.bindsOnTick:
            func(frameEvent, self.lastMouseState, self.lastKeyState)

        stats = self.renderWindow.getStatistics()
        curFPS = str(int(stats.lastFPS))
        #print curFPS

        return True

    def BindMouseMoved(self, func):
        self.bindsMouseMoved += [func]
    def BindMousePressed(self, func):
        self.bindsMousePressed += [func]
    def BindMouseReleased(self, func):
        self.bindsMouseReleased += [func]

    def BindMouseMovedReturn(self, func):
        self.bindsMouseMovedReturn = func
    def BindMousePressedReturn(self, func):
        self.bindsMousePressedReturn = func
    def BindMouseReleasedReturn(self, func):
        self.bindsMouseReleasedReturn = func

    def BindKeyPressed(self, func):
        self.bindsKeyPressed += [func]
    def BindKeyReleased(self, func):
        self.bindsKeyReleased += [func]

    def BindTick(self, func):
        self.bindsOnTick += [func]

    def BindAppClosed(self, func):
        self.bindsAppClosed += [func]




class NewPaging(ogrepaging.PageProvider):
    def __init__(self, sceneManager, scene):
        ogrepaging.PageProvider.__init__(self)
        self.sceneMgr = sceneManager
        self.scene = scene
        self.camera = scene.GetCamera().camera
        self.root = scene.gameRoot.GetOgreRoot()
        self.pageManager = None
        self.setupContent()

    def _OnTick(self, frameEvent, lastMouseState, lastKeyState):
        pass

    def _OnKeyReleased(self, evt):
        self.pressingKey = None
    def _OnKeyPressed(self, evt):
        if evt.key == OIS.KC_S:
            pass

        self.pressingKey = evt.key
    def _OnMouseMoved(self, lastMouseState):
        self.RotateCamera(-lastMouseState.relX*self.rotationSpeed, -lastMouseState.relY*self.rotationSpeed)

    def GetMouseRay(self, x, y, w, h):
        pos_w = float(x) / float(w)
        pos_h = float(y) / float(h)
        mouseRay = self.scene.GetCameraToViewportRay(pos_w, pos_h)
        return mouseRay

    def setupContent(self):
        self.pageManager = ogrepaging.PageManager()
        world = self.pageManager.createWorld()
        sec = world.createSection("Grid2D", self.sceneMgr)
        data = sec.getStrategyData()
        #// accept defaults for now


        self.pageManager.setDebugDisplayLevel(1)

        #// hook up self to provide pages procedurally
        self.pageManager.setPageProvider(self)

        self.camera.setPosition(0, 100, 0)

        self.pageManager.addCamera(self.camera);


    def prepareProceduralPage(self, page, section):
        #// say we populated something just so it doesn't try to load any more
        return True

    def loadProceduralPage(self, page, section):
        #// say we populated something just so it doesn't try to load any more
        return True


class Scene(object):
    def __init__(self, gameRoot):
        self.gameRoot = gameRoot

    def GetOgreSceneManager(self):
        return self.sceneManager
    def OnGameRootInit(self):
        self.ogreRoot = self.gameRoot.GetOgreRoot()
        self.sceneManager = self.ogreRoot.createSceneManager(ogre.ST_GENERIC)
        #self.sceneManager = self.ogreRoot.createSceneManager(ogre.ST_EXTERIOR_CLOSE)

        def CreateCamera():
            camera = self.sceneManager.createCamera("PlayerCam")
            cameraNode = self.sceneManager.getRootSceneNode().createChildSceneNode("PlayerCamNode")
            cameraNode.attachObject(camera)
            cameraNode.setFixedYawAxis(True)
            camera.setNearClipDistance(2)
            camera.setFarClipDistance(1000)

            defNodeOrient = cameraNode.getOrientation()
            return camera, cameraNode, defNodeOrient


        self.gameCamera = GameCamera(self, *CreateCamera())

        self.rn = self.sceneManager.getRootSceneNode()

        # Set ambient light
        self.sceneManager.setAmbientLight(ogre.ColourValue(0.8, 0.8, 0.8))

        # Fog
        # NB it's VERY important to set this before calling setWorldGeometry
        # because the vertex program picked will be different
        fadeColour = ogre.ColourValue(0.101, 0.125, 0.1836)
        #self.sceneManager.setFog(ogre.FOG_LINEAR, fadeColour, 0.001, 500, 1000)
        renderWindow = self.ogreRoot.getAutoCreatedWindow()
        renderWindow.getViewport(0).setBackgroundColour(fadeColour)

        #terrain_cfg = "terrain.cfg"
        #self.sceneManager.setWorldGeometry(terrain_cfg)

        self.terrain = NewPaging(self.sceneManager, self)



    def GetTerrain(self):
        return self.terrain
    def GetCamera(self):
        return self.gameCamera
    def GetGameRoot(self):
        return self.gameRoot
    def GetCameraToViewportRay(self, pos_w, pos_h):
        return self.gameCamera.camera.getCameraToViewportRay(pos_w, pos_h)
    def GetOgreSceneManager(self):
        return self.sceneManager


class OgreApplication(object):
    app_title = "New Terrain"
    def Init(self):
        def _InitOgre():
            # create root
            self.ogreRoot = ogre.Root()

            # Read the resources.cfg file and add all resource locations in it
            cf = ogre.ConfigFile()
            cf.load("resources.cfg")
            seci = cf.getSectionIterator()
            while seci.hasMoreElements():
                secName = seci.peekNextKey()
                settings = seci.getNext()

                for item in settings:
                    typeName = item.key
                    archName = item.value
                    ogre.ResourceGroupManager.getSingleton().addResourceLocation(archName, typeName, secName)

            # Show the config dialog if we don't yet have an ogre.cfg file
            if not self.ogreRoot.restoreConfig() and not self.ogreRoot.showConfigDialog():
                raise Exception("User canceled config dialog! (setupRenderSystem)")

            # createRenderWindow
            self.ogreRoot.initialise(True, self.app_title)

            # initializeResourceGroups
            ogre.TextureManager.getSingleton().setDefaultNumMipmaps(3)
            ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()

            # initialise event listener
            self.eventListener = OgreEventListener(self, self.ogreRoot.getAutoCreatedWindow(), True, True, False)
            self.ogreRoot.addFrameListener(self.eventListener)

        _InitOgre()
        self.gameRoot = GameRoot(self.ogreRoot, self.eventListener)

    def Launch(self):
        self.Init()
        self.ogreRoot.startRendering()

    def GetGameRoot(self):
        return self.gameRoot

def main():
    try:
        app = OgreApplication()
        app.Launch()
    except ogre.OgreException, e:
        print e

if __name__ == '__main__':
    main()


