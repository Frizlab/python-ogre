#!/usr/bin/env python
"""  To Do list:

Continue to ignore ptr() functions in Vectors2/3/4 and Matrix 2/3/4 - these simple return a pointer to the start of the 
underlying data memebers and can be accessed in python with matrix3.x, matrix3.y etc

FT [] operators for Matrix3/4 to return 3/4 array


"""

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


############################################################
##
##  Here is where we manually exclude stuff from Ogre - 
##
############################################################

def ManualExclude ( mb ):
    global_ns = mb.global_ns
    ogre_ns = global_ns.namespace( 'Ogre' )
    
    ## Specifically remove functions that we have wrapped in hand_made_wrappers.py
    ogre_ns.class_( "RenderTarget" ).member_functions( 'getCustomAttribute' ).exclude()
    ##ogre_ns.class_( "Mesh" ).member_functions( 'suggestTangentVectorBuildParams' ).exclude()
    # hand made wrapper to return correct type
    global_ns.class_('::Ogre::ResourceManager').mem_fun('getByName').exclude()
    global_ns.class_('::Ogre::ResourceManager').mem_fun('getByHandle').exclude()

    
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
# # #     for cls in ogre_ns.classes ():
# # #         if 'Factory' in cls.name:
# # #             print "Factory Class Excluded", cls
# # #             cls.exclude()
# # #     for fun in ogre_ns.mem_funs ():
# # #         if 'Factory' in fun.name:
# # #             print "Factory Function Excluded", fun
# # #             fun.exclude()
# # #             continue
# # #         for arg in fun.arguments:
# # #             if 'Factory' in arg.name:
# # #                 print "Factory Function Excluded", fun
# # #                 fun.exclude()
# # #                 break
        
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

    #exclude GpuLogicalIndexUseMap  NOTE:  Example use of Py++ to exclude a special variable........
    GpuLogicalBufferStruct = ogre_ns.class_( 'GpuLogicalBufferStruct' )
    GpuLogicalBufferStruct.variable( 'map' ).exclude()   
    
    ## THIS IS A UNION
    ogre_ns.class_('GpuProgramParameters').class_('AutoConstantEntry').variable('data').exclude()    
    v= ogre_ns.class_('GpuProgramParameters').class_('AutoConstantEntry').variable('fData')
    v.exclude() 
    # there are a set of const iterators that I'm not exposing as they need better understanding and testing
    # these functions rely on them so they are being excluded as well
# # #     ogre_ns.class_('SceneNode').member_functions('getAttachedObjectIterator').exclude()
# # #     ogre_ns.class_('Mesh').mem_fun('getBoneAssignmentIterator').exclude()
# # #     ogre_ns.class_('SubMesh').mem_fun('getBoneAssignmentIterator').exclude()
    
    
    # functions that take pointers to pointers 
    ogre_ns.class_( 'VertexElement').member_functions('baseVertexPointerToElement').exclude()
    mb.global_ns.mem_fun('::Ogre::InstancedGeometry::BatchInstance::getObjectsAsArray').exclude()
    #all constructors in this class are private, also some of them are public.
    if sys.platform=='win32':
        Skeleton = ogre_ns.class_( 'Skeleton' ).constructors().exclude()
    else:
        ogre_ns.class_( 'Skeleton' ).exclude()

    ogre_ns.free_functions ('any_cast').exclude () #not relevant for Python

    #AttribParserList is a map from string to function pointer, this class could not be exposed
    AttribParserList = ogre_ns.typedef( name="AttribParserList" )
    declarations.class_traits.get_declaration( AttribParserList ).exclude()
    ## AJM Error at compile time - errors when compiling or linking
    ogre_ns.calldefs ('peekNextPtr').exclude ()
    ogre_ns.calldefs ('peekNextValuePtr').exclude ()    #in many of the Iterator classes
    
    
