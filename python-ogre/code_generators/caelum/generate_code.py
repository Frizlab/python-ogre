#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------

## STARTER TEMPLATE..
## replace caelum with lowercase caelum name
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

MAIN_NAMESPACE = 'Caelum'

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
    excludes=['::Caelum::FlatCloudLayer::disableCloudCoverLookup'
                ,'::Caelum::FlatCloudLayer::getCloudSpeed'
                ,'::Caelum::OwnedPtr< Ogre::ManualObject, Caelum::MovableObjectOwnedPtrTraits< Ogre::ManualObject > >::reset'
                ,'::Caelum::OwnedPtr< Ogre::Material, Caelum::OwnedResourcePtrTraits< Ogre::Material, Ogre::MaterialPtr, Ogre::MaterialManager > >::reset'
                ,'::Caelum::OwnedPtr< Ogre::SceneNode, Caelum::SceneNodeOwnedPtrTraits >::reset'
                ,'::Caelum::OwnedPtr< Ogre::Entity, Caelum::MovableObjectOwnedPtrTraits< Ogre::Entity > >::reset'
                ,'::Caelum::OwnedPtr< Ogre::Mesh, Caelum::OwnedResourcePtrTraits< Ogre::Mesh, Ogre::MeshPtr, Ogre::MeshManager > >::reset'
                ,'::Caelum::PrivatePtr< Ogre::BillboardSet, Caelum::MovableObjectPrivatePtrTraits< Ogre::BillboardSet > >::reset'
                ,'::Caelum::PrivatePtr< Ogre::Entity, Caelum::MovableObjectPrivatePtrTraits< Ogre::Entity > >::reset'
                ,'::Caelum::PrivatePtr< Ogre::ManualObject, Caelum::MovableObjectPrivatePtrTraits< Ogre::ManualObject > >::reset'
                ,'::Caelum::PrivatePtr< Ogre::Material, Caelum::PrivateResourcePtrTraits< Ogre::Material, Ogre::MaterialPtr, Ogre::MaterialManager > >::reset'
                ,'::Caelum::PrivatePtr< Ogre::Mesh, Caelum::PrivateResourcePtrTraits< Ogre::Mesh, Ogre::MeshPtr, Ogre::MeshManager > >::reset'
                ,'::Caelum::PrivatePtr< Ogre::SceneNode, Caelum::SceneNodePrivatePtrTraits >::reset'
                ,'::Caelum::DefaultTypeDescriptor::clear'
                ]
    for m in excludes:
        try:
            global_ns.member_functions(m).exclude()
            print "Excluded:", m
        except:
            print "Failed to Excluded:", m
    
    excludes=['::Caelum::CameraBoundElement::CAMERA_FAR_DISTANCE_MULTIPLIER']
    for e in excludes:
        global_ns.variable(e).exclude()  
        
    excludes = ['::Caelum::ValuePropertyDescriptor'  # problem with OgreAny on getvalue -- needs wrapping
                ]
    for e in excludes:
        global_ns.class_(e).exclude()  
              
   
        
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
         
# #     global_ns.namespace( 'Ogre' ).class_('FrameListener').include(already_exposed=True)
# #     global_ns.namespace( 'Ogre' ).class_('RenderTargetListener').include(already_exposed=True)
        
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
    # Py++ doesn't know that this should be noncopyable so we set it here        
    main_ns.class_('CaelumSystem').noncopyable = True
