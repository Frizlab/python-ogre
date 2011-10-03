import os,sys, ctypes
if sys.platform == 'win32': 
    os.environ['PATH'] += ';' + __file__[0] 

    
    
import ogre.renderer.OGRE as ogre
import SampleFramework 
import math 

##
# OK so this doesn't work !!! The problem is that we can't override "size" in OgreDataStream as it isn't virtual
##
class FilePtr ( ogre.DataStream ):

    def __init__ ( self, filename ):
        ogre.DataStream.__init__(self)
        datain = file(filename, 'r').read() # should put error checking etc here
        self.length = len ( datain )
        self.source = ctypes.create_string_buffer( datain ) ## Note it allocates one extra byte
        
    def read ( self, dest, size ):
        if size <= self.length:
            print ogre.CastInt(dest)
            print "DEST:", dest
#             ctypes.memmove ( ogre.CastInt(dest), self.source, size ) # again should check here for 
#             print self.source.raw
            
            
        return size   
                
    def size ( self ):
        return self.length
        
class TutorialApplication(SampleFramework.Application): 


    def _createScene(self): 
        sm = self.sceneManager 
        
        ## a couple of tests to shown memory data stream usage..
        
        ## This is one way to use ctypes
        f= file("test.material", 'r')
        MatString = f.read()
        f.close()
        RawMemBuffer = ctypes.create_string_buffer( MatString  ) ## Note it allocates one extra byte
        ## Now we create the MemoryDataStream using the void pointer to the ctypes buffer
        dataptr = ogre.MemoryDataStream ( pMem = ogre.CastVoidPtr(ctypes.addressof ( RawMemBuffer )), 
                                                                                size = len (MatString) + 1 )
        ## Now lets get the string and print it...                                                                                
        p = dataptr.getAsString()
        print len(p)
        print p
        
        ## if you want you can confirm this 
        print dataptr
        print type(dataptr)
        #sys.exit()
        ogre.MaterialManager.getSingleton().parseScript( dataptr, "General" )   
        print "MATERIAL OK using Ctypes" 
        
        ## test iterator
        print "Using special iterator"
        for d in dataptr.getPtr():
            print chr(d),
                
        ## This is one way to use helper functions -- first with a list
        f= file("test.material", 'r')
        MatString = f.read()
        f.close()
        buf = []
        for c in MatString:
            buf.append( ord ( c ) )
        memDataStream = ogre.MemoryDataStream ( "MyBuffer", len (buf) )
        memDataStream.setData ( buf )
        p = memDataStream.getAsString()
        print len(p)
        print p
        print "MemoryBuffer OK using helper functions with a list" 
        
        f= file("test.material", 'r')
        MatString = f.read()
        f.close()
        memDataStream = ogre.MemoryDataStream ( "MyBuffer", len (MatString) )
        memDataStream.setData ( MatString )
        p = memDataStream.getAsString()
        print len(p)
        print p
        print "MemoryBuffer OK using helper functions with a string" 
        
        
# # #         fp = FilePtr ( "test.material")
# # #         dataptr = ogre.MemoryDataStream ( "Testing", sourceStream=fp )
# # #         p = dataptr.getDataPointer()
# # #         print "DataPointer", p
# # # #         print ogre.CastInt ( p )
# # #         sys.exit()
# # #         p=dataptr.getAsString()
# # #         print len ( p )
# # #         print p
# # #         
   
        sys.exit()

if __name__ == '__main__': 
    ta = TutorialApplication() 
    ta.go() 
