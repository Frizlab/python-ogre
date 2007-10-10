import ogre.renderer.OGRE as ogre 
import SampleFramework as sf 
import ctypes


class EnvMapApplication(sf.Application): 
    def _createScene( self ): 
        sceneManager = self.sceneManager 
        camera = self.camera 
        
        sceneManager.ambientLight = ogre.ColourValue (0.5, 0.5, 0.5) 

        light = sceneManager.createLight('MainLight') 
        light.setPosition (20, 80, 50) 

        entity = sceneManager.createEntity('head', 'ogrehead.mesh') 
        entity.setMaterialName("Examples/EnvMappedRustySteel") 
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(entity) 
        print "\n\nCHECKING OPTIONS"
        svO = ogre.StringVector()
        options = sceneManager.getOptionKeys(svO)
        for o in svO:
            print "Option: ", o
            svV = ogre.StringVector()
            values = sceneManager.getOptionValues(o, svV)
            for v in  svV:
                print "Possible Value: ", v
                
        ## octscenemanager (default) has the following options
        ## Size -- AxisAligneBox
        ## Depth -- Int
        ## ShowOctree -- bool
        ret,size = sceneManager.getOption ( 'Size', ogre.AxisAlignedBox() )
        print "Size: ", size
        ret,depth = sceneManager.getOption ( 'Depth', int(1) )
        print "Depth: ", depth
        ret,value = sceneManager.getOption ( 'ShowOctree', bool(0) )
        print "ShowOctree: ", value
        
        sceneManager.setOption( 'ShowOctree', True)
        sceneManager.setOption( 'Depth',depth)
        print dir(size)
        
        size.setMaximum((100,100,100))
        size.setMinimum((0,0,0))
        sceneManager.setOption( 'Size', size)
if __name__ == '__main__': 
 
    application = EnvMapApplication() 
    application.go() 
