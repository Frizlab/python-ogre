#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-MAIN_NAMESPACE and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------

## STARTER TEMPLATE..
## replace ogrepcz with lowercase project name
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

def ManualExclude ( mb ):
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns    
        
    global_ns.namespace('std').class_('map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >').exclude()
    
    excludes=[ # '::Ogre::PCZCamera',
            '::Ogre::PCZSceneManagerFactory',
            '::Ogre::PCZone::PortalSortDistance',
            '::std::map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >'
            ]
    for e in excludes:
        try:
            main_ns.class_(e).exclude()
            print "Excluded Class:", e
        except:
            print "Failed to Exclude Class:", e
            
    excludes=['::Ogre::PCZSceneManagerFactory::initMetaData',
              '::Ogre::PCZSceneManagerFactory::createInstance',
              '::Ogre::PCZSceneManagerFactory::destroyInstance',
              '::Ogre::Portal::setCorners',  # hand wrapped..
              '::Ogre::PCZCamera::isVisibile',
              '::Ogre::MapIterator< std::map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > >::peekNextValuePtr'
              ]
    for e in excludes:
        try:
            main_ns.member_functions(e).exclude()
            print "Excluded Member Function(s):", e
        except:
            print "Failed to exclude:", e
        
    excludes=['::Ogre::PCZSceneManagerFactory::FACTORY_TYPE_NAME'
               ]
    for e in excludes:
        try:
            main_ns.variable(e).exclude()
            print "Excluded Variable:", e
        except:
            print "Failed to exclude:", e

    std_ns = global_ns.namespace('std')
    if False:
        for c in std_ns.classes():
            if 'map' in c.decl_string and 'Ogre' in c.decl_string:
                print c.name, c.decl_string
#     std_ns.class_('::std::vector<Ogre::Plane, std::allocator<Ogre::Plane> >')    
# #     std_ns.class_('::std::map<std::string, std::allocator<std::string> >')
        sys.exit()    
                       
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
    
    includes = ['::Ogre::Portal::PORTAL_TYPE',
                '::Ogre::Portal::PortalIntersectResult',
#                 '::Ogre::PCZone::NODE_LIST_TYPE'
            ]                 
    for i in includes:
        try:
            main_ns.enum(i).include()  
        except:
            pass
    for c in main_ns.classes():
        if "Singleton" in c.decl_string:
            print c, c.name, c.decl_string                  
    includes = ['::Ogre::Singleton<Ogre::PCZoneFactoryManager>',
                'Ogre::Singleton<Ogre::PCZoneFactoryManager>',
                '::Ogre::Singleton<::Ogre::PCZoneFactoryManager>',
                'Singleton<PCZoneFactoryManager>',
                '::Ogre::AntiPortal',
                '::Ogre::PCZCamera',
                '::Ogre::ZoneData',
                #'::Ogre::Capsule',  # these aren't exposed in the dll
                #'::Ogre::Segment',
                '::Ogre::MapIterator<std::map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > >',
                '::Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::PCZoneFactory*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, Ogre::STLAllocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::PCZoneFactory*>, Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL> > > >',
#### AJM to review
###                '::std::map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >'
                ]  
    for i in includes:
        try:
            main_ns.class_(i).include()
            print "Forced Include:", i
        except:
            print "Failed to Include:", i
                        
    try:    #only in newer versions of Ogre...
        main_ns.class_('map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >').alias = "stdMapPCZone1"                            
        main_ns.class_('map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >').include()
    except:
        pass
    main_ns.class_('RaySceneQuery').include(already_exposed=True)            
    main_ns.class_('DefaultRaySceneQuery').include(already_exposed=True)  
    main_ns.class_('SceneQuery').include(already_exposed=True)  
    main_ns.class_('RaySceneQueryListener').include(already_exposed=True)  

    # for c in global_ns.classes():
        # if "_" in c.alias: # only care about classes we are include with ugly alaises
            # print "ALIAS:", c, c.alias, c.ignore

    c = global_ns.class_('::std::map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >')
    c.alias="stdMapOgrePCZone"
    c.wrapper_alias="Wrapper_stdMapOgrePCZone"
    c.exclude()
    c.include(already_exposed=True)
    
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
    includes = ['::Ogre::Singleton<Ogre::PCZoneFactoryManager>'
            ]             
    for c in global_ns.classes():
        for i in includes_already:
            if c.decl_string.startswith(i):
                print "Including Special(already Exposed):", c
                c.include(already_exposed=True)
        for i in includes:
            if c.decl_string.startswith(i):
                print "Including Special:", c
                c.include()                
                
    noncopy=[]
    # these showed up during threading -- possible needs to be done all the time (needs to be looked at)
    if environment._USE_THREADS:
        noncopy = noncopy + ['PCZSceneManager' ]
    for c in noncopy:
        main_ns.class_(c).noncopyable = True
        


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
         , messages.W1035
         , messages.W1040 
         , messages.W1038        
         , messages.W1041
