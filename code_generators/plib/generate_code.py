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
from pyplusplus.module_creator import sort_algorithms

import common_utils.extract_documentation as exdoc
import common_utils.var_checker as varchecker
import common_utils.ogre_properties as ogre_properties

MAIN_NAMESPACE = ''

## small helper function
def docit ( general, i, o ): 
    docs = "Python-Ogre Modified Function Call\\n" + general +"\\n"
    docs = docs + "Input: " + i + "\\n"
    docs = docs + "Output: " + o + "\\n\\\n"
    return '"'+docs+'"'

############################################################
##
##  Here is where we manually exclude stuff
##
############################################################

def ManualExclude ( mb ):
    global_ns = mb.global_ns
    main_ns = global_ns
 
    # things not yet implemented in the C source..
    excludes=['print', 'getName'] # don't like print functions at all
    for c in main_ns.classes():
        for m in c.member_functions(allow_empty=True):
            for e in excludes:
                if e == m.name:
                    print "Excluding:", c, m
                    m.exclude()
    
    ### Member Functions
    excludes=[  #'::slScheduler::waveOutClose'
#             ,'::slScheduler::waveOutWrite'
#             ,'::slScheduler::waveOutReset'
#             ,'::slScheduler::waveOutGetPosition'
#             ,'::slScheduler::waveOutPrepareHeader' 
            '::netMessage::print' # strange / ugly defination
            ,'::slSample::getBuffer' # returns char pointer -- needs wrapping
            ,'::slSample::getComment' # same..
            # have callbacks and need to be hand wrapped
            ,'::slScheduler::loopSample'
            ,'::slScheduler::loopMusic'
            ,'::slScheduler::playSample'
            ,'::slScheduler::playMusic'
            # handwrapped to handle python lists instead of c buffers
            ,'::netSocket::send'
            ,'::netSocket::sendto'
            ,'::netSocket::recv'
            ,'::netSocket::recvfrom'
            ,'::netBuffer::getData'  ## returns a char pointer to the data which needs wrapping
            ,'::netChannel::send'
            ,'::netChannel::recv'
            ,'::netMonitorServer::setCommandFunc'   ## needs to handle callbacks
            
            ,'::ssgIndexArray::get'
            ,'::ssgSimpleList::raw_get'
            ,'::ssgSimpleState::getTextureFilename'  # char pointers
            ,'::ssgStateSelector::getTextureFilename'
            ,'::ssgTexture::getFilename'
            ,'::ssgVtxArray::getIndex'  ## returns a short
            ,'::ssgLoaderOptions::make_path'
            ,'::ssgBase::copy_from'
            
            
           
           ## ,'::ssgBase::getName'
         ]
    for e in excludes:
        print "excluding ", e
        main_ns.member_functions(e).exclude()
        
    ### Free Functions
    excludes = ['::ssgShowStats'
                ,'::netCopyName'
                ,'::ulGetCWD'
                ,'::ulGetError'
                ,'::ulMakePath'
                ,'::ulOpenDir'
                ,'::ulStrDup'
                ,'::ssgSavePOV'
                ,'::ssgConvertTexture'
                ,'::netPoll'
                ]
    for e in excludes:
        main_ns.free_functions(e).exclude()
        
    ## Classes
    excludes = ['ssgBaseTransform','ssgEntityBinding','ssgHit', 'ssgSimpleState']
    for e in excludes:
        main_ns.class_(e).exclude()
    
        
    ## I have a challenge that Py++ doesn't recognise these classes by full name (perhaps because they are structs?)
    ## so I have to look through and match on a class by class basis
    excludeName = []
    for c in main_ns.classes():
        if c.name in excludeName:
            c.exclude()
      
    ### Variables        
    excludes = []
    for e in excludes:
        main_ns.variable(e).exclude()
        
    ### Typedefs    
    excludes = []
    for e in excludes:
        main_ns.typedefs(e).exclude()
        
    ### Operators        
    excludes=[]
    for e in excludes:
        main_ns.operators(e).exclude()
        
#     ### Constructors
#     for c in main_ns.class_('::plib::Pose').constructors():  ## these hide the working constructors
#         for a in c.arguments:
#             if 'NxVec3' in a.type.decl_string or 'NxQuat' in a.type.decl_string:
#                 c.exclude()
#                 break
                

