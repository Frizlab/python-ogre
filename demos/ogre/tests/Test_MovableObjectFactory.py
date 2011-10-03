# This code is in the Public Domain
import  ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ctypes, math,sys

class TestMovableObjectFactory(ogre.MovableObjectFactory):
    FACTORY_TYPE_NAME = None
    def __init__(self):
        ogre.MovableObjectFactory.__init__(self)
        print self.getTypeFlags() 
        print self.FACTORY_TYPE_NAME
        
    def __del__(self):
        pass
        
    def createInstanceImpl( self, name, params ):
        return ogre.MovableObject()
        
    def getType(self):
        return self.FACTORY_TYPE_NAME
        
    def destroyInstance(self, obj ):
        del obj
        
TestMovableObjectFactory.FACTORY_TYPE_NAME = "TestMovableFactory"

class TestApplication(sf.Application):
        
    def _createScene(self):
        self.testMovableObjectFactory = TestMovableObjectFactory()
        if not self.root.getSingleton().hasMovableObjectFactory("TestMovableFactory"):
            self.root.getSingleton().addMovableObjectFactory( self.testMovableObjectFactory )
            
        if not self.root.getSingleton().hasMovableObjectFactory("Should Fail"):
            print "OK #1"            
        if self.root.getSingleton().hasMovableObjectFactory("TestMovableFactory"):
            print "OK #2"
        sys.exit()
        
        
    
        
if __name__ == '__main__':
    try:
        application = TestApplication()
        application.go()
    except ogre.OgreException, e:
        print e
