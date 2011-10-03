#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-MAIN_NAMESPACE and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------

## STARTER TEMPLATE..
## replace plsm2 with lowercase project name
## set MAIN_NAMESPACE
## rename and configure .h files


import os, sys, time, shutil
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
try:
  from pyplusplus.creators_factory import sort_algorithms
except ImportError, e:
  from pyplusplus.module_creator import sort_algorithms

import common_utils.extract_documentation as exdoc
import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties
from common_utils import docit

MAIN_NAMESPACE = 'Ogre'

############################################################
##
##  Here is where we manually exclude stuff
##
############################################################

def NamespaceInclude ( main_ns, exposeAll=True, exposed = False ):
    if not exposeAll:
        return
    if exposed:
        main_ns.include( already_exposed = True )
        return
    main_ns.include()


def ManualExclude ( mb ):
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns    
    excludes=[ ]
    for e in excludes:
        main_ns.class_(e).exclude()
        print "Excluded Class:", e
                
    excludes=[]
    for e in excludes:
        main_ns.member_functions(e).exclude()
        print "Excluded Member Function(s):", e
        
    excludes=[]
    for e in excludes:
        main_ns.member_variables(e).exclude()
        print "Excluded Variable:", e

    #------------------------------------------------------------------------------------------
    # PagingLandScapeQueue has two find_nearest member functions, one takes a Vector3 argument 
    # the other two unsigned ints. The second calls q->getCoordinates which is only applies
    # to PagingLandScapePage. PagingLandScapeTile has no member function and getCoordinates
    # causes compile error, so exclude.
    #------------------------------------------------------------------------------------------
    for fun in main_ns.class_('PagingLandScapeQueue<Ogre::PagingLandScapeTile>').member_functions():
        #print "OCTREEXXXXXXXXXXXXXX", fun.name
        if fun.name.startswith('find_nearest') or fun.name.startswith('find_farest'):
            if fun.arguments[0].type.decl_string == 'unsigned int const' and fun.arguments[1].type.decl_string == 'unsigned int const':
                fun.exclude()
                print "Excluding Class %s Function %s" %('PagingLandScapeQueueTile', fun.name)

    #------------------------------------------------------------------------------------------
    # returns [2][2][2] array handwrapped    
    #------------------------------------------------------------------------------------------
    main_ns.class_('PagingLandScapeOctree').variables('mChildren').exclude()    


    #------------------------------------------------------------------------------------------
    # Undefined Symbols on link
    #------------------------------------------------------------------------------------------
    main_ns.class_('PagingLandScapeTileManager').member_functions('reset').exclude()  
    main_ns.class_('PagingLandScapeTexture').member_functions('setNeedUpdate').exclude()  
    main_ns.class_('PagingLandScapeRenderableManager').member_functions('InitTextureCoordinatesBuffers').exclude()  
    main_ns.class_('PagingLandScapeRenderableManager').member_functions('freeTextureCoordinatesBuffers').exclude()  
    main_ns.class_('PagingLandScapeRenderableManager').member_functions('getTextureCoordinatesBuffers').exclude()  
    main_ns.class_('PagingLandScapeRenderableManager').member_functions('setTextureCoordinatesBuffers').exclude()  
    main_ns.class_('PagingLandScapeListenerManager').member_functions('addListener').exclude()  
    main_ns.class_('PagingLandScapeListenerManager').member_functions('removeListener').exclude()  
    main_ns.class_('PagingLandScapeOctreeSceneManager').member_functions('shutdown').exclude()  
    main_ns.class_('PagingLandScapeOctreeSceneManager').member_functions('walkPagingLandScapeOctree').exclude()  
    main_ns.class_('PagingLandScapeMeshDecal').member_functions('SetSize').exclude()  
    main_ns.class_('PagingLandScapeIndexBufferManager').member_functions('getIndex').exclude()  
    main_ns.class_('PagingLandScapeRenderable').member_functions('_vertex').exclude()  
    main_ns.class_('PagingLandScapePageManager').member_functions('find_nearest').exclude()
    main_ns.class_('PagingLandScapePage').member_functions('reload').exclude()
    
    
