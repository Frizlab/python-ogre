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
##import register_exceptions

from pygccxml import parser
from pygccxml import declarations
from pyplusplus import messages
from pyplusplus import module_builder
from pyplusplus import decl_wrappers

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
from pyplusplus.module_creator import sort_algorithms

import common_utils.extract_documentation as exdoc
import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties

MAIN_NAMESPACE = 'Opcode'

NAMESPACES = [ 'IceCore', 'Opcode', 'IceMaths']

## small helper function
def docit ( general, i, o ): 
    docs = "Python-Ogre (Opcode) Modified Function Call\\n" + general +"\\n"
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
    excludes=["::Opcode::AABBQuantizedNoLeafTree::Walk", "::Opcode::AABBQuantizedTree::Walk",
                "::Opcode::AABBNoLeafTree::Walk", "::Opcode::AABBCollisionTree::Walk",
                "::Opcode::AABBTree::Walk",## "::Opcode::AABBOptimizedTree::Walk",
                "::Opcode::SAP_PairData::DumpPairs",
                "::Opcode::AABBTree::GetIndices","::Opcode::AABBTreeNode::GetPrimitives",
                "::Opcode::HybridModel::GetIndices","::Opcode::VolumeCollider::GetTouchedPrimitives", 
                # Quat's don't seem to be implemented
                "::IceMaths::Matrix3x3::FromQuat"
                ,"::IceMaths::Matrix3x3::FromQuatL2"
                ## in header but missing in source
                ,"::IceMaths::IndexedTriangle::ComputeCubeIndex"
                ,"::IceMaths::LSS::ComputeOBB"
                ,"::IceMaths::Matrix3x3::RotX"
                ,"::IceMaths::Matrix3x3::RotYX"
                ,"::IceMaths::Matrix3x3::RotY"
                ,"::IceMaths::Matrix3x3::RotZ"
                ,"::IceMaths::Matrix3x3::Rot"
                ,"::IceMaths::Matrix3x3::Normalize"
                ,"::IceMaths::Matrix3x3::Exp"
                ,"::IceMaths::Matrix3x3::FromTo"
                ,"::IceMaths::Point::Transform"
                ,"::IceMaths::Point::Mult2"
                ,"::IceMaths::Point::InvTransform"
                ,"::IceMaths::Point::Transform"
                ,"::IceMaths::Point::TransMult"
                ,"::IceMaths::Point::Unfold"
                ,"::IceMaths::Point::Mult"
                ,"::IceMaths::Point::Mac"
                ,"::IceMaths::Triangle::ComputeMoment"
                ,"::IceMaths::Sphere::Compute"
                ,"::IceMaths::Sphere::FastCompute"
                ,"::IceMaths::Matrix4x4::Rot"
                ]
    for e in excludes:
        print "excluding function", e
        global_ns.member_functions(e).exclude()
    excludes = ["::Opcode::BruteForceBipartiteBoxTest","::Opcode::BipartiteBoxPruning"
                ,"::IceMaths::Normalize1"
                ,"::IceMaths::Normalize2"
                ]
    for e in excludes:
        print "Excluding:", e
        global_ns.free_functions(e).exclude()
    
    excludes = ["::Opcode::AABBOptimizedTree" 
               ### Exclude matrix4x4 as boost/boost/tuple/detail/tuple_basic.hpp needs to be extended to take 16 arguments 
#                ,"::IceMaths::Matrix4x4"
#                ,"::IceMaths::Sphere"
               ]
    for e in excludes:
        global_ns.class_(e).exclude()
        
    excludes = ["::IceMaths::Matrix3x3::m", "::IceMaths::Matrix4x4::m"]
    for e in excludes:
        global_ns.variable(e).exclude()
    

############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################
    
def ManualInclude ( mb ):
    global_ns = mb.global_ns
    
            
############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixes ( mb ):    

    global_ns = mb.global_ns
    icemaths_ns = global_ns.namespace( 'IceMaths' )
