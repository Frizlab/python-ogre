#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------


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
try:
  from pyplusplus.creators_factory import sort_algorithms
except ImportError, e:
  from pyplusplus.module_creator import sort_algorithms

import common_utils.extract_documentation as exdoc
import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties

MAIN_NAMESPACE = 'NxOgre'

## small helper function
def docit ( general, i, o ): 
    docs = "Python-Ogre Modified Function Call\\n" + general +"\\n"
    docs = docs + "Input: " + i + "\\n"
    docs = docs + "Output: " + o + "\\n\\\n"
    return docs

############################################################
##
##  Here is where we manually exclude stuff
##
############################################################

def ManualExclude ( mb ):
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    for c in main_ns.classes():
        if c.decl_string.startswith('::NxOgre::Container<') and '&' in c.decl_string: # '*' not in c.decl_string:
            print "EXCLUDING: ", c
            c.exclude()
        
        ### Now some of the functions need to be excluded.. This is too much and could be cut back
        ### ie only some of the containers lists fail at compile time
        funlist = ['get','has','next','remove']
        if c.decl_string.startswith('::NxOgre::List<'):
            for f in funlist:
                try:
                    c.member_function(f).exclude()
                    print "EXCLUDING: ", c, f
                except:
                    pass
        for v in c.variables(allow_empty=True):
            if v.access_type !=  'public' :
                v.exclude()
                print "excluded", v, v.access_type
    for t in main_ns.typedefs():
        if t.decl_string.startswith('::NxOgre::Container<') and '&' in t.decl_string:# '*' not in t.decl_string:
            t.exclude()
            print "EXCLUDING: ", t

    # things not yet implemented in the C source..
    excludes=['::NxOgre::Compartment']
    for c in main_ns.classes():
        for m in c.member_functions(allow_empty=True):
            for e in excludes:
                if e in m.decl_string:
                    m.exclude()
        
    # problem with a constructor on Cloth   
    main_ns.class_('::NxOgre::Cloth').constructor(arg_types=[None,'::NxClothDesc','::NxMeshData',None,None]).exclude()
    
    # functions specified in the headers but not implemented
    main_ns.class_('::NxOgre::Blueprints::ActorBlueprint').member_function('unserialise',arg_types=[None]).exclude()
    
    
