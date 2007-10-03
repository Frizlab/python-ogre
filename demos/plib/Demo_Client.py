import ogre.addons.plib as plib
import ctypes

plib.netInit ()
sock = plib.netSocket ()
sock.open        ( False )
sock.setBlocking ( False )
sock.bind        ( "127.0.0.1", 4444 )

maxlen = 8096
storageclass = ctypes.c_char * (maxlen)
buffer_in=storageclass(" ")
ctypes.memset ( buffer_in, 0, maxlen )      # just showing how this can be done


while ( 1 ):
    buffer_in = sock.recv( maxlen, 0 )
    if len(buffer_in)  >= 0 : 
        print "Data in ("+str(len(buffer_in))+"):"+ buffer_in

sock.close ()
