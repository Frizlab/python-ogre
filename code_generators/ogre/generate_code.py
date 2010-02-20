#!/usr/bin/env python
#!/usr/bin/env python
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
_DEBUG=False
if not _DEBUG:
    try:
       import psyco
       psyco.full()
    except ImportError:
       pass

#add environment to the path
sys.path.append( os.path.join( '..', '..' ) )
#add common utils to the pass
sys.path.append( '..' )
sys.path.append( '.' )

##import pdb
import environment
import common_utils
import customization_data
import hand_made_wrappers
import register_exceptions
import re as re

import pygccxml
from pygccxml import parser
from pygccxml import declarations
from pyplusplus import messages
from pyplusplus import module_builder
from pyplusplus import decl_wrappers
from pyplusplus.code_creators import include

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
try:
  from pyplusplus.creators_factory import sort_algorithms
except ImportError, e:
  from pyplusplus.module_creator import sort_algorithms

import common_utils.extract_documentation as exdoc
import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties
from common_utils import docit


##
## EXPERIMENTAL for SWIG code generation
##
SWIG=False
if SWIG:
    from pyplusplus.module_builder import swig_module_builder_t
    import common_utils.swig_wrapper as swig


predefined_is_smart_ptr = declarations.smart_pointer_traits
class my_smart_ptr:
   @staticmethod
   def is_smart_pointer( v ):
        t = v
        if predefined_is_smart_ptr.is_smart_pointer( v ):
            return True
        v=t
        v = declarations.type_traits.remove_alias( v )
        v = declarations.type_traits.remove_cv( v )
        v = declarations.type_traits.remove_declarated( v )
        if not isinstance( v, ( declarations.class_declaration_t , declarations.class_t ) ):
            return False
        if not declarations.is_class( v ):
            return False

        cls = declarations.class_traits.get_declaration( v )
        if len(cls.bases) > 0:
            for b in cls.bases:
                r = b.related_class
                if r.decl_string.startswith ( '::Ogre::SharedPtr<' ) :
#                     print "MYSMART: Returning True:", cls
                    return True
        else:
            if cls.name.startswith ( 'SharedPtr<' ) or cls.name.endswith( 'SharedPtr' ):
#                 print "MYSMART: Returning True:", cls
                return True
        return False

   @staticmethod
   def value_type( type ):
      if my_smart_ptr.is_smart_pointer( type ):
          return declarations.internal_type_traits.get_by_name( type, "element_type" )

pygccxml.declarations.smart_pointer_traits = my_smart_ptr


HACK = True

MAIN_NAMESPACE = 'Ogre'

############################################################
##
##  Here is where we manually exclude stuff from Ogre -
##
############################################################

def ManualExclude ( mb ):
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns

    ## Specifically remove functions that we have wrapped in hand_made_wrappers.py
    main_ns.class_( "RenderTarget" ).member_functions( 'getCustomAttribute' ).exclude()

    # hand made wrapper to return correct types
    global_ns.class_('::Ogre::ResourceManager').mem_fun('getByName').exclude()
    global_ns.class_('::Ogre::ResourceManager').mem_fun('getByHandle').exclude()
    global_ns.class_('::Ogre::ResourceManager').mem_fun('load').exclude()
    global_ns.class_('::Ogre::ResourceManager').mem_fun('create').exclude()



    global_ns.class_('::Ogre::Node').member_functions('getChild').exclude()
    global_ns.class_('::Ogre::Node').member_functions('removeChild').exclude()
    global_ns.class_('::Ogre::Node').member_functions('getParent').exclude()

    # these need to be dynamically cast before returning their result
    global_ns.class_('::Ogre::OverlayElement').mem_fun('findElementAt').exclude()
    global_ns.class_('::Ogre::OverlayElement').mem_fun('clone').exclude()
# #     global_ns.class_('::Ogre::OverlayManager').mem_fun('createOverlayElement').exclude()


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
        classes = main_ns.classes( common_utils.decl_starts_with(prefix), allow_empty=True)
        classes.exclude()

    #AJM Set of functions in Particle system that don't get wrapped properly.. Rechecked 30Nov06 AJM
    ## Other 'Cmd..' classes are defined as _OgrePrivate, whereas these are not in the head file
    PartSys = main_ns.class_( "ParticleSystem" )
    PartSys.class_( "CmdIterationInterval" ).exclude()
    PartSys.class_( "CmdLocalSpace" ).exclude()
    PartSys.class_( "CmdNonvisibleTimeout" ).exclude()
    PartSys.class_( "CmdSorted" ).exclude()


    #exclude GpuLogicalIndexUseMap  NOTE:  Example use of Py++ to exclude a special variable........
    GpuLogicalBufferStruct = main_ns.class_( 'GpuLogicalBufferStruct' )
    GpuLogicalBufferStruct.variable( 'map' ).exclude()

    ## These are really unions
    main_ns.class_('GpuProgramParameters').class_('AutoConstantEntry').variable('data').exclude()
    main_ns.class_('GpuProgramParameters').class_('AutoConstantEntry').variable('fData').exclude()

    # functions that take pointers to pointers
    main_ns.class_( 'VertexElement').member_functions('baseVertexPointerToElement').exclude() ## now as a transformed funct

    if environment.ogre.version =="1.4":
        mb.global_ns.mem_fun('::Ogre::InstancedGeometry::BatchInstance::getObjectsAsArray').exclude()

    #all constructors in this class are private, also some of them are public.

    try: 
        main_ns.free_functions ('any_cast').exclude () #not relevant for Python
    except:
        pass

    #AttribParserList is a map from string to function pointer, this class could not be exposed
    AttribParserList = main_ns.typedef( name="AttribParserList" )
    declarations.class_traits.get_declaration( AttribParserList ).exclude()
    ## AJM Error at compile time - errors when compiling or linking
    main_ns.calldefs ('peekNextPtr').exclude ()
    main_ns.calldefs ('peekNextValuePtr').exclude ()    #in many of the Iterator classes

    main_ns.class_( "ErrorDialog" ).exclude()   # doesn't exist for link time
    main_ns.class_( 'CompositorInstance').class_('RenderSystemOperation').exclude() # doesn't exist for link time
    main_ns.class_( 'CompositorChain').mem_fun('_queuedOperation').exclude() #needs RenderSystemOperation

   ## changes due to expanded header file input
    try:
        main_ns.class_('OptimisedUtil').mem_fun('softwareVertexSkinning').exclude  # this isn't in the LINUX include for 1.4.1
    except:
        pass
    main_ns.class_('ShadowVolumeExtrudeProgram').variable('programNames').exclude()    #funky string[8] problem

    ## now for problem areas in the new unicode string handling - just excluding without 'thought' :)
    ## the variables are not present in the source (to check)
    ## most of the functions return pointers to 'stuff' that isn't handled at compile time
    main_ns.class_('UTFString').variable('mVoidBuffer').exclude()
    main_ns.class_('UTFString').variable('mStrBuffer').exclude()
    main_ns.class_('UTFString').variable('mWStrBuffer').exclude()
    main_ns.class_('UTFString').variable('mUTF32StrBuffer').exclude()
    main_ns.class_('UTFString').member_functions('at').exclude()
    main_ns.class_('UTFString').mem_fun('c_str').exclude()
    main_ns.class_('UTFString').mem_fun('data').exclude()
    main_ns.class_('UTFString').mem_fun('asUTF32_c_str').exclude()
    main_ns.class_('UTFString').variable('npos').exclude()


    try:
        main_ns.class_('CompositorChain').variable('BEST').exclude()
        main_ns.class_('CompositorChain').variable('LAST').exclude()
        main_ns.class_('Compiler2Pass').variable('SystemTokenBase').exclude()
    except:
        pass
    #main_ns.class_('AnimationTrack').variable('INVALID_KEY_INDEX').exclude()


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

    ## AJM 10/10/07 new excludes due to hand_make wrappers..
    global_ns.class_('::Ogre::SceneManager').mem_fun('setOption').exclude()
    global_ns.class_('::Ogre::SceneManager').mem_fun('getOption').exclude()
    global_ns.class_('::Ogre::ParticleSystem').mem_fun('_getIterator').exclude()

    ## AJM 26/09/08 Working to get renderqueue listener to wrap without changing Ogre API