# # #         
# # #     ### Member Functions
    excludes=[
            '::NxOgre::Container<std::string, NxOgre::FluidDrain*>::begin'
            ,'::NxOgre::Container<std::string, NxOgre::FluidDrain*>::get'
            ,'::NxOgre::Container<std::string, NxOgre::FluidDrain*>::getFirst'
            ,'::NxOgre::Container<std::string, NxOgre::FluidDrain*>::next'
            ,'::NxOgre::Container<std::string, NxOgre::FluidDrain*>::_begin'
            ,'::NxOgre::Container<std::string, NxOgre::FluidDrain*>::_next'
            ,'::NxOgre::Container<std::string, NxOgre::FluidEmitter*>::_begin'
            ,'::NxOgre::Container<std::string, NxOgre::FluidEmitter*>::_next'
            ,'::NxOgre::Container<std::string, NxOgre::FluidEmitter*>::getFirst'
            ,'::NxOgre::List<NxOgre::RemoteDebuggerConnection::Camera>::destroyAndEraseAll'
            ,'::NxOgre::List<NxOgre::RemoteDebuggerConnection::Camera>::dumpToConsole'
            ,'::NxOgre::UserAllocator::realloc'
            ,'::NxOgre::Cloth::duplicate'
            ,'::NxOgre::ClothRayCaster::getClosestCloth'
            ,'::NxOgre::Joint::getBreakableMaxForce'
            ,'::NxOgre::Joint::getBreakableMaxTorque'
            ,'::NxOgre::Joint::getGlobalAxis'
            ,'::NxOgre::Joint::setGlobalAxis'
            ,'::NxOgre::JointParams::setSpring'
            ,'::NxOgre::JointParams::setMotor'
            ,'::NxOgre::JointParams::setLimits'
            ,'::NxOgre::JointParams::setJointProjection'
            ,'::NxOgre::Scene::addMaterialPairsFromFile'
            ,'::NxOgre::Scene::addMaterialPair'
            ,'::NxOgre::Fluid::setName'
            ,'::NxOgre::Fluid::getNxScene'
            ,'::NxOgre::RayCaster::castBounds'
            ,'::NxOgre::Scene::getSimType'
            ,'::NxOgre::Scene::getGravity'
            ,'::NxOgre::Scene::createSphericalJoint'
            ,'::NxOgre::Scene::createPrismaticJoint'
            ,'::NxOgre::Scene::createFixedJoint'
            ,'::NxOgre::Scene::save'
            ,'::NxOgre::Scene::batchDestroyActors'
            ,'::NxOgre::SimpleIntersection::getResult'
            ,'::NxOgre::SoftBody::simulate'
            ,'::NxOgre::SoftBody::render'
            
# #             ,'::NxOgre::FileResourceSystem::getStream'  # takes ::NxOgre::ResourceIdentifier as argument which needs fixing
# #             ,'::NxOgre::OgreResourceSystem::getStream'
#             ,'::NxOgre::ResourceSystem::getStream'
#             ,'::NxOgre::RenderableSource::getStringType' # so we don't need a wrapper as it doesn't compile
#             ,'::NxOgre::RenderableSource::getType'
            ,'::NxOgre::UserAllocator::mallocDEBUG'
#             
# #             ,'::NxOgre::Blueprints::ActorFactory' # not implemented, only in header..
#             ,'::NxOgre::ContactStream::getShape'
#             ,'::NxOgre::ContactStream::getPatchNormal'
#             ,'::NxOgre::ContactStream::getPoint'
#             ,'::NxOgre::ContactStream::getNxActor'
#             ,'::NxOgre::ContactStream::getActor'
#             ,'::NxOgre::MemoryStream::skip'
#             ,'::NxOgre::MemoryStream::seek'
            ,'::NxOgre::WheelSet::createThreeWheelSet'
            ,'::NxOgre::WheelSet::createSixWheelSet'
            ,'::NxOgre::WheelSet::attachNewWheel'
            ,'::NxOgre::Wheel::addEntity'
            ,'::NxOgre::PhysXDriver::start'
            ,'::NxOgre::PhysXDriver::stop'
            ,'::NxOgre::PhysXDriver::hasHardware'
            ,'::NxOgre::PhysXDriver::reset'

#             ,'::NxOgre::WheelSet::setBrakeTorque'
#             ,'::NxOgre::WheelSet::turn'
#             ,'::NxOgre::Material::setDirOfAnisotropy'
#             ,'::NxOgre::OgreNodeRenderable::addSceneNode'
# #             ,'::NxOgre::ResourceStreamPtr::ResourceStreamPtr'
#             ,'::NxOgre::MaterialAlias::generateConversionList'
            
            ]
    for e in excludes:
        print "excluding ", e
        main_ns.member_functions(e).exclude()
 
    ## this is probably excessive :)
    names = ['_begin','_end', '_atEnd', '_next']
    for f in main_ns.member_functions():
        if f.name in names:
            print "excluding MF:", f.name, f 
            f.exclude() 
        elif f.name.startswith('getActors'):    # these return 'Actors' which doesn't exist
            print "excluding MF:", f.name    
            f.exclude()
                 
    ### Free Functions
    excludes = []
    for e in excludes:
        main_ns.free_functions(e).exclude()
        
    ## Classes
    excludes = [
#                'DistanceJoint'      ## constructor issue TOFIX
               'JointCallback'     ## also no suitable/defaul constructor TOFIX
               ,'Character'         ## defined in header but not very much implementation
#                ,'RenderableSource'
#                ,'ResourceManager'
#                ,'ResourceStreamPtr'
               ,'::NxOgre::Blueprints::ActorFactory'
               ,'State'
#                ,'MeshResource'
               ,'::NxOgre::Serialiser::SerialiserBase'
                ]
    for e in excludes:
        print "Excluding Class:", e
        main_ns.class_(e).exclude()
# # #     
# # #         
    ## I have a challenge that Py++ doesn't recognise these classes by full name (perhaps because they are structs?)
    ## so I have to look through and match on a class by class basis
    excludeName = ['Container<NxOgre::Scene::Renderables, float>'
                ,'Container<std::string, NxOgre::Actor*>'
                ,'List<NxOgre::CharacterHitReport*>'
                ,'List<NxOgre::RemoteDebuggerConnection::Camera>'
                ,'List<NxOgre::Blueprints::ActorBlueprint*>'
                ,'Container<std::string, NxOgre::Joint*>'
                ,'Container<std::string, NxOgre::RayCastHit>'
#                 ,'::NxOgre::Container< std::basic_string< char, std::char_traits< char >, std::allocator< char > >, NxOgre::RayCastHit >'
                ]
    for c in main_ns.classes():
