import ogre.renderer.OGRE as ogre

data = [{'name': 'TestOutOfOrder', 'parent':'OtherBit', 'rType':'mesh', 'rName':'Rastaman', 'offset':ogre.Vector3(0,0,0)},
        {'name': 'RastamanMesh', 'parent':'root', 'rType':'mesh', 'rName':'Rastaman.mesh', 'offset':ogre.Vector3(0,0,0)},
        {'name': 'mrben', 'parent':'root', 'rType':'actor', 'rName':'rasta_gun'},
        {'name': 'Soil', 'parent':'mrben', 'rType':'mesh', 'rName':'backpack.mesh'},
        {'name': 'OtherBit', 'parent':'mrben', 'rType':'mesh', 'rName':'spade.mesh'},
        {'name': 'EverSpeak', 'parent':'RastamanMesh', 'rType':'particle', 'rName':'Particles/Smoke'},
        {'name': 'EverSpeak2', 'parent':'EverSpeak', 'rType':'mesh', 'rName':'Rastaman.mesh'},
        {'name': 'EverSpeak3', 'parent':'EverSpeak2', 'rType':'mesh', 'rName':'Rastaman.mesh'}]

class mediaTree:
    def __init__(self, items, level='root'):
        self.item = None
        self.tree = {}
        self.level = level
        self.node = None
        self.isBuilt = False
        self.liveResource = None # the bit we gotta delete - this tree holds the only ref to the resource
        for item in items:
            if item['name'] == self.level:
                self.item = item
            if item['parent'] == self.level:
                self.tree[item['name']] = mediaTree(items, item['name'])
    
    def printTree(self, level='root', indent=1):
        if level == self.level:
            print '_' * indent, ':::', self.level
            print '_' * indent, self.item
            print '_' * indent, 'Number of kids', str(len(self.tree))
            for b in self.tree:
                self.tree[b].printTree(b, indent+10)
        
    def find(self, name, item=[]):
        if name == self.level:
            if not self.isBuilt:
                item.append(self.item)
            else:
                item.append([self.node, self.liveResource])
        else: [self.tree[b].find( name ) for b in self.tree]
        return item
                
    def getKids(self, level, retList=[]):
        if level == self.level:
            retList += [self.tree[k].item for k in self.tree]
        else:
            [self.tree[k].getKids(level) for k in self.tree]
        return retList
                
    def build(self, sceneManager, actorName, parentNode=None):
        if self.item:
            print 'Building Item', self.item['name'], 'of type', self.item['rType'],
            print 'as child of', self.item['parent']
            name = actorName + self.item['name']
            self.prepResource(sceneManager, name)
            self.node = 'Node' # self.node = parentNode.createChildSceneNode()
            # self.node.attachObject(self.liveResource).
            self.isBuilt = True
        for b in self.tree:
            self.tree[b].build( sceneManager, actorName, self.node )

    def destroy(self, sceneManager):
        # destroy children first
        # recurse backwards up to the OgreNode
        print 'performing destroy on', self.level
        for k in self.tree:
            self.tree[k].destroy( sceneManager )
        if self.node and self.liveResource:
            print 'destroying data for:', self.level
            #self.destroyResource( sceneManager )
            #sceneManager.destroySceneNode(self.node)


    def destroyResource(self, sceneManager):
        if self.liveResource:
            if self.item['rType'] == 'mesh':
                sceneManager.destroyEntity( self.liveResource )
            elif self.item['rType'] == 'particle':
                sceneManager.destroyParticleSystem( self.liveResource )
            elif self.item['rType'] == 'ribbon':
                sceneManager.destroyRibbonTrail( self.liveResource )

    def prepResource(self, sceneManager, name):
        if self.item['rType'] == 'mesh':
            #self.liveResource = sceneManager.createEntity(name, self.item['rName'])
            self.liveResource = 'Mesh'
        elif self.item['rType'] == 'particle':
            #self.liveResource = sceneManager.createParticleSystem(name, self.item['rName'])
            self.liveResource = 'Particle'
        elif self.item['rType'] == 'billboard':
            self.liveResource = 'Billboard'
        elif self.item['rType'] == 'actor':
            #self.liveResource.Delete() # call the delete method of the nested actor
            self.liveResource = 'Actor'
        
            
tree = mediaTree(data)
tree.printTree()
k = tree.getKids('mrben')
f = tree.find('OtherBit')
print 'found', str(f)
print 'kids of mrben', str(k)
tree.build('scn', 'testActor')
tree.destroy('scn')



