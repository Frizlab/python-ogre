import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.physics.bullet as bullet
t = bullet.btTransform()
ms = bullet.btDefaultMotionState (t)
s = bullet.btBoxShape(bullet.btVector3(10,10,10))
body = bullet.btRigidBody(1, ms, s)
print body


collisionConfiguration = bullet.btDefaultCollisionConfiguration()
dispatcher = bullet.btCollisionDispatcher (collisionConfiguration)

worldAabbMin = bullet.btVector3(-1000,-1000,-1000)
worldAabbMax = bullet.btVector3(1000,1000,1000)
maxProxies = 32766

broadphase = bullet.btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies)

solver = bullet.btSequentialImpulseConstraintSolver()

world = bullet.btDiscreteDynamicsWorld(dispatcher, broadphase , solver, collisionConfiguration)
world.getDispatchInfo().m_enableSPU = True
world.setGravity(bullet.btVector3(0,-10,0))

print world
print dir(world)
print solver
print broadphase
print dispatcher
print collisionConfiguration 
for x in range (30):        
    world.stepSimulation( x * 1/30)
    
world.stepSimulation(0)
world.stepSimulation(-0.333)


class OgreMotionState(bullet.btMotionState): 
    def __init__(self, initalPosition): 
        bullet.btMotionState.__init__(self)
        self.Pos=initalPosition 
        self.Position=ogre.Vector3() 
        self.Quaternion=ogre.Quaternion() 
        
    def getWorldTransform(self, WorldTrans): 
        print WorldTrans
        print WorldTrans.Rotation.x(),WorldTrans.Rotation.y(),WorldTrans.Rotation.z()
        print self.Pos.getOrigin()
        WorldTrans.setOrigin(self.Pos.getOrigin())
        WorldTrans.setBasis(self.Pos.getBasis())
        
        print WorldTrans.Rotation.x(),WorldTrans.Rotation.y(),WorldTrans.Rotation.z()
        print WorldTrans
        print "Pos", self.Pos
        t=self.Pos
        print "T", t
        t.setOrigin(WorldTrans.getOrigin())
        print "T1", t
        
        
        
         
    def setWorldTransform(WorldTrans):
        print "setWorldTrans", WorldTrans 
        self.Position=ogre.Vector3(WorldTrans.getOrigin().x(),WorldTrans.getOrigin().y(),WorldTrans.getOrigin().z()) 
        self.Quaternion=ogre.Quaternion(WorldTrans.getRoation().w(),WorldTrans.getRoation().x(),WorldTrans.getRoation().y(),WorldTrans.getRoation().z()) 
        print "setWorldTrans", WorldTrans 

shape=bullet.btCapsuleShape(10, 20) 
print "Creating motionState"
motionState=OgreMotionState(bullet.btTransform(bullet.btMatrix3x3(1.0,1.0,1.0,-1.0,-1.0,-1.0,2,-2,-1))) 
Object=bullet.btRigidBody(10.0, motionState, shape) # ...this should work in my eyes 

#Object=bullet.btRigidBody(bullet.btRigidBodyConstructionInfo())   ...well obviously not in bullet 2.6.9