
#if defined(_WIN32)
  // nothing
#else
  int __dummy__(...);
  #define __builtin_va_arg_pack_len __dummy__
#endif

#ifdef ANDYHERE
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include "RakNetTypes.h" 
#include "PacketPriority.h"
#include "MessageIdentifiers.h"
#include "RakNetDefines.h"
#include "Export.h"

#include "RakAssert.h"
#include "RakMemoryOverride.h"
#include "RakNetCommandParser.h"
#include "RakNetStatistics.h"
#include "RakNetTime.h"
#include "RakNetTransport.h"
#include "RakNetVersion.h"
#include "RakPeer.h"
#include "RakSleep.h"
#include "RakString.h"
#include "RakThread.h"

#else

#include "RakPeer.h"  // this needs to be first so winsock2.h is included before windows.h 
// #include "AsynchronousFileIO.h"
#include "AutopatcherPatchContext.h"
#include "AutopatcherRepositoryInterface.h"
#include "AutoRPC.h"
#include "BigInt.h"
#include "BitStream.h"
#include "BitStream_NoTemplate.h"
#include "CheckSum.h"
#include "ClientContextStruct.h"
#include "CommandParserInterface.h"
#include "ConnectionGraph.h"
#include "ConsoleServer.h"
#include "DataBlockEncryptor.h"
#include "DataCompressor.h"
#include "DirectoryDeltaTransfer.h"
// // #include "DS_BinarySearchTree.h"
// // #include "DS_BPlusTree.h"
// // #include "DS_BytePool.h"
// // #include "DS_ByteQueue.h"
// // #include "DS_Heap.h"
// // #include "DS_HuffmanEncodingTree.h"
// // #include "DS_HuffmanEncodingTreeFactory.h"
// // #include "DS_HuffmanEncodingTreeNode.h"
// // #include "DS_LinkedList.h"
// // #include "DS_List.h"
// // #include "DS_Map.h"
//#include "DS_MemoryPool.h"
// // #include "DS_OrderedChannelHeap.h"
// // #include "DS_OrderedList.h"
// // #include "DS_Queue.h"
// // #include "DS_QueueLinkedList.h"
// // #include "DS_RangeList.h"
// // #include "DS_Table.h"
// // #include "DS_Tree.h"
// // #include "DS_WeightedGraph.h"
#include "EmailSender.h"
#include "EpochTimeToString.h"
#include "Export.h"
#include "ExtendedOverlappedPool.h"
//#include "FCMHost.h"
#include "FileList.h"
#include "FileListNodeContext.h"
#include "FileListTransfer.h"
#include "FileListTransferCBInterface.h"
#include "FileOperations.h"
#include "FormatString.h"
#include "FullyConnectedMesh.h"
#include "FunctionThread.h"
#include "Gen_RPC8.h"
#include "GetTime.h"
#include "GridSectorizer.h"
#include "HTTPConnection.h"
#include "IncrementalReadInterface.h"
#include "InlineFunctor.h"
#include "InternalPacket.h"
#include "Itoa.h"
// #include "Kbhit.h"
// #include "LightweightDatabaseClient.h"
// #include "LightweightDatabaseCommon.h"
// #include "LightweightDatabaseServer.h"
#include "LinuxStrings.h"
#include "LogCommandParser.h"
#include "MessageFilter.h"
#include "MessageIdentifiers.h"
#include "MTUSize.h"
#include "NativeTypes.h"
//#include "NatPunchthrough.h"
#include "NetworkIDManager.h"
#include "NetworkIDObject.h"
#include "PacketConsoleLogger.h"
#include "PacketFileLogger.h"
#include "PacketLogger.h"
#include "PacketPool.h"
#include "PacketPriority.h"
#include "Platform.h"
#include "PluginInterface.h"
#include "RakAssert.h"
// // #include "RakMemoryOverride.h"
#include "RakNetCommandParser.h"
#include "RakNetDefines.h"
#include "RakNetStatistics.h"
#include "RakNetTime.h"
#include "RakNetTransport.h"
#include "RakNetTypes.h"
#include "RakNetVersion.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakSleep.h"
#include "RakString.h"
#include "RakThread.h"
#include "Rand.h"
#include "ReadyEvent.h"
#include "RefCountedObj.h"
#include "ReliabilityLayer.h"
#include "Replica.h"
#include "ReplicaEnums.h"
#include "ReplicaManager.h"
#include "ReplicaManager2.h"
// // // #include "Rijndael-Boxes.h"
#include "Rijndael.h"
#include "Router.h"
#include "RouterInterface.h"
#include "RPCMap.h"
#include "RPCNode.h"
#include "RSACrypt.h"
#include "SHA1.h"
#include "SimpleMutex.h"
#include "SimpleTCPServer.h"
#include "SingleProducerConsumer.h"
#include "SocketLayer.h"
#include "StringCompressor.h"
#include "StringTable.h"
#include "SuperFastHash.h"
#include "SystemAddressList.h"
#include "TableSerializer.h"
#include "TCPInterface.h"
#include "TelnetTransport.h"
#include "ThreadPool.h"
#include "ThreadsafePacketLogger.h"
#include "TransportInterface.h"
#include "WSAStartupSingleton.h"
//#include "_FindFirst.h"

#endif