#     std_ns = global_ns.namespace('std')
#     for c in std_ns.classes():
#         if 'map' in c.decl_string or 'Ogre' in c.decl_string:
#             print c.name, c.decl_string
#     std_ns.class_('::std::vector<Ogre::Plane, std::allocator<Ogre::Plane> >')    
# #     std_ns.class_('::std::map<std::string, std::allocator<std::string> >')


    # hand wrapped
    #main_ns.class_('::Ogre::PagingLandScapeSceneManager').member_functions('setOption').exclude()
    #main_ns.class_('::Ogre::PagingLandScapeSceneManager').member_functions('getOption').exclude()
    main_ns.class_('::Ogre::PagingLandScapeOptions').member_functions('setOption').exclude()
    main_ns.class_('::Ogre::PagingLandScapeOptions').member_functions('getOption').exclude()
    main_ns.class_('::Ogre::PagingLandScapeSceneManager').member_functions('setOption').exclude()
    main_ns.class_('::Ogre::PagingLandScapeSceneManager').member_functions('getOption').exclude()
        
############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################


                    
def ManualInclude ( mb ):
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns  
    
    includes = [    ]                 
    for i in includes:
        main_ns.enum(i).include()  
    for c in main_ns.classes():
        if "Singleton" in c.decl_string:
            print c, c.name, c.decl_string                  
    includes = []  
    for i in includes:
        try:
            main_ns.class_(i).include()
            print "Forced Include:", i
        except:
            pass

    #for c in main_ns.classes():
    #    print c.name
    # classes already exposed, to expand..            
    includes = ['RaySceneQuery'
                ,'DefaultRaySceneQuery'
                ,'SceneNode'
                ,'SceneQuery'
                ,'RaySceneQueryListener'
                ,'ColourValue'
                ,'WireBoundingBox'
                ,'Math']
    for i in includes:
        main_ns.class_(i).include(already_exposed=True)            

    
    
############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixes ( mb ):    
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns
        
    includes_already = ['::std::vector<Ogre::PlaneBoundedVolume, std::allocator<Ogre::PlaneBoundedVolume> >',
                '::std::vector<Ogre::Plane, std::allocator<Ogre::Plane> >',
                '::std::map<std:string'                
                ]
    includes = []             
    for c in global_ns.classes():
        for i in includes_already:
            if c.decl_string.startswith(i):
                print "Including Special(already Exposed):", c
                c.include(already_exposed=True)
        for i in includes:
            if c.decl_string.startswith(i):
                print "Including Special:", c
                c.include()                
                
    # base class camera private copy ctor
    noncopy=['PagingLandScapeOctreeCamera','PagingLandScapeCamera']
    for n in noncopy:
        cls = global_ns.class_(n)
        print "Setting Class %s non copyable" % ( n )
        cls.noncopyable = True
        # ensure derived classes are also non copyable
        #for d in cls._derived:
        #    print "Setting Class %s non copyable as base class %s non copyable" % ( d.related_class.name, n )
        #    d.noncopyable = True        
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
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns
                
    def create_output( size ):
        return [ ft.output( i ) for i in range( size ) ]
        
# #     # this is from the Ogre generate wrapper and is needed to remove vertexData from the camera        
# #     known = ['indexBuffer', 'vertexData', 'indexData']
# #     for c in mb.classes():
# #         for v in c.variables(allow_empty = True ):
# #             if v.name in known:
# #                v.apply_smart_ptr_wa = True    
# #                print "Applying Smart Pointer (know): ",  v.name, " of class: ",  c.name
    
    
###############################################################################
##
##  Now for the AUTOMATIC stuff that should just work
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
        