#         , messages.W1036 # pointer to Python immutable member
#         , messages.W1033 # unnamed variables
#         , messages.W1018 # expose unnamed classes
#         , messages.W1049 # returns reference to local variable
#         , messages.W1014 # unsupported '=' operator
          )
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.ogrepcz.root_dir, "python_ogrepcz.h" )
                        , environment.ogrepcz.cache_file )

    defined_symbols = [ 'OGRE_NONCLIENT_BUILD', 'OGRE_GCC_VISIBILITY', 'OGRE_PCZPLUGIN_EXPORTS',
                        '__PYTHONOGRE_BUILD_CODE']
    defined_symbols.append( 'VERSION_' + environment.ogrepcz.version )  
    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        defined_symbols.append('BOOST_HAS_WINTHREADS')

    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ogrepcz.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogrepcz.cflags
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
#     std_ns = global_ns.namespace('std')
#     std_ns.include(already_exposed=True) 
    
    # We don't include all of MAIN_NAMESPACE otherwise we get the same full wrapper
    # so instead we include classes with names that start with PCZ
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
# #     main_ns.include( already_exposed = True )  ## force the ogre class to be exposed as well...
    
    for c in main_ns.classes():
        if c.name.startswith ('PCZ') or c.name.startswith ('PCPlane') or  c.name.startswith ('Portal'):
            c.include()
            print "Including ", c, c.already_exposed
            
    for c in ['Portal','PCPlane']:
        main_ns.class_(c).include()
        print "Including ", c            
    
    common_utils.AutoExclude ( mb, MAIN_NAMESPACE )
    ManualInclude ( mb )
    ManualExclude ( mb )
    common_utils.AutoInclude ( mb, MAIN_NAMESPACE )
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
    common_utils.addDetailVersion ( mb, environment, environment.ogrepcz )

    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor() # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_ogrepcz_' , doc_extractor= extractor )
    
    for inc in environment.ogrepcz.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ogrepcz.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.ogrepcz.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.ogrepcz.version ) )

    mb.split_module(environment.ogrepcz.generated_dir, huge_classes, use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generated directory..
    
#     common_utils.copyTree ( sourcePath = environment.Config.PATH_INCLUDE_ogrepcz, 
#                             destPath = environment.ogrepcz.generated_dir, 
#                             recursive=False )
    if environment.ogre.version.startswith("1.7"):
        ## have a code generation issue that needs resolving...
        filesToFix=['PCZoneFactoryManager.pypp.cpp']
        for filename in filesToFix:
            fname = os.path.join( environment.ogrepcz.generated_dir, filename)
            try:
                f = open(fname, 'r')
                buf = f.read()
                f.close()
                buf = buf.replace ( " MEMCATEGORY_GENERAL", " Ogre::MEMCATEGORY_GENERAL")
                buf = buf.replace ( "<MEMCATEGORY_GENERAL", "<Ogre::MEMCATEGORY_GENERAL")
                f = open ( fname, 'w+')
                f.write ( buf )
                f.close()
                print "UGLY FIX OK:", fname
            except:
                print "ERROR: Unable to fix:", fname        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
