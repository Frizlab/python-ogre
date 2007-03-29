# /*
# SimpleScenes_Crash.h
# ----------------------
# A reimplementation of the ODE "crashing into a wall" demo 
# using Ogre and the OgreOde wrapper.
# */
from SimpleScenes import *
import ogre.renderer.OGRE as ogre
import ogre.physics.OgreOde as OgreOde
import ogre.io.OIS as OIS
# /*
# The crash test extends the base test class
# */
class SimpleScenes_Crash( SimpleScenes ):
    ## Standard constructor/destructor
    def __init__ ( self,  world) :
        SimpleScenes(self, world)
        self._rocket_node_explosion = self._mgr.getRootSceneNode ().createChildSceneNode ("rocket_node_explosion_node")
        
        self._rocketParticles_explosion = self._mgr.createParticleSystem ("rocket_node_explosion", "myExplosionTemplate")
        self._rocketParticles_smoke_white = self._mgr.createParticleSystem ("rocketParticles_smoke_white", "myExplosionSmokewhiteTemplate")
        self._rocketParticles_smoke = self._mgr.createParticleSystem ("rocketParticles_smoke", "myExplosionSmokeTemplate")
        
        self._rocket_node_explosion.attachObject(self._rocketParticles_explosion)
        self._rocket_node_explosion.attachObject(self._rocketParticles_smoke_white)
        self._rocket_node_explosion.attachObject(self._rocketParticles_smoke)
        
        self._rocketParticles_explosion.setSpeedFactor(0.0)
        self._rocketParticles_smoke_white.setSpeedFactor(0.0)
        self._rocketParticles_smoke.setSpeedFactor(0.0)
        
        self._rocket_node_explosion.setVisible(False)

        ## create helicopter

        ## Main bit of the helicopter
        self._apache_body = self._mgr.createEntity("Apache Body", "apache_body.mesh")
        
        self._apache_body.setQueryFlags(VEHICLE_QUERY_MASK)
        
        self._apache_body.setCastShadows(True)
        
        self._apache_body_node = self._mgr.getRootSceneNode().createChildSceneNode("Apache Body")
        self._apache_body_node.attachObject(self._apache_body)
        self._apache_body_node.setPosition(0,1.2,0)
        
        ## Main rotor
        self._main_rotor = self._mgr.createEntity("Main Rotor", "main_rotor.mesh")
        self._main_rotor.setCastShadows(True)
        
        self._main_rotor_node = self._apache_body_node.createChildSceneNode("Main Rotor")
        self._main_rotor_node.attachObject(self._main_rotor)
        self._main_rotor_node.setPosition(0,0.987322,0.573885)
        
        ## Tail rotor
        self._tail_rotor = self._mgr.createEntity("Tail Rotor", "tail_rotor.mesh")
        self._tail_rotor.setCastShadows(True)
        
        self._tail_rotor_node = self._apache_body_node.createChildSceneNode("Tail Rotor")
        self._tail_rotor_node.attachObject(self._tail_rotor)
        self._tail_rotor_node.setPosition(0.174927,0.173132,-3.50708)
        
        ## Set up the parameters for the main rotor
        self._main_min = 2.0
        self._main_max = 90.0
        self._main_spd = self._main_min
        self._main_response = 40.0
        self._main_scale = 1.0
        
        ## Set up the parameters for the tail rotor
        self._tail_min = -80.0
        self._tail_max = 80.0
        self._tail_response = 90.0
        self._tail_scale = 0.5
        self._tail_spd = 0.0
        
        ## Other simulation parameters
        self._rotate_scale = 20.0
        self._thrust = 50.0
        self._linear_damp = 12.5
        self._angular_damp = 25.0
        self._thrust_offs = Ogre.Vector3(0,0.1,0)
        self._fire_rate = 0.5
        self._fire_time = self._fire_rate
        self._fire_force = 250.0
        self._fire_mass = 0.1
        self._fire_size = 0.25
        self._rockets = 0

        ## Create a space for the helicopter to live in
        self._apacheself._space = OgreOde.SimpleSpace(self._world, self._space) ## NEW
        self._apacheself._space.setInternalCollisions(False)
        self._apacheself._space.setAutoCleanup(False)  

        ## Create the physical body of the helicopter
        OgreOde.BoxMass apache_mass(1.5,Vector3(1.4,1.46,4.24))
        self._apache = OgreOde.Body(self._world)  ## NEW
        self._apache_body_node.attachObject(self._apache)
        self._apache.setMass(apache_mass)

        self._apache.setDamping(self._linear_damp, self._angular_damp)

        self._bodies.append(self._apache)
        _last_node = self._apache_body_node

        ## Create the geometry of the helicopter from several bits

        ## Main body
        OgreOde.TransformGeometry* trans = OgreOde.TransformGeometry(self._world, self._apacheself._space) ## NEW
        trans.setBody(self._apache)

        self._apache_body.setUserObject(trans)

        OgreOde.BoxGeometry* geom = OgreOde.BoxGeometry(Vector3(1.4,1.46,4.24), world, 0) ## NEW
        trans.setEncapsulatedGeometry(geom)
        geom.setPosition(Vector3(0.013,0.014,1.052))
        self._geoms.append(geom)
        self._geoms.append(trans)



        ## Left wing
        trans = OgreOde.TransformGeometry(self._world, self._apacheself._space) ## NEW
        geom = OgreOde.BoxGeometry(Vector3(0.75,0.55,1.05), world, 0) ## NEW     
        trans.setBody(self._apache)
        trans.setEncapsulatedGeometry(geom)
        geom.setPosition(Vector3(-1.089,-0.486,0.446))     
        self._geoms.append(geom)
        self._geoms.append(trans)

        ## Right wing
        trans = OgreOde.TransformGeometry(self._world, self._apacheself._space)  ## NEW
        geom = OgreOde.BoxGeometry(Vector3(0.75,0.55,1.05), world, 0) ## NEW
        trans.setBody(self._apache)
        trans.setEncapsulatedGeometry(geom)
        geom.setPosition(Vector3(1.089,-0.505,0.446))      
        self._geoms.append(geom)
        self._geoms.append(trans)

        ## Tail boom
        trans = OgreOde.TransformGeometry(self._world, self._apacheself._space) ## NEW
        geom = OgreOde.BoxGeometry(Vector3(0.73,0.66,3.28), world, 0) ## NEW     
        trans.setBody(self._apache)
        trans.setEncapsulatedGeometry(geom)
        geom.setPosition(Vector3(0,-0.533,-2.104))     
        self._geoms.append(geom)
        self._geoms.append(trans)

        ## Tail flipper (probably not the technically correct name!)
        trans = OgreOde.TransformGeometry(self._world, self._apacheself._space) ## NEW
        geom = OgreOde.BoxGeometry(Vector3(1.61,0.30,0.45), world, 0) ## NEW
        trans.setBody(self._apache)
        trans.setEncapsulatedGeometry(geom)
        geom.setPosition(Vector3(-0.014,-0.622,-3.778))        
        self._geoms.append(geom)
        self._geoms.append(trans)

        ## Tail fin
        trans = OgreOde.TransformGeometry(self._world, self._apacheself._space) ## NEW
        geom = OgreOde.BoxGeometry(Vector3(0.30,1.27,0.96), world, 0) ## NEW     
        trans.setBody(self._apache)
        trans.setEncapsulatedGeometry(geom)
        geom.setPosition(Vector3(0,-0.042,-3.491)) 
        self._geoms.append(geom)
        self._geoms.append(trans)

        ## Rear wheel - a separate body connected by a ball joint, doesn't have a SceneNode
        OgreOde.SphereMass wheel_mass(0.02,0.06)
        self._rear_wheel = OgreOde.Body(self._world) ## NEW
        self._rear_wheel.setMass(wheel_mass)
        self._rear_wheel.setPosition(Vector3(0.0,0.068,-3.937))
        OgreOde.BallJoint* ballself._joint = OgreOde.BallJoint(self._world) ## NEW
        ballself._joint.attach(self._apache,self._rear_wheel)
        ballself._joint.setAnchor(self._rear_wheel.getPosition())
        OgreOde.SphereGeometry* sphere = OgreOde.SphereGeometry(0.06, self._world, self._apacheself._space) ## NEW
        sphere.setBody(self._rear_wheel)
        self._geoms.append(sphere)
        self._bodies.append(self._rear_wheel)
        self._joints.append(ballself._joint)

        ## Left front wheel - connected by a hinge joint so it can only roll forward
        OgreOde.SphereMass front_wheel_mass(0.03,0.17)
        self._left_front = OgreOde.Body(self._world) ## NEW
        self._left_front.setMass(front_wheel_mass)
        self._left_front.setPosition(Vector3(-0.699,0.199,1.163))
        OgreOde.HingeJoint* hingeself._joint = OgreOde.HingeJoint(self._world) ## NEW
        hingeself._joint.attach(self._apache,self._left_front)
        hingeself._joint.setAnchor(self._left_front.getPosition())
        hingeself._joint.setAxis(Vector3.UNIT_X)
        sphere = OgreOde.SphereGeometry(0.17, self._world, self._apacheself._space) ## NEW
        sphere.setBody(self._left_front)
        self._geoms.append(sphere)
        self._bodies.append(self._left_front)
        self._joints.append(hingeself._joint)

        ## Right front wheel - connected by a hinge joint, doesn't have a SceneNode
        self._right_front = OgreOde.Body(self._world)  ## NEW
        self._right_front.setMass(front_wheel_mass)
        self._right_front.setPosition(Vector3(0.699,0.185,1.163))
        hingeself._joint = OgreOde.HingeJoint(self._world) ## NEW
        hingeself._joint.attach(self._apache,self._right_front)
        hingeself._joint.setAnchor(self._right_front.getPosition())
        hingeself._joint.setAxis(Vector3.UNIT_X)
        sphere = OgreOde.SphereGeometry(0.17, self._world, self._apacheself._space) ## NEW
        sphere.setBody(self._right_front)

        self._geoms.append(sphere)
        self._bodies.append(self._right_front)
        self._joints.append(hingeself._joint)

        ## Create the wall of boxes to crash into, make it 
        ## smaller in Debug mode for performance reasons
        int i = 0
        for y in range (1):
            for x in range (-2.0 + (y * 0.5), 2.0 - (y * 0.5) :
                ## Create the Ogre box
                name = "Box_" + str(i)
                box = self._mgr.createEntity(name,"crate.mesh")
                box.setQueryFlags (GEOMETRY_QUERY_MASK)
                box.setCastShadows(True)

                node = self._mgr.getRootSceneNode().createChildSceneNode(name)
                node.attachObject(box)
                node.setScale(0.1,0.1,0.1)

                ## Set the position and rotate that, then rotate the box by the same amount
                q= ODE.Quaternion()
                q.FromAngleAxis(Ogre.Degree(45),Ogre.Vector3.UNIT_Y)
                pos = Ogre.Vector3(x,y + 0.5,-10.0)
                node.setPosition(q * pos)
                node.setOrientation(q)

                ## Create a box for ODE and attach it to the Ogre version
                body = OgreOde.Body(self._world, name + "Body")  ## NEW
                node.attachObject(body)
                body.setMass(OgreOde.BoxMass(0.01,Vector3(1,1,1)))
                body.setDamping (0.005, 0.005)

                geom = OgreOde.BoxGeometry(Vector3(1.0,1.0,1.0),self._world, self._space)  ## NEW
                ## Tie the collision geometry to the physical body
                geom.setBody(body)
                box.setUserObject(geom)

                ## Keep track of the ODE objects for house keeping
                self._bodies.append(body)
                self._geoms.append(geom)

                i+=1

    def __del__( self ):
        ## Destroy the non-standard scene nodes that make up the helicopter
        self._apache_body_node.removeAndDestroyChild("Main Rotor")
        self._apache_body_node.removeAndDestroyChild("Tail Rotor")
        
        ## Remove the entities that represent the scene nodes we destroyed
        self._mgr.destroyEntity("Main Rotor")
        self._mgr.destroyEntity("Tail Rotor")
        
        ## Delete the space in which the helicopter lived
        del self._apacheself._space

    ## Return our name for the test application to display
    def getName(self):
        return "Test Crash"

    ## Return a description of the keys that the user can use in this test
    def getKeys(self):
        return "I/K - Throttle, J/L - Turn, G/B/V/N - Thrust, X - Fire"


    def frameEnded(self,  time,  input,  mouse):
        ## Do default processing
        SimpleScenes.frameEnded(self, time, input, mouse)
        
        ## Throttle up or down
        if (input.isKeyDown(KC_I)):
            self._main_spd += (self._main_response * time)
        elif (input.isKeyDown(KC_K)):
            self._main_spd -= (self._main_response * time)

        ## Thrust left, right, forward, or back
        self._thrust_force = Ogre.Vector3.ZERO
        if (input.isKeyDown(KC_G)) self._thrust_force.z += self._thrust
        if (input.isKeyDown(KC_B)) self._thrust_force.z -= self._thrust
        if (input.isKeyDown(KC_V)) self._thrust_force.x += self._thrust
        if (input.isKeyDown(KC_N)) self._thrust_force.x -= self._thrust

        ## Clamp the main rotor speed
        self._main_spd = std.max(self._main_min,std.min(self._main_max,self._main_spd))

        ## Turn left or right or automatically stop the tail rotor
        if (input.isKeyDown(KC_J)):
            self._tail_spd += (self._tail_response * time)
        elif (input.isKeyDown(KC_L)):
            self._tail_spd -= (self._tail_response * time)
        else:
            if (self._tail_spd < 0.0):
                self._tail_spd += (self._tail_response * time)
                if (self._tail_spd > 0.0):
                    self._tail_spd = 0.0
            elif (self._tail_spd > 0.0):
                self._tail_spd -= (self._tail_response * time)
                if (self._tail_spd < 0.0):
                    self._tail_spd = 0.0
        
        ## Clamp the tail rotor speed
        self._tail_spd = max(self._tail_min,min(self._tail_max,self._tail_spd))

        ## Rotate the tail rotor scene node
        self._tail_rotor_node.rotate(Vector3.UNIT_X,Radian(self._tail_spd * time * self._rotate_scale))

        ## If there's some power being applied to the main rotor then...
        if ((self._main_spd > self._main_min) or (!input.isKeyDown(KC_K))):
            ## Rotate the main rotor scene node
            self._main_rotor_node.rotate(Vector3.UNIT_Y,Radian(self._main_spd * time * self._rotate_scale))

        ## Fire rockets. Woo!
        self._fire_time += time
        if ((self._fire_time > self._fire_rate) and (input.isKeyDown(KC_X))):
            self._fire_time = 0.0
            self._rockets++

            ## Create a rocket node and attach a particle system to it
            name = "Rocket_" + str(self._rockets))
            rocketParticles = self._mgr.createParticleSystem (name, "OgreOdeDemos/Rocket")
            rocket_node = self._mgr.getRootSceneNode ().createChildSceneNode (name)
            rocket_node.attachObject(rocketParticles)
            
            ## Alternate firing between the left and right pods and 
            ## convert it from body coordinates to world
            ## Do the same with the firing "force" vector
            pos = Ogre.Vector3(self._apache.getPointWorldPosition (Vector3 (1.35 * ((self._rockets & 1)?-1.0:1.0),-0.55,0.95)))
            force  Ogre.Vector3(self._apache.getVectorToWorld (Vector3 (0,0,self._fire_force)))

            rocket_node.setPosition (pos)

            ## Create a sphere for the physical body
            rocket_body = OgreOde.Body (self._world)  ## NEW
            rocket_geom = OgreOde.SphereGeometry (self._fire_size, self._world, self._space)  ## NEW

            rocket_body.setMass (OgreOde.SphereMass (self._fire_mass,self._fire_size))
            rocket_body.setAffectedByGravity (False)
            rocket_geom.setBody (rocket_body)

            rocket_node.attachObject (rocket_body)
            
            self._bodies.append (rocket_body)
            self._geoms.append (rocket_geom)

            ## Fire it off by applying an initial force
            rocket_body.addForce (force)

            ## Initialise the rocket's life span to zero
            self._rocket_list[rocket_geom] = 0.0

            
        ## Check all the rockets
        for i in self._rocket_list:

            ## Increase the time it's lived
            rocketTime = i.second
            rocketTime += time

            ## If it had it's emitter disabled (see below) more than 2 seconds ago then kill it
            if (rocketTime < 0.0):
                ## Get the geometry's body and kill any particle system attached to the same node
                killParticleSystem (i.first)

                body = i.first.getBody ()
                ## Manually remove the body from the list of managed bodies
                for bi in self._bodies:
                    if ((bi) == body): 
                        bi = self._bodies.erase(bi)
                    else :
                        bi += 1
                    
                ## Manually delete the geometry from the list of managed geometries
                for gi in self._geoms:
                    if (gi == i.first):
                        gi = self._geoms.erase(gi)
                    else: 
                        gi += 1
                    
                ## Delete the actual body and geometry
                del body                       
                del i.first

                ## Erase the rocket from the hash map
                self._rocket_list.erase(i)
                i += 1
            else :
                ## If the rocket has been alive for more than 2 seconds then kill its emitters (so it fades away)
                if (rocketTime > 3.0):
                    sphereGeom = i.first
                    killEmitters(sphereGeom, rocketTime)
                i += 1

                    
    ## Override the base collision callback, 'cos we don't want the "if connected" check doing
    def collision(self, contact):
        firstGeom = contact.getFirstGeometry()
        secondGeom = contact.getSecondGeometry()

        rocketGeom = 0
        otherGeom = 0
        time = 0
        
        if (firstGeom.getClass () == OgreOde.Geometry.Class_Sphere or 
            secondGeom.getClass () == OgreOde.Geometry.Class_Sphere):
            ## If a rocket's hit something then stop emitting particles
            ## check if it's a sphere (rocket proxy Geom...)
            if (firstGeom.getClass () == OgreOde.Geometry.Class_Sphere):
                li = self._rocket_list.find((firstGeom))
                if (li != self._rocket_list.end()):
                    rocketGeom = li.first
                    time = li.second
                    otherGeom = secondGeom

            if (secondGeom.getClass () == OgreOde.Geometry.Class_Sphere):
