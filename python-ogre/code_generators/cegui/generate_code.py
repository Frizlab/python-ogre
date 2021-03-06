#!/usr/bin/env python

#
import os, sys, time

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
import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties


def filter_declarations( mb ):
    global_ns = mb.global_ns
    global_ns.exclude()
    global_ns.namespace('std').class_('pair<float, float>').include()
    
    CEGUI_ns = global_ns.namespace( 'CEGUI' )
    CEGUI_ns.include()
    
    ## not available at link time.. 
    CEGUI_ns.free_functions("lbi_greater").exclude()
    CEGUI_ns.free_functions("lbi_less").exclude()
    
    ## Dumb fix to remove all Properties classes -  unfortunately the properties in these classes are indicated as public
    ## however within their 'parent' class they are defined as private..
    ## see MultiLineEditboxProperties
    ##
    for cls in mb.global_ns.namespace ('CEGUI').classes():
        if "Properties" in cls.decl_string:
            print "Excluding:", cls.name
            cls.exclude()
          
                   
    ## EventNamespace causes failure when loading the python module
    ## possibly because of the ugly Properties fix above :) 
    for cls in mb.global_ns.namespace ('CEGUI').classes():
        try:
            cls.variable('EventNamespace').exclude()
        except:
            pass
    ## turns out that in SOME classes this also fails registration (Combodroplist for example)
    for cls in mb.global_ns.namespace ('CEGUI').classes():
        try:
            cls.variable('WidgetTypeName').exclude()
        except:
            pass
    ## fix due to new gccxml        
    try:
        global_ns.member_function('::CEGUI::ListboxItem::setSelectionColours', arg_types=['__restrict__ ::CEGUI::colour &']).exclude()
        global_ns.member_function('::CEGUI::ListboxTextItem::setTextColours', arg_types=['__restrict__ ::CEGUI::colour &']).exclude()
    except:
        pass       

    ## Exclude protected and private that are not pure virtual
    query = declarations.access_type_matcher_t( 'private' ) \
            & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
    CEGUI_ns.calldefs( query, allow_empty=True ).exclude()
    
    ## lets work around a bug in GCCXMl - http://language-binding.net/pygccxml/design.html#patchers
    draws = mb.member_functions( 'draw' )   # find all the draw functions
    for draw in draws:
        for arg in draw.arguments:  
            if arg.default_value == '0ffffffff':
                arg.default_value = '0xffffffff'

         
    ## class to exclude
    excludes=['::CEGUI::FactoryModule',
            '::CEGUI::ScriptFunctor',
            '::CEGUI::CEGUIRQListener',
            '::CEGUI::RefCounted< CEGUI::FormattedRenderedString >' # not in the debug version
            ]
    for e in excludes:
        try:
            CEGUI_ns.class_( e ).exclude()     
        except:
            print "FAILED to exclude:", e
    ## now have functions in String that return uint arrays a need to be wrapped
    sc = CEGUI_ns.class_( "String" )
    sc.member_functions('data').exclude()
    sc.member_functions('ptr').exclude()
    
    ## and only remove the at functions that are not returning constants
    ## the const version returns by value which is good, the non const returns a reference which doesn't compile
    sc.member_function( 'at', lambda decl: decl.has_const == False ).exclude()
    
    ## CEGUI::WindowManager::loadWindowLayout can take a function pointer as an agrument whcih isn't supported yet
    ## so lets remove the versions that expose the pointer 
    lo = CEGUI_ns.class_( 'WindowManager' ).member_function( 'loadWindowLayout', arg_types=[None, None, None, None, None] )
    lo.arguments[3].type = lo.arguments[4].type     #AJM Not sure how args work so setting the func pointer to a void pointer
    
    ## OgreCEGUIRenderer.h has an assumed namespace in one of the default agrs that we need to fix
    try:
        orRe = CEGUI_ns.constructor('OgreCEGUIRenderer', arg_types=[None, None, None, None] )
        pos = orRe.arguments[1].default_value.index("RENDER_QUEUE_OVERLAY")
        tempstring = orRe.arguments[1].default_value[:pos]+"::Ogre::"+orRe.arguments[1].default_value[pos:]
        orRe.arguments[1].default_value = tempstring
    except:
        pass
    # try to force this one..
    ## a string one that stops pydoc working against CEGUI
    CEGUI_ns.class_('ListHeader').variable('SegmentNameSuffix').exclude()
    #Exclude non default constructors of iterator classes. 
    for cls in CEGUI_ns.classes():
       if not declarations.templates.is_instantiation( cls.name ):
           continue
       name = declarations.templates.name( cls.name )
       if not name.endswith( 'Iterator' ):
           continue
       #default constructor does not have arguments
       constructors = cls.constructors( lambda decl: bool( decl.arguments )
                                                      , allow_empty=True
                                                      , recursive=False )
       constructors.exclude()

