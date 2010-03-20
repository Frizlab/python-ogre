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
import ogre.io.OIS as ois
import ogre.renderer.ogresdksample as sdk
import sdk_framework as sf
import math as math

class sample (sf.sample):
    def __init__ ( self ):
        sf.sample.__init__(self)
        self.mInfo["Title"] = "Smoke"
        self.mInfo["Description"] = "Demonstrates depth-sorting of particles in particle systems."
        self.mInfo["Thumbnail"] = "thumb_smoke.png"
        self.mInfo["Category"] = "Effects"

    def frameRenderingQueued(self, evt):
        # spin the head around and make it float up and down
        self.mPivot.setPosition(0, math.sin(self.mRoot.getTimer().getMilliseconds() / 150.0) * 10, 0)
        self.mPivot.yaw(ogre.Radian(-evt.timeSinceLastFrame * 1.5))
        return sf.sample.frameRenderingQueued(self, evt)
        
    def setupContent(self):
        mSceneMgr = self.getSceneManager()
        mSceneMgr.setSkyBox(True, "Examples/EveningSkyBox")

        # dim orange ambient and two bright orange lights to match the skybox
        mSceneMgr.setAmbientLight(ogre.ColourValue(0.3, 0.2, 0))
        light = mSceneMgr.createLight()
        light.setPosition(2000, 1000, -1000) 
        light.setDiffuseColour(1, 0.5, 0) 
        light = mSceneMgr.createLight()
        light.setPosition(-2000, 1000, 1000) 
        light.setDiffuseColour(1, 0.5, 0) 

        self.mPivot = mSceneMgr.getRootSceneNode().createChildSceneNode()  # create a pivot node

        # create a child node and attach an ogre head and some smoke to it
        headNode = self.mPivot.createChildSceneNode(ogre.Vector3(100, 0, 0)) 
        headNode.attachObject(mSceneMgr.createEntity("Head", "ogrehead.mesh"))
        headNode.attachObject(mSceneMgr.createParticleSystem("Smoke", "Examples/Smoke"))
        self.mCamera.setPosition(0, 30, 350)

if __name__ == '__main__':
    con = sf.context()
    s = sample ()
    con.go(s)


