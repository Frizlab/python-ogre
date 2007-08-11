##
## This is a very ugly hack in progress to test collisions (without Physics)
## 
## It uses the Ogre Intersection Query functions with the intention of extending it
## to use Opcode to see if there is a performance difference
##

import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import random, sys
import ogre.physics.Opcode as opcode
from math import *

## use a global Collision entity list with each entiry name formated to allow easy lookup
## into the arry -- ie xxx_0 is in pos 0
CollEntities = []


class CollisionEntity:
#     Opcode.OPCODE_Model* mCollisionModel
#     unsigned int* mIndexList
#       Point* mVertexList
#     Entity* mEntity

    def __init__( self, ent ):
        self.CollisionModel =  opcode.OPCODE_Model()
        self.IndexList= []  ## unsigned int -- ctypes??
        self.VertexList = [] ## opcode.Point
        self.Entity = ent

    def __del__ ( self ):
        pass
        
    def initialise(self):
        self.OPCC = opcode.OPCODECREATE() 
        numVertices = 0
        numIndices = 0
        useSharedVertices = False
    
        if not self.Entity:
            return False
    
        pMesh = self.Entity.getMesh()
    
        for i in range ( pMesh.getNumSubMeshes() ):
            pSubMesh = pMesh.getSubMesh(i)
            if pSubMesh.useSharedVertices:
                useSharedVertices = True
            else:
                numVertices += pSubMesh.vertexData.vertexCount
            numIndices += pSubMesh.indexData.indexCount
    
        if useSharedVertices:
            numVertices += pMesh.sharedVertexData.vertexCount
            
            storageclass = ctypes.c_float * 3
            test=storageclass(1.1)
#         mVertexList = new Point[numVertices];
#         mIndexList = new unsigned int[numIndices];
    
        self.mVertexList = []
        self.mIndexList = []
    
        print "Total: numSubmeshes = " , pMesh.getNumSubMeshes() 
        print "Total: numVertices = " ,numVertices 
        print "Total: numIncides = " , numIndices
    
        ## Count the number of vertices and incides so we can Set them 
        indexCount = 0
        vertListCount = 0
    
        if useSharedVertices:
            ## Real* pVertices (x, y, z order x numVertices)
            elem = pMesh.sharedVertexData.vertexDeclaration.findElementBySemantic(ogre.VES_POSITION)
    
            if not elem:
                ogre.Except(Exception.ERR_ITEM_NOT_FOUND, "Can't find position elements in the " 
                    "mesh to be written!", "MeshSerializerImpl.writeGeometry")
            
            vbuf = pMesh.sharedVertexData.vertexBufferBinding.getBuffer(elem.getSource())
            
            ## need space for the 3 verticies
            storageclass = ctypes.c_float * 3
            test=storageclass(1.1)
            
            for j in range ( pMesh.sharedVertexData.vertexCount ):
                vbuf.readData(j * vbuf.getVertexSize(), 3 * ctypes.sizeof(ctype.c_float), ctypes.addressof(test))
                self.mVertexList.append( opcode.Point() )
                self.mVertexList[vertListCount].Set(test[0], test[1], test[2])
                vertListCount+=1
    
        for i in range ( pMesh.getNumSubMeshes ):
            pSubMesh = pMesh.getSubMesh(i)
            if not pSubMesh.useSharedVertices:
                ## Real* pVertices (x, y, z order x numVertices)
                elem = pSubMesh.vertexData.vertexDeclaration.findElementBySemantic(ogre.VES_POSITION)
                if not elem:
                    ogre.Except(Exception.ERR_ITEM_NOT_FOUND, "Can't find position elements in the " 
                        "mesh to be written!", "MeshSerializerImpl.writeGeometry")
            
                vbuf = pSubMesh.vertexData.vertexBufferBinding.getBuffer(elem.getSource())
                
                ## need space for the verticies
                storageclass = ctypes.c_float * (pSubMesh.vertexData.vertexCount * 3)
                test=storageclass(1.1)
    
                vbuf.readData(0, pSubMesh.vertexData.vertexCount * 3 * ctypes.sizeof(ctype.c_float), 
                                                                                ctypes.addressof(test))
    
                for j in range ( pSubMesh.vertexData.vertexCount ):
                    self.mVertexList.append( opcode.Point() )
                    self.mVertexList[vertListCount].Set(test[j], test[j+1], test[j+2])
                    vertListCount += 1
    
            ibuf = pSubMesh.indexData.indexBuffer
            ## need space for the verticies
            storageclass = ctypes.c_unsignedshort * pSubMesh.indexData.indexCount
            test2=storageclass(1.1)
            
            ibuf.readData(0, ibuf.getSizeInBytes(), ctypes.addressof(test2))
    
            for j in range ( pSubMesh.indexData.indexCount ):
                self.mIndexList.append (test2[j])   # unsigned short
                indexCount += 1
    
        print "Total: counted comp vertex = ", vertListCount
        print "Total: counted incides = " , indexCount
    
        self.CollisionModel = opcode.OPCODE_Model()
    
        if not self.CollisionModel:
            return False
    
        self.OPCC.NbTris = numIndices / 3
        self.OPCC.NbVerts = numVertices
        self.OPCC.Tris = mIndexList
        self.OPCC.Verts = mVertexList
        self.OPCC.Rules = opcode.SPLIT_COMPLETE | opcode.SPLIT_SPLATTERPOINTS | opcode.SPLIT_GEOMCENTER
        self.OPCC.NoLeaf = True
        self.OPCC.Quantized = True
    
        return self.CollisionModel.Build(self.OPCC)
    

    def getWorldMatrix(self, matrix):
        world = self.Entity.getSubEntity(0).getWorldTransforms()
        if ( self.Entity.getSubEntity(0).getNumWorldTransforms() > 1):
            print "Error: Num World Transforms ", self.Entity.getSubEntity(0).getNumWorldTransforms()

        for i in range ( 4 ):
          matrix.m[0][i] = world[i][0]
          matrix.m[1][i] = world[i][1]
          matrix.m[2][i] = world[i][2]
          matrix.m[3][i] = world[i][3]
      
      