#     # Functions that have void pointers in their argument list need to change to unsigned int's  
#     pointee_types=[]
#     ignore_names=[]
#     common_utils.Fix_Void_Ptr_Args  ( main_ns ) # , pointee_types, ignore_names )

    # and change functions that return a variety of pointers to instead return unsigned int's
    pointee_types=['unsigned int','int', 'float', '::Ogre::Real', '::Ogre::uchar', '::Ogre::uint8', 'unsigned char', 'char']
    ignore_names=[]## 'ptr', 'useCountPointer']  # these are function names we know it's cool to exclude
    common_utils.Fix_Pointer_Returns ( main_ns, pointee_types, ignore_names )   

    
    # and change functions that return a variety of pointers to instead return unsigned int's
    pointee_types=[]
    ignore_names=[]  # these are function names we know it's cool to exclude
    common_utils.Fix_Pointer_Returns ( main_ns ) # , pointee_types, ignore_names )   

    # functions that need to have implicit conversions turned off
    ImplicitClasses=[] 
    common_utils.Fix_Implicit_Conversions ( main_ns, ImplicitClasses )
    
    if os.name =='nt':
        Fix_NT( mb )
    elif os.name =='posix':
        Fix_Posix( mb )
        
    common_utils.Auto_Document( mb, MAIN_NAMESPACE )
        
 
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
        
def fixUnionStructs(mb):
    # there appears to be a problem with structs with no name. Py++ flattens out
    # the structures. Exclude and hand wrap. TO DO check easier way to do this.    
    global_ns = mb.global_ns
    excludes = [ 'included_'
                 ,'x_'
                 ,'y_'
                 ,'z_'
                 ,'height_'
                 ,'width_'
                 ,'renderLevel_'
                 ,'subX_'
                 ,'subZ_' ]
    for v in global_ns.variables():
        if v.name in excludes and v.parent.name=='':
            v.exclude()


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
#         , messages.W1035
#         , messages.W1040 
#         , messages.W1038        
#         , messages.W1041
#         , messages.W1036 # pointer to Python immutable member
#         , messages.W1033 # unnamed variables
#         , messages.W1018 # expose unnamed classes
#         , messages.W1049 # returns reference to local variable
#         , messages.W1014 # unsupported '=' operator
#          )
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.plsm2.root_dir, "python_plsm2.h" )
                        , environment.plsm2.cache_file )


    defined_symbols = [ '_MSC_VER'
                        ,'_PLSM_OCTREE'
                        ,'OGRE_NONCLIENT_BUILD'
                        , 'OGRE_GCC_VISIBILITY'
                        ,'__PYTHONOGRE_BUILD_CODE' ]
    defined_symbols.append( 'VERSION_' + environment.plsm2.version )

    # threads not tested    
    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        defined_symbols.append('BOOST_HAS_WINTHREADS')

    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.plsm2.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.plsm2.cflags
                                           )
                                           
    # if this module depends on another set it here                                           
    mb.register_module_dependency ( environment.ogre.generated_dir )
    
    # normally implicit conversions work OK, however they can cause strange things to happen so safer to leave off
    mb.constructors().allow_implicit_conversion = False                                           
    
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
            
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()
    
