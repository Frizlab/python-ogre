import ogre.addons.plib as plib
import ctypes

plib.netInit ()

sock = plib.netSocket () 
sock = plib.netSocket ()
if not sock.open ( False ):
    raise RuntimeError ("Unable to open a socket")
sock.setBlocking ( False )
if sock.connect ( "127.0.0.1", 4444 ) == -1:
    raise RuntimeError ("Unable to connect socket")
    
count = 0
print "Sending data."
while ( count < 10 ):
    msg = "Hello :" + str ( count )
    ret = sock.send ( msg, 0 )
    count +=1

sock.close ()