#     global_ns.class_('::Ogre::RenderQueueListener').mem_fun('renderQueueStarted').exclude()
#     global_ns.class_('::Ogre::RenderQueueListener').mem_fun('renderQueueEnded').exclude()

    ## as we now include all protected functions there are a couple of problem areas that popped up
    main_ns.constructor("IndexData",arg_types=['::Ogre::IndexData const &']).exclude()
    global_ns.class_('::Ogre::OverlayManager').\
        mem_fun('destroyOverlayElementImpl', arg_types=['::Ogre::OverlayElement *',None] ).exclude()


    excludes = ['::Ogre::Any::getType'  ## this returns a std::type_info which doesn't make any sense in Python
    ]
    for e in excludes:
        print "excluding ", e
        main_ns.member_functions(e).exclude()


    ## change due to CVS Ogre update (Thanks Dermont)
    AttribParserList = main_ns.typedef( name="AttribParserList" )
    declarations.class_traits.get_declaration( AttribParserList ).exclude()

    main_ns.class_( 'MaterialSerializer' ).mem_fun('invokeParser').exclude()

    main_ns.class_('OverlayManager' ).mem_fun('parseNewElement').exclude()

    #Exclude non default constructors of iterator classes.
    for cls in main_ns.classes():
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
# #         print "Setting Iterator to copyable:", cls
#         print dir(cls)
#         sys.exit()
# #         cls.noncopyable = False

    ## Remove private classes , and those that are internal to Ogre...
    private_decls = common_utils.private_decls_t(environment.ogre.include_dirs)
    for cls in main_ns.classes():
#         print "MC:", cls
        if private_decls.is_private( cls ):
            cls.exclude()
            print '{*} Excluding class "%s" as marked as private' % cls.decl_string

    for func in main_ns.calldefs():
        if private_decls.is_private( func ):
            if func.virtuality == declarations.VIRTUALITY_TYPES.PURE_VIRTUAL:
                continue
            func.exclude()
            print '{*} Excluding function "%s" as marked as internal' % declarations.full_name( func )

    # this change was for 1.7 but also needed for 1.4
    noncopy=['Camera','Frustum', 'Log' ]

    # these showed up during threading -- possible needs to be done all the time (needs to be looked at)
    if environment._USE_THREADS:
        noncopy = noncopy + ['Compositor', 'DefaultHardwareBufferManager', 'DefaultSceneManager', 'Font', 'FontManager',
                             'HighLevelGpuProgramManager','Material', 'Mesh', 'MeshManager',
                             'ParticleSystemManager', 'Pass', 'PatchMesh', 'ResourceGroupManager',
                             'Skeleton', 'SkeletonInstance', 'SkeletonManager', 'UnifiedHighLevelGpuProgram']
    for c in noncopy:
        main_ns.class_(c).noncopyable = True


    # changes for Ogre after 1.4
    if not environment.ogre.version.startswith("1.4"):

# don't seem to be needed with 1.6.4
#        if not environment.ogre.version.startswith("1.7"):
#            main_ns.class_("ResourceGroupManager").mem_fun("_notifyWorldGeometryPrepareStageEnded").exclude()
#            main_ns.class_("ResourceGroupManager").mem_fun("_notifyWorldGeometryPrepareStageStarted").exclude()

        #needed for linux
        try:
            main_ns.member_function('::Ogre::ResourceGroupManager::addCreatedResource').exclude()
        except:
            pass
        # these don't exist
        main_ns.class_("ScriptCompiler").mem_fun("removeNameExclusion").exclude()
        main_ns.class_("ScriptCompiler").mem_fun("addNameExclusion").exclude()

        for cls in main_ns.classes():
            if cls.decl_string.startswith ("::Ogre::AllocatedObject") or\
                cls.decl_string.startswith("::Ogre::STLAllocator") or\
                cls.decl_string.startswith("::Ogre::CategorisedAllocPolicy") :
                    print "Excluding Allocator class", cls
                    cls.exclude()

    # turns out there are some variables we didn't catch that are actually classes of boost::recursive_mutex
    # which isn't exposed... May be related only to threading ??
    # could limit it to known classes however safer to handle everything
#     ec = ['Pass', 'ResourceBackgroundQueue', 'SceneManager']
    for c in main_ns.classes():
        for v in c.variables( allow_empty=True):
            if v.name.endswith('Mutex'):
                print "Excluding possible Mutex Variable:", v
                v.exclude()


    if environment.ogre.version.startswith("1.7"): 
        # most of these are issues with protected members so fail at compile time
        # and they have ugly long names so we do the crazy searching as below
        
        # typically one of the 'classes/variables' in the map/list/.. is protected so we could change this
        # list and do something intelligent -- a possible future improvement..
        ex = ['::Ogre::list<Ogre::Profiler::Profile',
              '::Ogre::map<std::string, Ogre::SceneManager::MovableObjectCollection*',
              '::Ogre::list<Ogre::ResourceGroupManager::ResourceLocation',
              '::Ogre::map<std::string, Ogre::Compiler2Pass::TokenState',
              '::Ogre::vector<Ogre::ProgressiveMesh::PMTriangle',
              '::Ogre::map<Ogre::CompositorManager::TextureDef',
              '::Ogre::map<Ogre::HardwareVertexBuffer*, Ogre::HardwareBufferManagerBase::VertexBufferLicense',
              '::Ogre::map<Ogre::Light*, Ogre::SceneManager::LightClippingInfo',
              '::Ogre::map<std::string, std::list<Ogre::Profiler::ProfileHistory',
              '::Ogre::map<Ogre::Pass*, std::vector<Ogre::Renderable*',
              '::Ogre::map<Ogre::Resource*',
              '::Ogre::map<std::string, Ogre::ResourceGroupManager::ResourceGroup*',
              '::Ogre::map<Ogre::Vector3, unsigned int, Ogre::EdgeListBuilder::vectorLess',
              '::Ogre::map<Ogre::Vector3, unsigned long, Ogre::EdgeListBuilder::vectorLess', #64 bit version
              '::Ogre::map<unsigned int, Ogre::Vector3',
              '::Ogre::set<Ogre::ProgressiveMesh::PMTriangle*',
              '::Ogre::set<Ogre::ProgressiveMesh::PMVertex*',
              '::Ogre::vector<Ogre::BillboardChain::ChainSegment',
              '::std::vector<Ogre::RenderWindowDescription',  # this one needs Memory Allocator namespace fixed
              '::Ogre::vector<Ogre::Compiler2Pass::LexemeTokenDef',
              '::Ogre::vector<Ogre::Compiler2Pass::TokenRule',
              '::Ogre::vector<Ogre::Compiler2Pass::TokenIns',
              '::Ogre::vector<Ogre::EdgeListBuilder::CommonVertex',
              '::Ogre::vector<Ogre::EdgeListBuilder::Geometry',
              '::Ogre::vector<Ogre::EdgeListBuilder::vectorLess',
              '::Ogre::vector<Ogre::GpuSharedParametersUsage::CopyDataEntry',
              '::Ogre::vector<Ogre::ProgressiveMesh::PMFaceVertex',
              '::Ogre::vector<Ogre::ProgressiveMesh::PMVertex',
              '::Ogre::vector<Ogre::ProgressiveMesh::PMTriangle',
              '::Ogre::vector<Ogre::ProgressiveMesh::PMWorkingData',
              '::Ogre::vector<Ogre::SceneManager::LightInfo',
              
 
              
              ]
#===============================================================================
#        c = main_ns.class_('::Ogre::ResourceGroupManager::ResourceGroup')
#        print "\nSPECIAL\n",c
#        print dir(c)
#        print type(c)
#        print c.bases
#        print c.decl_string
#        print c.parent
#        for con in c.public_members:
#            print "PM:",con
#===============================================================================
        
        for c in main_ns.classes():
