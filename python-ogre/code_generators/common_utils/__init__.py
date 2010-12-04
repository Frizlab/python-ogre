import os, shutil
import sys
import shared_ptr
import datetime
import subprocess
##import hashlib ## makes this 2.5 dependent
import md5
import logging

from pygccxml import declarations
from pyplusplus.decl_wrappers import property_t

from pyplusplus import function_transformers as ft
from pyplusplus.function_transformers import transformers as transformers

from pyplusplus.module_builder import call_policies


import time
import var_checker as varchecker
import ogre_properties as ogre_properties

import PO_FuncTransform

def input_c_string( *args, **keywd ):
    def creator( function ):
        return PO_FuncTransform.input_c_string_t( function, *args, **keywd )
    return creator
    
ft.input_c_string = input_c_string
_logger = None

def setup_logging (logfilename):
    global _logger
    # set up logging to file
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%m-%d %H:%M',
                        filename=logfilename,
                        filemode='w')
    # define a Handler which writes INFO messages or higher to the sys.stderr
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    # set a format which is simpler for console use
    formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
    # tell the handler to use this format
    console.setFormatter(formatter)
    # add the handler to the root logger
    logging.getLogger('').addHandler(console)
    _logger = logging.getLogger('PythonOgre')

def log_error (msg):
    if _logger:
        _logger.error (msg)
        
def log_warning  (msg):
    if _logger:
        _logger.warn(msg)
        
def log_info (msg):
    if _logger:
        _logger.info(msg)

def log_exclude (msg, ok=True, extra=None):
    """ helper specifically for excluding stuff (classes, memeber functions...)
    """
    if ok:
        _msg =  "Excluded: " + str(msg)
        if extra:
            _msg += ' (' + extra + ')'
        log_info( _msg )
    else:
        _msg =  "Unable to excluded: " + str(msg)
        if extra:
            _msg += ' (' + extra + ')'
        log_info( _msg )
        
def log_include (msg, ok=True, extra=None):
    if ok:
        _msg =  "Included: " + str(msg)
        if extra:
            _msg += ' (' + extra + ')'
        log_info( _msg )
    else:
        _msg =  "Unable to include: " + str(msg)
        if extra:
            _msg += ' (' + extra + ')'
        log_info( _msg )

def configure_shared_ptr( mb ):
    exposer = shared_ptr.exposer_t( mb )
    exposer.expose()

def docit ( general, i, o ): 
    """ helper function to generate a semi nice documentation string
    """
    docs = "Python-Ogre Modified Function Call\\n" + general +"\\n"
    docs = docs + "Input: " + i + "\\n"
    docs = docs + "Output: " + o + "\\n\\\n"
    return docs
        
def docInfo ( mb, module ):
    i = '"This module is part of the Python-Ogre project (www.python-ogre.org)\nIt was built on %(timestamp)s and is a member of the %(version)s release. "'
    timestamp = time.ctime()
    version = environment.PythonOgreMajorVersion +"."+environment.PythonOgreMinorVersion+"."+PythonOgrePatchVersion
    t = i % {'timestamp':timestamp, 'version':version}
    add_constants( mb, { '__info__' :  t } ) 
  
def getSVNVersion ( environment ):
  """ return a string with the current SVN version number
  """
  if hasattr( environment.Config, "SVNVersionCommmand"):
      cmd = environment.Config.SVNVersionCommand
  else:
      cmd = "svnversion"        
  process = subprocess.Popen (cmd, shell=True,stdout=subprocess.PIPE, stderr=subprocess.PIPE )
  try:
      out,err = process.communicate()
      returncode = process.returncode
      svnversion = out
  except:
      returncode = -1
      svnversion = "0000"
  return svnversion.strip()
 
def addDetailVersion ( mb, env, envClass ):
    """Add detailed version and build information to the module to assist with debugging etc
    """