# #     c = icemaths_ns.class_('Matrix4x4').operators('[]')
# #     c.call_policies= call_policies.convert_array_to_tuple( 4, call_policies.memory_managers.none )   
# #     c.include() 
# #     c.documentation=docit ("Return Type Change", "None", "Tuple with 4 floats's (the matrix 'line')")
# #     c = icemaths_ns.class_('Matrix3x3').operators('[]')
# #     c.call_policies= call_policies.convert_array_to_tuple( 3, call_policies.memory_managers.none )    
# #     c.include()
# #     c.documentation=docit ("Return Type Change", "None", "Tuple with 3 floats's (the matrix 'line')")

              
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
    for ns in NAMESPACES:
        main_ns = global_ns.namespace( ns )
    
        # vars that are static consts but have their values set in the header file are bad
        Remove_Static_Consts ( main_ns )
        
        ## Exclude protected and private that are not pure virtual
        query = declarations.access_type_matcher_t( 'private' ) \
                & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
        try:
            non_public_non_pure_virtual = main_ns.calldefs( query )
            non_public_non_pure_virtual.exclude()
        except:
            pass
    
        #Virtual functions that return reference could not be overriden from Python
        query = declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.VIRTUAL ) \
                & declarations.custom_matcher_t( lambda decl: declarations.is_reference( decl.return_type ) )
        try:
            main_ns.calldefs( query ).virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
        except:
            pass
               
def AutoInclude( mb ):
    pass

    
def AutoFixes ( mb ): 
    """ now we fix a range of things automatically - typically by going through 
    the entire name space trying to guess stuff and fix it:)
    """       
    global_ns = mb.global_ns
    for ns in NAMESPACES:
        main_ns = global_ns.namespace( ns )
    
        # arguments passed as refs but not const are not liked by boost
    #     Fix_Ref_Not_Const ( main_ns )
        
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
    ImplicitClasses=[] 
    for className in ImplicitClasses:
        mb.class_(className).constructors().allow_implicit_conversion = True
                    
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
            
         
                    
def Fix_Pointer_Returns ( mb ):
    """ Change out functions that return a variety of pointers to base types and instead
    have them return the address the pointer is pointing to (the pointer value)
    This allow us to use CTypes to handle in memory buffers from Python
    
    Also - if documentation has been set then ignore the class/function as it means it's been tweaked else where
    """
    pointee_types=['unsigned int','int', 'float', 'unsigned char', '::udword', '::sbyte' ]
    known_names=[]  # these are function names we know it's cool to exclude
    for fun in mb.member_functions():
        if declarations.is_pointer (fun.return_type) and not fun.documentation:
            for i in pointee_types:
                if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                    if not fun.name in known_names:
                        print "Excluding (function):", fun, "as it returns (pointer)", i
                    fun.exclude()
    try:                
        for fun in mb.member_operators():
            if declarations.is_pointer (fun.return_type) and not fun.documentation:
                for i in pointee_types:
                    if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                        print "Excluding (operator):", fun
                        fun.exclude()
    except:
        pass



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
    messages.disable( 
# #           Warnings 1020 - 1031 are all about why Py++ generates wrapper for class X
          messages.W1020
        , messages.W1021
        , messages.W1022
        , messages.W1023
        , messages.W1024
        , messages.W1025
        , messages.W1026
        , messages.W1027
        , messages.W1028
        , messages.W1029
        , messages.W1030
        , messages.W1031
#         , messages.W1035
#         , messages.W1040 
#         , messages.W1038        
        , messages.W1041
        , messages.W1036 # pointer to Python immutable member
#         , messages.W1033 # unnamed variables
#         , messages.W1018 # expose unnamed classes
        , messages.W1049 # returns reference to local variable
        , messages.W1014 # unsupported '=' operator
         )
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.opcode.root_dir, "python_opcode.h" )
                        , environment.opcode.cache_file )

    defined_symbols = ['OPCODE_EXPORTS'] #, 'OPC_USE_CALLBACKS' ]
    defined_symbols.append( 'VERSION_' + environment.opcode.version )  
    
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.opcode.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogre.cflags
                                           )
    # NOTE THE CHANGE HERE                                           
    mb.constructors().allow_implicit_conversion = False                                           
    
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
    
        
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()
    for ns in NAMESPACES:
        main_ns = global_ns.namespace( ns )
        main_ns.include()
    
   
    AutoExclude ( mb )
    ManualExclude ( mb )
    AutoInclude ( mb )
    ManualInclude ( mb )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    
    AutoFixes ( mb )
    ManualFixes ( mb )
            
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    for ns in NAMESPACES:
        Set_Call_Policies ( mb.global_ns.namespace (ns) )
    