############################################################
##
##  And there are things that manually need to be INCLUDED 
##
############################################################
    
def ManualInclude ( mb ):
    global_ns = mb.global_ns
    main_ns = global_ns
      
    includes=['netInit']
    ### Free Functions
    for e in includes:
        main_ns.free_functions(e).include()
        
############################################################
##
##  And things that need manual fixes, but not necessarly hand wrapped
##
############################################################
def ManualFixes ( mb ):    

    global_ns = mb.global_ns
    
                
############################################################
##
##  And things that need to have their argument and call values fixed.
##  ie functions that pass pointers in the argument list and of course we need
##  to read the updated values - so instead we pass them back 
##  as new values in a tuple (ETC ETC)
##
############################################################
        
def ManualTransformations ( mb ):
    def _ReturnUnsignedInt( type_ ):
        """helper to return an UnsignedInt call for tranformation functions
        """
        return declarations.cpptypes.unsigned_int_t()

    global_ns = mb.global_ns
    main_ns = global_ns
        
    def create_output( size ):
        return [ ft.output( i ) for i in range( size ) ]
    
    ## hack to get things working -- needs to be wrapped to handle a python call back        
    change = ['loopSample', 'loopMusic','playSample','playMusic']
    for fun in change:
        f = global_ns.class_('::slScheduler').mem_fun(fun)
        f.add_transformation( ft.modify_type('cb',_ReturnUnsignedInt ), alias=fun )
        print "Changed ", f
   
# #     for fun in global_ns.member_functions():
# #         
# #         arg_position = 0
# #         trans=[]
# #         desc=""
# #         for arg in fun.arguments:
# #             if arg.type.decl_string == 'float[4] *':
# #                 f=declarations.cpptypes.float_t()
# #                 fp = declarations.cpptypes.pointer_t( f )
# # # #                 arg.type = fp
# #                 trans.append(ft.input_static_array (arg_position,16 ) )
# #                 desc = desc + " Transform " + arg.name + " (position:" + str(arg_position) +") as an input array of 16 floats"
# #             arg_position +=1
# #         if trans:
# #             print "Tranformation applied to ", fun, desc
# #             fun.add_transformation ( * trans )
# #             fun.documentation = '\"Python-Ogre: Function Modified\\n\\\n' + desc  +'\"'     
# #             fun.include()
# #             fun.exportable = True
# # #     f = global_ns.member_function('::ssgEntity::cull')
# # #     f.include()
# # #     f.add_transformation (ft.input_static_array('m',16) )
        
       
###############################################################################
##
##  Now for the AUTOMATIC stuff that should just work
##
###############################################################################
    
    
def AutoExclude( mb ):
    """ Automaticaly exclude a range of things that don't convert well from C++ to Python
    """
    global_ns = mb.global_ns
    main_ns = global_ns
    
    # vars that are static consts but have their values set in the header file are bad
    Remove_Static_Consts ( main_ns )
    
    ## Exclude protected and private that are not pure virtual
    query = declarations.access_type_matcher_t( 'private' ) \
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
    
    f=main_ns.free_function("ssgLoadAC")
    print "\n\n",f,"\n\n"    
#     f.include()
    print f.exportable, f.ignore
    print "\n\n"
                   
def AutoInclude( mb ):
    main_ns = mb.global_ns  ##   doesn't have it's own namespace..
    validPrefix= ['sl','sm','net','ul', 'ssg']
        
    for cls in main_ns.classes():
        for p in validPrefix:
            if cls.name.startswith ( p ) and cls.name[len(p)].isupper():
                print "Including class: ", cls.name
                cls.include()
            
    for cls in main_ns.enums():
        for p in validPrefix:
            if cls.name.startswith ( p ): # and cls.name[len(p)].isupper():
                print "Including enum: ", cls.name
                cls.include()
            elif cls.name.startswith (p.upper()):
                print "Including enum: ", cls.name
                cls.include()
            

    for cls in main_ns.free_functions():
        for p in validPrefix:
            if cls.name.startswith ( p ) and cls.name[len(p)].isupper():
                print "Including free function: ", cls.name
                cls.include()

    
