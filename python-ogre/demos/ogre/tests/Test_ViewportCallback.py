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
               
#     def preRenderTargetUpdate(self,evt):
#         print "preRenderTargetUpdate", self.name, evt.source, evt.source.name
#         print  evt.source.NumViewports
#            
#     def postRenderTargetUpdate(self,evt):
#         print "postRenderTargetUpdate"

    def preViewportUpdate(self,evt):
#         print "preViewportUpdate", self.name, evt.source
#         print dir(evt)
        print evt.source.Camera.name, evt.source.Camera, evt.source.ZOrder
        print evt.RenderQueueInvocationSequenceName
#         print dir(evt.source)
#         print "\n\n"
           
#     def postViewportUpdate(self,evt):
#         print "postViewportUpdate", self.name, evt.source

class TestApplication(SampleFramework.Application): 
    def _createScene(self): 
        sm = self.sceneManager 
        cam = self.camera
        rw = self.renderWindow
        self.listener1 = TestListener( "one" )
        self.listener2 = TestListener ( "two" )
             
        self.mTestCam = self.sceneManager.createCamera('TestCam')
        self.mTestCam.setPosition(ogre.Vector3(0, 0, 200))
        self.mTestCam.lookAt(ogre.Vector3(0, 0, -200))
        self.mTestCam.NearClipDistance = 5

#         sm.destroyAllCameras()
        vp1 = rw.getViewport(0)
        vp1.setDimensions(0,0,.4,.5)
#         vp2 = rw.addViewport ( self.mTestCam, 2 )
        print "VP:", rw.getNumViewports()
#         rw.removeAllViewports()
#         rw.removeAllListeners()
        self.vp2 = rw.addViewport ( self.mTestCam, 2 )
        self.vp2.BackgroundColour = 5,2,0
        self.vp2.setDimensions(.5,.5,.2,.5)
        print dir(self.vp2)

#         self.vp3 = rw.addViewport ( self.mTestCam, 3 )
#         self.vp4 = rw.addViewport ( self.mTestCam, 4 )
        rw.addListener( self.listener1 )
#         rw.addListener( self.listener2 )


if __name__ == '__main__': 
    ta = TestApplication() 
    ta.go() 