#IceMaths::Quat IceMaths::Matrix3x3::operator ::IceMaths::Quat() const [casting operator]
    c = mb.global_ns.namespace ( 'IceMaths').class_('Matrix3x3')
    for o in c.operators():
        if 'Quat' in o._decl_string():
            o.exclude()
            print "Excluded Op", o, "\n", o._decl_string()
##  OP float const * IceMaths::Point::operator float const *() const [casting operator]
    c = mb.global_ns.namespace ( 'IceMaths').class_('Point')
    for o in c.operators():
        if 'float const *' in o._decl_string():
            o.exclude()
            print "Excluded Op", o, "\n", o._decl_string()
    ##Spere(udword nb_verts, const Point* verts);  Not Implemented
    c = mb.global_ns.namespace ( 'IceMaths').class_('Sphere')
    for o in c.constructors(arg_types=("::udword","::IceMaths::Point const *") ):
        o.exclude()
    c = mb.global_ns.namespace ( 'IceMaths').class_('Matrix4x4')
    for o in c.constructors():
        if len (o.arguments) > 10:
            o.exclude()
    for o in c.member_functions():
        if len (o.arguments) > 10:
            print "Too Many Arguments - excluded:", o, o._decl_string()
            o.exclude()
    c = mb.global_ns.namespace ( 'IceMaths').class_('Matrix4x4')
    for o in c.casting_operators():
        rett = o.return_type.decl_string
        if 'Quat' in rett or 'PR' in rett:
            o.exclude()
            print "Excluded Op", o
             
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    NoPropClasses = [""]
    for ns in NAMESPACES:
        main_ns = global_ns.namespace( ns )
        for cls in main_ns.classes():
            if cls.name not in NoPropClasses:
                cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            
    common_utils.add_constants( mb, { 'opcode_version' :  '"%s"' % environment.opcode.version.replace("\n", "\\\n") 
                                      , 'python_version' : '"%s"' % sys.version.replace("\n", "\\\n" ) } )
                                      
    ## need to create a welcome doc string for this...                                  
    common_utils.add_constants( mb, { '__doc__' :  '"opcode DESCRIPTION"' } ) 
    
    
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor("") # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_opcode_' , doc_extractor= extractor )
    
    for inc in environment.opcode.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.opcode.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.opcode.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.opcode.version ) )

    mb.split_module(environment.opcode.generated_dir, huge_classes)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generaated directory..
#     additional_files=[
#             os.path.join( environment.shared_ptr_dir, 'py_shared_ptr.h'),
#             os.path.join( os.path.abspath(os.path.dirname(__file__) ), 'python_ogre_masterlist.h' ),
#             os.path.join( os.path.abspath(os.path.dirname(__file__) ), 'generators.h' ),
#             os.path.join( os.path.abspath(os.path.dirname(__file__) ), 'custom_rvalue.cpp' ),
#             os.path.join( environment.include_dir, 'tuples.hpp' )
#             ]            
#     for sourcefile in additional_files:
#         p,filename = os.path.split(sourcefile)
#         destfile = os.path.join(environment.ogre.generated_dir, filename ) 
#     
#         if not common_utils.samefile( sourcefile ,destfile ):
#             shutil.copy( sourcefile, environment.ogre.generated_dir )
#             print "Updated ", filename, "as it was missing or out of date"
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
