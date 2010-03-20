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
import sys
try:
   import psyco
   psyco.full()
except ImportError:
   sys.exit()

import ctypes as ctypes
import math
import ogre.renderer.OGRE as ogre
import array as array

PLANE_SIZE =3000.0

CIRCLES_MATERIAL ="Examples/Water/Circles"
class buffer( object ):
    def __init__ (self, buffer):
        self.buffer = buffer
        try: #aaume it's an index buffer
            self.type = buffer.getType()  # 16 or 32 bit indicies 
            self.numItems = buffer.getNumIndexes()
            self.itemSize = buffer.getIndexSize()  # size in bytes, 2 or 4
            self.buffType = 1
        except: # must be a vertex buffer
            self.numItems = buffer.getNumVertices()
            self.itemSize = buffer.getVertexSize()  # size in bytes, 2 or 4
            self.buffType = 0
        
    def lock (self, * args ) : # start, size, options):
        if len ( args ) != 1 and len (args ) != 3:
            print "ERROR IN ARG LIST", len (args), args
            raise ValueError 
            
        if len (args) == 3:    
            self.lock = self.buffer.lock(args[0], args[1], args[2] ) # this returns a void pointer
        else:
            self.lock = self.buffer.lock(args[0] ) # this returns a void pointer
        self.buffAddress = ogre.castAsInt ( self.lock ) # and we store it as an unsigned long so was can get to it 
        
        # and now we make a ctypes byte array that points to the index buffer
        if self.buffType == 1 : # index buffer so 'ints'
            if self.itemSize == 4:
                self.rawBuffer=(ctypes.c_uint32 * (self.numItems)).from_address(self.buffAddress)
            elif self.itemSize == 2:
                self.rawBuffer=(ctypes.c_ushort * (self.numItems)).from_address(self.buffAddress)
        else: # assume it's a vertex buffer with floats
            self.rawBuffer=(ctypes.c_float * (self.itemSize*self.numItems)).from_address(self.buffAddress)
            
        
    def unlock ( self ):
        self.buffer.unlock()
        
    def __getitem__ ( self, index ):
        return self.rawBuffer[index]
#        i = index * self.indexSize # need addres into ctypes array
##        r = self.rawBuffer
#        value = 0
#        offset = 0
#        if self.indexSize == 4:
#            value = (r[i] << 24 ) + (r[i+1] << 16)
#            offset = 2
#        value = value + (r[i+offset] << 8) + (r[i+offset+1])
#        return value
        
    
    def __setitem__ ( self, index, value ):
        self.rawBuffer[index] = value
#        offset = 0
#        if self.indexSize == 4: # 32 bit index -- needs 4 bytes
#            uhb = value >> 24
#            ulb = (value >> 16) & 255
#            ctypes.memset ( self.buffAddress + index , uhb, 1 )
#            ctypes.memset ( self.buffAddress + index + 1, ulb, 1 )
#            offset = 2    
            
#        lhb = (value >> 8) & 255 # high order byte
#        llb = value & 255
##        ctypes.memset ( self.buffAddress + index + offset, lhb, 1 )
#        ctypes.memset ( self.buffAddress + index + offset + 1 , llb, 1 )