#     decCode = """
#         boost::python::tuple getCompileTime () { return ( boost::python::make_tuple( __TIME__ , __DATE__) ); }
#         """   
#     regCode = """ 
#         bp::def( "getCompileTime", &getCompileTime,
#                 "Python-Ogre Helper Function: Return the Time and Date the module was compiled.\\n\\
#                 Input: None\\n\\
#                 Ouput: Tuple [time, date]");
#         """                  
          
    po = ".".join( (env.PythonOgreMajorVersion,env.PythonOgreMinorVersion, env.PythonOgrePatchVersion) )
    t = datetime.datetime.now().isoformat(' ').strip()
    t = str (datetime.date.today().month) # making this less granular so it doesn't change 'every' time I do a regenerate
    v = envClass.version
    s = getSVNVersion( env )
    detail = "_".join( (po,s,envClass.name, v, t) )
    
    if hasattr( envClass, 'docstring' ):
        docstring = envClass.docstring
    else:
        docstring = "PythonOgre %s Module - Version %s" % (envClass.name, v)
        
    add_constants ( mb, { 'PythonOgreDetail__' : '"%s"' % detail.replace("\n", "\\\n" ) 
                        , 'CompileTime__' : '__TIME__' 
                        , 'CompileDate__' : '__DATE__' 
                        , 'PythonVersion__' : '"%s"' % sys.version.replace("\n", "\\\n" )
                        , 'Version__' : '"%s"' % v.replace("\n", "\\\n" )
                        , 'PythonOgreVersion__' : '"%s"' % po.replace("\n", "\\\n" )
                        , '__doc__' : '"%s"' % docstring.replace("\n", "\\\n" )
                        } )
    
#     mb.add_declaration_code( decCode )
#     mb.add_registration_code ( regCode )
           
def _ReturnUnsignedInt( type_ ):
    """helper to return an UnsignedInt call for tranformation functions
    """
    return declarations.cpptypes.unsigned_int_t()    
    
class decl_starts_with (object):
    def __init__ (self, prefix):
        self.prefix = prefix
    def __call__ (self, decl):
        return self.prefix in decl.name

# 
# an alterate to the Py++ unique name generator that makes (imho) nicer names
#
## TO USE:
# from pyplusplus.decl_wrappers import algorithm
# algorithm.create_valid_name = common_utils.PO_create_valid_name   
#
    
import re        
def PO_create_valid_name(name):
    """Create valid name\\Python identifier from a string
    As input this functions takes valid C++ name\identifier and replaces all invalid
    characters. 
    Invalid characters are introduced by a template instantiation.
    """
    __RE_VALID_IDENTIFIER = re.compile( r"[_a-z]\w*", re.I | re.L | re.U )
    match_found = __RE_VALID_IDENTIFIER.match(name)
    if match_found and ( match_found.end() - match_found.start() == len(name) ):
        return name
    invalids='>,: \t&()[]=.$'  
    upper=False
    ret = []
    for c in name:
        if c not in invalids:
            if upper:
                c = c.upper()
                upper = False
            if c == '*':
                c = 'Ptr' 
            elif c == '<':
                c = '_'                               
            ret.append(c)
        else:
            upper = True               
    retstr = ''.join(ret)                         
    return retstr
            

## Lets go looking for 'private' OGRE classes as defined by comments etc in the include files
class private_decls_t:
    ## First build a list of include file name/ line number for all instances of 'private' classes
    ## note the index + 'x' to get things to the same line as gcc..  However potential bugs in code
    ## depending upon the include file 'formating'.  We are capturing the line the comment is on, gcc
    ## records the line the opening brace '{' is on...
    def __init__( self, include_dirs ):
        self.__private = {} #fname : [line ]
        for include_dir in include_dirs:    
            for fname in os.listdir( include_dir ):
                full_name = os.path.join( include_dir, fname )
                if not os.path.isfile( full_name ):
                    continue
                fobj = file( full_name, 'r' )
                for index, line in enumerate( fobj ):
                    if '_OgrePrivate' in line:
                        if not self.__private.has_key( fname ):
                            self.__private[ fname ] = []
                        if '{' in line:     ## AJM Ugly hack - assumes there won't be blank lines between class name and opening brace
                            index = index + 1   #enumerate calcs from 0, while gccxml from 1
                        else:
                            index = index + 2   #one line down to the opening brace
                        self.__private[ fname ].append( index ) #enumerate calcs from 0, while gccxml from 1
                    line = line.strip()
    
                    ## Again this next bit assumes an opening brace on the same line as the method or class :(
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

