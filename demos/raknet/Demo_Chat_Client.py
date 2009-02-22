import os, sys, time, msvcrt
import ctypes
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
   ## Pointers to the interfaces of our server and client.
   ## Note we can easily have both in the same program
   client=raknet.RakNetworkFactory.GetRakPeerInterface()
 
   print "This is a sample implementation of a text based chat client."
   print "Connect to the project 'Chat Example Server'.\n"

   clientPort="0"
   ip = "127.0.0.1"
   serverPort="10000"

   ## Connecting the client is very simple.  0 means we don't care about
   ## a connectionValidationInteger, and False for low priority threads
   socketDescriptor= raknet.SocketDescriptor (int(clientPort),'')
   
   client.Startup(1,30,socketDescriptor, 1)
   client.SetOccasionalPing(True)
   
   b = client.Connect(ip, int(serverPort), "Rumpelstiltskin", len("Rumpelstiltskin"))  
   if b:
      print "Attempting connection"
   else:
      print"Bad connection attempt.  Terminating."
      sys.exit(1)
   

   
   print("My IP is ", ctypes.string_at(client.GetLocalIP(0)))
   print("My GUID is ", ctypes.string_at(client.GetGuidFromSystemAddress(raknet.UNASSIGNED_SYSTEM_ADDRESS).ToString()))
   print("'quit' to quit. 'stat' to show stats. 'ping' to ping. 'disconnect' to disconnect. Type to talk.")
   
   ## Loop for input
   while (1):
      if msvcrt.kbhit():
      
         ## Notice what is not here: something to keep our network running.  It's
         ## fine to block on gets or anything we want
         ## Because the network engine was painstakingly written using threads.
         message=raw_input()
         
         if message=="quit":
            print("Quitting.")
            break

         if message=="stat":
             rss=client.GetStatistics(client.GetSystemAddressFromIndex(0))
             print rss #raknet.StatisticsToString(rss, message, 2)
             print("Ping=", client.GetAveragePing(client.GetSystemAddressFromIndex(0)))
             continue

         if message=="disconnect":
            client.CloseConnection(client.GetSystemAddressFromIndex(0),True,0)
            print("Disconnecting.\n")
            continue

         if message=="ping":
            if client.GetSystemAddressFromIndex(0)!=raknet.UNASSIGNED_SYSTEM_ADDRESS:
               client.Ping(client.GetSystemAddressFromIndex(0))
            continue
         
         ## message is the data to send
         ## strlen(message)+1 is to send the null terminator
         ## HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
         ## RELIABLE_ORDERED means make sure the message arrives in the right order
         client.Send(message, len(message)+1, raknet.HIGH_PRIORITY, raknet.RELIABLE_ORDERED, '', 
                           raknet.UNASSIGNED_SYSTEM_ADDRESS, True)

      p = client.Receive()
      if not p:
         continue ## Didn't get any packets
         
#     ## We got a packet, get the identifier with our handy function
      pointer, packetIdentifier = GetPacketInformation(p)
      
#     ## Check if this is a network message packet
      if packetIdentifier == int(raknet.ID_DISCONNECTION_NOTIFICATION):
          ## Connection lost normally
          print("ID_DISCONNECTION_NOTIFICATION\n")
          break
      elif packetIdentifier == int(raknet.ID_ALREADY_CONNECTED):
          ## Connection lost normally
          print("ID_ALREADY_CONNECTED\n")
          break
      elif packetIdentifier == int(raknet.ID_REMOTE_DISCONNECTION_NOTIFICATION): ## Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
          print("ID_REMOTE_DISCONNECTION_NOTIFICATION\n")
          break
      elif packetIdentifier == int(raknet.ID_REMOTE_CONNECTION_LOST): ## Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
          print("ID_REMOTE_CONNECTION_LOST\n")
          break
      elif packetIdentifier == int(raknet.ID_REMOTE_NEW_INCOMING_CONNECTION): ## Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
          print("ID_REMOTE_NEW_INCOMING_CONNECTION\n")
          break
      elif packetIdentifier == int(raknet.ID_CONNECTION_BANNED): ## Banned from this server
          print("We are banned from this server.\n")
          break       
      elif packetIdentifier == int(raknet.ID_CONNECTION_ATTEMPT_FAILED):
          print("Connection attempt failed\n")
          break
      elif packetIdentifier == int(raknet.ID_NO_FREE_INCOMING_CONNECTIONS):
           ## Sorry, the server is full.  I don't do anything here but
          ## A real app should tell the user
          print("ID_NO_FREE_INCOMING_CONNECTIONS\n")
          break
      elif packetIdentifier == int(raknet.ID_MODIFIED_PACKET):
          ## Cheater!
          print("ID_MODIFIED_PACKET\n")
          break
      elif packetIdentifier == int(raknet.ID_INVALID_PASSWORD):
          print("ID_INVALID_PASSWORD\n")
          break
# 
      elif packetIdentifier == int(raknet.ID_CONNECTION_LOST):
          ## Couldn't deliver a reliable packet - i.e. the other system was abnormally
          ## terminated
          print("ID_CONNECTION_LOST\n")
          break
# 
      elif packetIdentifier == int(raknet.ID_CONNECTION_REQUEST_ACCEPTED):
          ## This tells the client they have connected
          print "ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", (p.systemAddress.ToString(), p.guid.ToString())
          continue
      else:
          ## It's a client, so just show the message - assume it's a printable string
          b = ctypes.string_at ( pointer )
          print b

      ## We're done with the packet
      client.DeallocatePacket(p)

   ## Be nice and let the server know we quit.
   client.Shutdown(300)

   ## We're done with the network
   raknet.RakNetworkFactory.DestroyRakPeerInterface(client)

   return 0

if __name__ == "__main__":
   main()