#     ## I'm going to exclude all iterators as there is a problem with CEGUIIteratorBase.h
    for cls in CEGUI_ns.classes():
# #         print "checking", cls.name
        if 'iterator' in cls.name.lower() :
            cls.exclude()
            print "Excluding Iterator", cls.name
            
    try:  # this is in the newer version of cegui so I'm OK if it fails     
        CEGUI_ns.class_('OgreCEGUIResourceProvider').exclude() # it's _ogrePrivate..
    except:
        pass
    ## Replaced these with 'useful' functions in the handwrappers - take and return python objects
    CEGUI_ns.class_( "Window" ).member_functions("setUserData").exclude()
    CEGUI_ns.class_( "Window" ).member_functions("getUserData").exclude()

    # Py++ doesn't know that this should be noncopyable so we set it here  
    nocopy=['EventSet','GlobalEventSet','MouseCursor','OgreCEGUIRenderer','CEGUIOgreRenderer',
            ]
    for c in nocopy:
        try:
            CEGUI_ns.class_(c).noncopyable = True
        except:
            pass
    for c in CEGUI_ns.classes():
        if c.name.endswith ("Exception"):
            c.noncopyable=True
    # changes to latest py++ can gccxml etc June 15 2008
    excludes = ['::CEGUI::ItemListBase::getSortCallback',
                '::CEGUI::OgreRenderer::createOgreImageCodec',
                '::CEGUI::RefCounted<CEGUI::FormattedRenderedString>::isValid']
    for f in excludes:
        try:
            CEGUI_ns.member_function(f).exclude()
        except:
            print "Couldn't exclude :",f
        
    CEGUI_ns.class_('RawDataContainer').exclude() # has pointers that need to be handled -- hopefully not needed    
    
    CEGUI_ns.member_function("::CEGUI::WindowManager::loadWindowLayout", arg_types=[None,None,None,None,None]).exclude()   
    
    
    
    global_ns.namespace( 'Ogre' ).class_('SceneManager').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('RenderWindow').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('TexturePtr').include(already_exposed=True)

    if environment.isLinux():
        for c in CEGUI_ns.classes():
            if c.name.endswith ('Exception') and c.name != 'Exception' :
                c.exclude()
                print "Excluded:", c

        e = ['::CEGUI::OgreRenderer::destroyOgreImageCodec',
        #     '::CEGUI::FileIOException',
            ]
        for c in e:
            global_ns.member_functions(c).exclude()
            
    cls = CEGUI_ns.class_('::CEGUI::RefCounted< CEGUI::BoundSlot >')
    cls.operator('==').exclude() # not in debug build

def set_call_policies( mb ):
    CEGUI_ns = mb.global_ns.namespace ('CEGUI')

    # Set the default policy to deal with pointer/reference return types to reference_existing object
    # as this is the CEGUI Default.
    mem_funs = CEGUI_ns.calldefs ()
    mem_funs.create_with_signature = True #Generated code will not compile on
    #MSVC 7.1 if function has throw modifier.
    for mem_fun in mem_funs:
        if mem_fun.call_policies:
            continue
        if declarations.is_pointer (mem_fun.return_type) or declarations.is_reference (mem_fun.return_type):
            mem_fun.call_policies = call_policies.return_value_policy(
                call_policies.reference_existing_object )
 
             
    ## OK so the CEGUI String class is ugly (from a python perspective) 
    ## I'm going to fix the pointer arguments so you can use ctypes to use the class
    ## however real work is in hand_made_wrappers with a new 'assign' function
    for fun in CEGUI_ns.class_('String').member_functions():
        arg_position = 0
        for arg in fun.arguments:
            if declarations.type_traits.is_pointer(arg.type): # 
                fun.add_transformation( ft.modify_type(arg_position,common_utils._ReturnUnsignedInt ), alias=fun.name )
                fun.documentation = common_utils.docit ("Modified Input Argument to work with CTypes",
                                            "Argument "+arg.name+ "(pos:" + str(arg_position)\
                                            +") takes a CTypes.adddressof(xx)", "...")
                break
            arg_position +=1
        
    for fun in CEGUI_ns.class_('String').constructors():
        arg_position = 0
        for arg in fun.arguments:
            if declarations.type_traits.is_pointer(arg.type):
                fun.add_transformation( ft.modify_type(arg_position,common_utils._ReturnUnsignedInt ), alias=fun.name )
                fun.documentation = common_utils.docit ("Modified Input Argument to work with CTypes",
                                            "Argument "+arg.name+ "(pos:" + str(arg_position)\
                                            +") takes a CTypes.adddressof(xx)", "...")
                break
            arg_position +=1
        
                
def configure_exception(mb):
    #We don't exclude  Exception, because it contains functionality, that could
    #be useful to user. But, we will provide automatic exception translator
    Exception = mb.namespace( 'CEGUI' ).class_( 'Exception' )
    Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getMessage().c_str()' )

