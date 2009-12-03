import ogre.renderer.OGRE as ogre
import ogre.physics.bullet as bullet
import ogre.physics.OgreBulletC as bulletC
import ogre.physics.OgreBulletD as bulletD

class BulletWrapper(sf.FrameListener):
    def __init__(self, renderWindow, camera, sceneManager, ):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.sceneManager = sceneManager
        self.camera = camera
        self.renderWindow=renderWindow
        
        self.gStaticBodyRestitution  = 0.1
        self.gStaticBodyFriction     = 0.8

        self.gDynamicBodyRestitution = 0.6
        self.gDynamicBodyFriction    = 0.6
        self.gDynamicBodyMass        = 1.0

        self.gCubeBodyBounds      = ogre.Vector3 (1, 1, 1)
        self.gCylinderBodyBounds  = ogre.Vector3 (0.5, 1, 0.5)
        self.gConeBodyBounds      = ogre.Vector3 (1, 1, 1)
        self.gSphereBodyBounds    = 1.0
        self.mNumEntitiesInstanced = 0
        
        self.mEntities=[]
        self.mShapes=[]
        self.mBodies=[]
        
    def initWorld(self, gravityVector, bounds):
        # Start Bullet
        self.world = bulletD.DynamicsWorld (self.sceneManager, bounds, gravityVector) 

        # add Debug info display tool
        debugDrawer = DebugDrawer()

        self.world.setDebugDrawer(debugDrawer)

        node = self.sceneManager.getRootSceneNode().createChildSceneNode("debugDrawer", Ogre.Vector3.ZERO()) 
        node.attachObject (debugDrawer) 
        
# -------------------------------------------------------------------------
    def addGround(self):
        self.addStaticPlane(
            self.gStaticBodyRestitution, 
            self.gStaticBodyFriction) 
# -------------------------------------------------------------------------
    def addCube(self, instanceName,pos, q, size, bodyRestitution, bodyFriction, bodyMass)
        entity = self.sceneManager.createEntity(
            instanceName + str(self.mNumEntitiesInstanced),"Bulletbox.mesh") 
        # "Crate.mesh") 
        # "Crate1.mesh") 
        # "Crate2.mesh") 
        entity.setQueryFlags (ogre.GEOMETRY_QUERY_MASK) 
        entity.setCastShadows(true) 

        entity.setMaterialName("Bullet/box") 

        sceneCubeShape = bullet.BoxCollisionShape(size) 

        defaultBody = bulletC.RigidBody(
            "defaultCubeRigid" + str(self.mNumEntitiesInstanced), 
            self.world) 

        node = self.sceneManager.getRootSceneNode ().createChildSceneNode () 
        node.attachObject (entity) 

        defaultBody.setShape (node,  sceneCubeShape, bodyRestitution, bodyFriction, bodyMass, pos, q) 

        self.mEntities.append(entity) 
        self.mShapes.append(sceneCubeShape) 
        self.mBodies.append(defaultBody) 
        self.mNumEntitiesInstanced += 1 

        return defaultBody 

# -------------------------------------------------------------------------
def addSphere(self, instanceName,pos, q, radius,
                                         bodyRestitution, bodyFriction, 
                                         bodyMass)
        entity = self.sceneManager.createEntity(
            instanceName + str(self.mNumEntitiesInstanced),
            "ellipsoid.mesh") 

        entity.setQueryFlags (ogre.GEOMETRY_QUERY_MASK) 
        entity.setCastShadows(true) 

        entity.setMaterialName("Bullet/box") 

        sceneCubeShape = bulletC.SphereCollisionShape(radius) 

        defaultBody = bulletC.RigidBody(
            "defaultSphereRigid" + str(self.mNumEntitiesInstanced),
            self.world) 

        node = self.sceneManager.getRootSceneNode ().createChildSceneNode () 
        node.attachObject (entity) 

        defaultBody.setShape (node,  sceneCubeShape, bodyRestitution, bodyFriction, bodyMass, pos, q) 

        self.mEntities.append(entity) 
        self.mShapes.append(sceneCubeShape) 
        self.mBodies.append(defaultBody) 
        self.mNumEntitiesInstanced += 1

        return defaultBody 


