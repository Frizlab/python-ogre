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

## Note the first frame doesn't get displayed ...........
import sys
sys.path.insert(0,'..')
import PythonOgreConfig


import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ogre.io.OIS as OIS
import ogre.addons.ogrevideoffmpeg as VIDEO


NUM_VIDEOS = 1
runTime = 0

class VideoApplication(sf.Application):

    def __init__(self):
        "Init Render Application"
        self.videoPlayer  = []
        sf.Application.__init__(self)

    def __del__(self):
        "Init Render Application"
        self.videoPlayer  = []
        sf.Application.__del__(self)

    def _createScene(self):
        global runTime, NUM_VIDEOS
        sceneManager = self.sceneManager
        camera = self.camera
        
        sceneManager.ambientLight = [0.9, 0.9, 0.9]

        # Create the SkyBox
        sceneManager.setSkyBox(True, "Examples/CloudyNoonSkyBox")

        for i in xrange(NUM_VIDEOS):
            c = sceneManager.createEntity("cubo" + str(i), "cube.mesh")
            print "++Entity is:", c
            n = sceneManager.getRootSceneNode().createChildSceneNode()
            print "++Node is:", n
            n.attachObject(c)
            n.setScale(2,2,2)
            n.setPosition(ogre.Vector3(i*250, 0, 0))
            #self.cubeEntities.append(c)

            v = VIDEO.cVideoPlayer("videoMaterial" + str(i),  "konqi_ad1_nl.avi" ) #.ogg")
            print "++Video is:", v 
            #v = VIDEO.cVideoPlayer("videoMaterial" + str(i), "test.divx")
            v.setLoop(True)
            self.videoPlayer.append(v)

        
    def _createFrameListener(self):
        self.frameListener = VideoListener(self.renderWindow, self.camera, self.videoPlayer)
        self.root.addFrameListener(self.frameListener)

class VideoListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, videos):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.videoPlayer = videos
        self.camera = camera
        self.sm =  camera.getSceneManager()
        self.first=True
        self.runTime=0

    def frameStarted(self, frameEvent):
        global runTime, NUM_VIDEOS
        runTime+= frameEvent.timeSinceLastFrame*1000
        for i in xrange(NUM_VIDEOS):
            self.videoPlayer[i].refresh(int(runTime))
            self.sm.getEntity('cubo' + str(i)).setMaterialName("videoMaterial" + str(i))
        
        if (self.Keyboard.isKeyDown(OIS.KC_1)): self.videoPlayer[0].play()
        if (self.Keyboard.isKeyDown(OIS.KC_2)): self.videoPlayer[0].pause()
        if (self.Keyboard.isKeyDown(OIS.KC_3)): self.videoPlayer[0].stop()
        if (self.Keyboard.isKeyDown(OIS.KC_4)): self.videoPlayer[0].goToSecond(20)

        return sf.FrameListener.frameStarted(self, frameEvent)



if __name__ == '__main__':
    try:
        application = VideoApplication()
        application.go()
    except ogre.OgreException, e:
        print e
