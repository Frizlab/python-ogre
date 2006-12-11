#!/usr/bin/env python


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
from pyplusplus import module_builder

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies

import ogre_properties

def filter_declarations( mb ):

    global_ns = mb.global_ns
    global_ns.exclude()
    
    ogre_ns = global_ns.namespace( 'Ogre' )
    ogre_ns.include()
    
    startswith = [
        # Don't include, we'll never need.
        'D3D', 'GL', 'WIN32', '_'  
    ]

    if environment.ogre.version == "CVS":
        mb.global_ns.class_( 'vector<Ogre::Vector4, std::allocator<Ogre::Vector4> >' ).exclude( )

    ## Remove private classes , and those that are internal to Ogre...
    private_decls = common_utils.private_decls_t(environment.ogre.include_dirs)
    for cls in ogre_ns.classes():
        if private_decls.is_private( cls ):
            cls.exclude()
            print '{*} class "%s" is marked as private' % cls.decl_string

    #RenderOperation class is marked as private, but I think this is a mistake
    ogre_ns.class_('RenderOperation').include()

    for func in ogre_ns.calldefs():
        if private_decls.is_private( func ):
            if func.virtuality == declarations.VIRTUALITY_TYPES.PURE_VIRTUAL:
                continue
            func.exclude()
            print '{*} function "%s" is marked as internal' % declarations.full_name( func )

    ## Exclude protected and private that are not pure virtual
    query = ~declarations.access_type_matcher_t( 'public' ) \
            & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
    non_public_non_pure_virtual = ogre_ns.calldefs( query )
    non_public_non_pure_virtual.exclude()

    ## Exclude all public not pure virtual, that starts with '_'
    ## AJM - no real reason to exclude these functions - and more to the point we sometimes need them :)
