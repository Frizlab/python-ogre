# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http:##python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http:##www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------


## This is a very ugly hack in progress to test collisions (without Physics)
## 
## Currently it uses the Ogre Intersection Query functions will be cleaned up and
## replaced with Opcode to see if there is a performance difference
##

import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import random, sys
import ogre.physics.Opcode as opcode
from math import *

class OpcodeBoxesApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        ## Set ambient light
        sceneManager.setAmbientLight ( (0.6, 0.6, 0.6) )
        ## Create a skydome
        sceneManager.setSkyDome (True, "Examples/CloudySky",5,8)
        
        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 50)

        plane = ogre.Plane()
        plane.normal = ogre.Vector3.UNIT_Y
        plane.d = 200
        mm = ogre.MeshManager.getSingleton()
        mm.createPlane('FloorPlane', 'General', plane, 200000.0, 200000.0,
               20, 20, True, 1, 50, 50, ogre.Vector3.UNIT_Z)
               
        entity = sceneManager.createEntity('floor', 'FloorPlane')
        entity.setMaterialName('Examples/RustySteel')
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(entity)

        # create head entity
        headNode = sceneManager.getRootSceneNode().createChildSceneNode()
        entity = sceneManager.createEntity('head', 'ogrehead.mesh')
        headNode.attachObject(entity)

        # make sure the camera track this node
#         self.camera.setAutoTracking(True, headNode)

        # create the camera node & attach camera
        cameraNode = sceneManager.getRootSceneNode().createChildSceneNode()
        cameraNode.attachObject(self.camera)
        
        # add some fog
        sceneManager.setFog(ogre.FOG_EXP, (1, 1, 1), 0.0002)  ## change .0002 to .002 for lots of fog
        self.numBoxes  =1000
    
            

                
    def _createFrameListener(self):
        self.frameListener = OpcodeBoxesListener(self.renderWindow, self.camera, self.sceneManager, self.numBoxes)
        self.root.addFrameListener(self.frameListener)
        
class OpcodeBoxesQueryListener(ogre.IntersectionSceneQueryListener):
    def __init__ ( self ):
        ogre.IntersectionSceneQueryListener.__init__ ( self )
        
    def queryResult (  self, first, second ):
        # Lets change the material so I can see the intersecting objects
        first.setMaterialName('Examples/Chrome')
        second.setMaterialName('Examples/TextureEffect2')
        return True
        
      
class OpcodeBoxesListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, sm, num):
        sf.FrameListener.__init__(self, renderWindow, camera)
        # Create an intersection query
        self.intersectSceneQuery =  sm.createIntersectionQuery()
        # and a listener to receive the results
        self.querylistener = OpcodeBoxesQueryListener()
        
        # I then create a range of crates and barrels to test with
        self.numBoxes = num
        self.sceneManager = sm
        self.CreateBoxes ( num )
        
    def frameStarted(self, frameEvent):
        self.intersectSceneQuery.execute( self.querylistener )
        return sf.FrameListener.frameStarted(self, frameEvent)
    def frameEnded ( self, ev ):
        self.UpdateBoxes()
        return sf.FrameListener.frameEnded(self, ev)
        
    def CreateBoxes (self, num):
        self.numBoxes=num
        self._bodies=[]
        names = ['WoodPallet','Barrel']
        self.mBoxes = [] #opcode.AABB[mNbBoxes];
        self.mBoxPtrs = []   #new const AABB*[mNbBoxes];
        self.mBoxTime = []   #new float[mNbBoxes];
        self.nodes = []
        self.mSpeed = .000001
        self.mAmplitude = .5
        
        for i in range ( self.numBoxes ):
        
            ## Create the visual representation (the Ogre entity and scene node)
            name = "object" + str(i)
            entity = self.sceneManager.createEntity(name, names[i % len(names)] + ".mesh")
            
            node = self.sceneManager.getRootSceneNode().createChildSceneNode(name)
            node.attachObject(entity)
            entity.setNormaliseNormals(True)
            entity.setCastShadows(True)
            
            ## Pick a size
            scale= 100
            size = ogre.Vector3((random.random() * 0.5 + 0.1) * scale,
                        (random.random() * 0.5 + 0.1) * scale,
                        (random.random() * 0.5 + 0.1) * scale)
            position = opcode.Point((random.random() -0.5 ) * 800.0,
                        (random.random() - 0.5 ) * 500.0,
                        (random.random() - 0.5 ) * 800.0)
            extents = opcode.Point(2 + random.random() * 2.0,
                        2.0 + random.random() * 2.0,
                        2.0 + random.random() * 2.0)
                   
            node.setScale (size.x * 0.1,size.y * 0.1,size.z * 0.1) 
            node.setPosition (position.x, position.y, position.z)
                       
            self._bodies.append ( entity)
            box = opcode.AABB()
            box.SetCenterExtents(position, extents)
            self.mBoxes.append (box)
            self.nodes.append( node)
            self.mBoxTime.append ( 2000.0*random.random() )
        

    def UpdateBoxes(self):
        for i in range ( self.numBoxes):
            self.mBoxTime[i] += self.mSpeed
            Extents = opcode.Point()
            Center = opcode.Point()
            
            self.mBoxes[i].GetExtents(Extents)
            n = self.nodes[i]
            Center.x = cos(self.mBoxTime[i]*2.17)*self.mAmplitude + sin(self.mBoxTime[i])*self.mAmplitude*0.5
            Center.y = cos(self.mBoxTime[i]*1.38)*self.mAmplitude + sin(self.mBoxTime[i]*self.mAmplitude)
            Center.z = sin(self.mBoxTime[i]*0.777)*self.mAmplitude
            pos = n.Position
            pos.x += Center.x
            pos.y += Center.y
            pos.z += Center.y
            
            n.Position = pos
            if i % 2  :
                self._bodies[i].setMaterialName('RustyBarrel')
            else:
                self._bodies[i].setMaterialName('WoodPallet')
    
if __name__ == '__main__':
    try:
        application = OpcodeBoxesApplication()
        application.go()
    except ogre.OgreException, e:
        print e

    
