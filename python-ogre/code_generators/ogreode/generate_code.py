#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------

## STARTER TEMPLATE..
## replace ogreode with lowercase project name
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

# setup a shortcut to the logger functions
from common_utils import log_exclude as log_exclude
from common_utils import log_include as log_include

MAIN_NAMESPACE = ['OgreOde','OgreOde_Prefab', 'OgreOde_Loader']

############################################################
##
##  Here is where we manually exclude stuff
##
############################################################

def ManualExclude ( mb ):
    global_ns = mb.global_ns
    
    #
    # Exclude problem functions
    #
    excludes=[  '::OgreOde_Loader::DotLoader::loadFile', 
                '::OgreOde::EntityInformer::getIndices',   # returns const * to TriangleIndex
                '::OgreOde_Prefab::Vehicle::load',
#                '::OgreOde::Geometry::setUserObject',   #hand wrapped
#                '::OgreOde::Geometry::getUserObject',   #hand wrapped
                '::OgreOde::PlaneBoundedRegionGeometry::_planeCallback',
                '::OgreOde::TerrainGeometry::_heightCallback',
                '::OgreOde::EntityInformer::getIndices', ## unsigned int const *
#                 '::OgreOde::RagdollFactory::requestTypeFlags', # causes issues with moveableobjectfactory
                '::OgreOde::EntityInformer::getBoneVertices',  # hand wrapped
                '::OgreOde::TriangleMeshData::getIndices', ## needs to be hand wrapped
#                 '::OgreOde::PlaneBoundedRegionGeometryPlaneListener::planesAt',
                '::OgreOde::World::fastStep', # it's not in the external lib
                ]
    for e in excludes:
        try:
            global_ns.member_functions(e).exclude()
            log_exclude(e)
        except:
            log_exclude(e, False) # log it failing

    global_ns.namespace( 'OgreOde' ).class_("CircularBuffer<OgreOde::BodyState*>").exclude() 
    #
    # Exclude problem variables
    #
    excludes = ['::OgreOde::Body::MovableType',
                '::OgreOde::Utility::Infinity'
                ]
    for e in excludes:
        try:
            global_ns.variable(e).exclude()
            log_exclude(e)
        except:
            log_exclude(e, False) # log it failing
    #
    # Exclude problem classes
    #
    # Unfortunately the classes here being used with Maintainedlist don't implement ALL of
    # maintaintlist's functions - hence Py++ has tried to expose them all and it fails at
    # compile time.  Probably need to patch the source to implement null functions.
    excludes =['MaintainedList<OgreOde::Body>',
                'MaintainedList<OgreOde::Geometry>',
                'MaintainedList<OgreOde::Joint>',
                'MaintainedList<OgreOde::JointGroup>', 
                'MaintainedList<OgreOde::Space>',
                'PlaneBoundedRegionGeometry'  # has std::list <plane> causing an issue
                ]
    for e in excludes:
        try:
            global_ns.namespace( 'OgreOde' ).class_(e).exclude()
            log_exclude(e)
        except:
            log_exclude(e, False) # log it failing
            
    #global_ns.namespace("Ogre").class_('list<Ogre::Plane>').include(already_exposed=True)  
    
    try: # this only works with ogre 1.7 so wrap in a try..
        global_ns.class_('::std::list<Ogre::Plane, Ogre::STLAllocator<Ogre::Plane, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >').include(already_exposed=True)
        print "***SPECIAL 1 OK ***"
    except:
        print "FAIL ON 1"
    try: # this only works with ogre 1.7 so wrap in a try..
        global_ns.class_('::std::list< Ogre::Plane >').include(already_exposed=True)  #std::list< Ogre::Plane >
        print "***SPECIAL 2 OK ***"
    except:
        print "FAIL ON 2"
    # a specal set as the constructors require pointers to vertex buffers which we can't do 
    # so make them no initable (so boost won't allow you to try and make them)
    # and have helper functions in hand wrappers to use instead   
    excludes =['::OgreOde::TriangleMeshGeometry'  
               ,'::OgreOde::ComplexGeometry'
               ]
#     for c in excludes:
#         print dir ( global_ns.class_( c ) )
#           
                
    global_ns.class_('::Ogre::Plane').include(already_exposed=True)
    
############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################
    
def ManualInclude ( mb, ns ):
    global_ns = mb.global_ns
#     if MAIN_NAMESPACE:
#         main_ns = global_ns.namespace( MAIN_NAMESPACE )
#     else:
#         main_ns = global_ns    
        
############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixes ( mb, ns ):    
    global_ns = mb.global_ns
#     if MAIN_NAMESPACE:
#         main_ns = global_ns.namespace( MAIN_NAMESPACE )
#     else:
#         main_ns = global_ns
    if ns == 'OgreOde': # we only need to do this once
        ## here we adjust for functions that return poiners to ODE "ID's", which are really C structs
        ## I may have been over agressive in identifing these functions but hopefully not...
        for func in mb.namespace( 'OgreOde' ).member_functions():  
            if func.return_type.decl_string.endswith('ID'):
                print "Setting ", func.name, "to Opaque"
                func.opaque = True
                func.call_policies = call_policies.return_value_policy(
                    call_policies.return_opaque_pointer )

    ## Functions that return objects we need to manage
    FunctionsToMemoryManage=[\
        '::OgreOde::EntityInformer::createStaticTriangleMesh',
        '::OgreOde::EntityInformer::createSingleStaticBox',
        '::OgreOde::EntityInformer::createSingleDynamicBox',
        '::OgreOde::EntityInformer::createSingleDynamicSphere'        
        ]
    for cls in FunctionsToMemoryManage:
        global_ns.mem_fun(cls).call_policies = call_policies.return_value_policy( call_policies.manage_new_object )  

        
