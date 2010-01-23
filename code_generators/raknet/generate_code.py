#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------

## STARTER TEMPLATE..
## replace raknet with lowercase project name
## set MAIN_NAMESPACE
## rename and configure .h files


import os, sys, time, shutil
try:
   import psyco
   psyco.full()
except ImportError:
   pass

#add environment to the path
sys.path.append( os.path.join( '..', '..' ) )
#add common utils to the pass
sys.path.append( '..' )
sys.path.append( '.' )

import environment
import common_utils
import customization_data
import hand_made_wrappers


import pygccxml
from pygccxml import parser
from pygccxml import declarations
from pyplusplus import messages
from pyplusplus import module_builder
from pyplusplus import decl_wrappers
from pyplusplus.code_creators import include

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
try:
  from pyplusplus.creators_factory import sort_algorithms
except ImportError, e:
  from pyplusplus.module_creator import sort_algorithms

import common_utils.extract_documentation as exdoc
# # import common_utils.sphinx_extract as exdoc

import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties
from common_utils import docit

# override standard pretty name function
from pyplusplus.decl_wrappers import algorithm
# # # algorithm.create_valid_name = common_utils.PO_create_valid_name 


MAIN_NAMESPACE = ''

############################################################
##
##  Here is where we manually exclude stuff
##
############################################################

def ManualExclude ( mb ):
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns    
        
    excludes = [
               '::HTTPConnection::Post'  ## TODO Needs to be handwrapped to be useful
                #,'::RakPeerInterface::RegisterAsRemoteProcedureCall'
                # these are missing from source
                ,'::FileListTransfer::SetIncrementalReadInterface'
                ,'::NatPunchthrough::ConnectionRequest::GetAddressList'
#                 '::RakNetworkFactory::GetConsoleServer'
#                 ,'::RakNetworkFactory::GetTelnetTransport'
#                 ,'::RakNetworkFactory::GetRouter'
#                 ,'::RakNetworkFactory::GetReplicaManager'
#                 ,'::RakNetworkFactory::GetPacketLogger'
#                 ,'::RakNetworkFactory::GetRakNetTransport'
#                 ,'::RakNetworkFactory::GetRakNetCommandParser'
#                 '::RakPeerInterface::SetNetworkIDManager'
#                 ,'::RakPeerInterface::GetNetworkIDManager'
                
                ]
                   
         
    for e in excludes:
        try:
            main_ns.member_functions(e).exclude()
            print "Excluded Function:", e
        except:
            print "Unable to exclude:", e
#     i=['GetRakPeerInterface']
#     for fun in main_ns.class_('::RakNetworkFactory').member_functions():
#        if fun.name not in i:
#          fun.exclude()
#     main_ns.class_('::RakPeerInterface').exclude()   
#     return
            
    excludes = ['::ReplicaManager::ParticipantStruct'
               ,'::RakNet::Connection_RM2'
               ,'::RakNet::Replica2'
               ,'::RakNet::ReplicaManager2'
               ,'::RakNet::AutoRPC'
               ,'::RakNet::RakString'
               ,'::ReliabilityLayer'

               ]
    for e in excludes:   
      try:
         main_ns.class_(e).exclude()
         print "Excluded class:", e
      except:
         print "Failed to exclude", e   
         
         
    excludes = ['::RakNet::_RakRealloc'
               ,'::RakNet::_RakMalloc'
               ,'::RakNet::_RakFree'
               ]
    for e in excludes:   
      try:
         main_ns.free_function(e).exclude()
         print "Excluded freefunction:", e
      except:
         print "Failed to exclude", e         
         sys.exit()
       
    excludes = ['::CommandParserInterface::VARIABLE_NUMBER_OF_PARAMETERS'
               ,'::RakPeer::RemoteSystemStruct::reliabilityLayer'
               ]
    for e in excludes:
      print "Excluding Variable", e
      main_ns.variable(e).exclude()                    
############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################
    
