#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------

## STARTER TEMPLATE..
## replace bullet with lowercase project name
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
from pyplusplus.module_creator import sort_algorithms

import common_utils.extract_documentation as exdoc
import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties
from common_utils import docit

MAIN_NAMESPACE = ''

############################################################
##
##  Here is where we manually exclude stuff
##
############################################################

def ManualExclude ( mb ):
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns    
    # remove functions that fail during compile
    excludes=['remove', 'findBinarySearch','findLinearSearch']
    for c in global_ns.classes():
        if 'Array' in c.name:
            for m in c.member_functions():
                if m.name in excludes:
                    m.exclude()
    excludes=['resize', 'expand'] #needs 'T' changed to native class
    for c in global_ns.classes():
        if 'btAlignedObjectArray' in c.name:
            for m in c.member_functions():
                if m.name in excludes:
                    m.exclude()
    excludes=[ '::btCollisionDispatcher::getNewManifold'
            ,'::btCollisionDispatcher::defaultNearCallback'
            ,'::btCollisionDispatcher::getInternalManifoldPointer'
            ,'::btCollisionDispatcher::getNearCallback'
            ,'::btCollisionDispatcher::setNearCallback'
            ,'::btSimpleBroadphase::createProxy'
            # now for functions defined in headers but not implemented
            ,'::btRaycastVehicle::setRaycastWheelInfo'
            ,'::btCompoundShape::createAabbTreeFromChildren'
            ,'::btCollisionAlgorithm::getDispatcherId'
            ,'::btCollisionAlgorithmConstructionInfo::getDispatcherId'
            
            ,'::btCapsuleShape::calculateLocalInertia'
            ,'::btCapsuleShape::localGetSupportingVertexWithoutMargin'
            ,'::btCapsuleShape::batchedUnitVectorGetSupportingVertexWithoutMargin'
            
            ,'::btAxisSweep3Internal<unsigned short>::createProxy'
            ,'::btAxisSweep3Internal<unsigned int>::createProxy'
            ,'::btBU_Simplex1to4::getName'
            ,'::btBoxShape::getName'
            ,'::btBvhTriangleMeshShape::getName'
            ,'::btCollisionShape::getName'
            ,'::btDispatcher::getInternalManifoldPointer'
            ,'::btAxisSweep3Internal<unsigned int>::processAllOverlappingPairs'
            ,'::btAxisSweep3Internal<unsigned short>::processAllOverlappingPairs'
            ,'::btMultiSapBroadphase::createProxy'
            ,'::btMultiSapBroadphase::quicksort'
            ,'::btBvhTriangleMeshShape::setOptimizedBvh' # new in 2.70
            ,'::btQuantizedBvh::walkRecursiveQuantizedTreeAgainstQuantizedTree' # 2.72
            ]
            
    for e in excludes:
        print "excluding function", e
        global_ns.member_functions(e).exclude()
    excludes = []
    for e in excludes:
        print "Excluding:", e
        global_ns.free_functions(e).exclude()
    
    excludes = ['btAlignedAllocator<btCollisionObject*, 16u>'
#             ,'btAlignedAllocator<btCollisionShape*, 16>'
            ,'btAlignedAllocator<int, 16u>'
            ,'btAlignedAllocator<float, 16u>'
            ,'btAlignedAllocator<btPersistentManifold*, 16u>'
            ,'btAlignedAllocator<btTypedConstraint*, 16u>'
            ,'btAlignedAllocator<btRaycastVehicle*, 16u>'
            ,'btAlignedAllocator<btBroadphaseInterface*, 16u>'
            
            ,'btAlignedAllocator<btDbvtNode const*, 16u>'
#             ,'btAlignedObjectArray<short>'
#             ,'btAlignedAllocator<btDbvt::sStkNN, 16u>'
#             ,'btAlignedAllocator<btDbvt::sStkNP, 16u>'
#             ,'btAlignedAllocator<btDbvt::sStkNPS, 16u>'
            ,'btAlignedAllocator<btMultiSapBroadphase::btBridgeProxy*, 16u>'
            ,'btAlignedAllocator<btMultiSapBroadphase::btMultiSapProxy*, 16u>'
            ,'btAlignedAllocator<unsigned int, 16u>'
            ,'btAlignedAllocator<unsigned short, 16u>'
#             ,'btAlignedObjectArray<btDbvt::Node const*>'
#             ,'btAlignedObjectArray<btDbvt::sStkNN>'
#             ,'btAlignedObjectArray<btDbvt::sStkNP>'
#             ,'btAlignedObjectArray<btDbvt::sStkNPS>'
            ,'btAlignedObjectArray<btMultiSapBroadphase::btBridgeProxy*>'
            ,'btAlignedObjectArray<btMultiSapBroadphase::btMultiSapProxy*>'
#             ,'btAlignedObjectArray<btOdeContactJoint>'
#             ,'btAlignedObjectArray<btOdeJoint*>'
#             ,'btAlignedObjectArray<btOdeSolverBody>'
#             ,'btAlignedObjectArray<btOdeSolverBody*>'
#             ,'btAlignedObjectArray<btOdeTypedJoint>'

#             ,'btAlignedObjectArray<btIndexedMesh>'
#             ,'btAlignedObjectArray<int>'
#             ,'btAlignedObjectArray<btQuantizedBvhNode>'
#             ,'btAlignedObjectArray<btOptimizedBvhNode>'
#             ,'btAlignedObjectArray<btCollisionObject*>'
            ,'btDbvtBroadphase'
            ,'btContinuousDynamicsWorld'
#            ,'btSorLcpSolver'
            ]
