#
# A boids implementation by willism...
#
# Todos:
#    * Better structure the base class (class Boid)
#    * Maybe do some of the maintenance in update() instead of move()?
#    * Rotate each boid's scenenode in a realistic way
#    * Use a frustrum to determine which neighbors a boid can see

import ogre.renderer.OGRE as ogre

import base_actor

import math
import random

gauss = random.Random().gauss

def trim(vector, maxLength):    
    """If the vector is longer than the given maxLength, shorten it to
       be (approximately) the maxLength.  Return true if the vector was
       trimmed, false if not."""
    if vector.squaredLength() / (maxLength * maxLength) > 1.0:
        vector.normalise()
        vector *= maxLength
        return True
    
    return False

class Boid(base_actor.GameActor):

    def __init__(self):
        base_actor.GameActor.__init__(self)
        self.hasAI = True
        self.adjustHeight = False
        self.hasPhysics = False

        self.velocity = ogre.Vector3(0.0,0.0,1.0)
        self.accel = ogre.Vector3(0.0,0.0,0.0)

        self.maxAccel = 2.0
        self.maxSpeed = 10.0
        self.cruisingSpeed = 5.0
        self.wander_deviation = 1.0
        
        self.rules = []
        self.neighbors = []
        self.nearest_neighbor = None

        self.pitch = ogre.Radian(0.0)
        self.yaw = ogre.Radian(0.0)
        self.roll = ogre.Radian(0.0)        

    def find_neighbors(self):
        self.neighbors = []
        self.nearest_neighbor = None

    def move(self, time):        
        
        self.find_neighbors()
        
        self.accel = ogre.Vector3(0.0,0.0,0.0)
                
        for rule in self.rules:
            # Adjust acceleration according to current rule
            self.accel += rule()
            
            # Stop running rules when maxAccel reached            
            if trim(self.accel, self.maxAccel):
                break;
        
        # adjust velocity, then cut it down if needed
        self.velocity += self.accel * time
        trim(self.velocity, self.maxSpeed)
      
        self.OgreNode.roll(-self.roll)
        self.OgreNode.pitch(-self.pitch)
        self.OgreNode.yaw(-self.yaw)          
        
        (self.pitch,self.yaw,self.roll) = self.calculatePitchYawRoll()
        self.OgreNode.yaw(self.yaw)    
        self.OgreNode.pitch(self.pitch)    
        self.OgreNode.roll(self.roll)        
        
        self.OgreNode.position += self.velocity * time
        
    def calculatePitchYawRoll(self):
        lateral = self.velocity.crossProduct(self.accel).crossProduct(self.velocity)
        lateral.normalise()
        
        lateralMag = self.accel.dotProduct(lateral)
        
        if (lateralMag == 0):
            roll = 0
        else:
            roll = -math.atan2(9.8, lateralMag) + math.pi / 2.0

        pitch = -math.atan(self.velocity.y / math.sqrt(self.velocity.z * self.velocity.z + self.velocity.x * self.velocity.x))

        yaw = math.atan2(self.velocity.x, self.velocity.z);

        return (ogre.Radian(x) for x in (pitch, yaw, roll))
        
    def flock(self):
        """Steer toward center of nearby boids"""
        vector = ogre.Vector3(0.0,0.0,0.0)
        for boid in self.neighbors:
            vector += boid.OgreNode.position
            
        vector /= len(self.neighbors)
        vector -= self.OgreNode.position
        
        trim(vector, 0.3)
        return vector
        
    def maintain_distance(self):
        """Maintain comfortable distance from closest neighbor"""
        vector = ogre.Vector3(0.0,0.0,0.0)

        # Check if too close to neighbor
        if self.nearest_neighbor != None:
            if self.distance_to_neighbor < self.comfort_distance_near:
                vector = self.OgreNode.position - self.nearest_neighbor.OgreNode.position
                trim(vector, 0.5)
            
        return vector
        
    def match_velocity(self):
        """Try to match closest boid's velocity"""
        vector = ogre.Vector3(0.0,0.0,0.0)
        
        if self.nearest_neighbor != None:
            vector = self.nearest_neighbor.velocity - self.velocity
            trim(vector, 0.4)
            
        return vector
    
    def avoid_ground(self):
        """Try to avoid the ground"""
        if self.OgreNode.position.y < 12.0:
            return ogre.Vector3(0.0, 10.0, 0.0)
        return ogre.Vector3(0.0,0.0,0.0)
    
    def maintain_altitude(self):
        """Try to maintain altitude"""
        vector = ogre.Vector3(0.0, -self.velocity.y, 0.0)
        trim(vector, 0.1)
        return vector
        
    def stay_in_bubble(self):
        """Try to stay in the bubble"""
        vector = ogre.Vector3(0.0,0.0,0.0)
        
        if self.OgreNode.position.squaredLength() > 40000:
            vector = -self.OgreNode.position
            trim(vector, 2.0)

        return vector
    
    def maintain_cruising_speed(self):
        """Speed up or slow down to stay going a comfortable speed"""
        vector = ogre.Vector3(0.0,0.0,0.0)

        cruisingSpeedSquared = self.cruisingSpeed * self.cruisingSpeed
        
        if self.velocity.squaredLength() > cruisingSpeedSquared:
            vector = -self.velocity
        
        if self.velocity.squaredLength() < cruisingSpeedSquared:
            vector = ogre.Vector3(self.velocity)

        trim(vector, 0.1)
        
        return vector
    
    def wander(self):
        """wander a little bit"""
        (x,y,z) = [gauss(0.0,self.wander_deviation) for i in range(3)]
        return ogre.Vector3(x,y,z)