def ManualInclude ( mb ):
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns  
    includes = ['AVLBalancedBinarySearchTree',
                'BinarySearchTree',
                'BitStream',
                'BPlusTree',
                'BytePool',
                'CommandParserInterface',
#                 'Connection_RM2',
#                 'Connection_RM2Factory',
#                 'ConnectionGraph',
                'ConsoleServer',
                'CSHA1',
                'DataCompressor',
                'DirectoryDeltaTransfer',
                'FCMHost',
                'FileList',
                'FileListProgress',
                'FileListTransfer',
                'FLP_Printf',
                'FunctionThread',
                'FunctionThreadDependentClass',
                'Heap',
                'HTTPConnection',
                'HuffmanEncodingTree',
                'IncrementalReadInterface',
#                 'LightweightDatabaseClient',
#                 'LightweightDatabaseServer',
                'LinkedList',
                'List',
                'LogCommandParser',
                'Map',
                'MemoryPool',
                'MessageFilter',
                'NatPunchthrough',
                'NatPunchthroughLogger',
                'NetworkIDManager',
                'NetworkIDObject',
                'OrderedChannelHeap',
                'OrderedList',
                'PacketConsoleLogger',
                'PacketFileLogger',
                'PacketLogger',
                'PluginInterface',
                'Queue',
                'QueueLinkedList',
                'RakNetCommandParser',
                'RakNetRandom',
                'RakNetTransport',
                'RakNetTransportCommandParser',
                'RakNetworkFactory',
                'RakPeer',
                'RakPeerInterface',
                'RakString',
                'RangeList',
                'Replica',
                'Replica2',
                'ReplicaManager',
                'ReplicaManager2',
                'Router',
                'RouterInterface',
                'RSACrypt',
                'SimpleMutex',
                'SingleProducerConsumer',
                'StringCompressor',
                'StringTable',
                'SystemAddress',
                'Table',
                'TableSerializer',
                'TCPInterface',
                'TelnetTransport',
                'ThreadsafePacketLogger',
#                 'TransportInterface',  # only used by commandparser etc and has a send function that needs wrapping
                'Tree',
                'WeightedGraph'
                
#                 'SystemAddressList', # not in the dll
                
                ,'FileListTransferCBInterface'
#                 ,'RemoteClient'   # send and recv not in dll
                ,'InternalPacket'
                ,'ReceiveDownloadCompleteInterface'
                ,'SendDownloadCompleteInterface'
                ,'FileListNodeContext'
                ,'ReceiveConstructionInterface'
                ,'Packet'
                ,'SocketDescriptor'
                ,'FilterSet'
                ,'NetworkID'
                ,'RegisteredCommand'
                ,'RakNetGUID'
                ,'RakNetStatistics'
                ,'RakSleep'
                
                ,'PacketPriority'
                ,'ReplicaReturnResult'
                ,'PacketReliability'
                
                ,'DefaultMessageIDTypes'
                
                ]  
                    
    for c in includes:
        try:
            main_ns.class_('::'+c).include()
            print "Included Class ", c
        except:
            try:
               main_ns.enumeration('::'+c).include()
               print "Included enum ", c
            except:     
               try:
                  main_ns.free_function('::'+c).include()
                  print "Included free function ", c
               except:          
                  print "FAIL: Can't include class/enum/function ", c
               
    includes = ['::UNASSIGNED_SYSTEM_ADDRESS',
               '::UNASSIGNED_RAKNET_GUID'
               ]
    for c in includes:
        main_ns.variable(c).include()
        print "Included variable ", c
        try:
            main_ns.variable(c).include()
            print "Included variable ", c
        except:
            print "Unable to include var:", c
                                  
    main_ns.namespace ( 'RakNet').include()
#     main_ns.namespace ( 'RakPeer').include()
# #     main_ns.namespace ( 'DataStructures').include()
                                     
# #     for cls in main_ns.classes():
# #         try:
# #             if  cls.decl_string[2]=='d' and cls.decl_string[3].isupper():
# #                 print "Including Class:", cls.name
# #                 cls.include()
# #         except:
# #             pass
          
        
############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixes ( mb ):    
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns
              
############################################################
##
##  And things that need to have their argument and call values fixed.
##  ie functions that pass pointers in the argument list and of course we need
##  to read the updated values - so instead we pass them back 
##  as new values in a tuple (ETC ETC)
##
############################################################
        
def ManualTransformations ( mb ):
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns
                
    def create_output( size ):
        return [ ft.output( i ) for i in range( size ) ]
        
       
###############################################################################
##
##  Now for the AUTOMATIC stuff that should just work
##
###############################################################################
    
def AutoFixes ( mb, MAIN_NAMESPACE ): 
    """ now we fix a range of things automatically - typically by going through 
    the entire name space trying to guess stuff and fix it:)
    """    
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns
        
    # Functions that have void pointers in their argument list need to change to unsigned int's  
    pointee_types=[]
    ignore_names=[]
    #common_utils.Fix_Void_Ptr_Args  ( main_ns ) # , pointee_types, ignore_names )

    # and change functions that return a variety of pointers to instead return unsigned int's
    pointee_types=[]
    ignore_names=[]  # these are function names we know it's cool to exclude
    common_utils.Fix_Pointer_Returns ( main_ns ) # , pointee_types, ignore_names )   

    # functions that need to have implicit conversions turned off
    ImplicitClasses=[] 
    common_utils.Fix_Implicit_Conversions ( main_ns, ImplicitClasses )
    
    if os.name =='nt':
        Fix_NT( mb )
    elif os.name =='posix':
        Fix_Posix( mb )
        
        
 
