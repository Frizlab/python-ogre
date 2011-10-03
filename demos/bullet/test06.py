# 
# Updated to calculate a falling sphere:
# http://www.bulletphysics.com/mediawiki-1.5.8/index.php?title=Hello_World
#

import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
from ogre.physics import bullet

# Set up bullet
collisionConfiguration = bullet.get_btDefaultCollisionConfiguration()
dispatcher = bullet.get_btCollisionDispatcher1 (collisionConfiguration)
broadphase = bullet.btDbvtBroadphase()
solver = bullet.btSequentialImpulseConstraintSolver()

world = bullet.btDiscreteDynamicsWorld(dispatcher, broadphase , solver, collisionConfiguration)
world.setGravity(bullet.btVector3(0,-10,0))
world.getDispatchInfo().m_enableSPU = True
store=[]
for i in range(200):
   print(i)
   mass = 10
   shape = bullet.btBoxShape(bullet.btVector3(10, 10, 10))
   mots = bullet.btDefaultMotionState()
   o = bullet.btRigidBody(mass, mots, shape)
   world.addRigidBody(o)
   store.append(shape)
   store.append(o)
   store.append(mots)

for i in range(90):
   world.stepSimulation(0.0001)

# note we need to delete the world before the Object as there are issues with the destructor on Objects causing
# virtual functions being called errors
del world