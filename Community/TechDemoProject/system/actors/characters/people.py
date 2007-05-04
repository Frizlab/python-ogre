import ogre.renderer.OGRE as ogre

import base_actor

class RastaMan(base_actor.GameActor):
    def __init__(self):
        base_actor.GameActor.__init__(self)
        self.media = [{'name':'RastaManMesh', 'parent':'root', 'rType':'mesh', 'rName':'rastaMan.mesh'},
        {'name':'Fish', 'parent':'RastaManMesh', 'rType':'mesh', 'rName':'fish.mesh', 'offset':(0.2, -1, 0)},
        {'name':'Smokey', 'parent':'RastaManMesh', 'rType':'particle', 'rName':'Examples/Smoke', 'offset':(0.2, 0, 0)}]
        self.name = 'RastaMan'
        self.isAnimated = True
        self.hasAI = True
        self.animations = ['Run', 'Idle']
        self.nodeScale = 0.7
        self.heightAdjust = 5.0
        self.animSpeed = 0.3
        self.adjustHeight = False
        self.hasPhysics = True
        
    def setActorOptions(self, world):
        # Demonstrate some of the properties of the mediaTree
        print self.name, 'Setting Actor Options'
        self.MediaTree.printTree()
        Node, Entity = self.MediaTree.find('RastaManMesh')
        print 'Node:', str(Node), 'Entity:', str(Entity)
        Node.pitch(ogre.Degree(-90))
        self.curAnimationStates.append(Entity.getAnimationState('Run'))
        self.curAnimationStates[-1].enabled = True
        self.makeSphereCollision( world )
        Node, Entity = self.MediaTree.find('Fish')
        self.curAnimationStates.append(Entity.getAnimationState('swim'))
        self.curAnimationStates[-1].enabled = True
        
        
        

    def move(self, time):
        self.updateHeight()

        
    def Update(self, actors, player, updateAITime, time):
        pass
        
class Ball(base_actor.GameActor):
    def __init__(self):
        base_actor.GameActor.__init__(self)
        self.mesh = 'ellipsoid.mesh'
        self.name = 'ball'
        self.isAnimated = False
        self.hasAI = False
        self.hasPhysics = True
        
    def setActorOptions(self, world):
        self.makeSphereCollision(world)


#
# Following is a boids implementation by willism...
#
# Todos:
#    * This should probably be moved out into it's own file.
#    * Pull out some of the common stuff so that other actors
#      can use the boids algorithm (fish, butterflies, etc.)
#    * Maybe do some of the maintenance in update() instead of move()?
#

import random

birds = []
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
    
class Bird(base_actor.GameActor):
    def __init__(self):
        base_actor.GameActor.__init__(self)
        self.media = [{'name':'BirdMesh', 'parent':'root', 'rType':'mesh', 'rName':'Triangle.mesh'}]
        self.name = 'Bird'
        self.isAnimated = False
        self.hasAI = True
        self.adjustHeight = False
        self.hasPhysics = False
        
        self.rules = [self.avoid_ground,
                      self.maintain_distance,
                      self.stay_in_bubble,
                      self.maintain_altitude,
                      self.maintain_cruising_speed,
                      self.match_velocity,                      
                      self.flock,                      
                      self.wander]
        
        self.velocity = ogre.Vector3(0.0,0.0,5.0)
        self.accel = ogre.Vector3(0.0,0.0,0.0)
        
        self.comfort_distance_near = 2.5 + random.random() * 2.5
        self.wander_deviation = 2.0
        self.visibility = 50.0
        self.maxSpeed = 8.0
        self.maxAccel = 1.0
        self.cruisingSpeed = 5.0
        
        birds.append(self)
        
        self.nearest_neighbor = None
        self.neighbors = []

    def move(self, time):
        
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

        self.accel = ogre.Vector3(0.0,0.0,0.0)
                
        for rule in self.rules:
            # Adjust acceleration according to current rule
            self.accel += rule(birds)
            
            # Stop running rules when maxAccel reached            
            if trim(self.accel, self.maxAccel):
                break;
        
        # adjust velocity, then cut it down if needed
        self.velocity += self.accel * time
        trim(self.velocity, self.maxSpeed)
        
        self.OgreNode.position += self.velocity * time
        
    def flock(self, actors):
        """Steer toward center of nearby boids"""
        vector = ogre.Vector3(0.0,0.0,0.0)
        for boid in self.neighbors:
            vector += boid.OgreNode.position
            
        vector /= len(self.neighbors)
        vector -= self.OgreNode.position
        
        trim(vector, 0.4)
        return vector
        
    def maintain_distance(self, actors):
        """Maintain comfortable distance from closest neighbor"""
        vector = ogre.Vector3(0.0,0.0,0.0)

        # Check if too close to neighbor
        if self.nearest_neighbor != None:
            if self.distance_to_neighbor < self.comfort_distance_near:
                vector = self.OgreNode.position - self.nearest_neighbor.OgreNode.position
                trim(vector, 0.5)
            
        return vector
        
    def match_velocity(self, actors):
        """Try to match closest boid's velocity"""
        vector = ogre.Vector3(0.0,0.0,0.0)
        
        if self.nearest_neighbor != None:
            vector = self.nearest_neighbor.velocity - self.velocity
            trim(vector, 0.5)
            
        return vector
    
    def avoid_ground(self, actors):
        """Try to avoid the ground"""
        if self.OgreNode.position.y < 12.0:
            return ogre.Vector3(0.0, 1.0, 0.0)
        return ogre.Vector3(0.0,0.0,0.0)
    
    def maintain_altitude(self, actors):
        """Try to maintain altitude"""
        vector = ogre.Vector3(0.0, -self.velocity.y, 0.0)
        trim(vector, 0.1)
        return vector
        
    def stay_in_bubble(self, actors):
        """Try to stay in the bubble"""
        vector = ogre.Vector3(0.0,0.0,0.0)
        
        if self.OgreNode.position.squaredLength() > 40000:
            vector = -self.OgreNode.position
            trim(vector, 0.9)

        return vector
    
    def maintain_cruising_speed(self, actors):
        """Speed up or slow down to stay going a comfortable speed"""
        vector = ogre.Vector3(0.0,0.0,0.0)

        cruisingSpeedSquared = self.cruisingSpeed * self.cruisingSpeed
        
        if self.velocity.squaredLength() > cruisingSpeedSquared:
            vector = -self.velocity
        
        if self.velocity.squaredLength() < cruisingSpeedSquared:
            vector = ogre.Vector3(self.velocity)

        trim(vector, 0.1)
        
        return vector
    
    def wander(self, actors):
        # wander a little bit
        (x,y,z) = [gauss(0.0,self.wander_deviation) for i in range(3)]
        return ogre.Vector3(x,y,z)
        
    def Update(self, actors, player, updateAITime, time):
        pass