def fix_unnamed_classes( classes, namespace ):
    for unnamed_cls in classes:        
        named_parent = unnamed_cls.parent
        if not named_parent.name:
            named_parent = named_parent.parent
        if not named_parent or named_parent.ignore:
            continue
        for mvar in unnamed_cls.public_members:
            if not mvar.name:
                continue
            if mvar.ignore:
                continue
            if type (mvar) == type (declarations.destructor_t):
                continue 
            try:               
                if declarations.is_array (mvar.type):
                    template = '''def_readonly("%(mvar)s", &%(ns)s::%(parent)s::%(mvar)s)'''
                else:
                    template = '''def_readwrite("%(mvar)s", &%(ns)s::%(parent)s::%(mvar)s)'''
            except AttributeError:
                continue
            except:
                print ("**** Error in unnamed_classes", mvar)
            print ("Fixing Unnamed Class:", unnamed_cls, mvar, named_parent.name)
            named_parent.add_code( template % dict( ns=namespace, mvar=mvar.name, parent=named_parent.name ) )

def set_declaration_aliases(global_ns, aliases):
    for name, alias in aliases.items():
        try:
            cls = global_ns.class_( name )
            cls.alias = alias
            cls.wrapper_alias = alias + '_wrapper'
        except declarations.matcher.declaration_not_found_t:
            global_ns.decl( name, decl_type=declarations.class_declaration_t ).rename( alias )


def add_constants( mb, constants ):
    tmpl = 'boost::python::scope().attr("%(name)s") = %(value)s;'
    for name, value in constants.items():
        mb.add_registration_code( tmpl % dict( name=name, value=str(value) ) )
    
def add_properties( classes ):
    for cls in classes:
        cls.add_properties()

## note that we assume all the properties have been added by ourselves and we checked case insentistively for conflicts
##                
def add_LeadingLowerProperties ( cls ):
    new_props = []
    existing_names = []
    for prop in cls.properties:
        existing_names.append( prop.name)
    for prop in cls.properties:
        name = prop.name[0].lower() + prop.name[1:]
        if name not in existing_names:  # lets make sure it's different
            existing_names.append ( name )
            new_props.append( property_t( name, prop.fget, prop.fset, prop.doc, prop.is_static ) )
    cls.properties.extend( new_props )
    
    
def remove_DuplicateProperties ( cls ):
    unique = {}
    duplicate = False
    newlist = []
    # first build a list of duplicates
    for prop in cls.properties:
        if not unique.has_key (prop.name):
            unique[prop.name] = [prop]
        else:
            unique[prop.name].append ( prop )
            duplicate = True
    if duplicate:
        for k in unique.keys():  
            if len ( unique[k] ) > 1  :
                print ("DUPLICATE FOUND: ", cls, len ( unique[k] ))
                for p in unique[k]:
#                     print "   Checking", p.name, p.fget, p.fset
                    prefered = p
                    if "::get" in prefered.fget.decl_string:
                        break   # we break out if it's a 'get' function (otherwise, last one wins)
#                 print " Using ", prefered.fget
                newlist.append ( prefered )
            else:
                newlist.append ( unique[k][0] )
        ## this is ugly - we replace the existing properties array with our new 'prefered' unique list      
        cls._properties = newlist                         
    
def extract_func_name ( name ):
    """ extract the function name from a fully qualified description
    """
    end = name.index('(')  # position of open brace, function call is before that
    start = -1
    for x in range ( end, 0, -1 ):   # step back until we find the ':'
        if name[x] == ':' :
            start = x + 1
            break
    ## so now return the function call
    return name[start:end]
        
        
def Auto_Document ( mb, namespace=None ):
    """Indicate that the functions being exposed are declated protected or private in the C++ code
    this should warn people to be careful using them :) """
    global_ns = mb.global_ns
    if namespace:
        main_ns = global_ns.namespace( namespace )
    else:
        main_ns = global_ns
    query = declarations.access_type_matcher_t( 'private' ) 
    for c in main_ns.calldefs( query, allow_empty=True ):
