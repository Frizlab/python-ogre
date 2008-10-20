# NO_UNITTEST
"""
VERY simple and crude Python server

Receives data in a none state ful fashion -- loops as an echo server for a while
and after 10 loops sends a 'Done' to the client and tells the server to stop
 
"""

import socket as s
import SocketServer as ss
import sys

class Handler ( ss.BaseRequestHandler ):
    def handle ( self ):
        datain = self.request[0]  # get the input string
        s=datain.split()  ## Assume input data is <count><space><rest of info>
        count = int ( s[0] )
        
        sock = self.request[1] ## this is the incomming socket
        
        ## because this is a UDP demo we need to connect to send info back
        ## I suspect this isn't needed if we use TCP server
        sock.connect ( self.client_address ) 
        
        if count > 10:  # we are only looping 10 times
            sock.send ("Done")
            self.server.closenow = True # tell the server we are done..
        else:
            sock.send ( "Back at you: " + datain) ## send data back
            

class Server(ss.ThreadingMixIn, ss.UDPServer): pass

if __name__ == "__main__":
    # we create a server on the local machine listening to port 4444..
    # could also use "localhost" instead of gethostname()
    (family, socktype, proto, canonname, sockaddr) = s.getaddrinfo ( "127.0.0.1", 4444 )[0]
    myServer = Server ( sockaddr, Handler )
    myServer.closenow = False
    try:
        while not myServer.closenow:
            myServer.handle_request()
    except KeyboardInterrupt:
        sys.exit()
    except:
        sys.exit()        