ANIMATIONS_PER_SECOND = 100.0
class WaterMesh:
    def __init__ ( self, meshName, planeSize, complexity ):
        self.dirty = True # need to refo calcs
        self.meshName = meshName  
        self.complexity =  complexity  
        self.numFaces = 2 * complexity * complexity 
        self.numVertices = (complexity + 1) * (complexity + 1)  
        self.lastTimeStamp = 0  
        self.lastAnimationTimeStamp = 0 
        self.lastFrameTime = 0  
    
        ## initialize algorithm parameters
        self.PARAM_C = 0.3   ## ripple speed
        self.PARAM_D = 0.4   ## distance
        self.PARAM_U = 0.05   ## viscosity
        self.PARAM_T = 0.13   ## time
        self.useFakeNormals = False  
    
        ## allocate space for normal calculation
        self.vNormals=array.array('f')
        for x in range ( self.numVertices * 3 ):
            self.vNormals.append(0)
        
        ## create mesh and submesh
        self.mesh = ogre.MeshManager.getSingleton().createManual(meshName,
                ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        self.subMesh = self.mesh.createSubMesh() 
        self.subMesh.useSharedVertices=False 
    
        ## Vertex buffers
        ##self.subMesh.vertexData = ogre.VertexData() 
        self.subMesh.vertexData = ogre.createVertexData() 
        self.subMesh.vertexData.vertexStart = 0 
        self.subMesh.vertexData.vertexCount = self.numVertices 
    
        vdecl = self.subMesh.vertexData.vertexDeclaration 
        vbind = self.subMesh.vertexData.vertexBufferBinding 
    
    
        vdecl.addElement(0, 0, ogre.VertexElementType.VET_FLOAT3, ogre.VertexElementSemantic.VES_POSITION) 
        vdecl.addElement(1, 0, ogre.VertexElementType.VET_FLOAT3, ogre.VertexElementSemantic.VES_NORMAL) 
        vdecl.addElement(2, 0, ogre.VertexElementType.VET_FLOAT2, ogre.VertexElementSemantic.VES_TEXTURE_COORDINATES) 
    
        ## Prepare buffer for positions - todo: first attempt, slow
        self.posVertexBuffer = \
             ogre.HardwareBufferManager.getSingleton().createVertexBuffer(
                3 * ctypes.sizeof(ctypes.c_float),
                self.numVertices,
                ogre.HardwareBuffer.HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE) 
        vbind.setBinding(0, self.posVertexBuffer) 
    
        
        ## Prepare buffer for normals - write only
        self.normVertexBuffer = \
             ogre.HardwareBufferManager.getSingleton().createVertexBuffer(
                3*ctypes.sizeof(ctypes.c_float), 
                self.numVertices,
                ogre.HardwareBuffer.HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE) 
        vbind.setBinding(1, self.normVertexBuffer) 
    
        ## Prepare texture coords buffer - static one
        ## todo: optimize to write directly into buffer
        texcoordsBufData=array.array('f')
        for x in range (self.numVertices*2):
            texcoordsBufData.append(0)
        
        for y in range (complexity) :
            for x in range(complexity) :
                texcoordsBufData[2*(y*(complexity+1)+x)+0] = float(x) / complexity  
                texcoordsBufData[2*(y*(complexity+1)+x)+1] = 1.0 - (float(y) / (complexity))  
    
    
        texcoordsVertexBuffer = \
             ogre.HardwareBufferManager.getSingleton().createVertexBuffer(
                2*ctypes.sizeof(ctypes.c_float),
                self.numVertices,
                ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY) 
        texcoordsVertexBuffer.writeData(0,
            texcoordsVertexBuffer.getSizeInBytes(),
            texcoordsBufData.buffer_info()[0],
            True)  ## true?
        vbind.setBinding(2, texcoordsVertexBuffer) 
    
        ## Prepare buffer for indices
        self.indexBuffer = \
            ogre.HardwareBufferManager.getSingleton().createIndexBuffer(
                ogre.HardwareIndexBuffer.IT_16BIT,
                3*self.numFaces,
                ogre.HardwareBuffer.HBU_STATIC, True) 
               
        # using a helper object to handle the ctypes implement
        # Create a buffer object and then lock it
        faceVertexIndices = buffer ( self.indexBuffer )
        faceVertexIndices.lock (0, self.numFaces*3*2, ogre.HardwareBuffer.HBL_DISCARD) 
        
        for y in range (complexity) :
            for  x in range (complexity ) :
                twoface = (y*complexity+x)*2*3 # index into buffer
                p0 = y*(complexity+1) + x  
                p1 = y*(complexity+1) + x + 1  
                p2 = (y+1)*(complexity+1) + x  
                p3 = (y+1)*(complexity+1) + x + 1 
                # write a series of bytes
                faceVertexIndices [ twoface + 0]= p2
                faceVertexIndices [ twoface + 1]= p1
                faceVertexIndices [ twoface + 2]= p0
                faceVertexIndices [ twoface + 3]= p2
                faceVertexIndices [ twoface + 4]= p3
                faceVertexIndices [ twoface + 5]= p1
                
        self.indexBuffer.unlock() 
        ## Set index buffer for self submesh
        self.subMesh.indexData.indexBuffer = self.indexBuffer 
        self.subMesh.indexData.indexStart = 0 
        self.subMesh.indexData.indexCount = 3*self.numFaces 
    
    #  prepare vertex positions
    #  note - we use 3 vertex buffers, since algorighm uses two last phases
    #  to calculate the next one
    
    # we need 2 floats, and are going through the loop three times 
        #storageclass2 = ctypes.c_float * (self.numVertices *3 * 3 *2 )
        #self.vertexBuffers = storageclass2 (1.1)
        #
        # Change here to use an 'array' for the buffers
        self.vertexBuffers = []
        for x in range (3):
            self.vertexBuffers.append( array.array('f') )
            for y in range (self.numVertices *3):
                self.vertexBuffers[x].append(0)
        
        
        ## Note that in the C++ code self.vertexBuffers is a float * array[3], whcih indexes into float*numverticies*3
        ## we have made a single array bige nough to cope
        #self.vertexIndexSize = self.numVertices*3
        
        for b in range(3) :
            for y in range(complexity) :
                for x in range (complexity) :
                    numPoint = y*(complexity+1) + x  
                    VertexPos = 3 * numPoint
                    self.vertexBuffers[b][VertexPos + 0] = float(x) / float(complexity) * float(planeSize)
                    self.vertexBuffers[b][VertexPos + 1] = 0
                    self.vertexBuffers[b][VertexPos + 2] = float(y) / float(complexity) * float(planeSize) 
                        
        meshBounds = ogre.AxisAlignedBox(0,0,0, planeSize,0, planeSize) 
        self.mesh._setBounds(meshBounds) 
    
        self.currentBufNumber = 0  
        self.posVertexBuffer.writeData(0,
            self.posVertexBuffer.getSizeInBytes(), ## size
            self.vertexBuffers[0].buffer_info()[0], 
            True)  ## discard?
    
        self.mesh.load() 
        self.mesh.touch() 
    
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
        buf = self.currentBufNumber  
        ## scale pressure according to time passed
        depth = depth * self.lastFrameTime * ANIMATIONS_PER_SECOND  *.3
        self._PREP(0,0, buf,x,y,depth,absolute) 
        self._PREP(0,1, buf,x,y,depth,absolute) 
        self._PREP(1,0, buf,x,y,depth,absolute) 
        self._PREP(1,1, buf,x,y,depth,absolute)
        
    def _PREP(self, addx,addy, buf,x,y,depth,absolute) :
        vertex=3*((int)(y+addy)*(self.complexity+1)+(int)(x+addx)) +1 #AJM +1 ???? 
        diffy = y - math.floor(y+addy)
        diffx = x - math.floor(x+addx)
        dist=math.sqrt(diffy*diffy + diffx*diffx)
        power = 1.0 - dist
        if (power<0):
            power = 0
        if (absolute):
            self.vertexBuffers[buf][vertex] = depth*power
        else:
            self.vertexBuffers[buf][vertex] += depth*power
    
    # /* ========================================================================= */
    # gets height at given x and y, takes average value of the closes nodes */
    def hat(self, _x,_y, buf):
        return self.vertexBuffers[self.currentBufNumber][3*(int(_y)*(self.complexity+1)+(int(_x)))]

    def getHeight(self,  x, y):
        xa = math.floor(x) 
        xb = xa + 1
        ya = math.floor(y) 
        yb = ya + 1  
        yaxavg = hat(xa,ya) * (1.0-fabs(xa-x)) + hat(xb,ya) * (1.0-fabs(xb-x)) 
        ybxavg = hat(xa,yb) * (1.0-fabs(xa-x)) + hat(xb,yb) * (1.0-fabs(xb-x)) 
        yavg = yaxavg * (1.0-fabs(ya-y)) + ybxavg * (1.0-fabs(yb-y))  
        return yavg  
    
#     /* ========================================================================= */
    def calculateFakeNormals(self):
        buf=self.vertexBuffers[self.currentBufNumber]
        pNormals = self.normVertexBuffer.lock(
            0, self.normVertexBuffer.getSizeInBytes(), ogre.HardwareBuffer.HBL_DISCARD) 
        #storageclass1=ctypes.c_float * (self.normVertexBuffer.getSizeInBytes()*3)
        pNormalsAddress=(ctypes.c_float * (self.normVertexBuffer.getSizeInBytes()*3)).from_address(ogre.castAsInt(pNormals))
   
        for y in range (self.complexity) :
            start = 3*y*(self.complexity+1)
            nrow = pNormalsAddress[ start]
            row = buf[3 * y * (self.complexity+1) +1]
            rowup = buf [3*(y-1)*(self.complexity+1) +1]  
            rowdown = buf [ 3*(y+1)*(self.complexity+1) +1]  
            for x in range (self.complexity) :
                xdiff = row+ 3*x+3 - row +3*x-3  
                ydiff = rowup + 3*x - rowdown+3*x-3  
                norm = ogre.Vector3(xdiff,30,ydiff) 
                norm.normalise()
                
                pNormalsAddress[start+3*x+0] = norm.x 
                pNormalsAddress[start+3*x+1] = norm.y 
                pNormalsAddress[start+3*x+2] = norm.z 
    
        #ogre.setUint16 ( pNormals, buf )
        self.normVertexBuffer.unlock() 
    
#     /* ========================================================================= */
    def calculateNormals(self):
        ## zero normals
        for i in range(self.numVertices*3) :
            self.vNormals[i]=  0
            
        ## first, calculate normals for faces, add them to proper vertices
        
        # use helper function
        vinds = buffer ( self.indexBuffer)
        vinds.lock (0, self.indexBuffer.getSizeInBytes(), ogre.HardwareBuffer.HBL_READ_ONLY)
        
        pNormals = self.normVertexBuffer.lock(
            0, self.normVertexBuffer.getSizeInBytes(), ogre.HardwareBuffer.HBL_DISCARD) 
        pNormalsAddress=(ctypes.c_float * (self.normVertexBuffer.getSizeInBytes()*3)).from_address(ogre.castAsInt(pNormals))
       
        # make life easier (and faster) by using a local variables
        buf = self.vertexBuffers[self.currentBufNumber]
        vNormals = self.vNormals
        
        ## AJM so here's a case where accessing a C++ object from python shows a performance hit !!
        ## in this case we look through and the original C++ code creates and access ovre.Vector3 objects
        ## multiple times on each pass.
        for count in range(self.numFaces) :
            p0 = vinds[3*count]  
            p1 = vinds[3*count+1]  
            p2 = vinds[3*count+2]  
            # this is slow
            # v0= ogre.Vector3 (self.vertexBuffers[buf][3*p0], self.vertexBuffers[buf][3*p0+1], self.vertexBuffers[buf][3*p0+2]) 
            # v1 = ogre.Vector3 (self.vertexBuffers[buf][3*p1], self.vertexBuffers[buf][3*p1+1], self.vertexBuffers[buf][3*p1+2]) 
            # v2 = ogre.Vector3 (self.vertexBuffers[buf][3*p2], self.vertexBuffers[buf][3*p2+1], self.vertexBuffers[buf][3*p2+2]) 
            
            # so use python arrays instead of Vector3's
            i0 = 3*p0
            i1 = 3*p1
            i2 = 3*p2
            v0 = [buf[i0], buf[i0+1], buf[i0+2]]
            v1 = [buf[i1], buf[i1+1], buf[i1+2]] 
            v2 = [buf[i2], buf[i2+1], buf[i2+2]] 
            
            # Do the vector subtraction by 'hand' instead of original
            # diff2 = v0 - v1  
            diff1  = [v2[0]-v1[0],v2[1]-v2[1],v2[2]-v2[2]]  
            diff2  = [v0[0]-v1[0],v0[1]-v2[1],v0[2]-v2[2]]  
            
            # and now we need to do a crossProduct by hand..
            # fn = ogre.Vector3(*diff1).crossProduct(ogre.Vector3(*diff2)) 
            fn = [diff1[1] * diff2[2] - diff1[2] * diff2[1],
                diff1[2] * diff2[0] - diff1[0] * diff2[2],
                diff1[0] * diff2[1] - diff1[1] * diff2[0]]
            # And of course now add the values into the normals
            # self.vNormals[p0] += fn  
            # self.vNormals[p1] += fn  
            # self.vNormals[p2] += fn  
            vNormals[i0] += fn[0] 
            vNormals[i0+1] += fn[1] 
            vNormals[i0+2] += fn[2] 
            vNormals[i1] += fn[0] 
            vNormals[i1+1] += fn[1] 
            vNormals[i1+2] += fn[2] 
            vNormals[i2] += fn[0] 
            vNormals[i2+1] += fn[1] 
            vNormals[i2+2] += fn[2] 
    
        ## now normalize vertex normals
        complexity = self.complexity
        for y in range(complexity) :
            for x in range(complexity) :
                numPoint = y*(complexity+1) + x  
                v = 3*numPoint
                n = ogre.Vector3(vNormals[v],vNormals[v+1],vNormals[v+2])  
                n.normalise()  
                v = 3*numPoint
                pNormalsAddress [v] = n.x
                pNormalsAddress [v+1] = n.y
                pNormalsAddress [v+2] = n.z
    
        self.indexBuffer.unlock() 
        self.normVertexBuffer.unlock() 
        
#     /* ======================================================================== */
    def calcs ( self ):
        C = self.PARAM_C  ## ripple speed
        D = self.PARAM_D  ## distance
        U = self.PARAM_U  ## viscosity
        T = self.PARAM_T  ## time
        self.TERM1 = ( 4.0 - 8.0*C*C*T*T/(D*D) ) / (U*T+2)  
        self.TERM2 = ( U*T-2.0 ) / (U*T+2.0)  
        self.TERM3 = ( 2.0 * C*C*T*T/(D*D) ) / (U*T+2)  
        
    def updateMesh(self, timeSinceLastFrame):
        self.lastFrameTime = timeSinceLastFrame  
        self.lastTimeStamp += timeSinceLastFrame  
    
        ## do rendering to get ANIMATIONS_PER_SECOND
        while(self.lastAnimationTimeStamp <= self.lastTimeStamp):
    
            ## switch buffer numbers
            self.currentBufNumber = (self.currentBufNumber + 1) % 3  

            buf = self.vertexBuffers[self.currentBufNumber]
            buf1 = self.vertexBuffers[((self.currentBufNumber+2)%3)]  
            buf2 = self.vertexBuffers[((self.currentBufNumber+1)%3)]
    
    #       /* we use an algorithm from
    #        * http:##collective.valve-erc.com/index.php?go=water_simulation
    #        * The params could be dynamically changed every frame ofcourse
    #        */
            if False:  # we don't really need to do this every frame however it doesn't hurt
                if self.dirty:
                    self.calcs()
                    self.dirty = False
            else:
                C = self.PARAM_C  ## ripple speed
                D = self.PARAM_D  ## distance
                U = self.PARAM_U  ## viscosity
                T = self.PARAM_T  ## time
                TERM1 = ( 4.0 - 8.0*C*C*T*T/(D*D) ) / (U*T+2)  
                TERM2 = ( U*T-2.0 ) / (U*T+2.0)  
                TERM3 = ( 2.0 * C*C*T*T/(D*D) ) / (U*T+2)  
                
            for y in range(self.complexity) : ## don't do anything with border values
                row = 3*y*(self.complexity+1) + 1 
                row1 = row
                row1up = 3*(y-1)*(self.complexity+1) +1
                row1down = 3*(y+1)*(self.complexity+1) +1 
                row2 = row 
                for x in range (self.complexity) :
                    _inc = 3*x
                    buf[row+(_inc)] =\
                        TERM1 * buf1[row1 + _inc ] +\
                        TERM2 * buf2[row2 + _inc ] +\
                        TERM3 * ( buf1[row1 + _inc -3] +\
                                    buf1[row1 + _inc +3] +\
                                    buf1[row1up + _inc] +\
                                    buf1[row1down + _inc]
                                    )
                                    
                                    
            self.lastAnimationTimeStamp += (1.0 / ANIMATIONS_PER_SECOND) 
    
        if (self.useFakeNormals):
            self.calculateFakeNormals() 
        else :
            self.calculateNormals() 
    
        ## set vertex buffer
        self.posVertexBuffer.writeData(0,
            self.posVertexBuffer.getSizeInBytes(), ## size
            self.vertexBuffers[self.currentBufNumber].buffer_info()[0], ## source
            True)  ## discard?

#/* =========================================================================*/
#/*               WaterCircle class                                          */
#/* =========================================================================*/

class WaterCircle :
    CIRCLE_SIZE = 500.0
    CIRCLE_TIME = 0.5
    # some buffers shared by all circles
    _posnormVertexBuffer = None
    _indexBuffer = None
    _texcoordsVertexBuffers = []
    _first = True
    
    def __init__ ( self, name, x, y,sm,COMPLEXITY ):
        global PLANE_SIZE, CIRCLES_MATERIAL
        self.sceneManager = sm
        self.name = name
        self._prepareMesh ()
        self.node = sm.getRootSceneNode().createChild(name).castAsSceneNode()
        self.node.translate(x*(PLANE_SIZE/COMPLEXITY), 10, y*(PLANE_SIZE/COMPLEXITY))
        self.entity = sm.createEntity(name, name)
        self.entity.setMaterialName(CIRCLES_MATERIAL)
        self.node.attachObject(self.entity)
        self.tm = 0
        self.lvl = 0
        self.setTextureLevel()
		
    
    def _prepareMesh(self):
        
        self.mesh = ogre.MeshManager.getSingleton().createManual(self.name,
            ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        self.subMesh = self.mesh.createSubMesh()
        self.subMesh.useSharedVertices=False

        numVertices = 4

        if (WaterCircle._first) : # first Circle, create some static common data
            WaterCircle._first = False

            #static buffer for position and normals
            WaterCircle._posnormVertexBuffer =\
                ogre.HardwareBufferManager.getSingleton().createVertexBuffer(
                6*ctypes.sizeof(ctypes.c_float), #size of one vertex data
                4, # number of vertices
                ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, # usage
                False) # no shadow buffer
                    
            posnormBufData = buffer ( WaterCircle._posnormVertexBuffer )
            print "!!!"
            posnormBufData.lock(ogre.HardwareBuffer.HBL_DISCARD)
            
            for i in range (numVertices):
                posnormBufData[6*i+0]=(float(i%2)-0.5)*WaterCircle.CIRCLE_SIZE; # pos X
                posnormBufData[6*i+1]=0; # pos Y
                posnormBufData[6*i+2]=(float(i/2)-0.5)*WaterCircle.CIRCLE_SIZE; # pos Z
                posnormBufData[6*i+3]=0 ; # normal X
                posnormBufData[6*i+4]=1 ; # normal Y
                posnormBufData[6*i+5]=0 ; # normal Z
                
            posnormBufData.unlock()

            #static buffers for 16 sets of texture coordinates
            for lvl in range ( 16 ):
                WaterCircle._texcoordsVertexBuffers.append(
                    ogre.HardwareBufferManager.getSingleton().createVertexBuffer(
                    2*ctypes.sizeof(ctypes.c_float), # size of one vertex data
                    numVertices, # number of vertices
                    ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, # usage
                    False)) # no shadow buffer
                texcoordsBufData = buffer( WaterCircle._texcoordsVertexBuffers[lvl])
                print "2222"
                texcoordsBufData.lock(ogre.HardwareBuffer.HBL_DISCARD)
                x0 = float(lvl % 4) * 0.25 
                y0 = float(lvl / 4) * 0.25
                y0 = 0.75-y0  # upside down
                for i in range (4):
                    texcoordsBufData[i*2 + 0]=x0 + 0.25 * float(i%2)
                    texcoordsBufData[i*2 + 1]=y0 + 0.25 * float(i/2)
                texcoordsBufData.unlock()

#			Index buffer for 2 faces
            faces = array.array('H',[2,1,0,2,3,1]) # unsigned short array
            WaterCircle._indexBuffer =\
				ogre.HardwareBufferManager.getSingleton().createIndexBuffer(
					ogre.HardwareIndexBuffer.IT_16BIT,
					6,
					ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY)
            WaterCircle._indexBuffer.writeData(0,
				WaterCircle._indexBuffer.getSizeInBytes(),
				faces.buffer_info()[0],
				True); # true?

#		Initialize vertex data
        self.subMesh.vertexData = ogre.createVertexData()
        self.subMesh.vertexData.vertexStart = 0
        self.subMesh.vertexData.vertexCount = 4
        #first, set vertex buffer bindings
        vbind = self.subMesh.vertexData.vertexBufferBinding 
        vbind.setBinding(0, WaterCircle._posnormVertexBuffer)
        vbind.setBinding(1, WaterCircle._texcoordsVertexBuffers[0])
        #now, set vertex buffer declaration
        vdecl = self.subMesh.vertexData.vertexDeclaration
        vdecl.addElement(0, 0, ogre.VertexElementType.VET_FLOAT3, ogre.VertexElementSemantic.VES_POSITION);
        vdecl.addElement(0, 3*ctypes.sizeof(ctypes.c_float), ogre.VertexElementType.VET_FLOAT3, ogre.VertexElementSemantic.VES_NORMAL);
        vdecl.addElement(1, 0, ogre.VertexElementType.VET_FLOAT2, ogre.VertexElementSemantic.VES_TEXTURE_COORDINATES);

        #		Initialize index data
        self.subMesh.indexData.indexBuffer = WaterCircle._indexBuffer
        self.subMesh.indexData.indexStart = 0
        self.subMesh.indexData.indexCount = 6

        #		set mesh bounds
        circleBounds = ogre.AxisAlignedBox (-WaterCircle.CIRCLE_SIZE/2.0, 0, -WaterCircle.CIRCLE_SIZE/2.0,
            WaterCircle.CIRCLE_SIZE/2.0, 0, WaterCircle.CIRCLE_SIZE/2.0)
        self.mesh._setBounds(circleBounds)
        self.mesh.load()
        self.mesh.touch()

    def setTextureLevel(self):
        self.subMesh.vertexData.vertexBufferBinding.setBinding(1, WaterCircle._texcoordsVertexBuffers[self.lvl])
        
    def __del__ ( self ):   
        ogre.MeshManager.getSingleton().remove(self.mesh.getHandle())
        self.sceneManager.destroyEntity(self.entity.getName())
        self.sceneManager.getRootSceneNode().castAsSceneNode().removeChild(self.node.getName())

    def animate(self, timeSinceLastFrame):
        lastlvl = self.lvl
        self.tm += timeSinceLastFrame
        self.lvl = int( float(self.tm)/WaterCircle.CIRCLE_TIME * 16 )
        if (self.lvl<16 and self.lvl!=lastlvl):
            self.setTextureLevel()

