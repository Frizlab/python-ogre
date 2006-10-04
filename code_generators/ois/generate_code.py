#!/usr/bin/env python

#-------------------------------------------------------------------------------
# TODO:
# 1.    void* as a function argument - they are currently wrapped (and compile/load etc) due to latest CVS of boost.
#       However probably don't actually work
# 2.    Properties.py and calling 'properties.create' - commented out at the moment, not sure if it is really needed?

import os, sys, time
import environment, ois_customization_data, hand_made_wrappers, properties2

from pyplusplus import code_creators
from pyplusplus import module_builder
from pyplusplus.module_builder import call_policies

from pygccxml import declarations
from pygccxml.declarations import access_type_matcher_t
from pygccxml.declarations import type_traits
from pygccxml import parser

class decl_starts_with (object):
    def __init__ (self, prefix):
        self.prefix = prefix
    def __call__ (self, decl):
        return self.prefix in decl.name

def fix_unnamed_classes (mb):
    ois_ns = mb.namespace  ('OIS')
    try:    # wrap it with try as there might not be any unnamed classes
       for unnamed_cls in ois_ns.classes( '' ):
           named_parent = unnamed_cls.parent
           if not named_parent.name:
               named_parent = named_parent.parent
   
           for mvar in unnamed_cls.public_members:
               if not mvar.name:
                   continue
   
               if declarations.is_array (mvar.type):
                   template = '''def_readonly("%(mvar)s", &OIS::%(parent)s::%(mvar)s)'''
               else:
                   template = '''def_readwrite("%(mvar)s", &OIS::%(parent)s::%(mvar)s)'''
               named_parent.add_code( template % dict( mvar=mvar.name, parent=named_parent.name ) )
    except:
      print "No Unnamed classes to deal with"


class private_decls_t:
    def __init__( self ):
        self.__private = {} #fname : [line ]
        for fname in os.listdir( environment.headers_dir ):
            full_name = os.path.join( environment.headers_dir, fname )
            if not os.path.isfile( full_name ):
                continue
            fobj = file( full_name, 'r' )
            for index, line in enumerate( fobj ):
                if '_OgrePrivate' in line:
                    if not self.__private.has_key( fname ):
                        self.__private[ fname ] = []
                    self.__private[ fname ].append( index + 1 ) #enumerate calcs from 0, while gccxml from 1
                line = line.strip()
                if line.startswith( '/// Internal method ' ) or line.startswith( '/** Internal class' ):
                    if not self.__private.has_key( fname ):
                        self.__private[ fname ] = []
                    self.__private[ fname ].append( index + 2 ) #enumerate calcs from 0, while gccxml from 1

            fobj.close()

    def is_private( self, decl ):
        if None is decl.location:
            return False
        file_name = os.path.split( decl.location.file_name )[1]
        return self.__private.has_key( file_name ) and decl.location.line in self.__private[ file_name ]

def filter_declarations( mb ):
    global_ns = mb.global_ns
    global_ns.exclude()
    OIS_ns = global_ns.namespace( 'OIS' )
    OIS_ns.include()
      
    startswith = [
        # Don't include, we'll never need.
    ]

    ## Remove private classes , and those that are internal to OIS...
    ## Needs to be fixed for OIS
#     private_decls = private_decls_t()
#     for cls in OIS_ns.classes():
#         if private_decls.is_private( cls ):
#             cls.exclude()
#             print '{*} class "%s" is marked as private' % cls.decl_string

#     for func in OIS_ns.calldefs():
#         if private_decls.is_private( func ):
#             if func.virtuality == declarations.VIRTUALITY_TYPES.PURE_VIRTUAL:
#                 continue
#             func.exclude()
#             print '{*} function "%s" is marked as internal' % declarations.full_name( func )

    ## Exclude protected and private that are not pure virtual
#     query = ~declarations.access_type_matcher_t( 'public' ) \
#             & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
#     non_public_non_pure_virtual = OIS_ns.calldefs( query )
#     non_public_non_pure_virtual.exclude()

#     ## Exclude all public not pure virtual, that starts with '_'
#     ### AJM Although it you want to include them thats OK :)
#     if True:
#         query = declarations.access_type_matcher_t( 'public' ) \
#                 & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL ) \
#                 & declarations.custom_matcher_t( lambda decl: decl.name.startswith( '_' ) )
#         non_public_non_pure_virtual = OIS_ns.calldefs( query )
#         non_public_non_pure_virtual.exclude()
#         
#     else:   
#         ## AJM Having allowed functions starting with '_' I now need to exclude these are MSVC 7 compiler 
#         ## pukes with an overflow when compiling 'StaticGeometry.pypp.cpp'..
#         GeomRegion = OIS_ns.class_( "StaticGeometry" ).class_("Region")
#         GeomRegion.member_functions('_notifyCurrentCamera'). exclude()
#         GeomRegion.member_functions('_updateRenderQueue'). exclude()
        
    ## Now get rid of a wide range of classes as defined earlier in startswith...
    for prefix in startswith:
        classes = OIS_ns.classes (decl_starts_with(prefix), allow_empty=True)  ### NOTE the PREFIX is used here !!!!
        classes.exclude()



