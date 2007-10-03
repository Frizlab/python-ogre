""" 
VERY simple and crude client code.
Assumes there is a server that will echo the sent data and when it's time to close
the server will send "Done"

"""

import socket as s

if __name__ == "__main__":
    # we need to send (and recieve) data on the local machine using port 4444
    (family, socktype, proto, canonname, sockaddr) = s.getaddrinfo ( "127.0.0.1",4444)[0] #s.gethostname(), 4444 )[0]
    
    socket = s.socket( s.AF_INET, s.SOCK_DGRAM ) # create the client socket, note I'm testing with UDP traffic
    socket.connect ( sockaddr )
    loop = True
    count = 0
    while loop:
        socket.send ( str(count) + " hello out there")
        incomming = socket.recv ( 256 )
        print "Received:", incomming
        if incomming == "Done":  ## special -- it's up to the server to decided when we are done...
            loop = False
        count += 1
    socket.close()
    