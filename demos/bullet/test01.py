import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE
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