# #     if environment.isWindows():
# #         excludes.extend(['btAlignedAllocator<btOdeContactJoint, 16u>'
# #            ,'btAlignedAllocator<btOdeJoint*, 16u>'
# #            ,'btAlignedAllocator<btOdeSolverBody, 16u>'
# #            ,'btAlignedAllocator<btOdeSolverBody*, 16u>'
# #            ,'btAlignedAllocator<btOdeTypedJoint, 16u>'
# #            ] )

    for e in excludes:
        print "excluding class", e
        global_ns.class_(e).exclude()
        
    excludes = ['::btPolyhedralConvexShape::m_optionalHull'  ## needs Hull from Extras
            ]
    for e in excludes:
        global_ns.variable(e).exclude()
    
    # an Operator that won't compile
    for o in global_ns.class_('btQuadWord').operators():
        if "const" in o.decl_string:
            o.exclude()
    global_ns.class_('btQuaternion').operators("operator-", arg_types=[]).exclude()
    
    
    
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
    global_ns.enumerations('PHY_ScalarType').include()
        
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
        
    ## issue where the defaults are &0.0 instead of 0.0
    fixes=['btTransform', 'btRigidBody']
    for e in fixes:
        for c in global_ns.class_(e).constructors():
            for a in c.arguments:
                if a.default_value and "&0.0" in a.default_value:
                    a.default_value = "::btVector3( (0.0), (0.0), (0.0) )"
                    
    ## Bug ingccxml where it doesn't get the namespace for default values
    ## btCollisionWorld::addCollisionObject
    f=main_ns.class_('btCollisionWorld').mem_fun('addCollisionObject')
    f.arguments[1].default_value = 'btBroadphaseProxy::DefaultFilter'  # (short int)(btBroadphaseProxy::DefaultFilter)
    f.arguments[2].default_value = 'btBroadphaseProxy::AllFilter'  # (short int)(btBroadphaseProxy::AllFilter)
      
    f = ['::btBroadphaseInterface::createProxy',
        '::btDispatcher::getNewManifold'
        ]
    for fun in f:
        print "setting call policy", fun
        main_ns.member_function(fun).call_policies = call_policies.default_call_policies()
            
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
        
        
def AutoInclude( mb, dumy ):
    global_ns = mb.global_ns
    global_ns.exclude()
    main_ns = global_ns  ##  Bullet doesn't have it's own namespace..
    for cls in main_ns.classes():
        try:
            if  cls.decl_string[2:4]=='bt' and cls.decl_string[4].isupper():
                cls.include()
        except:
            pass
     ## and we'll need the free functions as well
    for funcs in main_ns.free_functions ():
        if funcs.name[0:2]=='bt' and funcs.name[2].isupper():
            funcs.include()
            
    for var in main_ns.variables ():
        if len(var.name) > 2:
            if var.name[0:2]=='bt' and var.name[2].isupper():
                var.include()
    for var in main_ns.typedefs ():
        if len(var.name) > 2:
            if var.name[0:2]=='bt' and var.name[2].isupper():
                var.include()                


        
            
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
#         , messages.W1035
#         , messages.W1040 
#         , messages.W1038        
#         , messages.W1041
        , messages.W1036 # pointer to Python immutable member
        , messages.W1033 # unnamed variables
        , messages.W1018 # expose unnamed classes
        , messages.W1049 # returns reference to local variable
        , messages.W1014 # unsupported '=' operator
         )
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.bullet.root_dir, "python_bullet.h" )
                        , environment.bullet.cache_file )

    defined_symbols = [ 'BULLET_EXPORTS', '__GCCXML__' ]
    defined_symbols.append( 'VERSION_' + environment.bullet.version )  
    
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.bullet.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.bullet.cflags
                                           )
                                           
    # if this module depends on another set it here                                           
    ## mb.register_module_dependency ( environment.ogre.generated_dir )
    
    # normally implicit conversions work OK, however they can cause strange things to happen so safer to leave off
    mb.constructors().allow_implicit_conversion = False                                           
    
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
            
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()
    
    if MAIN_NAMESPACE == "" :
        main_ns = global_ns
        main_ns.include()
    else:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
        main_ns.exclude ()
    
    AutoInclude ( mb, MAIN_NAMESPACE ) ## note we use our own version, not common_utils
    common_utils.AutoExclude ( mb, MAIN_NAMESPACE )
    ManualInclude ( mb )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    AutoFixes ( mb, MAIN_NAMESPACE )
    ManualFixes ( mb )
    ManualExclude ( mb )
    common_utils.Auto_Functional_Transformation ( main_ns  )
    
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    common_utils.Set_DefaultCall_Policies ( main_ns )
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    NoPropClasses = [""]
    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            
    ## add additional version information to the module to help identify it correctly 
    common_utils.addDetailVersion ( mb, environment, environment.bullet )
                                      
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor() # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_bullet_' , doc_extractor= extractor )
    
    for inc in environment.bullet.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.bullet.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.bullet.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.bullet.version ) )

    mb.split_module(environment.bullet.generated_dir, huge_classes, use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generated directory..
    additional_dirs=[]
    for d in additional_dirs:
        for f in os.listdir(d):
            if f.endswith('cpp') or f.endswith('.h'):
                sourcefile = os.path.join(d, f)
                destfile = os.path.join(environment.bullet.generated_dir, f ) 
                if not common_utils.samefile( sourcefile ,destfile ):
                    shutil.copy( sourcefile, environment.bullet.generated_dir )
                    print "Updated ", f, "as it was missing or out of date"
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
