#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------


import os, sys, time, shutil

#add environment to the path
sys.path.append( os.path.join( '..', '..' ) )
#add common utils to the pass
sys.path.append( '..' )
sys.path.append( '.' )

import environment
import common_utils
import customization_data
import hand_made_wrappers
import register_exceptions

from pygccxml import parser
from pygccxml import declarations
from pyplusplus import messages
from pyplusplus import module_builder
from pyplusplus import decl_wrappers

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
try:
  from pyplusplus.creators_factory import sort_algorithms
except ImportError, e:
  from pyplusplus.module_creator import sort_algorithms

import common_utils.extract_documentation as exdoc
import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties

MAIN_NAMESPACE = 'Ogre'
AUDIO = True

## small helper function
def docit ( general, i, o ): 
    docs = "Python-Ogre Modified Function Call\\n" + general +"\\n"
    docs = docs + "Input: " + i + "\\n"
    docs = docs + "Output: " + o + "\\n\\\n"
    return docs

############################################################
##
##  Here is where we manually exclude stuff
##
############################################################

def ManualExclude ( mb ):
    global_ns = mb.global_ns
    main_ns = global_ns #.namespace( MAIN_NAMESPACE )

    # things not yet implemented in the C source..
    excludes=[]
#     for c in main_ns.classes():
#         for m in c.member_functions(allow_empty=True):
#             for e in excludes:
#                 if e in m.decl_string:
#                     m.exclude()
     
    ### Member Functions
    excludes=['::Ogre::TheoraVideoClip::setPriority'
            ,'::Ogre::TheoraVideoClip::setNumPrecachedFrames'
            ,'::Ogre::TheoraVideoClip::getWidth'
            ,'::Ogre::TheoraVideoClip::getHeight'
            ,'::Ogre::TheoraVideoManager::setNumWorkerThreads'
            ,'::Ogre::TheoraVideoManager::setDefaultNumPrecachedFrames'
            ,'::Ogre::TheoraVideoManager::getNumWorkerThreads'
            ,'::Ogre::TheoraVideoManager::getDefaultNumPrecachedFrames'
            ,'::TheoraVideoClip::getNextFrame'
            ,'::TheoraVideoFrame::getBuffer'
            ,'::TheoraVideoClip::setPriority'  #not implemented
            ,'::Ogre::OgreVideoManager::getTheoraVideoManager'
            ]
    for e in excludes:
        try:
            main_ns.member_functions(e).exclude()
            print "excluding ", e
        except:
            print "Unable to exclude: ", e

    ### Free Functions
    excludes = []
    for e in excludes:
        main_ns.free_functions(e).exclude()
        
    ## Classes
# #     excludes = ['::Ogre::TheoraSeekUtility'
# #                 ,'::Ogre::TheoraVideoController::CmdRenderFx'
# #                 ,'::Ogre::TheoraVideoController::CmdSeekingEnabled']
# #     for e in excludes:
# #         main_ns.class_(e).exclude()
    
        
    ## I have a challenge that Py++ doesn't recognise these classes by full name (perhaps because they are structs?)
    ## so I have to look through and match on a class by class basis
    excludeName = [ ]
    for c in main_ns.classes():
        if c.name in excludeName:
            c.exclude()
       
      
    ### Variables        
    excludes = []
    for e in excludes:
        main_ns.variable(e).exclude()
        
    ### Typedefs    
    excludes = []
    for e in excludes:
        main_ns.typedefs(e).exclude()
        
    ### Operators        
    excludes=[]
    for e in excludes:
        main_ns.operators(e).exclude()
        
    ### Constructors
    
                

############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################
    
def ManualInclude ( mb ):
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    if AUDIO:
       main_ns.class_('::Ogre::OpenAL_AudioInterface').include()
       main_ns.class_('::Ogre::OpenAL_AudioInterfaceFactory').include()
    
    ## need this as it's a base class in Theora
#     c = main_ns.class_('::Ogre::ExternalTextureSource')
#     c.include()
#     for cls in c.classes():
#         if cls.name.startswith('Cmd'):
#             cls.exclude()
#             
# # #     c = main_ns.class_('::Ogre::TheoraVideoController')
# # #     print dir ( c )
# # #     print c.bases
# # #     for i in c.bases:
# # #         print "Base: ", i
# # #     print dir (i)    
#     
# #     main_ns.enum('::Ogre::eTexturePlayMode').include()
#     main_ns.enum('::Ogre::eAudioSampleFormat').include()
#     main_ns.class_('::Ogre::RingBuffer').include(already_exposed=True)
#     main_ns.class_('::Ogre::TexturePtr').include(already_exposed=True)
    
