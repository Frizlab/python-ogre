#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------

## STARTER TEMPLATE..
## replace mygui with lowercase project name
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

MAIN_NAMESPACE = 'MyGUI'

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
    c=main_ns.class_('KeyCode')   
    print dir(c) 
    for o in c.operators():
      print "O:",o.name, o
    for o in c.member_operators():
      print "MO:", o.name, o
    for o in c.properties:
      print "P:", o.name, o
    c.operator ('=').exclude()
#     c.operator ('!=').exclude()
    for o in c.decls():
      print "D:",o.name, o
    
    c=main_ns.class_('::MyGUI::delegates::CMultiDelegate1< std::string const& >')
    for o in c.operators():
      print "O:",o.name, o
    c.operator('+=').exclude()
    c.operator('-=').exclude()
        
    e = ['::MyGUI::delegates::CDelegate3< MyGUI::Widget*, std::string const&, std::string const& >',
         '::MyGUI::delegates::CDelegate2< MyGUI::Widget*, unsigned int >',
         '::MyGUI::ControllerPosition',
         '::MyGUI::IBNotifyItemData',
         '::MyGUI::ScrollViewBase'  # doesn have export parameters
         ]
    for c in e:
        print "Excluding:",c
        main_ns.class_(c).exclude()

    for c in main_ns.classes():
        if 'delegates' in c.decl_string:
            print "Excuding asssign operator from:", c
            c.operators('=').exclude()  
#             try:
#                c.operators('()').exclude()
#                print "Special exclude operator:", c
#             except:
#                pass               
    c=['::MyGUI::Font',
       '::MyGUI::FontManager',
       '::MyGUI::Widget'
       ]
    for e in c:
        main_ns.class_(e).noncopyable=True
    c=['::MyGUI::LogManager::LevelsName'
       ]
    for e in c:
        main_ns.variable(e).exclude()
    
    c=['::MyGUI::utility::templates::split'
       ,'::MyGUI::utility::templates::parseColour'
       ,'::MyGUI::templates::get_vector_resource_path'
       ,'::MyGUI::templates::get_resource_path'
       ]
    for e in c:
      try:
         main_ns.free_functions(e).exclude() 
      except:
         print "Unable to exclude function:", e   
    
    for f in main_ns.member_functions():
      if f.name == 'onKeyButtonPressed' or f.name == 'onKeyButtonReleased':
         print "Excluding:", f
         f.exclude()
		 
    # excluding all delegates    
    excludes = ['::MyGUI::delegates::CDelegate1<MyGUI::Widget*>',
               #'::MyGUI::delegates::CDelegate1<float>',
               '::MyGUI::delegates::CDelegate2< MyGUI::Widget*, MyGUI::KeyCode >',
               '::MyGUI::delegates::CDelegate3< MyGUI::Widget*, MyGUI::KeyCode, unsigned int >',
               '::MyGUI::delegates::CDelegate2<MyGUI::Widget*, MyGUI::Widget*>',
               '::MyGUI::delegates::CDelegate2<MyGUI::Widget*, bool>',
               '::MyGUI::delegates::CDelegate2<MyGUI::Widget*, int>',
               #'::MyGUI::delegates::CDelegate2<MyGUI::Widget*, size_t>',
               '::MyGUI::delegates::CDelegate3<MyGUI::Widget*, int, int>',
               '::MyGUI::delegates::CDelegate4<MyGUI::Widget*, int, int, MyGUI::MouseButton>',
               #'::MyGUI::delegates::CDelegate3<MyGUI::Widget*, const std::string&, const std::string&>',
               #'::MyGUI::delegates::CDelegate3<MyGUI::Widget*, MyGUI::Widget* &, size_t &>',
               #'::MyGUI::delegates::CDelegate2<MyGUI::Widget*, const MyGUI::ToolTipInfo & >'
               ]
    for e in excludes:
      main_ns.class_(e).operators('()').exclude()

    excludes = ['::MyGUI::KeyCode']
    for c in excludes:
        main_ns.class_(c).exclude()
    
    for f in main_ns.class_('MenuCtrl').member_functions():
      for a in f.arguments:
         if a.default_value =="Normal":
            a.default_value = "::MyGUI::MenuItemType::Normal"
    main_ns.member_function('::MyGUI::Progress::setProgressStartPoint').arguments[0].default_value="::MyGUI::Align::Left"
    
    excludes = [#'::MyGUI::Gui::injectKeyRelease', 
                #'::MyGUI::Gui::injectKeyPress',
                '::MyGUI::InputManager::injectKeyPress',
                '::MyGUI::InputManager::injectKeyRelease',
                '::MyGUI::ItemBox::notifyKeyButtonPressed',
                '::MyGUI::ItemBox::notifyKeyButtonReleased'
                ,'::MyGUI::Message::createMessageBox'  ## bad enum conversion
#                 ,'::MyGUI::MenuCtrl::insertItemAt'
#                 ,'::MyGUI::MenuCtrl::insertItem'
                #,'::MyGUI::Edit::getTextSelect' # missing
                ,'::MyGUI::ScrollView::notifyMousePressed'
                ,'::MyGUI::ScrollView::notifyMouseReleased'
				,'::MyGUI::Any::getType'
                ]
				
    for e in excludes:
      main_ns.member_functions(e).exclude()
	  
	# excluding manualy methods with KeyCode parameters
	# thi one function doesn't compile
    main_ns.member_functions('::MyGUI::Gui::injectKeyPress', arg_types=['::MyGUI::KeyCode', None]).exclude()
    main_ns.member_functions('::MyGUI::Gui::injectKeyRelease', arg_types=['::MyGUI::KeyCode']).exclude()
                  
