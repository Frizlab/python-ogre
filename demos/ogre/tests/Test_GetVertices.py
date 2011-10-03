import os,sys 
if sys.platform == 'win32': 
    os.environ['PATH'] += ';' + __file__[0] 

import ogre.renderer.OGRE as ogre 
import SampleFramework 
import math 

def GetIndicies ( sm ) :
    outbuff = []
    
    numTris = sm.indexData.indexCount
    if numTris == 0 : return outbuff
    else : numTris /= 3
    
    ibuf = sm.indexData.indexBuffer
    
    if ibuf.getType() == ogre.HardwareIndexBuffer.IT_32BIT:
        Use32Bit = True
    else: Use32Bit = False
    
    buffer = ibuf.lock(ogre.HardwareBuffer.HBL_READ_ONLY)
    address = ogre.CastInt ( buffer )
        
    for i in range( numTris ):
        if Use32Bit:
            l = ogre.getUint32 ( ogre.CastVoidPtr ( address ), 3 )
            address += ibuf.getIndexSize() * 3 
        else:
            l = ogre.getUint16 ( ogre.CastVoidPtr ( address ), 3 )
            address += ibuf.getIndexSize() * 3
        outbuff.append(tuple(l))
    return outbuff
    
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
        outbuff.append(tuple(points))
        newaddress += vbuf.getVertexSize()
    return outbuff
            

class TutorialApplication(SampleFramework.Application): 
    def _createScene(self): 

        self.entity = self.sceneManager.createEntity('head', 'ogrehead.mesh')
        mesh=self.entity.Mesh
        
        v = []
        i = []
        for m in range(mesh.numSubMeshes):
            sm = mesh.getSubMesh(m)
            v += GetVerticies ( sm )
            i += GetIndicies ( sm )            
#         print v    
        print i
        sys.exit()
        
 

if __name__ == '__main__': 
    ta = TutorialApplication() 
    ta.go() 