#                 std.map<OgreOde.SphereGeometry*,Real>.iterator li = 
#                     self._rocket_list.find(static_cast<OgreOde.SphereGeometry*>(secondGeom))
                li = self._rocket_list.find((secondGeom))
                if (li != self._rocket_list.end()):
                    rocketGeom = li.first
                    time = &(li.second)
                    otherGeom = firstGeom
            if (rocketGeom  and  time > 0):
                ## prevent collision against Apache
                if (otherGeom.getSpace() == self._apacheself._space):
                    return  False

                ## there, Add an Explosion Effect (could make two sphere, one that destroy, one for the wave effect only.)
                explosionCenter =Ogre.Vector3(contact.getPosition ())
                explosionAreaQuery =  self._mgr.createSphereQuery(Sphere(explosionCenter, 20), 0xFFFFFFFF)

                ## asking for movable physic object (no particles system or static geometry (here the plane))
                explosionAreaQuery.setQueryMask (GEOMETRY_QUERY_MASK | VEHICLE_QUERY_MASK)
                explosionAreaQuery.setQueryTypeMask(SceneManager.ENTITY_TYPE_MASK)

                result = explosionAreaQuery.execute ()
                if ( not result.movables.empty()):
                    SceneQueryResultMovableList.const_iterator i = result.movables.begin()
                    while(i != result.movables.end()):
                        ##GEOMETRY_QUERY_MASK
                        affectedGeom = (i.getUserObject ())
                        assert (affectedGeom != rocketGeom)
                        assert (affectedGeom)
                        affectedBody = affectedGeom.getBody ()                        
                        assert (affectedBody)
