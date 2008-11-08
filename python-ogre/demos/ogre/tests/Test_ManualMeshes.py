import ogre.renderer.OGRE as ogre
import sys
import SampleFramework 
import math 

class ManuallyDefinedMesh: 
    def __init__(self,Name, Vertices_Pos, Vertices_Norm, Faces, Vertices_Colour=None, CreateImediately=True): 
        """ An experimental class to define meshes programatically
        'Name' is the name by which the mesh will be referenced 
        Vertices_Pos, Vertices_Norm are botj lists of triplets 
        Faces is a simple list of vertex indices
        Vertices_Colour is an optional list of ogre.ColourValue's
        You can delay the mesh loading by setting 'CreateImediately' to False
        Even if you assign colors to your vertexes, you will have to assign a
        material to each entity created based on this mesh."""
        
        assert len(Vertices_Pos)==len(Vertices_Norm) 
        assert (len(Faces)%3)==0 #Each face is defined by 3 consecutive vertex indices 
        
        if Vertices_Colour is not None : 
            assert len(Vertices_Pos)==len(Vertices_Colour) 
        
        meshMgr = ogre.MeshManager.getSingleton() 
        self.mesh = meshMgr.createManual(Name, ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        self.subMesh = mesh.createSubMesh() 
      
        POSITION_ELEMENT = 0 
        NORMAL_ELEMENT = 1 
        UV_ELEMENT = 2  
        COLOUR_ELEMENT = 0  
        
        self.Vertices_Pos = Vertices_Pos 
        self.Vertices_Norm = Vertices_Norm 
        self.Vertices_Colour = Vertices_Colour 
        
        #  ?????????????? / Not sure about this, without the get crash 
        mesh.sharedVertexData = ogre.VertexData() 
        mesh.sharedVertexData.vertexCount = len(Vertices_Pos) 

        #work-around to prevent destruction 
        self.sharedVertexData = mesh.sharedVertexData      
      
        declaration = self.mesh.sharedVertexData.vertexDeclaration 
        offset = 0 
        offset += ogre.VertexElement.getTypeSize(ogre.VET_FLOAT3) 
        declaration.addElement(0, offset, ogre.VET_FLOAT3, ogre.VES_POSITION) 
        offset += ogre.VertexElement.getTypeSize(ogre.VET_FLOAT3) 

        declaration.addElement(0, offset, ogre.VET_FLOAT3, ogre.VES_NORMAL) 
        offset += ogre.VertexElement.getTypeSize(ogre.VET_FLOAT3) 

        hBuffMgr = ogre.HardwareBufferManager.getSingleton() 
        vbuf = hBuffMgr.createVertexBuffer(offset, mesh.sharedVertexData.vertexCount, 
                                           ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY) 


# #         pVertex = vbuf.lock(declaration, ogre.HardwareBuffer.HBL_DISCARD) 
        pVertex = vbuf.lock(ogre.HardwareBuffer.HBL_DISCARD) 
        print pVertex
        
        pVert= ( ctypes.c_float * (mesh.sharedVertexData.vertexCount)).from_address ( ogre.CastInt ( pVertex ) ) 
      
        #loop through the position array 
        index = 0
        for i in range(len(Vertices_Pos)): 
            vx, vy, vz = Vertices_Pos[i] 
            nx, ny, nz = Vertices_Norm[i] 
            pVert[i
            pVertex.setFloat(i, POSITION_ELEMENT, vx, vy, vz) 
            pVertex.setFloat(i, NORMAL_ELEMENT, nx, ny, nz) 
            
        index = 0
        for i in range ( 3 ):
            off = i*4
            pVert[ index ] = 0 + off
            index +=1
            pVert[ index ] = 3 + off
            index +=1
            pVert[ index ] = 1 + off
            index +=1
            pVert[ index ] = 0 + off
            index +=1
            pVert[ index ] = 2 + off
            index +=1
            pVert[ index ] = 3 + off
            index +=1

            
            
        bind = mesh.sharedVertexData.vertexBufferBinding 
        bind.setBinding(0,vbuf) #Set vertex buffer binding so buffer 0 is bound to positional data 

        subMesh.indexData.indexCount = len(Faces) 
        subMesh.indexData.indexBuffer = hBuffMgr.createIndexBuffer(ogre.HardwareIndexBuffer.IT_16BIT, 
                                                len(Faces), ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY) 

        ibuf = subMesh.indexData.indexBuffer 
        subMesh.indexData.indexBuffer.writeIndexes(0, Faces) 

        vbuf.unlock() 
        
            
        #2nd buffer , colours 
        if Vertices_Colour is not None : 
            offset = 0 
            declaration.addElement(1, offset, ogre.VET_COLOUR, ogre.VES_DIFFUSE) 
            offset += ogre.VertexElement.getTypeSize(ogre.VET_COLOUR) 
            vbuf = ogre.HardwareBufferManager.getSingleton().createVertexBuffer( 
                    offset, mesh.sharedVertexData.vertexCount, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY) 
# #             pVertex = vbuf.lock(declaration, ogre.HardwareBuffer.HBL_DISCARD) 
            pVertex = vbuf.lock(ogre.HardwareBuffer.HBL_DISCARD) 
    
            for i in range(len(Vertices_Colour)): 
                pVertex.setColour(i, COLOUR_ELEMENT, Vertices_Colour[i]) 
            vbuf.unlock() 
    
            #Set vertex buffer binding so buffer 1 is bound to our colour buffer 
            bind.setBinding(1, vbuf)        

        subMesh.useSharedVertices = True 
        subMesh.indexData.indexBuffer = ibuf 
        subMesh.indexData.indexCount = len(Faces) 
        subMesh.indexData.indexStart = 0 
        self.indexData = subMesh.indexData 
        
        if CreateImediately: 
            self.MakeAvailable() 
            
    def MakeAvailable(self,BoundBox=(None,None,None,None,None,None), BoundSphereRadius=None): 
        
        if None in BoundBox: #Simple guess of the bounding box 
            xcoord = [i[0] for i in self.Vertices_Pos] 
            ycoord = [i[1] for i in self.Vertices_Pos] 
            zcoord = [i[2] for i in self.Vertices_Pos] 
            BoundBox = (min(xcoord),min(ycoord),min(zcoord),max(xcoord),max(ycoord),max(zcoord)) 
        
        if BoundSphereRadius is None : #Very naive guess of the bounding sphere radius 
            BoundSphereRadius = max([abs(BoundBox[i+3]-BoundBox[i]) for i in range(3)]) 
        
        mesh=self.mesh 
        
        mesh._setBounds(ogre.AxisAlignedBox(*BoundBox)) 
        mesh._setBoundingSphereRadius(BoundSphereRadius)    
        
        mesh.load() 
        
    def Print(self): 
        mesh = self.mesh 
        print "Number SubMeshes",mesh.numSubMeshes 
        for i in range(mesh.numSubMeshes): 
            for f in (mesh.getSubMesh(i).indices): 
                print f          
            for v in ( mesh.getSubMesh(i).getVertices(ogre.Vector3(0.0,0.0,0.0), 
                       ogre.Quaternion.IDENTITY, ogre.Vector3(1.0,1.0,1.0))): 
                print v

                                
class TutorialApplication(SampleFramework.Application): 
    def _createScene(self): 
        sm = self.sceneManager 

        self.headNode = sm.getRootSceneNode().createChildSceneNode('head1')
        self.headNode.position=-70,0,0
        self.entity = sm.createEntity('head', 'ogrehead.mesh')
        self.headNode.attachObject(self.entity)                
        
        self.TestManual()

    def TestManual ( self ):
        sqrt13 = 0.577350269 # sqrt(1/3) 
        x = 30.0 
        y = 30.0 
        z = 30.0 
        vert_pos = [ 
        (-x,y,-z),        #0 position 
        (x,y,-z),         #1 position 
        (x,-y,-z),        #2 position 
        (-x,-y,-z),       #3 position 
        (-x,y,z),         #4 position 
        (x,y,z),          #5 position 
        (x,-y,z),         #6 position 
        (-x,-y,z)         #7 position 
        ] 
        vert_norm = [ 
        (-sqrt13,sqrt13,-sqrt13),     #0 normal 
        (sqrt13,sqrt13,-sqrt13),      #1 normal 
        (sqrt13,-sqrt13,-sqrt13),     #2 normal 
        (-sqrt13,-sqrt13,-sqrt13),    #3 normal 
        (-sqrt13,sqrt13,sqrt13),      #4 normal 
        (sqrt13,sqrt13,sqrt13),       #5 normal 
        (sqrt13,-sqrt13,sqrt13),      #6 normal 
        (-sqrt13,-sqrt13,sqrt13)      #7 normal 
        ] 
        faces = [ 
        0,2,3, 
        0,1,2, 
        1,6,2, 
        1,5,6, 
        4,6,5, 
        4,7,6, 
        0,7,4, 
        0,3,7, 
        0,5,1, 
        0,4,5, 
        2,7,3, 
        2,6,7] 
        
        rs = self.root.getRenderSystem() 
#         colours=[] 
#         colours.append(rs.convertColourValue(ogre.ColourValue(1.0,0.0,0.0))) #0 colour 
#         colours.append(rs.convertColourValue(ogre.ColourValue(1.0,1.0,0.0))) #1 colour 
#         colours.append(rs.convertColourValue(ogre.ColourValue(0.0,1.0,0.0))) #2 colour 
#         colours.append(rs.convertColourValue(ogre.ColourValue(0.0,0.0,0.0))) #3 colour 
#         colours.append(rs.convertColourValue(ogre.ColourValue(1.0,0.0,1.0))) #4 colour 
#         colours.append(rs.convertColourValue(ogre.ColourValue(1.0,1.0,1.0))) #5 colour 
#         colours.append(rs.convertColourValue(ogre.ColourValue(0.0,1.0,1.0))) #6 colour 
#         colours.append(rs.convertColourValue(ogre.ColourValue(0.0,0.0,1.0))) #7 colour 
        
        mm = ManuallyDefinedMesh("cubeMesh",vert_pos,vert_norm,faces ) #,colours) 
        print mm
        mm.Print()
        
      
        
        self.cubeEntity = sceneManager.createEntity("myCubeEntity", "cubeMesh")        
        self.cubeEntity.setMaterialName("Test/SimpleVertexColor") 
        self.nd=sceneManager.rootSceneNode.createChildSceneNode() 
        self.nd.attachObject(self.cubeEntity) 
    
                        
if __name__ == '__main__':
    ta = TutorialApplication() 
    ta.go() 

        