###############################################################################
##
## here are the helper functions that do much of the work
##
###############################################################################     
def Fix_Posix ( mb ):
    """ fixup for posix specific stuff -- note only expect to be called on a posix machine
    """
    ## we could do more here if need be...
    if sys.platform == 'darwin':
        pass
    elif sys.platform.startswith ('linux'):
        pass


def Fix_NT ( mb ):
    """ fixup for NT systems
    """
        

#
# the 'main'function
#            
def generate_code():  
# # #     messages.disable( 
# # # #           Warnings 1020 - 1031 are all about why Py++ generates wrapper for class X
# # #           messages.W1020
# # #         , messages.W1021
# # #         , messages.W1022
# # #         , messages.W1023
# # #         , messages.W1024
# # #         , messages.W1025
# # #         , messages.W1026
# # #         , messages.W1027
# # #         , messages.W1028
# # #         , messages.W1029
# # #         , messages.W1030
# # #         , messages.W1031
# # #         , messages.W1035
# # #         , messages.W1040 
# # #         , messages.W1038        
# # #         , messages.W1041
# # #         , messages.W1036 # pointer to Python immutable member
# # #         , messages.W1033 # unnamed variables
# # #         , messages.W1018 # expose unnamed classes
# # #         , messages.W1049 # returns reference to local variable
# # #         , messages.W1014 # unsupported '=' operator
# # #          )
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.raknet.root_dir, "python_raknet.h" )
                        , environment.raknet.cache_file )

    defined_symbols = [ '_WIN32', '__PYTHONOGRE_BUILD_CODE', '_RELEASE', '_RAKNET_DLL', '_CRT_NONSTDC_NO_DEPRECATE' ] # '_RAKNET_LIB' ]_RAKNET_DLL

    defined_symbols.append( 'VERSION_' + environment.raknet.version )  
    
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.raknet.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.raknet.cflags
                                           )
                                           
    # if this module depends on another set it here                                           
#     mb.register_module_dependency ( environment.ogre.generated_dir )
    
    # normally implicit conversions work OK, however they can cause strange things to happen so safer to leave off
    mb.constructors().allow_implicit_conversion = False                                           
    
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
            
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()
    main_ns = global_ns## .namespace( MAIN_NAMESPACE )
#     main_ns.include()
       
    ManualInclude ( mb )
    
    common_utils.AutoExclude ( mb, MAIN_NAMESPACE )
    ManualExclude ( mb )
    common_utils.AutoInclude ( mb, MAIN_NAMESPACE )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    AutoFixes ( mb, MAIN_NAMESPACE )
    ManualFixes ( mb )
    
    common_utils.Auto_Functional_Transformation ( main_ns  )
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    common_utils.Set_DefaultCall_Policies ( main_ns )
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    NoPropClasses = [""]
    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            
    # THIS MUST BE AFTER Auto_Functional_Transformation
    common_utils.Auto_Document( mb, MAIN_NAMESPACE )
    
    ## add additional version information to the module to help identify it correctly 
    common_utils.addDetailVersion ( mb, environment, environment.raknet )

    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor() # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_raknet_' , doc_extractor= extractor )
    
    for inc in environment.raknet.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.raknet.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.raknet.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.raknet.version ) )

    mb.split_module(environment.raknet.generated_dir, huge_classes, use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generated directory..
    # we could change raknet to be dll exported in raknet source but this would mean patching :(.
    # so instead we simply include the missing class in our own wrapper..
    additional_files=[ 'ReliabilityLayer.cpp','RakMemoryOverride.cpp','DataBlockEncryptor.cpp',
                     'SocketLayer.cpp','rijndael.cpp', 'CheckSum.cpp','WSAStartupSingleton.cpp', 'RakPeer.cpp',
                     'RakThread.cpp'
                     ]
    additional_files=[]                     
    for sourcefile in additional_files:
        sourcefile = os.path.join(environment.Config.PATH_INCLUDE_raknet, sourcefile )
        p,filename = os.path.split(sourcefile)
        destfile = os.path.join(environment.raknet.generated_dir, filename ) 
    
        if not common_utils.samefile( sourcefile ,destfile ):
            shutil.copy( sourcefile, environment.raknet.generated_dir )
            print "Updated ", filename, "as it was missing or out of date"
    
#     common_utils.copyTree ( sourcePath = environment.Config.PATH_INCLUDE_raknet, 
#                             destPath = environment.raknet.generated_dir, 
#                             recursive=False )
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
