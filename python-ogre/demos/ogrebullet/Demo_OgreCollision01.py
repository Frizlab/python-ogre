##
## This is a very ugly hack in progress to test collisions (without Physics)
## 
## It uses the Ogre Intersection Query functions with the intention of extending it
## to use Opcode to see if there is a performance difference
##
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.physics.bullet as bullet
import ogre.physics.OgreBulletC as bulletC
import ogre.physics.OgreBulletD as bulletD

import SampleFramework as sf
import random, sys
from math import *

BULLET = True

# enum QueryFlags
# {
# 	ANY_QUERY_MASK					= 1<<0,
# 	RAGDOLL_QUERY_MASK				= 1<<1,
# 	GEOMETRY_QUERY_MASK				= 1<<2,
# 	VEHICLE_QUERY_MASK				= 1<<3,
# 	STATIC_GEOMETRY_QUERY_MASK		= 1<<4
# };
           
class OgreCollideApplication(sf.Application):
    #
    # first lets create a nice scene with a floor plane (that also get's collided
    #
    def _createScene(self):
        sceneManager = self.sceneManager
        ## Set ambient light
        sceneManager.setAmbientLight ( (0.6, 0.6, 0.6) )
        
#         sceneManager.setWorldGeometry('terrain.cfg')

        ## Create a skydome
        sceneManager.setSkyDome (True, "Examples/CloudySky",5,8)
        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 50)

        # create the floor
        plane = ogre.Plane()
        plane.normal = ogre.Vector3().UNIT_Y
        plane.d = 200
        mm = ogre.MeshManager.getSingleton()
        mm.createPlane('FloorPlane', 'General', plane, 200000.0, 200000.0,
               20, 20, True, 1, 50, 50, ogre.Vector3().UNIT_Z)
        entity = sceneManager.createEntity('floor', 'FloorPlane')
        entity.setMaterialName('Examples/RustySteel')
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(entity)

        # create the camera node & attach camera
        cameraNode = sceneManager.getRootSceneNode().createChildSceneNode()
        cameraNode.attachObject(self.camera)
        
        # how many barrels and pallets do we want        
        self.numBoxes = 100
        if BULLET:
            bounds = ogre.AxisAlignedBox  ((-1000, -1000, -1000),(1000,  1000,  1000))
            self.world = bulletC.CollisionsWorld(self.sceneManager, bounds )
            
    # we need a framelistener to make life easy    
    def _createFrameListener(self):
        self.frameListener = OgreCollideListener(self.renderWindow, self.camera, self.sceneManager, self.numBoxes, self)
        self.root.addFrameListener(self.frameListener)
        
#     def _chooseSceneManager(self):
#         self.sceneManager = self.root.createSceneManager("TerrainSceneManager")
 
               
class OgreCollideQueryListener(ogre.IntersectionSceneQueryListener):
    """ the listener that gets called for each pair of intersecting objects """   
    def __init__ ( self ):
        ogre.IntersectionSceneQueryListener.__init__ ( self )
        
    def queryResult (  self, first, second ):
        """ the actual function that gets called - for the demo I simply change the material type
        so we see the collisions - except I don't change the floor material"""
        if first.name != "floor":
            first.setMaterialName('Examples/Chrome')
        if second.name != "floor":
            second.setMaterialName('Examples/TextureEffect2')
        return True
      
class OgreCollideListener(sf.FrameListener):
    """ the frame listener we are using"""
    
    def __init__(self, renderWindow, camera, sm, num, app):
        sf.FrameListener.__init__(self, renderWindow, camera)
        if not BULLET:
            # Create an intersection query
            self.intersectSceneQuery =  sm.createIntersectionQuery()
            # and a listener to receive the results
            self.querylistener = OgreCollideQueryListener()
        
        # I then create a range of crates and barrels to test with
        self.numBoxes = num
        self.sceneManager = sm
        self.parent = app
        self.frametime = 0
        self.entitiesInstanced = 0
        self.paused = False
        self.doOnestep = False
        self.entities =[]
        self.shapes=[]
        self.bodies=[]
        self.CreateBoxes ( num )
        
    def frameStarted(self, frameEvent):
        """ called each frame (at the start), check if it's time to do collision checking and 
        if so do it, otherwise pass through"""
        self.frametime += frameEvent.timeSinceLastFrame 
        print dir (self.parent.world.getBulletCollisionWorld()       )
        sys.exit()
        # for performance reasons lets check for collisions about 5 times a second 
        if self.frametime > 0.2:
            self.ResetBoxes()
            if BULLET:
                if not ( self.paused or self.doOnestep):
                    self.parent.world.getBulletCollisionWorld().performDiscreteCollisionDetection()
#                     objs = self.parent.world.getBulletCollisionWorld().getCollidingObjects()
#                     if len(objs) > 0:
#                         print "Colliding", objs
            else:
                self.intersectSceneQuery.execute( self.querylistener )
            self.frametime = 0
        return sf.FrameListener.frameStarted(self, frameEvent)
        
    def frameEnded ( self, ev ):
        """and at the end of the frame we update the objects positions in preparation for the 
        next frame"""
        self.UpdateBoxes(ev.timeSinceLastFrame)
        return sf.FrameListener.frameEnded(self, ev)
        
        
