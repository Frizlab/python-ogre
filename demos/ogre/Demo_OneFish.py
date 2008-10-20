# this demo shows a fish and a ninja
# based on Wx version but without the Wx

# Keyboard strokes
# 1,2 switches cams
# 3 autocam

# please feel free to distribute as part of python-ogre demos    -PythonOgreUser101-
# improving python-ogre one demo at a time

import sys
sys.path.insert(0,'..')
import PythonOgreConfig


import ogre.renderer.OGRE as ogre
import SampleFramework
import ogre.io.OIS as OIS


class TutorialFrameListener(SampleFramework.FrameListener):
    
    def __init__(self, renderWindow, camera, sceneManager,AnimStateList,AnimSpeedList, AnimEntity, AnimNode):
        
        SampleFramework.FrameListener.__init__(self, renderWindow, camera)
 
        self.mAnimStateList = AnimStateList
        self.mAnimSpeedList = AnimSpeedList
        self.mAnimEntity    = AnimEntity        
        self.mAnimNode      = AnimNode

        # populate the camera and scene manager containers
        self.camNode = camera.parentSceneNode.parentSceneNode
        self.sceneManager = sceneManager
        self.toggle=0


    def frameStarted(self, evt):
        
        self.mAnimEntity[0].setPosition (self.mAnimEntity[1].position )
        
        #Then, takes care of the direction that the fish has to face to 
        directionToGo = self.mAnimEntity[2].position - self.mAnimEntity[1].position
        src =  self.mAnimEntity[0].Orientation *  (-ogre.Vector3().UNIT_X)         
        quat = src.getRotationTo(directionToGo) #Calculates the quaternion of the appropriate rotation 
        self.mAnimEntity[0].rotate(quat)        #Rotates, the fish. Here, we know that no rotation of 180 
                                                #degrees will occur so we do not have to care about it 


        self.mAnimStateList[0].addTime(evt.timeSinceLastFrame * 4.0)
        self.mAnimStateList[1].addTime(evt.timeSinceLastFrame * 1.0)
        self.mAnimStateList[2].addTime(evt.timeSinceLastFrame * 1.0)


        if self._isToggleKeyDown(OIS.KC_1, 1.0):
            self.camera.parentSceneNode.detachObject(self.camera)
            self.camNode = self.sceneManager.getSceneNode("CamNode1")
            self.sceneManager.getSceneNode("PitchNode1").attachObject(self.camera)
            
        elif self._isToggleKeyDown(OIS.KC_2, 1.0):
            self.camera.parentSceneNode.detachObject(self.camera)
            self.camNode = self.sceneManager.getSceneNode("CamNode2")
            self.sceneManager.getSceneNode("PitchNode2").attachObject(self.camera)

        if self._isToggleKeyDown(OIS.KC_3, 0.50):
            self.toggle = not self.toggle
            print self.toggle
            self.camera.setAutoTracking(self.toggle, self.mAnimNode)
            
        return SampleFramework.FrameListener.frameStarted(self, evt)

     