#===============================================================================
#            if 'SharedPtr' not in c.decl_string:
#                if c.decl_string.startswith('::Ogre::map<'):
#                    l = c.decl_string.split(',')
#                    f=l[0][12:].rstrip('*&')
#                    s=l[1].lstrip().rstrip('*&')
#                    for cc in [f,s]:
#                        print "Checking for:", cc
#                        if cc.startswith ( 'Ogre' ):
#                            try:
#                                cclass=main_ns.member_variable('::'+cc)
#                                if cclass.access_type != 'public':
#                                    print "ISSUE with:", c
#                                    c.exclude()
#                            except:
#                                pass
#                elif c.decl_string.startswith('::Ogre::list<') or c.decl_string.startswith('::Ogre::vector<'):
#                    l = c.decl_string.split('<') # first get rid of pre <
#                    l = l[1].split(',') # now get first class
#                    cc=l[0].lstrip().rstrip('*&')
#                    print "Checking for:", cc
#                    if cc.startswith ( 'Ogre' ):
#                        try:
#                            cclass=main_ns.member_variable('::'+cc)
#                            if cclass.access_type != 'public':
#                                print "ISSUE with:", c
#                                c.exclude()
#                        except:
#                            pass        
#===============================================================================
            for e in ex:
                if c.decl_string.startswith(e):
                    print "Excluding:", c
                    c.exclude()
                    
        excludelist= ['::Ogre::FileHandleDataStream', # uses FILE (c style) -- can use FileStreamDataStream instead
                      '::Ogre::Node::DebugRenderable', # not implemented
                      ]
        for c in excludelist:
            try:
                main_ns.class_(c).exclude()
                print "Excluded:", c
            except:
                pass
        excludelist= ['::Ogre::HashedVector<Ogre::Light*>::resize',  # compile time issue
                      '::Ogre::HashedVector<Ogre::Light*>::at',
                      '::Ogre::SceneManager::_pauseRendering', # bug in source needs fixing
                      '::Ogre::SceneManager::_resumeRendering',
                      #'::Ogre::CompositorManager::getCustomCompositionPass',
                      '::Ogre::NedPoolingPolicy::deallocateBytes',
                      
                      # these need the source patched -- they are implemented in the header file not the cpp one
                      '::Ogre::Mesh::unnameSubMesh',
                      '::Ogre::OverlayManager::hasOverlayElementImpl',
                      '::Ogre::OverlayManager::hasOverlayElement',
                      '::Ogre::Rectangle2D::setCorners',
                      #'::Ogre::RenderSystem::getVertexWindingInverted',
                      #'::Ogre::RenderSystem::getVertexWindingInverted'
                      '::Ogre::TextureUnitState::setCompositorReference',
                      '::Ogre::TextureUnitState::getReferencedMRTIndex',
                      '::Ogre::TextureUnitState::getReferencedTextureName',
                      '::Ogre::TextureUnitState::getReferencedCompositorName',
                      ]
        for c in excludelist:
            try:
                main_ns.member_functions(c).exclude()
                print "Excluded:", c
            except:
                print "ERROR: Unable to exclude:", c


####################b########################################
##
##  And there are things that manually need to be INCLUDED
##
############################################################

def ManualInclude ( mb ):
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )

    ## It's a structure that doesn't get included by default...
    main_ns.class_("VertexBoneAssignment_s").include()
    # A couple of Std's that need exposing
    std_ns = global_ns.namespace("std")


    includes = ['vector<bool>',
        'vector<int>',
        'vector<long>',
        'vector<float>',
        'vector<unsigned int>',
        'vector<char>',
        'vector<unsigned char>',
        'vector<float>',
        'pair<unsigned int, unsigned int>',
        "pair<bool, float>",
        "pair<Ogre::SharedPtr<Ogre::Resource>, bool>",
        "list<Ogre::SceneNode*>",
        "pair<bool, std::string>"
        "pair<std::string const, Ogre::SharedPtr<Ogre::Resource> >",
 ##       "pair<std::string const, Ogre::MovableObject*>",
 ##       "pair<std::string const, Ogre::Node*>",
        "pair<bool, Ogre::SharedPtr<Ogre::Resource> >",
        "pair<bool, Ogre::Vector3>",
        'pair<unsigned char, unsigned char>',
        ]

    for c in includes:
      try:
         std_ns.class_(c).include()
         print "Included std class:", c
      except:
         print "Problem including:", c

    if not environment.ogre.version =="1.4":
        main_ns.class_("AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> >").include()
        main_ns.class_("AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)1> >").include()
        main_ns.class_("AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)2> >").include()
        main_ns.class_("AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)3> >").include()
        main_ns.class_("AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)4> >").include()
        main_ns.class_("AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)5> >").include()
        main_ns.class_("AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)6> >").include()
        main_ns.class_("AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)7> >").include()
    
#        main_ns.class_("::Ogre::AnyNumUshort").include()
    
    #RenderOperation class is marked as private, but I think this is a mistake
    main_ns.class_('RenderOperation').include()

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
    main_ns.class_('MaterialPtr').include()

    ex = ['::Ogre::EdgeListBuilder::vectorLess',
          '::Ogre::EdgeListBuilder::CommonVertex'
        ]
    for c in ex:
        try:
            main_ns.class_(c).exclude()
            print "Excluded:", c
        except:
            print "Failed to Exclude:", c

############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixesPreAuto ( mb ):
    """ fixes that need to be applied before the automatic fixes """
    pass

