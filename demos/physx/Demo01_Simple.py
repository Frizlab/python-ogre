import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.physics.PhysX as physx
import ogre.physics.OgreOde as OgreOde
import SampleFramework as sf
import ctypes
from ctypes import *
from math import *

# -------------------------------------------------------------------------------------------------
# This code is based on a physx ogre tutorial by makiolo 
# http://blogricardo.wordpress.com/2008/07/22/como-hacer-juegos-profesionales-linux-ogre-physx-2a-parte/
#
# -------------------------------------------------------------------------------------------------
# 
# class  MyMemoryWriteBuffer(physx.MemoryWriteBuffer):
#     def __init__(self):
#         physx.MemoryWriteBuffer.__init__(self)
#     def storeFloat(self,f):
#         f = physx.MemoryWriteBuffer.storeFloat(self,f)
#         print f 
#         return f
#     def storeDouble(self,f):
#         print "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"    
#         f = physx.MemoryWriteBuffer.storeDouble(self,f)
#         print f 
#         return f
#     def storeBuffer(self,buffer, size):
#         print "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"    
#         physx.MemoryWriteBuffer.storeBuffer(self,buffer, size)
#     def storeDword(self,w):
#         print "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"    
#         w = physx.MemoryWriteBuffer.storeDouble(self,w)
#         print w 
#         return w
    


               
# -------------------------------------------------------------------------------------------------
# Nx to/from Ogre Conversion Utilitites
# -------------------------------------------------------------------------------------------------
class CMathUtilities():


    def str2int(str):
        return int(str)
        
    def int2str(n):
        return str(n)
        
    def sphericalPolar(radio, pitch, yaw):
        temp = physx.NxVec3()
      	temp.x=radio* physx.sinf(pitch)*physx.sinf(yaw)
      	temp.y=radio*physx.cosf(pitch)
      	temp.z=radio*physx.sinf(pitch)*physx.cosf(yaw)
      	return temp
        
    def toNxVec3(ogreVec3):
      	return physx.NxVec3( ogreVec3.x, ogreVec3.y, ogreVec3.z)

    def toVector3(nxVec3):
      	return ogre.Vector3( nxVec3.x, nxVec3.y, nxVec3.z)

        
    def toNxQuat(q):
      	orientation = physx.NxQuat()
      	orientation.x = q.x
      	orientation.y = q.y
      	orientation.z = q.z
      	orientation.w = q.w
      	return orientation

    def toQuaternion(q):
      	orientation = ogre.Quaternion()
      	orientation.x = q.x
      	orientation.y = q.y
      	orientation.z = q.z
      	orientation.w = q.w
      	return orientation
        
    def getSizeofNxVec3():
        return 12
    def getSizeofNxU32():
        return 4
        
    str2int = staticmethod(str2int)
    int2str = staticmethod(int2str)
    sphericalPolar = staticmethod(sphericalPolar)
    toNxVec3 = staticmethod(toNxVec3)
    toVector3 = staticmethod(toVector3)
    toNxQuat = staticmethod(toNxQuat)
    toQuaternion = staticmethod(toQuaternion)
    getSizeofNxVec3 = staticmethod(getSizeofNxVec3)
    getSizeofNxU32 = staticmethod(getSizeofNxU32)


    
# -------------------------------------------------------------------------------------------------
# Mesh Utilities
# -------------------------------------------------------------------------------------------------
    