# -------------------------------------------------------------------------
    def addCylinder(self, instanceName,pos,q, size,bodyRestitution, bodyFriction, 
                                          bodyMass):
        entity = self.sceneManager.createEntity(
            instanceName + str(self.mNumEntitiesInstanced),
            "cylinder.mesh") 
        #   "capsule.mesh") 
        # "Barrel.mesh") 
        # "Barrel1.mesh") 
        # "Barrel2.mesh") 

        entity.setQueryFlags (ogre.GEOMETRY_QUERY_MASK) 
        entity.setCastShadows(true) 

        entity.setMaterialName("Bullet/box") 

        sceneCubeShape = bulletC.CylinderCollisionShape(size, ogre.Vector3.UNIT_X()) 

        defaultBody = bulletC.RigidBody(
            "defaultCylinderRigid" + str(self.mNumEntitiesInstanced), 
            self.world) 

        node = self.sceneManager.getRootSceneNode ().createChildSceneNode () 
        node.attachObject (entity) 

        defaultBody.setShape (node, sceneCubeShape, bodyRestitution, bodyFriction, bodyMass, pos, q) 

        self.mEntities.append(entity) 
        self.mShapes.append(sceneCubeShape) 
        self.mBodies.append(defaultBody) 
        self.mNumEntitiesInstanced++ 

        return defaultBody 

# -------------------------------------------------------------------------
    def addCone(self, instanceName,pos, q, size,
                                       bodyRestitution, bodyFriction, 
                                       bodyMass):
        entity = self.sceneManager.createEntity(
            instanceName + str(self.mNumEntitiesInstanced),
            #"Cone.mesh") 
            "Mesh.mesh") 

        entity.setQueryFlags (ogre.GEOMETRY_QUERY_MASK) 
        entity.setCastShadows(true) 

        entity.setMaterialName("Bullet/box") 

        sceneCubeShape = bulletC.ConeCollisionShape(size.x, size.y, ogre.Vector3.UNIT_Y) 

        defaultBody = bulletC.RigidBody(
            "defaultConeRigid" + str(self.mNumEntitiesInstanced), 
            self.world) 

        node = self.sceneManager.getRootSceneNode ().createChildSceneNode () 
        node.attachObject (entity) 

        defaultBody.setShape (node, sceneCubeShape, bodyRestitution, bodyFriction, bodyMass, pos, q) 

        self.mEntities.append(entity) 
        self.mShapes.append(sceneCubeShape) 
        self.mBodies.append(defaultBody) 
        self.mNumEntitiesInstanced +=1 

        return defaultBody 

# -------------------------------------------------------------------------
    def addStaticTrimesh(self, instanceName,meshName,pos, 
                                                q, bodyRestitution, bodyFriction,
                                                bool castShadow):
        sceneEntity = self.sceneManager.createEntity(instanceName + str(self.mNumEntitiesInstanced), meshName) 
        sceneEntity.setCastShadows (castShadow) 

        trimeshConverter = bulletC.StaticMeshToShapeConverter(sceneEntity) 
        sceneTriMeshShape = trimeshConverter.createTrimesh() 
        sceneRigid = bulletC.RigidBody(
            instanceName + "Rigid" + str(self.mNumEntitiesInstanced),
            self.world) 

        node = self.sceneManager.getRootSceneNode ().createChildSceneNode () 
        node.attachObject (sceneEntity) 

        sceneRigid.setStaticShape(node, sceneTriMeshShape, bodyRestitution, bodyFriction, pos) 

        self.mEntities.append(sceneEntity) 
        self.mBodies.append(sceneRigid) 
        self.mNumEntitiesInstanced +=1

        return sceneRigid 

