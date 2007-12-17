import socket as s
import SocketServer as ss
import random

NAMES = ['Whittle', 'Bark', 'Chafer', 'Cap', 'Bolete', 'Willow', 'Chestnut',
                'Sunny', 'Lemon', 'Spotted', 'Morsel', 'Gummy', 'Fibrous' 'Old-Mans',
                'Death', 'Mulch']

class ProspectingManager(ss.BaseRequestHandler):
    def handle(self):
        try:
            datain = self.request[0]  # get the input string
            s=datain.split()  ## Assume input data is <count><space><rest of info>
            
            sock = self.request[1] ## this is the incomming socket
            
            fname = random.choice(NAMES) + ' ' + random.choice(NAMES)
            
            ## because this is a UDP demo we need to connect to send info back
            ## I suspect this isn't needed if we use TCP server
            #sock.connect ( self.client_address ) 
            
            sock.send (fname) ## send data back
        except:
            pass
        
        
        
class Server(ss.ThreadingMixIn, ss.TCPServer): pass

if __name__ == "__main__":
    # we create a server on the local machine listening to port 4444..
    # could also use "localhost" instead of gethostname()
    (family, socktype, proto, canonname, sockaddr) = s.getaddrinfo ( "127.0.0.1", 4444 )[0]
    myServer = Server ( sockaddr, ProspectingManager )
    myServer.closenow = False
    print 'Server Started'
    while not myServer.closenow:
        myServer.handle_request()
        
        
