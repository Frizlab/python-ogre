#!/usr/bin/env python

#-------------------------------------------------------------------------------
# TODO:
# 1.    void* as a function argument - they are currently wrapped (and compile/load etc) due to latest CVS of boost.
#       However probably don't actually work
# 2.    Properties.py and calling 'properties.create' - commented out at the moment, not sure if it is really needed?

import os, sys, time

#add environment to the path
sys.path.append( os.path.join( '..', '..' ) )
#add common utils to the pass
sys.path.append( '..' )
sys.path.append( '.' )

import shutil

import environment
import common_utils
import customization_data
import hand_made_wrappers

from pyplusplus import module_builder
from pyplusplus import module_creator
from pyplusplus.module_builder import call_policies

from pygccxml import parser
from pygccxml import declarations


def filter_declarations( mb ):
    global_ns = mb.global_ns
    global_ns.exclude()
    ogrenewt_ns = global_ns.namespace( 'BasicJoints' )
    ogrenewt_ns.include()
    ogrenewt_ns = global_ns.namespace( 'PrebuiltCustomJoints' )
    ogrenewt_ns.include()
    ogrenewt_ns = global_ns.namespace( 'CollisionPrimitives' )
    ogrenewt_ns.include()
#     
#     ogrenewt_ns = global_ns.namespace( 'OgreNewt' )
#     
    temp_ns = global_ns.namespace( 'Converters' )
    temp_ns.include()
    temp_ns = global_ns.namespace( 'CollisionTools' )
    temp_ns.include()
    temp_ns = global_ns.namespace( 'MomentOfInertia' )
    temp_ns.include()
    
    
    ogrenewt_ns = global_ns.namespace( 'OgreNewt' )
    ogrenewt_ns.include()

    
    ## these need to be excluded due to callback functions - need to wrap them
    ogrenewt_ns.class_( "World" ).member_functions("setLeaveWorldCallback").exclude()
    ogrenewt_ns.class_( "Body" ).member_functions("addBouyancyForce").exclude()
    ogrenewt_ns.class_( "Body" ).member_functions("setAutoactiveCallback").exclude()
    ogrenewt_ns.class_( "Body" ).member_functions("setCustomForceAndTorqueCallback").exclude()
    ogrenewt_ns.class_( "Body" ).member_functions("setCustomTransformCallback").exclude()
#    ogrenewt_ns.class_( "Body" ).member_functions("setStandardForceCallback").exclude()
    ogrenewt_ns.class_( "Debugger" ).member_functions("getSingleton").exclude()
    ogrenewt_ns.class_( "BodyIterator" ).member_functions("getSingleton").exclude()

     
    
#     ptr_to_fundamental_query \
#         = lambda f: declarations.is_pointer( f.return_type ) \
#                     and declarations.is_fundamental( declarations.remove_pointer( f.return_type ) )
                    
#     ogrenewt_ns.calldefs( ptr_to_fundamental_query ).exclude()

### and we need the free functions - mainly fro
    for func in ogrenewt_ns.free_functions ():
        print "FREE Func:", func.name
        func.include()
#         if func.name[0]=='d':
#             print "Including Function", func.name
#             func.include()
  
            
    ## Exclude protected and private that are not pure virtual
#     query = ~declarations.access_type_matcher_t( 'public' ) \
#             & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
#     non_public_non_pure_virtual = ogrenewt_ns.calldefs( query )
#     non_public_non_pure_virtual.exclude()
    
   
def set_call_policies( mb ):
    ogrenewt_ns = mb.global_ns.namespace( 'OgreNewt' )

    # Set the default policy to deal with pointer/reference return types to reference_existing object
    # as this is the ogrenewt Default.
    mem_funs = ogrenewt_ns.calldefs ()
    mem_funs.create_with_signature = True #Generated cogrenewt will not compile on
    #MSVC 7.1 if function has throw modifier.
    resolver = module_creator.built_in_resolver_t()
    for mem_fun in mem_funs:
        if mem_fun.call_policies:
            continue
        decl_call_policies = resolver( mem_fun )
        if decl_call_policies:
            mem_fun.call_policies = decl_call_policies
            continue
        rtype = declarations.remove_alias( mem_fun.return_type )
        if declarations.is_pointer(rtype) or declarations.is_reference(rtype):
#             mem_fun.call_policies \
#                 = call_policies.return_value_policy( call_policies.reference_existing_object )
            mem_fun.call_policies \
               = call_policies.return_value_policy( '::boost::python::return_pointee_value' )


def generate_ogrenewt():
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.ogrenewt.root_dir, "python_ogrenewt.h" )
                        , environment.ogrenewt.cache_file )

    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ogrenewt.include_dir
                                          , define_symbols=['ogrenewt_NONCLIENT_BUILD']
                                          , indexing_suite_version=2 )

    filter_declarations (mb)

#     query = lambda decl: isinstance( decl, ( declarations.class_t, declarations.class_declaration_t ) ) \
#                          and decl.name.startswith( 'dx' )
#     mb.global_ns.decls( query ).opaque = True                         
       

    common_utils.set_declaration_aliases( mb.global_ns, customization_data.aliases(environment.ogrenewt.version) )

    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True

    set_call_policies (mb)
    hand_made_wrappers.apply( mb )

    ogrenewt_ns = mb.namespace ('OgreNewt')
    common_utils.add_properties( ogrenewt_ns.classes() )

    common_utils.add_constants( mb, { 'ogrenewt_version' :  '"%s"' % environment.ogrenewt.version
                                      , 'python_version' : '"%s"' % sys.version } )


    #Creating cogrenewt creator. After this step you should not modify/customize declarations.
    mb.build_code_creator (module_name='_ogrenewt_')
    for incs in environment.ogrenewt.include_dir:
        mb.code_creator.user_defined_directories.append( incs )
    mb.code_creator.replace_included_headers( customization_data.header_files(environment.ogrenewt.version) )

    huge_classes = map( mb.class_, customization_data.huge_classes(environment.ogrenewt.version) )

    mb.split_module(environment.ogrenewt.generated_dir, huge_classes)

    return_pointee_value_source_path \
        = os.path.join( environment.pyplusplus_install_dir
                        , 'pyplusplus_dev'
                        , 'pyplusplus'
                        , 'code_repository'
                        , 'return_pointee_value.hpp' )

    return_pointee_value_target_path \
        = os.path.join( environment.ogrenewt.generated_dir, 'return_pointee_value.hpp' )

    if not os.path.exists( return_pointee_value_target_path ):
        shutil.copy( return_pointee_value_source_path, environment.ogrenewt.generated_dir )


if __name__ == '__main__':
    start_time = time.clock()
    generate_ogrenewt()
    print 'Python-ogrenewt source cogrenewt was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