def ManualFixes ( mb ):

    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )

    # return arrays
    ##  const Vector3* ----
    for f in main_ns.mem_funs( return_type='::Ogre::Vector3 const *', allow_empty=True):
        if f.name.startswith("get") and "Corner" in f.name:
            f.call_policies = call_policies.convert_array_to_tuple( 8, call_policies.memory_managers.none )
            f.include()
            f.documentation=docit ("Return Type Change", "None", "Tuple with 8 Vector3's")
    ### NOTE that we "include" things here again as they've probably been excluded in AutoFixes..

    ## this one points to an array of [2] floats
    c =main_ns.class_('BillboardChain').mem_fun('getOtherTextureCoordRange')
    c.call_policies = call_policies.convert_array_to_tuple( 2, call_policies.memory_managers.none )
    c.include()
    c.documentation=docit ("Return Type Change", "None", "Tuple with 2 floats's")

    ## and these ones return
    c = main_ns.class_('Matrix4').operators('[]')
    c.call_policies= call_policies.convert_array_to_tuple( 4, call_policies.memory_managers.none )
    c.include()
    c.documentation=docit ("Return Type Change", "None", "Tuple with 4 floats's (the matrix 'line')")
    c = main_ns.class_('Matrix3').operators('[]')
    c.call_policies= call_policies.convert_array_to_tuple( 3, call_policies.memory_managers.none )
    c.include()
    c.documentation=docit ("Return Type Change", "None", "Tuple with 3 floats's (the matrix 'line')")

    #VertexCacheProfiler constructor uses enum that will be defined later.
    #I will replace second default value to be int instead of enum
    #arg_types=[None,None] - 2 arguments, with whatever type
    VertexCacheProfiler = main_ns.constructor( 'VertexCacheProfiler', arg_types=[None,None] )
    VertexCacheProfiler.arguments[1].default_value = "int(%s)" % VertexCacheProfiler.arguments[1].default_value

    ### General fixes..... really only needed in Linux, but no harm in Windows
    c = mb.namespace( MAIN_NAMESPACE ).class_( 'Skeleton' )
    c.mem_fun( '_mergeSkeletonAnimations' ).arguments[-1].default_value = '::Ogre::StringVector()'

    ## Special due to bug in gccxml
    f=main_ns.class_('PatchMesh').mem_fun('define')
    f.arguments[4]._set_default_value ('::Ogre::PatchSurface::AUTO_LEVEL')
    f.arguments[5]._set_default_value ('::Ogre::PatchSurface::AUTO_LEVEL')

    f=main_ns.class_('MeshManager').mem_fun('createBezierPatch')
    f.arguments[6]._set_default_value ( '::Ogre::PatchSurface::AUTO_LEVEL')
    f.arguments[7]._set_default_value ( '::Ogre::PatchSurface::AUTO_LEVEL')


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


    ## need some help here as the function overloads are causing issues
    f = global_ns.class_('::Ogre::GpuProgramParameters').\
        mem_fun('setNamedConstant', arg_types=['::Ogre::String const &','::Ogre::Real'] )
    f.arguments[1].name="real"
    f = global_ns.class_('::Ogre::GpuProgramParameters').\
        mem_fun('setNamedConstant', arg_types=['::Ogre::String const &','int'] )
    f.arguments[1].name="int"

    # need to stop the automatic char to array conversion for this function
    try:
        global_ns.member_function ('::Ogre::NedAllocPolicy::allocateBytes').documentation = "This is to stop automatic conversion"
    except:
        pass
    try:
        global_ns.member_function ('::Ogre::NedPoolingPolicy::allocateBytes').documentation = "This is to stop automatic conversion"
    except:
        pass
    # have to hand wrap a set of functions that are virtual and return a pointer to a static string
    funcNames = ['getType', 'getTypeName']
    for funcName in funcNames:
        for f in main_ns.member_functions(funcName):
            if f.virtuality == declarations.VIRTUALITY_TYPES.PURE_VIRTUAL:
                if not f.parent.name.startswith('placeholder'):
                    main_ns.class_(f.parent.name).member_function(funcName).exclude()
                    values={'class_name':f.parent.name, 'function_name':f.name }
                    code=hand_made_wrappers.WRAPPER_WRAPPER_GetTypeFix % values
                    main_ns.class_(f.parent.name).add_wrapper_code( code )
                    code=hand_made_wrappers.WRAPPER_REGISTRATION_GetTypeFix % values
                    main_ns.class_(f.parent.name).add_registration_code( code )
                    print 'Adjusted pure virtual function returning a static pointer via hand wrapper %s:%s' % ( f.parent.name, f.name)

    if environment.ogre.version.startswith("1.7"):
        # Lots of new wrappers to handle all with uglky aliases that cause issues so rather than many hand entries
        # in python_ogre_aliases I'm trying to be a little smart
        pat = re.compile ("::Ogre::(\w+?)<.*?Ogre::(.+?)[<*,]")
        dups = {}

        if 0:
            c = global_ns.class_('::Ogre::vector<Ogre::RenderWindowDescription, Ogre::STLAllocator<Ogre::RenderWindowDescription, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >')
            print "FFFFF", c
            print c.decl_string
            for cc in c.constructors():
                if not cc.is_copy_constructor:
                    print "OLD:", cc.parent.cache.full_name
                    cc.parent.cache.full_name = cc.parent.cache.full_name.replace('(Ogre::MemoryCategory)0','Ogre::MEMCATEGORY_GENERAL')
                    print "NEW:", cc.parent.cache.full_name
                    print cc
                    print cc.decl_string
            print c.decl_string


           
        for c in global_ns.member_functions():
            #
            # This code is redundent if a patch is applied to OGRE
            #
            for f in c.arguments:
                if "<MEMCATEGORY_GENERAL>" in f.type.decl_string:
                    print "MEMFIX:",f.type
                    try:
                        f.type.declaration.name = f.type.declaration.name.replace("<MEMCATEGORY_GENERAL>","<Ogre::MEMCATEGORY_GENERAL>")
                    except AttributeError:
                         try:
                             f.type.base.declaration.name = f.type.base.declaration.name.replace("<MEMCATEGORY_GENERAL>","<Ogre::MEMCATEGORY_GENERAL>")
                         except AttributeError:
                             f.type.base.base.declaration.name = f.type.base.base.declaration.name.replace("<MEMCATEGORY_GENERAL>","<Ogre::MEMCATEGORY_GENERAL>")
                    except:
                         print "FAILED!!!", f, f.type
                         print type(f), type(f.type)
                if f.default_value and "<MEMCATEGORY_GENERAL>" in f.default_value:
                    f.default_value = f.default_value.replace("<MEMCATEGORY_GENERAL>", "<Ogre::MEMCATEGORY_GENERAL>")
                            # print "PROBLEM", f, type(f.type), dir(f.type)
#        sys.exit()          
        for c in global_ns.classes():
            if "_" in c.alias: # only care about classes we are include with ugly alaises # not c.ignore and 
                # print "1a:", c, c.alias
                m = pat.match(c.decl_string)
                if m:
                    # print "is M"
                    if not "STLAllocator" in m.group(1):
                        const = ""
                        if "_Const_iterator" in c.decl_string:
                            const = "Const"
                        newalias = "".join ( m.groups()) + const
                        newalias = newalias.replace("::", "") ## sometimes need to remove namespace ::'s
                        newalias = newalias.replace(" ", "_") ## and can't have spaces in name  
                        newalias = newalias.replace("&)", "") ## one issue where the regex didn't work  
                         
                        if not dups.has_key(newalias):
                            dups[newalias]=0
                        else:
                            dups[newalias]=dups[newalias]+1
                            newalias = newalias + "_" +str(dups[newalias])
                        print "Set Alias:", newalias, "  ", c.decl_string
                        c.alias = newalias
                        c.wrapper_alias = 'wrapper_' + newalias
                    else:
                        ## any that show up here need a manual entry in the python_ogre_aliases.h file
                        print "NOT Changed:",c.decl_string
                        print m.groups()
                else:
                    ## any that show up here need a manual entry in the python_ogre_aliases.h file
                    print "NO MATCH:", c.decl_string

        # need to fixup return values that show up as invalid typdefs in generated code
        #
        # again this has been fixed with a patch to the C++ code
        #
        # for f in global_ns.member_functions():
            # if f.return_type:
                # if "<MEMCATEGORY_GENERAL>" in f.return_type.decl_string:
                    # print "MEMFIX2:",f.return_type
                    # if type (f.return_type) == declarations.cpptypes.declarated_t:
                        # print "TWEAKED:", f, f.return_type
                        # f.return_type.declaration.name = f.return_type.declaration.name.replace("<MEMCATEGORY_GENERAL>","<Ogre::MEMCATEGORY_GENERAL>")
                    # elif type (f.return_type) == declarations.cpptypes.pointer_t:
                        # f.return_type.base.declaration.name = f.return_type.base.declaration.name.replace("<MEMCATEGORY_GENERAL>","<Ogre::MEMCATEGORY_GENERAL>")
                        # print "CHANGED:", f.return_type.base.declaration.name
                    # else:
                        # print "*** ISSUE:", f, type(f.return_type)
                        # # sys.exit()
                    
                    # # pdb.set_trace()

##
# fix up any ugly name alias
##
def ManualAlias ( mb ):

    AliasFixList = [
    ["::Ogre::SceneManager::estimateWorldGeometry",
    ["::Ogre::DataStreamPtr &", "::Ogre::String const &"],
    ft.modify_type('typename',declarations.remove_reference),
    "estimateWorldGeometry"],
    ]

    if 0:
        for fix in AliasFixList:
            c=mb.member_function(fix[0] ,arg_types=fix[1])
            c.add_transformation( fix[2], alias=fix[3])
        mb.member_function('::Ogre::Math::Abs', arg_types['::Ogre::Real']).alias="AbsReal"

    #c=mb.namespace('std').class_('std::set<Ogre::Texture*,std::less<Ogre::Texture*>,Ogre::STLAllocator<Ogre::Texture*,Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL> > >')
    #c.alias="stdSetOgreTexture"

    count =0
#    for c in mb.decls():
#        #if not c.ignore:#  and not c.alias:
#        if c.decl_string.startswith('::std::set<Ogre::Texture'):
#            c.alias = 'stdSetOgreTexture'

    #sys.exit()
############################################################
##
##  And things that need to have their argument and call values fixed.
##  ie functions that pass pointers in the argument list and of course we need
##  to read the updated values - so instead we pass them back
##  as new values in a tuple (ETC ETC)
##
############################################################

def ManualTransformations ( mb ):
    ns = mb.global_ns.namespace ( MAIN_NAMESPACE )

    def create_output( size ):
        return [ ft.output( i ) for i in range( size ) ]

    # this is now handled automatically in the common utilities...