# # # #     ogre_ns.calldefs ('getChildIterator').exclude ()
    
    ogre_ns.class_( "ErrorDialog" ).exclude()   # doesn't exist for link time
    ogre_ns.class_( 'CompositorInstance').class_('RenderSystemOperation').exclude() # doesn't exist for link time
    ogre_ns.class_( 'CompositorChain').mem_fun('_queuedOperation').exclude() #needs RenderSystemOperation
    
    #as reported by mike with linux:bp::arg("flags")=(std::_Ios_Fmtflags)0
    if os.name == 'posix':
        ogre_ns.class_('StringConverter').member_functions('toString').exclude()    
   
   ## changes due to expanded header file input
   
    ogre_ns.class_('OptimisedUtil').mem_fun('softwareVertexSkinning').exclude
    ogre_ns.class_('ShadowVolumeExtrudeProgram').variable('programNames').exclude()    #funky sring[8] problem
            
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
    
    global_ns.class_('::Ogre::UnifiedHighLevelGpuProgramFactory').exclude()
    global_ns.class_('::Ogre::UnifiedHighLevelGpuProgram::CmdDelegate').exclude()
#     global_ns.class_('::Ogre::UnifiedHighLevelGpuProgram::CmdDelegate').mem_fun('doGet').exclude()
#     global_ns.class_('::Ogre::UnifiedHighLevelGpuProgram::CmdDelegate').mem_fun('doSet').exclude()
#     
    
############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################
    
def ManualInclude ( mb ):
    global_ns = mb.global_ns
    ogre_ns = global_ns.namespace( 'Ogre' )
    
#     ogre_ns.class_( "StaticGeometry" ).class_("Region").include()
#     ogre_ns.class_( "StaticGeometry" ).class_("LODBucket").include()
#     ogre_ns.class_( "StaticGeometry" ).class_("MaterialBucket").include()
    

    ## It's a structure that doesn't get included by default...
    ogre_ns.class_("VertexBoneAssignment_s").include()
    # A couple of Std's that need exposing
    std_ns = global_ns.namespace("std")
    std_ns.class_("pair<unsigned, unsigned>").include()
    std_ns.class_("pair<bool, float>").include()
    std_ns.class_("pair<Ogre::SharedPtr<Ogre::Resource>, bool>").include()
    
    ## handle the hashmaps -- TODO FIX under LINUX
    if sys.platform=='win32':
        stdex_ns = global_ns.namespace("stdext")
        for cls in stdex_ns.classes():
            print "Checking", cls
            if cls.name.startswith ("hash"):
                print "Including", cls
                cls.include()
    
    
    #RenderOperation class is marked as private, but I think this is a mistake
    ogre_ns.class_('RenderOperation').include()
    
#     ogre_ns.class_('_ConfigOption').noncopyable = False
        
############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixes ( mb ):    

    global_ns = mb.global_ns
    ogre_ns = global_ns.namespace( 'Ogre' )

    # return arrays
    ##  const Vector3* ----
    for f in ogre_ns.mem_funs( return_type='::Ogre::Vector3 const *', allow_empty=True):
        if f.name.startswith("get") and "Corner" in f.name:
            f.call_policies = call_policies.convert_array_to_tuple( 8, call_policies.memory_managers.none )
            f.include()
   
    ### NOTE that we "include" things here again as they've probably been excluded in AutoFixes..
    
    ## this one points to an array of [2] floats        
    c =ogre_ns.class_('BillboardChain').mem_fun('getOtherTextureCoordRange')
    c.call_policies = call_policies.convert_array_to_tuple( 2, call_policies.memory_managers.none )    
    c.include()
            
    ## and these ones return
    c = ogre_ns.class_('Matrix4').operators('[]')
    c.call_policies= call_policies.convert_array_to_tuple( 4, call_policies.memory_managers.none )   
    c.include() 
    c = ogre_ns.class_('Matrix3').operators('[]')
    c.call_policies= call_policies.convert_array_to_tuple( 3, call_policies.memory_managers.none )    
    c.include()

            
        
    #VertexCacheProfiler constructor uses enum that will be defined later.
    #I will replace second default value to be int instead of enum
    #arg_types=[None,None] - 2 arguments, with whatever type
    VertexCacheProfiler = ogre_ns.constructor( 'VertexCacheProfiler', arg_types=[None,None] )
    VertexCacheProfiler.arguments[1].default_value = "int(%s)" % VertexCacheProfiler.arguments[1].default_value
      
    if sys.platform == 'win32':
        # need to force these
        hwnd = global_ns.class_("HWND__")
        hwnd.opaque = True
        _iobuf = global_ns.class_("_iobuf")# need the file handle in Ogre::FileHandleDataStream::FileHandleDataStream
        _iobuf.opaque = True
        
    ## we apply smart ptr to sharedptr classes automatically, however these are harder to identify    
    cls = mb.class_( "IndexData" )
    v = cls.variable( "indexBuffer" )
    v.apply_smart_ptr_wa = True
    cls = mb.class_( "SubMesh" )
    v = cls.variable( "vertexData" )
    v.apply_smart_ptr_wa = True
    v = cls.variable( "indexData" )
    v.apply_smart_ptr_wa = True   
    
    # make UFTstrings behave as real Python strings..
    UTFString = mb.class_( 'UTFString' )
    UTFString.mem_fun( 'asUTF8' ).alias = '__str__'
    UTFString.mem_fun( 'asWStr' ).alias = '__unicode__'

    # expose << operators as __str__ functions
    ClassList=['Vector3','Vector2','Vector4','Matrix4', 'ColourValue', 'Quaternion']
    for cls in ClassList:
        mb.class_(cls).add_registration_code ( 'def(str(bp::self))' )
    
             
