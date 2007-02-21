## test iterators
# This code is in the Public Domain
import  Ogre as ogre
import SampleFramework as sf
import ctypes, math, sys

class TestApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.setSkyDome(True, 'Examples/CloudySky', 5.0, 8.0)
        e = sceneManager.createEntity('ninja', 'ninja.mesh')

        s = self.sceneManager.createStaticGeometry("bing") 
        
        ## Lets fill in the static geometry 
        for  x in range(5):
            pos = ogre.Vector3 ( x*100, 1,1) 
            orientation = ogre.Quaternion()
            orientation.FromAngleAxis(
                ogre.Degree(15),
                ogre.Vector3.UNIT_Y) 
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

if __name__ == '__main__':
    import exceptions
    try:
        application = TestApplication()
        application.go()
    except ogre.Exception, e:
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