#         print "PRIVATE:", c
        s = c.documentation
        if not s:
            s = ""
        c.documentation="Private declaration.\\n"+s
    query = declarations.access_type_matcher_t( 'protected' ) 
    for c in main_ns.calldefs( query, allow_empty=True ):
#         print "PROTECTED:", c
        s = c.documentation
        if not s:
            s = ""
        c.documentation="Protected declaration.\\n"+s
       


def Auto_Functional_Transformation ( mb, ignore_funs=[], special_vars=[]): 
    toprocess = []   
    aliases={}
    for fun in mb.member_functions(allow_empty=True):
        toprocess.append( fun )
    for fun in mb.free_functions(allow_empty=True):
        toprocess.append( fun )
    for fun in toprocess:
        fun_demangled = fun.demangled  # need to check as extern functions don't have demangled name...
        if fun_demangled: 
#         try:   # ugly wrapping in a try :(    
            fullname = fun.demangled.split('(')[0]
            if fullname not in ignore_funs and not fun.ignore:
                outputonly = False
                arg_position = 0
                trans=[]
                desc=""
                ft_type = None
                ctypes_conversion = False
                for arg in fun.arguments:
                    rawarg =  declarations.remove_declarated(
                        declarations.remove_const( 
                            declarations.remove_reference( 
                                declarations.remove_pointer ( arg.type ))))
                                               
                    
                    ## now check if the arg is a fundemental type (int float etc), a void
                    ##  or a special ..
                    if declarations.is_arithmetic (rawarg)\
                            or declarations.is_void(rawarg)\
                            or arg.type.decl_string in special_vars:
                        if declarations.is_pointer(arg.type):   #we convert any pointers to unsigned int's
                            # now look to see if it's a char * and if so we treat it as a string..
# #                             print "**" , declarations.remove_alias( rawarg ), declarations.type_traits.create_cv_types( declarations.cpptypes.char_t())
                            if declarations.remove_alias( rawarg ) in declarations.type_traits.create_cv_types( declarations.cpptypes.char_t() ): 
                                print ("MATCHED CString", fun)
                                trans.append( ft.input_c_string(arg_position, 4096 ) )
                                desc = desc +"Argument: "+arg.name+ "( pos:" + str(arg_position) + " - " +\
                                    arg.type.decl_string + " ) takes a python string. \\n"
                                ctypes_conversion = True                                
                                ctypes_arg = arg.type.decl_string.split()[0]
                                ft_type = 'CTYPES'
                            else:
                                trans.append( ft.modify_type(arg_position,_ReturnUnsignedInt ) )
                                desc = desc +"Argument: "+arg.name+ "( pos:" + str(arg_position) + " - " +\
                                    arg.type.decl_string + " ) takes a CTypes.addressof(xx). \\n"
                                ctypes_conversion = True                                
                                ctypes_arg = arg.type.decl_string.split()[0]
                                ft_type = 'CTYPES'
                        elif declarations.is_reference(arg.type)and not declarations.is_const(declarations.remove_reference( arg.type)):  # seen functions passing const ref's 
                            trans.append( ft.inout(arg_position ) )
                            desc = desc + "Argument: "+arg.name+ "( pos:" + str(arg_position) + " - " +\
                                arg.type.decl_string + " ) converted to an input/output (change to return types).\\n"
                            ft_type = 'INOUT'                                
                        elif declarations.is_reference(arg.type):
                            print ("Warning: - possible code change.", fun,arg," not wrapped as const reference to base type invalid")
                        else:
                            pass # it isn't a pointer or reference so doesn't need wrapping
                    else:
                        pass # it's not a var we need to handle
                    arg_position += 1
                if trans:
                    const_return = False #  declarations.is_const(fun)
                    if fun.decl_string.endswith('const'):
                        const_return=True
                    simple_return = declarations.is_arithmetic(fun.return_type) or declarations.is_void(fun.return_type)
                    nonpublic_destructor = declarations.is_class(fun.parent) and declarations.has_destructor(fun.parent) and\
                            not declarations.has_public_destructor(fun.parent)
                
                    if fun.documentation or fun.transformations:   # it's already be tweaked:
                        print ("AUTOFT ERROR: Duplicate Tranforms.", fun, fun.documentation)
                        
                    # if the class has a protected destruction AND the return value is const or a non arithmatic value then exclude it.
                    elif nonpublic_destructor and const_return:
                        print ("AUTOFT ERROR Const: Parent has non public destructor and const return.", fun.parent.name, fun.return_type.decl_string, fun)
                        fun.documentation="Python-Ogre Warning: function required transformation - not possible due to non public destructor and const return value.."
                    elif nonpublic_destructor and not simple_return:
                        print ("AUTOFT ERROR Const: Parent has non public destructor and complex return value.", fun.parent.name, fun.return_type.decl_string, fun)
                        fun.documentation="Python-Ogre Warning: function required transformation - not possible due to non public destructor and complex return value.."
                    else:
                        new_alias = fun.name
                        if ctypes_conversion:   # only manage name changes if ctypes changing
                            # now lets look for a duplicate function name with the same number arguments
                            f= [None]*len(fun.arguments)
                            s = mb.member_functions("::" + fullname, arg_types=f, allow_empty=True)
                            if len (s) > 1: 
                                # there are duplicate names so need to create something unique
                                ctypes_arg = ctypes_arg.replace("::", "_") # to clean up function names...
                                new_alias = fun.name + ctypes_arg[0].upper() + ctypes_arg[1:]
                                # now for REAL ugly code -- we have faked a new alias and it may not be unique
                                # so we track previous alias + class name to ensure unique names are generated
                                keyname = fullname + new_alias # we use the full class + function name + alias as the key
                                if keyname in aliases: # already exists, need to fake another version..
                                    new_alias = new_alias + "_" + str( aliases[keyname] )
                                    aliases[keyname] = aliases[keyname] + 1
                                else:
                                    aliases[keyname] = 1   
                                desc = desc + "\\\nWARNING FUNCTION NAME CHANGE - from "+fun.name + " -- " + fun.decl_string +" to " + new_alias + " \\n"                                    
                                print ("INFO: Adjusting Alias as multiple overlapping functions:", new_alias)
                            
                        print ("AUTOFT OK: Tranformed ", fun.return_type.decl_string, fun, "(",new_alias,")")
                        fun.add_transformation ( * trans ,  **{"alias":new_alias}  )
                        fun.documentation = docit ("Auto Modified Arguments:",
                                                        desc, "...")