############################################################
##
##  And things that need to have their argument and call values fixed.
##  ie functions that pass pointers in the argument list and of course we need
##  to read the updated values - so instead we pass them back 
##  as new values in a tuple (ETC ETC)
##
############################################################
        
def ManualTransformations ( mb ):
    ns = mb.global_ns.namespace ('Ogre')
        
    def create_output( size ):
        return [ ft.output( i ) for i in range( size ) ]
        
    def docit ( general, i, o ): 
        docs = "Python-Ogre Modified Function Call\\n" + general +"\\n"
        docs = docs + "Input: " + i + "\\n"
        docs = docs + "Output: " + o + "\\n\\\n"
        return docs

    rt_cls = ns.class_('RenderTarget')
    x=rt_cls.mem_fun('getMetrics')
    x.add_transformation( *create_output(3) )
    x.documentation = docit ( "","no arguments", "tuple containing width, height, colourDepth")
    
    x=rt_cls.mem_fun( 'getStatistics', arg_types=['float &']*4 )
    x.add_transformation( *create_output(4) )
    x.documentation = docit ("", "no arguments", "tuple - lastFPS, avgFPS, bestFPS, worstFPS")
    
    x = ns.mem_fun('::Ogre::RenderQueueListener::renderQueueEnded')
    x.add_transformation(ft.output('repeatThisInvocation'))
    x.documentation = docit ("","queueGroupId, invocation", "tuple - repeatThisInvocation")
    
    x = ns.mem_fun('::Ogre::RenderQueueListener::renderQueueStarted') 
    x.add_transformation(ft.output('skipThisInvocation'))
    x.documentation = docit ("","queueGroupId, invocation", "tuple - repeatThisInvocation")
    
    x=ns.mem_fun('::Ogre::RenderWindow::getMetrics')
    x.add_transformation( *create_output(5) )
    x.documentation = docit ("","no arguments", "tuple - width, height, colourDepth, left, top")
    
    x=ns.mem_fun('::Ogre::Viewport::getActualDimensions')
    x.add_transformation( *create_output(4) )
    x.documentation = docit ("","no arguments", "tuple - left, top, width, height")

    x=ns.mem_fun('::Ogre::CompositorChain::RQListener::renderQueueStarted')
    x.add_transformation(ft.output("skipThisQueue"))
    x.documentation = docit ("", "id, invocation", "skipThisQueue" )
    
    x=ns.mem_fun('::Ogre::CompositorChain::RQListener::renderQueueEnded') 
    x.add_transformation(ft.output("repeatThisQueue"))
    x.documentation = docit ("", "id, invocation", "repeatThisQueue" )
    
    x=ns.mem_fun('::Ogre::PanelOverlayElement::getUV') 
    x.add_transformation(ft.output('u1'), ft.output('v1'), ft.output('u2'), ft.output('v2') )
    x.documentation = docit ("", "no arguments", "tuple - u1, v1, u2, v2" )
    
    x=ns.mem_fun('::Ogre::ExternalTextureSource::getTextureTecPassStateLevel')
    x.add_transformation( *create_output(3) )  
    x.documentation = docit ("", "no arguments", "tuple - TechniqueLevel, PassLevel,StateLevel")
              
    x=ns.mem_fun('::Ogre::Mesh::suggestTangentVectorBuildParams' )
    x.add_transformation(ft.output('outSourceCoordSet'), ft.output('outIndex') )
    x.documentation = docit ("", "targetSemantic","outSourceCoordSet, outIndex" )
    
    # these are * * 's so need more work
