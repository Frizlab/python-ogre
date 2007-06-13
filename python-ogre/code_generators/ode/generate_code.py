#!/usr/bin/env python

#-------------------------------------------------------------------------------
# TODO:
# 1.    void* as a function argument - they are currently wrapped (and compile/load etc) due to latest CVS of boost.
#       However probably don't actually work
# 2.    Properties.py and calling 'properties.create' - commented out at the moment, not sure if it is really needed?


# # /* these types are mainly just used in headers */
# # typedef dReal dVector3[4];
# # typedef dReal dVector4[4];
# # typedef dReal dMatrix3[4*3];
# # typedef dReal dMatrix4[4*4];
# # typedef dReal dMatrix6[8*6];
# # typedef dReal dQuaternion[4];

## stuff to handle
## arguments named "result" with a type of dVector3 or dVector4 changed to an output of 3 or 4 reals
## any other dVector3 (or any thing in the list above define as an in/out

## wonder if we should just make everything an in/out
## PROBLEM:  Current array function transformers can only be in or out, not both...
## so lets set anything named result to output, all others to input..


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

from pygccxml import parser
from pygccxml import declarations
from pyplusplus import messages
from pyplusplus import module_builder
from pyplusplus import decl_wrappers

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
from pyplusplus.module_creator import call_policies_resolver
from pyplusplus.module_creator import sort_algorithms

from pyplusplus import code_repository

import common_utils.extract_documentation as exdoc
import common_utils.var_checker as varchecker
import defination_finder as df

def AutoArrayArgs( mb ):
    """ Fix Functions that return a pointer to a float structure..  Challenge is that all of the 
    sensible names (dVector3, dMatrix4) are typedefs to dReal (a float *) so in the gcc xml file
    we lose the names and so don't know what the real stuctures are.  
    
    So we use an external function in "defination_finfer.py" to go back to the header files for
    each function and determine what the structure really is.
    
    We also don't know if these structures are for input or output or both to the function -- so
    we look for a variable called 'result' as treat it as an output, everything else as an input
    
    Need to get a mod to Py++ to support in/out
    """
    
    defcheck = df.def_finder()
    names={'dVector3':4, 'dVector3':4, 'dMatrix3':12,'dMatrix4':16, 'dMatrix6':48, 'dQuaternion':4}
    for fun in mb.global_ns.calldefs():
        if len(fun.name) < 2 or "std::" in fun.decl_string:
            continue
        if fun.name[0] != 'd' or (fun.name[1] != fun.name[1].upper()):
            continue
# #         print "CHECKING", fun
        transforms=[]       ## we collect the transformations for a function and execute in one go
        desc=""
        arg_position = 0
        for arg in fun.arguments:
            if arg.name=='result':  # it's an output
                originalDef = defcheck(fun, arg_position)  ## here is where we go and get the real def for the arg
                for n in names:
                    if n in originalDef:
                        transforms.append(ft.output_static_array(arg_position, size=names[n]) )
                        desc=desc+"Transforming "+ arg.name+ " pos("+str(arg_position)+\
                             ") as an OUTPUT array with "+ str(names[n])+ " elements"
                        break
            else:
# #                 print "Arg Pos", arg_position, arg.name
                originalDef = defcheck(fun, arg_position)  ## here is where we go and get the real def for the arg
                for n in names:
                    if n in originalDef:
                        transforms.append(ft.input_static_array(arg_position, size=names[n]) )
                        desc=desc+"Transforming "+ arg.name+ " pos("+str(arg_position)+\
                             ") as an INPUT array with "+ str(names[n])+ "elements"
                        break
            arg_position += 1
        if transforms:
            fun.add_transformation( *transforms )
            fun.documentation = "Python-Ogre: Function Array Modified\\n\\\n" + desc

def ReturnReals( mb ):
    """ many functions return dReal * which we need to make tuples
    however we don't relaly know how big to make the tuple so we use a dictionary
    to determine the size based upon the function name
    """
    names = {'Rotation':12, 'Position':3, 'Quaternion':4, 'Vel':3, 'Force':3, 'Torque':3 }

    for fun in mb.global_ns.calldefs():
        if fun.return_type:
            if "::dReal const *" == fun.return_type.decl_string:  ## OK we need to process it
                for n in names:
                    if fun.name.endswith (n):
# #                         print "Fixing return on",fun.name
                        fun.call_policies = call_policies.convert_array_to_tuple( 
                                            names[n], call_policies.memory_managers.none )
                        fun.include()
                        break

            
