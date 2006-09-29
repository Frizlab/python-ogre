# This code is in the Public Domain
import Ogre as ogre, sys
import SampleFramework as sf
class LightingApplication(sf.Application):
    def __init__(self):
        self.mAnimStateList=[]	
        sf.Application.__init__(self)
        
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera
        
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.setSkyBox(True, 'Examples/SpaceSkyBox')

        entity = sceneManager.createEntity('head', 'ogrehead.mesh')
        sceneManager.getRootSceneNode().attachObject(entity)

        redYellowLightsNode = sceneManager.getRootSceneNode().createChildSceneNode()
        redYellowLights = sceneManager.createBillboardSet('RedYellowLights', 20)
        redYellowLights.materialName = 'Examples/Flare'
        redYellowLightsNode.attachObject(redYellowLights)
        self.redYellowLightsNode = redYellowLightsNode

        greenBlueLightsNode = sceneManager.getRootSceneNode().createChildSceneNode()
        greenBlueLights = sceneManager.createBillboardSet('GreenBlueLights', 20)
        greenBlueLights.materialName = 'Examples/Flare'
        greenBlueLightsNode.attachObject(greenBlueLights)
        self.greenBlueLightsNode = greenBlueLightsNode

        # Red light billboard, in "off" state
        redLightPosition = ogre.Vector3(78, -8, -70)
        self.redLightBoard = redYellowLights.createBillboard(redLightPosition, ogre.ColourValue(1, 1, 1))
        self.redLightBoard.colour = ogre.ColourValue(0, 0, 0)
        
        # Blue light billboard, in "off" state        
        blueLightPosition = ogre.Vector3(-90, -8, -70)
        blueLightBoard = greenBlueLights.createBillboard(blueLightPosition, ogre.ColourValue(1, 1, 1))
        blueLightBoard.colour = ogre.ColourValue(0, 0, 0)
        
        # Yellow light billboard, in "off" state        
        yellowLightPosition = ogre.Vector3(-4.5, 30, -80)
        yellowLightBoard = redYellowLights.createBillboard(yellowLightPosition, ogre.ColourValue(1, 1, 1))
        yellowLightBoard.colour = ogre.ColourValue(0, 0, 0)
        
        # Green light billboard, in "off" state        
        greenLightPosition = ogre.Vector3(50, 70, 80)
        greenLightBoard = redYellowLights.createBillboard(greenLightPosition, ogre.ColourValue(1, 1, 1))
        greenLightBoard.colour = ogre.ColourValue(0, 0, 0)

        # Red light, in "off" state
        self.redLight = sceneManager.createLight('RedFlyingLight')
        self.redLight.type = ogre.Light.LT_POINT
        self.redLight.position = redLightPosition
        self.redLight.diffuseColour = ogre.ColourValue(0, 0, 0)
        redYellowLightsNode.attachObject(self.redLight)

        # Blue light, in "off" state
        blueLight = sceneManager.createLight('BlueFlyingLight')
        blueLight.type = ogre.Light.LT_POINT
        blueLight.position = blueLightPosition
        blueLight.diffuseColour = (0, 0, 0)
        greenBlueLightsNode.attachObject(blueLight)

        # Yellow light, in "off" state
        yellowLight = sceneManager.createLight('YellowFlyingLight')
        yellowLight.type = ogre.Light.LT_POINT
        yellowLight.position = yellowLightPosition
        yellowLight.diffuseColour = 0, 0, 0
        redYellowLightsNode.attachObject(yellowLight)

        # Green light, in "off" state
        greenLight = sceneManager.createLight('GreenFlyingLight')
        greenLight.type = ogre.Light.LT_POINT
        greenLight.position = greenLightPosition
        greenLight.diffuseColour = 0, 0, 0
        greenBlueLightsNode.attachObject(greenLight)
        
        # Light flashers
        redLightFlasher = LightFlasher(self.redLight, self.redLightBoard, (1, 0, 0))
        greenLightFlasher = LightFlasher(greenLight, greenLightBoard, (0, 1, 0))
        blueLightFlasher = LightFlasher(blueLight, blueLightBoard, (0, 0, 1))
        yellowLightFlasher = LightFlasher(yellowLight, yellowLightBoard, (1.0,1.0,0.0))
        
        # Light controller functions
        redLightControllerFunc = LightFlasherControllerFunction(ogre.WFT_SINE, 0.5, 0.0)
        blueLightControllerFunc = LightFlasherControllerFunction(ogre.WFT_SINE, 0.75, 0.5)
        yellowLightControllerFunc = LightFlasherControllerFunction(ogre.WFT_SINE, 0.25, 0.0)
        greenLightControllerFunc = LightFlasherControllerFunction(ogre.WFT_SINE, 0.25, 0.5)
        