#     x = ns.mem_fun('::Ogre::AnimationTrack::getKeyFramesAtTime' )
#     x.add_transformation(ft.output('keyFrame1'), ft.output('keyFrame2') )
    
#     x = ns.mem_fun('::Ogre::Mesh::prepareMatricesForVertexBlend' )
#     x.add_transformation(ft.output('blendMatrices') )
#     
#     x = ns.mem_fun('::Ogre::Mesh::softwareVertexBlend' )
#     x.add_transformation(ft.output('blendMatrices') )
#     
#     x = ns.mem_fun('::Ogre::OptimisedUtil::softwareVertexSkinning' )
#     x.add_transformation(ft.output('blendMatrices') )
    
#     x = ns.mem_fun('::Ogre::NumericSolver::solveNxNLinearSysDestr')
#     x.add_transformation(ft.output('coeff') )
    
#     x = ns.mem_fun('::Ogre::SkeletonInstance::_getAnimationImpl')
#     x.add_transformation(ft.output('linker') )
#     x = ns.mem_fun('::Ogre::SkeletonInstance::getAnimation', arg_types=[None,None])
#     x.add_transformation(ft.output('linker') )
# #     x = ns.mem_fun('::Ogre::Skeleton::_getAnimationImpl')
# #     x.add_transformation(ft.output('linker') )
#     x = ns.mem_fun('::Ogre::Skeleton::getAnimation', arg_types=[None,None])
#     x.add_transformation(ft.output('linker') )
    
    x = ns.mem_fun('::Ogre::RenderQueue::RenderableListener::renderableQueued')
    x.add_transformation(ft.output('ppTech') )
    x.documentation = docit ("UNTESTED", "rend, groupID, priority", "ppTech" )
    
    ##
    ## now we handle some specials..
    ##


    image_size = """ 
namespace{ 
struct ImageSize{ 
    ssize_t operator()( boost::python::tuple args ) const{ 
        boost::python::object self = args[0];
        Ogre::Image& img = boost::python::extract<Ogre::Image&>( self ); 
        return img.getSize(); 
    } 
}; 
} 
""" 
        
    Image = ns.class_( 'Image' ) 
    Image.add_declaration_code( image_size ) 
    for f in Image.mem_funs( 'getData' ): 
        f.call_policies = call_policies.return_range( f, 'ImageSize' ) 
        f.documentation = "Python-Ogre Modified Return Range \\n"
        if f.has_const: 
            f.alias = 'getReadOnlyData' 

    memorydatastream_size = """ 
namespace{ 
struct MDSSize{ 
    ssize_t operator()( boost::python::tuple args ) const{ 
        boost::python::object self=args[0];
        Ogre::MemoryDataStream& mds = boost::python::extract<Ogre::MemoryDataStream&>( self ); 
        return mds.size(); 
    } 
}; 
} 
"""     
    MDS = ns.class_( 'MemoryDataStream' ) 
    MDS.add_declaration_code( memorydatastream_size ) 
    f = MDS.mem_fun( 'getPtr' ) 
    f.call_policies = call_policies.return_range( f, 'MDSSize' ) 
    f.documentation = "Python-Ogre Modified Return Range \\n"
    
    gpu_pp_int_size = """ 
namespace{ 
struct GpuProgramParametersGetIntPointerSize{ 
    ssize_t operator()( boost::python::tuple args ) const{ 
        boost::python::object self = args[0]; 
        Ogre::GpuProgramParameters& gpupp = boost::python::extract<Ogre::GpuProgramParameters&>( self ); 
        boost::python::object pos_obj = args[1]; 
        ssize_t offset = boost::python::extract<ssize_t>( pos_obj ); 
        return gpupp.intBufferSize - offset; 
        }
}; 
struct GpuProgramParametersGetFloatPointerSize{ 
    ssize_t operator()( boost::python::tuple args ) const{ 
        boost::python::object self = args[0]; 
        Ogre::GpuProgramParameters& gpupp = boost::python::extract<Ogre::GpuProgramParameters&>( self ); 
        boost::python::object pos_obj = args[1]; 
        ssize_t offset = boost::python::extract<ssize_t>( pos_obj ); 
        return gpupp.floatBufferSize - offset; 
        }
}; 
} 
""" 
  
 
#     GpuProgramParameters = ns.class_( 'GpuProgramParameters' ) 
#     GpuProgramParameters.add_declaration_code( gpu_pp_int_size ) 
#     f = GpuProgramParameters.mem_fun( 'getIntPointer' ) 
#     f.call_policies = call_policies.return_range( f, 'GpuProgramParametersGetIntPointerSize' ) 
#     f.documentation = "Python-Ogre Modified Return Range \\n"
#     f = GpuProgramParameters.mem_fun( 'getFloatPointer' ) 
#     f.call_policies = call_policies.return_range( f, 'GpuProgramParametersGetFloatPointerSize' ) 
#     f.documentation = "Python-Ogre Modified Return Range \\n"
    
