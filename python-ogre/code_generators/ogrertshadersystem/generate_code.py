#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre and is covered by the LGPL
# For the latest info, see http://python-ogre.org/
#
# -----------------------------------------------------------------------------

## STARTER TEMPLATE..
## replace ogrertshadersystem with lowercase project name
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


import pygccxml
from pygccxml import parser
from pygccxml import declarations
from pyplusplus import messages
from pyplusplus import module_builder
from pyplusplus import decl_wrappers
from pyplusplus.code_creators import include

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

# override standard pretty name function
from pyplusplus.decl_wrappers import algorithm
algorithm.create_valid_name = common_utils.PO_create_valid_name 


MAIN_NAMESPACE = '::Ogre::RTShader'

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
    ex =    ['::Ogre::RTShader::SGMaterialSerializerListener::createSGPassList',
            # issue with cannot access protected class declared in class 'Ogre::RTShader::ProgramProcessor'
            '::Ogre::RTShader::ProgramProcessor::generateLocalSplitParameters',
            '::Ogre::RTShader::ProgramProcessor::mergeParameters',
            '::Ogre::RTShader::ProgramProcessor::mergeParametersByPredefinedCombinations',
            '::Ogre::RTShader::ProgramProcessor::mergeParametersReminders',
            '::Ogre::RTShader::ProgramProcessor::rebuildFunctionInvocations',
            '::Ogre::RTShader::ProgramProcessor::rebuildParameterList',
            '::Ogre::RTShader::ProgramProcessor::replaceParametersReferences'
            ]
    for c in ex:
        try:
            main_ns.member_function(c).exclude()
        except:
            print "failed to exclude:", c
    ex = []
    for c in ex:
        try:
            main_ns.member_variable(c).exclude()
        except:
            print "failed to exclude:", c
    ex=["Ogre::RTShader::ShaderGenerator::SGPass","Ogre::RTShader::ProgramProcessor"]
    for c in ex:
        try:
            main_ns.class_(c).exclude()
        except:
            print "failed to exclude:", c
 
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
    #for c in main_ns.classes():
    #    if c.name.startswith ('Page') or c.name.startswith ('SimplePage'):
    #        print "Including :", c
    #        c.include()
    inc = []
    for c in inc:
        main_ns.class_(c).include()
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
    func = main_ns.member_function("::Ogre::RTShader::FunctionInvocation::pushOperand")      
    func.arguments[2].default_value = "(int) ::Ogre::RTShader::Operand::OPM_ALL"
    for c in global_ns.namespace('std').classes():
        print c
        if 'vector<Ogre::RTShader::ProgramProcessor::MergeParameter,Ogre::STLAllocator<Ogre::RTShader::' in c.decl_string:
            print "FOUND!!"
            c.alias = "stdVectoRTShaderProgramProcessorMergeParameter"
    #c = main_ns.class_('std::vector<Ogre::RTShader::ProgramProcessor::MergeParameter,Ogre::STLAllocator<Ogre::RTShader::ProgramProcessor::MergeParameter, Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL> > >')
    #c.alias = "stdVectoRTShaderProgramProcessorMergeParameter"
#     sys.exit(-1)
    for c in main_ns.classes():
        if "SGPass" in c.name:
            print "Excluding:", c
            c.exclude()
    
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
        
    # Functions that have void pointers in their argument list need to change to unsigned int's  
    pointee_types=[]
    ignore_names=[]

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
                        os.path.join( environment.ogrertshadersystem.root_dir, "python_ogrertshadersystem.h" )
                        , environment.ogrertshadersystem.cache_file )

    defined_symbols = environment.defined_symbols 
    defined_symbols .append('OGRE_RTSHADERSYSTEM_EXPORTS')
    defined_symbols.append( 'VERSION_' + environment.ogrertshadersystem.version )  
    
    #
    # build the core Py++ system from the GCCXML created source
    #    
    mb = module_builder.module_builder_t( [ xml_cached_fc ]
                                          , gccxml_path=environment.gccxml_bin
                                          , working_directory=environment.root_dir
                                          , include_paths=environment.ogrertshadersystem.include_dirs
                                          , define_symbols=defined_symbols
                                          , indexing_suite_version=2
                                          , cflags=environment.ogrertshadersystem.cflags
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
    common_utils.AutoInclude ( mb, MAIN_NAMESPACE )
    ManualInclude ( mb )
    ManualExclude ( mb )
    # here we fixup functions that expect to modifiy their 'passed' variables    
    ManualTransformations ( mb )
    AutoFixes ( mb, MAIN_NAMESPACE )
    ManualFixes ( mb )
    
    common_utils.Auto_Functional_Transformation ( main_ns ) #, special_vars=[]  )
   
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
            
    # THIS MUST BE AFTER Auto_Functional_Transformation
    common_utils.Auto_Document( mb, MAIN_NAMESPACE )
    
    ## add additional version information to the module to help identify it correctly 
    common_utils.addDetailVersion ( mb, environment, environment.ogrertshadersystem )

    ##########################################################################################
    #
    # Creating the code. After this step you should not modify/customize declarations.
    #
    ##########################################################################################
    extractor = exdoc.doc_extractor() # I'm excluding the UTFstring docs as lots about nothing 
    mb.build_code_creator (module_name='_ogrertshadersystem_' , doc_extractor= extractor )
    
    for inc in environment.ogrertshadersystem.include_dirs:
        mb.code_creator.user_defined_directories.append(inc )
    mb.code_creator.user_defined_directories.append( environment.ogrertshadersystem.generated_dir )
    mb.code_creator.replace_included_headers( customization_data.header_files( environment.ogrertshadersystem.version ) )

    huge_classes = map( mb.class_, customization_data.huge_classes( environment.ogrertshadersystem.version ) )

    mb.split_module(environment.ogrertshadersystem.generated_dir, huge_classes, use_files_sum_repository=False)

    ## now we need to ensure a series of headers and additional source files are
    ## copied to the generated directory..
    
#     common_utils.copyTree ( sourcePath = environment.Config.PATH_INCLUDE_ogrertshadersystem, 
#                             destPath = environment.ogrertshadersystem.generated_dir, 
#                             recursive=False )
        
    if environment.ogre.version.startswith("1.7"):
        ## have a code generation issue that needs resolving...
        filesToFix=['CGProgramProcessor.pypp.cpp', 'GLSLProgramProcessor.pypp.cpp','HLSLProgramProcessor.pypp.cpp',
                    'ProgramProcessor.pypp.cpp', 'stdMapRTShaderParameterOperand.pypp.cpp', 
                    'stdMapRTShaderParameterParameter.pypp.cpp',
                    'CGProgramProcessor.pypp.cpp']
        for filename in filesToFix:
            fname = os.path.join( environment.ogrertshadersystem.generated_dir, filename)
            try:
                f = open(fname, 'r')
                buf = f.read()
                f.close()
                print "CHECKING:", fname
                if (" MEMCATEGORY_GENERAL" in buf) or ("<MEMCATEGORY_GENERAL" in buf):
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