def filter_declarations( mb ):
    global_ns = mb.global_ns
    global_ns.exclude()
    
    #
    # Deal with Opcode first
    #
    opcode_ns = global_ns.namespace( 'Opcode' )
    opcode_ns.include()
    
    
    
    
    # keep it simple to start with and don't try for Ice
#     icecore_ns = global_ns.namespace( 'IceCore' )
#     icecore_ns.include()
#     icemaths_ns = global_ns.namespace( 'IceMaths' )
#     icemaths_ns.include()
    
    ode_ns = global_ns  ##  Ode doesn't have it's own namespace..  .namespace( 'ode' )
    for cls in ode_ns.classes():
# #         print "Checking ", cls.decl_string
        try:
            if  cls.decl_string[2]=='d' and cls.decl_string[3].isupper():
# #                 print "Including Class:", cls.name
                cls.include()
        except:
            pass
    ## and the dxXXclasses        
    for cls in ode_ns.classes():
# #         print "Checking ", cls.decl_string
        if  cls.decl_string[2:4]=='dx' and cls.decl_string[4].isupper():
# #             print "Including dxClass:", cls.name
            cls.include()
     ## and we'll need the free functions as well
    for funcs in ode_ns.free_functions ():
# #         print "FREE Func:", funcs.name
        if funcs.name[0]=='d' and funcs.name[1].isupper():
# #             print "Including Function", funcs.name
            funcs.include()
            
    for var in ode_ns.variables ():
# #         print "Checking Variable:", var.name
        if len(var.name) > 2:
            if var.name[0]=='d' and var.name[1].isupper():
# #                 print "Including variable", var.name
                var.include()
    for var in ode_ns.typedefs ():
# #         print "Checking typedef:", var.name
        if len(var.name) > 2:
            if var.name[0]=='d' and var.name[1].isupper():
# #                 print "Including typedef", var.name
                var.include()                
#         print "Member Func:", funcs.name
#         if funcs.name[0]=='d':
#             print "Including Member Function", funcs.name
#             funcs.include()

## these either don't exist in the source or have strange arguments
    ignore=(  "dGeomGetBodyNext", "dGeomMoved", "dPrintMatrix",
        "dWorldGetAutoDisableAngularAverageThreshold",
        "dWorldGetAutoDisableLinearAverageThreshold", 
        "dWorldSetAutoDisableAngularAverageThreshold",
        "dWorldSetAutoDisableLinearAverageThreshold"     )
    for cls in ignore:
        try:
            ode_ns.free_function(cls).exclude()
        except:
            pass
       
    # #     
    # in hand wrappers to handle pyobjects...
    ode_ns.class_( "dGeom" ).member_functions( "getData").exclude()
    ode_ns.class_( "dGeom" ).member_functions( "setData").exclude()
    ode_ns.class_( "dBody" ).member_functions( "setData").exclude()
    ode_ns.class_( "dBody" ).member_functions( "getData").exclude()
    
    ode_ns.class_( "dSpace" ).member_functions( "collide").exclude()
    
         

    ## Exclude protected and private that are not pure virtual
    ### need to be careful here as it removes free functions
    query = ( declarations.access_type_matcher_t( 'private' ) | declarations.access_type_matcher_t( 'protected' ) )\
            & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
    non_public_non_pure_virtual = ode_ns.calldefs( query )
# #     print "TO EXCLUDE:", non_public_non_pure_virtual
    non_public_non_pure_virtual.exclude()
    
    #For some reason Py++ does not honor call policies in this case.
    #You will have to expose them by hand
    dContactGeom = ode_ns.class_( 'dContactGeom' )
    g12 = dContactGeom.variables( lambda d: d.name in ('g1', 'g2' ) )
    g12.exclude()
    #g12.getter_call_policies = call_policies.return_value_policy( call_policies.return_opaque_pointer )

    
    excludes=["::Opcode::AABBQuantizedNoLeafTree::Walk", "::Opcode::AABBQuantizedTree::Walk",
                "::Opcode::AABBNoLeafTree::Walk", "::Opcode::AABBCollisionTree::Walk","::Opcode::AABBTree::Walk",
                "::Opcode::AABBOptimizedTree::Walk", "::Opcode::SAP_PairData::DumpPairs",
                ]
    for e in excludes:
        global_ns.member_functions(e).exclude()
    excludes = ["::Opcode::BruteForceBipartiteBoxTest","::Opcode::BipartiteBoxPruning"]
    for e in excludes:
        global_ns.free_functions(e).exclude()
    
    global_ns.class_("::Opcode::AABBOptimizedTree").exclude()
    
    
