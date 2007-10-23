#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------

#  TO EXCLUDE
# Menu:
#   bool evtHndlr_listItemCreated(const EventArgs& e);
# 	bool evtHndlr_listItemMouseEnters(const EventArgs& e);
# 	bool evtHndlr_listItemMouseLeaves(const EventArgs& e);
# Widget:
#     Ogre::Vector2 convertPixelToRelativePoint(const Ogre::Vector2& point);
# 				
# 		
		
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

MAIN_NAMESPACE = 'QuickGUI'
quickgui="xxx"  ##do a search and replace on quickgui

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
    
 
    NotExported=[]
    for c in NotExported:
        main_ns.class_( c ).exclude()
                        
    
    NonExistant = []
    for cls in NonExistant:
        try:
            main_ns.class_(cls[0]).member_function(cls[1]).exclude()
        except  declarations.matcher.declaration_not_found_t, e:
            pass 
    excludes=[\
            '::QuickGUI::QuadContainer::_populateRenderObjectList'
            ,'::QuickGUI::QuadContainer::_updateRenderQueue'
            ,'::QuickGUI::HorizontalTrackBar::_getButtonSize'
#             ,'::QuickGUI::List::getNumberOfListItems'
            ,'::QuickGUI::Panel::getScrollPane'
            ,'::QuickGUI::ScrollPane::_showVScrollBars'
            ,'::QuickGUI::ScrollPane::_showHScrollBars'
            ,'::QuickGUI::ScrollPane::getVerticalButtonLayout'
            ,'::QuickGUI::ScrollPane::getHorizontalButtonLayout'
            ,'::QuickGUI::VerticalScrollBar::getScrollButtonSize'
            ,'::QuickGUI::Effect::linearInterpolate'
                ]
    for e in excludes:
        print "excluding function", e
        global_ns.member_functions(e).exclude()
                      
# 				

############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################
    
def ManualInclude ( mb ):
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    
    ## now expose but don't create class that exist in other modules
    global_ns.namespace( 'Ogre' ).class_('OverlayContainer').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('Vector2').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('Vector3').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('Vector4').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('ColourValue').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('RenderTexture').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('RenderOperation').include(already_exposed=True)
    global_ns.namespace( 'Ogre' ).class_('UTFString').include(already_exposed=True)
    global_ns.class_('::Ogre::FontPtr').include(already_exposed=True)
    global_ns.class_('::Ogre::RenderQueueListener').include(already_exposed=True)
    global_ns.class_('::Ogre::SceneManager').include(already_exposed=True)
    global_ns.class_('::Ogre::Viewport').include(already_exposed=True)
    global_ns.class_('::Ogre::Image').include(already_exposed=True)
#     global_ns.class_('::Ogre::FloatRect').include(already_exposed=True)
    
    
    
    
############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixes ( mb ):    

    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )

              
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
        
#     x =global_ns.mem_fun('::QuickGUI::GUIManager::renderQueueEnded')
#     x.add_transformation(ft.inout('repeatThisQueue'))
#     x.documentation = docit ("","queueGroupId, invocation", "tuple - repeatThisInvocation")
#     
#     x = global_ns.mem_fun('::QuickGUI::GUIManager::renderQueueStarted') 
#     x.add_transformation(ft.inout('skipThisQueue'))
#     x.documentation = docit ("","queueGroupId, invocation", "tuple - skipThisInvocation")        
    
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
    query = declarations.access_type_matcher_t( 'private' ) \
            & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
    main_ns.calldefs( query ).exclude()
    
    #Virtual functions that return reference could not be overriden from Python
    query = declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.VIRTUAL ) \
            & declarations.custom_matcher_t( lambda decl: declarations.is_reference( decl.return_type ) )
    try:
        main_ns.calldefs( query ).virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
        pass
               
def AutoInclude( mb ):
    pass

    
