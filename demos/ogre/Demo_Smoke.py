# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http://www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import sys
sys.path.insert(0,'..')
import PythonOgreConfig
import ogre.renderer.OGRE as ogre
import SampleFramework as sf
# import multiprocessing as mp
# import ctypes as ctypes


class SmokeApplication(sf.Application):
    def go1(self, queue):
        "Starts the rendering loop."
        self.queue=queue
        if not self._setUp():
            return
        if self._isPsycoEnabled():
            self._activatePsyco()
        self.root.startRendering()
        
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.setSkyDome(True, 'Examples/CloudySky', 5.0, 8.0)

        self.fountainNode = sceneManager.getRootSceneNode().createChildSceneNode()
        
        psm = ogre.ParticleSystemManager.getSingleton()
        self.particleSystem2 = sceneManager.createParticleSystem("Smoke", "Examples/Smoke") #'fountain1', 'Examples/Smoke')
        node = self.fountainNode.createChildSceneNode()
        node.attachObject(self.particleSystem2)
        ogre.LogManager.getSingleton().logMessage( "INNER COUNT -- ANDY" )
        self.item = ogre.Any ( self.root )
        a1 = ogre.getAddress( self.item )
        print ("Address:", a1)
        #oo = ogre.castAsObject (a1)
        #print "OBJECT:", oo
        #print dir(oo)
        #d = oo.getData()
        #print "DATA:", d
#         self.queue = mp.Queue()
#         p = mp.Process(target=self.test) #args=(q,))
#         p.start()
#
#         self.queue.put ( a1 )
        
    def __del__(self):
        del self.particleSystem2
        sf.Application.__del__(self)     
        

    def test (self):
        count = 0
        inner = 0
        
        while 1:
            a1 = self.queue.get()
            print ("ADDRESS FROM QUEUE:", a1)
            #oo = ogre.castAsObject (a1)
            #d = oo.getData()
            #print "OBJECT FROM QUEUE:", type(oo)
            #print d
        
        
    
if __name__ == '__main__':
    #try:
        application = SmokeApplication()
        application.go()
        #p1 = mp.Process(target=test,args=(q,))
        #p1.start()
        
        
    #except ogre.OgreException, e:
    #    print e
