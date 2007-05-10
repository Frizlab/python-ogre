import ogre.renderer.OGRE as ogre

import psyco
psyco.full()

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


