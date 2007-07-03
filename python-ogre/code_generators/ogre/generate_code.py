#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------
"""  To Do list:

Continue to ignore ptr() functions in Vectors2/3/4 and Matrix 2/3/4 - these simple return a pointer to the start of the 
underlying data memebers and can be accessed in python with matrix3.x, matrix3.y etc

Functions with Matrices and Vertices need to be managed - as they expect a pointer to an array of Matrix4/Vector3 etc
Review all the functions with 'Check' beside them in the output as they probably don't work........

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
import register_exceptions

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
HACK = True
## small helper function
def docit ( general, i, o ): 
    docs = "Python-Ogre Modified Function Call\\n" + general +"\\n"
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
    ogre_ns = global_ns.namespace( 'Ogre' )
    
    ## Specifically remove functions that we have wrapped in hand_made_wrappers.py
    ogre_ns.class_( "RenderTarget" ).member_functions( 'getCustomAttribute' ).exclude()

    # hand made wrapper to return correct types
    global_ns.class_('::Ogre::ResourceManager').mem_fun('getByName').exclude()
    global_ns.class_('::Ogre::ResourceManager').mem_fun('getByHandle').exclude()
    global_ns.class_('::Ogre::ResourceManager').mem_fun('load').exclude()
    global_ns.class_('::Ogre::ResourceManager').mem_fun('create').exclude()
    

    global_ns.class_('::Ogre::Node').member_functions('getChild').exclude()
    global_ns.class_('::Ogre::Node').member_functions('removeChild').exclude()
#     global_ns.class_('::Ogre::Node').member_functions('createChild').exclude()
    global_ns.class_('::Ogre::Node').member_functions('getParent').exclude()
    
    global_ns.class_('::Ogre::OverlayElement').mem_fun('findElementAt').exclude()
    global_ns.class_('::Ogre::OverlayElement').mem_fun('clone').exclude() # virtual, but not in code.
    
    startswith = [
        'WIN32'
        , 'MemoryManager'   ## it's a specialised C++ mem manger not needed in Python
        , 'RadixSort' ## these show up as ugly aliases but are never exposed - and are really protected
        , 'Angle'   ## obtuse implicit conversion between Radian and Degree - causes compile issues 
        , 'StringConverter' ## the tostring introduces registration order issues which means it doesn't really work
    ]

    
    ## Now get rid of a wide range of classes as defined earlier in startswith...
    for prefix in startswith:
        ### NOTE the PREFIX is used here !!!!
        classes = ogre_ns.classes( common_utils.decl_starts_with(prefix), allow_empty=True)
        classes.exclude()
        
    #AJM Set of functions in Particle system that don't get wrapped properly.. Rechecked 30Nov06 AJM
    ## Other 'Cmd..' classes are defined as _OgrePrivate, whereas these are not in the head file
    PartSys = ogre_ns.class_( "ParticleSystem" )
    PartSys.class_( "CmdIterationInterval" ).exclude()
    PartSys.class_( "CmdLocalSpace" ).exclude()
    PartSys.class_( "CmdNonvisibleTimeout" ).exclude()
    PartSys.class_( "CmdSorted" ).exclude()

    ## Functions defined in .h files but not implemented in source files
#     ogre_ns.class_('Root').mem_fun('termHandler').exclude()
#     ogre_ns.class_( "StaticGeometry" ).class_("Region").member_functions('getLights').exclude() 

    #exclude GpuLogicalIndexUseMap  NOTE:  Example use of Py++ to exclude a special variable........
    GpuLogicalBufferStruct = ogre_ns.class_( 'GpuLogicalBufferStruct' )
    GpuLogicalBufferStruct.variable( 'map' ).exclude()   
    
    ## THIS IS A UNION
    ogre_ns.class_('GpuProgramParameters').class_('AutoConstantEntry').variable('data').exclude()    
    v = ogre_ns.class_('GpuProgramParameters').class_('AutoConstantEntry').variable('fData')
    v.exclude() 
    
    
    # functions that take pointers to pointers 
    ogre_ns.class_( 'VertexElement').member_functions('baseVertexPointerToElement').exclude() ## now as a transformed funct
    mb.global_ns.mem_fun('::Ogre::InstancedGeometry::BatchInstance::getObjectsAsArray').exclude()
    #all constructors in this class are private, also some of them are public.

    ogre_ns.free_functions ('any_cast').exclude () #not relevant for Python

    #AttribParserList is a map from string to function pointer, this class could not be exposed
    AttribParserList = ogre_ns.typedef( name="AttribParserList" )
    declarations.class_traits.get_declaration( AttribParserList ).exclude()
    ## AJM Error at compile time - errors when compiling or linking
    ogre_ns.calldefs ('peekNextPtr').exclude ()
    ogre_ns.calldefs ('peekNextValuePtr').exclude ()    #in many of the Iterator classes
    
        
    ogre_ns.class_( "ErrorDialog" ).exclude()   # doesn't exist for link time
    ogre_ns.class_( 'CompositorInstance').class_('RenderSystemOperation').exclude() # doesn't exist for link time
    ogre_ns.class_( 'CompositorChain').mem_fun('_queuedOperation').exclude() #needs RenderSystemOperation
    
   
   ## changes due to expanded header file input
    try:             
        ogre_ns.class_('OptimisedUtil').mem_fun('softwareVertexSkinning').exclude  # this isn't in the LINUX include for 1.4.1  
    except:
        pass  
    ogre_ns.class_('ShadowVolumeExtrudeProgram').variable('programNames').exclude()    #funky string[8] problem
            
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
    
    # this one expects a pointer to matrix4 and will return mBoneList.size() of them
    global_ns.class_('::Ogre::Skeleton').mem_fun('_getBoneMatrices').exclude()
    
    #new in Ogre 1.4 RC2
    global_ns.class_('::Ogre::SceneManager').mem_fun('getQueuedRenderableVisitor').exclude()
    # in hand made wrappers
    global_ns.class_('::Ogre::BillboardSet').mem_fun('getTextureCoords').exclude()
    global_ns.class_('::Ogre::BillboardSet').mem_fun('setTextureCoords').exclude()
#     global_ns.class_('::Ogre::RenderQueueListener').mem_fun('renderQueueStarted').exclude()
#     global_ns.class_('::Ogre::RenderQueueListener').mem_fun('renderQueueEnded').exclude()

    ## as we now include all protected functions tere are a couple of problem areas that popped up
    ogre_ns.constructor("IndexData",arg_types=['::Ogre::IndexData const &']).exclude()
    global_ns.class_('::Ogre::OverlayManager').\
        mem_fun('destroyOverlayElementImpl', arg_types=['::Ogre::OverlayElement *',None] ).exclude()

    ## change due to CVS Ogre update (Thanks Dermont)
    AttribParserList = ogre_ns.typedef( name="AttribParserList" )
    declarations.class_traits.get_declaration( AttribParserList ).exclude()

    ogre_ns.class_( 'MaterialSerializer' ).mem_fun('invokeParser').exclude()
    
    ogre_ns.class_('OverlayManager' ).mem_fun('parseNewElement').exclude()




############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################
    
def ManualInclude ( mb ):
    global_ns = mb.global_ns
    ogre_ns = global_ns.namespace( 'Ogre' )
    
    ## It's a structure that doesn't get included by default...
    ogre_ns.class_("VertexBoneAssignment_s").include()
    # A couple of Std's that need exposing
    std_ns = global_ns.namespace("std")
    std_ns.class_("pair<unsigned, unsigned>").include()
    std_ns.class_("pair<bool, float>").include()
    if not HACK:
        std_ns.class_("pair<Ogre::SharedPtr<Ogre::Resource>, bool>").include()
    
    
    #RenderOperation class is marked as private, but I think this is a mistake
    ogre_ns.class_('RenderOperation').include()
    
    ## Now we find all << operators and expose them as __str__ methods..  Makes "print xx" work nicely
    ## we simply include any relevant << operators and Py++/Boost does the work for us
    for oper in global_ns.free_operators( '<<' ):
        rtype = declarations.remove_declarated( declarations.remove_reference( oper.return_type ) )
        # we don't want the ones from std::...
        if not( declarations.is_std_ostream( rtype ) or declarations.is_std_wostream( rtype ) ):
            continue
        type_or_decl = declarations.remove_declarated(
                    declarations.remove_const( 
                        declarations.remove_reference( oper.arguments[1].type ) ) )
        if not isinstance( type_or_decl, declarations.declaration_t ):
            continue
        # ugly hack until a patch gets applied to Ogre source
        Expose = True
        for c in ['StaticFaceGroup']:
            if c in type_or_decl.decl_string:
                Expose = False
                oper.exclude() ## need to specifically exclude now...
        if type_or_decl.ignore == False and Expose:
            print "OPERATOR<<:", oper
            oper.include()

        
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
            f.documentation=docit ("Return Type Change", "None", "Tuple with 8 Vector3's")
    ### NOTE that we "include" things here again as they've probably been excluded in AutoFixes..
    
    ## this one points to an array of [2] floats        
    c =ogre_ns.class_('BillboardChain').mem_fun('getOtherTextureCoordRange')
    c.call_policies = call_policies.convert_array_to_tuple( 2, call_policies.memory_managers.none )    
    c.include()
    c.documentation=docit ("Return Type Change", "None", "Tuple with 2 floats's")
            
    ## and these ones return
    c = ogre_ns.class_('Matrix4').operators('[]')
    c.call_policies= call_policies.convert_array_to_tuple( 4, call_policies.memory_managers.none )   
    c.include() 
    c.documentation=docit ("Return Type Change", "None", "Tuple with 4 floats's (the matrix 'line')")
    c = ogre_ns.class_('Matrix3').operators('[]')
    c.call_policies= call_policies.convert_array_to_tuple( 3, call_policies.memory_managers.none )    
    c.include()
    c.documentation=docit ("Return Type Change", "None", "Tuple with 3 floats's (the matrix 'line')")

    #VertexCacheProfiler constructor uses enum that will be defined later.
    #I will replace second default value to be int instead of enum
    #arg_types=[None,None] - 2 arguments, with whatever type
    VertexCacheProfiler = ogre_ns.constructor( 'VertexCacheProfiler', arg_types=[None,None] )
    VertexCacheProfiler.arguments[1].default_value = "int(%s)" % VertexCacheProfiler.arguments[1].default_value
      
    ### General fixes..... really only needed in Linux, but no harm in Windows
    c = mb.namespace( 'Ogre' ).class_( 'Skeleton' )
    c.mem_fun( '_mergeSkeletonAnimations' ).arguments[-1].default_value = '::Ogre::StringVector()'

            
    ## we apply smart ptr to sharedptr classes automatically, however these are harder to identify   
#     known = ['indexBuffer', 'vertexData', 'indexData']
#     for v in mb.member_variables():
#         if v.name in known:
#             print "Setting ", v," to smart pointer"
#             v.apply_smart_ptr_wa = True
# \   cls = mb.class_( "IndexData" )
#     v = cls.variable( "indexBuffer" )
#     v.apply_smart_ptr_wa = True
#     cls = mb.class_( "SubMesh" )
#     v = cls.variable( "vertexData" )
#     v.apply_smart_ptr_wa = True
#     v = cls.variable( "indexData" )
#     v.apply_smart_ptr_wa = True   
#     
    ## Functions that return objects we need to manage
    FunctionsToMemoryManage=[\
        '::Ogre::VertexData::clone',
        '::Ogre::IndexData::clone',
        '::Ogre::Pose::clone',
        '::Ogre::Animation::clone'
        ]
    for cls in FunctionsToMemoryManage:
        global_ns.mem_fun(cls).call_policies = call_policies.return_value_policy( call_policies.manage_new_object )
    
    # make UTFstrings behave as real Python strings..
    UTFString = mb.class_( 'UTFString' )
    UTFString.mem_fun( 'asUTF8' ).alias = '__str__'
    UTFString.mem_fun( 'asWStr' ).alias = '__unicode__'
    
    ## Math abs function is overloaded (with degree and radian) and doesn't know what to "return"
    ## so exclude the overloads except the one that takes a real...
#     cls = mb.class_("Math")
#     cls.member_functions("Abs").exclude()
#     cls.mem_fun('Abs', arg_types=['::Ogre::Real']).include()
    
    
# #     # ensure functions that take UTFString can also take a python string
# #     utf = ogre_ns.class_( 'UTFString' )
# #     def has_utf_type( utf, var ): 
# #         res =  declarations.is_same( utf, declarations.remove_const( var.type ) )
# # # #         if "String" in var.type.decl_string:
# # # #             print "***", utf, var
# # # #             print var.type
# # # #             print var.type.decl_string
# # # #             nake_type1 = declarations.type_traits.remove_declarated( utf )
# # # #             nake_type2 = declarations.type_traits.remove_declarated(  var.type  )
# # # #         
# # # #             print res, "Comparing NAKE ",nake_type1, nake_type2
# # # #             print res, "Comparing  ",declarations.remove_const( var.type ), utf
# #         if res:
# #             print "UTFFIX", utf,var,var.type
# #         return res
# #     ogre_ns.variables( lambda var: has_utf_type( utf, var ) ).use_make_functions = True
# #     #ogre_ns.variables( lambda var: has_utf_type( utf, var ) ).apply_smart_ptr_wa  = True

    ## need some help here as the function overloads are causing issues
    f = global_ns.class_('::Ogre::GpuProgramParameters').\
        mem_fun('setNamedConstant', arg_types=['::Ogre::String const &','::Ogre::Real'] )
    f.arguments[1].name="real"
    f = global_ns.class_('::Ogre::GpuProgramParameters').\
        mem_fun('setNamedConstant', arg_types=['::Ogre::String const &','int'] )
    f.arguments[1].name="int"
    
    
    f = global_ns.class_('::Ogre::GpuProgramParameters').\
        mem_fun('setNamedConstant', arg_types=['::Ogre::String const &','float const *', None, None] )
    f.add_transformation( ft.modify_type('val',_ReturnUnsignedInt ), alias='setNamedConstantFloat' )
    f.documentation = docit ("Modified Input Argument (val) to work with CTypes",
                                            "Argument val takes a CTypes.adddressof(xx)", "...")
            
    f = global_ns.class_('::Ogre::GpuProgramParameters').\
        mem_fun('setNamedConstant', arg_types=['::Ogre::String const &','double const *', None, None] )
    f.add_transformation( ft.modify_type('val',_ReturnUnsignedInt ), alias='setNamedConstantDouble' )
    f.documentation = docit ("Modified Input Argument (val) to work with CTypes",
                                            "Argument val takes a CTypes.adddressof(xx)", "...")
        
    f = global_ns.class_('::Ogre::GpuProgramParameters').\
        mem_fun('setNamedConstant', arg_types=['::Ogre::String const &','int const *', None, None] )
    f.add_transformation( ft.modify_type('val',_ReturnUnsignedInt ), alias='setNamedConstantInt' )
    f.documentation = docit ("Modified Input Argument (val) to work with CTypes",
                                            "Argument val takes a CTypes.adddressof(xx)", "...")
    
    
    
    #.default_value = "int(%s)" % VertexCacheProfiler.arguments[1].default_value    

##
# fix up any ugly name alias
##
def ManualAlias ( mb ):
    return
    AliasFixList = [
    ["::Ogre::SceneManager::estimateWorldGeometry",
    ["::Ogre::DataStreamPtr &", "::Ogre::String const &"],
    ft.modify_type('typename',declarations.remove_reference),
    "estimateWorldGeometry"],
    ]
    
    for fix in AliasFixList:
        c=mb.member_function(fix[0] ,arg_types=fix[1])
        c.add_transformation( fix[2], alias=fix[3])
             
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
        
    rt_cls = ns.class_('RenderTarget')
    x=rt_cls.mem_fun('getMetrics')
    x.add_transformation( *create_output(3) )
    x.documentation = docit ( "","no arguments", "tuple containing width, height, colourDepth")
    
    x=rt_cls.mem_fun( 'getStatistics', arg_types=['float &']*4 )
    x.add_transformation( ft.output(0),ft.output(1),ft.output(2),ft.output(3), alias="getStatisticsList" )
    x.documentation = docit ("", "no arguments", "tuple - lastFPS, avgFPS, bestFPS, worstFPS")
    
    x = ns.mem_fun('::Ogre::RenderQueueListener::renderQueueEnded')
    x.add_transformation(ft.inout('repeatThisInvocation'))
    #x.documentation = docit ("","queueGroupId, invocation", "tuple - repeatThisInvocation")
    
    x = ns.mem_fun('::Ogre::RenderQueueListener::renderQueueStarted') 
    x.add_transformation(ft.inout('skipThisInvocation'))
    #x.documentation = docit ("","queueGroupId, invocation", "tuple - repeatThisInvocation")
    
    x=ns.mem_fun('::Ogre::RenderWindow::getMetrics')
    x.add_transformation( *create_output(5) )
    x.documentation = docit ("","no arguments", "tuple - width, height, colourDepth, left, top")
    
    x=ns.mem_fun('::Ogre::Viewport::getActualDimensions')
    x.add_transformation( *create_output(4) )
    x.documentation = docit ("","no arguments", "tuple - left, top, width, height")
    
    x=ns.mem_fun('::Ogre::BillboardSet::getParametricOffsets')
    x.add_transformation( *create_output(4) )
    x.documentation = docit ("","no arguments", "tuple - left, right, top, bottom")
    
    x=ns.mem_fun('::Ogre::Compiler2Pass::isFloatValue')
    x.add_transformation( *create_output(2) )
    x.documentation = docit ("","no arguments", "tuple - Return Value(True/False), fvalue, charsize")
    
    x=ns.mem_fun('::Ogre::UTFString::_utf16_to_utf32')
    x.add_transformation( ft.output('out_uc') )
    x.documentation = docit ("","uint16", "tuple - size_t, out character")
    
    x=ns.mem_fun('::Ogre::UTFString::_utf8_to_utf32')
    x.add_transformation( ft.output('out_uc') )
    x.documentation = docit ("","char", "tuple - size_t, out character")
    
    x=ns.mem_fun('::Ogre::Frustum::calcProjectionParameters')
    x.add_transformation( *create_output(4) )
    x.documentation = docit ("","no arguments", "tuple - left, right, bottom, top")
    
    x=ns.mem_fun('::Ogre::StaticGeometry::getRegionIndexes')
    x.add_transformation( ft.output('x'), ft.output('y'), ft.output('z') )
    x.documentation = docit ("","Vector", "tuple - x,y,z")
    
    x=ns.mem_fun('::Ogre::InstancedGeometry::getBatchInstanceIndexes')
    x.add_transformation( ft.output('x'), ft.output('y'), ft.output('z') )
    x.documentation = docit ("","Vector", "tuple - x,y,z")
    
    x=ns.mem_fun('::Ogre::CompositorChain::RQListener::renderQueueStarted')
    x.add_transformation(ft.inout("skipThisQueue"))
    x.documentation = docit ("", "id, invocation", "skipThisQueue" )
    
    x=ns.mem_fun('::Ogre::CompositorChain::RQListener::renderQueueEnded') 
    x.add_transformation(ft.inout("repeatThisQueue"))
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
      
    x=ns.mem_fun('::Ogre::PixelUtil::getBitDepths')
    x.add_transformation(ft.output_static_array('rgba',4) )
    x.documentation = docit ("", "format", "rgba" )
    
    x=ns.mem_fun('::Ogre::PixelUtil::getBitMasks')
    x.add_transformation(ft.output_static_array('rgba',4) )
    x.documentation = docit ("", "format", "rgba" )

    
    ## these need updates to Py++ to handle pointers 
# #     x = ns.mem_fun('::Ogre::PixelUtil::unpackColour', arg_types=['float *','float *','float *','float *',None,None])
# #     x.add_transformation(ft.output('r'), ft.output('g'), ft.output('b'), ft.output('a') )
# #     x.documentation = docit ("", "Pixelformat, src", "r,g,b,a" )

# #     x = ns.mem_fun('::Ogre::Frustum::projectSphere')
# #     x.add_transformation(ft.output('left'), ft.output('top'), ft.output('right'), ft.output('bottom') )
# #     x.documentation = docit ("", "Sphere", "result, left, top, right, bottom" )
# #             
# #     x = ns.mem_fun('::Ogre::Camera::projectSphere')
# #     x.add_transformation(ft.output('left'), ft.output('top'), ft.output('right'), ft.output('bottom') )
# #     x.documentation = docit ("", "Sphere", "result, left, top, right, bottom" )
    
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
    
# # #     x = ns.mem_fun('::Ogre::RenderQueue::RenderableListener::renderableQueued')
# # #     x.add_transformation(ft.output('ppTech') )
# # #     x.documentation = docit ("UNTESTED", "rend, groupID, priority", "ppTech" )
    
    ##
    ## now we handle some specials..
    ##


    pixelBox_size = """ 
