
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


xml_cached_fc = parser.create_cached_source_fc(
                    os.path.join( environment.ogre.root_dir, "python_ogre.h" )
                    , environment.ogre.cache_file )

defined_symbols = [ 'OGRE_NONCLIENT_BUILD' ]
defined_symbols.append( 'OGRE_VERSION_' + environment.ogre.version )  

mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                      , gccxml_path=environment.gccxml_bin
                                      , working_directory=environment.root_dir
                                      , include_paths=environment.ogre.include_dirs
                                      , define_symbols=defined_symbols
                                      , indexing_suite_version=2
                                       )
mb.BOOST_PYTHON_MAX_ARITY = 25
mb.classes().always_expose_using_scope = True

global_ns = mb.global_ns
global_ns.exclude()
ogre_ns = global_ns.namespace( 'Ogre' )
ogre_ns.include()

# cls = ogre_ns.class_('Node')
# print cls
# print dir(cls)
# print cls.parent
# cls = ogre_ns.class_('SceneNode')
# print cls
# print dir(cls)
# print "Bases: ",cls.bases
# for b in cls.bases:
#     bases[b.related_class.name]=1
#     print b.access_type
    
    
# for op in mb.operators():
#    print op
for cls in mb.classes():
	print cls
sys.exit()

bases={}
for cls in ogre_ns.classes():
    for b in cls.bases:  ## lets store the parent/child relationships...
        try: 
            bases[b.related_class.name].append( cls.name )
        except:
            bases[b.related_class.name]= [cls.name]
for b in bases.keys():
        try:
            cls = ogre_ns.class_( b ) # get the class
        except:
            continue
        for f in cls.member_functions ( allow_empty = True):    # go through the member functions
            s = f.return_type.decl_string.split()[0]    # get the actual return type
            if "::Ogre::" + cls.name == s:    # does the member function return a class (of it's own type)
                print "\n",f            ## print it out...
                for c1 in bases[b]:
                    print "Used by", c1
            if cls.name.endswith ('Manager'):  ## It's a special - Ogre has manager classes & smart pointers :)
                basename = cls.name.rsplit('Manager',1)[0]  #get the base name
                if "::Ogre::" + basename + 'Ptr' == s:    # does it return the class Ptr ...
                    print "\n",f
                    for c1 in bases[b]:
                        print "Used by", c1
            
                     

#    for op in cls.operators( allow_empty = True ):
#       print cls, op
#     print cls
#     for f in cls.member_functions ( allow_empty = True):
#         for a in f.arguments:
#             print cls, f, a, a.type, a.type.decl_string
