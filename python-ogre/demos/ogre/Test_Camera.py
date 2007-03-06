import os,sys 
if sys.platform == 'win32': 
    os.environ['PATH'] += ';' + __file__[0] 

import Ogre as ogre 
import SampleFramework 
import math 

class TutorialApplication(SampleFramework.Application): 
    def _createScene(self): 
        sm = self.sceneManager 
        cam = self.camera
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
        
        sys.exit()

if __name__ == '__main__': 
    ta = TutorialApplication() 
    ta.go() 
