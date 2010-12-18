
def header_files( version ):
    ret= [  
            'RakPeer.h',
            'AsynchronousFileIO.h',
            'AutopatcherPatchContext.h',
            'AutopatcherRepositoryInterface.h',
            'AutoRPC.h',
#             'BigInt.h',
            'BitStream.h',
#             'BitStream_NoTemplate.h',
            'CheckSum.h',
            'ClientContextStruct.h',
            'CommandParserInterface.h',
            'ConnectionGraph.h',
            'ConsoleServer.h',
            'DataBlockEncryptor.h',
            'DataCompressor.h',
            'DirectoryDeltaTransfer.h',
# #             'DS_BinarySearchTree.h',
# #             'DS_BPlusTree.h',
# #             'DS_BytePool.h',
# #             'DS_ByteQueue.h',
# #             'DS_Heap.h',
# #             'DS_HuffmanEncodingTree.h',
# #             'DS_HuffmanEncodingTreeFactory.h',
# #             'DS_HuffmanEncodingTreeNode.h',
# #             'DS_LinkedList.h',
# #             'DS_List.h',
# #             'DS_Map.h',
            'DS_MemoryPool.h',
# #             'DS_OrderedChannelHeap.h',
# #             'DS_OrderedList.h',
# #             'DS_Queue.h',
# #             'DS_QueueLinkedList.h',
# #             'DS_RangeList.h',
# #             'DS_Table.h',
# #             'DS_Tree.h',
# #             'DS_WeightedGraph.h',
            'EmailSender.h',
            'EpochTimeToString.h',
            'Export.h',
            'ExtendedOverlappedPool.h',
##            'FCMHost.h',
            'FileList.h',
            'FileListNodeContext.h',
            'FileListTransfer.h',
            'FileListTransferCBInterface.h',
            'FileOperations.h',
            'FormatString.h',
            'FullyConnectedMesh.h',
            'FunctionThread.h',
            'Gen_RPC8.h',
            'GetTime.h',
            'GridSectorizer.h',
            'HTTPConnection.h',
            'IncrementalReadInterface.h',
            'InlineFunctor.h',
            'InternalPacket.h',
            'Itoa.h',
#             'Kbhit.h',
#             'LightweightDatabaseClient.h',
#             'LightweightDatabaseCommon.h',
#             'LightweightDatabaseServer.h',
            'LinuxStrings.h',
            'LogCommandParser.h',
            'MessageFilter.h',
            'MessageIdentifiers.h',
            'MTUSize.h',
            'NativeTypes.h',
##            'NatPunchthrough.h',
            'NetworkIDManager.h',
            'NetworkIDObject.h',
            'PacketConsoleLogger.h',
            'PacketFileLogger.h',
            'PacketLogger.h',
            'PacketPool.h',
            'PacketPriority.h',
            'Platform.h',
            'PluginInterface.h',
            'RakAssert.h',
            'RakMemoryOverride.h',
            'RakNetCommandParser.h',
            'RakNetDefines.h',
            'RakNetStatistics.h',
            'RakNetTime.h',
            'RakNetTransport.h',
            'RakNetTypes.h',
            'RakNetVersion.h',
            'RakNetworkFactory.h',
            'RakPeerInterface.h',
            'RakSleep.h',
            'RakString.h',
            'RakThread.h',
            'Rand.h',
            'ReadyEvent.h',
            'RefCountedObj.h',
            'ReliabilityLayer.h',
            'Replica.h',
            'ReplicaEnums.h',
            'ReplicaManager.h',
            'ReplicaManager2.h',
# # # #             'Rijndael-Boxes.h',
            'Rijndael.h',
            'Router.h',
            'RouterInterface.h',
            'RPCMap.h',
            'RPCNode.h',
            'RSACrypt.h',
            'SHA1.h',
            'SimpleMutex.h',
            'SimpleTCPServer.h',
            'SingleProducerConsumer.h',
            'SocketLayer.h',
            'StringCompressor.h',
            'StringTable.h',
            'SuperFastHash.h',
            'SystemAddressList.h',
            'TableSerializer.h',
            'TCPInterface.h',
            'TelnetTransport.h',
            'ThreadPool.h',
            'ThreadsafePacketLogger.h',
            'TransportInterface.h',
            'WSAStartupSingleton.h',
#             '_FindFirst.h'
            ]
            
    ret1 = ['RakPeerInterface.h'
         ,'RakNetworkFactory.h'
         ,'RakNetTypes.h'
         ,'PacketPriority.h'
         ,'MessageIdentifiers.h'
         ,'RakNetDefines.h'
         ,'Export.h'
         ,'RakAssert.h'
         ,'RakMemoryOverride.h'
         ,'RakNetCommandParser.h'
         ,'RakNetStatistics.h'
         ,'RakNetTime.h'
         ,'RakNetTransport.h'
         ,'RakNetVersion.h'
         ,'RakPeer.h'
         ,'RakSleep.h'
         ,'RakString.h'
         ,'RakThread.h'

         ]            
    return ret
                
def huge_classes( version ):
    return []
   