import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.physics.OgreOde as OgreOde
import ogre.io.OIS as OIS
import SampleFramework as sf
import sys

#------------------------------------------------------------------------------------------------
carNames = [
    "Jeep",
    "JeepSway",
    "Subaru"
]
carFileNames= [
    "jeep.ogreode",
    "jeep.ogreode",
    "subaru.ogreode"
]
sSelectedCar = 1
maxNumCar = 3

#------------------------------------------------------------------------------------------------
class GranTurismOgreFrameListener ( sf.FrameListener ):
    def __init__(self, win, cam,time_step,root,world) :
        sf.FrameListener.__init__(self, win,cam)
        self.camera = cam
        self.vehicle = None
        self.world=world
        self.sSelectedCar = 2
        self.maxNumCar = 3
    
        # Reduce move speed
        self.MoveSpeed = 25
        self.dotOgreOdeLoader = OgreOde.DotLoader(self.world)
        self.TimeUntilNextToggle  = 0
        
        
        self._stepper_mode_choice = 2
        self._stepper_choice = 3
        
        
        time_scale = 1.0
        max_frame_time =1.0 / 4
        frame_rate = 1.0 / 60
        
        if self._stepper_mode_choice == 0:
            self.stepModeType = OgreOde.StepHandler.BasicStep
        elif self._stepper_mode_choice == 1:
            self.stepModeType = OgreOde.StepHandler.FastStep
        elif self._stepper_mode_choice == 2:
            self.stepModeType = OgreOde.StepHandler.QuickStep
        else:
            self.stepModeType = OgreOde.StepHandler.QuickStep
        
        if self._stepper_choice == 0:
            self._stepper = OgreOde.StepHandler(self.world, OgreOde.StepHandler.QuickStep, time_step,
            max_frame_time,
            time_scale)
        elif self._stepper_choice == 1:
            self._stepper = OgreOde.ExactVariableStepHandler(self.world, OgreOde.StepHandler.QuickStep, time_step,
            max_frame_time,
            time_scale)
        elif self._stepper_choice == 2:
            self._stepper = OgreOde.ForwardFixedStepHandler(self.world, OgreOde.StepHandler.QuickStep, time_step,
            max_frame_time,
            time_scale)
        elif self._stepper_choice == 3:
            self._stepper = OgreOde.ForwardFixedInterpolatedStepHandler(self.world, OgreOde.StepHandler.QuickStep, time_step,
            max_frame_time,
            time_scale)
        

        self._stepper.setAutomatic(OgreOde.StepHandler.AutoMode_PostFrame, root)

        root.getSingleton().setFrameSmoothingPeriod(5.0)

        self.changeCar()

        # Load up our UI and display it
        pOver = ogre.OverlayManager.getSingleton().getByName("OgreOdeDemos/Overlay")
        ogre.OverlayManager.getSingleton().getOverlayElement("OgreOdeDemos/Name").setCaption("Name: "+ "GranTurismo (car + trimesh)")
        ogre.OverlayManager.getSingleton().getOverlayElement("OgreOdeDemos/Keys").setCaption("Keys: " + "I/K - Accelerate/Brake, J/L - Turn, X - Change drive mode, N - Change Car")
        ogre.OverlayManager.getSingleton().getOverlayElement("OgreOdeDemos/OtherKeys").setCaption("Extra: " + "E - Debug Object")
        pOver.show()

    #------------------------------------------------------------------------------------------------
    def __del__(self ):
        del self._stepper

    #------------------------------------------------------------------------------------------------
    def changeCar( self ):
        self.sSelectedCar = (self.sSelectedCar + 1) % self.maxNumCar
        
        del self.vehicle
        self.vehicle = self.dotOgreOdeLoader.loadVehicle (carFileNames[self.sSelectedCar], carNames[self.sSelectedCar])

        # Initially (i.e. in the config file) it's rear wheel drive
        self.drive = 'R'

        # Move the vehicle
        v_pos = self.camera.getPosition() + (self.camera.getDirection() * 15.0)
        #v_pos.y += 10
        self.vehicle.setPosition(v_pos)

        self.updateInfo()
    #------------------------------------------------------------------------------------------------
    def updateInfo( self ):
        pOver = ogre.OverlayManager.getSingleton().getByName("OgreOdeDemos/Overlay")
        newInfo = "Info: " + carNames[self.sSelectedCar]
        if self.drive =='R':
            # Switch from rear to front
            newInfo = newInfo + " & Front wheel drive"
        elif self.drive == 'F':
            # Switch from front to all
            newInfo = newInfo + " & All wheel drive"
            # Switch from all to rear
        elif self.drive == '4':
            newInfo = newInfo + " & Rear wheel drive"
        ogre.OverlayManager.getSingleton().getOverlayElement("OgreOdeDemos/Info").setCaption(newInfo)

    #------------------------------------------------------------------------------------------------
    def frameStarted( self, evt):
        time = evt.timeSinceLastFrame

        ret = sf.FrameListener.frameStarted(self,evt)
        self.TimeUntilNextToggle -= time
        if (self.TimeUntilNextToggle <= 0) :
            # Switch debugging objects on or off
            if (self.Keyboard.isKeyDown(OIS.KC_E)):
                World.getSingleton ().setShowDebugObjects(not World.getSingleton ().getShowDebugObjects())
                self.TimeUntilNextToggle = 0.5

            if self.Keyboard.isKeyDown(OIS.KC_N):
                self.changeCar()
                self.TimeUntilNextToggle = 0.5

            if self.Keyboard.isKeyDown(OIS.KC_U):
                self._stepper.pause(False)
                self.TimeUntilNextToggle = 0.5
            if self.Keyboard.isKeyDown(OIS.KC_P):
                self._stepper.pause(True)
                self.TimeUntilNextToggle = 0.5
            # Change the drive mode between front, rear and 4wd
            if self.Keyboard.isKeyDown(OIS.KC_X):
                if self.drive == 'R':
                    self.drive = 'F'

                    self.vehicle.getWheel(0).setPowerFactor(1)
                    self.vehicle.getWheel(1).setPowerFactor(1)
                    self.vehicle.getWheel(2).setPowerFactor(0)
                    self.vehicle.getWheel(3).setPowerFactor(0)

                    self.updateInfo()

                    # Switch from front to all
                elif self.drive ==  'F':
                    self.drive = '4'

                    self.vehicle.getWheel(0).setPowerFactor(0.6)
                    self.vehicle.getWheel(1).setPowerFactor(0.6)
                    self.vehicle.getWheel(2).setPowerFactor(0.4)
                    self.vehicle.getWheel(3).setPowerFactor(0.4)

                    self.updateInfo()

                    # Switch from all to rear
                elif self.drive == '4':
                    self.drive = 'R'

                    self.vehicle.getWheel(0).setPowerFactor(0)
                    self.vehicle.getWheel(1).setPowerFactor(0)
                    self.vehicle.getWheel(2).setPowerFactor(1)
                    self.vehicle.getWheel(3).setPowerFactor(1)

                    self.updateInfo()
                self.TimeUntilNextToggle = 0.5

        if( not self._stepper.isPaused()):
            self.vehicle.setInputs(self.Keyboard.isKeyDown(OIS.KC_J),
                                self.Keyboard.isKeyDown(OIS.KC_L),
                                self.Keyboard.isKeyDown(OIS.KC_I),
                                self.Keyboard.isKeyDown(OIS.KC_K))
            self.vehicle.update(time)

            # Thanks to Ahmed!
            followFactor = 0.2
            camHeight = 2.0
            camDistance = 7.0
            camLookAhead = 8.0

            q = self.vehicle.getSceneNode().getOrientation()
            toCam = self.vehicle.getSceneNode().getPosition()

            toCam.y += camHeight
            toCam.z -= camDistance * q.zAxis().z
            toCam.x -= camDistance * q.zAxis().x

            self.camera.move( (toCam - self.camera.getPosition()) * followFactor )
            self.camera.lookAt(self.vehicle.getSceneNode().getPosition() + ((self.vehicle.getSceneNode().getOrientation() * ogre.Vector3.UNIT_Z) * camLookAhead))
        return ret