# # # # #                         Vector3 forceAreaDirection = (*i).getParentNode().getPosition() - explosionCenter
                        forcePower = forceAreaDirection.squaredLength () / 5
                        forceAreaDirection.normalise ()

                        ## Apply the main rotor force
                        affectedBody.addForce(forceAreaDirection * forcePower)
                        
                        i += 1

                ## remove particle and rocket geom
                killEmitters(rocketGeom, time)

                ## add explosion particles
                self._rocket_node_explosion.setPosition(explosionCenter)
                self._rocket_node_explosion.setVisible(True)

                resetParticleSystem(self._rocketParticles_explosion, True, 0.1)
                resetParticleSystem(self._rocketParticles_smoke_white, True, 0.3)
                resetParticleSystem(self._rocketParticles_smoke, True, 0.6)
                
        ## Set the friction at the contact
        contact.setCoulombFriction(5.0)

        ## Yes, this collision is valid
        return True

    ## Will get called just before each time step, since a timestep zeros the force accumulators
    def addForcesAndTorques( self ):
        ## Apply the main rotor force
        self._apache.addForce(Vector3(0,self._main_spd * self._main_scale,0))

        ## Apply the tail rotor torque
        self._apache.addRelativeTorque(Vector3(0,self._tail_spd * self._tail_scale,0))

        ## Apply the thrust force
        self._apache.addRelativeForceAtRelative(self._thrust_force,self._thrust_offs)


    ## Kill the emitters for any particle systems attached to the same node
    ## as the body with which the specified geometry is associated (if that makes sense!)
    def killEmitters( self, geom, time = 0):
        ## Find the body
        body = geom.getBody()
        if (body):
            ## Find the associated scene node
            node = body.getParentNode()
            if (node):
                ## Kill the emitters of any attached particle systems
                for i in range (node.numAttachedObjects() ):
                    obj = node.getAttachedObject(i)
                    if (obj.getMovableType() == "ParticleSystem"):
                        obj.removeAllEmitters()

            ## Set the life span value so that we can easily tell 
            ## that its had its emitters deleted
            if (time): 
                time = -100.0

   def resetParticleSystem( self , ps,  enable,  delay):
      numEmitters = ps.getNumEmitters()
      for i in range( numEmitters ):
         pe = ps.getEmitter(i)
         pe.setEnabled(enable)
         pe.setStartTime (delay)
      if enable: 
         ps.setSpeedFactor( 1.0 )
      else:
         ps.setSpeedFactor( 0.0 )
       
    ## Kill the particle system and scene node associated with this geometry
    def killParticleSystem(self, geom):
        ## Find the body
        body = geom.getBody()
        if (body):
            ## Find the scene node
            node = body.getParentNode()
            if (node):
                obj = 0             
                for i in range( node.numAttachedObjects()):
                    ## If this object isn't an OgreOde.Body then it must be
                    ## (in our demo) a particle system that we need to delete
                    can_obj = node.getAttachedObject(i)
                    if (can_obj.getMovableType() != "OgreOde.Body"): 
                        obj = can_obj

                ## Delete the node
                self._mgr.getRootSceneNode().removeAndDestroyChild(node.getName())

                ## If we found a particle system then delete it
                if (obj): 
                    self._mgr.destroyParticleSystem(obj.getName())

