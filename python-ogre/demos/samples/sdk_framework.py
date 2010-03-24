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
from ogre.renderer.ogresdksample import *

context = SampleContext   # easy to use shortcut

class sample (SdkSample):
    """
    A simple framework class to fix any python issues when using the
    sample sdk class(es)
    """
    def __init__ ( self ):
        SdkSample.__init__(self)
        self.mInfo["Help"] = "Not much help here -- enjoy using Python-Ogre.  'F': Stats\b'G' - More stats"
        self.running=True
        
    def frameRenderingQueued(self, evt):
        return (SdkSample.frameRenderingQueued(self, evt) and self.running)

    def keyPressed(self,evt):
        SdkSample.keyPressed(self, evt)
        if evt.key == ois.KC_Q or evt.key == ois.KC_ESCAPE:
            self.running = False
        return True