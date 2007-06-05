import os,sys 
if sys.platform == 'win32': 
    os.environ['PATH'] += ';' + __file__[0] 

import ogre.renderer.OGRE as ogre 
import SampleFramework 
import math 


def GetVerticies ( sm ) :
    outbuff = []
    pmesh = sm.parent
    if pmesh and sm.useSharedVertices :
        vertex_data = pmesh.sharedVertexData
    else : vertex_data = sm.vertexData

    # get the start of the element
    posElem = vertex_data.vertexDeclaration.findElementBySemantic(ogre.VertexElementSemantic.VES_POSITION)
    vbuf = vertex_data.vertexBufferBinding.getBuffer(posElem.getSource())
    
    # get the start of the actual buffer
    vertex = vbuf.lock(ogre.HardwareBuffer.HBL_READ_ONLY)
    
    # what we need is the actual address of the buffer, so we have to check if there's an offset as well
    # the is basically what baseVertexPointerToElement does 
    newaddress = posElem.getOffset() + ogre.CastInt(vertex) # note the cast to int to get the address
        
    for i in range (vertex_data.vertexCount):
        points = ogre.getFloat( ogre.CastVoidPtr ( newaddress), 3 ) # note the getFloat and Cast back to a pointer :)
        outbuff += points
        newaddress += vbuf.getVertexSize()
    return outbuff
            

class TestListener(ogre.RenderTargetListener):
    def __init__(self, name):
        "Init Render Application"
        ogre.RenderTargetListener.__init__( self )
        self.name = name
        pass
        
    def preRenderTargetUpdate(self,evt):
        print "preRenderTargetUpdate"
           
    def postRenderTargetUpdate(self,evt):
        print "postRenderTargetUpdate"

        
    def preViewportUpdate(self,evt):
        print "preViewportUpdate", name, evt.source
           
    def postViewportUpdate(self,evt):
        print "postViewportUpdate", name, evt.source

class TutorialApplication(SampleFramework.Application): 
    def _createScene(self): 
        sm = self.sceneManager 
        cam = self.camera
        rw = self.renderWindow
        
        print "Cam Position"
        print cam
        cam.setPosition ( 10,11,12)
        print cam
        pos = cam.getPosition()
        print pos
        pos = cam.position
        print pos
        cam.position=20,30,400
        print cam.position
        cam.position=0,0,400
        
#         self.mTestCam = sm.createCamera("TestCam")
#         self.listener1 = TestListener( "one" )
#         self.listener2 = TestListener ( "two" )
#         vp1 = rw.getViewport(0)
#         vp2 = rw.addViewport ( self.mTestCam, 1 )
#         rw.addListener( self.listener1 )

        # create head entity (left)
        self.headNode = sm.getRootSceneNode().createChildSceneNode('head1')
        self.headNode.position=-70,0,0
        self.entity = sm.createEntity('head', 'ogrehead.mesh')
        self.headNode.attachObject(self.entity)
        
        # create head entity (right)
        self.headNode2 = sm.createSceneNode('Head2')
        self.headNode.position=70,0,0
        sm.getRootSceneNode().addChild(self.headNode2)
        self.entity2 = sm.createEntity('head2', 'ogrehead.mesh')
        self.headNode2.attachObject(self.entity2)
        
        hh = sm.getRootSceneNode().removeChild('Head2')
        print "\nremoved\n",hh,"\n"
        sm.getRootSceneNode().addChild(hh)

        print "\n\nENTITY"
        mesh=self.entity.Mesh
        
        v = []
        i = []
        for m in range(mesh.numSubMeshes):
            sm = mesh.getSubMesh(m)
            v += GetVerticies ( sm )
            
        print v    
        
        
        
        sys.exit()
        
        
#         self.childnode = sm.createSceneNode('childnode')    # renders fine
#         print "ChildNode" , self.childnode
#         sm.getRootSceneNode().addChild(self.childnode)
#         self.childnode.attachObject(self.entity)
#         
        
#         sm.getRootSceneNode().removeChild(1)
#         sm.getRootSceneNode().addChild(self.node1)
#         self.node1.attachObject(self.entity)
#         
#         print "rnode", self.node1
#         self.entity1 = sm.createEntity('head1', 'ogrehead.mesh')
#         self.childnode.attachObject(self.entity1)
#         
#         childnode2 = sm.getRootSceneNode().createChild('nnn')
#         print "smNode", sm.getRootSceneNode()
#         print "ChildNode2", childnode2
#         self.entity1 = sm.createEntity('head1', 'ogrehead.mesh')
#         childnode2.attachObject(self.entity1)
               
#         works = True   # changing this to False, I no longer see the entity
#         
#         self.rootnode = sm.getRootSceneNode()
#         if works:
#            self.childnode = sm.createSceneNode('childnode')    # renders fine
#            self.rootnode.addChild(self.childnode)
#         else:
#            self.childnode = self.rootnode.createChild('childnode')  # doesn't render
#         
#         self.entity = sm.createEntity('sample','RZR-002.mesh')
#         self.childnode.attachObject(self.entity)

if __name__ == '__main__': 
    ta = TutorialApplication() 
    ta.go() 
