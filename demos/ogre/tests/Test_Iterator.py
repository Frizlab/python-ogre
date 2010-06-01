## test iterators
# This code is in the Public Domain
import  ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ctypes, math, sys

class TestApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera
        
        anames = camera.getAnimableValueNames()
        print len(anames), anames
        for n in anames:
            print "Camera AnimableValue: ", n

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        l = sceneManager.createLight("MainLight") 
        l.setType(ogre.Light.LT_DIRECTIONAL) 
        l.setDirection(-0.5, -0.5, 0) 
        anames = l.getAnimableValueNames()
        print len(anames), anames
        for n in anames:
            print "Light AnimableValue:", n
        
        av = l.createAnimableValue(anames[0])
        print "\nAnimableValue:", av
        print av.type, av.ValueType
        
        
        sceneManager.setSkyDome(True, 'Examples/CloudySky', 5.0, 8.0)
        e = sceneManager.createEntity('ninja', 'ninja.mesh')

        s = self.sceneManager.createStaticGeometry("bing") 
        
        ## Lets fill in the static geometry 
        for  x in range(5):
            pos = ogre.Vector3 ( x*100, 1,1) 
            orientation = ogre.Quaternion()
            orientation.FromAngleAxis(
                ogre.Degree(15),
                ogre.Vector3().UNIT_Y)
            scale = ogre.Vector3( 1, 1, 1)
            s.addEntity(e, pos, orientation, scale) 
        s.build() 
        
        print "\n\n  About to test Iterators \n\n"
        for reg in s.getRegionIterator():   ##
            for lod in reg.getLODIterator(): 
                for mat in lod.getMaterialIterator():  
                    for geom in mat.getGeometryIterator(): 
                        print "Iterators:", reg, lod, mat, geom
                        
        ## If it doesn't crash then it must be OK :)
        print"\n\n   Iterators OK !!\n\n"
        
        print "testing plugins"
        pit  = self.root.getInstalledPlugins()
        print dir(pit)
        for p in pit:
            print "Name %s" %  (p.getName())

            
        # create head entity
        headNode = sceneManager.getRootSceneNode().createChildSceneNode()
        print "*******", headNode
        entity = sceneManager.createEntity('head', 'ogrehead.mesh')
        headNode.attachObject(entity)
        childnode = headNode.createChildSceneNode("Child_Base")    
        for x in range(5):
            pos = ogre.Vector3 ( x*100, 1,20) 
            orientation = ogre.Quaternion()
            orientation.FromAngleAxis(
                ogre.Degree(15),
                ogre.Vector3().UNIT_Y)
            scale = ogre.Vector3( 1, 1, 1)
            
#             sn = childnode.createChild()
#             sn = sn.createChildSceneNode()
#             se = sceneManager.createEntity('snetity'+str(x), 'ogrehead.mesh')
#             sn.attachObject(se)
#             sn.position = pos
#             sn.orientation = orientation
            childnode = childnode.createChildSceneNode("Child_"+str(x))
            ce = sceneManager.createEntity('Childeetity'+str(x), 'ogrehead.mesh')
            childnode.attachObject(ce)
            childnode.position = pos
            childnode.orientation = orientation
        childItor = headNode.getChildIterator()
        print "****", childItor, dir(childItor)
        for c in childItor:
         print "##", c
        while(childItor.hasMoreElements()): 
            child = childItor.getNext().castAsSceneNode() 
            print child.name
            objItor = child.getAttachedObjectIterator() 
            while objItor.hasMoreElements():
                print "OBJ:", objItor.getNext()
            
            
            
            
if __name__ == '__main__':
    import exceptions
    try:
        application = TestApplication()
        application.go()
    except ogre.OgreException, e:
        print e
#        print dir(e)
    except exceptions.RuntimeError, e:
        print "Runtime error:", e
    except exceptions.TypeError, e:
        print "Type error:", e
    except exceptions.AttributeError, e:
        print "Attribute error:", e
    except exceptions.NameError, e:
        print "Name error:", e
    except Exception,inst:
        print "EException"
        print type(inst)     # the exception instance
        print inst.args      # arguments stored in .args
        print inst
    except exceptions.ValueError,e:
        print "ValueError",e
    except :
        print "Unexpected error:", sys.exc_info()[0]