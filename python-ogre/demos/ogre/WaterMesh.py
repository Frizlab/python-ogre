# /*
# -----------------------------------------------------------------------------
# This source file is part of OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http:##www.ogre3d.org/

# Copyright (c) 2000-2006 Torus Knot Software Ltd
# Also see acknowledgements in Readme.html

# You may use self sample code for anything you like, it is not covered by the
# LGPL like the rest of the engine.
# -----------------------------------------------------------------------------
# */

import ctypes as ctypes
import math
import ogre.renderer.OGRE as Ogre
import random

ANIMATIONS_PER_SECOND = 100.0
class WaterMesh:
    def __init__ ( self, meshName, planeSize, complexity ):
        self.meshName = meshName  
        self.complexity =  complexity  
        self.numFaces = 2 * complexity * complexity 
        self.numVertices = (complexity + 1) * (complexity + 1)  
        self.lastTimeStamp = 0  
        self.lastAnimationTimeStamp = 0 
        self.lastFrameTime = 0  
        self.vNormals=[]
    
        ## initialize algorithm parameters
        self.PARAM_C = 0.3   ## ripple speed
        self.PARAM_D = 0.4   ## distance
        self.PARAM_U = 0.05   ## viscosity
        self.PARAM_T = 0.13   ## time
        self.useFakeNormals = False  
    
        ## allocate space for normal calculation
        self.vNormals=[]
        for x in range ( self.numVertices ):
            self.vNormals.append(Ogre.Vector3() )
    
        ## create mesh and submesh
        mesh = Ogre.MeshManager.getSingleton().createManual(meshName,
                Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        subMesh = mesh.createSubMesh() 
        subMesh.useSharedVertices=False 
    
        ## Vertex buffers
        subMesh.vertexData = Ogre.VertexData() 
        subMesh.vertexData.vertexStart = 0 
        subMesh.vertexData.vertexCount = self.numVertices 
    
        vdecl = subMesh.vertexData.vertexDeclaration 
        vbind = subMesh.vertexData.vertexBufferBinding 
    
    
        vdecl.addElement(0, 0, Ogre.VertexElementType.VET_FLOAT3, Ogre.VertexElementSemantic.VES_POSITION) 
        vdecl.addElement(1, 0, Ogre.VertexElementType.VET_FLOAT3, Ogre.VertexElementSemantic.VES_NORMAL) 
        vdecl.addElement(2, 0, Ogre.VertexElementType.VET_FLOAT2, Ogre.VertexElementSemantic.VES_TEXTURE_COORDINATES) 
    
        ## Prepare buffer for positions - todo: first attempt, slow
        self.posVertexBuffer = \
             Ogre.HardwareBufferManager.getSingleton().createVertexBuffer(
                3 * ctypes.sizeof(ctypes.c_float),
                self.numVertices,
                Ogre.HardwareBuffer.HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE) 
        vbind.setBinding(0, self.posVertexBuffer) 
    
        ## Prepare buffer for normals - write only
        self.normVertexBuffer = \
             Ogre.HardwareBufferManager.getSingleton().createVertexBuffer(
                3*ctypes.sizeof(ctypes.c_float), 
                self.numVertices,
                Ogre.HardwareBuffer.HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE) 
        vbind.setBinding(1, self.normVertexBuffer) 
    
        ## Prepare texture coords buffer - static one
        ## todo: optimize to write directly into buffer
        storageclass = ctypes.c_float * (self.numVertices*2)
        texcoordsBufData=storageclass(1.1)
        
        for y in range (complexity) :
            for x in range(complexity) :
                texcoordsBufData[2*(y*(complexity+1)+x)+0] = x / complexity  
                texcoordsBufData[2*(y*(complexity+1)+x)+1] = 1.0 - (y / (complexity))  
    
    
        texcoordsVertexBuffer = \
             Ogre.HardwareBufferManager.getSingleton().createVertexBuffer(
                2*ctypes.sizeof(ctypes.c_float),
                self.numVertices,
                Ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY) 
        texcoordsVertexBuffer.writeData(0,
            texcoordsVertexBuffer.getSizeInBytes(),
            ctypes.addressof(texcoordsBufData),
            True)  ## true?
        vbind.setBinding(2, texcoordsVertexBuffer) 
    
        ## Prepare buffer for indices
        self.indexBuffer = \
            Ogre.HardwareBufferManager.getSingleton().createIndexBuffer(
                Ogre.HardwareIndexBuffer.IT_16BIT,
                3*self.numFaces,
                Ogre.HardwareBuffer.HBU_STATIC, True) 
                
        ## get the address of the self.indexBuffer
        faceVertexIndices = self.indexBuffer.lock(0, self.numFaces*3*2, Ogre.HardwareBuffer.HBL_DISCARD) 
        faceVertexIndices = Ogre.CastInt ( faceVertexIndices )
        for y in range (complexity) :
            for  x in range (complexity ) :
                twoface = faceVertexIndices + (y*complexity+x)*2*3 
                p0 = y*(complexity+1) + x  
                p1 = y*(complexity+1) + x + 1  
                p2 = (y+1)*(complexity+1) + x  
                p3 = (y+1)*(complexity+1) + x + 1 
                ctypes.memset ( twoface + 0, p2, 1 )
                ctypes.memset ( twoface + 1, p1, 1 )
                ctypes.memset ( twoface + 2, p0, 1 )
                ctypes.memset ( twoface + 3, p2, 1 )
                ctypes.memset ( twoface + 4, p3, 1 )
                ctypes.memset ( twoface + 5, p1, 1 )
        self.indexBuffer.unlock() 
        ## Set index buffer for self submesh
        subMesh.indexData.indexBuffer = self.indexBuffer 
        subMesh.indexData.indexStart = 0 
        subMesh.indexData.indexCount = 3*self.numFaces 
    
    #  prepare vertex positions
    #  note - we use 3 vertex buffers, since algorighm uses two last phases
    #  to calculate the next one
        storageclass2 = ctypes.c_float * (self.numVertices*3* 3 )
        self.vertexBuffers = storageclass2 (1.1)
        
        
        ## Note that in the C++ code self.vertexBuffers is a float * array[3], whcih indexes into float*numverticies*3
        ## we have made a single array bige nough to cope
        self.vertexIndexSize = self.numVertices*3
        
        for b in range(3) :
            for y in range(complexity) :
                for x in range (complexity) :
                    numPoint = y*(complexity+1) + x  
# #                     vertex = self.vertexBuffers[b] + 3.0*numPoint  
                    VertexPos = (self.vertexIndexSize * b) + 3 * numPoint
                    self.vertexBuffers[VertexPos + 0] = x / (complexity * planeSize  )
                    self.vertexBuffers[VertexPos + 1] = 0
                    self.vertexBuffers[VertexPos + 2] = y / (complexity * planeSize  )
                        
        meshBounds = Ogre.AxisAlignedBox(0,0,0, planeSize,0, planeSize) 
        mesh._setBounds(meshBounds) 
    
        self.currentBufNumber = 0  
        self.posVertexBuffer.writeData(0,
            self.posVertexBuffer.getSizeInBytes(), ## size
            ctypes.addressof(self.vertexBuffers) + self.currentBufNumber*self.vertexIndexSize, ## source
            True)  ## discard?
    
        mesh.load() 
        mesh.touch() 
    
    #  ========================================================================= 
    def __del__ (self):
        pass
            
    # ===========================================================================
#       /** "pushes" a mesh at position [x,y]. Note, that x,y are float, hence 
#   *   4 vertices are actually pushed
#   *   @note 
#   *       This should be replaced by push with 'radius' parameter to simulate
#   *       big objects falling into water
#   */

    def push( self, x, y, depth, absolute=False):
        buf = self.vertexBuffers[self.currentBufNumber*self.vertexIndexSize]+1  
        ## scale pressure according to time passed
        depth = depth * self.lastFrameTime * ANIMATIONS_PER_SECOND  
        self._PREP(0,0) 
        self._PREP(0,1) 
        self._PREP(1,0) 
        self._PREP(1,1)
        
    def _PREP(self, addx,addy) :
        return
#         *vertex=buf+3*((int)(y+addy)*(complexity+1)+(int)(x+addx)) 
#         diffy = y - floor(y+addy)
#         diffx = x - floor(x+addx)
#         dist=sqrt(diffy*diffy + diffx*diffx)
#         power = 1 - dist
#         if (power<0):
#             power = 0
#         if (absolute):
#             *vertex = depth*power
#         else:
#             *vertex += depth*power
    
    def hat(self, _x,_y):
        self.buf[3*(_y*(complexity+1)+(_x))]
         
    
    # /* ========================================================================= */
    # gets height at given x and y, takes average value of the closes nodes */

    def getHeight(self,  x, y):
    
        self.buf = self.vertexBuffers[self.currentBufNumber*self.vertexIndexSize]  
        xa = floor(x) 
        xb = xa + 1  
        ya = floor(y) 
        yb = ya + 1  
        yaxavg = hat(xa,ya) * (1.0-fabs(xa-x)) + hat(xb,ya) * (1.0-fabs(xb-x)) 
        ybxavg = hat(xa,yb) * (1.0-fabs(xa-x)) + hat(xb,yb) * (1.0-fabs(xb-x)) 
        yavg = yaxavg * (1.0-fabs(ya-y)) + ybxavg * (1.0-fabs(yb-y))  
        return yavg  
    
#     /* ========================================================================= */
    def calculateFakeNormals(self):
        return ## AJM
        buf = self.currentBufNumber*self.vertexIndexSize + 1 
        pNormals = self.normVertexBuffer.lock(
            0,self.normVertexBuffer.getSizeInBytes(), Ogre.HardwareBuffer.HBL_DISCARD) 
        for y in range (self.complexity) :
# # #             nrow = pNormals + 3*y*(complexity+1) 
            row = buf + 3 * y * (self.complexity+1)  
            rowup = buf + 3*(y-1)*(self.complexity+1)  
            rowdown = buf + 3*(y+1)*(self.complexity+1)  
            for x in range (self.complexity) :
                xdiff = row+ 3*x+3 - row +3*x-3  
                ydiff = rowup + 3*x - rowdown+3*x-3  
                norm = Ogre.Vector3(xdiff,30,ydiff) 
                norm.normalise()
                buf.append( norm.x ) 
                buf.append( norm.y ) 
                buf.append( norm.z ) 
# # #                 nrow[3*x+0] = norm.x 
# # #                 nrow[3*x+1] = norm.y 
# # #                 nrow[3*x+2] = norm.z 
    
        Ogre.setUint16 ( pNormals, buf )
        self.normVertexBuffer.unlock() 
    
#     /* ========================================================================= */
    def calculateNormals(self):
        buf = self.vertexBuffers[self.currentBufNumber*self.vertexIndexSize] + 1 
        ## zero normals
        
        for i in range(self.numVertices) :
            self.vNormals.append( Ogre.Vector3().ZERO_Copy )
        return ## AJM
        ## first, calculate normals for faces, add them to proper vertices
        buf = self.vertexBuffers[self.currentBufNumber*self.vertexIndexSize]  
        vinds = self.indexBuffer.lock(
            0, self.indexBuffer.getSizeInBytes(), Ogre.HardwareBuffer.HBL_READ_ONLY) 
        pNormals = self.normVertexBuffer.lock(
            0, self.normVertexBuffer.getSizeInBytes(), Ogre.HardwareBuffer.HBL_DISCARD) 
        for i in range(self.numFaces) :
            p0 = vinds[3*i]  
            p1 = vinds[3*i+1]  
            p2 = vinds[3*i+2]  
            v0=Ogre.Vector3 (buf[3*p0], buf[3*p0+1], buf[3*p0+2]) 
            v1 = Ogre.Vector3 (buf[3*p1], buf[3*p1+1], buf[3*p1+2]) 
            v3 = Ogre.Vector3 (buf[3*p2], buf[3*p2+1], buf[3*p2+2]) 
            diff1  = v2 - v1  
            diff2 = v0 - v1  
            fn = diff1.crossProduct(diff2) 
            self.vNormals[p0] += fn  
            self.vNormals[p1] += fn  
            self.vNormals[p2] += fn  
    
        ## now normalize vertex normals
        for y in (self.complexity) :
            for x in (self.complexity) :
                numPoint = y*(self.complexity+1) + x  
                n = self.vNormals[numPoint]  
                n.normalise()  
                normal = pNormals + 3*numPoint  
                normal[0]=n.x 
                normal[1]=n.y 
                normal[2]=n.z 
    
    
        self.indexBuffer.unlock() 
        self.normVertexBuffer.unlock() 
    
#     /* ======================================================================== */
    def updateMesh(self, timeSinceLastFrame):
        self.lastFrameTime = timeSinceLastFrame  
        self.lastTimeStamp += timeSinceLastFrame  
    
        ## do rendering to get ANIMATIONS_PER_SECOND
        while(self.lastAnimationTimeStamp <= self.lastTimeStamp):
    
            ## switch buffer numbers
            self.currentBufNumber = (self.currentBufNumber + 1) % 3  
            
#             buf = self.vertexBuffers[self.currentBufNumber*self.vertexIndexSize] + 1   ## +1 for Y coordinate

            buf = self.currentBufNumber*self.vertexIndexSize + 1
            buf1 = (((self.currentBufNumber+2)%3) * self.vertexIndexSize) + 1  
            buf2 = (((self.currentBufNumber+1)%3) * self.vertexIndexSize) + 1 
    
    #       /* we use an algorithm from
    #        * http:##collective.valve-erc.com/index.php?go=water_simulation
    #        * The params could be dynamically changed every frame ofcourse
    #        */
            C = self.PARAM_C  ## ripple speed
            D = self.PARAM_D  ## distance
            U = self.PARAM_U  ## viscosity
            T = self.PARAM_T  ## time
            TERM1 = ( 4.0 - 8.0*C*C*T*T/(D*D) ) / (U*T+2)  
            TERM2 = ( U*T-2.0 ) / (U*T+2.0)  
            TERM3 = ( 2.0 * C*C*T*T/(D*D) ) / (U*T+2)  
            for y in range(self.complexity) : ## don't do anything with border values
                row = buf + 3*y*(self.complexity+1)  
                row1 = buf1 + 3*y*(self.complexity+1)  
                row1up = buf1 + 3*(y-1)*(self.complexity+1)  
                row1down = buf1 + 3*(y+1)*(self.complexity+1)  
                row2 = buf2 + 3*y*(self.complexity+1)  
                for x in range (self.complexity) :
                    self.vertexBuffers[row+(3*x)] =\
                        TERM1 * self.vertexBuffers[row1 + 3*x ] +\
                        TERM2 * self.vertexBuffers[row2 + 3*x ] +\
                        TERM3 * ( self.vertexBuffers[row1 + 3*x -3] +\
                                    self.vertexBuffers[row1 + 3*x +3] +\
                                    self.vertexBuffers[row1up + 3*x] +\
                                    self.vertexBuffers[row1down + 3*x]
                                    )
            self.lastAnimationTimeStamp += (1.0 / ANIMATIONS_PER_SECOND) 
    
        if (self.useFakeNormals):
            self.calculateFakeNormals() 
        else :
            self.calculateNormals() 
    
        ## set vertex buffer
        self.posVertexBuffer.writeData(0,
            self.posVertexBuffer.getSizeInBytes(), ## size
            ctypes.addressof (self.vertexBuffers) + self.currentBufNumber*self.vertexIndexSize, ## source
            True)  ## discard?

