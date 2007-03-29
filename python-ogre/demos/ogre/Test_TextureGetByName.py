# This code is in the Public Domain
import  Ogre as ogre
import SampleFramework as sf
import ctypes, math,sys

class TestApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        
        ## getByName returns a 'Resource' object
        tm=ogre.TextureManager.getSingleton()
        for r in tm.getResourceIterator():
            name = r.getName()
            # now try to get it byname
            Res = tm.getByName(name)
            print "**TEST** : ", name, " should be a Texture object", Res
        fm=ogre.FontManager.getSingleton()
        for r in fm.getResourceIterator():
            name = r.getName()
            # now try to get it byname
            Res = fm.getByName(name)
            print "**TEST** : ", name, " should be a Font object", Res
       
        
        ##### code for testing OverlayContainer.getChildIterator
        ##### --strank
        overman = ogre.OverlayManager.getSingleton()
        statspanel = overman.getOverlayElement('Core/StatPanel', False)
        print statspanel
        print dir(statspanel)
        
        # Old/C++ style (works fine)
#         childiter = statspanel.getChildIterator()
#         while childiter.hasMoreElements():
#             name = childiter.getNext().getName()
#             print "found child:", name
        # Using Python Iterators
        for child in statspanel.getChildIterator():
            print "Child:", child.getName()
        
        sys.exit()        
        
        
        

        
if __name__ == '__main__':
    try:
        application = TestApplication()
        application.go()
    except ogre.Exception, e:
        print e
