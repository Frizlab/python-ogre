import os, sys, time, msvcrt
import ctypes as ctypes
import ogre.renderer.OGRE as ogre  # only do this to get boost into the path :)
import ogre.addons.raknet as raknet

## Copied from Multiplayer.cpp
## If the first byte is ID_TIMESTAMP, then we want the 5th byte
## Otherwise we want the 1st byte
def GetPacketInformation (p):
   if not p:
      return 0,255
   # ugly here -- p.data is the address of the pointer to the data...      
   pointer = ctypes.c_uint.from_address(p.data)
   # now we need to get the first byte that it's pointing to..
   c = ctypes.c_ubyte.from_address(pointer.value)
   if c.value == int(raknet.ID_TIMESTAMP):
      print "TIMESTAMP"
      pointer.value += 5
      control = ctypes.c_ubyte.from_address(pointer.value)
   return pointer.value, c.value


def main():
   server=raknet.RakNetworkFactory.GetRakPeerInterface()
   i = server.GetNumberOfAddresses()
   server.SetIncomingPassword("Rumpelstiltskin", len("Rumpelstiltskin"))

   ## Record the first client that connects to us so we can pass it to the ping function
   clientID=raknet.UNASSIGNED_SYSTEM_ADDRESS


   print("This is a sample implementation of a text based chat server.\n")
   print("Connect to the project 'Chat Example Client'.\n")
   print("Difficulty: Beginner\n\n")

   ## A server
   portstring = "10000"
   
   print("Starting server.")
   socketDescriptor = raknet.SocketDescriptor (int(portstring),"127.0.0.1")
   b = server.Startup(32, 30, socketDescriptor, 1 )
   server.SetMaximumIncomingConnections(2)
   if b:
      print("Server started, waiting for connections.")
   else:
      print("Server failed to start.  Terminating.")
      sys.exit(1)

   server.SetOccasionalPing(True)

   print("My IP is ", ctypes.string_at(server.GetLocalIP(0)))
   print("My GUID is ", ctypes.string_at(server.GetGuidFromSystemAddress(raknet.UNASSIGNED_SYSTEM_ADDRESS).ToString()))
   print("'quit' to quit. 'stat' to show stats. 'ping' to ping. 'disconnect' to disconnect. Type to talk.")
   
   ## Loop for input
   while (1):
   
      ## This sleep keeps RakNet responsive
      raknet.RakSleep(30)

      if (msvcrt.kbhit()):
         ## Notice what is not here: something to keep our network running.  It's
         ## fine to block on gets or anything we want
         ## Because the network engine was painstakingly written using threads.
         message=raw_input()

         if message=="quit":
            print("Quitting.")
            break

         if message=="stat":
            rss=server.GetStatistics(server.GetSystemAddressFromIndex(0))
#           StatisticsToString(rss, message, 2)
#           print("%s", message)
            print "Ping %i\n" % (server.GetAveragePing(server.GetSystemAddressFromIndex(0) ) )
            continue

         if message== "ping":
            server.Ping(clientID)
            continue

         if message=="kick":
            server.CloseConnection(clientID, True, 0)
            continue

         if message=="ban":
#           print("Enter IP to ban.  You can use * as a wildcard\n")
#           gets(message)
#           server.AddToBanList(message)
#           print("IP %s added to ban list.\n", message)
            continue


         ## Append Server: to the message so clients know that it ORIGINATED from the server
         ## All messages to all clients come from the server either directly or by being
         ## relayed from other clients
      
         ## message2 is the data to send
         ## strlen(message2)+1 is to send the null terminator
         ## HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
         ## RELIABLE_ORDERED means make sure the message arrives in the right order
         ## We arbitrarily pick 0 for the ordering stream
         ## UNASSIGNED_SYSTEM_ADDRESS means don't exclude anyone from the broadcast
         ## True means broadcast the message to everyone connected
         m = "Server: " + message
         print len (m)
         server.Send(m, len (m)+1, raknet.HIGH_PRIORITY, raknet.RELIABLE_ORDERED, '', raknet.UNASSIGNED_SYSTEM_ADDRESS, True)

      ## Get a packet from either the server or the client

      p,data, time_ = server.ReceiveList()
      
      if not p:
         continue ## Didn't get any packets
         
      packetIdentifier = int(data[0]) # first byte is type of packet
      
      ## Check if this is a network message packet
      if packetIdentifier == int(raknet.ID_DISCONNECTION_NOTIFICATION):
          ## Connection lost normally
          print("ID_DISCONNECTION_NOTIFICATION\n")
    
      elif packetIdentifier == int(raknet.ID_NEW_INCOMING_CONNECTION):
           ## Somebody connected.  We have their IP now
          print("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", p.systemAddress.ToString(), p.guid.ToString())
          clientID=p.systemAddress ## Record the player ID of the client

      elif packetIdentifier == int(raknet.ID_MODIFIED_PACKET):
          ## Cheater!
          print("ID_MODIFIED_PACKET\n")

      elif packetIdentifier == int(raknet.ID_CONNECTION_LOST):
          print("ID_CONNECTION_LOST\n")
          
      else:
          ## The server knows the static data of all clients, so we can prefix the message
          ## With the name data
          
          s= "".join(map(chr,data))
          print s

          ## Relay the message.  We prefix the name for other clients.  This demonstrates
          ## That messages can be changed on the server before being broadcast
          ## Sending is the same as before
          server.Send(s, len(s)+1, raknet.HIGH_PRIORITY, raknet.RELIABLE_ORDERED, '', p.systemAddress, True)

      ## We're done with the packet
      server.DeallocatePacket(p)

   server.Shutdown(300)
   ## We're done with the network
   raknet.RakNetworkFactory.DestroyRakPeerInterface(server)

   return 0

if __name__ == "__main__":
   main()
