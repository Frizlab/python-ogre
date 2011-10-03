import ogre.renderer.OGRE as ogre 
from ogre.physics.bullet import *

if __name__ == '__main__': 

#    collisionConfiguration = bullet.btDefaultCollisionConfiguration()
#    dispatcher = bullet.btCollisionDispatcher (collisionConfiguration)
#    
#    # worldAabbMin = bullet.btVector3(-1000,-1000,-1000)
#    # worldAabbMax = bullet.btVector3(1000,1000,1000)
#    # maxProxies = 32766
#    # broadphase = bullet.btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies)
#    # solver = bullet.btSequentialImpulseConstraintSolver()
#    
#    broadphase = bullet.btDbvtBroadphase()
#    solver = bullet.btSequentialImpulseConstraintSolver()
# 


   
   #Set up the collision configuration and dispatcher
#    collisionConfiguration = btDefaultCollisionConfiguration()
#    collisionConfiguration = set_btDefaultCollisionConfiguration()
#    print collisionConfiguration
#    
#    c = get_btDefaultCollisionConfiguration()
#    print c
#    
#    
#    dispatcher = get_btCollisionDispatcher(c)
#    print dir(dispatcher)
#    print "111111"
#    print dispatcher.getCollisionConfiguration()
# #    collisionConfiguration = btDefaultCollisionConfiguration()
# #    dispatcher = btCollisionDispatcher (collisionConfiguration)
# 
#    print "Getting broadphase"
#    broadphase = btDbvtBroadphase()
# 
#    print "Done broadphase"  
#    #The actual physics solver
# 
#    print "2"  
#    worldAabbMin = btVector3(-100,-100,-100)
#    worldAabbMax = btVector3(100,100,100)
#    print "1"  
#    maxProxies = 1024
#    
#    test="asdfaassssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss"
# #    broadphase = btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies)
#    solver = btSequentialImpulseConstraintSolver()
#    print "1"  
#    
#    #The world
#    print "1"  
#    dynamicsWorld = btDiscreteDynamicsWorld(dispatcher,broadphase,solver,c) # collisionConfiguration)
   print "start"
   dynamicsWorld = makeWorld()
   print "1"  
   print collisionConfiguration
   print dir ( collisionConfiguration )
   print dynamicsWorld
   print dir ( dynamicsWorld )