def set_call_policies( mb ):
    OIS_ns = mb.global_ns.namespace ('OIS')

    # Set the default policy to deal with pointer/reference return types to reference_existing object
    # as this is the OIS Default.
    mem_funs = OIS_ns.calldefs ()
    mem_funs.create_with_signature = True #Generated code will not compile on
    #MSVC 7.1 if function has throw modifier.
    for mem_fun in mem_funs:
        if mem_fun.call_policies:
            continue
        if declarations.is_pointer (mem_fun.return_type) or declarations.is_reference (mem_fun.return_type):
            mem_fun.call_policies = call_policies.return_value_policy(
                call_policies.reference_existing_object )

                
                
def generate_alias (mb):
    gns = mb.global_ns
#     for cls in mb.classes():
#         print "CLASS2: ", cls
#     for item in mb.decls():
#         print "DECL2: ", item    

    for name, alias in ois_customization_data.aliases.items():
        try:
            cls = gns.class_( name )
            cls.alias = alias
            cls.wrapper_alias = alias + '_wrapper'
            #print '{!}setting alias to %s ' % str( decl )
            #for source_alias in cls.aliases:
            #    print '  {source code defines next alias} %s' % str( source_alias )
        except declarations.matcher.declaration_not_found_t:
            gns.decl( name, decl_type=declarations.class_declaration_t ).rename( alias )
                
# def generate_alias (mb):
#     for name, alias in ois_customization_data.name2alias.items():
#         print "Looking for ", name
#         try:
#             decl = mb.class_( name )
#             print '{!}setting %s class to %s ' % (name, alias)
#             for source_alias in decl.aliases:
#                 print '  {source code defines next alias} %s' % str( source_alias )
#             decl.alias = alias
#             decl.wrapper_alias = alias + '_wrapper'
#         except  Exception, error:
#             print "==>Not Found..", name
#     for name, alias in ois_customization_data.name2alias_class_decl.items():
#         try:
#             decl = mb.decl( name, lambda decl: isinstance( decl, declarations.class_declaration_t ) )
#             decl.alias = alias
#             print '{!}setting %s variable to %s ' % (name, alias)
#         except  Exception, error:
#             print "==>Not Found..", name
#     for cls in mb.classes():
#         print "CLASS2: ", cls
#     for item in mb.decls():
#         print "DECL2: ", item    

def configure_exception(mb):
    #We don't exclude  Exception, because it contains functionality, that could
    #be useful to user. But, we will provide automatic exception translator
    Exception = mb.namespace( 'OIS' ).class_( 'Exception' )
## OIS doesn't have a full description function so can't make it pretty
#     Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getFullDescription().c_str()' )


def generate_code():
    xml_cached_fc = parser.create_cached_source_fc( "python_ois.h", environment.declarations_cache_file )
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_path
                                          , working_directory=environment.working_dir
                                          , include_paths=[environment.headers_dir]
                                          , define_symbols=['OIS_NONCLIENT_BUILD']
                                          , start_with_declarations=['OIS']
                                          , indexing_suite_version=2 )

    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
    
    filter_declarations (mb)

    fix_unnamed_classes (mb)
    generate_alias (mb)
#    shared_ptr.configure (mb)
    configure_exception( mb )
    hand_made_wrappers.apply( mb )

#    for cls in mb.namespace( 'Ogre' ).classes():
#         cls.add_registration_code( 'std::cout << "registering class %s" << std::endl;' % cls.alias, False )

    #Creating code creator. After this step you should not modify/customize declarations.
    mb.build_code_creator (module_name='_ois_')

    # Create properties for accessors
    properties2.create (mb)
    set_call_policies (mb) # remove all the undefined call policies...

    mb.code_creator.user_defined_directories.append( environment.headers_dir )
    mb.code_creator.user_defined_directories.append( environment.build_dir )
    mb.code_creator.replace_included_headers( ois_customization_data.header_files )

    huge_classes = map( mb.class_, ois_customization_data.huge_classes )

    mb.split_module(environment.build_dir, huge_classes)

# vim:et:ts=4:sts=4:sw=4

if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Python-OIS source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
