#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-OgreDshow and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------


import os, sys, time, shutil

#add environment to the path
sys.path.append( os.path.join( '..', '..' ) )
sys.path.append ( 'c:/development/pywork/code_generators')

import environment
import hand_made_wrappers
import common_utils
import common_utils.extract_documentation as exdoc
import customization_data

from pygccxml import parser
from pygccxml import declarations
from pyplusplus import messages
from pyplusplus import module_builder
from pyplusplus import decl_wrappers

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
from pyplusplus.module_creator import sort_algorithms

## small helper function
def docit ( general, i, o ): 
    docs = "Python-OgreDshow Modified Function Call\\n" + general +"\\n"
    docs = docs + "Input: " + i + "\\n"
    docs = docs + "Output: " + o + "\\n\\\n"
    return docs

############################################################
##
##  Here is where we manually exclude stuff from Ogre - 
##
############################################################

def ManualExclude ( mb ):
    global_ns = mb.global_ns
    OgreDshow_ns = global_ns.namespace( 'OgreUtils' )
    ## This does nothing - RenderSystemList.xx still gets created
    ##global_ns.namespace('std').class_('vector<Ogre::RenderSystem*, std::allocator<Ogre::RenderSystem*> >').exclude()

############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################
    
def ManualInclude ( mb ):
    global_ns = mb.global_ns
    OgreDshow_ns = global_ns.namespace( 'OgreUtils' )
    
    # now for classes in Ogre that are referenced
# #     for c in global_ns.classes():
# #         print "Class:",c
# #         print "alais", c.alias
# #         print c.name
# #     print dir (  global_ns.classes() )   
    global_ns.namespace( 'Ogre' ).class_('Vector2').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('TexturePtr').include(already_exposed=True)
#     global_ns.namespace( 'Ogre' ).class_('Plugin').include(already_exposed=True)
#     global_ns.namespace( 'Ogre' ).class_('FrameEvent').include(already_exposed=True)
#     global_ns.namespace( 'Ogre' ).class_('FrameListener').include(already_exposed=True)
    
    ## AJM if i do this next line then a "complete" RenderSystemList.pypp.cpp is created
#    global_ns.namespace('std').class_('vector<Ogre::RenderSystem*, std::allocator<Ogre::RenderSystem*> >').include(already_exposed=True)

    
   

        
############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixes ( mb ):    

    global_ns = mb.global_ns
    ar_ns = global_ns.namespace( 'OgreUtils' )
    
    ## TO FIX
#     x = ar_ns.class_('TrackerBase').mem_fun('setOgreRoot')
#     x.call_policies = call_policies.return_value_policy(
#                 call_policies.with_custodian_and_ward )
    
             
############################################################
##
##  And things that need to have their argument and call values fixed.
##  ie functions that pass pointers in the argument list and of course we need
##  to read the updated values - so instead we pass them back 
##  as new values in a tuple (ETC ETC)
##
############################################################
        
def ManualTransformations ( mb ):
    ar_ns = mb.global_ns.namespace ('OgreUtils')
        
    def create_output( size ):
        return [ ft.output( i ) for i in range( size ) ]
        
#     x = ar_ns.class_('TrackerBase').mem_fun('queryMarker')
#     x.add_transformation(ft.output('bVisible'))
#     x.documentation = docit ("**** Assume bVisible is always an 'output' variable","id, transform", "bVisible")
#     
#     x = ar_ns.class_('BCHTracker').mem_fun('queryMarker')
#     x.add_transformation(ft.output('bVisible'))
#     x.documentation = docit ("**** Assume bVisible is always an 'output' variable","id, transform", "bVisible")

#     x = ar_ns.class_('TestTracker').mem_fun('queryMarker')
#     x.add_transformation(ft.output('bVisible'))
#     x.documentation = docit ("**** Assume bVisible is always an 'output' variable","id, transform", "bVisible")

###############################################################################
##
##  Now for the AUTOMATIC stuff that should just work with all versions of Ogre
##
###############################################################################
    
    
def AutoExclude( mb ):
    """ Automaticaly exclude a range of things that don't convert well from C++ to Python
    """
    global_ns = mb.global_ns
    OgreDshow_ns = global_ns.namespace( 'OgreUtils' )
    
    ## Exclude protected and private that are not pure virtual
    query = ~declarations.access_type_matcher_t( 'public' ) \
            & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
    non_public_non_pure_virtual = OgreDshow_ns.calldefs( query )
    non_public_non_pure_virtual.exclude()

            
def AutoInclude( mb ):
    pass

    
def AutoFixes ( mb ): 
    """ now we fix a range of things automatically - typically by going through 
    the entire name space trying to guess stuff and fix it:)
    """       
    global_ns = mb.global_ns
    OgreDshow_ns = global_ns.namespace( 'OgreUtils' )
    
