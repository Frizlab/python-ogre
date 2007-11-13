import ogre.renderer.OGRE as ogre
import ctypes
import random


class ds (ogre.DataStream):
    def __init__ ( self, width, height, depth, format ):
        ogre.DataStream.__init__(self)
        
        ## Note that this is for testing to ensure we get the right size buffer
        self._size= ogre.PixelUtil.getMemorySize(width, height, depth, format )
        self.buffer=[]
        self.position = 0
        for i in range ( self._size ):
            self.buffer.append(random.randint ( 0, 255 ))

    def tell ( self ):
        print "**tell", self.position
        return self.position
                              
    def seek ( self, pos ):
        print "**seek", pos
        if pos < self._size and pos >= 0:
            self.position = pos
    
    def size ( self ):
        print "**size", self._size
        return self._size
                  
    def skip ( self, pos ):
        print "**skip", pos
        temppos = self.position + pos
        if temppos >=0 and temppos < self._size:
            self.position = temppos

    def eof ( self ):
        print "**eof"
        if self.position == self._size - 1:
            return True
        return False
        
    def read ( self, dest, count ):
        print "**read", count
        if count <= 0 : return
        pointer = ctypes.c_void_p ( dest )
        
        for x in range ( count ):
            pointer[x] = self.buffer[self.position]
            if self.position == self._size-1:
                break
            self.position += 1
        
                          

width = 240
height = 240
depth = 8
format = ogre.PixelFormat.PF_BYTE_BGR
requiredSize = ogre.PixelUtil.getMemorySize(width, height, depth, format )
mDs = ogre.MemoryDataStream (  requiredSize )
pointer = mDs.getPtr ()
for i in range ( requiredSize):
    pointer[i] = random.randint ( 0, 255 )
img=ogre.Image()
img.loadRawData(mDs, 240, 240, 8, ogre.PixelFormat.PF_BYTE_BGR)

