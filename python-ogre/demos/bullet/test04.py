import sys, os
sys.path.insert(0,'..')
import PythonOgreConfig

if sys.platform == 'win32': 
    newpath = os.path.join ( os.path.abspath(os.path.dirname(__file__)), '../../plugins')
    os.environ['PATH'] =  newpath +';' + os.environ['PATH']
    
import ogre.renderer.OGRE as ogre
import ogre.physics.bullet as bullet
import ogre.physics.OgreBulletC as bulletC
import SampleFramework as sf

class BulletListener( sf.FrameListener ):
    def __init__(self, rw, cam, world ):
        sf.FrameListener.__init__(self, rw, cam)
        self.world = world
        
    def frameStarted(self, evt):
        self.world.stepSimulation( evt.timeSinceLastFrame )
        return sf.FrameListener.frameStarted( self, evt )
    
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


        
class BulletApplication( sf.Application ):
    def _createScene( self ):
        print "\nSetting up Bullet"
        self.setupBullet()
        print "\nCreating Boxes"
#         self.createBoxes()
        print "\nDone"
        
        
    def __del__(self):
        if self.world:
            del self.world
        #sf.Application.__del__(self)
        
    def setupBullet( self ):
        self.collisionConfiguration = bullet.btDefaultCollisionConfiguration() 
        self.dispatcher = bullet.btCollisionDispatcher (self.collisionConfiguration) 
        worldAabbMin = bullet.btVector3(-1000,-1000,-1000) 
        worldAabbMax = bullet.btVector3(1000,1000,1000) 
        maxProxies = 32# 766 
        self.broadphase = bullet.btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies) 
        self.solver = bullet.btSequentialImpulseConstraintSolver() 
        self.world = bullet.btDiscreteDynamicsWorld(self.dispatcher, self.broadphase , self.solver, self.collisionConfiguration) 
        self.world.stepSimulation(0.1)
        self.world.stepSimulation(1)
        print "Leaving setup"
        
    def createBoxes( self ):
        print 2
        ent = self.sceneManager.createEntity( "box", "cube.mesh" )
        print 2
        node = self.sceneManager.getRootSceneNode().createChildSceneNode()
        print 2
        node.attachObject( ent )
        print 2
        ent.setMaterialName( "Examples/RustySteel")
        print 2
        
        mass = 10
        fallInertia = bullet.btVector3(0, 0, 0)
        print 2
        shape=bullet.btSphereShape(1)
        print 2
        shape.calculateLocalInertia(mass, fallInertia)
        print "Creating motionState"
        motionState=OgreMotionState(bullet.btTransform(bullet.btQuaternion(0, 0, 0, 1), bullet.btVector3(0, 50, 0)))
        print "creating construct"
        construct = bullet.btRigidBody.btRigidBodyConstructionInfo(mass, motionState, shape, fallInertia)
        print "creating object"
        Object=bullet.btRigidBody(construct) # ...this should work in my eyes 
        print "adding rigidBody"
        self.world.addRigidBody(Object)
        print "completed" 
        self.world.stepSimulation(1)   
        
    def _createFrameListener( self ):
        self.fl = BulletListener( self.renderWindow, self.camera, None )
        self.fl.world = self.world
        self.root.addFrameListener( self.fl )


if __name__=="__main__":
    app = BulletApplication()
    app.go()