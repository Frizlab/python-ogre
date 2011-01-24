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
import re

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

# override standard pretty name function
from pyplusplus.decl_wrappers import algorithm
algorithm.create_valid_name = common_utils.PO_create_valid_name 


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
                    
                    
    excludes=['resize', 'expand'] # needs 'T' (default value) changed to native class
    for c in global_ns.classes():
        if 'btAlignedObjectArray' in c.name:
            for m in c.member_functions():
                if m.name in excludes:
                    m.exclude()
            
    excludes=[ ##'::btBroadphaseInterface::rayTest'
    
            '::btAxisSweep3Internal< unsigned short >::createProxy'
            ,'::btAxisSweep3Internal< unsigned int >::createProxy'
            ,'::btBvhTriangleMeshShape::setOptimizedBvh'  # has a funky default argument that could be fixed at some point TOFIX
            
            # some (all?) of these should be hand wrapped 
            # TOFIX
            ,'::btCollisionDispatcher::getNewManifold'
            ,'::btCollisionDispatcher::defaultNearCallback'
            ,'::btCollisionDispatcher::getInternalManifoldPointer'
            ,'::btCollisionDispatcher::getNearCallback'
            ,'::btCollisionDispatcher::setNearCallback'
            
            ,'::btDbvtBroadphase::createProxy'
            ,'::btSimpleBroadphase::createProxy'
            ,'::btDispatcher::getInternalManifoldPointer'
            
            ,'::btRaycastVehicle::setRaycastWheelInfo' ## only implemented in header, causes undefined symbol import 
            ,'::btGImpactCollisionAlgorithm::getAverageTreeCollisionTime'
            ,'::btGImpactCollisionAlgorithm::getAverageTriangleCollisionTime'
            ,'::btGImpactBvh::getAverageTreeCollisionTime'
            ,'::btGImpactBvh::getAverageTriangleCollisionTime'
            ,'::btGImpactQuantizedBvh::getAverageTreeCollisionTime'
            ,'::btSolve2LinearConstraint::resolveBilateralPairConstraint'
            ,'::btSolve2LinearConstraint::resolveUnilateralPairConstraint'
            ## stuff in headers but not in library
            ,'::btCollisionAlgorithm::getDispatcherId'
            ,'::btAxisSweep3Internal<unsigned int>::processAllOverlappingPairs' 
            ,'::btAxisSweep3Internal<unsigned short>::processAllOverlappingPairs'
            ,'::btCollisionAlgorithmConstructionInfo::getDispatcherId'
            ,'::btCompoundShape::createAabbTreeFromChildren'
            ,'::btMultiSapBroadphase::quicksort'
            ,'::btQuantizedBvh::walkRecursiveQuantizedTreeAgainstQuantizedTree'
            ,'::btSequentialImpulseConstraintSolver::resolveSplitPenetrationImpulseCacheFriendly'       
            
            ,'::btCollisionShape::getName'
            ,'::btCollisionObject::getUserPointer'
            ,'::btCollisionObject::setUserPointer'
            
            # new in .74
            ,'::btSliderConstraint::testLinLimits2'
            ,'::btDynamicsWorld::setInternalTickCallback' # linux
            
#             # new as jan 2010
#             ,'::btAlignedObjectArray< float >::at'
#             ,'::btAlignedObjectArray< unsigned short >::at'
#             ,'::btAlignedObjectArray< unsigned int >::at'
#             ,'::btAlignedObjectArray< int >::at'
#             ,'::btAlignedObjectArray< short >::at'
#             ,'::btAlignedObjectArray< btHashInt >::at'
#
#             ,'::btAlignedObjectArray< float >::expandNonInitializing'
#             ,'::btAlignedObjectArray< short >::expandNonInitializing'
#             ,'::btAlignedObjectArray< unsigned short >::expandNonInitializing'
#             ,'::btAlignedObjectArray< unsigned int >::expandNonInitializing'
            ,'::btHashMap< btHashPtr, btOptimizedBvh* >::find'
            ,'::btHashMap< btHashPtr, btOptimizedBvh* >::getAtIndex'
            ,'::btHashMap< btHashPtr, bParse::bChunkInd >::find'
            ,'::btHashMap< btHashPtr, bParse::bChunkInd >::getAtIndex'
            ,'::btHashMap< btHashPtr, btTriangleInfoMap* >::find'
            ,'::btHashMap< btHashPtr, btTriangleInfoMap* >::getAtIndex'
            
              ]
    for e in excludes:
        try:
            global_ns.member_functions(e).exclude()
            print "excluding function", e
        except:
            print "Unable to exclude function:", e    
        
    excludes = []
    for e in excludes:
        print "Excluding:", e
        global_ns.free_functions(e).exclude()
    
    excludes = [  
            'btAlignedAllocator<btDbvtNode const*, 16u>'
            ,'::btAlignedObjectArray< btDbvt::sStkNP >'
            ,'::btAlignedAllocator< btBroadphaseInterface*, 16u >'
            ,'::btAlignedObjectArray< btWheelInfo >'   # TOFIX bullet issue with wheels ???
            ,'::btAlignedAllocator< btCollisionObject*, 16u >'
            ,'::btAlignedAllocator<btPersistentManifold*, 16u >'
            ,'::btAlignedAllocator< btTypedConstraint*, 16u >'
            ,'::btAlignedAllocator< btCharacterControllerInterface*, 16u >'
            ,'::btAlignedAllocator< btMultiSapBroadphase::btBridgeProxy*, 16u >'
            ,'::btAlignedAllocator< btMultiSapBroadphase::btMultiSapProxy*, 16u >'
            ,'::btAlignedAllocator< unsigned short, 16u >'
            ,'::btAlignedAllocator< unsigned int, 16u >'
            ,'::btAlignedAllocator< void*, 16u >'
            ,'::btAlignedAllocator< btActionInterface*, 16u >'
            ,'::btAlignedAllocator< btVector3DoubleData*, 16u >'
            ,'::btAlignedAllocator< btVector3FloatData*, 16u >'
            ,'::btAlignedObjectArray< void* >'
            ,'::btAlignedObjectArray< btMultiSapBroadphase::btBridgeProxy* >'
            ,'::btAlignedObjectArray< btMultiSapBroadphase::btMultiSapProxy* >'
            ,'::btAlignedAllocator< btRaycastVehicle*, 16u >'
            ,'::btThreadSupportInterface'
            ,'::btAlignedAllocator< btRigidBody*, 16u >'
#            ,'::btAlignedAllocator< PosixThreadSupport::btSpuStatus, 16u >' # linux 
#            ,'::btAlignedObjectArray< PosixThreadSupport::btSpuStatus >'
             ,'::btAlignedAllocator<btCollisionAlgorithm*,16u>'
             ,'::btAlignedAllocator< btCollisionShape*, 16u >'
             ,'::btAlignedAllocator< btGImpactMeshShapePart*, 16u >'
             ,'::btAlignedAllocator< btHullTriangle*, 16u >'
             ,'::btAlignedAllocator< btMaterial*, 16u >'
             ,'::btAlignedAllocator< btPlane*, 16u >'
             ,'::btGeometryUtil'
             ,'::btAlignedAllocator< btChunk*, 16u >'
             ,'::btAlignedAllocator< short, 16u >'
             ,'::btAlignedAllocator< short*, 16u >'
             ,'::btAlignedAllocator< btTriangleIndexVertexArray*, 16u >' #2.76
             ,'::btAlignedAllocator< btTriangleInfoMap*, 16u >' #2.76
             # btHashString::btHashString doesn't have a default constructor
             # looks like 'fundemental' types are not working'
             ,'::btAlignedObjectArray< btHashInt >'
             ,'::btAlignedObjectArray< btHashPtr >'
             ,'::btAlignedObjectArray< btTransform >'
             ,'::btAlignedObjectArray< btTriangle >'
             ,'::btAlignedObjectArray< btTypedConstraint::btConstraintInfo1 >'
             ,'::btAlignedObjectArray< char* >'
             ,'::btAlignedObjectArray< char const* >'
             ,'::btAlignedObjectArray< btHashString >'
             ,'::btAlignedObjectArray< float >'
             ,'::btAlignedObjectArray< short >'
             ,'::btAlignedObjectArray< int >'
             ,'::btAlignedObjectArray< unsigned short >'
             ,'::btAlignedObjectArray< unsigned int >'
             ,'::btAlignedObjectArray< short* >'
             ,'::btAlignedObjectArray< int* >'
             ,'::btHashMap< btHashPtr, void* >'
             ,'::btHashMap< btHashPtr, char const* >'
             ,'::btHashMap< btHashInt, int >'
             ,'::btHashMap< btHashString, int >'
             ,'::btHashMap< btHashInt, btTriangleInfo >'
             ,'::btTriangleInfoMap'
#             ,'::btHashMap< btHashPtr, btPointerUid > >' # fails on Linux, maybe coudl be non-copyable

             ,'::btDefaultSerializer'  # btHashInt::btHashInt no default const
             ,'::btAlignedAllocator< bParse::bStructHandle*, 16u >'
             ,'::btHashMap< btHashPtr, bParse::bStructHandle* >'
             ,'::btAlignedAllocator< btOptimizedBvh*, 16u >'
             
             ,'::btHashMap< btHashPtr, btCollisionObject* >'
             ,'::btHashMap< btHashPtr, btCollisionShape* >'
             ,'::btHashMap< btHashString, btCollisionShape* >'
             ,'::btHashMap< btHashString, btRigidBody* >'
             ,'::btHashMap< btHashString, btTypedConstraint* >'
             
            ]
            
    for e in excludes:
      try:   
         global_ns.class_(e).exclude()
         print "Excluded class", e
      except:
         print "WARNING: Unable to exclude", e    
              