#         # Convert to shared pointers
#         redLightFlasher = ogre.SharedPtr(redLightFlasher)
#         greenLightFlasher = ogre.SharedPtr(greenLightFlasher)
#         blueLightFlasher = ogre.SharedPtr(blueLightFlasher)
#         yellowLightFlasher = ogre.SharedPtr(yellowLightFlasher)
#         
#         redLightControllerFunc = ogre.SharedPtr(redLightControllerFunc)
#         blueLightControllerFunc = ogre.SharedPtr(blueLightControllerFunc)
#         yellowLightControllerFunc = ogre.SharedPtr(yellowLightControllerFunc)
#         greenLightControllerFunc = ogre.SharedPtr(greenLightControllerFunc)
#   
#         # Convert to necessary pointers to then use to create controllers
#         redLightFlasher = ogre.FloatControllerValue(redLightFlasher)
#         greenLightFlasher = ogre.FloatControllerValue(greenLightFlasher)
#         blueLightFlasher = ogre.FloatControllerValue(blueLightFlasher)
#         yellowLightFlasher = ogre.FloatControllerValue(yellowLightFlasher)


       # create controller, after this is will get updated on its own
        #self.func = ogre.FloatControllerFunction(ogre.WaveformControllerFunction(ogre.WFT_SINE, 0.75, 0.5))#.pointer
        self.func= ogre.FloatControllerFunctionPtr(LightFlasherControllerFunction(ogre.WFT_SINE, 0.5, 0.0))#.pointer
        
        
        self.contMgr = ogre.ControllerManager.getSingleton()
        
        
#        val = ogre.FloatControllerValue(LightFlasher(mLight, bb, mMinLightColour, mMaxLightColour, mMinFlareSize, mMaxFlareSize))
        
        self.val = ogre.FloatControllerValue(LightFlasher(self.redLight, self.redLightBoard, (1, 0, 0)))#.pointer
        
        print "Creating Controller"
        
        self.controller = self.contMgr.createController(self.contMgr.getFrameTimeSource(), self.val, self.func)
#        print controller ," Done"

        
        #### AJM BROKEN - enabling this call causes a Windows crash...
##        redLightControllerFunc = ogre.FloatControllerFunction(redLightControllerFunc)
#         blueLightControllerFunc = ogre.FloatControllerFunction(blueLightControllerFunc)
#         yellowLightControllerFunc = ogre.FloatControllerFunction(yellowLightControllerFunc)
#         greenLightControllerFunc = ogre.FloatControllerFunction(greenLightControllerFunc)

#         # Light controllers
#         controllerManager = ogre.ControllerManager.getSingleton()
# #             
#         self.redLightController = controllerManager.createController(
#             controllerManager.getFrameTimeSource(),
#              ogre.FloatControllerValue(redLightFlasher), #redLightFlasher.pointer,
#              redLightControllerFunc.get_pointer())
# #         self.blueLightController = controllerManager.createController(
#             controllerManager.getFrameTimeSource(),
#             blueLightFlasher,
#             blueLightControllerFunc)
#         self.yellowLightController = controllerManager.createController(
#             controllerManager.getFrameTimeSource(),
#             yellowLightFlasher,
#             yellowLightControllerFunc)
#         self.greenLightController = controllerManager.createController(
#             controllerManager.getFrameTimeSource(),
#             greenLightFlasher,
#             greenLightControllerFunc)

			
    def _createFrameListener(self):
        print "_createFrameListener" , self
        self.frameListener = LightingListener(self.renderWindow, self.camera,
                                               self.redYellowLightsNode, self.greenBlueLightsNode )
        print "Created Lighting listener", self.frameListener
        self.root.addFrameListener(self.frameListener)
        print "Frame Listener created", self

class LightingListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, redYellowLightsNode, greenBlueLightsNode):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.redYellowLightsNode = redYellowLightsNode
        self.greenBlueLightsNode = greenBlueLightsNode

    def frameStarted(self, frameEvent):
#         self.redYellowLightsNode.yaw(ogre.Degree(frameEvent.timeSinceLastFrame * 10.0),
#                                       ogre.Node.TS_LOCAL)
#         self.greenBlueLightsNode.pitch(ogre.Degree(frameEvent.timeSinceLastFrame * 20.0),
#                                         ogre.Node.TS_LOCAL)
# 		std::vector<AnimationState*>::iterator animi;
# 		for (animi = mAnimStateList.begin(); animi != mAnimStateList.end(); ++animi)
# 		{
# 			(*animi)->addTime(evt.timeSinceLastFrame);
# 		}


        sfret = sf.FrameListener.frameStarted(self, frameEvent)
        return sfret

        
        
        
class LightFlasherControllerFunction(ogre.WaveformControllerFunction):
    def __init__(self, waveType, frequency, phase):
        ogre.WaveformControllerFunction.__init__(
            self, waveType, 0, frequency, phase, 1, True)

class LightFlasher(ogre.FloatControllerValue):

    def __init__(self, light, billboard, maxColour):
        ogre.FloatControllerValue.__init__(self)
 
    def getValue(self):
        return self.intensity

    def setValue(self, value):
        self.intensity = value

        newColour = self.maxColour * value
        newColour.a = self.maxColour.a

        self.light.diffuseColour = newColour
        self.billboard.colour = newColour
        


if __name__ == '__main__':
    try:
        application = LightingApplication()
        application.go()
    except ogre.Exception, e:
        print "#########"
        print e
        print e.fulDescription
        print e.source,e.file,e.line
    except:
        print "Unexpected error:", sys.exc_info()[0]
        raise
    
