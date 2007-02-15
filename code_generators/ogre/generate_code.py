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
from pyplusplus import messages
from pyplusplus import module_builder
from pyplusplus import decl_wrappers

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
from pyplusplus.module_creator import sort_algorithms

import common_utils.extract_documentation as exdoc
import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties


def filter_declarations( mb ):

    global_ns = mb.global_ns
    global_ns.exclude()
    
    ogre_ns = global_ns.namespace( 'Ogre' )
    ogre_ns.include()
    
    startswith = [
        'WIN32'
        , 'MemoryManager'   ## it's a specialised C++ mem manger not needed in Python
        , 'RadixSort' ## these show up as ugly aliases but are never exposed - and are really protected
    ]

    if not environment.ogre.version.startswith("1.2") and os.name =='nt':
        mb.global_ns.class_( 'vector<Ogre::Vector4, std::allocator<Ogre::Vector4> >' ).exclude( )
    
    ## Now get rid of a wide range of classes as defined earlier in startswith...
    for prefix in startswith:
        ### NOTE the PREFIX is used here !!!!
        classes = ogre_ns.classes( common_utils.decl_starts_with(prefix), allow_empty=True)
        classes.exclude()

### AJM - OK, so I think we can remove all FACTORY classes etc - not really sure but thought we
### do so and see if anyone complained
    for cls in ogre_ns.classes ():
        if 'Factory' in cls.name:
            print "Factory Class Excluded", cls
            cls.exclude()
    for fun in ogre_ns.mem_funs ():
        if 'Factory' in fun.name:
            print "Factory Function Excluded", fun
            fun.exclude()
            continue
        for arg in fun.arguments:
            if 'Factory' in arg.name:
                print "Factory Function Excluded", fun
                fun.exclude()
                break
                    
        
        
    #AJM Set of functions in Particle system that don't get wrapped properly.. Rechecked 30Nov06 AJM
    ## Other 'Cmd..' classes are defined as _OgrePrivate, whereas these are not in the head file
    PartSys = ogre_ns.class_( "ParticleSystem" )
    PartSys.class_( "CmdIterationInterval" ).exclude()
    PartSys.class_( "CmdLocalSpace" ).exclude()
    PartSys.class_( "CmdNonvisibleTimeout" ).exclude()
    PartSys.class_( "CmdSorted" ).exclude()

    ## Functions defined in .h files but not implemented in source files
    ogre_ns.class_('Root').mem_fun('termHandler').exclude()
    ogre_ns.class_( "StaticGeometry" ).class_("Region").member_functions('getLights').exclude() 
    
    ## It's a structure that doesn't get included by default...
    ogre_ns.class_("VertexBoneAssignment_s").include()

   
    #exclude GpuLogicalIndexUseMap  NOTE:  Example use of Py++ to exclude a special variable........
    GpuLogicalBufferStruct = ogre_ns.class_( 'GpuLogicalBufferStruct' )
    GpuLogicalBufferStruct.variable( 'map' ).exclude()   
    
    ## THIS IS A UNION
    ogre_ns.class_('GpuProgramParameters').class_('AutoConstantEntry').variable('data').exclude()    
    v= ogre_ns.class_('GpuProgramParameters').class_('AutoConstantEntry').variable('fData')
    v.exclude() 
     
    
    # there are a set of consiterators that I'm not exposing as they need better understanding and testing
    # these functions rely on them so they are being excluded as well
    ogre_ns.class_('SceneNode').member_functions('getAttachedObjectIterator').exclude()
    ogre_ns.class_('Mesh').mem_fun('getBoneAssignmentIterator').exclude()
    ogre_ns.class_('SubMesh').mem_fun('getBoneAssignmentIterator').exclude()
    
    # A couple of Std's that need exposing
    std_ns = global_ns.namespace("std")
    std_ns.class_("pair<unsigned, unsigned>").include()
    std_ns.class_("pair<bool, float>").include()


    # functions that take pointers to pointers 
    ogre_ns.class_( 'VertexElement').member_functions('baseVertexPointerToElement').exclude()
    mb.global_ns.mem_fun('::Ogre::InstancedGeometry::BatchInstance::getObjectsAsArray').exclude()

    # return arrays
    ##  const Vector3* ----
    for f in ogre_ns.mem_funs( return_type='::Ogre::Vector3 const *', allow_empty=True):
        if f.name.startswith("get") and "Corner" in f.name:
            f.call_policies = call_policies.convert_array_to_tuple( 8, call_policies.memory_managers.none )
   
    #all constructors in this class are private, also some of them are public.
    if sys.platform=='win32':
        Skeleton = ogre_ns.class_( 'Skeleton' ).constructors().exclude()
    else:
        ogre_ns.class_( 'Skeleton' ).exclude()

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
    ogre_ns.class_( "Mesh" ).member_functions( 'suggestTangentVectorBuildParams' ).exclude()
    
    ## Expose functions that were not exposed but that other functions rely on    
    ### ogre_ns.class_("OverlayManager").member_functions('addOverlayElementFactory').include()
    
    ## AJM Error at compile time - errors when compiling or linking
    ogre_ns.calldefs ('peekNextPtr').exclude ()
    ogre_ns.calldefs ('peekNextValuePtr').exclude ()    #in many of the Iterator classes
    ogre_ns.calldefs ('getChildIterator').exclude ()
    
    ogre_ns.class_( "ErrorDialog" ).exclude()   # doesn't exist for link time
    ogre_ns.class_( 'CompositorInstance').class_('RenderSystemOperation').exclude() # doesn't exist for link time
    ogre_ns.class_( 'CompositorChain').mem_fun('_queuedOperation').exclude() #needs RenderSystemOperation
    
    #as reported by mike with linux:bp::arg("flags")=(std::_Ios_Fmtflags)0
    if os.name == 'posix':
        ogre_ns.class_('StringConverter').member_functions('toString').exclude()    
   
   ## changes due to expanded header file input
   
    ogre_ns.class_('OptimisedUtil').mem_fun('softwareVertexSkinning').exclude
   
    ogre_ns.class_('ShadowVolumeExtrudeProgram').variable('programNames').exclude()    #funky sring[8] problem
        