class Collider:
    
    def __init__(self):
        self.collider = opcode.AABBTreeCollider()
        self.colCache = opcode.BVTCache()   
           
        self.collider.SetFirstContact (True)
        self.collider.SetFullBoxBoxTest (True)
        self.collider.SetFullPrimBoxTest (True)
        self.collider.SetTemporalCoherence (False)

    def collide(self, ent0, ent1):
        pass
        
        
class GameEntity :
    def __init__( self, sm, type_, count ):
        self.sceneManager  = sm
        self.name = "GEntity_" + str(count)
        self.entity = self.sceneManager.createEntity(self.name, type_ + ".mesh")
        self.node = self.sceneManager.getRootSceneNode().createChildSceneNode(self.name)
        self.node.attachObject(self.entity)
        self.entity.setNormaliseNormals(True)
        self.entity.setCastShadows(True)
        self.collisionEntity = None
        self.userData=None
        self.materialname = None
            
    def setPosition(self, v):
        self.node.setPosition(v)
        
#     def setPosition(self, x, y, z):
#         self.node.setPosition(self, x, y, z)
        
    def getPosition ( self ):
        return self.node.getPosition()
    def setScale ( self, x,y,z ):
        self.node.setScale ( x,y,z )    
    def setMaterialName ( self, mat ):
        if self.materialname != mat:
            self.entity.setMaterialName ( mat )
            self.materialname = mat
        
    def setUserData ( self, ud ):
        self.UserData = ud
    def getUserData ( self ):
        return self.UserData
              
    def createCollider(self):
        self.Collider = Collider()
        if not self.Collider:
            return False
        return True

    def createEntity(self, ent):
        ce = CollisionEntity(ent)
        return ce

    def collide(self, ent1, ent2):
        return self.Collider.collide(ent1,ent2)
           
class OpcodeCollideApplication(sf.Application):
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
        plane.normal = ogre.Vector3.UNIT_Y
        plane.d = 200
        mm = ogre.MeshManager.getSingleton()
        mm.createPlane('FloorPlane', 'General', plane, 200000.0, 200000.0,
               20, 20, True, 1, 50, 50, ogre.Vector3.UNIT_Z)
        entity = sceneManager.createEntity('floor', 'FloorPlane')
        entity.setMaterialName('Examples/RustySteel')
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(entity)

        # create the camera node & attach camera
        cameraNode = sceneManager.getRootSceneNode().createChildSceneNode()
        cameraNode.attachObject(self.camera)
        
        # how many barrels and pallets do we want        
        self.numBoxes = 300
            
    # we need a framelistener to make life easy    
    def _createFrameListener(self):
        self.frameListener = OpcodeCollideListener(self.renderWindow, self.camera, self.sceneManager, self.numBoxes)
        self.root.addFrameListener(self.frameListener)
        