#         print c.decl_string   
        print ("Checking:",c.name)
        if c.name in excludeName:
            print ("SPECIAL excluding ", c.name)
            c.exclude()
        # a temporary fix for container based classes -- still an issue with them though...
        # AND this is an overkill -- not all classes need these removed...
        if c.decl_string.startswith ('::NxOgre::Container<'):
            for f in c.member_functions(allow_empty=True):
                if f.name in ['begin','get','next','destroyAndEraseAll','destroyAllOwned','CopyTo']:
                    f.exclude()    
      
    ### Variables        
    excludes = [   ## '::NxOgre::WheelSet::mEngine'   # desctuctor in WheelSet is protected so can't wrap this..
                ]
    for e in excludes:
        main_ns.variable(e).exclude()
        
    ### Typedefs    
    excludes = [ '::NxOgre::RayCastReport'
                ]
    for e in excludes:
        print "Excluding Typedef", e
        main_ns.typedefs(e).exclude()
        
    ### Operators        
    excludes=['::NxOgre::Container<std::string, NxOgre::FluidDrain*>::operator[]'
            ,'::NxOgre::Container<std::string, NxOgre::FluidEmitter*>::operator[]']
    for e in excludes:
        main_ns.operators(e).exclude()
        
    ### Constructors
    for c in main_ns.class_('::NxOgre::Pose').constructors():  ## these hide the working constructors
        for a in c.arguments:
            if 'NxVec3' in a.type.decl_string or 'NxQuat' in a.type.decl_string:
                c.exclude()
                break
# # #                 

############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################
    
def ManualInclude ( mb ):
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
#     for f in main_ns.member_functions():
#         print f
#     sys.exit()
    c=main_ns.class_('ClothVertex')
    c.include()
    for m in c.member_functions():
        m.exclude()
    c.member_function('getGlobalPosition').include()  ## this is the only function implemented
    
# #     global_ns.namespace( 'Ogre' ).class_('AxisAlignedBox').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Radian').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('SceneNode').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('IndexData').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('SceneManager').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Vector3').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Matrix4').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Degree').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Quaternion').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Node').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Serializer').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('FrameListener').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Matrix3').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Material').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Camera').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('MeshPtr').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('FrameEvent').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Root').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('Entity').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('SubMesh').include(already_exposed=True)
        
    
        
############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixes ( mb ):    

    global_ns = mb.global_ns
    
    # fix issue where the namespace isn't in the default values
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    
    for c in main_ns.constructors():
        for a in c.arguments:
            if a.default_value and a.default_value.startswith("param"):
                t1,t2=a.default_value.split('<')
                a.default_value="NxOgre::"+t1+"<NxOgre::"+t2
            elif a.default_value and a.default_value == 'IN_ORDER':
                a.default_value = 'NxOgre::NxIterator::IN_ORDER'
    for c in main_ns.member_functions():
        for a in c.arguments:
            if a.default_value and a.default_value.startswith("param"):
                t1,t2=a.default_value.split('<')
                a.default_value="NxOgre::"+t1+"<NxOgre::"+t2
                
                
    Fix_Implicit_Conversions ( main_ns )

              
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
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
        
    def create_output( size ):
        return [ ft.output( i ) for i in range( size ) ]
#     c = main_ns.mem_fun('::NxOgre::Cloth::raycast')
#     c.add_transformation(ft.inout('vertexId'))
#     c.documentation = docit('','VertexId is in/out','bool, vertexId')
    
#     for x in ns.member_functions('::NxOgre::Params::Set'):    
#         x.add_transformation(ft.inout("arg2"))
#         x.documentation = docit ("", "arg2 is an in/out","ret,arg2" )
    
###############################################################################
##
##  Now for the AUTOMATIC stuff that should just work
##
###############################################################################
    
    
def AutoExclude( mb ):
    """ Automaticaly exclude a range of things that don't convert well from C++ to Python
    """
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    
    # vars that are static consts but have their values set in the header file are bad
    Remove_Static_Consts ( main_ns )
    
    ## Exclude protected and private that are not pure virtual
    query = ~declarations.access_type_matcher_t( 'public' ) \
            & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
    non_public_non_pure_virtual = main_ns.calldefs( query )
    non_public_non_pure_virtual.exclude()
    
    #Virtual functions that return reference could not be overriden from Python
    query = declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.VIRTUAL ) \
            & declarations.custom_matcher_t( lambda decl: declarations.is_reference( decl.return_type ) )
    try:            
        main_ns.calldefs( query ).virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
        pass
               