#     main_ns.class_('LayeredClouds').noncopyable = True
              
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

    return        
    x= main_ns.mem_funs('::Caelum::Astronomy::getGregorianDateFromJulianDay')
    x.add_transformation( ft.output('year'), ft.output('month'), ft.output('day'), alias = 'getGregorianDateFromJulianDay' )
    x.documentation = docit ("","JulianDay", "tuple - year,month,day")
        
    x=main_ns.mem_fun('::Caelum::Astronomy::getGregorianDateTimeFromJulianDay')
    x.add_transformation( ft.output('year'), ft.output('month'), ft.output('day'),ft.output('hour'), ft.output('minute'), ft.output('second'), alias='getGregorianDateTimeFromJulianDay' )
    x.documentation = docit ("","JulianDay", "tuple - year,month,day,hour, minute, second")

    x=main_ns.mem_fun('::Caelum::Astronomy::convertEclipticToEquatorialRad')
    x.add_transformation( ft.inout ('rasc'), ft.inout('decl'), alias='convertEclipticToEquatorialRad')
    x.documentation = docit("","","tuple - rasc, decl")
    
    x=main_ns.mem_fun('::Caelum::Astronomy::convertRectangularToSpherical')
    x.add_transformation( ft.inout ('rasc'), ft.inout('decl'), ft.inout('dist'), alias='convertRectangularToSpherical')
    x.documentation = docit("","","tuple - rasc, decl, dist")
    
    x=main_ns.mem_fun('::Caelum::Astronomy::convertSphericalToRectangular')
    x.add_transformation( ft.inout ('x'), ft.inout('y'), ft.inout('z'), alias='convertSphericalToRectangular')
    x.documentation = docit("","","tuple - x,y,z")
    
    x=main_ns.mem_fun('::Caelum::Astronomy::convertEquatorialToHorizontal')
    x.add_transformation( ft.inout ('azimuth'), ft.inout('altitude'), alias='convertEquatorialToHorizontal')
    x.documentation = docit("","","tuple - azimuth,altitude")
    
    x=main_ns.mem_fun('::Caelum::Astronomy::getHorizontalSunPosition', arg_types=['::Caelum::LongReal','::Caelum::LongReal','::Caelum::LongReal','::Caelum::LongReal &','::Caelum::LongReal &'])
    x.add_transformation( ft.inout ('azimuth'), ft.inout('altitude'), alias='getHorizontalSunPositionDouble')
    x.documentation = docit("","","tuple - azimuth,altitude")

    x=main_ns.mem_fun('::Caelum::Astronomy::getHorizontalMoonPosition',arg_types=['::Caelum::LongReal','::Caelum::LongReal','::Caelum::LongReal','::Caelum::LongReal &','::Caelum::LongReal &'])
    x.add_transformation( ft.inout ('azimuth'), ft.inout('altitude'), alias='getHorizontalMoonPositionDouble')
    x.documentation = docit("","","tuple - azimuth,altitude")
    
    x=main_ns.mem_fun('::Caelum::Astronomy::getEclipticMoonPositionRad')
    x.add_transformation( ft.inout ('lon'), ft.inout('lat'), alias='getEclipticMoonPositionRad')
    x.documentation = docit("","","tuple - lon,lat")

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
# #         , messages.W1035
# #         , messages.W1040 
# #         , messages.W1038        
# #         , messages.W1041
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
                        os.path.join( environment.caelum.root_dir, "python_caelum.h" )
                        , environment.caelum.cache_file )

    defined_symbols = [ 'OGRE_NONCLIENT_BUILD','__PYTHONOGRE_BUILD_CODE' ]
    defined_symbols.append( 'VERSION_' + environment.caelum.version )  
    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        defined_symbols.append('BOOST_HAS_WINTHREADS')
    
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.caelum.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.caelum.cflags
                                           )
                                           
    # if this module depends on another set it here                                           
    mb.register_module_dependency ( environment.ogre.generated_dir )
    
    # normally implicit conversions work OK, however they can cause strange things to happen so safer to leave off
# #     mb.constructors().allow_implicit_conversion = False                                           
    
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
            
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    main_ns.include()
       
    common_utils.AutoExclude ( mb, MAIN_NAMESPACE )
    ManualExclude ( mb )
    common_utils.AutoInclude ( mb, MAIN_NAMESPACE )
    ManualInclude ( mb )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    AutoFixes ( mb, MAIN_NAMESPACE )
    ManualFixes ( mb )
    common_utils.Auto_Functional_Transformation ( main_ns  )

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
    common_utils.addDetailVersion ( mb, environment, environment.caelum )
                                      
   
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor() # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_caelum_' , doc_extractor= extractor )
    
    for inc in environment.caelum.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.caelum.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.caelum.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.caelum.version ) )

    mb.split_module(environment.caelum.generated_dir, huge_classes, use_files_sum_repository=False)

    additional_dirs=[ [environment.Config.PATH_caelum, '.'],
                ]
    for d,dd in additional_dirs:
        for f in os.listdir(d):
            if f.endswith('cpp') or f.endswith('.h'):
                sourcefile = os.path.join(d, f)
                destfile = os.path.join(environment.caelum.generated_dir, dd, f ) 
                if not os.path.exists ( os.path.join(environment.caelum.generated_dir, dd ) ):
                    os.mkdir ( os.path.join(environment.caelum.generated_dir, dd ) )
                if not common_utils.samefile( sourcefile ,destfile ):
                    shutil.copy( sourcefile, destfile )
                    print "Updated ", f, "as it was missing or out of date"
       
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