#     def _chooseSceneManager(self):
#         self.sceneManager = self.root.createSceneManager("TerrainSceneManager")
 
               
class OpcodeCollideQueryListener(ogre.IntersectionSceneQueryListener):
    """ the listener that gets called for each pair of intersecting objects """   
    def __init__ ( self ):
        ogre.IntersectionSceneQueryListener.__init__ ( self )
        
    def queryResult (  self, first, second ):
        """ the actual function that gets called - for the demo I simply change the material type
        so we see the collisions - except I don't change the floor material
        Also we do a lookup based upon the obect counter into the global list of 
        collision entites andmake the change via that object, not directly on the 'entity' """
        
        if first.name != "floor":
            name,count = first.name.split('_')
            CollEntities[int(count)].setMaterialName('Examples/Chrome')
        if second.name != "floor":
            name,count = second.name.split('_')
            CollEntities[int(count)].setMaterialName('Examples/TextureEffect2')
        return True
      
class OpcodeCollideListener(sf.FrameListener):
    """ the frame listener we are using"""
    
    def __init__(self, renderWindow, camera, sm, num):
        sf.FrameListener.__init__(self, renderWindow, camera)
        # Create an intersection query
        self.intersectSceneQuery =  sm.createIntersectionQuery()
        # and a listener to receive the results
        self.querylistener = OpcodeCollideQueryListener()
        
        # I then create a range of crates and barrels to test with
        self.numBoxes = num
        self.sceneManager = sm
        self.CreateBoxes ( num )
        self.frametime = 0
        
    def frameStarted(self, frameEvent):
        """ called each frame (at the start), check if it's time to do collision checking and 
        if so do it, otherwise pass through"""
        self.frametime += frameEvent.timeSinceLastFrame 
        # for performance reasons lets check for collisions about 5 times a second 
        if self.frametime > 0.2:
            self.ResetBoxes()
            self.intersectSceneQuery.execute( self.querylistener )
            self.frametime = 0
        return sf.FrameListener.frameStarted(self, frameEvent)
        
    def frameEnded ( self, ev ):
        """and at the end of the frame we update the objects positions in preparation for the 
        next frame"""
        self.UpdateBoxes(ev.timeSinceLastFrame)
        return sf.FrameListener.frameEnded(self, ev)
        
    def CreateBoxes (self, num):
        self.numBoxes=num
        self.mBoxTime = []   #new float[mNbBoxes]
        self.mSpeed = .000001
        self.mAmplitude = .5
        for i in range ( self.numBoxes ):
            if i % 2:
                object = GameEntity( self.sceneManager,"Barrel", len (CollEntities) )
            else:
                object = GameEntity( self.sceneManager,"WoodPallet", len (CollEntities) )
            CollEntities.append( object)
               
            ## Create the visual representation (the Ogre entity and scene node)
            ## Pick a size
            scale= 100
            size = ogre.Vector3((random.random() * 0.5 + 0.1) * scale,
                        (random.random() * 0.5 + 0.1) * scale,
                        (random.random() * 0.5 + 0.1) * scale)
            position = ogre.Vector3((random.random() -0.5 ) * 800.0,
                        (random.random() - 0.5 ) * 500.0,
                        (random.random() - 0.5 ) * 800.0)
                   
            object.setScale (size.x * 0.1,size.y * 0.1,size.z * 0.1) 
            object.setPosition (v=position)
            # we need access to the entity and nodes to reset the material           
            object.setUserData( 100.0*random.random() )
        
    def ResetBoxes(self):
        """ set the objects back to their original materials
        """
        for i in range ( self.numBoxes):
            if i % 2  :
                CollEntities[i].setMaterialName('RustyBarrel')
            else:
                CollEntities[i].setMaterialName('WoodPallet')

    def UpdateBoxes(self, timesincelastframe):
        """ move the objects in an interesting way :) """
        for i in range ( self.numBoxes):
            object = CollEntities[i]
            t = object.getUserData()
            t += timesincelastframe * 0.001 # self.mSpeed
            object.setUserData ( t )
            Center = ogre.Vector3()
            Center.x = cos(t*2.17)*self.mAmplitude + sin(t)*self.mAmplitude*0.5
            Center.y = cos(t*1.38)*self.mAmplitude + sin(t*self.mAmplitude)
            Center.z = sin(t*0.777)*self.mAmplitude
            pos = object.getPosition()
            pos.x += Center.x
            pos.y += Center.y
            pos.z += Center.y
            object.setPosition( pos )
            
    
if __name__ == '__main__':
    try:
        application = OpcodeCollideApplication()
        application.go()
    except ogre.OgreException, e:
        print e

    