## -------------------------------------------------------------------------
    def addCube( self, instanceName, pos, q, size,
                    mesh = "WoodPallet.mesh",
                    material = "WoodPallet",
                    bodyRestitution=0, bodyFriction=0, bodyMass=0) :
                    
        entity = self.sceneManager.createEntity(
            instanceName + str(self.entitiesInstanced),
            mesh)
      
        entity.setQueryFlags ( 1<<2 ) ##GEOMETRY_QUERY_MASK)
        entity.setCastShadows(True)
    
        entity.setMaterialName(material)
    
        sceneCubeShape = bulletC.BoxCollisionShape(size)
    
        ## if we want dynamics we do this
#         defaultBody = bulletD.RigidBody(
#             "defaultCubeRigid" + StringConverter::toString(mNumentitiesInstanced), 
#             mWorld);

        ## it's just collisions so this
        defaultBody = bulletC.Object(
            "BulletCube" + str(self.entitiesInstanced), 
            self.parent.world, True)
        print "Setting default body", sceneCubeShape, pos, q
        defaultBody.setShape ( sceneCubeShape, pos, q)
        
# #         node = self.parent.sceneManager.getRootSceneNode ().createChildSceneNode ()
        node = defaultBody.getRootNode()
        
        node.attachObject (entity)
        ##node.attachObject(defaultBody)
        self.nodes.append ( node )
        self.entities.append(entity)
        self.shapes.append(sceneCubeShape)
        self._bodies.append(entity) # defaultBody)
                        
        self.entitiesInstanced+=1
        return defaultBody;
        
  
            
    def CreateBoxes (self, num):
        self.numBoxes=num
        self._bodies=[]
        names = ['WoodPallet','Barrel']
        self.mBoxTime = []   #new float[mNbBoxes]
        self.nodes = []
        self._shapes = []
        self._bBodies = []
        self.objects=[]
        self.shapes=[]
        self.mSpeed = .000001
        self.mAmplitude = .5
        for i in range ( self.numBoxes ):
            ## Create the visual representation (the Ogre entity and scene node)
            name = "object" + str(i)
            entityname = names[i % len(names)]
            
            ## Pick a size
            scale= 100
            size = ogre.Vector3((random.random() * 0.5 + 0.1) * scale,
                        (random.random() * 0.5 + 0.1) * scale,
                        (random.random() * 0.5 + 0.1) * scale)
            position = ogre.Vector3((random.random() -0.5 ) * 800.0,
                        (random.random() - 0.5 ) * 500.0,
                        (random.random() - 0.5 ) * 800.0)
            q = ogre.Quaternion(0,0,0,1)
            
# # # # #             entity = self.addCube ( name, position, q, size ) 
# # # # #             node = entity.getRootNode()
#             
            entity = self.sceneManager.createEntity(name, entityname + ".mesh")
            
#             entity.setQueryFlags ( 1<<2 ) ##GEOMETRY_QUERY_MASK)
            entity.setCastShadows(True)
    
# # # #             entity.setMaterialName(material)
            

            if BULLET:
                shape = bulletC.StaticMeshToShapeConverter( entity )
                if entityname == 'WoodPalletNONE':
                    shape = shape.createBox()
                else:
                    shape = shape.createCylinder()

                shape = bulletC.BoxCollisionShape(size)                    
                bBody = bulletC.Object( "BulletObject" + str(i), self.parent.world, True)
                print "Setting bbody shape", bBody,shape, position, q
                print dir ( bBody )
                print "Calling setShape"
# #                 bBody.setShape ( shape, position, q)
                print "\n1111"
                node = bBody.getRootNode() ##self.sceneManager.getRootSceneNode().createChildSceneNode()
                node.attachObject(entity)
                self.parent.world.addObject ( bBody )
                print "ADDED SHAPE"
            node.setScale (size.x * 0.1,size.y * 0.1,size.z * 0.1) 
            node.setPosition (position)
            # we need access to the entity and nodes to reset the material           
            self._bodies.append (entity)
            self.nodes.append( node )
            self.objects.append (bBody)
            self.shapes.append ( shape )
            self.mBoxTime.append ( 10.0*random.random() )
        
    def ResetBoxes(self):
        """ set the objects back to their original materials
        """
        for i in range ( self.numBoxes):
            if i % 2  :
                self._bodies[i].setMaterialName('RustyBarrel')
            else:
                self._bodies[i].setMaterialName('WoodPallet')

    def UpdateBoxes(self, timesincelastframe):
        """ move the objects in an interesting way :) """
        for i in range ( self.numBoxes):
            self.mBoxTime[i] += timesincelastframe * 0.001 # self.mSpeed
            Center = ogre.Vector3()
            n = self.objects[i]
            Center.x = cos(self.mBoxTime[i]*2.17)*self.mAmplitude + sin(self.mBoxTime[i])*self.mAmplitude*0.5
            Center.y = cos(self.mBoxTime[i]*1.38)*self.mAmplitude + sin(self.mBoxTime[i]*self.mAmplitude)
            Center.z = sin(self.mBoxTime[i]*0.777)*self.mAmplitude
            pos = n.getRootNode().Position
            pos.x += Center.x
            pos.y += Center.y
            pos.z += Center.y
            n.setPosition (pos)
            
    
if __name__ == '__main__':
    try:
        application = OgreCollideApplication()
        application.go()
    except ogre.OgreException, e:
        print e

    