# -------------------------------------------------------------------------
    def addStaticPlane( self, bodyRestitution, bodyFriction):
        # Use a load of meshes to represent the floor
        i = 0 
        s = self.sceneManager.createStaticGeometry("StaticFloor") 
        s.setRegionDimensions(ogre.Vector3(160.0, 100.0, 160.0)) 
        # Set the region origin so the center is at 0 world
        s.setOrigin(ogre.Vector3.ZERO(0) 
        for (z in range (-80.0 , 80.0 , 20.0)):
            for ( x in range ( -80.0, 80.0 , 20.0) ):
                name = "Plane_" + str(i) 
                i + = 1

                entity = self.sceneManager.createEntity(name, "plane.mesh") 
                entity.setMaterialName("BulletPlane") 
                entity.setQueryFlags (ogre.STATIC_GEOMETRY_QUERY_MASK) 
                #entity.setUserObject(_plane) 
                entity.setCastShadows(false) 
                s.addEntity(entity, ogre.Vector3(x,0,z)) 
        s.build() 

        Shape = bulletC.StaticPlaneCollisionShape (ogre.Vector3(0,1,0), 0) 

        defaultPlaneBody = bulletC.RigidBody(
            "Plane" + str(self.mNumEntitiesInstanced), 
            self.world) 


        defaultPlaneBody.setStaticShape (Shape, bodyRestitution, bodyFriction) 

        self.mBodies.append(defaultPlaneBody) 
        self.mShapes.append(Shape) 

        self.mNumEntitiesInstanced + = 1 

        return defaultPlaneBody 

    def throwDynamicObject(self,  key)
    
    trowDist = 2.0
    if key == KC_B
        if ( self.checkIfEnoughPlaceToAddObject(trowDist)):
            vec =ogre.Vector3(self.camera.getDerivedPosition())
            body = self.addCube("cube", vec, Quaternion(0,0,0,1), 
                self.gCubeBodyBounds, self.gDynamicBodyRestitution, self.gDynamicBodyFriction, self.gDynamicBodyMass);

            body.setLinearVelocity(
                self.camera.getDerivedDirection().normalisedCopy() * mShootSpeed
    elif  key == KC_N: 
        if ( checkIfEnoughPlaceToAddObject(trowDist)):
            vec = ogre.vector3 (self.camera.getDerivedPosition());
            body = addSphere("sphere", vec, Quaternion(0,0,0,1), 
                gSphereBodyBounds, 
                gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);

            body.setLinearVelocity(
                self.camera.getDerivedDirection().normalisedCopy() * mShootSpeed
    case KC_H: 
        if ( checkIfEnoughPlaceToAddObject(trowDist))
        {
            const Vector3 vec (self.camera.getDerivedPosition());
             body = addCylinder("cylinder", vec, Quaternion(0,0,0,1), 
                gCylinderBodyBounds, 
                gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);

            body.setLinearVelocity(
                self.camera.getDerivedDirection().normalisedCopy() * mShootSpeed
                );
        }
        break;
    case KC_G : 
        if ( checkIfEnoughPlaceToAddObject(trowDist))
        {
            const Vector3 vec (self.camera.getDerivedPosition());
             body = addCone("cone", vec, Quaternion(0,0,0,1), 
                gConeBodyBounds, 
                gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);

            body.setLinearVelocity(
                self.camera.getDerivedDirection().normalisedCopy() * mShootSpeed
                );
        }
        break;
    }
}
// -------------------------------------------------------------------------
void OgreBulletListener::dropDynamicObject(BULLET_KEY_CODE key)
{
    const float dropDist = 10.0f;
    switch(key)
    {
    case KC_J: 
        if ( checkIfEnoughPlaceToAddObject(dropDist))
        {
            const Vector3 vec (self.camera.getDerivedPosition());
             body = addCube("cube", 
                vec + self.camera.getDerivedDirection().normalisedCopy() * 10, 
                Quaternion(0,0,0,1), 
                gCubeBodyBounds, gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);

        }
        break;
    case KC_K: 
        if ( checkIfEnoughPlaceToAddObject(dropDist))
        {
            const Vector3 vec (self.camera.getDerivedPosition());
             body = addSphere("sphere", 
                vec + self.camera.getDerivedDirection().normalisedCopy() * 10, 
                Quaternion(0,0,0,1), 
                gSphereBodyBounds, 
                gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);

        }
        break;
    case KC_U : 
        if ( checkIfEnoughPlaceToAddObject(dropDist))
        {
            const Vector3 vec (self.camera.getDerivedPosition());
             body = addCylinder("Cylinder", vec, Quaternion(0,0,0,1), 
                gCylinderBodyBounds, 
                gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);

        }
        break;
    case KC_I: 
        if ( checkIfEnoughPlaceToAddObject(dropDist))
        {
            const Vector3 vec (self.camera.getDerivedPosition());
             body = addCone("Cone", 
                vec + self.camera.getDerivedDirection().normalisedCopy() * 10, 
                Quaternion(0,0,0,1), 
                gConeBodyBounds, 
                gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);
        }
        break;
    }
}        