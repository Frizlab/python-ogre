import ogre.renderer.OGRE as ogre
import ogre.io.OIS as ogreio

import ode
import xode.parser

import random





# Collision callback
def near_callback(args, geom1, geom2):
    """Callback function for the collide() method.

    This function checks if the given geoms do collide and
    creates contact joints if they do.
    """

    # Check if the objects do collide
    contacts = ode.collide(geom1, geom2)

    # Create contact joints
    world,contactgroup = args
    for c in contacts:
        print "Contact!"
        c.setBounce(0.8)
        c.setMu(5000)
        j = ode.ContactJoint(world, contactgroup, c)
        j.attach(geom1.getBody(), geom2.getBody())





class World:
    def __init__(self, sceneManager):
        self.sceneManager = sceneManager

        self.world = ode.World()
        self.world.setGravity( (0,-9.81,0) )
        self.space = ode.QuadTreeSpace((0, 0, 0), (2000, 500, 2000), 10)          
        self.contactgroup = ode.JointGroup()
        
        numberOfSpheres = random.randint(4, 25)
        
        self.sphereBodies = []
        self.sphereGeoms = []
        for i in range(numberOfSpheres):
            body = ode.Body(self.world)
            mass = ode.Mass()
            radius = random.random()*5+1
            mass.setSphere(1/500.0, radius)
            print "Mass: " + str(mass.mass)
            body.setMass(mass)
            body.setPosition( (random.random()*200, random.random()*50+20, random.random()*200) )
            body.addForce( (0, 200*mass.mass, 0) )
            self.sphereBodies.append(body)   
            geom = ode.GeomSphere(self.space, radius)
            geom.setBody(body) 
            self.sphereGeoms.append(geom)
                
        f = open("vehicle.xml", "r")
        xml = f.read()
        f.close()
        
        p = xode.parser.Parser()
        root = p.parseString(xml)
        
        self.ground = root.namedChild('ground').getODEObject()
        wrongSpace = root.namedChild('space').getODEObject()
        wrongSpace.remove(self.ground)
        self.space.add(self.ground)
        #del wrongSpace
        


        print "halloj: " + str(self.ground)
        


        
    def setupObjects(self):
        
        sceneManager = self.sceneManager
        
        n, d = self.ground.getParams()
        plane = ogre.Plane(n, d)
        mm = ogre.MeshManager.getSingleton()
        mm.createPlane('ground', ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME,
                      plane, 1500, 1500, 20, 20, True, 1, 5, 5, (0, 0, 1))
        ent = sceneManager.createEntity("GroundEntity", "ground")
        sceneManager.rootSceneNode.createChildSceneNode().attachObject(ent)
        ent.setMaterialName("Examples/Rockwall")
        ent.castShadows = False
        
        self.sphereNodes = []
        for i in range(len(self.sphereBodies)):
            sphereBody = self.sphereBodies[i]
            entity = sceneManager.createEntity("Sphere" + str(i), "sphere.mesh")
            pos = sphereBody.getPosition()
            print "Creating sphereNode at " + str(pos)
            node = sceneManager.rootSceneNode.createChildSceneNode("SphereNode" + str(i), pos)
            node.attachObject(entity)
            radius = self.sphereGeoms[i].getRadius()
            node.scale = (radius/100.0, radius/100.0, radius/100.0)
            self.sphereNodes.append(node)
    
    def advanceFrame(self,deltaTime):
        print "Time since last frame " + str(deltaTime)
        
        # Detect collisions and create contact joints
        
        deltaTime = 0.02
        if deltaTime > 0:
          #  for a in range(2):
                self.space.collide((self.world,self.contactgroup), near_callback)
                
                self.world.step(deltaTime)
            
                self.contactgroup.empty()
            
        print "Looping through the " + str(len(self.sphereNodes)) + " nodes"
        for i in range(len(self.sphereNodes)):
            body = self.sphereBodies[i]
            pos = body.getPosition()
            force = -0.05 * body.getMass().mass
            body.addForce( (force*pos[0], 0, force*pos[2]) )
            node = self.sphereNodes[i]
            node.position = pos
            print "Position: " + str(node.position)
            
        #self.node.position = self.body.getPosition()

    
    