###############################################################################
##
##  Now for the AUTOMATIC stuff that should just work with all versions of Ogre
##
###############################################################################
    
    
def AutoExclude( mb ):
    """ Automaticaly exclude a range of things that don't convert well from C++ to Python
    """
    global_ns = mb.global_ns
    ogre_ns = global_ns.namespace( 'Ogre' )
    
    # vars that are static consts but have their values set in the header file are bad
    Remove_Static_Consts ( ogre_ns )
    
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

            
def AutoInclude( mb ):
    pass

    
def AutoFixes ( mb ): 
    """ now we fix a range of things automatically - typically by going through 
    the entire name space trying to guess stuff and fix it:)
    """       
    global_ns = mb.global_ns
    ogre_ns = global_ns.namespace( 'Ogre' )
    
    # arguments passed as refs but not const are not liked by boost
    Fix_Ref_Not_Const ( ogre_ns )
    
    # Functions that have void pointers in their argument list need to change to unsigned int's  
    Fix_Void_Ptr_Args  ( ogre_ns )
    
    # Allow conversion between Vectors/Colourvalue etc and Python lists      
    Add_Auto_Conversions( mb )
    
    # now we fix up the smart pointers ...
    Set_Smart_Pointers ( ogre_ns )  
    
    # and change functions that return a variety of pointers to instead return unsigned int's
    Fix_Pointer_Returns ( ogre_ns )   

    # functions that need to have implicit conversions turned off
    Fix_Implicit_Conversions ( ogre_ns)
    
    
 
###############################################################################
##
## here are the helper functions that do much of the work
##
###############################################################################     

 
def Fix_Implicit_Conversions ( mb ):
    # and we need to remove the conversion here as radians doesn't work as expected
    cls=mb.class_('Radian')
    cls.constructors().allow_implicit_conversion = False
    cls=mb.class_('Degree')
    cls.constructors().allow_implicit_conversion = False
#     for cls in ogre_ns.class_( lambda cls: cls.name in ( 'Radian', 'Degree' ) ):
#        cls.constructors().allow_implicit_conversion = False
    
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
                    
def Add_Auto_Conversions( mb ):
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

                
def Set_Smart_Pointers( mb ):
    """ we need to identify 'smart pointer' which are any of the SharedPtr classes
    """
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
                
def Set_Exception(mb):
    """We don't exclude  Exception, because it contains functionality, that could
    be useful to user. But, we will provide automatic exception translator
    """
    Exception = mb.namespace( 'Ogre' ).class_( 'Exception' )
    Exception.include()
    Exception.mem_fun('what').exclude() # declared with empty throw
    Exception.mem_fun('getNumber').exclude() # declared with empty throw
    Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getFullDescription().c_str()' )
    
    ## there are two identical constructors so we need to remove one of them
    for c in Exception.constructors(arg_types=[None]):
        c.exclude() ## exclude the first constructor..
        break
        
    
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
                print "Fixed Void Ptr", fun, arg_position
                break
            arg_position +=1
    pointee_types=['unsigned int','int', 'float', '::Ogre::Real', '::Ogre::uchar', '::Ogre::uint8', 'unsigned char']
    for fun in mb.member_functions():
        arg_position = 0
        for arg in fun.arguments:
            if declarations.is_pointer(arg.type):
                for i in pointee_types:
                    if arg.type.decl_string.startswith ( i ):
                        print "NEED TO CHECK",fun, arg_position
                        break
            arg_position +=1
            
                    