############################################################
##
##  And things that need to have their argument and call values fixed.
##  ie functions that pass pointers in the argument list and of course we need
##  to read the updated values - so instead we pass them back 
##  as new values in a tuple (ETC ETC)
##
############################################################
        
def ManualTransformations ( mb, ns ):
    global_ns = mb.global_ns
#     if MAIN_NAMESPACE:
#         main_ns = global_ns.namespace( MAIN_NAMESPACE )
#     else:
#         main_ns = global_ns
                
    def create_output( size ):
        return [ ft.output( i ) for i in range( size ) ]
        
    
###############################################################################
##
##  Now for the AUTOMATIC stuff that should just work
##
###############################################################################
    
def AutoFixes ( mb, ns ): 
    """ now we fix a range of things automatically - typically by going through 
    the entire name space trying to guess stuff and fix it:)
    """    
    global_ns = mb.global_ns
    main_ns = mb.global_ns.namespace(ns)
        
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
        
    common_utils.Auto_Document( mb, ns )
        
 
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
        , messages.W1035
        , messages.W1040 
        , messages.W1038        
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
                        os.path.join( environment.ogreode.root_dir, "python_ogreode.h" )
                        , environment.ogreode.cache_file )

    defined_symbols = ['OGRE_NONCLIENT_BUILD', 'ODE_LIB','__PYTHONOGRE_BUILD_CODE' ]
    defined_symbols.append( 'OGREODE_VERSION_' + environment.ogreode.version )
      
    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        defined_symbols.append('BOOST_HAS_WINTHREADS')
        
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ogreode.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogreode.cflags
                                           )
                                           
    # if this module depends on another set it here                                           
    mb.register_module_dependency ( environment.ogre.generated_dir )
    mb.register_module_dependency ( environment.ode.generated_dir )

    # normally implicit conversions work OK, however they can cause strange things to happen so safer to leave off
    mb.constructors().allow_implicit_conversion = False                                           
    
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
            
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()

    query = lambda decl: isinstance( decl, ( declarations.class_t, declarations.class_declaration_t ) ) \
                         and decl.name.startswith( 'dx' )
    mb.global_ns.decls( query ).opaque = True  
    for ns in MAIN_NAMESPACE:
        main_ns = global_ns.namespace( ns )
        main_ns.include()
           
        common_utils.AutoExclude ( mb, ns )
        common_utils.AutoInclude ( mb, ns )
        ManualInclude ( mb, ns )
        # here we fixup functions that expect to modifiy their 'passed' variables    
        ManualTransformations ( mb, ns )
        AutoFixes ( mb, ns )
        ManualFixes ( mb, ns )
        common_utils.Auto_Functional_Transformation ( main_ns )

        #
        # We need to tell boost how to handle calling (and returning from) certain functions
        #
        common_utils.Set_DefaultCall_Policies ( mb.global_ns.namespace ( ns ) )
    ManualExclude ( mb )
    
    #mb.global_ns.class_('::std::list<Ogre::Plane, Ogre::STLAllocator<Ogre::Plane, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >').include(already_exposed=True)
    try:
        mb.global_ns.class_('::std::list<Ogre::Plane, Ogre::STLAllocator<Ogre::Plane, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >').exclude()
    except:
        pass
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    NoPropClasses = [""]
    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            
    ## add additional version information to the module to help identify it correctly 
    common_utils.addDetailVersion ( mb, environment, environment.ogreode )

    
    for cls in global_ns.classes():
        if not cls.ignore:
            print cls, cls.decl_string
    for cls in mb.global_ns.namespace("std").classes():
        if not cls.ignore:
            print cls, cls.decl_string
    
    
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor() # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_ogreode_' , doc_extractor= extractor )
    
    for inc in environment.ogreode.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ogreode.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.ogreode.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.ogreode.version ) )

    mb.split_module(environment.ogreode.generated_dir, huge_classes, use_files_sum_repository=False)

    if False:
        ## UGLY FIX
        ## we have a class that needs to be removed and for some reason py++ isn't doing it :(
        ##
        for f in  ['StdListPlane.pypp.cpp','StdListPlane.pypp.hpp']:
            fname = os.path.join( environment.ogreode.generated_dir, f)
            os.remove ( fname ) #remove the extra files
            
        # now remove the entries for the stdlistplane class     
        fname = os.path.join( environment.ogreode.generated_dir, '_ogreode_.main.cpp')  
        f = open(fname, 'r')
        buf = f.readlines()
        f.close()
        newbuf  =[]
        for line in buf:
            if ('stdlistplane' not in line ) and ('StdListPlane' not in line):
                newbuf.append(line)
        f = open ( fname, 'w+')
        f.writelines ( newbuf )
        f.close()
       
    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generated directory..
    
#     common_utils.copyTree ( sourcePath = environment.Config.PATH_INCLUDE_ogreode, 
#                             destPath = environment.ogreode.generated_dir, 
#                             recursive=False )
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
