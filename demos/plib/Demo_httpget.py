import ogre.addons.plib as plib

class HTTPClient (plib.netBufferChannel):

    def __init__( self,  host,  path ):
        plib.netBufferChannel.__init__(self)
        self.open ()
        self.connect ( host, 80 )
    
        s = "GET %s HTTP/1.1\r\n" % path 
        self.bufferSend ( s, len(s) ) 
        h =  "host: %s\r\n" % host 
        self.bufferSend ( h, len(h) ) ;
        c = "Connection: close\r\n\r\n" 
        self.bufferSend ( c, len(c) )

 
    def handleBufferRead (self, bufferin):
        print "in handlebufferread", bufferin
        s = bufferin.getData_str()
        print s
        bufferin.remove()

if __name__ == "__main__":
  plib.netInit ( ) 
  client = HTTPClient ( "localhost", "test" ) ## "plib.sourceforge.net", "/index.html" ) ;
  plib.netChannel.loop ( 0 )