#------------------------------------------------------------------------------------------------
class GranTurismOgreApplication (sf.Application, OgreOde.CollisionListener ):

    def __init__ (self):
        sf.Application.__init__(self)
        OgreOde.CollisionListener.__init__( self )
        self._world = 0
        self._time_step = 0.01
        self._track = 0

    #------------------------------------------------------------------------------------------------
    def _chooseSceneManager(self):
        self.sceneManager = self.root.createSceneManager( ogre.ST_GENERIC, "ExampleGrandTurismo" )

    #------------------------------------------------------------------------------------------------
    def _setupResourcesNOT(self):
        sf.setupResources()
        rsm = ResourceGroupManager.getSingletonPtr()
        groups = rsm.getResourceGroups()
        if (std.find(groups.begin(), groups.end(), "OgreOde") == groups.end()):
            rsm.createResourceGroup("OgreOde")
            try:
                rsm.addResourceLocation("../../../../../ogreaddons/OgreOde/demos/Media","FileSystem", "OgreOde")
            except ogre.OgreException, e:
                error = e.getFullDescription()
                rsm.addResourceLocation("../../../OgreOde/demos/Media","FileSystem", "OgreOde")

    #------------------------------------------------------------------------------------------------
    def _createCamera(self):
        # Create the camera
        self.camera = self.sceneManager.createCamera("PlayerCam")

        self.camera.setPosition(ogre.Vector3(125,-14,8))
        self.camera.lookAt(self.camera.getPosition() + ogre.Vector3(0,0,1))
        self.camera.setNearClipDistance( 1 )
        self.camera.setFarClipDistance( 1000 )

    #------------------------------------------------------------------------------------------------
    # Just override the mandatory create scene method
    def _createScene(self):
        # Set ambient light
        self.sceneManager.setAmbientLight(ogre.ColourValue(0.5, 0.5, 0.5))
        self.sceneManager.setSkyBox(True,"GranTurismOgre/Skybox")

        # Create a light
        l = self.sceneManager.createLight("MainLight")

        l.setPosition(20,800,50)
        l.setSpecularColour(1,0.9,0)

        l.setCastShadows(True)

        self.sceneManager.setShadowTechnique(ogre.SHADOWTYPE_STENCIL_MODULATIVE)
        self.sceneManager.setShadowColour(ogre.ColourValue(0.5,0.5,0.5))

        self._world = OgreOde.World(self.sceneManager)

        self._world.setGravity(ogre.Vector3(0,-9.80665,0))
        self._world.setCFM(10e-5)
        self._world.setERP(0.8)
        self._world.setAutoSleep(True)
        self._world.setContactCorrectionVelocity(1.0)

        # Create something that will step the world automatically
        self._world.setCollisionListener(self)

        track_node = self.sceneManager.getRootSceneNode().createChildSceneNode("track")
        track_mesh = self.sceneManager.createEntity("track","racingcircuit.mesh")
        track_node.attachObject(track_mesh)

        ei = OgreOde.EntityInformer(track_mesh)
        print ei
        print self._world.getDefaultSpace()
        self._track = ei.createStaticTriangleMesh(self._world, self._world.getDefaultSpace())

    #------------------------------------------------------------------------------------------------
    # Create new frame listener
    def _createFrameListener(self):
        self.FrameListener= GranTurismOgreFrameListener(self.renderWindow, self.camera,self._time_step,self.root, self._world)
        self.root.addFrameListener(self.FrameListener)
    #------------------------------------------------------------------------------------------------
    def collision(self, contact):
        if not OgreOde.Vehicle.handleTyreCollision(contact):
            contact.setBouncyness(0.0)
            contact.setCoulombFriction(18.0)
        return True
    #------------------------------------------------------------------------------------------------
    def __del__(self):
        del self._track
        del self._world

if __name__ == '__main__':
    try:
        application = GranTurismOgreApplication()
        application.go()
    except ogre.OgreException, e:
        print e