#     sys.exit()  
                         
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
        
# #     # Functions that have void pointers in their argument list need to change to unsigned int's  
# #     pointee_types=[]
# #     ignore_names=[]
# #     common_utils.Fix_Void_Ptr_Args  ( main_ns ) # , pointee_types, ignore_names )

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

def filter_declarations( mb ):	
    """ filter class declarations
    """
    global_ns = mb.global_ns
    global_ns.exclude()
    #global_ns.namespace('std').class_('pair<float, float>').include()
    
    MyGUI_ns = global_ns.namespace( 'MyGUI' )
    MyGUI_ns.include()
    
    global_ns.namespace( 'Ogre' ).class_('SharedPtr<Ogre::Resource>').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('HardwarePixelBufferSharedPtr').include(already_exposed=True)
    
    global_ns.namespace( 'OIS' ).class_('KeyEvent').include(already_exposed=True)
    global_ns.namespace( 'OIS' ).class_('MouseEvent').include(already_exposed=True)	
	
    # Exclude protected and private that are not pure virtual
    query = declarations.access_type_matcher_t( 'private' ) \
            & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
    MyGUI_ns.calldefs( query, allow_empty=True ).exclude()
    #global_ns.namespace( 'OIS' ).class_('MouseButtonID').include(already_exposed=True)	

def set_call_policies( mb ):
    MyGUI_ns = mb.global_ns.namespace ('MyGUI')

    # Set the default policy to deal with pointer/reference return types to reference_existing object
    # as this is the MyGUI Default.
    mem_funs = MyGUI_ns.calldefs ()
    mem_funs.create_with_signature = True #Generated code will not compile on
    #MSVC 7.1 if function has throw modifier.
    for mem_fun in mem_funs:
        if mem_fun.call_policies:
            continue
        if declarations.is_pointer (mem_fun.return_type) or declarations.is_reference (mem_fun.return_type):
            mem_fun.call_policies = call_policies.return_value_policy(
                call_policies.reference_existing_object )
 
             
    ##OK so the MyGUI String class is ugly (from a python perspective) 
    ##I'm going to fix the pointer arguments so you can use ctypes to use the class
    ## however real work is in hand_made_wrappers with a new 'assign' function
    # for fun in MyGUI_ns.class_('String').member_functions():
        # arg_position = 0
        # for arg in fun.arguments:
            # if declarations.type_traits.is_pointer(arg.type): # 
                # fun.add_transformation( ft.modify_type(arg_position,common_utils._ReturnUnsignedInt ), alias=fun.name )
                # fun.documentation = common_utils.docit ("Modified Input Argument to work with CTypes",
                                            # "Argument "+arg.name+ "(pos:" + str(arg_position)\
                                            # +") takes a CTypes.adddressof(xx)", "...")
                # break
            # arg_position +=1
        
    # for fun in MyGUI_ns.class_('String').constructors():
        # arg_position = 0
        # for arg in fun.arguments:
            # if declarations.type_traits.is_pointer(arg.type):
                # fun.add_transformation( ft.modify_type(arg_position,common_utils._ReturnUnsignedInt ), alias=fun.name )
                # fun.documentation = common_utils.docit ("Modified Input Argument to work with CTypes",
                                            # "Argument "+arg.name+ "(pos:" + str(arg_position)\
                                            # +") takes a CTypes.adddressof(xx)", "...")
                # break
            # arg_position +=1
        
                
def configure_exception(mb):
    #We don't exclude  Exception, because it contains functionality, that could
    #be useful to user. But, we will provide automatic exception translator
    Exception = mb.namespace( 'CEGUI' ).class_( 'Exception' )
    Exception.translate_exception_to_string( 'PyExc_RuntimeError',  'exc.getMessage().c_str()' )