namespace{ 
struct PixelBoxSize{ 
    bp::ssize_t operator()( boost::python::tuple args ) const{ 
        boost::python::object self = args[0];
        Ogre::PixelBox& pb = boost::python::extract<Ogre::PixelBox&>( self ); 
        return pb.getSize(); 
    } 
}; 
} 
""" 

    image_size = """ 
namespace{ 
struct ImageSize{ 
    bp::ssize_t operator()( boost::python::tuple args ) const{ 
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
    bp::ssize_t operator()( boost::python::tuple args ) const{ 
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
    bp::ssize_t operator()( boost::python::tuple args ) const{ 
        boost::python::object self = args[0]; 
        Ogre::GpuProgramParameters& gpupp = boost::python::extract<Ogre::GpuProgramParameters&>( self ); 
        boost::python::object pos_obj = args[1]; 
        bp::ssize_t offset = boost::python::extract<bp::ssize_t>( pos_obj ); 
        return gpupp.intBufferSize - offset; 
        }
}; 
struct GpuProgramParametersGetFloatPointerSize{ 
    bp::ssize_t operator()( boost::python::tuple args ) const{ 
        boost::python::object self = args[0]; 
        Ogre::GpuProgramParameters& gpupp = boost::python::extract<Ogre::GpuProgramParameters&>( self ); 
        boost::python::object pos_obj = args[1]; 
        bp::ssize_t offset = boost::python::extract<bp::ssize_t>( pos_obj ); 
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
    query = declarations.access_type_matcher_t( 'private' ) \
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
    
    if os.name =='nt':
        Fix_NT( mb )
    elif os.name =='posix':
        Fix_Posix( mb )
        
    Auto_Document( mb )
 
###############################################################################
##
## here are the helper functions that do much of the work
##
###############################################################################  
def Auto_Document ( mb ):
    """Indicate that the functions being exposed are declated protected or private in the C++ code
    this should warn people to be careful using them :) """
    global_ns = mb.global_ns
    ogre_ns = global_ns.namespace( 'Ogre' )

    query = declarations.access_type_matcher_t( 'private' ) 
    for c in ogre_ns.calldefs( query, allow_empty=True ):
        print "PRIVATE:", c
        s = c.documentation
        if not s:
            s = ""
        c.documentation="<<private declaration>>\\n"+s
    query = declarations.access_type_matcher_t( 'protected' ) 
    for c in ogre_ns.calldefs( query, allow_empty=True ):
        print "PROTECTED:", c
        s = c.documentation
        if not s:
            s = ""
        c.documentation="<<protected declaration>>\\n"+s
        
  
def Fix_Posix ( mb ):
    """ fixup for posix specific stuff -- note only expect to be called on a posix machine
    """
    ## we could do more here if need be...
    if sys.platform == 'darwin':
        pass
    elif sys.platform.startswith ('linux'):
        pass
    mb.global_ns.class_('vector<int, std::allocator<int> >').alias='VectorInt'
    mb.global_ns.class_('vector<std::pair<unsigned, unsigned>, std::allocator<std::pair<unsigned, unsigned> > >').alias='VectorUnsignedUnsigned'
    #as reported by mike with linux:bp::arg("flags")=(std::_Ios_Fmtflags)0
    mb.namespace( 'Ogre' ).class_('StringConverter').member_functions('toString').exclude()    

    ## grab the operator== and operator!= and exclude them
    ## NOTE: Defination for these are "extern bool..." so I wonder if we should exclude any "extern" operators
    for o in mb.namespace('Ogre').free_operators(arg_types=['::Ogre::ShadowTextureConfig const &', 
            '::Ogre::ShadowTextureConfig const &'], allow_empty=True):
        o.exclude()
        
    ## And even though we have excluded the operators we also need to exclude the equality 
    ## otherwise it causes undefined symbols __ZN4OgreeqERKNS_19ShadowTextureConfigES2_
    ## -- change file is --_ShadowTextureConfig__value_traits.pypp.hpp
    c = mb.namespace( 'Ogre' ).class_( 'ShadowTextureConfig' )
    c.equality_comparable = False



def Fix_NT ( mb ):
    """ fixup for NT systems
    """
    mb.global_ns.class_( 'vector<Ogre::Vector4, std::allocator<Ogre::Vector4> >' ).exclude( )
    Skeleton = mb.namespace( 'Ogre' ).class_( 'Skeleton' ).constructors().exclude()
    
    ## handle the hashmaps -- TODO FIX under LINUX ???
    stdex_ns = mb.global_ns.namespace("stdext")
    for cls in stdex_ns.classes():
        if cls.name.startswith ("hash"):
            cls.include()
            
    # need to force these
    hwnd = mb.global_ns.class_("HWND__")
    hwnd.opaque = True
    _iobuf = mb.global_ns.class_("_iobuf")# need the file handle in Ogre::FileHandleDataStream::FileHandleDataStream
    _iobuf.opaque = True
    
        
        
def Fix_Implicit_Conversions ( mb ):
    """By default we disable explicit conversion, however sometimes it makes sense
    """
    ImplicitClasses=['Radian','Degree', 'TimeIndex', 'LiSPSMShadowCameraSetup' ] 
    # AnimationStateControllerValue, Any, SceneQuery, 
    # BorderRenderable, SceneNode, CompositionPass, CompositionTargetPass, CompositionTechnique
    # CompositorChain, CompositorInstance::TargetOperation, TextureUnitState, DefaultAxisAlignedBoxSceneQuery
    # DefaultIntersectionSceneQuery, DefaultPlaneBoundedVolumeListSceneQuery, DefaultRaySceneQuery
      # DefaultSphereSceneQuery, DefaultSceneManager, 
    for className in ImplicitClasses:
        mb.class_(className).constructors().allow_implicit_conversion = True
 
    
    
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
                    fun.add_transformation( ft.modify_type(arg_position,declarations.remove_reference ), alias=fun.name )
            arg_position +=1
 
                               
def Add_Auto_Conversions( mb ):
    """
    Allows to pass Python tuple as argument to function, instead of
       * ColourValue
       * Vector[2|3|4]
       * Matrix[3|4]
       * Quaternion
       * UTFString's (passing a string)
    """
    rvalue_converters = ( 
        'register_pytuple_to_colour_value_conversion'
        , 'register_pytuple_to_vector2_conversion'
        , 'register_pytuple_to_vector3_conversion'
        , 'register_pytuple_to_vector4_conversion'
        , 'register_pystring_to_utfstring_conversion' 
        , 'register_pytuple_to_quaternion_conversion'
        , 'register_pytuple_to_matrix3_conversion' )
# #         , 'register_pytuple_to_matrix4_conversion' ) ## can't do this until we extend tuple template in boost
        
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
    """ we need to identify 'smart pointers' which are any of the SharedPtr classes
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
    
    # now I want to get some specials, the are not classes so haven't been found so far
    # this is potentially too broad brushed in it's approach but seems OK so far -- however if
    # need bewe could check for specific classes - the current list being:
    #
    # InstancedGeometry,SubMeshLodGeometryLink,OptimisedSubMeshGeometry,SubMesh
    # StaticGeometry,SubMeshLodGeometryLink,OptimisedSubMeshGeometry,
    # RenderOperation,IndexData,EdgeListBuilder,Geometry,EdgeData,EdgeGroup
                  
    known = ['indexBuffer', 'vertexData', 'indexData']
    for c in mb.classes():
        for v in c.variables(allow_empty = True ):
            if v.name in known:
               v.apply_smart_ptr_wa = True    
               print "Applying Smart Pointer (know): ",  v.name, " of class: ",  c.name
           
                           
#~ def Set_Exception(mb):
    #~ """We don't exclude  Exception, because it contains functionality, that could
    #~ be useful to user. But, we will provide automatic exception translator
    #~ """
    #~ Exception = mb.namespace( 'Ogre' ).class_( 'Exception' )
    #~ Exception.include()
    #~ Exception.mem_fun('what').exclude() # declared with empty throw
    #~ Exception.mem_fun('getNumber').exclude() # declared with empty throw
    #~ Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getFullDescription().c_str()' )
    
#~ # #     ## there are two identical constructors so we need to remove one of them
#~ # #     for c in Exception.constructors(arg_types=[None]):
#~ # #         c.exclude() ## exclude the first constructor..
#~ # #         break
        
    
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
                fun.add_transformation( ft.modify_type(arg_position,_ReturnUnsignedInt ), alias=fun.name )
                fun.documentation = docit ("Modified Input Argument to work with CTypes",
                                            "Argument "+arg.name+ "(pos:" + str(arg_position)\
                                            +") takes a CTypes.adddressof(xx)", "...")
                #print "Fixed Void Ptr", fun, arg_position
                break
            arg_position +=1
            
    ## lets go and look for stuff that might be a problem        
    pointee_types=['unsigned int','int', 'float', '::Ogre::Real', '::Ogre::uchar', '::Ogre::uint8',
             'unsigned char', 'Matrices', 'Vertices']
    function_names=['Matrices', 'Vertices']
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
                        print "CHECK ", fun, str(arg_position)
                        fun.documentation=docit("SUSPECT - MAYBE BROKEN", "....", "...")
                        break
            arg_position +=1

    
    ## Now we look for pointers to particular types and decide what to do with them??        
# # #     pointee_types=['unsigned int','int', 'float', '::Ogre::Real', '::Ogre::uchar', '::Ogre::uint8',
# # #              'unsigned char', '::Ogre::Matrix4']
# # #     for fun in mb.member_functions():
# # #         arg_position = 0
# # #         trans=[]
# # #         desc=""
# # #         for arg in fun.arguments:
# # #             if declarations.is_pointer(arg.type) and "const" not in arg.type.decl_string:
# # #                 for i in pointee_types:
# # #                     if arg.type.decl_string.startswith ( i ):
# # #                         trans.append(ft.output (arg_position ) )
# # #                         desc = desc + " Transform " + arg.name + " (position:" + str(arg_position) +") as an output."
# # #                         break
# # #             arg_position +=1
# # #         if trans:
# # #             print "Tranformation applied to ", fun, desc
# # #             fun.add_transformation ( * trans )
# # #             fun.documentation = "Python-Ogre: Function Modified\\n\\\n" + desc

            
                    
def Fix_Pointer_Returns ( mb ):
    """ Change out functions that return a variety of pointer to base types and instead
    have them return the address the pointer is pointing to (the pointer value)
    This allow us to use CTypes to handle in memory buffers from Python
    
    Also - if documentation has been set then ignore the class/function as it means it's been tweaked else where
    """
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
# #         , messages.W1041 # properties that aren't exposed
        , messages.W1036 # pointer to Python immutable member
        , messages.W1033 # unnamed variables
        , messages.W1018 # expose unnamed classes
        , messages.W1049 # returns reference to local variable
        , messages.W1014 # unsupported '=' operator
         )
#     sort_algorithms.USE_CALLDEF_ORGANIZER = True   ## tried this to remove a couple of order issues, without success :)
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.ogre.root_dir, "python_ogre.h" )
                        , environment.ogre.cache_file )

