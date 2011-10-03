#cython: boundscheck=False
#
# cython model file to improve watermesh speed
#
# python BuildCython.py build_ext --inplace
#
import ctypes
# we need sqrt so need to tell cython about it
cdef extern from "math.h":
    double sqrt ( double )
                
cdef enum LockOptionsDef:  ## these are from OgrehardwareBuffer.h
    HBL_NORMAL, HBL_DISCARD, HBL_READ_ONLY,  HBL_NO_OVERWRITE
 
def calcNormalsPrep ( self, ANIMATIONS_PER_SECOND_in ):
    cdef int comp
    cdef int y,x, row, row1, row1up, row1down, _inc, row2, bufnum, address
    cdef float C,D,U,T,TERM1,TERM2,TERM3, timeStamp, lastAnimationTimeStamp
    cdef int ANIMATIONS_PER_SECOND = ANIMATIONS_PER_SECOND_in
    comp = self.complexity
    timeStamp = self.lastTimeStamp
    lastAnimationTimeStamp = self.lastAnimationTimeStamp
  
    # and now the same into the float array buffer
    cdef float * buf
    cdef float * buf1
    cdef float * buf2
    
#       /* we use an algorithm from
#        * http:##collective.valve-erc.com/index.php?go=water_simulation
#        * The params could be dynamically changed every frame of course
#        */
    C = self.PARAM_C  ## ripple speed
    D = self.PARAM_D  ## distance
    U = self.PARAM_U  ## viscosity
    T = self.PARAM_T  ## time
    TERM1 = ( 4.0 - 8.0*C*C*T*T/(D*D) ) / (U*T+2)  
    TERM2 = ( U*T-2.0 ) / (U*T+2.0)  
    TERM3 = ( 2.0 * C*C*T*T/(D*D) ) / (U*T+2)  
    while(lastAnimationTimeStamp <= timeStamp):

        ## switch buffer numbers
        self.currentBufNumber = (self.currentBufNumber + 1) % 3  
        
        bufnum = self.currentBufNumber
        
        address=self.vertexBuffers[bufnum].buffer_info()[0]
        buf = <float *> address + 1
        address=self.vertexBuffers[((bufnum+2)%3)].buffer_info()[0]
        buf1 = <float *> address + 1
        address=self.vertexBuffers[((bufnum+1)%3)].buffer_info()[0]
        buf2 = <float *> address + 1

        for y in xrange(1,comp) : ## don't do anything with border values
            row = 3*y*(comp+1) 
            row1 = row
            row1up = 3*(y-1)*(comp+1)
            row1down = 3*(y+1)*(comp+1) 
            row2 = row 
            for x in xrange (1,comp) :
                _inc = 3*x
                buf[row+(_inc)] =\
                    TERM1 * buf1[row1 + _inc ] +\
                    TERM2 * buf2[row2 + _inc ] +\
                    TERM3 * ( buf1[row1 + _inc -3] +\
                                buf1[row1 + _inc +3] +\
                                buf1[row1up + _inc] +\
                                buf1[row1down + _inc]
                                )
        lastAnimationTimeStamp += (1.0 / ANIMATIONS_PER_SECOND) 
    self.lastAnimationTimeStamp = lastAnimationTimeStamp        
                
def calcNormalsFast ( numFaces, vinds_in,  buf_in, vNormals_in, comp, pNormalsAddress_in, vertices ) :
    cdef int loopCount, count,  temp, i0, i1, i2,
    cdef int v0[3], v1[3], v2[3]
    cdef double d1, d2, d3,d4,d5,d6,d7,d8,d9
    cdef double diff1[3], diff2[3], fn[3]
    cdef int x, y, complexity
    cdef int v, numPoint, numVertices
    cdef double n[3], fLength, fInvLength
    
    numVertices = vertices
    
    # here is how we take our inbound buffer and setup a c pointer to the raw data
    cdef unsigned short * vinds
    cdef int address
    address = vinds_in.buffAddress # this was the return from the original buffer lock
    vinds = <unsigned short *>address
    
    # and now the same into the float array buffer
    cdef float * buf
    address=buf_in.buffer_info()[0]
    buf = <float *> address
    
    # and again
    cdef float * vNormals
    address=vNormals_in.buffer_info()[0]
    vNormals = <float *> address
    
    # and again
    cdef float * pNormalsAddress
    address=ctypes.addressof(pNormalsAddress_in)
    pNormalsAddress = <float *> address

    # now zero the buffer
    for i in range(numVertices*3):
            vNormals[i]= 0

    loopCount = numFaces
    
    for count in range(loopCount) :
            # as we are using a flat array for vector3's we need to multiply by 3
        if True:
            temp = 3 * count
            i0 = 3*vinds[temp]
            i1 = 3*vinds[temp+1]
            i2 = 3*vinds[temp+2]
        
            # Do the vector subtraction by 'hand' instead of original
            #	Vector3 diff1 = v2 - v1 ;#
            #	Vector3 diff2 = v0 - v1 ;
            d1 = buf[i2]
            d2 = buf[i1]
            diff1[0]=d1-d2
            d3= buf[i2+1]
            d4 =buf[i1+1]
            diff1[1]=d3-d4
            d5=buf[i2+2]
            d6=buf[i1+2]
            diff1[2]=d5-d6
            d7=buf[i0]
            diff2[0]=d7-d2
            d8=buf[i0+1]
            diff2[1]=d8-d4
            d9=buf[i0+2]
            diff2[2]=d9-d6

        
        # And of course now add the values into the normals
#		Vector3 fn = diff1.crossProduct(diff2);
            fn[0] = diff1[1] * diff2[2] - diff1[2] * diff2[1]
            fn[1] = diff1[2] * diff2[0] - diff1[0] * diff2[2]
            fn[2] = diff1[0] * diff2[1] - diff1[1] * diff2[0]
            vNormals[i0] += fn[0] 
            vNormals[i0+1] += fn[1] 
            vNormals[i0+2] += fn[2] 
            vNormals[i1] += fn[0] 
            vNormals[i1+1] += fn[1] 
            vNormals[i1+2] += fn[2] 
            vNormals[i2] += fn[0] 
            vNormals[i2+1] += fn[1] 
            vNormals[i2+2] += fn[2] 
   
    # now for 2nd part...
    complexity = comp

    for y in range(complexity) :
        for x in range(complexity) :
            numPoint = y*(complexity+1) + x  
            v = 3*numPoint
            n[0] = vNormals[v]
            n[1] = vNormals[v+1]
            n[2] = vNormals[v+2]  
            fLength = sqrt( n[0] * n[0] + n[1] * n[1] + n[2] * n[2] )
            # Will also work for zero-sized vectors, but will change nothing
            if ( fLength > 1e-08 ):
                fInvLength = 1.0 / fLength
                n[0] *= fInvLength
                n[1] *= fInvLength
                n[2] *= fInvLength
                
            v = 3*numPoint
            pNormalsAddress [v] = n[0]
            pNormalsAddress [v+1] = n[1]
            pNormalsAddress [v+2] = n[2]
         
            