# def change_cls_alias( ns ):
    # for cls in ns.classes():
        # if 1 < len( ns.classes( cls.name ) ):
            # print "Length of classes is", len( ns.classes( cls.name ) ), " for ", cls.name
            # for c in ns.classes( cls.name ):
               # print "* ", c.decl_string
            # print cls.decl_string
            # alias = cls.decl_string[ len('::CEGUI::'): ]
            # print "Adjust Alias for:",cls.decl_string, " with ", alias.replace ('::','')
            # cls.alias = alias.replace( '::', '' )
            # cls.wrapper_alias = cls.alias + 'Wrapper' # or 'Wrapper' ??
            #cls.exclude()



	
#
# the 'main'function
#            
def generate_code():  
#    messages.disable( 
#           Warnings 1020 - 1031 are all about why Py++ generates wrapper for class X
#          messages.W1020
#        , messages.W1021
#        , messages.W1022
#        , messages.W1023
#        , messages.W1024
#        , messages.W1025
#        , messages.W1026
#        , messages.W1027
#        , messages.W1028
#        , messages.W1029
#        , messages.W1030
#        , messages.W1031
#        , messages.W1035
#        , messages.W1040 
#        , messages.W1038        
#        , messages.W1041
#        , messages.W1036 # pointer to Python immutable member
#        , messages.W1033 # unnamed variables
#        , messages.W1018 # expose unnamed classes
#        , messages.W1049 # returns reference to local variable
#        , messages.W1014 # unsupported '=' operator
#         )
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    print os.path.join( environment.mygui.root_dir, "python_mygui.h" )
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.mygui.root_dir, "python_mygui.h" )
                        , environment.mygui.cache_file )						
	

    defined_symbols = ['OGRE_NONCLIENT_BUILD', 'OGRE_GCC_VISIBILITY', #'BOOST_PYTHON_NO_PY_SIGNATURES', 
						'__PYTHONOGRE_BUILD_CODE', 'MYGUI_NONCLIENT_BUILD', 'MYGUI_DONT_USE_OBSOLETE'] #MYGUI_NO_OIS
    
    defined_symbols.append( 'VERSION_' + environment.mygui.version )  
    
    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        if environment.isWindows():
            defined_symbols.append('BOOST_HAS_WINTHREADS')

    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.mygui.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.mygui.cflags
                                           )
										   
    filter_declarations(mb)
    # if this module depends on another set it here                                           
    mb.constructors().allow_implicit_conversion = False
    mb.register_module_dependency ( environment.ogre.generated_dir )
    print environment.ogre.generated_dir
    
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

    common_utils.Auto_Functional_Transformation ( main_ns, special_vars=['::Ogre::Real &','::Ogre::ushort &','size_t &']  )

    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    common_utils.Set_DefaultCall_Policies ( mb.global_ns.namespace ( MAIN_NAMESPACE ) )
    
    #global_ns.namespace( 'MyGUI' ).class_('Widget').include(already_exposed=True)
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    set_call_policies (mb)
	
    NoPropClasses = [""]
    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            
    ## add additional version information to the module to help identify it correctly 
    common_utils.addDetailVersion ( mb, environment, environment.mygui )

    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor() # I'm excluding the UTFstring docs as lots about nothing 
	
	
    mb.build_code_creator (module_name='_mygui_' , doc_extractor= extractor )
    
    for inc in environment.mygui.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.mygui.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.mygui.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.mygui.version ) )

    mb.split_module(environment.mygui.generated_dir, huge_classes, use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generated directory..
    additional_files= os.listdir(environment.Config.PATH_INCLUDE_mygui)
    additional_files = []
    for f in additional_files:
        if f.endswith('cpp') or f.endswith('.h'):
            sourcefile = os.path.join(environment.Config.PATH_INCLUDE_mygui, f)
            destfile = os.path.join(environment.mygui.generated_dir, f ) 
        
            if not common_utils.samefile( sourcefile ,destfile ):
                shutil.copy( sourcefile, environment.mygui.generated_dir )
                print "Updated ", f, "as it was missing or out of date"
				
    # copying event callback header file
    print "Copying 'python_mygui_callback.h'"
    shutil.copy2(os.path.join( environment.mygui.root_dir, "python_mygui_callback.h" ), os.path.join(environment.mygui.generated_dir, "python_mygui_callback.h"))
    
#     common_utils.copyTree ( sourcePath = environment.Config.PATH_INCLUDE_mygui, 
#                             destPath = environment.mygui.generated_dir, 
#                             recursive=False )
        
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