## this is to fix specific challenges where a class (CaratIndex for example) is defined in multiple namespaces
##   
def change_cls_alias( ns ):
    for cls in ns.classes():
        if 1 < len( ns.classes( cls.name ) ):
            print "Length of classes is", len( ns.classes( cls.name ) ), " for ", cls.name
            for c in ns.classes( cls.name ):
               print "* ", c.decl_string
            print cls.decl_string
            alias = cls.decl_string[ len('::CEGUI::'): ]
            print "Adjust Alias for:",cls.decl_string, " with ", alias.replace ('::','')
            cls.alias = alias.replace( '::', '' )
            cls.wrapper_alias = cls.alias + 'Wrapper' # or 'Wrapper' ??
            ##cls.exclude()
# #         for f in cls.member_functions( allow_empty=True):
# #             if 1 < len( ns.member_functions( f.name ) ):
# #                 print "Length of functions is", len( ns.member_functions( f.name ) ), " for ", f.name
# #                 alias = cls.name + f.name
# #                 print "Adjust Alias for:",cls.name, f.name, " with ", alias
# # #                 f.alias = alias 
# #                 f.wrapper_alias = alias + 'Wrapper' 



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
# #         , messages.W1040 
# #         , messages.W1038        
#         , messages.W1041
#         , messages.W1036 # pointer to Python immutable member
#         , messages.W1033 # unnamed variables
#         , messages.W1018 # expose unnamed classes
# #         , messages.W1049 # returns reference to local variable
#         , messages.W1014 # unsupported '=' operator
#          )
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.cegui.root_dir, "python_CEGUI.h" )
                        , environment.cegui.cache_file )
                        
    defined_symbols = [ 'OGRE_NONCLIENT_BUILD', 'CEGUI_NONCLIENT_BUILD','OGRE_GCC_VISIBILITY','__PYTHONOGRE_BUILD_CODE',
                        'OGRE_GUIRENDERER_EXPORTS']
    defined_symbols.append( 'VERSION_' + environment.cegui.version.replace ('.','_')   )

    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        defined_symbols.append('BOOST_HAS_WINTHREADS')

    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.cegui.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogre.cflags
                                          )
    # if this module depends on another set it here                                           
    mb.register_module_dependency ( environment.ogre.generated_dir )
                                          
    filter_declarations (mb)
                                          
    ns=mb.global_ns.namespace ('CEGUI')                                      
    print "\n\n"
    c=ns.class_('::CEGUI::Checkbox')
    for f in c.member_functions():
        print "LENGTH ", len( ns.member_functions(f.name ) ), "for ", f.name
        for c in ns.member_functions(f.name ):
           print "==:", c.decl_string
        print f.decl_string  
# #     sys.exit()
    
                                            
   
    change_cls_alias( mb.global_ns.namespace ('CEGUI') )

    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
    
    mb.class_('::CEGUI::EventSet').member_functions('subscribeEvent').exclude()
#     mb.class_('::CEGUI::Scrollbar').member_functions('subscribeEvent').exclude()
    
    configure_exception( mb )

    hand_made_wrappers.apply( mb )

    set_call_policies (mb)
    for cls in mb.global_ns.namespace ('CEGUI').classes():
        cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t()  )
        common_utils.add_LeadingLowerProperties ( cls )
        
    v=mb.global_ns.namespace ('CEGUI') .class_('ScriptModule').variable('d_identifierString')
    print "INCLUDE:", v
    v.include()   
    print "DONE" 
                                      
    ## add additional version information to the module to help identify it correctly 
    common_utils.addDetailVersion ( mb, environment, environment.cegui )
    
    #Creating code creator. After this step you should not modify/customize declarations.
    extractor = exdoc.doc_extractor("")
    
    mb.build_code_creator (module_name='_cegui_', doc_extractor= extractor)
    print "VARIABLE exportable:", v.exportable
    
    for incs in environment.cegui.include_dirs:
        mb.code_creator.user_defined_directories.append( incs )
    mb.code_creator.user_defined_directories.append( environment.cegui.generated_dir )
    
#     mb.code_creator.replace_included_headers( customization_data.header_files(environment.cegui.version) )
    ## we need to remove the previous one
    lastc = mb.code_creator.creators[ mb.code_creator.last_include_index() ]
    mb.code_creator.remove_creator( lastc )  
    # and now add our precompiled ones..
    for x in range (len (customization_data.header_files( environment.cegui.version ) ), 0 ,-1 ):
        h = customization_data.header_files( environment.cegui.version )[x-1]        
        mb.code_creator.adopt_creator ( include.include_t ( header= h ), 0)


    huge_classes = map( mb.class_, customization_data.huge_classes(environment.cegui.version) )
    mb.split_module(environment.cegui.generated_dir, huge_classes)

if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Python-CEGUI source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