class CMeshUtilities():
    def getExtents ( mesh ) :
     
        vMin = ogre.Vector3(99999,99999,99999)
        vMax = ogre.Vector3(-99999,-99999,-99999)
        i = []
        for m in range(mesh.numSubMeshes):
            sm = mesh.getSubMesh(m)
            pmesh = sm.parent
            if pmesh and sm.useSharedVertices :
                vertex_data = pmesh.sharedVertexData
            else : vertex_data = sm.vertexData

            # get the start of the element
            posElem = vertex_data.vertexDeclaration.findElementBySemantic(ogre.VertexElementSemantic.VES_POSITION)
            vbuf = vertex_data.vertexBufferBinding.getBuffer(posElem.getSource())
            
            # get the start of the actual buffer
            vertex = vbuf.lock(ogre.HardwareBuffer.HBL_READ_ONLY)
            
            # what we need is the actual address of the buffer, so we have to check if there's an offset as well
            # the is basically what baseVertexPointerToElement does 
            newaddress = posElem.getOffset() + ogre.CastInt(vertex) # note the cast to int to get the address
                
            for i in range (vertex_data.vertexCount):
                points = ogre.getFloat( ogre.CastVoidPtr ( newaddress), 3 ) # note the getFloat and Cast back to a pointer :)
                print points
                if (points[0]<vMin.x):
                    vMin.x = points[0]
                if (points[1]<vMin.y):
                    vMin.y = points[1]
                if (points[2]<vMin.z):
                    vMin.z = points[2]
                if (points[0]>vMax.x):
                    vMax.x = points[0]
                if (points[1]>vMax.y):
                    vMax.y = points[1]
                if (points[2]>vMax.z):
                    vMax.z = points[2]
                    
                #outbuff.append(tuple(points))
                newaddress += vbuf.getVertexSize()
        return vMin, vMax

    def getVertices ( sm, convertToNx = True ) :
        outbuff = []
        pmesh = sm.parent
        if pmesh and sm.useSharedVertices :
            vertex_data = pmesh.sharedVertexData
        else : vertex_data = sm.vertexData

        # get the start of the element
        posElem = vertex_data.vertexDeclaration.findElementBySemantic(ogre.VertexElementSemantic.VES_POSITION)
        vbuf = vertex_data.vertexBufferBinding.getBuffer(posElem.getSource())
        
        # get the start of the actual buffer
        vertex = vbuf.lock(ogre.HardwareBuffer.HBL_READ_ONLY)
        
        # what we need is the actual address of the buffer, so we have to check if there's an offset as well
        # the is basically what baseVertexPointerToElement does 
        newaddress = posElem.getOffset() + ogre.CastInt(vertex) # note the cast to int to get the address
            
        for i in range (vertex_data.vertexCount):
            points = ogre.getFloat( ogre.CastVoidPtr ( newaddress), 3 ) # note the getFloat and Cast back to a pointer :)
            outbuff.append(physx.NxVec3(points[0], points[1], points[2]))
            newaddress += vbuf.getVertexSize()
        return outbuff

    def getIndices ( sm ) :
        outbuff = []
        
        numTris = sm.indexData.indexCount
        if numTris == 0 : return outbuff
        else : numTris /= 3
        
        ibuf = sm.indexData.indexBuffer
        
        if ibuf.getType() == ogre.HardwareIndexBuffer.IT_32BIT:
            Use32Bit = True
        else: Use32Bit = False
        
        buffer = ibuf.lock(ogre.HardwareBuffer.HBL_READ_ONLY)
        address = ogre.CastInt ( buffer )
            
        for i in range( numTris ):
            if Use32Bit:
                l = ogre.getUint32 ( ogre.CastVoidPtr ( address ), 3 )
                address += ibuf.getIndexSize() * 3 
            else:
                l = ogre.getUint16 ( ogre.CastVoidPtr ( address ), 3 )
                address += ibuf.getIndexSize() * 3
            outbuff.append(l[0])
            outbuff.append(l[1])
            outbuff.append(l[2])
        #mMaterials[index_offset++] = currentMaterialIndex;            
        return outbuff
        
    def getMeshInfo(mesh):
        v = []
        i = []
        for m in range(mesh.numSubMeshes):
            sm = mesh.getSubMesh(m)
            v += CMeshUtilities.getVertices ( sm )
            i += CMeshUtilities.getIndices ( sm )            
        return v, i
        
        
    getExtents = staticmethod(getExtents)
    getIndices = staticmethod(getIndices)   
    getVertices = staticmethod(getVertices)   
    getMeshInfo = staticmethod(getMeshInfo)   
    