# #     main_ns.class_('::Ogre::SoundManager').include()
    
    
############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixes ( mb ):    

    global_ns = mb.global_ns
    
    # fix issue where the namespace isn't in the default values
    
    # issues with call policies not being set
    fix = ['::TheoraVideoManager::getAudioInterfaceFactory',
            '::TheoraVideoManager::createVideoClip',
            '::TheoraVideoManager::getSingleton',
            '::TheoraVideoManager::getSingletonPtr',
            '::TheoraVideoManager::getVideoClipByName',
            '::TheoraVideoManager::requestWork',
            '::TheoraAudioInterfaceFactory::createInstance',
            '::TheoraVideoClip::getAudioInterface',
            '::TheoraVideoClip::getNextFrame',
            '::TheoraVideoClip::getTimer',
            '::TheoraVideoFrame::getBuffer'
            
            ]
    for m in fix:
        mf = global_ns.member_functions(m)
        mf.call_policies = call_policies.return_value_policy(
                call_policies.reference_existing_object )

    
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
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
        
    def create_output( size ):
        return [ ft.output( i ) for i in range( size ) ]
 
    
###############################################################################
##
##  Now for the AUTOMATIC stuff that should just work
##
###############################################################################
    
    
def AutoExclude( mb ):
    """ Automaticaly exclude a range of things that don't convert well from C++ to Python
    """
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    
    # vars that are static consts but have their values set in the header file are bad
    Remove_Static_Consts ( main_ns )
    
    ## Exclude protected and private that are not pure virtual
    query = ~declarations.access_type_matcher_t( 'public' ) \
            & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
    non_public_non_pure_virtual = main_ns.calldefs( query )
    non_public_non_pure_virtual.exclude()
    
    #Virtual functions that return reference could not be overriden from Python
    query = declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.VIRTUAL ) \
            & declarations.custom_matcher_t( lambda decl: declarations.is_reference( decl.return_type ) )
    try:            
        main_ns.calldefs( query ).virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
        pass
               
def AutoInclude( mb ):
    main_ns = mb.global_ns  ##   doesn't have it's own namespace..
    
    for c in main_ns.classes():
        if ( c.name.startswith ('Theora')
            or c.name.startswith ('OgreVideo')
            ):
            c.include()
            print "Including:", c
            
#     for cls in main_ns.classes():
#         try:
#             if  cls.decl_string[2:4]=='Nx' and cls.decl_string[4].isupper():
#                 cls.include()
#         except:
#             pass
#      ## and we'll need the free functions as well
#     for funcs in main_ns.free_functions ():
#         if funcs.name[0:2]=='Nx' and funcs.name[2].isupper():
#             funcs.include()
#             
    for var in main_ns.variables ():
        if var.name.startswith ('Theora'):
             var.include()
    for var in main_ns.typedefs ():
        if var.name.startswith ('Theora'):
             var.include()
    for var in main_ns.enums ():
        if var.name.startswith ('Theora'):
             var.include()
                

    
def AutoFixes ( mb ): 
    """ now we fix a range of things automatically - typically by going through 
    the entire name space trying to guess stuff and fix it:)
    """       
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    
    # arguments passed as refs but not const are not liked by boost
    #Fix_Ref_Not_Const ( main_ns )
    
    # Functions that have void pointers in their argument list need to change to unsigned int's  
    Fix_Void_Ptr_Args  ( main_ns )
    
    # and change functions that return a variety of pointers to instead return unsigned int's
    Fix_Pointer_Returns ( main_ns )   

    # functions that need to have implicit conversions turned off
    Fix_Implicit_Conversions ( main_ns)
    
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
        
        
def Fix_Implicit_Conversions ( mb ):
    """By default we disable explicit conversion, however sometimes it makes sense
    """
#     for c in mb.classes():
#         if c.name.endswith ('Params'):
#             print "Implicit Conversion:", c
#             c.constructors().allow_implicit_conversion = True
#             
#     ImplicitClasses=['::NxOgre::Pose'] 
#     for className in ImplicitClasses:
#         mb.class_(className).constructors().allow_implicit_conversion = True
                    