def Fix_Pointer_Returns ( mb ):
    """ Change out functions that return a variety of pointer to base types and instead
    have them return the address the pointer is pointing to (the pointer value)
    This allow us to use CTypes to handle in memory buffers from Python
    
    Also - if documentation has been set then ignore the class/function as it means it's been tweaked else where
    """
    pointee_types=['unsigned int','int', 'float', '::Ogre::Real', '::Ogre::uchar', '::Ogre::uint8', 'unsigned char']
    known_names=['ptr', 'useCountPointer']  # these are function names we know it's cool to exclude
    for fun in mb.member_functions():
        if declarations.is_pointer (fun.return_type):
            for i in pointee_types:
                if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                    if not fun.name in known_names:
                        print "Excluding (function):", fun
                    fun.exclude()
    for fun in mb.member_operators():
        if declarations.is_pointer (fun.return_type):
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
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
    
        
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()
    ogre_ns = global_ns.namespace( 'Ogre' )
    ogre_ns.include()
    
# #     ogre_ns.class_( "StaticGeometry" ).class_("Region").include()
# #     ogre_ns.class_( "StaticGeometry" ).class_("LODBucket").include()
# #     ogre_ns.class_( "StaticGeometry" ).class_("MaterialBucket").include()
    
    AutoExclude ( mb )
    ManualExclude ( mb )
    AutoInclude ( mb )
    ManualInclude ( mb )
    AutoFixes ( mb )
    ManualFixes ( mb )
    
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    
    #Py++ can not expose static pointer member variables
    ogre_ns.vars( 'ms_Singleton' ).disable_warnings( messages.W1035 )
    
    # Ogre is "special" in that some classes are unnnamed and need fixing
    common_utils.fix_unnamed_classes( ogre_ns.classes( name='' ), 'Ogre' )
    print "8", ogre_ns.class_( "StaticGeometry" ).class_("Region").ignore
    
    common_utils.configure_shared_ptr(mb)
    print "9", ogre_ns.class_( "StaticGeometry" ).class_("Region").ignore
    
    Set_Exception( mb )
        
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    Set_Call_Policies ( mb.global_ns.namespace ('Ogre') )
    print "10", ogre_ns.class_( "StaticGeometry" ).class_("Region").ignore
    
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )
    print "11", ogre_ns.class_( "StaticGeometry" ).class_("Region").ignore
    
   
    NoPropClasses = ["UTFString"]
    for cls in ogre_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
        ## because we want backwards pyogre compatibility lets add leading lowercase properties
        common_utils.add_LeadingLowerProperties ( cls )

    common_utils.add_constants( mb, { 'ogre_version' :  '"%s"' % environment.ogre.version.replace("\n", "\\\n") 
                                      , 'python_version' : '"%s"' % sys.version.replace("\n", "\\\n" ) } )

    print "12", ogre_ns.class_( "StaticGeometry" ).class_("Region").ignore
   ##sys.exit()
    print "13", ogre_ns.class_( "StaticGeometry" ).typedef("RegionMap").ignore

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

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generaated directory..
    additional_files=[
            os.path.join( environment.shared_ptr_dir, 'py_shared_ptr.h'),
            os.path.join( os.path.abspath(os.path.dirname(__file__) ), 'python_ogre_masterlist.h' ),
            os.path.join( os.path.abspath(os.path.dirname(__file__) ), 'generators.h' ),
            os.path.join( os.path.abspath(os.path.dirname(__file__) ), 'custom_rvalue.cpp' ),
            os.path.join( environment.include_dir, 'tuples.hpp' )
            ]            
    for sourcefile in additional_files:
        p,filename = os.path.split(sourcefile)
        destfile = os.path.join(environment.ogre.generated_dir, filename ) 
    
        if not common_utils.samefile( sourcefile ,destfile ):
            shutil.copy( sourcefile, environment.ogre.generated_dir )
            print "Updated ", filename, "as it was missing or out of date"
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Python-OGRE source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