# -------------------------------------------------------------------------------------------------
# Densities
# -------------------------------------------------------------------------------------------------
class DENSITY():
    Aluminium = 2700
    Cork = 250
    Copper = 8960
    Ice = 920
    Brick = 7900
    SoftWood = 200
    MediumWood = 500
    HardWood = 800
    Lead = 11300
    Glass = 3000
    LaminatedGlass = 3600
    
    ##Liquidos
    Acetone = 790
    Oil = 920
    Water = 1025
    Water2 = 1000
    Alcohol = 790
    Gasoline = 680
    Milk = 1030
    Mercury = 13600
    ##Gas
    Air = 1
    Butane = 3
    
    

# -------------------------------------------------------------------------------------------------
# PhysxListener
# -------------------------------------------------------------------------------------------------
class PhysxStepper():
    def __init__(self, nxscene, bodies):
        # timing
      	self.FPS  = 30
      	maxIter = self.FPS / 15.0 
      	self.timePerPass = 1.0 / self.FPS
        maxTimestep = self.timePerPass / float(maxIter)
        nxscene.setTiming( maxTimestep, int(maxIter), physx.NX_TIMESTEP_FIXED )

        self.nxscene = nxscene
        self.bodies  = bodies

    def step(self):
      	self.nxscene.simulate(self.timePerPass)
      	self.nxscene.flushStream()

    def update(self):
        #----------------------------------------------------------------
        # TODO update self.nxscene.fetchResults to return errorNumber
      	#self.nxscene.fetchResults( physx.NX_ALL_FINISHED,
        #                           True , ctypes.addressof(self.errorNumber) )
        #--------------------------------------------------------------------------------
      	self.nxscene.fetchResults( physx.NX_ALL_FINISHED, True  )
        for body in self.bodies:
            body.Update()


# -------------------------------------------------------------------------------------------------
# Body
# -------------------------------------------------------------------------------------------------
class Body():

    def __init__(self, actorDesc, physXManager):
        self.actor = None
        self.dynamic = False
        self.userDataList = []
        self.physXManager = physXManager
        self.actor = physXManager.getNxScene().createActor(actorDesc)
        #actor.raiseBodyFlag(physx.NX_BF_KINEMATIC)
        try:
            if actorDesc.body is not None:
                self.dynamic = True
        except:        
            pass        

        
    # ------------------------------------------------------------------------------
    def setUserData(self):
        self.actor.setUserData(self.userDataList)
    # ------------------------------------------------------------------------------
    def addEntity(self, ent):
        self.userDataList.append(ent)
        
    # ------------------------------------------------------------------------------
    # create a debug node and attach to actor's user data list
    # ------------------------------------------------------------------------------
    def addDebugNode(self, name, shape, radius = 5, parentNode = None):
    
        # create debug object, fudge from ogreode
        if shape.getType() == physx.NX_SHAPE_BOX:
            size = CMathUtilities.toVector3(shape.dimensions)* 2 * 1.02
            b = OgreOde.BoxDebugObject( size )
        elif shape.getType() == physx.NX_SHAPE_SPHERE:
            r = shape.radius*1.05
            b = OgreOde.SphereDebugObject( r )
        else:
            b = OgreOde.SphereDebugObject(radius)
            
        # attach debug object to main or parent node
        if not parentNode:
            parentNode = self.physXManager.sceneManager.getRootSceneNode()
        debugNodeName = name + "_Debug"
        _debug_node = parentNode.createChildSceneNode(debugNodeName) 
        _debug_node.attachObject( b )
        _debug_node.setPosition( CMathUtilities.toVector3(shape.localPose.t ) )

        # enable and add to actor's user data list
        b.setMode(OgreOde.DebugObject.Mode_Enabled)
        self.userDataList.append(b)
        
    # ------------------------------------------------------------------------------
    def cleanUp(self):
        for s in self.actor.getUserData():
            try:
                sn = s.getParentSceneNode()
                self.physXManager.sceneManager.destroySceneNode(sn.getName())
            except:
                pass        
        self.actor.setUserData(None)
        self.userDataList = []
        self.physXManager.gScene.releaseActor(self.actor)
        self.actor = None
            
    # ------------------------------------------------------------------------------
    def position(self, position, transVec = physx.NxVec3(0,0,0) ):
        self.actor.setGlobalPosition(position)
        self.actor.moveGlobalPosition( self.actor.getGlobalPosition() + self.actor.getGlobalOrientation() * transVec )
        print len(self.actor.getUserData())
        for u in self.actor.getUserData():
            self.setNodePosToActor(u.getParentNode() )

    # ------------------------------------------------------------------------------
    def setNodePosToActor(self, node):
        node.setPosition( CMathUtilities.toVector3(self.actor.getGlobalPosition()) )
        node.setOrientation( CMathUtilities.toQuaternion(self.actor.getGlobalOrientationQuat()) )
                
    # ------------------------------------------------------------------------------
    def Update(self):
        if not self.dynamic:
            return 
        for u in self.actor.getUserData():
            if u:
                self.setNodePosToActor(u.getParentNode() )

                    
