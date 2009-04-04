# 
# Updated to calculate a falling sphere:
# http://www.bulletphysics.com/mediawiki-1.5.8/index.php?title=Hello_World
#

import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.physics.bullet as bullet
m_clock=bullet.btClock()

def getDeltaTimeMicroseconds() :
		dt = m_clock.getTimeMicroseconds()
		m_clock.reset()
		return dt
		
# t = bullet.btTransform()
# ms = bullet.btDefaultMotionState (t)
# s = bullet.btBoxShape(bullet.btVector3(10,10,10))
# body = bullet.btRigidBody(1, ms, s)
# print body

# collisionConfiguration = bullet.btDefaultCollisionConfiguration()

collisionConfiguration = bullet.get_btDefaultCollisionConfiguration()
dispatcher = bullet.get_btCollisionDispatcher1 (collisionConfiguration)

# worldAabbMin = bullet.btVector3(-1000,-1000,-1000)
# worldAabbMax = bullet.btVector3(1000,1000,1000)
# maxProxies = 32766
# broadphase = bullet.btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies)
# solver = bullet.btSequentialImpulseConstraintSolver()

broadphase = bullet.btDbvtBroadphase()
solver = bullet.btSequentialImpulseConstraintSolver()

world = bullet.btDiscreteDynamicsWorld(dispatcher, broadphase , solver, collisionConfiguration)
world.setGravity(bullet.btVector3(0,-10,0))
world.getDispatchInfo().m_enableSPU = True

print "WORLD:=", world

print solver
print broadphase
print dispatcher
print collisionConfiguration 
for x in range (30):        
    world.stepSimulation( x * 1/30)
    print getDeltaTimeMicroseconds()
    
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
        
    def setWorldTransform(self, WorldTrans):
        #print "setWorldTrans", WorldTrans 
        self.Position=ogre.Vector3(WorldTrans.getOrigin().x(),WorldTrans.getOrigin().y(),WorldTrans.getOrigin().z()) 
        self.Quaternion=ogre.Quaternion(WorldTrans.getRotation().w(),WorldTrans.getRotation().x(),WorldTrans.getRotation().y(),WorldTrans.getRotation().z()) 
        #print "setWorldTrans", WorldTrans 
        

mass = 10
fallInertia = bullet.btVector3(0, 0, 0)
shape=bullet.btSphereShape(1)
shape.calculateLocalInertia(mass, fallInertia)
print "Creating motionState"
motionState=OgreMotionState(bullet.btTransform(bullet.btQuaternion(0, 0, 0, 1), bullet.btVector3(0, 50, 0))) 
print "1"
construct = bullet.btRigidBody.btRigidBodyConstructionInfo(mass, motionState, shape, fallInertia)
print "1"
Object=bullet.btRigidBody(construct) # ...this should work in my eyes 
print dir (Object)
print "1"
world.addRigidBody(Object)
print "1"

for x in range (90):        
    world.stepSimulation( x * 1/30)
    
    print "OBJECT:",
    print Object.getAabb(bullet.btVector3(0,0,0), bullet.btVector3(10,10,10))
    print Object.getMotionState
    pos = Object.getMotionState().Position
    print x, "height: " + str(pos.y)

# note we need to delete the world before the Object as there are issues with the destructor on Objects causing
# virtual functions being called errors
del world

 	  	 