def AutoFixes ( mb ): 
    """ now we fix a range of things automatically - typically by going through 
    the entire name space trying to guess stuff and fix it:)
    """       
    global_ns = mb.global_ns
    main_ns = global_ns
    
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
    pass
                    
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
            if declarations.type_traits.is_void_pointer(arg.type) or arg.type.decl_string == "void const *":
                fun.add_transformation( ft.modify_type(arg_position,_ReturnUnsignedInt ), alias=fun.name )
                fun.documentation = docit ("Modified Input Argument to work with CTypes",
                                            "Argument "+arg.name+ "(pos:" + str(arg_position)\
                                            +") takes a CTypes.adddressof(xx)", "...")
                print "Fixed Void Ptr", fun, arg_position
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
    for fun in mb.member_operators(allow_empty=True):
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
#         , messages.W1040 
#         , messages.W1038 
#         , messages.W1039       
#         , messages.W1041
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
                        os.path.join( environment.plib.root_dir, "python_plib.h" )
                        , environment.plib.cache_file )

    defined_symbols = [ ]
    if environment._USE_THREADS:
        defined_symbols.append('BOOST_HAS_THREADS')
        defined_symbols.append('BOOST_HAS_WINTHREADS')
    defined_symbols.append( 'VERSION_' + environment.plib.version )  
    
    undefined_symbols = []
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.plib.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogre.cflags
                                           )
    # NOTE THE CHANGE HERE                                           
    mb.constructors().allow_implicit_conversion = False                                           
 
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
        
    #
    # We filter (both include and exclude) specific classes and functions that we want to wrap
    # 
    global_ns = mb.global_ns
    global_ns.exclude()
    main_ns = global_ns
# #     main_ns.include()
    
   
    AutoInclude ( mb )
    AutoExclude ( mb )
    ManualExclude ( mb )
    ManualInclude ( mb )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    
    AutoFixes ( mb )
    ManualFixes ( mb )
            
    #
    # We need to tell boost how to handle calling (and returning from) certain functions
    #
    Set_Call_Policies ( mb.global_ns )
    
    #
    # the manual stuff all done here !!!
    #
    hand_made_wrappers.apply( mb )

    NoPropClasses = [""]
    for cls in main_ns.classes():
        if cls.name not in NoPropClasses:
            cls.add_properties( recognizer=ogre_properties.ogre_property_recognizer_t() )
            
    common_utils.add_constants( mb, { 'PROJECT_version' :  '"%s"' % environment.plib.version.replace("\n", "\\\n") 
                                      , 'python_version' : '"%s"' % sys.version.replace("\n", "\\\n" ) } )
                                      
    ## need to create a welcome doc string for this...                                  
    common_utils.add_constants( mb, { '__doc__' :  '"plib PROJECT DESCRIPTION"' } ) 
    
    f = global_ns.member_function('::ssgEntity::cull')
    f.include()
    
    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor("") # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_plib_' ) ## no docs as issues here, doc_extractor= extractor )
    
    for inc in environment.plib.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.plib.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.plib.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.plib.version ) )

    mb.split_module(environment.plib.generated_dir, huge_classes,use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generaated directory..
    additional_files = []
    paths = [environment.Config.PATH_plib
            ]

    for p in paths:
        additional_files = os.listdir(p)
        for f in additional_files:
            if f.endswith('cxx') or f.endswith('.h'):
                sourcefile = os.path.join(p, f)
                destfile = os.path.join(environment.plib.generated_dir, f ) 
        
                if not common_utils.samefile( sourcefile ,destfile ):
                    shutil.copy( sourcefile, environment.plib.generated_dir )
                    print "Updated ", f, "as it was missing or out of date"       
                    
                    
                    

if __name__ == '__main__':
    start_time = time.clock()
# #     import trace
# #     
# #     # create a Trace object, telling it what to ignore, and whether to
# #     # do tracing or line-counting or both.
# #     tracer = trace.Trace(
# #         ignoredirs=[sys.prefix, sys.exec_prefix],
# #         trace=1,
# #         count=1,
# #         countcallers=1,
# #         countfuncs=1 )
# #         #,
# #         #outfile="c:\temp\out.trace")
# #     
# #     # run the new command using the given tracer
# #     tracer.run('generate_code()')
# #     
# #     # make a report, placing output in /tmp
# #     r = tracer.results()
# #     r.write_results(show_missing=True, coverdir="\temp")
    generate_code()
    print 'Source code was updated( %f minutes ).' % (  ( time.clock() - start_time )/60 )