#         except:
#             print "Unexpected error:", sys.exc_info()[0]
#             sys.exit()

def Fix_Void_Ptr_Args ( mb, pointee_types=['unsigned int','int', 'float', 'unsigned char', 'char', 'bool'],  ignore_names=[] ):
    """ we modify functions that take void *'s in their argument list to instead take
    unsigned ints, which allows us to use CTypes buffers
    """
    raise RuntimeError( "Fix_Void_Ptr_Args is depreciated,  use Auto_Functional_Transformation instead")
    
    def fixVoids ( fun ):
        arg_position = 0
        trans=[]
        desc=""
        for arg in fun.arguments:
            if arg.type.decl_string == 'void const *' or arg.type.decl_string == 'void *':
                trans.append( ft.modify_type(arg_position,_ReturnUnsignedInt ) )
                desc = desc +"Argument: "+arg.name+ "( pos:" + str(arg_position) +") takes a CTypes.addressof(xx). "
            arg_position +=1
        if trans:
#             if fun.virtuality == "pure virtual":
#                 print "*** WARNING: Unable to apply transformation to PURE VIRTUAL function", fun, desc
#             else:
            print ("Tranformation applied to ", fun, desc, fun.virtuality)
            fun.add_transformation ( * trans , **{"alias":fun.name}  )
            fun.documentation = docit ("Modified Input Argument to work with CTypes",
                                            desc, "...")
                                            
    for fun in mb.member_functions():
        fixVoids ( fun )
    
    for fun in mb.constructors():
        fixVoids ( fun )
     
                                                                       
    ## lets go and look for stuff that might be a problem  
    def fixPointerTypes ( fun,  pointee_types=[], ignore_names=[], Exclude=False ):
        if fun.documentation or fun.ignore: return ## means it's been tweaked somewhere else
        for n in ignore_names:
            if n in fun.name or n in fun.parent.name:  ## exclude class
                print ("Ignoring: ", fun)
                return
        trans=[]
        desc=""
        arg_position=-1
        for arg in fun.arguments:
            arg_position +=1
            if declarations.is_pointer(arg.type): ## and "const" not in arg.type.decl_string:
                for i in pointee_types:
                    if arg.type.decl_string.startswith(i):
                        if Exclude: 
                            print ("WARNING: Excluding:", fun," due to pointer argument", arg.type.decl_string)
                            fun.exclude()
                            return
                        else:                        
