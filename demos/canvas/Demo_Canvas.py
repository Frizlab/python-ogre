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
import os
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ogre.gui.canvas as canvas
import ogre.io.OIS as OIS

from math import *


class SkyBoxApplication(sf.Application):
    def __del__ (self ):
        del self.doc
        sf.Application.__del__(self)
        
    def _setUpResources ( self ):
        # first load the default resources
        sf.Application._setUpResources ( self )
        # Now load any extra resource locations that we might need.. 
        path = os.path.join(os.getcwd(), 'media' ) 
        ogre.ResourceGroupManager.getSingleton().addResourceLocation(path,"FileSystem", "General")

    def _createScene(self):
        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.setSkyBox(True, "Examples/SpaceSkyBox")
        
        # Need a light 
        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 50)

        self.doc = canvas.Document(sceneManager, self.viewport, "DemoLayout.txt")

        
    def _createFrameListener(self):
        self.frameListener = SkyBoxListener(self.renderWindow, self.camera, self.sceneManager, self)
        self.frameListener.doc = self.doc
        self.root.addFrameListener(self.frameListener)


class SkyBoxListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, sceneManager, app):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.sceneManager = sceneManager
        self.app=app
        self.lastTime = 0
        self.maxDrawsPerSecond = 1.0/50
        self.animateText = False
        self.totalTime = 0
        self.wireframe= False
                          
    def frameRenderingQueued(self, frameEvent):
        retval = sf.FrameListener.frameRenderingQueued(self, frameEvent)
        time = frameEvent.timeSinceLastFrame
        self.totalTime += time
        app=self.app
 
        self.lastTime += time
        if self.lastTime < self.maxDrawsPerSecond:
            return retval  # not time to update canvas
                
        self.lastTime = 0                

        backstory = app.doc.getElementByName("backstory")
        stats = app.doc.getElementByName("stats")
        pulser = app.doc.getElementByName("pulser")

        pulser.addTime(int(frameEvent.timeSinceLastFrame*1000))

        if self.animateText:
            delta =  (sin(self.totalTime*0.7) + 1) / 2
            backstory.setSize(int(320 + 100 * delta), int(465 + 70 * -delta))

        frameStats = app.renderWindow.getStatistics()
        stats.setText("FPS: " + str(int(frameStats.lastFPS)) + "\nBatch: " + str(frameStats.batchCount))

        if self._isToggleKeyDown(OIS.KC_F1, 0.4):
			stats = frameStats
			ogre.LogManager.getSingleton().logMessage("Current FPS: " + str(stats.lastFPS))
			ogre.LogManager.getSingleton().logMessage("Triangle Count: " + str(stats.triangleCount))
			ogre.LogManager.getSingleton().logMessage("Batch Count: " + str(stats.batchCount))
        if self._isToggleKeyDown(OIS.KC_F2, 0.4):
            self.wireframe = not self.wireframe
            if self.wireframe:
                self.camera.setPolygonMode(ogre.PM_SOLID)
            else:
                self.camera.setPolygonMode(ogre.PM_WIREFRAME)
        if self._isToggleKeyDown(OIS.KC_F3, 0.4):
            self.animateText = not self.animateText
            
        self.app.doc.update()
        return retval

if __name__ == '__main__':
    try:
        application = SkyBoxApplication()
        application.go()
    except ogre.OgreException, e:
        print e