# # #     rt_cls = ns.class_('RenderTarget')
# # #     x=rt_cls.mem_fun('getMetrics')
# # #     x.add_transformation( *create_output(3) )
# # #     x.documentation = docit ( "","no arguments", "tuple containing width, height, colourDepth")
# # #
# # #     x=rt_cls.mem_fun( 'getStatistics', arg_types=['float &']*4 )
# # #     x.add_transformation( ft.output(0),ft.output(1),ft.output(2),ft.output(3), alias="getStatisticsList" )
# # #     x.documentation = docit ("", "no arguments", "tuple - lastFPS, avgFPS, bestFPS, worstFPS")
# # #
# # #     # This doesn't work at the moment as Py++ ignores it ??
# # # #
#     x = ns.mem_fun('::Ogre::RenderQueueListener::renderQueueEnded')
#     x.add_transformation(ft.inout('repeatThisInvocation'))
#     x.documentation = docit ("","queueGroupId, invocation", "tuple - repeatThisInvocation")
#
#     x = ns.mem_fun('::Ogre::RenderQueueListener::renderQueueStarted')
#     x.add_transformation(ft.inout('skipThisInvocation'))
#     x.documentation = docit ("","queueGroupId, invocation", "tuple - skipThisInvocation")
#
#     x = ns.mem_fun('::Ogre::ResourceGroupManager::fireScriptStarted')
#     x.add_transformation(ft.inout('skipScript'))
#     x.documentation = docit ("","scriptName, skipScript", "tuple - skipThisInvocation")
#
# # #
# # #     x=ns.mem_fun('::Ogre::RenderWindow::getMetrics')
# # #     x.add_transformation( *create_output(5) )
# # #     x.documentation = docit ("","no arguments", "tuple - width, height, colourDepth, left, top")
# # #
# # #     x=ns.mem_fun('::Ogre::Viewport::getActualDimensions')
# # #     x.add_transformation( *create_output(4) )
# # #     x.documentation = docit ("","no arguments", "tuple - left, top, width, height")
# # #
# # #     x=ns.mem_fun('::Ogre::BillboardSet::getParametricOffsets')
# # #     x.add_transformation( *create_output(4) )
# # #     x.documentation = docit ("","no arguments", "tuple - left, right, top, bottom")
# # #
# # #     x=ns.mem_fun('::Ogre::Compiler2Pass::isFloatValue')
# # #     x.add_transformation( *create_output(2) )
# # #     x.documentation = docit ("","no arguments", "tuple - Return Value(True/False), fvalue, charsize")
# # #
# # #     x=ns.mem_fun('::Ogre::UTFString::_utf16_to_utf32')
# # #     x.add_transformation( ft.output('out_uc') )
# # #     x.documentation = docit ("","uint16", "tuple - size_t, out character")
# # #
# # #     x=ns.mem_fun('::Ogre::UTFString::_utf8_to_utf32')
# # #     x.add_transformation( ft.output('out_uc') )
# # #     x.documentation = docit ("","char", "tuple - size_t, out character")
# # #
# # #     x=ns.mem_fun('::Ogre::Frustum::calcProjectionParameters')
# # #     x.add_transformation( *create_output(4) )
# # #     x.documentation = docit ("","no arguments", "tuple - left, right, bottom, top")
# # #
# # #     x=ns.mem_fun('::Ogre::StaticGeometry::getRegionIndexes')
# # #     x.add_transformation( ft.output('x'), ft.output('y'), ft.output('z') )
# # #     x.documentation = docit ("","Vector", "tuple - x,y,z")
# # #
# # #     x=ns.mem_fun('::Ogre::InstancedGeometry::getBatchInstanceIndexes')
# # #     x.add_transformation( ft.output('x'), ft.output('y'), ft.output('z') )
# # #     x.documentation = docit ("","Vector", "tuple - x,y,z")
# # #
#     x=ns.mem_fun('::Ogre::CompositorChain::RQListener::renderQueueStarted')
#     x.add_transformation(ft.inout("skipThisQueue"))
#     x.documentation = docit ("", "id, invocation", "skipThisQueue" )
#
#     x=ns.mem_fun('::Ogre::CompositorChain::RQListener::renderQueueEnded')
#     x.add_transformation(ft.inout("repeatThisQueue"))
#     x.documentation = docit ("", "id, invocation", "repeatThisQueue" )
# # #
# # #     x=ns.mem_fun('::Ogre::PanelOverlayElement::getUV')
# # #     x.add_transformation(ft.output('u1'), ft.output('v1'), ft.output('u2'), ft.output('v2') )
# # #     x.documentation = docit ("", "no arguments", "tuple - u1, v1, u2, v2" )
# # #
# # #     x=ns.mem_fun('::Ogre::ExternalTextureSource::getTextureTecPassStateLevel')
# # #     x.add_transformation( *create_output(3) )
# # #     x.documentation = docit ("", "no arguments", "tuple - TechniqueLevel, PassLevel,StateLevel")
# # #
# # #     x=ns.mem_fun('::Ogre::Mesh::suggestTangentVectorBuildParams' )
# # #     x.add_transformation(ft.output('outSourceCoordSet'), ft.output('outIndex') )
# # #     x.documentation = docit ("", "targetSemantic","outSourceCoordSet, outIndex" )
# # #
# # #     x=ns.mem_fun('::Ogre::PixelUtil::getBitDepths')
# # #     x.add_transformation(ft.output_static_array('rgba',4) )
# # #     x.documentation = docit ("", "format", "rgba" )
# # #
# # #     x=ns.mem_fun('::Ogre::PixelUtil::getBitMasks')
# # #     x.add_transformation(ft.output_static_array('rgba',4) )
# # #     x.documentation = docit ("", "format", "rgba" )
# # #
# # #





# # #     ## these need updates to Py++ to handle pointers
#     x = ns.mem_fun('::Ogre::PixelUtil::packColour', arg_types=['Ogre::uint8','Ogre::uint8','Ogre::uint8','Ogre::uint8',None,None])
#     x.add_transformation(ft.modify_type(5,_ReturnUnsignedInt ), alias= "packColourUint" )
#     x.documentation = docit ("", "r,g,b,a,Pixelformat, src", "" )
#
#     x = ns.mem_fun('::Ogre::PixelUtil::packColour', arg_types=['float','float','float','float',None,None])
#     x.add_transformation(ft.modify_type(5,_ReturnUnsignedInt ), alias= "packColourFloat" )
#     x.documentation = docit ("", "r,g,b,a,Pixelformat, src", "" )
#
#     x = ns.mem_fun('::Ogre::PixelUtil::unpackColour', arg_types=['float *','float *','float *','float *',None,None])
#     x.add_transformation(ft.output('r'), ft.output('g'), ft.output('b'), ft.output('a'), alias="unpackColourFloat )
#     x.documentation = docit ("", "Pixelformat, src", "r,g,b,a" )
#
#     x = ns.mem_fun('::Ogre::PixelUtil::unpackColour', arg_types=['float *','float *','float *','float *',None,None])
#     x.add_transformation(ft.output('r'), ft.output('g'), ft.output('b'), ft.output('a'), alias="unpackColourFloat )
#     x.documentation = docit ("", "Pixelformat, src", "r,g,b,a" )

# # #
# # # # #     x = ns.mem_fun('::Ogre::Frustum::projectSphere')
# # # # #     x.add_transformation(ft.output('left'), ft.output('top'), ft.output('right'), ft.output('bottom') )
# # # # #     x.documentation = docit ("", "Sphere", "result, left, top, right, bottom" )
# # # # #
# # # # #     x = ns.mem_fun('::Ogre::Camera::projectSphere')
# # # # #     x.add_transformation(ft.output('left'), ft.output('top'), ft.output('right'), ft.output('bottom') )
# # # # #     x.documentation = docit ("", "Sphere", "result, left, top, right, bottom" )
# # #
# # #     # these are * * 's so need more work
# # # #     x = ns.mem_fun('::Ogre::AnimationTrack::getKeyFramesAtTime' )
# # # #     x.add_transformation(ft.output('keyFrame1'), ft.output('keyFrame2') )
# # #
# # # #     x = ns.mem_fun('::Ogre::Mesh::prepareMatricesForVertexBlend' )
# # # #     x.add_transformation(ft.output('blendMatrices') )
# # # #
# # # #     x = ns.mem_fun('::Ogre::Mesh::softwareVertexBlend' )
# # # #     x.add_transformation(ft.output('blendMatrices') )
# # # #
# # # #     x = ns.mem_fun('::Ogre::OptimisedUtil::softwareVertexSkinning' )
# # # #     x.add_transformation(ft.output('blendMatrices') )
# # #
# # # #     x = ns.mem_fun('::Ogre::NumericSolver::solveNxNLinearSysDestr')
# # # #     x.add_transformation(ft.output('coeff') )
# # #
# # # #     x = ns.mem_fun('::Ogre::SkeletonInstance::_getAnimationImpl')
# # # #     x.add_transformation(ft.output('linker') )
# # # #     x = ns.mem_fun('::Ogre::SkeletonInstance::getAnimation', arg_types=[None,None])
# # # #     x.add_transformation(ft.output('linker') )
# # # # #     x = ns.mem_fun('::Ogre::Skeleton::_getAnimationImpl')
# # # # #     x.add_transformation(ft.output('linker') )
# # # #     x = ns.mem_fun('::Ogre::Skeleton::getAnimation', arg_types=[None,None])
# # # #     x.add_transformation(ft.output('linker') )
# # #
# # # # # #     x = ns.mem_fun('::Ogre::RenderQueue::RenderableListener::renderableQueued')
# # # # # #     x.add_transformation(ft.output('ppTech') )
# # # # # #     x.documentation = docit ("UNTESTED", "rend, groupID, priority", "ppTech" )
# # #
# # #     ##
# # #     ## now we handle some specials..
# # #     ##

