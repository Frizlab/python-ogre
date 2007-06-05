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
import ogre.renderer.OGRE as ogre
import SampleFramework as sf

class CameraTrackApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        
        sceneManager.setAmbientLight ( (0.7, 0.7, 0.7) )
#        sceneManager.setSkyBox (True, "Examples/CloudySky") ## AJM THIS FAILS FOR SOME REASON?
        sceneManager.setSkyDome (True, "Examples/CloudySky")
        
        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 50)
        

        plane = ogre.Plane()
        plane.normal = ogre.Vector3.UNIT_Y
        plane.d = 200
        mm = ogre.MeshManager.getSingleton()
        mm.createPlane('FloorPlane', 'General', plane, 200000.0, 200000.0,
               20, 20, True, 1, 200.0, 200.0,  (0, 0, 1),
               ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, 
               True,True )


        entity = sceneManager.createEntity('floor', 'FloorPlane')
        entity.setMaterialName('Examples/RustySteel')
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(entity)

        # create head entity
        headNode = sceneManager.getRootSceneNode().createChildSceneNode()
        entity = sceneManager.createEntity('head', 'ogrehead.mesh')
        headNode.attachObject(entity)

        #print type(entity[0])
        # make sure the camera track this node
        self.camera.setAutoTracking(True, headNode)

        # create the camera node & attach camera
        cameraNode = sceneManager.getRootSceneNode().createChildSceneNode()
        cameraNode.attachObject(self.camera)
        
        print "\n\n",self.camera,"\n"
#
        # set up spline animation of node
        
        animation = sceneManager.createAnimation('CameraTrack', 10)
        animation.interpolationMode = ogre.Animation.IM_SPLINE
        
        animationTrack = animation.createNodeTrack(0, cameraNode)
#         
        key = animationTrack.createNodeKeyFrame(0)
#         
#         key = animationTrack.createNodeKeyFrame(2.5)
#         key.setTranslate ( (500.0, 500.0, -1000.0))
#         
#         key = animationTrack.createNodeKeyFrame(5)
#         key.setTranslate (  (-1500.0, 1000.0, -600.0))
#         
#         key = animationTrack.createNodeKeyFrame(7.5)
#         key.setTranslate ( (0.0, -100.0, 0.0))
#         
#         key = animationTrack.createNodeKeyFrame(10.0)
#         key.setTranslate (  (0.0, 0.0, 0.0))

        print "\n\n About to do a clone"
#         newAnim = ogre.Animation('NewTrack', 10);
#         newAnim.mInterpolationMode = animation.InterpolationMode
#         newAnim.mRotationInterpolationMode = animation.RotationInterpolationMode
        print animation
        newanim = animation.clone ('NewTrack')
        print "\n",newanim,"\n"
        
        
        self.animationState = sceneManager.createAnimationState('NewTrack')
        
#         self.animationState = sceneManager.createAnimationState('CameraTrack')
        self.animationState.setEnabled (True)


        
        # add some fog
        sceneManager.setFog(ogre.FOG_EXP, (1, 1, 1), 0.0002)

    def _createFrameListener(self):
        self.frameListener = CameraTrackListener(self.renderWindow, self.camera, self.animationState)
        self.root.addFrameListener(self.frameListener)

class CameraTrackListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, animationState):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.animationState = animationState

    def frameStarted(self, frameEvent):
        #MyTest #Crash    
        self.animationState.addTime(frameEvent.timeSinceLastFrame)
        return sf.FrameListener.frameStarted(self, frameEvent)



if __name__ == '__main__':
    try:
        application = CameraTrackApplication()
        application.go()
    except ogre.OgreException, e:
        print e

    
