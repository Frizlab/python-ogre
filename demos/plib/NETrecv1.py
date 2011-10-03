import ogre.addons.plib as plib

plib.netInit ()
sock = plib.netSocket ()
if not sock.open ( False ):
    raise RuntimeError ("Unable to open a socket")
sock.setBlocking ( False )
if sock.bind ( "127.0.0.1", 4444 ) == -1:
    raise RuntimeError ("Unable to bind socket")
print "Waiting to receive data."
while ( True ):
    # use recv to recive the input data as a list of invidual bytes -- suitable for binary data etc
    # use recv_str to receive the input data as a string
    # specify maxlen if you need to receive greater than 2048 ie recv(4096)
    buffer_in = sock.recv_str( )  
    if len(buffer_in)  >0 : 
        print "Received:", buffer_in

sock.close ()
