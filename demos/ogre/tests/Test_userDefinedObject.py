import os,sys, ctypes
if sys.platform == 'win32': 
    os.environ['PATH'] += ';' + __file__[0] 

    
    
import ogre.renderer.OGRE as ogre
import SampleFramework 
import math 



class UD ( ogre.UserDefinedObject ):
    def __init__ ( self, id=99, name='Default' ):
        ogre.UserDefinedObject.__init__(self)
        self.id = id
        self.name = name
        
    def getTypeName ( self ):
        return self.name  
        
    def getTypeID ( self ):
        return self.id      
       
class TutorialApplication(SampleFramework.Application): 


    def _createScene(self): 
        sm = self.sceneManager
         
        ent1 = sm.createEntity("Robot","robot.mesh") 
        ud = UD( )
        ent1.setUserObject ( ud )
        
        nud = ent1.getUserObject()
        print nud
        print nud.id, nud.name
        print nud.getTypeName()
        print nud.getTypeID()   
        
        # now test Any
        list=[1,2,3,4]
        a=ogre.Any(list)
        b=a.getData()
        if len(b) != 2:
            print "ERROR", b
        if b[0] != 'p':
            print "ERROR", b
        if len(b[1]) != 4:
            print "ERROR", b
        print b
        list[1]=12
        print b[1]
        c=ogre.createAny(['us',123])
        d=c.getData()
        print d
        c=ogre.createAny(['l',123])
        d=c.getData()
        print d
        c=ogre.createAny(['ul',123])
        d=c.getData()
        print d
        c=ogre.createAny(['uc',5])
        d=c.getData()
        print d
        c=ogre.createAny(['p',list])
        #del list
        d=c.getData()
        print d
        
        a=ogre.Any( nud )
        print a
        b=a.getData()
        print b
        print b[1].id, b[1].name
        sys.exit()

if __name__ == '__main__': 
    ta = TutorialApplication() 
    ta.go() 