# # #
# # #     pixelBox_size = """
# # # namespace{
# # # struct PixelBoxSize{
# # #     bp::ssize_t operator()( boost::python::tuple args ) const{
# # #         boost::python::object self = args[0];
# # #         Ogre::PixelBox& pb = boost::python::extract<Ogre::PixelBox&>( self );
# # #         return pb.getSize();
# # #     }
# # # };
# # # }
# # # """
# # # #     c = ns.class_( 'PixelBox' )
# # # #     c.add_declaration_code( pixelBox_size )
# # # #     for f in c.mem_funs( 'getData' ):
# # # #         f.call_policies = call_policies.return_range( f, 'ImageSize' )
# # # #         f.documentation = "Python-Ogre Modified Return Range \\n"
# # # #         if f.has_const:
# # # #             f.alias = 'getReadOnlyData'


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

# # #     gpu_pp_int_size = """
# # # namespace{
# # # struct GpuProgramParametersGetIntPointerSize{
# # #     bp::ssize_t operator()( boost::python::tuple args ) const{
# # #         boost::python::object self = args[0];
# # #         Ogre::GpuProgramParameters& gpupp = boost::python::extract<Ogre::GpuProgramParameters&>( self );
# # #         boost::python::object pos_obj = args[1];
# # #         bp::ssize_t offset = boost::python::extract<bp::ssize_t>( pos_obj );
# # #         return gpupp.intBufferSize - offset;
# # #         }
# # # };
# # # struct GpuProgramParametersGetFloatPointerSize{
# # #     bp::ssize_t operator()( boost::python::tuple args ) const{
# # #         boost::python::object self = args[0];
# # #         Ogre::GpuProgramParameters& gpupp = boost::python::extract<Ogre::GpuProgramParameters&>( self );
# # #         boost::python::object pos_obj = args[1];
# # #         bp::ssize_t offset = boost::python::extract<bp::ssize_t>( pos_obj );
# # #         return gpupp.floatBufferSize - offset;
# # #         }
# # # };
# # # }
# # # """
# # #
# # #
# # #     GpuProgramParameters = ns.class_( 'GpuProgramParameters' )
# # #     GpuProgramParameters.add_declaration_code( gpu_pp_int_size )
# # #     f = GpuProgramParameters.mem_fun( 'getIntPointer' )
# # #     f.call_policies = call_policies.return_range( f, 'GpuProgramParametersGetIntPointerSize' )
# # #     f.documentation = "Python-Ogre Modified Return Range \\n"
# # #     f = GpuProgramParameters.mem_fun( 'getFloatPointer' )
# # #     f.call_policies = call_policies.return_range( f, 'GpuProgramParametersGetFloatPointerSize' )
# # #     f.documentation = "Python-Ogre Modified Return Range \\n"

###############################################################################
##
##  Now for the AUTOMATIC stuff that should just work with all versions of Ogre
##
###############################################################################


def AutoFixes ( mb, MAIN_NAMESPACE ):
    """ now we fix a range of things automatically - typically by going through
    the entire name space trying to guess stuff and fix it:)
    """
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns


    # Allow conversion between Vectors/Colourvalue etc and Python lists
    Add_Auto_Conversions( mb )

    # now we fix up the smart pointers ...
    Set_Smart_Pointers ( main_ns )

    ### This is now handled by Auto_Funtional_Transformation...

# # #     # Functions that have void pointers in their argument list need to change to unsigned int's
# # #     pointee_types=['unsigned int','int', 'float', '::Ogre::Real', '::Ogre::uchar', '::Ogre::uint8', '::Ogre::uint16'
# # #              'unsigned char', 'char', 'bool']
# # #     ignore_names=['Matrices', 'Vertices', 'ExceptionFactory', 'UTFString' ]
# # #     common_utils.Fix_Void_Ptr_Args  ( main_ns, pointee_types, ignore_names )



    # and change functions that return a variety of pointers to instead return unsigned int's
    pointee_types=['unsigned int','int', 'float', '::Ogre::Real', '::Ogre::uchar', '::Ogre::uint8', 'unsigned char', 'char']
    ignore_names=[]## 'ptr', 'useCountPointer']  # these are function names we know it's cool to exclude
    common_utils.Fix_Pointer_Returns ( main_ns, pointee_types, ignore_names )

    ## By Default we turn off all implicit conversion (search for "allow_implicit_conversion" further down)
    ## however there are certain functions that need to have it enabled and hence we add them here...

    ImplicitClasses=['Radian','Degree', 'TimeIndex', 'LiSPSMShadowCameraSetup' ]
    common_utils.Fix_Implicit_Conversions ( main_ns, ImplicitClasses )

    # variables that are readonly and mutable need to be changed from 'vars' to properties so there
    # is a copy made of the C++ variable before passing into Python
    ToFixClasses=['Matrix3', 'Matrix4','Vector3','Vector2', 'ColourValue', 'Quaternion']
    knownNonMutable=['unsigned int','int', 'float','::Ogre::Real', '::Ogre::uchar',
                      '::Ogre::uint8', 'unsigned char', 'char']
    common_utils.Fix_ReadOnly_Vars ( mb, ToFixClasses, knownNonMutable )


#     ## note change to clear prefix_output as this will force all transforms to be inout (and not 'output') to ensure the arguments are matched
#     ## problem with overload virtual fuctions from parent class such as getMetrics in RenderWindow and RenderTarget
#     common_utils.Auto_Functional_Transformation ( main_ns, special_vars=['::Ogre::Real &','::Ogre::ushort &','size_t &']  )

    # arguments passed as refs but not const are not liked by boost
    Fix_Ref_Not_Const ( main_ns )

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
      mb.global_ns.member_function('::Ogre::WindowEventUtilities::_CarbonWindowHandler').exclude()
    elif sys.platform.startswith ('linux'):
        pass
    mb.global_ns.class_('vector<int, std::allocator<int> >').alias='VectorInt'
    try:
        mb.global_ns.class_('vector<std::pair<unsigned int, unsigned int>, std::allocator<std::pair<unsigned int, unsigned int> > >').alias='VectorUnsignedUnsigned'
    except:
        pass
    try: #new in 1.7 on Linux
        mb.global_ns.class_('::Ogre::Profiler').exclude()

        mb.global_ns.class_('::Ogre::map<std::string, std::_List_iterator<Ogre::Profiler::ProfileHistory>, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, std::_List_iterator<Ogre::Profiler::ProfileHistory> >, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >').exclude()
    except:
        pass
    #as reported by mike with linux:bp::arg("flags")=(std::_Ios_Fmtflags)0
    mb.namespace( MAIN_NAMESPACE ).class_('StringConverter').member_functions('toString').exclude()

    ## grab the operator== and operator!= and exclude them
    ## NOTE: Defination for these are "extern bool..." so I wonder if we should exclude any "extern" operators
    for o in mb.namespace( MAIN_NAMESPACE ).free_operators(arg_types=['::Ogre::ShadowTextureConfig const &',
            '::Ogre::ShadowTextureConfig const &'], allow_empty=True):
        o.exclude()

    ## And even though we have excluded the operators we also need to exclude the equality
    ## otherwise it causes undefined symbols __ZN4OgreeqERKNS_19ShadowTextureConfigES2_
    ## -- change file is --_ShadowTextureConfig__value_traits.pypp.hpp
    c = mb.namespace( MAIN_NAMESPACE ).class_( 'ShadowTextureConfig' )
    c.equality_comparable = False

    ## handle a problem hashmap
    mb.member_function('::Ogre::Mesh::getSubMeshNameMap').exclude()
    stdex_ns = mb.global_ns.namespace("__gnu_cxx")
    stdex_ns.class_('hash_map<std::string, unsigned short, __gnu_cxx::hash<std::string>, std::equal_to<std::string>, std::allocator<unsigned short> >').exclude
    for f in mb.member_functions():
        if f.name == 'getChildIterator':
            print "LINUX SPECIAL:", f
            #f.exclude()
    #mb.member_functions('::Ogre::Node::getChildIterator').exclude()

    ## handle the hashmaps -- TODO FIX under LINUX ???
    #stdex_ns = mb.global_ns.namespace("stdext")
    #for cls in stdex_ns.classes():
    #    if cls.name.startswith ("hash"):
    #        cls.include()

