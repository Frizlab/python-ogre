#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------

## STARTER TEMPLATE..
## replace ois with lowercase project name
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
from pyplusplus.code_creators import include

import common_utils.extract_documentation as exdoc
#import common_utils.sphinx_extract as exdoc


import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties
from common_utils import docit

MAIN_NAMESPACE = 'OIS'

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
    excludes=["::OIS::JoyStickState::mVectors"]
    for e in excludes:
        main_ns.variable(e).exclude()
    if environment.isMac():
        for c in main_ns.classes():
            if 'JoyStick' in c.name:
                print "excluding:", c
                c.exclude()
        for c in main_ns.member_functions():
            if 'JoyStick' in c.name:
                print "excluding:", c
                c.exclude()
        for c in main_ns.variables():
            if 'JoyStick' in c.name:
                print "excluding:", c
                c.exclude()

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

    try:        ## this section broken on linux
        # needed to extract values from a vector<bool>
        global_ns.namespace ( 'std' ).class_("_Vb_reference<unsigned int, int, std::vector<bool, std::allocator<bool> > >").include()

        # changes relating to vector<bool> support
        global_ns.namespace ( 'std' ).class_("_Vb_reference<unsigned int, int, std::vector<bool, std::allocator<bool> > >").\
            member_function("_Getptr").exclude() ## call_policies=call_policies.return_value_policy( call_policies.reference_existing_object )

        # exclude the '=' and then include the simple one -- not sure this is needed ??
        for o in global_ns.namespace ( 'std' ).class_("_Vb_reference<unsigned int, int, std::vector<bool, std::allocator<bool> > >").operators('='):
            o.exclude()
        o = global_ns.namespace ( 'std' ).class_("_Vb_reference<unsigned int, int, std::vector<bool, std::allocator<bool> > >").operator('=',arg_types=["bool"])
        o.include()
    except:
        pass ## AJM TOFIX

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
    mb.class_( "Mouse" ).member_function( "getMouseState" ).call_policies =\
        call_policies.return_value_policy( call_policies.reference_existing_object )
    mb.class_( "JoyStick" ).member_function( "getJoyStickState" ).call_policies =\
        call_policies.return_value_policy( call_policies.reference_existing_object )


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
                        os.path.join( environment.ois.root_dir, "python_ois.h" )
                        , environment.ois.cache_file )

    defined_symbols = ['OIS_NONCLIENT_BUILD' ]
    defined_symbols.append( 'VERSION_' + environment.ois.version )
    if environment.isMac():
        defined_symbols.append ('OIS_APPLE_PLATFORM')
    #
    # build the core Py++ system from the GCCXML created source
    #
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ois.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ois.cflags
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
    common_utils.addDetailVersion ( mb, environment, environment.ois )

    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor() # I'm excluding the UTFstring docs as lots about nothing
    mb.build_code_creator (module_name='_ois_' , doc_extractor= extractor )

    for inc in environment.ois.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ois.generated_dir )

# # #     mb.code_creator.replace_included_headers( customization_data.header_files( environment.ois.version ) )
    ## we need to remove the previous one
    lastc = mb.code_creator.creators[ mb.code_creator.last_include_index() ]
    mb.code_creator.remove_creator( lastc )
    # and now add our precompiled ones..
    for x in range (len (customization_data.header_files( environment.ois.version ) ), 0 ,-1 ):
        h = customization_data.header_files( environment.ois.version )[x-1]
        mb.code_creator.adopt_creator ( include.include_t ( header= h ), 0)


    huge_classes = map( mb.class_, customization_data.huge_classes( environment.ois.version ) )

    mb.split_module(environment.ois.generated_dir, huge_classes, use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generated directory..

#     common_utils.copyTree ( sourcePath = environment.Config.PATH_INCLUDE_ois,
#                             destPath = environment.ois.generated_dir,
#                             recursive=False )

if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
