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
        # Old/C++ style (works fine)
#         childiter = statspanel.getChildIterator()
#         while childiter.hasMoreElements():
#             name = childiter.getNext().getName()
#             print "found child:", name
        # Using Python Iterators
        for child in statspanel.getChildIterator():
            print "Child:", child.getName()
        
        sys.exit()        
###        self.testManualObjectNonIndexed()
#         entiter = self.sceneManager.getMovableObject("test", ogre.ManualObjectFactory.FACTORY_TYPE_NAME)
#         entiter = self.sceneManager.getMovableObjectIterator("test")
        
        
        
    def testManualObjectNonIndexed(self):
            self.sceneManager.setAmbientLight(ogre.ColourValue(0.5, 0.5, 0.5))
            dir1=ogre.Vector3(-1, -1, 0.5)
            dir1.setNormalise()
            l = self.sceneManager.createLight("light1")
            l.setType(ogre.Light.LT_DIRECTIONAL)
            l.setDirection(dir1)
            n = l.getAnimableValueNames()
            v = l.createAnimableValue ('VALUE')
    
            plane = ogre.Plane()
            plane.setNormal ( ogre.Vector3.UNIT_Y )
            plane.d = 100
            ogre.MeshManager.getSingleton().createPlane("Myplane",
                ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, plane,
                1500,1500,10,10,True,1,5,5,ogre.Vector3.UNIT_Z)
            pPlaneEnt = self.sceneManager.createEntity( "plane", "Myplane" )
            pPlaneEnt.setMaterialName("2 - Default")
            pPlaneEnt.setCastShadows(False)
            self.sceneManager.getRootSceneNode().createChildSceneNode().attachObject(pPlaneEnt)
    
            man = self.sceneManager.createMovableObject("test", ogre.ManualObjectFactory.FACTORY_TYPE_NAME)
    
            man.begin("Examples/OgreLogo")
            ## Define a 40x40 plane, non-indexed
            man.setPosition(-20, 20, 20)
            man.setNormal(0, 0, 1)
            man.textureCoord(0, 0)
    
            man.setPosition(-20, -20, 20)
            man.setNormal(0, 0, 1)
            man.textureCoord(0, 1)
    
            man.setPosition(20, 20, 20)
            man.setNormal(0, 0, 1)
            man.textureCoord(1, 0)
    
            man.setPosition(-20, -20, 20)
            man.setNormal(0, 0, 1)
            man.textureCoord(0, 1)
    
            man.setPosition(20, -20, 20)
            man.setNormal(0, 0, 1)
            man.textureCoord(1, 1)
    
            man.setPosition(20, 20, 20)
            man.setNormal(0, 0, 1)
            man.textureCoord(1, 0)
    
            man.end()
    
            man.begin("Examples/BumpyMetal")
    
            ## Define a 40x40 plane, non-indexed
            man.setPosition(-20, 20, 20)
            man.setNormal(0, 1, 0)
            man.textureCoord(0, 0)
    
            man.setPosition(20, 20, 20)
            man.setNormal(0, 1, 0)
            man.textureCoord(0, 1)
    
            man.setPosition(20, 20, -20)
            man.setNormal(0, 1, 0)
            man.textureCoord(1, 1)
    
            man.setPosition(20, 20, -20)
            man.setNormal(0, 1, 0)
            man.textureCoord(1, 1)
    
            man.setPosition(-20, 20, -20)
            man.setNormal(0, 1, 0)
            man.textureCoord(1, 0)
    
            man.setPosition(-20, 20, 20)
            man.setNormal(0, 1, 0)
            man.textureCoord(0, 0)
    
            man.end()
    
    
            self.sceneManager.getRootSceneNode().createChildSceneNode().attachObject(man)
               

        
if __name__ == '__main__':
    try:
        application = TestApplication()
        application.go()
    except ogre.Exception, e:
        print e