##  Note - you need to do all the 'excludes' AFTER you've included all the classes you want..
##  Otherwise things get screwed up...

## Remove private classes , and those that are internal to Ogre...
    private_decls = common_utils.private_decls_t(environment.ogre.include_dirs)
    for cls in ogre_ns.classes():
        if private_decls.is_private( cls ):
            cls.exclude()
            print '{*} class "%s" is marked as private' % cls.decl_string


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

    #Virtual functions that return reference could not be overriden from Python
    query = declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.VIRTUAL ) \
            & declarations.custom_matcher_t( lambda decl: declarations.is_reference( decl.return_type ) )
    ogre_ns.calldefs( query ).virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL

    #RenderOperation class is marked as private, but I think this is a mistake
    ogre_ns.class_('RenderOperation').include()
    
    if sys.platform == 'win32':
        # need to force these
        hwnd = global_ns.class_("HWND__")
        hwnd.opaque = True
        _iobuf = global_ns.class_("_iobuf")# need the file handle in Ogre::FileHandleDataStream::FileHandleDataStream
        _iobuf.opaque = True

    #Exclude non default constructors of iterator classes. 
    for cls in ogre_ns.classes():
        if not declarations.templates.is_instantiation( cls.name ):
           continue
        name = declarations.templates.name( cls.name )
        if not name.endswith( 'Iterator' ):
           continue
        #default constructor does not have arguments
        constructors = cls.constructors( lambda decl: bool( decl.arguments )
                                                      , allow_empty=True
                                                      , recursive=False )
        constructors.exclude()
        # and while we are here we have problems with '=' on these classes
        try:
            cls.operator('=').exclude()
        except:
            pass
            
    ## now for problem areas in the new unicode string handling - just excluding without 'thought' :)
    ## the variables are not present in the source (to check)
    ## most of the functions return pointers to 'stuff' that isn't handled at compile time
    ogre_ns.class_('UTFString').variable('mVoidBuffer').exclude()
    ogre_ns.class_('UTFString').variable('mStrBuffer').exclude()
    ogre_ns.class_('UTFString').variable('mWStrBuffer').exclude()
    ogre_ns.class_('UTFString').variable('mUTF32StrBuffer').exclude()
    ogre_ns.class_('UTFString').member_functions('at').exclude()
    ogre_ns.class_('UTFString').mem_fun('c_str').exclude()
    ogre_ns.class_('UTFString').mem_fun('data').exclude()  
    ogre_ns.class_('UTFString').mem_fun('asUTF32_c_str').exclude()
    
    ## missing symbols at link time, including constructor and destructor!
    global_ns.class_('::Ogre::InstancedGeometry::MaterialBucket').mem_fun('getGeometryBucketList').exclude()
    global_ns.class_('::Ogre::InstancedGeometry::MaterialBucket').mem_fun('getMaterialBucketMap').exclude()
    
    global_ns.class_('::Ogre::UnifiedHighLevelGpuProgram::CmdDelegate').mem_fun('doGet').exclude()
    global_ns.class_('::Ogre::UnifiedHighLevelGpuProgram::CmdDelegate').mem_fun('doSet').exclude()
    global_ns.class_('::Ogre::UnifiedHighLevelGpuProgram::CmdDelegate').exclude()
    
       