birds = []
        
class Bird(Boid):
    def __init__(self):
        Boid.__init__(self)
        self.media = [{'name':'BirdMesh', 'parent':'root', 'rType':'mesh', 'rName':'Triangle.mesh'}]
        self.name = 'Bird'
        self.isAnimated = False
        
        self.rules = [self.avoid_ground,
                      self.maintain_distance,
                      self.stay_in_bubble,
                      self.maintain_altitude,
                      self.maintain_cruising_speed,
                      self.match_velocity,                      
                      self.flock,                      
                      self.wander]
        
        self.velocity = ogre.Vector3(0.0,0.0,5.0)
        
        self.comfort_distance_near = 2.5 + random.random() * 2.5
        self.visibility = 50.0
        self.maxSpeed = 8.0
        
        birds.append(self)
        
    def find_neighbors(self):

        self.neighbors = []
        self.nearest_neighbor = None
        self.distance_to_neighbor = self.visibility
        
        # Find out info about neighbors
        for actor in birds:
            if actor is not self:
                distance = (self.OgreNode.position - actor.OgreNode.position).length()
    
                if distance < self.visibility:
                    self.neighbors.append(actor)
    
                if distance < self.distance_to_neighbor:
                    self.nearest_neighbor = actor
                    self.distance_to_neighbor = distance
        
    def Update(self, actors, player, updateAITime, time):
        pass
    
    
class Dragon(Boid):
    def __init__(self):
        Boid.__init__(self)
        self.media = [{'name':'DragonMesh', 'parent':'root', 'rType':'mesh', 'rName':'dragon.mesh'},
        {'name':'Smokey', 'parent':'DragonMesh', 'rType':'particle', 'rName':'Examples/DragonFire', 'offset':(100.0, 0.0, 80.0)}]
        self.name = 'Bird'
        self.isAnimated = False
        self.nodeScale = 0.1

        self.velocity = ogre.Vector3(0.0,0.0,-50.0)
        self.maxSpeed = 100.0
        
        self.rules = [self.avoid_ground, self.stay_in_bubble, self.maintain_cruising_speed, self.wander]

    def setActorOptions(self, world):
        pass

    def calculatePitchYawRoll(self):
        (p,y,r) = Boid.calculatePitchYawRoll(self)
        
        return (p + ogre.Radian(math.pi), y, r + ogre.Radian(math.pi))