import os,sys 
if sys.platform == 'win32': 
    os.environ['PATH'] += ';' + __file__[0] 

import ogre.renderer.OGRE as ogre 
import SampleFramework 
import math 

class TestListener(ogre.RenderTargetListener):
    def __init__(self, name):
        "Init Render Application"
        ogre.RenderTargetListener.__init__( self )
        self.name = name
        pass
        
    def preRenderTargetUpdate(self,evt):
        print "preRenderTargetUpdate"
           
    def postRenderTargetUpdate(self,evt):
        print "postRenderTargetUpdate"

        
    def preViewportUpdate(self,evt):
        print "preViewportUpdate", name, evt.source
           
    def postViewportUpdate(self,evt):
        print "postViewportUpdate", name, evt.source

class TutorialApplication(SampleFramework.Application): 
    def _createScene(self): 
        sm = self.sceneManager 
        cam = self.camera
        rw = self.renderWindow
        
        print "Cam Position"
        print cam
        cam.setPosition ( 10,11,12)
        print cam
        pos = cam.getPosition()
        print pos
        pos = cam.position
        print pos
        cam.position=20,30,40
        print cam.position
        
        self.mTestCam = sm.createCamera("TestCam")
        self.listener1 = TestListener( "one" )
        self.listener2 = TestListener ( "two" )
        vp1 = rw.getViewport(0)
        vp2 = rw.addViewport ( self.mTestCam, 1 )
        rw.addListener( self.listener )


if __name__ == '__main__': 
    ta = TutorialApplication() 
    ta.go() 