def Add_Auto_Conversions( mb ):
    pass
    
      
def Set_Call_Policies( mb ):
    """ set the return call policies on classes that this hasn't already been done for.
    Set the default policy to deal with pointer/reference return types to reference_existing object
    """
    mem_funs = mb.calldefs ()
    mem_funs.create_with_signature = True #Generated code will not compile on
    #MSVC 7.1 if function has throw modifier.
    for mem_fun in mem_funs:
        if mem_fun.call_policies:
            continue
        if not mem_fun.call_policies and \
                    (declarations.is_reference (mem_fun.return_type) or declarations.is_pointer (mem_fun.return_type) ):
            mem_fun.call_policies = call_policies.return_value_policy(
                call_policies.reference_existing_object )

                                
def Set_Exception(mb):
    pass
    #~ """We don't exclude  Exception, because it contains functionality, that could
    #~ be useful to user. But, we will provide automatic exception translator
    #~ """
    #~ Exception = mb.namespace( 'Ogre' ).class_( 'Exception' )
    #~ Exception.include()
    #~ Exception.mem_fun('what').exclude() # declared with empty throw
    #~ Exception.mem_fun('getNumber').exclude() # declared with empty throw
    #~ Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getFullDescription().c_str()' )
            
    
def _ReturnUnsignedInt( type_ ):
    """helper to return an UnsignedInt call for tranformation functions
    """
    return declarations.cpptypes.unsigned_int_t()
    
def Fix_Void_Ptr_Args ( mb ):
    """ we modify functions that take void *'s in their argument list to instead take
    unsigned ints, which allows us to use CTypes buffers
    """
    for fun in mb.member_functions():
        arg_position = 0
        for arg in fun.arguments:
            if declarations.type_traits.is_void_pointer(arg.type):
                fun.add_transformation( ft.modify_type(arg_position,_ReturnUnsignedInt ) )
                fun.documentation = docit ("Modified Input Argument to work with CTypes",
                                            "Argument "+arg.name+ "(pos:" + str(arg_position)\
                                            +") takes a CTypes.adddressof(xx)", "...")
                #print "Fixed Void Ptr", fun, arg_position
                break
            arg_position +=1
            
   ## lets go and look for stuff that might be a problem        
    pointee_types=['unsigned int',' int ', ' float ', ' Real ', 'uchar', 'uint8',
             'unsigned char']
                          
    function_names=[]
    for fun in mb.member_functions():
        if fun.documentation or fun.ignore: continue ## means it's been tweaked somewhere else
        for n in function_names:
            if n in fun.name:
                print "CHECK :", fun
                break
        arg_position = 0
        for arg in fun.arguments:
            if declarations.is_pointer(arg.type): ## and "const" not in arg.type.decl_string:
                for i in pointee_types:
                    if i in arg.type.decl_string:
                        print '"',arg.type.decl_string, '"'
                        print "CHECK ", fun, str(arg_position)
                        fun.documentation=docit("SUSPECT - MAYBE BROKEN", "....", "...")
                        break
            arg_position +=1

## NEED To do the same for constructors
    for fun in mb.constructors():
        arg_position = 0
        for arg in fun.arguments:
            if declarations.is_pointer(arg.type): ## and "const" not in arg.type.decl_string:
                for i in pointee_types:
                    if i in arg.type.decl_string:
                        print '"',arg.type.decl_string, '"'
                        print "Excluding: ", fun
                        fun.exclude()
                        break
            arg_position +=1         
                    
def Fix_Pointer_Returns ( mb ):
    """ Change out functions that return a variety of pointer to base types and instead
    have them return the address the pointer is pointing to (the pointer value)
    This allow us to use CTypes to handle in memory buffers from Python
    
    Also - if documentation has been set then ignore the class/function as it means it's been tweaked else where
    """
    pointee_types=['unsigned int','int', 'float', 'unsigned char']
    known_names=[]  # these are function names we know it's cool to exclude
    for fun in mb.member_functions(allow_empty=True):
        if declarations.is_pointer (fun.return_type) and not fun.documentation:
            for i in pointee_types:
                if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                    if not fun.name in known_names:
                        print "Excluding (function):", fun, "as it returns (pointer)", i
                    fun.exclude()
    for fun in mb.member_operators(allow_empty=True):
        if declarations.is_pointer (fun.return_type) and not fun.documentation:
            for i in pointee_types:
                if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                    print "Excluding (operator):", fun
                    fun.exclude()