def Fix_NT ( mb ):
    """ fixup for NT systems
    """
    try:
        # change in py++ so this doesn't get found?
        mb.global_ns.class_( 'vector<Ogre::Vector4, std::allocator<Ogre::Vector4> >' ).exclude( )
    except:
        pass
    Skeleton = mb.namespace( MAIN_NAMESPACE ).class_( 'Skeleton' ).constructors().exclude()

    ## handle the hashmaps -- TODO FIX under LINUX ???
    stdex_ns = mb.global_ns.namespace("stdext")
    for cls in stdex_ns.classes():
        if cls.name.startswith ("hash"):
            cls.include()
# #             cls.exclude() ### AJM TOFIX hash_maps are missing at the moment?

    # need to force these
    hwnd = mb.global_ns.class_("HWND__")
    hwnd.opaque = True
    _iobuf = mb.global_ns.class_("_iobuf")# need the file handle in Ogre::FileHandleDataStream::FileHandleDataStream
    _iobuf.opaque = True
    ## handle a problem hashmap
    mb.member_function('::Ogre::Mesh::getSubMeshNameMap').exclude()


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
                    print "Fixing Const:", arg.type.base, "\n",fun, "\n",fun.parent.name+"::"+fun.name+"::"+arg.name+" ("+arg.type.decl_string+")"
                    if len (fun.transformations) == 0:
                        try:
                            fun.add_transformation( ft.modify_type(arg_position,declarations.remove_reference ), alias=fun.name )
                            fun.documentation = docit ("Fixed Constant Var " + arg.name + " (pos:"+str(arg_position) +")",
                                                            "...", "...")
                        except:
                            print "FAILED!!!"
                    else:
                        print "*** Problem adding transform as will cause duplicates", fun
#                 else:
#                     print "arg OK ", fun, arg.name, arg.type.decl_string
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

def Set_Smart_Pointers( mb ):
    """ we need to identify 'smart pointers' which are any of the SharedPtr classes
    """
    knownSmartClasses= ['MaterialPtr',
        'CompositorPtr',
        'FontPtr',
        'GpuProgramPtr',
        'HardwareIndexBufferSharedPtr',
        'HardwarePixelBufferSharedPtr',
        'HardwareVertexBufferSharedPtr',
        'HighLevelGpuProgramPtr',
        'MeshPtr',
        'PatchMeshPtr',
        'SkeletonPtr',
        'TexturePtr',
        ]
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
        elif cls.name in knownSmartClasses:
           v.apply_smart_ptr_wa = True
           print "Applying Smart Pointer: ",  v.name, " of class: ",  cls.name
    # now for some specials by variable name..
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
    #~ Exception = mb.namespace( MAIN_NAMESPACE ).class_( 'Exception' )
    #~ Exception.include()
    #~ Exception.mem_fun('what').exclude() # declared with empty throw
    #~ Exception.mem_fun('getNumber').exclude() # declared with empty throw
    #~ Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getFullDescription().c_str()' )

#~ # #     ## there are two identical constructors so we need to remove one of them
#~ # #     for c in Exception.constructors(arg_types=[None]):
#~ # #         c.exclude() ## exclude the first constructor..
#~ # #         break

def autoCasting ( main_ns, ignores = ['ParamCommand','MovableObjectFactory']  ):
    """ looks for classes that have parents (bases) and there are overlapping/hidden functions
    When we find one insert a asPARENT helper function just in case it might be needed

    Only real world case I know of is casting a Bone to a Node so you can create a new node.

    """

    CastReg=\
    """
    def( "%(functionName)s", &::%(className)s_%(functionName)s,\
        "Python-Ogre Hand Wrapped to cast to a parent(base) type\\n\
        In this case from a %(className)s to a %(castName)s",\
        bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
        """
    CastDec=\
    """
    %(returnType)s *
    %(className)s_%(functionName)s ( %(classDecl)s * me ) {
    return ( (%(returnType)s * ) me );
    }
    """
    masterlist = {}
    for c in main_ns.classes():
        if len(c.bases) > 0:
            for b in c.bases:
                r = b.related_class
                if not '<' in r.decl_string : ##and c.name != 'BillboardSet':  # don't worry about templates or factories..
                    if not r.name in ignores: # there are some bases that we don't care about overlaps on
                        for f in c.member_functions(allow_empty=True):
                            if r.member_functions(f.name, allow_empty=True ): # look for each function in upper classes
                                key=r.name+'::'+f.name
                                if masterlist.has_key(key):
                                    masterlist[key]+=1
                                else:
                                    masterlist[key]=1
#                                 print "WARNING Overlaping functions:", c.name+"::"+f.name+ " in "+r.name
#                                 values = {  'returnType':r.decl_string, 'functionName': "as"+r.name,
#                                             'className':c.name, 'classDecl': c.decl_string,
#                                             'castName':r.name }
#
# #                                 regcode = CastReg % values
# #                                 deccode = CastDec % values
# #                                 c.add_declaration_code( deccode )
# #                                 c.add_registration_code( regcode )
#                                 print "Hand wrapper (as"+r.name+") created to cast from", c.name, "to", r.name, f.name ## b.access
# #                                 break
    print "WARNING: Functions that may not be accessable"
    i= masterlist.keys()
    i.sort()
    for k in i:
        print k, masterlist[k]
#     sys.exit()
def FindProtectedVars ( mb ):
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )

#     for c in main_ns.classes():
#         for v in c.variables(allow_empty=True):
#             if v.access_type == 'protected':
#                 print "Protected Variable:", v, v.why_not_exportable(), v.exportable
    v =main_ns.class_('SceneManager').variable('mSceneNodes')
    v.set_exportable(True)
    v._exportable_reason = None
#     print v,v.why_not_exportable(), v.exportable
#     v.access_type = 'public'
#     sys.exit()

#
# the 'main'function
#
def generate_code():
    messages.disable(
        messages.W1005 # using a non public variable type for argucments or returns
# # #           Warnings 1020 - 1031 are all about why Py++ generates wrapper for class X
        , messages.W1020
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
# #         , messages.W1035
# #         , messages.W1040
        , messages.W1041 # overlapping names when creating a property
# # #         , messages.W1038
        , messages.W1036 # pointer to Python immutable member
        , messages.W1033 # unnamed variables
# # #         , messages.W1018 # expose unnamed classes
# #         , messages.W1049 # returns reference to local variable
# #         , messages.W1014 # unsupported '=' operator
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
    defined_symbols = environment.defined_symbols                    
    defined_symbols.append('__GCCXML__')
    undefine_symbols=[]
    
    defined_symbols.append( 'OGRE_VERSION_' + environment.ogre.version )

    environment.ogre.include_dirs.insert ( 0,os.getcwd() )

    
    if environment.Config._SVN: # building Ogre 1.7
        defined_symbols.append ('HAVE_OGRE_BUILDSETTINGS_H') # it uses the cmake buildsettings include
    #
    # build the core Py++ system from the GCCXML created source
    #
    if not SWIG:
        mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ogre.include_dirs
                                          , define_symbols=defined_symbols
                                          , undefine_symbols=undefine_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogre.cflags
                                           )

    else:
        mb = swig_module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ogre.include_dirs
                                          , define_symbols=defined_symbols
                                          , undefine_symbols=undefine_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogre.cflags
                                           )

    # Disable all implicit conversions
    mb.constructors().allow_implicit_conversion = False

    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True



    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    #
    global_ns = mb.global_ns
    global_ns.exclude()
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    main_ns.include()

    # f=main_ns.member_function ("::Ogre::Technique::getGPUDeviceNameRuleIterator")
    # print f
    # print dir(f)
    # print f.return_type
    # print type(f.return_type)
    # print dir(f.return_type)
    # sys.exit()