#                             trans.append( ft.inout(arg_position) )
#                             desc = desc +"Argument: "+arg.name+ "( pos:" + str(arg_position) +") now an in/out. "
                            print ("WARNING: Function has pointer argument: ", fun, arg.type.decl_string)
                            fun.documentation=docit("SUSPECT - MAYBE BROKEN due to pointer argument", "....", "...")
                            return
#         if trans:
#             print "Tranformation (auto) applied to ", fun, desc, fun.virtuality
#             fun.add_transformation ( * trans , **{"alias":fun.name}  )
#             fun.documentation = docit ("Auto Modified Arguments:",
#                                             desc, "...")
        
    for fun in mb.member_functions():
        fixPointerTypes ( fun, pointee_types, ignore_names )
    for fun in mb.constructors():
        fixPointerTypes ( fun, pointee_types, [], Exclude=True )
   
                    
def Fix_Pointer_Returns ( mb, pointee_types=['unsigned int','int', 'float','char','unsigned char',
                            'bool', '::Ogre::uint8', '::Ogre::uint16', '::Ogre::uint32' ], known_names=[]):
    """ Change out functions that return a variety of pointer to base types and instead
    have them return the address the pointer is pointing to (the pointer value)
    This allow us to use CTypes to handle in memory buffers from Python
    
    Also - if documentation has been set then ignore the class/function as it means it's been tweaked else where
    """
    for fun in mb.member_functions( allow_empty = True ):
        if declarations.is_pointer (fun.return_type) and not fun.documentation:
#            print "Checking", fun, fun.return_type.decl_string
            for i in pointee_types:
                if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                    if not fun.name in known_names:
                        print ("WARNING: Func (", fun.name, ") returns ", i, ".Using ctype return_addressof functionality")
                        fun.call_policies = call_policies.return_value_policy( call_policies.return_addressof )
                        fun.documentation=docit("CTYPE Integration - returns address of return value", "...", "Address of Return Value")
#                         print "WARNING: Excluding (function):", fun, "as it returns (pointer)", i
#                         fun.exclude()
    for fun in mb.member_operators( allow_empty = True ):
        if declarations.is_pointer (fun.return_type) and not fun.documentation:
            for i in pointee_types:
                if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                    print ("WARNING: Excluding (operator):", fun)
                    fun.exclude()
                    
    # Change 15 Feb 2008 -- adding free function management                
    for fun in mb.free_functions( allow_empty = True ):
        if declarations.is_pointer (fun.return_type) and not fun.documentation:
            for i in pointee_types:
                if fun.return_type.decl_string.startswith ( i ) and not fun.documentation:
                    if not fun.name in known_names:
                        print ("WARNING: Excluding (free function):", fun, "as it returns (pointer)", i)
                        fun.exclude()
                        
    # Update 30 July 2008 -- support for void * variables to be exposed with ctypes handling                        
    for v in mb.variables( allow_empty=True ):
        supported = ['void const *','void *',
                     'char const *','char *',
                     'unsigned char const *','unsigned char *'
                     ]
        if v.type.decl_string in supported:
            try:  # this only works on memeber vars not global ones so need to check
                if v.access_type == 'public' and not v.documentation:
    #                 if not v.parent.noncopyable:    ## this test as we don't support classes with protected destructors
                    print ("CTYPE Implementation on ", v, v.access_type)
                    v.expose_address = True
            except : #RunTimeError:
                pass
                                
                    