def find_nonconst ( mb ):
    """ we have problems with sharedpointer arguments that are defined as references
    but are NOT const.  Boost doesn't understand how to match them and you get a C++ Signature match fails.
    In reality the Ogre code probably needs to be patched as all of these should (??) be const.  However we'll fix it 
    with a function transformation wrapper
    """
    funcs = mb.member_functions( )
    for fun in funcs:
        arg_position = 0
        for arg in fun.arguments:
            if 'Ptr' in arg.type.decl_string:
                 if not 'const' in arg.type.decl_string and '&' in arg.type.decl_string:
                    #print "Fixing Const", fun.parent.name,"::", fun.name, "::", arg_position
                    fun.add_transformation( ft.modify_type(arg_position,declarations.remove_reference ) )
            arg_position +=1
                    
def add_auto_conversions( mb ):
    """
    Allows to pass Python tuple as argument to function, instead of
       * ColourValue
       * Vector[2|3|4]
       * TODO: Matrix[3|4]
    """
    rvalue_converters = ( 
        'register_pytuple_to_colour_value_conversion'
        , 'register_pytuple_to_vector2_conversion'
        , 'register_pytuple_to_vector3_conversion'
        , 'register_pytuple_to_vector4_conversion'
        , 'register_pystring_to_utfstring_conversion' )
        
    for converter in rvalue_converters:
        mb.add_declaration_code( 'void %s();' % converter )
        mb.add_registration_code( '%s();' % converter )
    
    environment.ogre.generated_dir
    
    custom_rvalue_path = os.path.join(
                            os.path.abspath(os.path.dirname(__file__) )
                            , 'custom_rvalue.cpp' )
    
    if not os.path.exists( os.path.join(environment.ogre.generated_dir, 'custom_rvalue.cpp' ) ):
        shutil.copy( custom_rvalue_path, environment.ogre.generated_dir )
  
    if not common_utils.samefile( os.path.join(environment.ogre.generated_dir, 'custom_rvalue.cpp' )
                                  , custom_rvalue_path ):
        print "Updated custom_rvalue.cpp as it was missing or out of date"                                      
        shutil.copy( custom_rvalue_path, environment.ogre.generated_dir )
  
    UTFString = mb.class_( 'UTFString' )
    UTFString.mem_fun( 'asUTF8' ).alias = '__str__'
    UTFString.mem_fun( 'asWStr' ).alias = '__unicode__'
  
def set_call_policies( mb ):
#
    # Set the default policy to deal with pointer/reference return types to reference_existing object
    # as this is the Ogre Default.
    mem_funs = mb.calldefs ()
    mem_funs.create_with_signature = True #Generated code will not compile on
    #MSVC 7.1 if function has throw modifier.
    pointee_types=['unsigned int','int', 'float', '::Ogre::Real', '::Ogre::uchar', 'unsigned char']
    for mem_fun in mem_funs:
        if declarations.is_pointer (mem_fun.return_type):
            for i in pointee_types:
                if mem_fun.return_type.decl_string.startswith ( i ):
                    print "Excluding", mem_fun
                    mem_fun.exclude()
                    ##mem_fun.add_transformation( ft.modify_return_type( ReturnUnsignedInt ) )

