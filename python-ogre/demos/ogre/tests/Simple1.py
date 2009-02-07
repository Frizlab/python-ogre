import ogre.renderer.OGRE as ogre 
import SampleFramework as sf 
  
  
class TutorialApplication(sf.Application): 
  
    def _createScene(self):                
        overlayManager = ogre.OverlayManager.getSingleton() 
        overlay = overlayManager.create ( "overlay" ) 
        panel = overlayManager.createOverlayElement ( "Panel", "container1" ) 
        panel.setDimensions ( 1, 1 ) 
        panel.setPosition ( 0, 0 ) 
        overlay.add2D ( panel ) 
        overlay.show ( ) 
        
        textBox = overlayManager.createOverlayElement ( "TextArea", "txt1" ) 
        textBox.setDimensions ( 500.0, 300.0 ) 
        textBox.setMetricsMode ( ogre.GMM_PIXELS ) 
        textBox.setPosition ( 100.0, 20.0 ) 
#         textBox.setParameter ( "font_name", "bluehigh" ) 
        textBox.setParameter ( "char_height", "40" ) 
        textBox.setCaption ( "Ciao Mondo!" ) 
        
        panel.addChild ( textBox ) 
  
    def _createCamera (self): 
        self.camera =  self.sceneManager.createCamera ('PlayerCam') 
        self.camera.position = (0, 150, -500) 
        self.camera.lookAt ((0, 0, 0)) 
        self.camera.nearClipDistance = 5 
  
    def _createViewports (self): 
        viewport = self.renderWindow.addViewport (self.camera) 
        viewport.backGroundColor = (0, 0, 0) 
        self.camera.aspectRatio = float (viewport.actualWidth) / float (viewport.actualHeight) 
  
  
if __name__ == '__main__': 
    ta = TutorialApplication () 
    ta.go () 