#     for c in global_ns.classes():
#       if c.name.startswith ('btAligned'):
#          c.exclude()
             
    excludes = ['::btPolyhedralConvexShape::m_optionalHull'  ## needs Hull from Extras
                #'::btRaycastVehicle::m_wheelInfo' ## TOFIX -- might be a bullet issue with wheels
                ,'::btCompoundCollisionAlgorithm::m_childCollisionAlgorithms'
            ]
    for e in excludes:
      try:
        global_ns.variable(e).exclude()
      except:
         print "WARNING: Unable to exclude variable ", e        
    
        
    excludes = [ '::btAlignedObjectArray< btCharacterControllerInterface* >::operator[]'
            ,'::btHashMap< btHashPtr, btTriangleInfoMap* >::operator[]'
            ,'::btHashMap< btHashPtr, btOptimizedBvh* >::operator[]'
            ,'::btHashMap< btHashPtr, bParse::bChunkInd >::operator[]'
            ]
    for e in excludes:
      try:
        global_ns.operators(e).exclude()
        print "Excluded operator",e
      except:
         print "WARNING: Unable to exclude operator", e         
        
    # an Operator that won't compile
    for c in global_ns.classes():
        for o in c.operators(allow_empty=True):
            if type(o) == type (decl_wrappers.calldef_wrapper.casting_operator_t()):
                print "EXCLUDING Casting Operator:", o
                o.exclude()
    global_ns.class_('btQuaternion').operators("operator-", arg_types=[]).exclude()
    
    noncopy = ['btDbvtBroadphase','btRaycastVehicle','btAlignedObjectArray< btHashInt >',
                'btBulletWorldImporter','btHashMap< btHashPtr, btOptimizedBvh* >', 
                'btHashMap< btHashPtr, bParse::bChunkInd >',
                'btHashMap< btHashPtr, btTriangleInfoMap* >',
                'btHashMap< btHashPtr, btPointerUid > >'
                ]
    for c in noncopy:
        main_ns.class_(c).noncopyable = True
    
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
    #auto include doesn't check for classes bt32***
    main_ns.class_('bt32BitAxisSweep3').include()

    namesp = ['::','btTransform']
    operators = ['::btVector3','::btQuaternion','::btScalar','::btMatrix3x3','::btTransform','bool']
    badOp=['::btQuaternion (*)( ::btQuaternion const & )']
    ## to check on btAlignedAllocator equality operator

    for o in global_ns.class_("::btQuaternion").operators():
        print o, o.ignore, o.name, o.decl_string
    for op in global_ns.operators():
        if op.parent._name in namesp:
            if op.return_type.decl_string in operators and op.decl_string not in badOp:
                print "Including Main NameSpace Operators Name %s Return Type %s" %( op._name, op.return_type.decl_string)
                op.include()
  
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

    # another relate class with incorrect default argument namespace
    f=main_ns.class_('btDiscreteDynamicsWorld').mem_fun('addCollisionObject')
    f.arguments[1].default_value = 'btBroadphaseProxy::StaticFilter'  
    
    f = ['::btBroadphaseInterface::createProxy',
        '::btMultiSapBroadphase::createProxy',
        '::btDispatcher::getNewManifold'
        ]
    for fun in f:
        print "setting call policy", fun
        main_ns.member_function(fun).call_policies = call_policies.default_call_policies()
    
    for fun in main_ns.member_functions(name='rayTest'):
        for a in fun.arguments:
            if a.default_value and "&0.0" in a.default_value:
                a.default_value = "::btVector3( (0.0), (0.0), (0.0) )"

    f=main_ns.class_('btConvexPointCloudShape').mem_fun('setPoints')
    for a in f.arguments:
        if a._name == 'localScaling':
            a.default_value= "::btVector3( (1.0), (1.0), (1.0) )"

    main_ns.class_( '::btHashMap< btHashPtr, bParse::bStructHandle* >' ).constructors().exclude()

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
                           
    print dir ( main_ns )
    for var in main_ns.enums ():
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

    defined_symbols = [ 'BULLET_EXPORTS', '__GCCXML__', '_MSC_VER',
                        '__MINGW32__'   # needed to turn off allocator allignment which boost can't do..
                        
                        ]
    defined_symbols.append( 'VERSION_' + environment.bullet.version )  
    if sys.platform.startswith ( 'linux' ):
        defined_symbols.append('USE_PTHREADS')
    
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
    
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    # Do this earlier than normal as I need to override the default in ManualFixes
    common_utils.Set_DefaultCall_Policies ( main_ns )
    
    ManualFixes ( mb )
    ManualExclude ( mb )
    common_utils.Auto_Functional_Transformation ( main_ns ,special_vars=['btScalar *'] )
    
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    NoPropClasses = [""]
    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            rec = ogre_properties.ogre_property_recognizer_t()
            rec.addSetterType ( 'btScalar' ) # this type is a 'float/double' however we need to tell py++ such so it creates setters
            cls.add_properties( recognizer=rec )
            
    common_utils.Auto_Document( mb, MAIN_NAMESPACE )
            
    ## add additional version information to the module to help identify it correctly 
    common_utils.addDetailVersion ( mb, environment, environment.bullet )

    mem_fun = main_ns.member_function('::btVector3::setX')
    ##print "setter:", property_recognizer_i  (mem_fun)
    if len( mem_fun.arguments ) != 1:
        print 'False1'
    if not declarations.is_void( mem_fun.return_type ):
        print 'False2'
    if mem_fun.has_const:
        print 'False3'
    if mem_fun.overloads:
        print 'False4' 
    print "OK"
            
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
# #     extractor = exdoc.doc_extractor( "Ogre" ) 
# #     mb.build_code_creator (module_name='_ogre_' , doc_extractor= extractor )

    extractor = exdoc.doc_extractor() # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_bullet_' , doc_extractor= extractor )
    
    for inc in environment.bullet.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.bullet.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.bullet.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.bullet.version ) )

    mb.split_module(environment.bullet.generated_dir, huge_classes, use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generaated directory..
    additional_files=[
            os.path.join( os.path.abspath(os.path.dirname(__file__) ), 'python_bullet_masterlist.h' )	    
            ]
    if environment.isLinux(): #sBulletDNAlen is defined in the cpp file not the header!! 
       additional_files.append ( os.path.join( environment.Config.PATH_Bullet, 'src', 'LinearMath','btSerializer.cpp' ) )
    for sourcefile in additional_files:
        p,filename = os.path.split(sourcefile)
        destfile = os.path.join(environment.bullet.generated_dir, filename )

        if not common_utils.samefile( sourcefile ,destfile ):
            shutil.copy( sourcefile, environment.bullet.generated_dir )
            print "Updated ", filename, "as it was missing or out of date"


        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