##                    mem_fun.call_policies = call_policies.return_value_policy( '::boost::python::return_pointee_value' )
                    break
        if mem_fun.call_policies:
            continue
        if not mem_fun.call_policies and \
                    (declarations.is_reference (mem_fun.return_type) or declarations.is_pointer (mem_fun.return_type) ):
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
    Exception.include()
    Exception.mem_fun('what').exclude() # declared with empty throw
    Exception.mem_fun('getNumber').exclude() # declared with empty throw
    Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getFullDescription().c_str()' )

def ReturnUnsignedInt( type_ ):
    return declarations.cpptypes.unsigned_int_t()
    
def FixVoidPtrArgs ( mb ):
    for fun in mb.member_functions():
        arg_position = 0
        for arg in fun.arguments:
            if declarations.type_traits.is_void_pointer(arg.type):
                fun.add_transformation( ft.modify_type(arg_position,ReturnUnsignedInt ) )
                print "Fixed Void Ptr", fun, arg_position
                break
            arg_position +=1
                    

def add_transformations ( mb ):
    ns = mb.global_ns.namespace ('Ogre')
        
    def create_output( size ):
        return [ ft.output( i ) for i in range( size ) ]
#     ns.mem_fun('::Ogre::Image::getData').add_transformation ( \
#                     ft.modify_type(0,ReturnUnsignedInt ) )
#    
    rt_cls = ns.class_('RenderTarget')
    rt_cls.mem_fun('getMetrics').add_transformation( *create_output(3) )
    rt_cls.mem_fun( 'getStatistics', arg_types=['float &']*4 ).add_transformation( *create_output(4) )
    ns.mem_fun('::Ogre::RenderQueueListener::renderQueueEnded') \
        .add_transformation(ft.output('repeatThisInvocation'))
    ns.mem_fun('::Ogre::RenderQueueListener::renderQueueStarted') \
        .add_transformation(ft.output('skipThisInvocation'))
    ns.mem_fun('::Ogre::RenderWindow::getMetrics').add_transformation( *create_output(5) )
    ns.mem_fun('::Ogre::Viewport::getActualDimensions').add_transformation( *create_output(4) )
    ns.mem_fun('::Ogre::CompositorChain::RQListener::renderQueueStarted') \
        .add_transformation(ft.output("skipThisQueue"))
    ns.mem_fun('::Ogre::CompositorChain::RQListener::renderQueueEnded') \
        .add_transformation(ft.output("repeatThisQueue"))
    ns.mem_fun('::Ogre::PanelOverlayElement::getUV') \
        .add_transformation(ft.output('u1'), ft.output('v1'), ft.output('u2'), ft.output('v2') )
    ### ns.class_('Matrix3').mem_fun('Matrix3').add_transformation(ft.inputarray(9))........        
#     ns.mem_fun('::Ogre::Font::getGlyphTexCoords') \
#         .add_transformation(ft.output(1), ft.output(2),ft.output(3), ft.output(4))
    ns.mem_fun('::Ogre::ExternalTextureSource::getTextureTecPassStateLevel').add_transformation( *create_output(3) )

def query_containers_with_ptrs(decl):
    if not isinstance( decl, declarations.class_types ):
       return False
    if not decl.indexing_suite:
       return False
    return declarations.is_pointer( decl.indexing_suite.element_type )