#     query = declarations.access_type_matcher_t( 'public' ) \
#             & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL ) \
#             & declarations.custom_matcher_t( lambda decl: decl.name.startswith( '_' ) )
#     non_public_non_pure_virtual = ogre_ns.calldefs( query )
#     non_public_non_pure_virtual.exclude()


    ## MSVC 7.1 pukes on StaticGeometry so need to make it smaller..
    ogre_ns.class_( "StaticGeometry" ).class_("QueuedSubMesh").exclude()
    ogre_ns.class_( "StaticGeometry" ).class_("QueuedGeometry").exclude()
    ogre_ns.class_( "StaticGeometry" ).class_("SubMeshLodGeometryLink").exclude()
    
    GeomRegion = ogre_ns.class_( "StaticGeometry" ).class_("Region")
    GeomRegion.member_functions('getLights'). exclude() # fails at link time
    

    ## Now get rid of a wide range of classes as defined earlier in startswith...
    for prefix in startswith:
        ### NOTE the PREFIX is used here !!!!
        classes = ogre_ns.classes( common_utils.decl_starts_with(prefix), allow_empty=True)
        classes.exclude()

    #Virtual functions that return reference could not be overriden from Python
    query = declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.VIRTUAL ) \
            & declarations.custom_matcher_t( lambda decl: declarations.is_reference( decl.return_type ) )
    ogre_ns.calldefs( query ).virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL

    #Only usefull from C++
    ogre_ns.class_( "MemoryManager" ).exclude()

    ## AJM Error at compile time - errors when compiling or linking
    ogre_ns.class_( "MemoryDataStream" ).member_functions( 'getCurrentPtr' ).exclude()
    ogre_ns.class_( "MemoryDataStream" ).member_functions( 'getPtr' ).exclude()
    ogre_ns.calldefs ('peekNextPtr').exclude ()
    ogre_ns.calldefs ('peekNextValuePtr').exclude ()    #in many of the Iterator classes
    ogre_ns.class_("GpuProgramParameters").class_("AutoConstantEntry").exclude() # Autoconstant name space variables missing in compile

    ogre_ns.class_( 'RenderSystemOperation' ).exclude() # AJM in OgreCompositorInstance
    ogre_ns.calldefs ('getChildIterator').exclude ()

    #AJM Set of functions in Particle system that don't get wrapped properly.. Rechecked 30Nov06 AJM
    PartSys = ogre_ns.class_( "ParticleSystem" )
    PartSys.class_( "CmdIterationInterval" ).exclude()
    PartSys.class_( "CmdLocalSpace" ).exclude()
    PartSys.class_( "CmdNonvisibleTimeout" ).exclude()
    PartSys.class_( "CmdSorted" ).exclude()
    
    ## AJM Try to force    
    ogre_ns.class_("ParticleAffectorFactory").include()
    ogre_ns.class_("HardwareIndexBufferSharedPtr").include()
    
    
    
    # These members have Ogre::Real * methods which need to be wrapped. # recheck 29/12/06 AJM
    ogre_ns.class_ ('Matrix3').member_operators (symbol='[]').exclude ()
    ogre_ns.class_ ('Matrix4').member_operators (symbol='[]').exclude ()

    #returns reference to "const Real *" # recheck 29/12/06 AJM
    ogre_ns.class_ ('BillboardChain').calldef( 'getOtherTextureCoordRange' ).exclude() 

    #all constructors in this class are private, also some of them are public.
    Skeleton = ogre_ns.class_( 'Skeleton' ).constructors().exclude()

    #"properties.py" functionality exports as "data" property function, that
    #returns reference to non-const uchar*, this caused generated code to
    #raise compilation error
    ogre_ns.class_( "Image" ).member_functions( 'getData' ).exclude()

    ogre_ns.free_functions ('any_cast').exclude () #not relevant for Python

    #AttribParserList is a map from string to function pointer, this class could not be exposed
    AttribParserList = ogre_ns.typedef( name="AttribParserList" )
    declarations.class_traits.get_declaration( AttribParserList ).exclude()

    #VertexCacheProfiler constructor uses enum that will be defined later.
    #I will replace second default value to be int instead of enum
    #arg_types=[None,None] - 2 arguments, with whatever type
    VertexCacheProfiler = ogre_ns.constructor( 'VertexCacheProfiler', arg_types=[None,None] )
    VertexCacheProfiler.arguments[1].default_value = "int(%s)" % VertexCacheProfiler.arguments[1].default_value

    ## now specifically remove functions that we have wrapped in hand_made_wrappers.py
    ogre_ns.class_( "RenderTarget" ).member_functions( 'getCustomAttribute' ).exclude()
#     ogre_ns.class_( "RenderTarget" ).member_functions( 'getMetrics' ).exclude()
#     ogre_ns.class_( "RenderWindow" ).member_functions( 'getMetrics' ).exclude()
    ogre_ns.class_( "Mesh" ).member_functions( 'suggestTangentVectorBuildParams' ).exclude()
#     ogre_ns.class_( "Viewport" ).member_functions( 'getActualDimensions' ).exclude()

    ## Expose functions that were no exposed but that other functions reliy on    
    ogre_ns.class_("OverlayManager").member_functions('addOverlayElementFactory').include()




def set_call_policies( mb ):
#
    # Set the default policy to deal with pointer/reference return types to reference_existing object
    # as this is the Ogre Default.
    mem_funs = mb.calldefs ()
    mem_funs.create_with_signature = True #Generated code will not compile on
    #MSVC 7.1 if function has throw modifier.
    for mem_fun in mem_funs:
        if mem_fun.call_policies:
            continue
        if declarations.is_pointer (mem_fun.return_type) or declarations.is_reference (mem_fun.return_type):
            mem_fun.call_policies = call_policies.return_value_policy(
                call_policies.reference_existing_object )

def set_smart_pointers( mb ):
    for v in mb.variables():
       if not declarations.is_class( v.type ):
           continue
       cls = declarations.class_traits.get_declaration( v.type )
       if cls.name.startswith( 'SharedPtr<' ):
           v.apply_smart_ptr_wa = True    
           print "Applying Smart Pointer: ",  v.name, " of class: ",  cls.name
       elif cls.name.endswith( 'SharedPtr' ):
           v.apply_smart_ptr_wa = True    
           print "Applying Smart Pointer: ",  v.name, " of class: ",  cls.name
                