#    for f in main_ns.calldefs ('getNext'):
#        print f
#    sys.exit()    
##    c=main_ns.class_('::Ogre::OverlayElementCommands::CmdHorizontalAlign')
##    print c
##    print "P:",c.parent
##    print c.parent.name, c.parent.decl_string
##    print type(c.parent)
##    print "PP:",c.parent.parent
##    print c.parent.parent.name, c.parent.parent.decl_string
##    print type (c.parent.parent)
##    
##    c=main_ns.class_('::Ogre::Node')
##    print c
##    print "P:",c.parent
##    print c.parent.name, c.parent.decl_string
##    print type(c.parent)
##    print "PP:",c.parent.parent
##    print c.parent.parent.name, c.parent.parent.decl_string
##    print type (c.parent.parent)
##    print dir (c.parent)
##    sys.exit()
    
##    for t in main_ns.typedefs():
##        print t.name, t.parent, t.decl_string
##    print type(t)
##    print dir(t) 
##    t=main_ns.typedef('::Ogre::ConfigFile::SettingsIterator')
##    print t
##    t=main_ns.typedef('::Ogre::ConfigFile::SectionIterator')
##    print t
##    
##    
##    
##       
##    sys.exit()
    
    autoCasting ( main_ns ) ##

    common_utils.AutoExclude ( mb, MAIN_NAMESPACE )
    ManualExclude ( mb )
    common_utils.AutoInclude ( mb, MAIN_NAMESPACE )
    ManualInclude ( mb )

    # here we fixup functions that expect to modifiy their 'passed' variables and are not autmatically fixed
    ManualTransformations ( mb )

    ManualAlias ( mb )
    AutoFixes ( mb, MAIN_NAMESPACE )
    ManualFixes ( mb )
    # note change to clear prefix_output as this will force all transforms to be inout (and not 'output') to ensure the arguments are matched
    # problem with overload virtual fuctions from parent class such as getMetrics in RenderWindow and RenderTarget
    common_utils.Auto_Functional_Transformation ( main_ns, special_vars=['::Ogre::Real &','::Ogre::ushort &','size_t &']  )

    FindProtectedVars ( mb )


    for cls in main_ns.classes():
        if not cls.ignore:
            try:
                for c in cls.constructors():
                    if c.access_type != 'public':
                        print "NPC:", c.access_type, c
            except:
                print "Class without constructors", cls
    #Py++ can not expose static pointer member variables
    main_ns.vars( 'ms_Singleton' ).disable_warnings( messages.W1035 )

    # Ogre is "special" in that some classes are unnnamed and need fixing
    common_utils.fix_unnamed_classes( main_ns.classes( name='' ), 'Ogre' )

    common_utils.configure_shared_ptr(mb)

    register_exceptions.register( mb )

    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    common_utils.Set_DefaultCall_Policies ( mb.global_ns.namespace ( "std" ) )

    common_utils.Set_DefaultCall_Policies ( mb.global_ns.namespace ( MAIN_NAMESPACE ) )

    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )


    NoPropClasses = ["UTFString"]

    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            common_utils.remove_DuplicateProperties ( cls )
            ## because we want backwards pyogre compatibility lets add leading lowercase properties
            common_utils.add_LeadingLowerProperties ( cls )


    #     # indicated where underlying libraries are protected etc in the doc strings
    # THIS MUST BE AFTER Auto_Functional_Transformation
    common_utils.Auto_Document( mb, MAIN_NAMESPACE )

    ## add additional version information to the module to help identify it correctly
    common_utils.addDetailVersion ( mb, environment, environment.ogre )
    common_utils.Find_Problem_Transformations ( main_ns )

    ##
    ## Experimental !!!
    ##
    if SWIG:
        swig.main_process_swig ( main_ns,'swig.in' )
        return
        
        
    count = 0
    for v in main_ns.variables():
        if not v.ignore:
            print v
            count +=1
    print "SPECIAL -- variables:", count
    count = 0
    for v in main_ns.member_functions():
        if not v.ignore:
            print v
            count +=1
    print "SPECIAL -- member functions:", count
    count=0
    for v in global_ns.classes():
        if not v.ignore:
            print "class:",v
            count +=1
    print "SPECIAL -- Number classes:", count



    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor( "Ogre" )
    mb.build_code_creator (module_name='_ogre_' , doc_extractor= extractor )

    for inc in environment.ogre.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ogre.generated_dir )

    # 21Nov07 - Change to include our own system header to allow precompiled headers..
    if len ( customization_data.header_files( environment.ogre.version ) ) == 1:
        # there is only a single include so lets insert it at the top of the list...
        lastc = mb.code_creator.creators[ mb.code_creator.last_include_index() ]
        mb.code_creator.remove_creator( lastc )  ## we need to remove the previous one

        # and now add our precompiled one..
        mb.code_creator.adopt_creator ( include.include_t ( header=customization_data.header_files( environment.ogre.version )[0] ), 0)
    else:
        mb.code_creator.replace_included_headers( customization_data.header_files( environment.ogre.version ) ) ##, leave_system_headers=False )


# #     special = main_ns.class_('RenderQueueListener')
# #     print "\n\n\nWRAPPER CODE"
# #     print special
# #     print dir(special)
# #     print special.wrapper_code
# #     print dir( special.wrapper_code )
# #     for l in special.wrapper_code:
# #         print "code:", l
# #         print l.text
# #     print "\n\n\n"

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.ogre.version ) )

    mb.split_module(environment.ogre.generated_dir, huge_classes, use_files_sum_repository=False )

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generaated directory..
    additional_files=[
            os.path.join( environment.shared_ptr_dir, 'py_shared_ptr.h'),
            os.path.join( os.path.abspath(os.path.dirname(__file__) ), 'python_ogre_precompiled.h' ),
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

    if environment.ogre.version.startswith("1.7"):
        ## have a code generation issue that needs resolving...
        filesToFix=['LightList.pypp.cpp', 'stdVectorRenderWindowDescription.pypp.cpp','InstancedGeometry.pypp.cpp']
        for filename in filesToFix:
            fname = os.path.join( environment.ogre.generated_dir, filename)
            try:
                f = open(fname, 'r')
                buf = f.read()
                f.close()
                if (" MEMCATEGORY_GENERAL" in buf) or ("<MEMCATEGORY_GENERAL" in buf):
                    buf = buf.replace ( " MEMCATEGORY_GENERAL", " Ogre::MEMCATEGORY_GENERAL")
                    buf = buf.replace ( "<MEMCATEGORY_GENERAL", "<Ogre::MEMCATEGORY_GENERAL")
                    f = open ( fname, 'w+')
                    f.write ( buf )
                    f.close()
                    print "UGLY FIX OK:", fname
            except:
                print "ERROR: Unable to fix:", fname
        
    count=0
    for v in global_ns.classes():
        if not v.ignore:
            print "class:",v
            count +=1
    print "SPECIAL -- Number classes:", count            
    
if __name__ == '__main__':

    start_time = time.clock()
#     import logging
#     from pygccxml import utils
#     logger = utils.loggers.cxx_parser
# #     logger.setLevel(logging.DEBUG)
    if _DEBUG:
        pdb.run('generate_code()')
    else:
        generate_code()
    print 'Python-OGRE source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