def AutoInclude( mb ):
    main_ns = mb.global_ns  ##   doesn't have it's own namespace..
# #     for cls in main_ns.classes():
# #         try:
# #             if  cls.decl_string[2:4]=='Nx' and cls.decl_string[4].isupper():
# #                 cls.include()
# #         except:
# #             pass
# #      ## and we'll need the free functions as well
# #     for funcs in main_ns.free_functions ():
# #         if funcs.name[0:2]=='Nx' and funcs.name[2].isupper():
# #             funcs.include()
# #             
# #     for var in main_ns.variables ():
# # #         print "checking var ", var.name
# #         if len(var.name) > 2:
# #             if var.name[0:2]=='Nx' and var.name[2].isupper():
# #                 var.include()
# #     for var in main_ns.typedefs ():
# # #         print "checking typedef ", var.name
# #         if len(var.name) > 2:
# #             if var.name[0:2]=='Nx' and var.name[2].isupper():
# #                 var.include()            
# #                 

    
def AutoFixes ( mb ): 
    """ now we fix a range of things automatically - typically by going through 
    the entire name space trying to guess stuff and fix it:)
    """       
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    
    # arguments passed as refs but not const are not liked by boost
    #Fix_Ref_Not_Const ( main_ns )
    
    # Functions that have void pointers in their argument list need to change to unsigned int's  
# #     Fix_Void_Ptr_Args  ( main_ns )

    # and change functions that return a variety of pointers to instead return unsigned int's
    pointee_types=['unsigned int','int', 'float', '::Ogre::Real', '::Ogre::uchar', '::Ogre::uint8', 'unsigned char', 'char']
    ignore_names=['ptr', 'useCountPointer']  # these are function names we know it's cool to exclude
    common_utils.Fix_Pointer_Returns ( main_ns, pointee_types, ignore_names )   
    
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
        
        
def Fix_Implicit_Conversions ( mb ):
    """By default we disable explicit conversion, however sometimes it makes sense
    
    """
    
    for c in mb.classes():
        if c.name.endswith ('Params'):
            print "Implicit Conversion:", c
            c.allow_implicit_conversion = True
            for con in c.constructors(allow_empty=True):
                print con.decl_string
                con.allow_implicit_conversion = True
    return
            
    ImplicitClasses=['::NxOgre::Pose'] 
    for className in ImplicitClasses:
        for c in mb.class_(className).constructors():
            print "Checking", c.decl_string
            if "Quat" in c.decl_string or "Real" in c.decl_string:   ## exclude quats as cause issues
                c.allow_implicit_conversion = False
                print "NOT OK"
            elif "Vector" in c.decl_string :
                c.allow_implicit_conversion = True
                print "OK"
            else:
                print "NOT OK"
        for op in mb.class_(className).operators():
            print "Checking **", op.decl_string
            
            
def Add_Auto_Conversions( mb ):
    pass
    
      
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

                                
def Set_Exception(mb):
    pass
    #~ """We don't exclude  Exception, because it contains functionality, that could
    #~ be useful to user. But, we will provide automatic exception translator
    #~ """
    #~ Exception = mb.namespace( 'Ogre' ).class_( 'Exception' )
    #~ Exception.include()
    #~ Exception.mem_fun('what').exclude() # declared with empty throw
    #~ Exception.mem_fun('getNumber').exclude() # declared with empty throw
    #~ Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getFullDescription().c_str()' )
            
    
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
                fun.documentation = docit ("Modified Input Argument to work with CTypes",
                                            "Argument "+arg.name+ "(pos:" + str(arg_position)\
                                            +") takes a CTypes.adddressof(xx)", "...")
                #print "Fixed Void Ptr", fun, arg_position
                break
            arg_position +=1
            
   ## lets go and look for stuff that might be a problem        
    pointee_types=['unsigned int',' int ', ' float ', ' Real ', 'uchar', 'uint8',
             'unsigned char']
                          
    function_names=[]
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
                        print '"',arg.type.decl_string, '"'
                        print "CHECK ", fun, str(arg_position)
                        fun.documentation=docit("SUSPECT - MAYBE BROKEN", "....", "...")
                        break
            arg_position +=1