def AutoExclude( mb, MAIN_NAMESPACE=None ):
    """ Automaticaly exclude a range of things that don't convert well from C++ to Python
    """
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns
    
    # vars that are static consts but have their values set in the header file are bad
    if os.name =='posix':
       Remove_Static_Consts ( main_ns )

    ## Exclude protected and private that are not pure virtual
    try:
        query = declarations.access_type_matcher_t( 'private' ) \
                & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
        non_public_non_pure_virtual = main_ns.calldefs( query )
        non_public_non_pure_virtual.exclude()
    except:
        pass        

    #Virtual functions that return reference could not be overriden from Python
    try:
        query = declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.VIRTUAL ) \
                & declarations.custom_matcher_t( lambda decl: declarations.is_reference( decl.return_type ) )
        main_ns.calldefs( query ).virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
        pass    
            
def AutoInclude( mb, MAIN_NAMESPACE=None ):
    global_ns = mb.global_ns
    if MAIN_NAMESPACE:
        main_ns = global_ns.namespace( MAIN_NAMESPACE )
    else:
        main_ns = global_ns
        
def Set_DefaultCall_Policies( mb ):
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

def Find_Problem_Transformations ( mb ):
    """ There are some cases where function transformations don't get applied as Py++ doesn't
    support all cases -- the only current one is pure virtual functions don't get managed correctly
    """
    return
    ACCESS_TYPES = declarations.ACCESS_TYPES
    VIRTUALITY_TYPES = declarations.VIRTUALITY_TYPES
    for f in mb.member_functions():
        if len (f.transformations) > 0:
            if f.parent.find_out_member_access_type( f ) == ACCESS_TYPES.PUBLIC:
                if f.virtuality == VIRTUALITY_TYPES.PURE_VIRTUAL:
                    print ("WARNING: Pure Virtual function with Transformations - they will not be applied:", f)
                    
def Remove_Static_Consts ( mb ):
    """ linux users have compile problems with vars that are static consts AND have values set in the .h files
    we can simply leave these out 
    This function is not currently used as source was fixed.."""
    checker = varchecker.var_checker()
    for var in mb.vars():
        if type(var.type) == declarations.cpptypes.const_t:
            if checker( var ):
                print ("Excluding static const ", var)
                var.exclude()                         
        
def Fix_Implicit_Conversions ( mb, ImplicitClasses=[] ):
    """By default we disable explicit conversion, however sometimes it makes sense
    """
    for className in ImplicitClasses:
        mb.class_(className).constructors().allow_implicit_conversion = True

def Fix_ReadOnly_Vars ( main_ns, ToFixClasses, knownNonMutable ):
    """ find read only variables that are "python" mutable, exclude them, add a getter function
    and expose them using properties -- this ensures a copy is made of the variable, otherwise you get a pointer
    to the original 'readonly' C++ variable AND CAN CHANGE IT :( -- it acts like a normal static variable
    which can cause interersting things to go wrong..
    """
    
    GetterFunction = \
    """
    // special function to replace an existing read only variable as 
    // we need to make a copy of the variable as python doesn't understand 'const'
    typedef const %(return_type)s ( *fget_Special_%(variable_name)s )(%(class_type)s &);
    %(return_type)s %(getter_name)s ( %(class_type)s & me )  {
        return me.%(variable_name)s;
        }
    """   
    GetterReg = \
    """
    add_property( // special :)
                "%(variable_name)s" // _Copy
                , fget_Special_%(variable_name)s ( &%(getter_name)s )
                , "special get property, built to access const variable" )
    """
#     ## fix -- the implementation above isn't really what I wanted :)        
#     GetterFunction = \
#     """
#     // special function to replace an existing read only variable as 
#     // we need to make a copy of the variable as python doesn't understand 'const'
#     typedef const %(return_type)s ( *fget_Special_%(variable_name)s )();
#     %(return_type)s %(getter_name)s (  )  {
#         return ::%(class_type)s::%(variable_name)s;
#         }
#     """   
   
    for cls in main_ns.classes():
        if cls.name in ToFixClasses:  # force fixes in specific classes
            for v in cls.variables(allow_empty=True):
                if v.is_read_only and v.exportable==True : # only if it's exposed..
                    if type (v.type) == declarations.cpptypes.const_t: # only care about const's
                        known = False
                        for k in knownNonMutable:
                            if v.type.decl_string.startswith (k):
                                known = True
                        if not known:
                            ## OK so it must be mutable so lets fix it..
                            v.exclude() ## remove it as a variable
                            print ("Excluding Problem Const", cls.name, v)
                            return_type, ignore = v.type.decl_string.split(' ',1) # remove cont etc from decl
                            return_type = return_type[2:] # remove leading ::
                            variable_name = v.name
                            class_type = cls.decl_string[2:] # again don't need leading '::'
                            getter_name = variable_name + '_Getter_Copy' # define a non conflicting name
                            values = { 'return_type':return_type, 'variable_name':variable_name, 
                                'class_type':class_type, 'getter_name':getter_name }
                            # create the actual getter function
                            deccode = GetterFunction % values
                            cls.add_declaration_code( deccode )
                            # and the property registration code
                            regcode = GetterReg % values
                            cls.add_registration_code( regcode )

                            