    defined_symbols = [ 'OGRE_NONCLIENT_BUILD', 'OGRE_GCC_VISIBILITY']
    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        defined_symbols.append('BOOST_HAS_WINTHREADS')
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
    ogre_ns = global_ns.namespace( 'Ogre' )
    ogre_ns.include()
    
# #     ogre_ns.class_( "StaticGeometry" ).class_("Region").include()
# #     ogre_ns.class_( "StaticGeometry" ).class_("LODBucket").include()
# #     ogre_ns.class_( "StaticGeometry" ).class_("MaterialBucket").include()
    
    AutoExclude ( mb )
    ManualExclude ( mb )
    AutoInclude ( mb )
    ManualInclude ( mb )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    ManualAlias ( mb )
    AutoFixes ( mb )
    ManualFixes ( mb )
    
    
    #Py++ can not expose static pointer member variables
    ogre_ns.vars( 'ms_Singleton' ).disable_warnings( messages.W1035 )
    
    # Ogre is "special" in that some classes are unnnamed and need fixing
    common_utils.fix_unnamed_classes( ogre_ns.classes( name='' ), 'Ogre' )
    
    common_utils.configure_shared_ptr(mb)
    
    register_exceptions.register( mb )
        
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    Set_Call_Policies ( mb.global_ns.namespace ('Ogre') )
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

# #     cls= ogre_ns.class_( "Camera" )
# #     cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
# #     sys.exit()
# #     
    NoPropClasses = ["UTFString"]
    for cls in ogre_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            common_utils.remove_DuplicateProperties ( cls )
            ## because we want backwards pyogre compatibility lets add leading lowercase properties
            common_utils.add_LeadingLowerProperties ( cls )
            
    common_utils.add_constants( mb, { 'ogre_version' :  '"%s"' % environment.ogre.version.replace("\n", "\\\n") 
                                      , 'python_version' : '"%s"' % sys.version.replace("\n", "\\\n" ) } )
                                      
    ## need to create a welcome doc string for this...                                  
    common_utils.add_constants( mb, { '__doc__' :  '"Python-Ogre Wrapper Library"' } ) 
    
    
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor( "Ogre" ) # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_ogre_' , doc_extractor= extractor )
    
    for inc in environment.ogre.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ogre.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.ogre.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.ogre.version ) )

    mb.split_module(environment.ogre.generated_dir, huge_classes, use_files_sum_repository=False )

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
