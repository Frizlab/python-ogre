# This code is in the Public Domain
import  ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ctypes, math,sys

class TestApplication(sf.Application):
    def _loadResources(self):
        """ needed to stop the framework loading resources as we want to do it
        """
        pass
        
    def _createScene(self):
        mm = ogre.MaterialManager.getSingleton()
        rm = ogre.ResourceGroupManager.getSingleton()
        ds = rm.openResource("bloom.material", "General", True)
        mm.parseScript(ds, "General")
        print "\n\n"
        for m in mm.getResourceIterator():
            print "Material: ", m.getName()
        print "\n"
        sys.exit()
        
        
    
        
if __name__ == '__main__':
    try:
        application = TestApplication()
        application.go()
    except ogre.OgreException, e:
        print e