def AutoFixes ( mb ): 
    """ now we fix a range of things automatically - typically by going through 
    the entire name space trying to guess stuff and fix it:)
    """       
    global_ns = mb.global_ns
    main_ns = global_ns.namespace( MAIN_NAMESPACE )
    
    # arguments passed as refs but not const are not liked by boost
    #Fix_Ref_Not_Const ( main_ns )
    
    # Functions that have void pointers in their argument list need to change to unsigned int's  
    Fix_Void_Ptr_Args  ( main_ns )
    
    # and change functions that return a variety of pointers to instead return unsigned int's
    Fix_Pointer_Returns ( main_ns )   

    # functions that need to have implicit conversions turned off
    Fix_Implicit_Conversions ( main_ns)
    
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
        
        
def Fix_Implicit_Conversions ( mb ):
    """By default we disable explicit conversion, however sometimes it makes sense
    """
    ImplicitClasses=[] 
    for className in ImplicitClasses:
        mb.class_(className).constructors().allow_implicit_conversion = True
                    
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
    try:
        for fun in mb.member_operators():
            if declarations.is_pointer (fun.return_type) and not fun.documentation:
                for i in pointee_types:
                    if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                        print "Excluding (operator):", fun
                        fun.exclude()
    except:
        pass


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
# # #         , messages.W1035
# # #         , messages.W1040 
# # #         , messages.W1038        
# # #         , messages.W1041
# # #         , messages.W1036 # pointer to Python immutable member
# # #         , messages.W1033 # unnamed variables
# # #         , messages.W1018 # expose unnamed classes
# # #         , messages.W1049 # returns reference to local variable
# # #         , messages.W1014 # unsupported '=' operator
         )
    #
    # Use GCCXML to create the controlling XML file.
    # If the cache file (../cache/*.xml) doesn't exist it gets created, otherwise it just gets loaded
    # NOTE: If you update the source library code you need to manually delete the cache .XML file   
    #
    xml_cached_fc = parser.create_cached_source_fc(
                        os.path.join( environment.quickgui.root_dir, "python_quickgui.h" )
                        , environment.quickgui.cache_file )

    defined_symbols = [ 'OGRE_NONCLIENT_BUILD',
                    'WIN32', 'NDEBUG', 'WINDOWS' ]  ## , 'QUICKGUI_EXPORTS'
    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        defined_symbols.append('BOOST_HAS_WINTHREADS')

    defined_symbols.append( 'VERSION_' + environment.quickgui.version )  
    
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.quickgui.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.quickgui.cflags
                                           )
    # NOTE THE CHANGE HERE                                           
    mb.constructors().allow_implicit_conversion = False   
                                            
    mb.register_module_dependency ( environment.ogre.generated_dir )

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
    ManualTransformations ( mb )
    
    AutoFixes ( mb )
    ManualFixes ( mb )
    
    mb.global_ns.namespace ('Ogre').vars( 'ms_Singleton' ).disable_warnings( messages.W1035 ) #singleton pointers coming up we need to ignore
                
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    Set_Call_Policies ( mb.global_ns.namespace (MAIN_NAMESPACE) )
    Set_Call_Policies ( mb.global_ns.namespace ('Ogre') )  ## need this as we are forcing singleton classes 
    
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    NoPropClasses = [""]
    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            
    common_utils.add_constants( mb, { 'quickgui_version' :  '"%s"' % environment.quickgui.version.replace("\n", "\\\n") 
                                      , 'python_version' : '"%s"' % sys.version.replace("\n", "\\\n" ) } )
                                      
    ## need to create a welcome doc string for this...                                  
    common_utils.add_constants( mb, { '__doc__' :  '"quickgui DESCRIPTION"' } ) 
    
    
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    #extractor = exdoc.doc_extractor("::Ogre") # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_quickgui_' ) #, doc_extractor= extractor )
    
    for inc in environment.quickgui.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.quickgui.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.quickgui.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.quickgui.version ) )

    mb.split_module(environment.quickgui.generated_dir, huge_classes,use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generated directory..
    common_utils.copyTree ( sourcePath = environment.Config.PATH_INCLUDE_quickgui, 
                            destPath = environment.quickgui.generated_dir, 
                            recursive=False )
                            
if __name__ == '__main__':
    start_time = time.clock()
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
