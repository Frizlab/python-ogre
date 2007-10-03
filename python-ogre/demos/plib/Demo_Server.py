import ogre.addons.plib as plib
import ctypes

plib.netInit ()

sock = plib.netSocket () 
sock.open        ( False )
sock.setBlocking ( False )
sock.connect     ( "127.0.0.1", 4444 )

count = 0
maxlen = 256
storageclass = ctypes.c_char * (maxlen)
buffer_out=storageclass(" ")
ctypes.memset ( buffer_out, 39, maxlen )      # just showing how this can be done


while ( count < 10 ):
    msg = "Hello :" + str ( count )
    print msg
    buffer_out[:len(msg)] = msg
    ret = sock.send ( ctypes.addressof(buffer_out), len(msg), 0 )
    print ret
    count +=1

sock.close ()