def Set_Call_Policies( mb ):
    """ set the return call policies on classes that this hasn't already been done for.
    Set the default policy to deal with pointer/reference return types to reference_existing object
    """
    ode_ns = mb.global_ns   ###  Again, no sperate namespace  .namespace ('ode')
    mem_funs = ode_ns.calldefs ()
    mem_funs.create_with_signature = True #Generated code will not compile on
    #MSVC 7.1 if function has throw modifier.
    resolver = call_policies_resolver.built_in_resolver_t()
    for mem_fun in mem_funs:
        if mem_fun.call_policies:
            continue
        decl_call_policies = resolver( mem_fun )
        if decl_call_policies:
            mem_fun.call_policies = decl_call_policies
            continue
        if not mem_fun.call_policies and \
                    (declarations.is_reference (mem_fun.return_type) or declarations.is_pointer (mem_fun.return_type) ):
            mem_fun.call_policies \
               = call_policies.return_value_policy( '::boost::python::return_pointee_value' )
 

def generate_code():
    messages.disable( 
#           Warnings 1020 - 1031 are all about why Py++ generates wrapper for class X
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
        , messages.W1035
        , messages.W1040 
        , messages.W1038        
        , messages.W1041
        , messages.W1036 # pointer to Python immutable member
        , messages.W1033 # unnamed variables
        , messages.W1018 # expose unnamed classes
        , messages.W1049 # returns reference to local variable
        , messages.W1014 # unsupported '=' operator
         )
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.ode.root_dir, "python_ode.h" )
                        , environment.ode.cache_file )

    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ode.include_dirs
                                          , define_symbols=['ode_NONCLIENT_BUILD', 'ODE_LIB',
                                          'ODE_DLL', 'OPCODE_EXPORTS', 'ICECORE_EXPORTS']
#                                          , start_with_declarations=['ode']
                                          , indexing_suite_version=2 )

    filter_declarations (mb)
    
    query = lambda decl: isinstance( decl, ( declarations.class_t, declarations.class_declaration_t ) ) \
                         and decl.name.startswith( 'dx' )
    mb.global_ns.decls( query ).opaque = True   
#     mb.global_ns.variable('dSpaceID').opaque=True
                          
#     ## here we adjust for functions that return poiners to ODE "ID's", which are really C structs
#     ## I may have been over agressive in identifing these functions but hopefully not...
#     for func in mb.namespace( 'OgreOde' ).member_functions():  
#         if func.return_type.decl_string.endswith('ID'):
#             print "Setting ", func.name, "to Opaque"
#             func.opaque = True
#             func.call_policies = call_policies.return_value_policy(
#                 call_policies.return_opaque_pointer )
        


    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
    
    Set_Call_Policies (mb)
    AutoArrayArgs( mb )
    ReturnReals( mb )
    hand_made_wrappers.apply( mb )

    ode_ns = mb.global_ns  ## .namespace ('ode')
    common_utils.add_properties( ode_ns.classes() )

    common_utils.add_constants( mb, { 'ode_version' :  '"%s"' % environment.ode.version
                                      , 'python_version' : '"%s"' % sys.version.replace("\n", "\\\n") } )

 
    #Creating code creator. After this step you should not modify/customize declarations.
    extractor = exdoc.doc_extractor("")
    mb.build_code_creator (module_name='_ode_' , doc_extractor= extractor)
    
    for inc in environment.ode.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ode.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files(environment.ode.version) )
    
# #     mb.code_creator.add_include( "__convenience.pypp.hpp" )
# #     mb.code_creator.add_system_header( code_repository.convenience.file_name )

    huge_classes = map( mb.class_, customization_data.huge_classes(environment.ode.version) )

    mb.split_module(environment.ode.generated_dir, huge_classes)

    return_pointee_value_source_path \
        = os.path.join( environment.pyplusplus_install_dir
                        , 'pyplusplus_dev'
                        , 'pyplusplus'
                        , 'code_repository'
                        , 'return_pointee_value.hpp' )

    return_pointee_value_target_path \
        = os.path.join( environment.ode.generated_dir, 'return_pointee_value.hpp' )

    if not os.path.exists( return_pointee_value_target_path ):
        shutil.copy( return_pointee_value_source_path, environment.ode.generated_dir )


if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Python-ode source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