def configure_exception(mb):
    #We don't exclude  Exception, because it contains functionality, that could
    #be useful to user. But, we will provide automatic exception translator
    Exception = mb.namespace( 'Ogre' ).class_( 'Exception' )
    Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getFullDescription().c_str()' )


def add_transformations ( mb ):
    ns = mb.global_ns.namespace ('Ogre')
    ns.class_('RenderTarget').mem_fun('getMetrics').add_transformation(ft.output(0), ft.output(1), ft.output(2) )
    ns.class_('RenderQueueListener').mem_fun('renderQueueStarted').add_transformation(ft.output('skipThisInvocation'))
    ns.class_('RenderWindow').mem_fun('getMetrics').add_transformation(ft.output(0), ft.output(1), ft.output(2),ft.output(3),ft.output(4) )
    ns.class_('Viewport').mem_fun('getActualDimensions').add_transformation(ft.output(0), ft.output(1), ft.output(2), ft.output(3) )
    ns.class_('CompositorChain').class_('RQListener').mem_fun('renderQueueStarted').add_transformation(ft.output("skipThisQueue"))
    ns.class_('CompositorChain').class_('RQListener').mem_fun('renderQueueEnded').add_transformation(ft.output("repeatThisQueue"))
            
            
#
# the 'main'function
#            
def generate_code():
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.ogre.root_dir, "python_ogre.h" )
                        , environment.ogre.cache_file )

    defined_symbols = [ 'OGRE_NONCLIENT_BUILD' ]
    if environment.ogre.version == "CVS":
        defined_symbols.append( 'OGRE_VERSION_CVS' )
        
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ogre.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2 )
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    filter_declarations (mb)
    #
    # Then we convert automatic 'long' names to simple ones - check the list in customization_data.py
    #
    common_utils.set_declaration_aliases( mb.global_ns, customization_data.aliases( environment.ogre.version ) )
    #
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True

    ogre_ns = mb.namespace( 'Ogre' )
    
    common_utils.fix_unnamed_classes( ogre_ns.classes( name='' ), 'Ogre' )

    common_utils.configure_shared_ptr(mb)
    configure_exception( mb )
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )
    
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    set_call_policies ( mb.global_ns.namespace ('Ogre') )
    
    # now we fix up the smart pointers ...
    set_smart_pointers ( mb.global_ns.namespace ('Ogre') )  
    
    # here we fixup functions that expect to modifiy their 'passed' variables    
    add_transformations ( mb )
    
    cls = mb.class_( "IndexData" )
    v = cls.variable( "indexBuffer" )
    v.apply_smart_ptr_wa = True
    cls = mb.class_( "SubMesh" )
    v = cls.variable( "vertexData" )
    v.apply_smart_ptr_wa = True
    v = cls.variable( "indexData" )
    v.apply_smart_ptr_wa = True
   

    #
    # Automatically add properties where we can - ie mirror set_variable() with .variable= capability
    # note that we are using a new function to filter existing properties..
    #
    for cls in ogre_ns.classes():
        cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )

##    common_utils.add_properties( ogre_ns.classes(), True )
    
    common_utils.add_constants( mb, { 'ogre_version' :  '"%s"' % environment.ogre.version
                                      , 'python_version' : '"%s"' % sys.version } )
#     for cls in mb.classes():
#         print cls
#     sys.exit(-1)

#     
#     #Creating code creator. After this step you should not modify/customize declarations.
    mb.build_code_creator (module_name='_ogre_')
    for inc in environment.ogre.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ogre.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.ogre.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.ogre.version ) )

    mb.split_module(environment.ogre.generated_dir, huge_classes)

    if not os.path.exists( os.path.join(environment.ogre.generated_dir, 'py_shared_ptr.h' ) ):
        shutil.copy( os.path.join( environment.shared_ptr_dir, 'py_shared_ptr.h' )
                     , environment.ogre.generated_dir )

if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Python-OGRE source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
