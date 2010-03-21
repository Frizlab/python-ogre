# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http:#python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http:#www.ogre3d.org/
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
        self.mInfo["Title"] = "Transparency"
        self.mInfo["Description"] = "Demonstrates the use of transparent materials (or scene blending)."
        self.mInfo["Thumbnail"] = "thumb_trans.png"
        self.mInfo["Category"] = "Effects"

    def frameRenderingQueued(self, evt):
        theta = self.mRoot.getTimer().getMilliseconds() / 1000.0

        # this is the equation for a PQ torus knot
        r = 28 * (2 + math.sin(theta * 3 / 2 + 0.2))
        x = r * math.cos(theta)
        y = r * math.sin(theta)
        z = 60 * math.cos(theta * 3 / 2 + 0.2)

        lastPos = self.mFishNode.getPosition();   # save fishy's last position
        self.mFishNode.setPosition(x, y, z);              # set fishy's new position

        # set fishy's direction based on the change in position
        self.mFishNode.setDirection(self.mFishNode.getPosition() - lastPos, ogre.Node.TS_PARENT, ogre.Vector3().NEGATIVE_UNIT_X )

        self.mFishSwim.addTime(evt.timeSinceLastFrame * 5);   # update fishy's swimming animation

        return sf.sample.frameRenderingQueued(self, evt)

    def setupContent(self):
        self.mSceneMgr.setSkyBox(True, "Examples/TrippySkyBox")

        self.mCamera.setPosition(0, 0, 300)   # set camera's starting position

        self.mSceneMgr.createLight().setPosition(20, 80, 50)   # add basic point light

        # create a torus knot model, give it the translucent texture, and attach it to the origin
        ent = self.mSceneMgr.createEntity("Knot", "knot.mesh")
        ent.setMaterialName("Examples/WaterStream")
        self.mSceneMgr.getRootSceneNode().attachObject(ent)

        # create a fishy and enable its swimming animation
        ent = self.mSceneMgr.createEntity("Fish", "fish.mesh")
        self.mFishSwim = ent.getAnimationState("swim")
        self.mFishSwim.setEnabled(True)

        # create a scene node, attach fishy to it, and scale it by a factor of 2
        self.mFishNode = self.mSceneMgr.getRootSceneNode().createChildSceneNode()
        self.mFishNode.attachObject(ent)
        self.mFishNode.setScale(2, 2, 2)

if __name__ == '__main__':
    con = sf.context()
    s = sample ()
    con.go(s)