def query_containers_with_ptrs(decl):
    if not isinstance( decl, declarations.class_types ):
       return False
    if not decl.indexing_suite:
       return False
    return declarations.is_pointer( decl.indexing_suite.element_type )

    
def Remove_Static_Consts ( mb ):
    """ linux users have compile problems with vars that are static consts AND have values set in the .h files
    we can simply leave these out """
    checker = varchecker.var_checker()
    for var in mb.vars():
        if type(var.type) == declarations.cpptypes.const_t:
            if checker( var ):
                print "Excluding static const ", var
                var.exclude()    

#
# the 'main'function
#            
def generate_code():  
#     messages.disable( 
# #           Warnings 1020 - 1031 are all about why Py++ generates wrapper for class X
#           messages.W1020
#         , messages.W1021
#         , messages.W1022
#         , messages.W1023
#         , messages.W1024
#         , messages.W1025
#         , messages.W1026
#         , messages.W1027
#         , messages.W1028
#         , messages.W1029
#         , messages.W1030
#         , messages.W1031
#         , messages.W1035
#         , messages.W1040 
#         , messages.W1038 
#         , messages.W1039       
#         , messages.W1041
#         , messages.W1036 # pointer to Python immutable member
#         , messages.W1033 # unnamed variables
#         , messages.W1018 # expose unnamed classes
#         , messages.W1049 # returns reference to local variable
#         , messages.W1014 # unsupported '=' operator
#          )
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.ogrevideo.root_dir, "python_theora.h" )
                        , environment.ogrevideo.cache_file )

    defined_symbols = [ 'THEORAVIDEO_PLUGIN_EXPORTS','OGRE_NONCLIENT_BUILD', 'OGRE_GCC_VISIBILITY','__PYTHONOGRE_BUILD_CODE']
    if os.name == 'nt':
        defined_symbols.append ( 'WIN32' )
    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        defined_symbols.append('BOOST_HAS_WINTHREADS')
    defined_symbols.append( 'VERSION_' + environment.ogrevideo.version )  
    
    undefined_symbols = []
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ogrevideo.include_dirs
                                          , define_symbols=defined_symbols
# #                                           , undefine_symbols = undefined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogre.cflags
                                           )
    # if this module depends on another set it here                                           
    mb.register_module_dependency ( environment.ogre.generated_dir )

    # NOTE THE CHANGE HERE                                           
    mb.constructors().allow_implicit_conversion = False                                           
    
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
    
        
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    main_ns.exclude()   ## Note we exclude so we don't get the ogre stuff
    
   
    AutoExclude ( mb )
    AutoInclude ( mb )
    ManualInclude ( mb )
    ManualExclude ( mb )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    
    AutoFixes ( mb )
    ManualFixes ( mb )
            
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    Set_Call_Policies ( mb.global_ns.namespace (MAIN_NAMESPACE) )
    c = main_ns.class_('::Ogre::ExternalTextureSource' ).include(already_exposed=True)
        
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    NoPropClasses = [""]
    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            
    ## add additional version information to the module to help identify it correctly 
    common_utils.addDetailVersion ( mb, environment, environment.ogrevideo )
    
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor("")  
    mb.build_code_creator (module_name='_ogrevideo_' , doc_extractor= extractor )
    
    for inc in environment.ogrevideo.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ogrevideo.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.ogrevideo.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.ogrevideo.version ) )

    mb.split_module(environment.ogrevideo.generated_dir, huge_classes,use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generaated directory..
    if AUDIO:
       additional_files=[
               os.path.join( environment.Config.PATH_DEMO_ogrevideo, 'OpenAL_AudioInterface.h'),
               os.path.join( environment.Config.PATH_DEMO_ogrevideo, 'OpenAL_AudioInterface.cpp'),
               ]            
       for sourcefile in additional_files:
           p,filename = os.path.split(sourcefile)
           destfile = os.path.join(environment.ogrevideo.generated_dir, filename ) 
       
           if not common_utils.samefile( sourcefile ,destfile ):
               shutil.copy( sourcefile, environment.ogrevideo.generated_dir )
               print "Updated ", filename, "as it was missing or out of date"
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