# # # #     ## Things we'll control and delete manually
# # # #     Entity *self._apache_body,*self._main_rotor,*self._tail_rotor
# # # #     SceneNode *self._apache_body_node,*self._main_rotor_node,*self._tail_rotor_node

# # # #     Real self._rotate_scale
# # # #     
# # # #     ## Physical things we'll need to apply forces to and delete manually
# # # #     OgreOde.Body *self._apache,*self._rear_wheel,*self._left_front,*self._right_front
# # # #     OgreOde.SimpleSpace* self._apache,self._space


# # # #     ## The worst helicopter flight model in the world!
# # # #     Real self._main_spd,self._main_response,self._main_scale,self._main_min,self._main_max
# # # #     Real self._tail_spd,self._tail_response,self._tail_scale,self._tail_min,self._tail_max
# # # #     Real self._thrust,self._linear_damp,self._angular_damp
# # # #     Vector3 self._thrust_offs,self._thrust_force

# # # #     ## Stuff to help us shoot things
# # # #     Real self._fire_rate,self._fire_time,self._fire_force,self._fire_mass,self._fire_size
# # # #     std.map<OgreOde.SphereGeometry*,Real> self._rocket_list
# # # #     int self._rockets  


# # # #     SceneNode * self._rocket_node_explosion
# # # #     ParticleSystem * self._rocketParticles_explosion
# # # #     ParticleSystem * self._rocketParticles_smoke_white
# # # #     ParticleSystem * self._rocketParticles_smoke
