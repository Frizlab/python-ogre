#!/usr/bin/env python

#-------------------------------------------------------------------------------
# TODO:
# 1.    void* as a function argument - they are currently wrapped (and compile/load etc) due to latest CVS of boost.
#       However probably don't actually work
# 2.    Properties.py and calling 'properties.create' - commented out at the moment, not sure if it is really needed?

import os, sys, time, shutil

#add environment to the path
sys.path.append( os.path.join( '..', '..' ) )
#add common utils to the pass
sys.path.append( '..' )

import environment
import common_utils
import customization_data
import hand_made_wrappers

from pygccxml import parser
from pygccxml import declarations
from pyplusplus import module_builder
from pyplusplus.decl_wrappers import property_t
from pyplusplus.module_builder import call_policies

def filter_declarations( mb ):
    global_ns = mb.global_ns
    global_ns.exclude()
    ogre_ns = global_ns.namespace( 'Ogre' )
    ogre_ns.include()

    startswith = [
        # Don't include, we'll never need.
        'D3D', 'GL',  'SDL', 'WIN32', 'Any', 'CompositorScriptCompiler', '_', 'Singleton',
        'MeshSerializerImpl', ## link problems - doesn't seem to exist at all ???

    ]

    if environment.ogre.version == "CVS":
        mb.global_ns.class_( 'vector<Ogre::Vector4, std::allocator<Ogre::Vector4> >' ).exclude( )

    ## Remove private classes , and those that are internal to Ogre...
    private_decls = common_utils.private_decls_t(environment.ogre.include_dir)
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
    ### AJM Although it you want to include them thats OK :)
    if True:
        query = declarations.access_type_matcher_t( 'public' ) \
                & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL ) \
                & declarations.custom_matcher_t( lambda decl: decl.name.startswith( '_' ) )
        non_public_non_pure_virtual = ogre_ns.calldefs( query )
        non_public_non_pure_virtual.exclude()

    else:
        ## AJM Having allowed functions starting with '_' I now need to exclude these are MSVC 7 compiler
        ## pukes with an overflow when compiling 'StaticGeometry.pypp.cpp'..
        GeomRegion = ogre_ns.class_( "StaticGeometry" ).class_("Region")
        GeomRegion.member_functions('_notifyCurrentCamera'). exclude()
        GeomRegion.member_functions('_updateRenderQueue'). exclude()

    ## it turns out that it pukes anyway on StaticGeometry so need to make it smaller..
    GeomRegion = ogre_ns.class_( "StaticGeometry" ).class_("Region").exclude()

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

    #AJM Set of functions in Particle system that don't get wrapped properly..
    PartSys = ogre_ns.class_( "ParticleSystem" )
    PartSys.class_( "CmdIterationInterval" ).exclude()
    PartSys.class_( "CmdLocalSpace" ).exclude()
    PartSys.class_( "CmdNonvisibleTimeout" ).exclude()
    PartSys.class_( "CmdSorted" ).exclude()

    # These members have Ogre::Real * methods which need to be wrapped.
    ogre_ns.class_ ('Matrix3').member_operators (symbol='[]').exclude ()
    ogre_ns.class_ ('Matrix4').member_operators (symbol='[]').exclude ()

    #returns reference to "const Real *"
    ogre_ns.class_ ('BillboardChain').calldef( 'getOtherTextureCoordRange' ).exclude()

    #all constructors in this class are private, also some of them are public.
    Skeleton = ogre_ns.class_( 'Skeleton' ).constructors().exclude()

    #"properties.py" functionality exports as "data" property function, that
    #returns reference to non-const uchar*, this cuased generated code to
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


def set_call_policies( mb ):
    ogre_ns = mb.global_ns.namespace ('Ogre')

    # Set the default policy to deal with pointer/reference return types to reference_existing object
    # as this is the Ogre Default.
    mem_funs = ogre_ns.calldefs ()
    mem_funs.create_with_signature = True #Generated code will not compile on
    #MSVC 7.1 if function has throw modifier.
    for mem_fun in mem_funs:
        if mem_fun.call_policies:
            continue
        if declarations.is_pointer (mem_fun.return_type) or declarations.is_reference (mem_fun.return_type):
            mem_fun.call_policies = call_policies.return_value_policy(
                call_policies.reference_existing_object )

def configure_exception(mb):
    #We don't exclude  Exception, because it contains functionality, that could
    #be useful to user. But, we will provide automatic exception translator
    Exception = mb.namespace( 'Ogre' ).class_( 'Exception' )
    Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getFullDescription().c_str()' )


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
                                          , include_paths=[environment.ogre.include_dir]
                                          , define_symbols=defined_symbols
                                          , start_with_declarations=['Ogre']
                                          , indexing_suite_version=2 )

    filter_declarations (mb)

    common_utils.set_declaration_aliases( mb.global_ns, customization_data.aliases( environment.ogre.version ) )

    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True

    ogre_ns = mb.namespace( 'Ogre' )
    common_utils.fix_unnamed_classes( ogre_ns.classes( name='' ), 'Ogre' )

    common_utils.configure_shared_ptr(mb)
    configure_exception( mb )
    ogre_ns.classes().add_properties()
    for cls in ogre_ns.classes():
        pyogre_props = []
        for prop in cls.properties:
            name = prop.name[0].lower() + prop.name[1:]
            pyogre_props.append( property_t( name, prop.fget, prop.fset, prop.doc, prop.is_static ) )
        cls.properties.extend( pyogre_props )
    hand_made_wrappers.apply( mb )

    set_call_policies (mb)

    #Creating code creator. After this step you should not modify/customize declarations.
    mb.build_code_creator (module_name='_ogre_')

    mb.code_creator.user_defined_directories.append( environment.ogre.include_dir )
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