## NEED To do the same for constructors
    for fun in mb.constructors():
        arg_position = 0
        for arg in fun.arguments:
            if declarations.is_pointer(arg.type): ## and "const" not in arg.type.decl_string:
                for i in pointee_types:
                    if i in arg.type.decl_string:
                        print '"',arg.type.decl_string, '"'
                        print "Excluding: ", fun
                        fun.exclude()
                        break
            arg_position +=1         
                    
def Fix_Pointer_Returns ( mb ):
    """ Change out functions that return a variety of pointer to base types and instead
    have them return the address the pointer is pointing to (the pointer value)
    This allow us to use CTypes to handle in memory buffers from Python
    
    Also - if documentation has been set then ignore the class/function as it means it's been tweaked else where
    """
    pointee_types=['unsigned int','int', 'float', 'unsigned char']
    known_names=[]  # these are function names we know it's cool to exclude
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
        , messages.W1039       
        , messages.W1041
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
                        os.path.join( environment.nxogre_09.root_dir, "python_nxogre.h" )
                        , environment.nxogre_09.cache_file )
    if os.name == 'nt':
        defined_symbols = [ 'NxExport','OGRE_NONCLIENT_BUILD', 'OGRE_GCC_VISIBILITY', 'WIN32', 'GCC_XML']#NXOGRE_EXPORTS'
    else:
        defined_symbols = [ 'LINUX','NX_LINUX', 'NX_DISABLE_FLUIDS', 'OGRE_NONCLIENT_BUILD', 'OGRE_GCC_VISIBILITY', 'GCC_XML']

    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        defined_symbols.append('BOOST_HAS_WINTHREADS')
    defined_symbols.append( 'VERSION_' + environment.nxogre_09.version )  
    
    undefined_symbols = []
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.nxogre_09.include_dirs
                                          , define_symbols=defined_symbols
# #                                           , undefine_symbols = undefined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogre.cflags
                                           )
    # NOTE THE CHANGE HERE                                           
    mb.constructors().allow_implicit_conversion = False                                           
    ## This module depends on Ogre and physx
    mb.register_module_dependency ( environment.ogre.generated_dir )
    mb.register_module_dependency ( environment.physx.generated_dir )

    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
    
        
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    main_ns.include()
    
   
    AutoExclude ( mb )
    ManualExclude ( mb )
    AutoInclude ( mb )
    ManualInclude ( mb )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    common_utils.Auto_Functional_Transformation ( main_ns )  #, special_vars=['::Ogre::Real &','::Ogre::ushort &','size_t &'] )
    ManualTransformations ( mb )
    
    AutoFixes ( mb )
    ManualFixes ( mb )
            
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    Set_Call_Policies ( mb.global_ns.namespace (MAIN_NAMESPACE) )
    
# #     for op in main_ns.operators():
# #         print "op1", op
# # #         if op.allow_implicit_conversion:
# # #             print "Implicit conversion on operator ", op
# #     print dir(op)
# #     for op in main_ns.constructors():
# #         print "con1", op
# #         if op.allow_implicit_conversion:
# #             print "Implicit conversion on constructor ", op
# #     print dir(op)            
# #     for op in main_ns.free_operators():
# #         print "op2", op
# # #         if op.allow_implicit_conversion:
# # #             print "Implicit conversion on free operator ", op
# #     print dir(op)
            
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    NoPropClasses = [""]
    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            
    common_utils.add_constants( mb, { 'PROJECT_version' :  '"%s"' % environment.nxogre_09.version.replace("\n", "\\\n") 
                                      , 'python_version' : '"%s"' % sys.version.replace("\n", "\\\n" ) } )
                                      
    ## need to create a welcome doc string for this...                                  
    common_utils.add_constants( mb, { '__doc__' :  '"NxOgre 0.9 PROJECT DESCRIPTION"' } ) 
    
    
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor("") # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_nxogre_09_' , doc_extractor= extractor )
    
    for inc in environment.nxogre_09.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.nxogre_09.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.nxogre_09.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.nxogre_09.version ) )

    mb.split_module(environment.nxogre_09.generated_dir, huge_classes,use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generaated directory..
    common_utils.copyTree ( sourcePath = environment.Config.PATH_NxOgre_09, 
                            destPath = environment.nxogre_09.generated_dir, 
                            recursive=False )
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