def remove_static_consts ( mb ):
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
#         , messages.W1040 
#         # Inaccessible property warning
#         , messages.W1041
#         , messages.W1036 # pointer to Python immutable member
#         , messages.W1033 # unnamed variables
#         , messages.W1018 # expose unnamed classes
#         , messages.W1049 # returns reference to local variable
#         , messages.W1014 # unsupported '=' operator
#          )
    ### sort_algorithms.USE_CALLDEF_ORGANIZER = True   ## tried this to remove a couple of order issues, without success :)
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.ogre.root_dir, "python_ogre.h" )
                        , environment.ogre.cache_file )

    defined_symbols = [ 'OGRE_NONCLIENT_BUILD' ]
    defined_symbols.append( 'OGRE_VERSION_' + environment.ogre.version )  
    
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ogre.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                           )
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    filter_declarations (mb)
    remove_static_consts ( mb.namespace( 'Ogre' ) )
    #
    # fix shared Ptr's that are defined as references but NOT const...
    #
    find_nonconst ( mb.namespace( 'Ogre' ) )
      
    FixVoidPtrArgs  ( mb.namespace( 'Ogre' ) )
          
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True

    ogre_ns = mb.namespace( 'Ogre' )
    
    #Py++ can not expose static pointer member variables
    ogre_ns.vars( 'ms_Singleton' ).disable_warnings( messages.W1035 )
    
    common_utils.fix_unnamed_classes( ogre_ns.classes( name='' ), 'Ogre' )
    
    common_utils.configure_shared_ptr(mb)
    configure_exception( mb )
        
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    set_call_policies ( mb.global_ns.namespace ('Ogre') )
    
    # now we fix up the smart pointers ...
    set_smart_pointers ( mb.global_ns.namespace ('Ogre') )  
    
    # here we fixup functions that expect to modifiy their 'passed' variables    
    add_transformations ( mb )
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )
    
    cls = mb.class_( "IndexData" )
    v = cls.variable( "indexBuffer" )
    v.apply_smart_ptr_wa = True
    cls = mb.class_( "SubMesh" )
    v = cls.variable( "vertexData" )
    v.apply_smart_ptr_wa = True
    v = cls.variable( "indexData" )
    v.apply_smart_ptr_wa = True
    NoPropClasses = ["UTFString"]
    for cls in ogre_ns.classes():
#     for cls in mb.global_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
        ## because we want backwards pyogre compatibility lets add leading lowercase properties
        common_utils.add_LeadingLowerProperties ( cls )

    common_utils.add_constants( mb, { 'ogre_version' :  '"%s"' % environment.ogre.version.replace("\n", "\\\n") 
                                      , 'python_version' : '"%s"' % sys.version.replace("\n", "\\\n" ) } )

    add_auto_conversions( mb )

    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor("")
    mb.build_code_creator (module_name='_ogre_' , doc_extractor= extractor)
    
    for inc in environment.ogre.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ogre.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.ogre.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.ogre.version ) )

    mb.split_module(environment.ogre.generated_dir, huge_classes)

    if not common_utils.samefile( os.path.join( environment.shared_ptr_dir, 'py_shared_ptr.h'),
                             os.path.join(environment.ogre.generated_dir, 'py_shared_ptr.h' ) ):
        print "Updated py_shared_ptr.h as it was missing or out of date"
        shutil.copy( os.path.join( environment.shared_ptr_dir, 'py_shared_ptr.h' )
                     , environment.ogre.generated_dir )
                     
    if not common_utils.samefile ( os.path.join( os.getcwd(), 'python_ogre_masterlist.h' ),
                            os.path.join( environment.ogre.generated_dir, 'python_ogre_masterlist.h' ) ) :            
        print "Updated python_ogre_masterlist.h as it was missing or out of date"
        shutil.copy( os.path.join( os.getcwd(), 'python_ogre_masterlist.h' )
                     , environment.ogre.generated_dir )
    
    generators_path = os.path.join( os.path.abspath(os.path.dirname(__file__) )
                                    , 'generators.h' )

    if not common_utils.samefile( generators_path,
                                    os.path.join(environment.ogre.generated_dir, 'generators.h' )
                                   ):
        print "Updated generators.h as it was missing or out of date"                                      
        shutil.copy( generators_path, environment.ogre.generated_dir )
        
    return_pointee_value_source_path = os.path.join( environment.pyplusplus_install_dir
                    , 'pyplusplus_dev'
                    , 'pyplusplus'
                    , 'code_repository'
                    , 'return_pointee_value.hpp' )

    return_pointee_value_target_path \
        = os.path.join( environment.ogre.generated_dir, 'return_pointee_value.hpp' )

    if not common_utils.samefile( return_pointee_value_source_path, return_pointee_value_target_path ):
        print "Updated return_pointee_value.hpp as it was missing or out of date"                                      
        shutil.copy( return_pointee_value_source_path, environment.ogre.generated_dir )


if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Python-OGRE source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