#     #
#     # I'm assuming that any 'std' classes etc we need will have already been exposed by the Ogre library
#     #
    # some of the std classes we need haven't been exposed i.e ColourValue 
    #std_ns = global_ns.namespace('std')
    #std_ns.include(already_exposed=True) 
    
    # We don't include all of MAIN_NAMESPACE otherwise we get the same full wrapper
    # so instead we include classes with names that start with PCZ
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    main_ns.include( already_exposed = True )  ## force the ogre class to be exposed as well...


            
    # We don't include all of MAIN_NAMESPACE otherwise we get the same full wrapper
    # so instead we include classes as follows:
    includes = [  'PagingLandScape'
                 ,'PagingLandscape'
                 ,'MsgQueType'
                 ,'distanceToBoxSort'
                 ,'FrontToBackNodeSorterOperator'
                 ,'LodTracker'
                 ,'Occlusion'
                 ,'OcclusionBoundingBox'
                 ,'OcclusionElement'
                 ,'OctreeSet'
                 ,'PoolSet'
                 ,'QuerySet'
                 ,'RegisterCameraTraversal'
                 ,'RenderLevel'
                 ,'Traversal'
                 ,'CHCTraversal'
                 ,'SWTraversal'
                 ,'TraversalConst'
                 ,'UnregisterCameraTraversal'
                 ,'ConstTraversalConst'
                 ,'ViewFrustumCullingTraversal'
                 ,'ViewFrustumCullingTraversalDirect'
                 ,'VisibilityData'
                 ,'CDRGNURBSSurface'
                 ,'Point4D'
                 ,'splinePoint'
                   ]
 
 
    # temp hack to overcome problem with gccxml fast delegates taken from plsm samples
    includes.append('CustomPagingLandScapeListener')
    
    for c in main_ns.classes():
        for i in includes:
            if c.name.startswith(i):
                print "Including Class: ", c
                c.include()
    
    # same for member functions            
    includes = ['vectorToBoxDistance']
    for f in main_ns.member_functions():
        for i in includes:
            if f.name.startswith(i):
                print "Including Function: ", f
                f.include()
                
    # same for std namespace            
    #includes = [  'std::list< Ogre::SceneNode* >'
    #             ,'std::vector< Ogre::ColourValue >'
    #             ,'std::vector <Ogre::Real>'       ]

    std_ns = global_ns.namespace('std')
    #for c in std_ns.classes():
    #    print "STDXX ", c.name
    
    
    #for c in std_ns.classes():
    #    for i in includes:
    #        if c.name.startswith(i):
    #            print "Including Std: ", c
    #            c.include()
                
    # problem with colorvalue again                
    #std_ns.class_('vector<Ogre::ColourValue, std::allocator<Ogre::ColourValue> >').exclude()
    #std_ns.class_('list<Ogre::PagingLandScapeData2D*, std::allocator<Ogre::PagingLandScapeData2D*> >').include()
    #std_ns.class_('list<Ogre::PagingLandScapeTile*, std::allocator<Ogre::PagingLandScapeTile*> >').include()
    #std_ns.class_('list<Ogre::PagingLandScapePage*, std::allocator<Ogre::PagingLandScapePage*> >').include()
    #std_ns.class_('list<Ogre::PagingLandScapeOctreeNode*, std::allocator<Ogre::PagingLandScapeOctreeNode*> >').include()

    stdex_ns = mb.global_ns.namespace("stdext")
    for cls in stdex_ns.classes():
        print "STDEXT ", cls.name
    #cc=jj


    # same for enums            
    includes = [  'culling_modes'
                 ,'Intersection'
                 ,'Neighbor'
                 ,'PageQueuingState'
                 ,'PageState' ]
                 
    for c in main_ns.enums():
        for i in includes:
            if c.name.startswith(i):
                print "Including Enum: ", c
                c.include()
                
    # exclude nameless structures
    fixUnionStructs(mb)
    
    common_utils.AutoExclude ( mb, MAIN_NAMESPACE )
    ManualExclude ( mb )
    common_utils.AutoInclude ( mb, MAIN_NAMESPACE )
    ManualInclude ( mb )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    AutoFixes ( mb, MAIN_NAMESPACE )
    ManualFixes ( mb )
    common_utils.Auto_Functional_Transformation ( main_ns )
    

    
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    common_utils.Set_DefaultCall_Policies ( mb.global_ns.namespace ( MAIN_NAMESPACE ) )
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    NoPropClasses = [""]
    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            
    ## add additional version information to the module to help identify it correctly 
    common_utils.addDetailVersion ( mb, environment, environment.plsm2 )

    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor() # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_plsm2_' , doc_extractor= extractor )
    
    for inc in environment.plsm2.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.plsm2.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.plsm2.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.plsm2.version ) )

    mb.split_module(environment.plsm2.generated_dir, huge_classes, use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generated directory..
    
    common_utils.copyTree ( sourcePath = environment.Config.PATH_INCLUDE_plsm2, 
                            destPath = environment.plsm2.generated_dir, 
                            recursive=False )
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