class TutorialApplication(SampleFramework.Application):

    def __init__(self):
        self.animationStates = []
        self.animationSpeeds = []
        self.sceneNodes=[]
        self.AnimNode=[]
        
        SampleFramework.Application.__init__(self)
    
    def _createScene(self):        
        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.shadowTechnique = ogre.SHADOWTYPE_STENCIL_ADDITIVE


        entity = sceneManager.createEntity("Ninja", "ninja.mesh")
        ninjaNode = sceneManager.getRootSceneNode().createChildSceneNode("NinjaNode",ogre.Vector3(0,0,0))
        ninjaNode.yaw( ogre.Degree(-180)) 
        ninjaNode.attachObject(entity)
        entity.castShadows = True

        
        entity = sceneManager.createEntity("Wanda", "fish.mesh")
        fishNode = sceneManager.getRootSceneNode().createChildSceneNode("fishNode",ogre.Vector3(0,0,0))
        fishNode.setScale (5,5,5)
        fishNode.attachObject(entity)
        entity.castShadows = True
        self.AnimNode = fishNode
        self.sceneNodes.append(fishNode )

        fishAnim = entity.getAnimationState('swim')
        fishAnim.enabled = True
        self.animationStates.append(fishAnim)
        self.animationSpeeds.append(ogre.Math.RangeRandom(0.5, 1.5))


        fishAnimationNode = sceneManager.getRootSceneNode().createChildSceneNode("fishAnimationNode")
        self.sceneNodes.append(fishAnimationNode)
        
        animation = sceneManager.createAnimation('fishTrack', 10)
        animation.interpolationMode = ogre.Animation.IM_SPLINE 
        animationTrack = animation.createNodeTrack(0, fishAnimationNode) 
        key = animationTrack.createNodeKeyFrame(0.0) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(1.0) 
        key.setTranslate (ogre.Vector3(80.0, 0.0, -40.0) )
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(2.0) 
        key.setTranslate ( ogre.Vector3(120.0, 0.0, 0.0)) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(3.0) 
        key.setTranslate(ogre.Vector3(80.0, 0.0, 80.0) )
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(5.0) 
        key.setTranslate ( ogre.Vector3(-80.0, 0.0, -40.0) )
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(6.0) 
        key.setTranslate ( ogre.Vector3(-120.0, 0.0, 0.0) )
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(7.0) 
        key.setTranslate ( ogre.Vector3(-80.0, 0.0, 100.0)) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(8.0) 
        key.setTranslate ( ogre.Vector3(30.0, 0.0, 90.0) )
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(9.0) 
        key.setTranslate( ogre.Vector3(-20.0, 0.0, 40.0) )
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(10.0) 
        key.setTranslate ( ogre.Vector3(0.0, 0.0, 0.0) )
        key.setScale (ogre.Vector3(5,5,5))
        
        fishAnimationState = sceneManager.createAnimationState('fishTrack')        
        fishAnimationState.Enabled = True

        self.animationStates.append(fishAnimationState)
        self.animationSpeeds.append(ogre.Math.RangeRandom(0.5, 1.5))

        fishLookAtNode = sceneManager.getRootSceneNode().createChildSceneNode("fishLookAtNode") 
        self.sceneNodes.append(fishLookAtNode)
        
        animation = sceneManager.createAnimation('fishLookAtTrack', 10) 
        animation.interpolationMode = ogre.Animation.IM_SPLINE 
        animationTrack = animation.createNodeTrack(0, fishLookAtNode) 
        key = animationTrack.createNodeKeyFrame(0.0) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(1.0) 
        key.Translate = ogre.Vector3(80.0, 0.0, -40.0) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(2.0) 
        key.Translate = ogre.Vector3(120.0, 0.0, 0.0) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(3.0) 
        key.Translate = ogre.Vector3(80.0, 0.0, 80.0) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(5.0) 
        key.Translate = ogre.Vector3(-80.0, 0.0, -40.0) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(6.0) 
        key.Translate = ogre.Vector3(-120.0, 0.0, 0.0) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(7.0) 
        key.Translate = ogre.Vector3(-80.0, 0.0, 100.0) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(8.0) 
        key.Translate = ogre.Vector3(30.0, 0.0, 90.0) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(9.0) 
        key.Translate = ogre.Vector3(-20.0, 0.0, 40.0) 
        key.setScale (ogre.Vector3(5,5,5))
        key = animationTrack.createNodeKeyFrame(10.0) 
        key.Translate = ogre.Vector3(0.0, 0.0, 0.0) 
        key.setScale (ogre.Vector3(5,5,5))

        fishLookAtAnimationState = sceneManager.createAnimationState('fishLookAtTrack') 
        fishLookAtAnimationState.Enabled = True 
        fishLookAtAnimationState.addTime(0.033) #Gives a 33ms headstart to the lookat node
        
        self.animationStates.append(fishLookAtAnimationState)
        self.animationSpeeds.append(ogre.Math.RangeRandom(0.5, 1.5))

        print "\n\n\n\n"
        print self.animationStates
        print "\n\n\n\n"

        plane = ogre.Plane(ogre.Vector3(0, 1, 0), -10)
        mm = ogre.MeshManager.getSingleton()
        mm.createPlane('ground', 'general', plane, 1500, 1500, 20, 20,
                       True, 1, 5, 5, ogre.Vector3(0, 0, 1),
                       ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, 
                       True,True )

        ent = sceneManager.createEntity("GroundEntity", "ground")
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(ent)        
        ent.setMaterialName("Examples/Rockwall")
        ent.castShadows = False

        light = sceneManager.createLight("Light1")
        light.type = ogre.Light.LT_POINT
        light.setPosition (250, 150, 250)
        light.setDiffuseColour (1, 1, 1)
        light.setSpecularColour (1, 1, 1)

        # create the first camera node/pitch node
        node = sceneManager.getRootSceneNode().createChildSceneNode("CamNode1", ogre.Vector3(0, 100, 300))
        node.yaw(ogre.Degree(0))  # look at the ninja
        node = node.createChildSceneNode("PitchNode1")
        node.attachObject(self.camera)

        # create the second camera node/pitch node
        node = sceneManager.getRootSceneNode().createChildSceneNode("CamNode2", ogre.Vector3(-100, 100, 400))
        node.createChildSceneNode("PitchNode2")

    def _createCamera(self):
        self.camera = self.sceneManager.createCamera("PlayerCam")
        self.camera.nearClipDistance = 5
        self.camera.setPosition (0, 60, 0)
        self.camera.lookAt(ogre.Vector3(0, 0, 0))

    def _createFrameListener(self):
        self.frameListener = TutorialFrameListener(self.renderWindow, self.camera, self.sceneManager,
                            self.animationStates, self.animationSpeeds, self.sceneNodes, self.AnimNode )
        self.root.addFrameListener(self.frameListener)
        self.frameListener.showDebugOverlay(True)

     
if __name__ == '__main__':
    try:
        ta = TutorialApplication()
        ta.go()
    except ogre.OgreException, e:
        print e