def samefile ( sourcefile, destfile):
    if not os.path.exists( destfile ):
        #print destfile,"doesn't exist"
        return False
    if not os.path.exists( sourcefile ):
        #print sourcefile,"doesn't exist (missing source file)"
        return False
        
#     inhash = hashlib.md5() #needs python 2.5 or greater
    inhash = md5.new()
    inhash.update ( open ( sourcefile ).read() )
#     outhash = hashlib.md5()
    outhash = md5.new()
    outhash.update ( open ( destfile ).read() )
    if inhash.digest() != outhash.digest():
        return False    
        
    ## probably don't need these as hash should handle it fine..    
    if os.stat(sourcefile).st_mtime > os.stat(destfile).st_mtime:
        return False
    if os.stat(sourcefile).st_size != os.stat(destfile).st_size:
        return False
            
    return True
                                
def copyTree ( sourcePath, destPath, recursive=False, extensions=['cpp','h','hxx','cxx','hpp'], collapse = False, files_in=None ):
    """ function to do a nice tree copy with file filtering and directory creation etc 
       
        Setting collapse to True makes us copy files from the destintion into a single source directory.
        This is needed when source files (.cpp) live in header directories (OgreBullet) and we are compiling them
        as part of the Python-Ogre build system -- we've setup Scons to expect all source files to be in a
        single (flat) directory
    """
    
    ## helper functions
    def filterExtensions ( listin, extensions):
        returnList=[]
        for i in listin:
            e=i.split('.')
            if len(e) >0:
                if e[-1] in extensions:
                    returnList.append(i)
        return returnList  
            
    def makePath ( pathIn ):
        if not os.path.exists (pathIn ):
            os.makedirs ( pathIn )
            
    def copyFile ( fileName, sourcePath, destPath):
        sourceFile = os.path.join ( sourcePath, fileName )
        destFile = os.path.join ( destPath, fileName)
        # we only copy the file if it's a different size and or date
        if not samefile( sourceFile ,destFile ):
            shutil.copyfile( sourceFile, destFile )
            print ("Updated ", destFile, "as it was missing or out of date")

    ## Main code starts here
    
    if not recursive: # Ok so we only care about a single directory and hence don't care about collapse setting
        if not files_in:
            files =  os.listdir( sourcePath )   # get files
            files = filterExtensions ( files, extensions )  # filter them
        else:
            files = files_in
        if len( files ) > 0: 
            makePath ( destPath )   # make a path if need be
            for fileName in files:
                copyFile ( fileName, sourcePath, destPath)  # copy the file
                
    else:   # need to do a recursive copy
        level = 0
        for currentDir, dirs, files in os.walk(sourcePath):
            # remember the initial directory so we can get the subdirs later
            if not level:
                rootPath= currentDir
                subPath = ""
                level += 1
            else:
                subPath = currentDir[ len (rootPath) + 1:] # extract the sub directory path
                
            # filter the file names
            files = filterExtensions ( files, extensions )
            
            if not collapse:
                # Make all the directories in the dest Tree (for this level)
                for dirName in dirs:
                    makePath ( os.path.join (  destPath, subPath, dirName ) )
                    
                # finally do the copy                
                for fileName in files:
                    copyFile ( fileName, currentDir, os.path.join(destPath, subPath ) )
            else:   # told to collapse so everything goes in the destination directory
                for fileName in files:
                    copyFile ( fileName, currentDir, destPath )                      
                    
                    
                    