# -------------------------------------------------------------------------------------------------
# PhysxListener
# -------------------------------------------------------------------------------------------------
class PhysxFrameListener(sf.FrameListener, PhysxStepper):
    def __init__(self, renderWindow, camera, nxscene, actors):
        sf.FrameListener.__init__(self, renderWindow, camera)
        PhysxStepper.__init__(self, nxscene, actors)
        
    def frameRenderingQueued(self, frameEvent):
        return sf.FrameListener.frameRenderingQueued(self, frameEvent)

    def frameStarted(self, frameEvent):
        PhysxStepper.step(self)
        return sf.FrameListener.frameStarted(self, frameEvent)

    def frameEnded(self, frameEvent):
        PhysxStepper.update(self)
        return sf.FrameListener.frameEnded(self, frameEvent)
        

# -------------------------------------------------------------------------------------------------
# PhysxManager
# -------------------------------------------------------------------------------------------------
class PhysxManager():
    def __init__(self):
    
        self.gphysxSDK = None
        self.gScene    = None
        self.gCooking  = None
        # create our own actors list NxScene getActors() not working
        self.bodies = []

        self.sceneManager = None
        
    def InitNX(self):
        # initialise physx
        gphysxSDK,errorCode = physx.NxCreatePhysicsSDK(physx.NX_PHYSICS_SDK_VERSION()) 

        if (physx.NXCE_NO_ERROR != errorCode):
            print "\nSDK create error (%d - %s).\nUnable to initialize the PhysX SDK, exiting the sample.\n\n" % (errorCode,"test")
            self.DeInitNX()
            return False

        # get the sdk
        self.gphysxSDK = physx.NxGetPhysicsSDK()

        # Set the physx parameters
        self.gphysxSDK.setParameter(physx.NX_SKIN_WIDTH, -0.01)
        # Set the debug visualization parameters
        self.gphysxSDK.setParameter(physx.NX_VISUALIZATION_SCALE, 1)
        self.gphysxSDK.setParameter(physx.NX_VISUALIZE_COLLISION_SHAPES, 1)
        self.gphysxSDK.setParameter(physx.NX_VISUALIZE_JOINT_LIMITS, 1)
        self.gphysxSDK.setParameter(physx.NX_VISUALIZE_JOINT_LOCAL_AXES, 1)
        self.gphysxSDK.setParameter(physx.NX_VISUALIZE_ACTOR_AXES, 1)
        return True
        
    # ------------------------------------------------------------------------------
    def createScene(self, sceneManager):
        self.sceneManager = sceneManager
        # Create the physx scene
        sceneDesc = physx.NxSceneDesc()
        sceneDesc.gravity = physx.NxVec3(0.0,-9.8,0.0)
        self.gScene = self.gphysxSDK.createScene(sceneDesc)
        if (not self.gScene): 
            print "Error !!!  Could not create the scene"
            self.DeInitNX()
            return False
        sceneDesc.collisionDetection = True
        
        # Create the default material
        m = physx.NxMaterialDesc ()
        m.restitution        = 0.5
        m.staticFriction     = 0.2
        m.dynamicFriction    = 0.2
        mat = self.gScene.getMaterialFromIndex(0)
        mat.loadFromDesc(m) 

        self.gCooking = physx.NxGetCookingLib(physx.NX_PHYSICS_SDK_VERSION())
        return True
        
    # ------------------------------------------------------------------------------
    def DeInitNX(self):
        if self.gphysxSDK:
            if not self.gScene is None:
            
                # delete bodies
                while len(self.bodies)>0:
                    body = self.bodies.pop()
                    body.cleanUp()
                    del body
                print "Cleaned Up Bodies"
                # delete scene
                self.gphysxSDK.releaseScene(self.gScene)
                self.gScene = 0
                print "Deleted Scene"
            # delete SDK
            physx.NxReleasePhysicsSDK(self.gphysxSDK)        
            self.gphysxSDK = 0
        
        print "Released PhysX SDK"

    # ------------------------------------------------------------------------------
    def getNxScene(self):
        return self.gScene
    def getPhysXSDK(self):
        return self.gphysxSDK
    def getPhysXCooking(self):
        return self.gCooking
    def getActors(self):
        return self.actors
    def getDebugObjects(self):
        return self.debugs

        
    # ------------------------------------------------------------------------------
    def createCube(self, name, density , vec3Extents , position, dynamic = True):
    
        # renderer
        ent = self.sceneManager.createEntity(name,'cube.mesh')
        ent.setQueryFlags(1<<2)
        ent.setMaterialName("Examples/RustySteel")
        node = self.sceneManager.getRootSceneNode().createChildSceneNode(name)
        node.translate(0, vec3Extents.y/2.0, 0)
      	node.scale(vec3Extents/100.0)
        node.attachObject(ent)

        # dimesions half extents
        shapeDimensions = CMathUtilities.toNxVec3( vec3Extents/2.0 )
        
        # box shape description
      	boxShapeDesc = physx.NxBoxShapeDesc()
        boxShapeDesc.name = "%sDesc" %(name)
      	boxShapeDesc.dimensions = shapeDimensions
      	#boxShapeDesc.localPose.t = physx.NxVec3(0, shapeDimensions.y ,0)
      	boxShapeDesc.density = density

        # actor description
      	actorDesc = physx.NxActorDesc()
        actorDesc.shapes.pushBack(boxShapeDesc)
        actorDesc.density = density
        actorDesc.globalPose.t  = CMathUtilities.toNxVec3(position)
        
        # add dynamic body
        if dynamic:
            bodyDesc = physx.NxBodyDesc()
            bodyDesc.angularDamping	= 0.0
            bodyDesc.linearDamping = 0.0
            actorDesc.body = bodyDesc
        
        
        body = Body(actorDesc, self)
        body.addEntity(ent)
        body.addDebugNode( name, boxShapeDesc)
        body.setUserData( )
        body.position(position = CMathUtilities.toNxVec3(position))
        self.bodies.append(body)

    # ------------------------------------------------------------------------------
    def createPlane(self, name, density , vec2Extents, position):

      	# Render
        plane = ogre.Plane()
        plane.normal = ogre.Vector3().UNIT_Y
        plane.d = 0
        mm = ogre.MeshManager.getSingleton()
        mm.createPlane(name, 'General', plane, vec2Extents.x, vec2Extents.y,
               20, 20, True, 1, 200.0, 200.0,  (0, 0, 1),
               ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, 
               True,True )

        ent = self.sceneManager.createEntity(name, name)
      	ent.setMaterialName("Examples/GrassFloor")
        sn = self.sceneManager.getRootSceneNode().createChildSceneNode()
      	sn.translate(vec2Extents.x/2.0 , 0 , vec2Extents.y / 2.0);
        sn.attachObject(ent)

        planeDesc = physx.NxPlaneShapeDesc()
        planeDesc.name = "%sDesc" %(name)
      	planeDesc.d = 0
        # actor description
      	actorDesc = physx.NxActorDesc()
        actorDesc.shapes.pushBack(planeDesc)

        body = Body(actorDesc, self)
        body.addEntity(ent)
        body.setUserData( )
        body.position(position = CMathUtilities.toNxVec3(position))
        self.bodies.append(body)

    # ------------------------------------------------------------------------------
    def createSphere(self, name, density , radius , position, dynamic = True):

      	# Render
        ent = self.sceneManager.createEntity(name,'sphere.mesh')
        ent.setQueryFlags(1<<2)
        ent.setMaterialName("Examples/RustySteel")
        node = self.sceneManager.getRootSceneNode().createChildSceneNode(name)
        node.translate(0, radius, 0)
      	node.scale( radius/100.0, radius/100.0, radius/100.0 )
        node.attachObject(ent)

        # sphere shape description
      	sphereShapeDesc = physx.NxSphereShapeDesc()
      	sphereShapeDesc.radius = radius
      	#sphereShapeDesc.localPose.t = physx.NxVec3(0, radius ,0)
      	sphereShapeDesc.density = density

        # actor description
      	actorDesc = physx.NxActorDesc()
        actorDesc.shapes.pushBack(sphereShapeDesc)
        actorDesc.density = density
        actorDesc.globalPose.t  = CMathUtilities.toNxVec3(position)
        
        # add dynamic body
        if dynamic:
            bodyDesc = physx.NxBodyDesc()
            bodyDesc.angularDamping	= 0.0
            bodyDesc.linearDamping = 0.0
            actorDesc.body = bodyDesc
        
        body = Body(actorDesc, self)
        body.addEntity(ent)
        body.addDebugNode( name, sphereShapeDesc)
        body.setUserData( )
        body.position(position = CMathUtilities.toNxVec3(position))
        self.bodies.append(body)


    # ------------------------------------------------------------------------------
    def createMesh(self, name, mesh_name, density, position):
   
      	# Render
        ent = self.sceneManager.createEntity(name, mesh_name)
        ent.setQueryFlags(1<<2)
        node = self.sceneManager.getRootSceneNode().createChildSceneNode('head')
        node.attachObject(ent)
      	node.scale( 0.1, 0.1, 0.1 )
        node.translate(position)

        vertices,indices = CMeshUtilities.getMeshInfo(ent.mesh)

      	meshDesc = physx.NxTriangleMeshDesc()
        meshDesc.name = "%sDesc" %(name)
        meshDesc.setToDefault()
        meshDesc.numVertices = len(vertices)
        meshDesc.pointStrideBytes = CMathUtilities.getSizeofNxVec3()
        meshDesc.setPoints(vertices, len(vertices))

        if len(indices)>0:    
            meshDesc.numTriangles = int(len(indices)/3)
            meshDesc.triangleStrideBytes = 3*CMathUtilities.getSizeofNxU32()
            meshDesc.setFaces(indices, len(indices))
        meshDesc.flags = 0
        
        numIndices = len(indices)
        numVerts   = len(vertices) 
        
        # check mesh description is valid 
        if not meshDesc.isValid():
            print "The mesh description created  is not valid" 
            print "NumVerts %d NumIndices %d MeshDesc NumVerts %d NumIndices %d" %(numVerts, numIndices, meshDesc.numVertices, meshDesc.numTriangles)
            sys.exit(1)
        else:
            print " NumVerts %d NumIndices %d MeshDesc NumVerts %d NumIndices %d" %(numVerts, numIndices, meshDesc.numVertices, meshDesc.numTriangles)
            
        del vertices,indices
        
       	meshShapeDesc = physx.NxTriangleMeshShapeDesc() 
        #meshShapeDesc.shapeFlags|= physx.NX_SF_FEATURE_INDICES

        
        # initilaise cooking    
        if not self.gCooking.NxInitCooking():
            print("Unable to initialize the cooking library. Please make sure that you have correctly installed the latest version of the AGEIA PhysX SDK.")
            sys.exit(1)
            
        writeBuffer = physx.MemoryWriteBuffer()
        status = self.gCooking.NxCookTriangleMesh(meshDesc, writeBuffer)
        if not status:
            print("Unable to cook a triangle mesh.")
            sys.exit(1)
        
        # don't call writeBuffer.data here, pass pointer to writeBuffer
        # in ctor and it handles the assignment of the buffer data
        memReadBuffer = physx.MemoryReadBuffer(writeBuffer)
        meshShapeDesc.meshData = self.gphysxSDK.createTriangleMesh(memReadBuffer)
        self.gCooking.NxCloseCooking()

        # actor description
      	actorDesc = physx.NxActorDesc()
        actorDesc.shapes.pushBack(meshShapeDesc)
        actorDesc.density = 0.02
        actorDesc.globalPose.t  = CMathUtilities.toNxVec3(position)
        

        body = Body(actorDesc, self)
        body.addEntity(ent)
        # sphere
        body.addDebugNode( name, meshShapeDesc, 5)
        body.setUserData( )
        body.position(position = CMathUtilities.toNxVec3(position))
        self.bodies.append(body)
        
        
        
        
        
        
