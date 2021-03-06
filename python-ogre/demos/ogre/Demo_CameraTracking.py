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
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
##from sf_OIS import * 
import SampleFramework as sf

class CameraTrackApplication(sf.Application):
    def _createScene(self):
        print "11"
        sceneManager = self.sceneManager
        
        sceneManager.setAmbientLight ( (0.7, 0.7, 0.7) )
#        sceneManager.setSkyBox (True, "Examples/CloudySky") ## AJM THIS FAILS FOR SOME REASON?
        sceneManager.setSkyDome (True, 'Examples/CloudySky',4.0,8.0)
        
        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 50)
        

        plane = ogre.Plane()
        plane.normal = ogre.Vector3().UNIT_Y
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
        
        key = animationTrack.createNodeKeyFrame(0)
        
        key = animationTrack.createNodeKeyFrame(2.5)
        key.setTranslate ( (500.0, 500.0, -1000.0))
        
        key = animationTrack.createNodeKeyFrame(5)
        key.setTranslate (  (-1500.0, 1000.0, -600.0))
        
        key = animationTrack.createNodeKeyFrame(7.5)
        key.setTranslate ( (0.0, -100.0, 0.0))
        
        key = animationTrack.createNodeKeyFrame(10.0)
        key.setTranslate (  (0.0, 0.0, 0.0))

        self.animationState = sceneManager.createAnimationState('CameraTrack')
        self.animationState.setEnabled (True)
		
        # add some fog
        sceneManager.setFog(ogre.FOG_EXP, (1, 1, 1), 0.0002)  ## change .0002 to .002 for lots of fog
        shadSetup = ogre.LiSPSMShadowCameraSetup() 
#         shadSetup = ogre.FocusedShadowCameraSetup() 
#         shadSetup = ogre.DefaultShadowCameraSetup() 
        self.TexCam = sceneManager.createCamera("ReflectCam")
        self.TexCam.setCustomViewMatrix(False)
#         self.TexCam.position = (1,0.5,1)
        shadSetup.getShadowCamera (self.sceneManager, self.camera, self.viewport, light, self.TexCam, 0) 
        self.sceneManager.setShadowCameraSetup( shadSetup )
        
        
        pssm = ogre.PSSMShadowCameraSetup() 
        #pssm.calculateSplitPoints(3, 5, shadowFarDistance) 
        #pssm.setSplitPadding(5) 
        #pssm.setOptimalAdjustFactor(0, 2) 
        #pssm.setOptimalAdjustFactor(1, 1) 
        #pssm.setOptimalAdjustFactor(2, 0.5) 
        pssm.getShadowCamera (self.sceneManager, self.camera, self.viewport, light, self.TexCam, 0) 
        
        # next line throws Boost.Python.ArgumentError as of 1.6RC1 
        self.sceneManager.setShadowCameraSetup(pssm) 

        
        mouseRay = self.camera.getCameraToViewportRay( 10,10 )
        raySceneQuery = self.sceneManager.createRayQuery(ogre.Ray())
        raySceneQuery.setRay(mouseRay)
        raySceneQuery.setSortByDistance(True)
        result = raySceneQuery.execute()


    def _createFrameListener(self):
        self.frameListener = CameraTrackListener(self.renderWindow, self.camera, self.animationState,self)
        self.root.addFrameListener(self.frameListener)

class CameraTrackListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, animationState, app):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.app = app
        self.camera=camera
        self.animationState = animationState

    def frameStarted(self, frameEvent):
        #MyTest #Crash    
        self.animationState.addTime(frameEvent.timeSinceLastFrame)
        
        ms = self.Mouse.getMouseState()
        mouseRay = self.app.camera.getCameraToViewportRay( ms.X.abs, ms.Y.abs )
        raySceneQuery = self.app.sceneManager.createRayQuery(ogre.Ray())
        raySceneQuery.setRay(mouseRay)
        raySceneQuery.setSortByDistance(True)
        result = raySceneQuery.execute()
        
        return sf.FrameListener.frameStarted(self, frameEvent)



if __name__ == '__main__':
    try:
        application = CameraTrackApplication()
        application.go()
    except ogre.OgreException, e:
        print e

    