# # #     # arguments passed as refs but not const are not liked by boost
# # #     Fix_Ref_Not_Const ( OgreDshow_ns )
# # #     
# # #     # Functions that have void pointers in their argument list need to change to unsigned int's  
# # #     Fix_Void_Ptr_Args  ( OgreDshow_ns )
# # #     
# # #     # Allow conversion between Vectors/Colourvalue etc and Python lists      
# # #     Add_Auto_Conversions( mb )
# # #     
# # #     # now we fix up the smart pointers ...
# # #     Set_Smart_Pointers ( OgreDshow_ns )  
# # #     
# # #     # and change functions that return a variety of pointers to instead return unsigned int's
# # #     Fix_Pointer_Returns ( OgreDshow_ns )   

    # functions that need to have implicit conversions turned off
# # #     Fix_Implicit_Conversions ( OgreDshow_ns)
# # #     
# # #     if os.name =='nt':
# # #         Fix_NT( mb )
# # #     elif os.name =='posix':
# # #         Fix_Posix( mb )
        
 
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
    """Some of the implicit conversion gets a little too smart and causes strange problems
    """
    nonImplicitClasses=['TestTracker','BCHTracker']
    for className in nonImplicitClasses:
        mb.class_(className).constructors().allow_implicit_conversion = False
    
    
def Fix_Ref_Not_Const ( mb ):
    """ we have problems with sharedpointer arguments that are defined as references
    but are NOT const.  Boost doesn't understand how to match them and you get a C++ Signature match fails.
    In reality the Ogre code probably needs to be patched as all of these should (??) be const.  However we'll fix it 
    with a function transformation wrapper
    """
    for fun in mb.member_functions( ):
        arg_position = 0
        for arg in fun.arguments:
            if 'Ptr' in arg.type.decl_string:
                 if not 'const' in arg.type.decl_string and '&' in arg.type.decl_string:
                    #print "Fixing Const", fun.parent.name,"::", fun.name, "::", arg_position
                    fun.add_transformation( ft.modify_type(arg_position,declarations.remove_reference ) )
            arg_position +=1
                    
    
      
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
    """We don't exclude  Exception, because it contains functionality, that could
    be useful to user. But, we will provide automatic exception translator
    """
    pass
#     Exception = mb.namespace( 'OgreUtils' ).class_( 'Exception' )
#     Exception.include()
#     Exception.mem_fun('what').exclude() # declared with empty throw
#     Exception.mem_fun('getNumber').exclude() # declared with empty throw
#     Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getFullDescription().c_str()' )
    
       
    
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
            

            
                    
def Fix_Pointer_Returns ( mb ):
    """ Change out functions that return a variety of pointer to base types and instead
    have them return the address the pointer is pointing to (the pointer value)
    This allow us to use CTypes to handle in memory buffers from Python
    
    Also - if documentation has been set then ignore the class/function as it means it's been tweaked else where
    """
    return
    pointee_types=['unsigned int','int', 'float', '::Ogre::Real', '::Ogre::uchar', '::Ogre::uint8', 'unsigned char']
    known_names=['ptr', 'useCountPointer']  # these are function names we know it's cool to exclude
    for fun in mb.member_functions():
        if declarations.is_pointer (fun.return_type) and not fun.documentation:
            for i in pointee_types:
                if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                    if not fun.name in known_names:
                        print "Excluding (function):", fun, "as it returns (pointer)", i
                    fun.exclude()
    for fun in mb.member_operators():
        if declarations.is_pointer (fun.return_type) and not fun.documentation:
            for i in pointee_types:
                if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                    print "Excluding (operator):", fun
                    fun.exclude()

    
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
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.ogredshow.root_dir, "python_ogredshow.h" )
                        , environment.ogredshow.cache_file )

    defined_symbols = [ 'ogredshow_NONCLIENT_BUILD' ]
    defined_symbols.append( 'ogredshow_VERSION_' + environment.ogredshow.version ) 
    defined_symbols.append ( 'OGRE_NONCLIENT_BUILD' )
    
    
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ogredshow.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogredshow.cflags
                                           )
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
    
        
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()
    OgreDshow_ns = global_ns.namespace( 'OgreUtils' )
    OgreDshow_ns.include()
        
    AutoExclude ( mb )
    ManualExclude ( mb )
    AutoInclude ( mb )
    ManualInclude ( mb )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    
    AutoFixes ( mb )
    ManualFixes ( mb )
    
  
    Set_Exception( mb )
        
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    Set_Call_Policies ( mb.global_ns.namespace ('OgreUtils') )
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    common_utils.add_constants( mb, { 'ogredhow_version' :  '"%s"' % environment.ogredshow.version.replace("\n", "\\\n") 
                                      , 'python_version' : '"%s"' % sys.version.replace("\n", "\\\n" ) } )
                                      
    ## need to create a welcome doc string for this...                                  
    common_utils.add_constants( mb, { '__doc__' :  '"Python-OgreDshow Wrapper Library"' } ) 
# #     mb.global_ns.class_('vector<Ogre::RenderSystem*, std::allocator<Ogre::RenderSystem*> >').exclude()
    
    
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor("")
    mb.build_code_creator (module_name='_ogredshow_' , doc_extractor= extractor)
    
    for inc in environment.ogredshow.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ogredshow.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.ogredshow.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.ogredshow.version ) )

    mb.split_module(environment.ogredshow.generated_dir , huge_classes)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generaated directory..
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Python-OgreDshow source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