# -------------------------------------------------------------------------------------------------
# Main Application
# -------------------------------------------------------------------------------------------------
class TutorialApplication (sf.Application):

    def __init__(self):
        sf.Application.__init__(self)
        self.physxManager = PhysxManager()

    # ------------------------------------------------------------------------------
    def _createScene (self):

        if not self.physxManager.InitNX():
            del self.physxManager
            sys.exit(1)

        if not self.physxManager.createScene( self.sceneManager):
            del self.physxManager
            sys.exit(1)
            
        self.physxManager.createCube(  "cube1",   DENSITY.Air , ogre.Vector3(20.0,20.0,20.0) , ogre.Vector3(0,400,0) ,True)
        self.physxManager.createSphere( "sphere1", DENSITY.SoftWood , 10.0 , ogre.Vector3(30,400,0) ,True)
        self.physxManager.createPlane( "plane1",   DENSITY.HardWood , ogre.Vector2(2000.0, 2000.0), ogre.Vector3(0,0,0) )
        
        # Not implemented since MemoryReadBuffer / MemoryWriteBuffer not wrapped in svn
        #self.physxManager.createMesh(  'head','razor.mesh', DENSITY.SoftWood, ogre.Vector3(0,10,0))

        # -------------------
        # crash ?????????????
        # -------------------
        #print dir(self.physxManager.getNxScene().getActors())

    def _createCamera(self):
        self.camera = self.sceneManager.createCamera('PlayerCam')
        self.camera.position =0,30,-90
        self.camera.lookAt((0,0,0))
        self.camera.nearClipDistance = 1
 
    def _createFrameListener(self):
        # pass self.actors list NxScene getActors() not working
        self.frameListener = PhysxFrameListener( self.renderWindow,
                                            self.camera,
                                            self.physxManager.gScene,
                                            self.physxManager.bodies )

        self.frameListener.showDebugOverlay(True)
        self.root.addFrameListener(self.frameListener)

    def cleanUp(self):
        self.physxManager.DeInitNX()
        del self.physxManager

    def __del__(self):
        if self.root:
            del self.root

if __name__ == '__main__':
    ta = TutorialApplication ()
    ta.go ()
    ta.cleanUp()

	  	 
