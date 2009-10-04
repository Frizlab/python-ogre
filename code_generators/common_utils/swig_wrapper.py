from pygccxml import declarations
from pygccxml import declarations
from pyplusplus import decl_wrappers
import os
import sys

# class
# constructor
# member function
# descructors
# variables

# 
# output Arrays
#
mainOutput = []
headerOutput = []
footerOutput = []
templateOutput = []
handWrapperOutput = []

def addOutput ( line, output = mainOutput ):
    output.append ( line )
    
def addHeader ( line ):
    headerOutput.append ( line )
#
# Handle the concept of public/protected access attributes
#
currentAccess = ""
def checkAccess( newAccess = "" ):
    global currentAccess
    if newAccess == currentAccess: 
        return
    currentAccess = newAccess
    mainOutput.append(newAccess + ':' )
    
def clearAccess ():
    global currentAccess
    currentAccess = ""
    
def setAccess ( access ):
    global currentAccess
    currentAccess = access

#
# handle various attributes = no intelligence, just do it
#

def is_virtual ( cls ):
    try:
        return cls.has_vtable
    except:
        if cls.virtuality != declarations.VIRTUALITY_TYPES.NOT_VIRTUAL:
            return True
    return False

def check_virtual ( defination ):
    if is_virtual(defination):
        return "virtual "
    return "" 
    
classClose = ""
currentNS=""
NScount = 0
def process_class ( cls, base='Ogre' ):
    global mainOutput, classClose, currentNS, NScount
    outString = ""
    #if check_virtual ( cls ):
    #    outString="\\VIRTUAL CLASS\n"
    classClose = "};"
    if currentNS != cls.parent.decl_string:
        if NScount >0:
            outString += "\n}; // close name space " + currentNS[2:] + "\n"
        NScount += 1    
        currentNS = cls.parent.decl_string
        outString += "namespace " + cls.parent.decl_string[2:] + " {\n"
        
##   Changing logic as namepsace's cause issues wiht SWIG generator
##   so going for fully qualified names  
##    if cls.parent.name != base:
##        outString += "namespace " + cls.parent.name + " {\n"
##        classClose += "\n};"
##    outString += "namespace " + cls.parent.decl_string + " {\n"
##    classClose += "\n};"
    
    
    outString +=  "class "  + cls.name
    
    first = 0 
    for b in cls.bases:
        r = b.related_class
        if ("Allocated" not in r.name 
            and not b.access_type == "private"
            and "Singleton" not in r.name
            and not is_virtual ( r ) ) :
                if first == 0:
                    outString += " : " + b.access_type + " " + r.name + " "
                if first > 0:
                    outString += " ," + b.access_type + " " + r.name                     
                first += 1
    outString += "\n{"
    mainOutput.append ( outString )
    
    # print mainOutput
    return outString
    
def process_class_close():
    mainOutput.append ( classClose )
    
def process_destructor ( destructor, class_name ):
    global mainOutput
    checkAccess ( destructor.access_type )
    outString = "\t" + check_virtual ( destructor )
    outString += destructor.decl_string
    mainOutput.append ( outString )
    
def process_constructor ( constructor, class_name ):
    global mainOutput
    checkAccess ( constructor.access_type )
    outString = "\t" + check_virtual ( constructor )
    outString += class_name + " ("
    count = 0
    for v in constructor.arguments:
        if count > 0:
            outString += " ,"
        outString += v.type.decl_string + " " + v.name
        count += 1
    if count == 0:
        outString += " void"                     
    outString +=  " );"
    mainOutput.append ( outString )
    #print "RECEIVED", constructor
    #print outString
    #print
    return outString
    
def process_function ( function ):
    global mainOutput
    checkAccess ( function.access_type )
    outString = "\t" + check_virtual ( function )
    outString += str(function.return_type) + " " + function.name + " ("     #the function name
    count = 0
    for v in function.arguments:   # handle any arguments
        if count > 0:
            outString += ", "
        outString += v.type.decl_string + " " + v.name + " "
        if v.default_value:
            outString += "= " + v.default_value
        count += 1
        if count == 0:
            outString += " void ",   # put void in if no arguments to be proper... 
    outString += " )"
    if function.virtuality == declarations.VIRTUALITY_TYPES.PURE_VIRTUAL:
        outString += " = 0"
    outString += ";"
    mainOutput.append ( outString )
    #print "Returning:", mainOutput
    return outString

def process_template ( cls ):
    outString =  "%template ("+ cls.alias +") " + cls.parent.name + "::"+cls.name+";"
    templateOutput.append ( outString )
    return outString
    
def getOutput ():
    output = ""
    for a in headerOutput, handWrapperOutput, mainOutput, templateOutput, footerOutput:
        output += "\n".join ( a )
        output += "\n"
    return output
    
def write ( filename = None ):
    if filename:
        try:
            f= open( filename, 'w' )
            f.write ( getOutput() )
            f.close()
        except:
            print "ERROR writing output to", filename
            raise
   
   
def check_args_exportable ( function, ns ):
    """ Look at each argument in the function and determine that we have exported it 
    or it's a special.
    """
    ret = True
    Specials = ['::Ogre::String']
    for a in function.arguments:
        rawarg =  declarations.remove_declarated(
            declarations.remove_const( 
                declarations.remove_reference( 
                    declarations.remove_pointer ( a.type ))))
                                               
        ## now check if the arg is a fundemental type (int float etc), a void
        ##  or a special ..
        if declarations.is_arithmetic (rawarg)\
                or declarations.is_void(rawarg)\
                or declarations.is_enum(rawarg):
            pass
        elif 'Ogre::' in a.type.decl_string: # assume it's a class and needs checking
            name = a.type.decl_string.split()[0] # let's grab the actual class name
            if name in Specials:  # we know that the classes in specials DO exist 
                pass
            else:
                try:
                    tcls = ns.class_(name)
                    if not tcls.exportable or tcls.ignore or type ( tcls.parent ) != decl_wrappers.namespace_wrapper.namespace_t: 
##                        print "check_args_exportable: NOT EXPORTABLE:", tcls, tcls.exportable, tcls.ignore , type ( tcls.parent )  
                        ret = False
                        break
                    else:
                        pass # print name, "IS exportable"
                except:
                    print "check_args_exportable: unable to find:", name
                    ret = False
        else:
            print "check_args_exportable: NOT SURE...", a, a.type, type(a.type)
        
    return ret            
  
def swig_process_functions ( cls, main_ns, countonly = False ):
    """ Handle all the functions in a particluar class,  if countonly==True then it will simply return the
    count of valid fucntions in the class -- allwing empty classes to be ignored
    """
    excludes = ['SceneManager::getQueuedRenderableVisitor', 'SceneManager::setQueuedRenderableVisitor',
                        'CompositorScriptCompiler::addLexemeTokenAction','CompositorScriptCompiler::addLexemeAction',
                        'Log::stream','LogManager::stream','RenderQueueInvocationSequence::iterator',
                        'Pose::getVertexOffsetIterator','GpuSharedParameters::getConstantDefinitionIterator',
                        'SceneManager::getAnimationStateIterator','SceneManager::getShadowTextureConfigIterator',
                        'GpuProgramParameters::getConstantDefinitionIterator','AnimationStateSet::getAnimationStateIterator',
                        'AnimationStateSet::getEnabledAnimationStateIterator'
                        ]
    count = 0                    
    if not countonly : clearAccess()
    for d in cls.member_functions(allow_empty=True):
        if not d.ignore and d.parent.name == cls.name: # and d.access_type == 'public' Can't be Private OR a subclass function
            fullname = cls.name + "::" + d.name
            if 0 and '<' in d.return_type.decl_string: # there's a template to handle
                if not countonly : addOutput("//\t" + d.name + " " + d.decl_string + "\t// Excluded as returns a template" )
                #print "TEMPLATE"
##            elif d.name.startswith('get') and d.name.endswith('Iterator'):
##                if not countonly : addOutput( "//\t" + d.name + " " + d.decl_string + "\t// Excluded as might return an iterator." ) 
##                #print "ITERATOR"
            elif fullname in excludes:
                if not countonly : addOutput( "//\t" + d.name + " " + d.decl_string + "\t// Excluded manually as problem function.")
                #print "MANUAL"
            elif not check_args_exportable( d, main_ns ):
                if not countonly : addOutput( "//\t" + d.name + " " + d.decl_string + "\t// Excluded function has arguments that are non public classes.") 
                #print "NON PUBLIC"
            else:  
                #print "PROCESSING", d
                if not countonly : process_function ( d )  
                count +=1
    return count

def main_process_swig ( main_ns, filename = None ):
    """ does all the work 
    """
    global NScount
    masterList = []
    toProcess = []
    for cls in main_ns.classes():
        if len (cls.bases) == 0:
            masterList.append (cls.decl_string)
    start= len(masterList)
    for x in range ( 8 ):
        for cls in main_ns.classes():
            if cls.decl_string not in masterList:
                OK = True
                for b in cls.bases:
                    r = b.related_class
                    if r.decl_string not in masterList:
                        OK = False
                        break
                if OK:
                    masterList.append(cls.decl_string)
        if start == len(masterList):
            break
        start = len(masterList)  
##    print masterList
##    sys.exit()
    nodefaultctor = ['RenderTexture', 'SceneManager', 'Log', 'Camera', 'SubEntity',
                'AxisAlignedBoxSceneQuery', 'SimpleRenderable', 'SphereSceneQuery', 'SphereSceneQuery',
                'PlaneBoundedVolumeListSceneQuery', 'HardwareVertexBuffer', 'HardwareIndexBuffer', 
                'OverlayContainer', 'Skeleton']
    
    addHeader ("%module OgreBindings")
    addHeader ("#pragma SWIG nowarn=362,363,383,384,389, 312, 350, 351, 394, 395, 314, 302, 503, 509, 490 ")
    addHeader ("%{\n#include \"python_ogre_masterlist.h\"\n%}")
    addHeader ("#define _OgreExport \n")
    
##    addHeader ("namespace Ogre {")


    # this is for swig style ignores...
    ignores = [\
              #  'ConfigFile::load(::Ogre::String const &, ::Ogre::String const & , ::Ogre::String const &, bool)',
                'Ogre::ConfigFile::load (::Ogre::String const &, ::Ogre::String const &, ::Ogre::String const & separators = "\\t:=", bool trimWhitespace = true)',
                'Ogre::FastHash; // (const char * data, int len, uint32 hashSoFar = 0)'
                ]
    for x in ignores:
        addHeader("%ignore " + x + ';')

    for h in nodefaultctor:
        addHeader ("%nodefaultctor Ogre::"+h+";")
    includes = ['std_string.i','std_pair.i','std_vector.i',
                'std_map.i','std_list.i','std_multimap.i','std_set.i',
                'c:\\development\\ogre\\ogremain\\include\\ogreiteratorwrapper.h',
                'c:\\development\\ogre\\ogremain\\include\\OgrePrerequisites.h'
                ]
    #includes = ['std_string.i']
    for x in includes:
        addHeader('%include "' + x + '";' )            

    ogreIncludes = ['OgrePrerequisites.h','OgreSingleton.h',
                'OgreException.h','OgreIteratorWrapper.h',
                'OgreMemoryAllocatedObject.h','OgreMemoryAllocatorConfig.h',
                'OgreMemoryTracker.h','OgreCommon.h','OgreFactoryObj.h',
                'OgrePlatform.h', 'OgreAtomicWrappers.h'
                ]
                
    for x in ogreIncludes:
        x = os.path.join('c:/development/ogre/ogremain/include',x)
        addHeader('%include "' + x + '";' )            

    applies = ['const std::string & {std::string &}', 'std::string & {std::string *}',
                'const std::string & {const ::Ogre::String &}', 'std::string & {::Ogre::String &}',
                'std::string & {::Ogre::String *}' 
                ]         
    for x in applies:
        addHeader("%apply " + x + ';')

    specialTemplates = ['(TemplateIteratorWrapperMapString) Ogre::IteratorWrapper< std::map< std::string,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > > *,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > > * >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > >,std::map< std::string,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > > *,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > > * >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > >::iterator,std::map< std::string,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > > *,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > > * >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > >::mapped_type >',
            '(TemplateIteratorWrapperMultiMapString) Ogre::IteratorWrapper< std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > >,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > >::iterator,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > >::mapped_type >',
            '(TemplateMapIteratorWrapperMapString) Ogre::MapIteratorWrapper< std::map< std::string,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > > *,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > > * >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > >,std::map< std::string,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > > *,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > > * >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > >::iterator >',
            '(TemplateMapIteratorWrapperMultiMapString) Ogre::MapIteratorWrapper< std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > >,std::multimap< std::string,std::string,std::less< std::string >,Ogre::STLAllocator< std::pair< std::string const,std::string >,Ogre::CategorisedAllocPolicy< (Ogre::MemoryCategory) 0 > > >::iterator >'
            ]
    for x in specialTemplates:
        addOutput ( "%template " + x + ";", templateOutput)
        
    # and this is how we do it py++ style....    
    ignores = [\
                ("::Ogre::ConfigFile::load", [ '::Ogre::String const &', '::Ogre::String const &', '::Ogre::String const &', 'bool'] )
               ]
    for x in ignores:
        main_ns.member_function(x[0], arg_types=x[1]).exclude()
        
    classtype = decl_wrappers.class_wrapper.class_t
    excludedList = ['EdgeData', 'EdgeGroup', 'Listener', 'Spines', 'Triangle', 'Edge', 'TempVertex', 
                    'ProfileFrame']
    excludedList = ['Font', 'UTFString', 'EdgeListBuilder', 'Any', 'Profiler', 'StaticGeometry',
                    'RenderQueueListener','LogListener', 'MeshSerializerListener','MeshSerializerListener',
                    'OverlayElementFactory','ResourceBackgroundQueue', 'Resource::Listener',
                    'EdgeData','ScriptTranslator','ScriptCompiler',
                    'GpuProgramManager', 'ResourceManager','ResourceLoadingListener',
                    'LogManager', 'SubEntity', 'ExceptionFactory']    
    for e in excludedList:
        main_ns.class_("::Ogre::"+e).exclude()
    excludedList=[]
       
##    n = time.clock()
##    for c in main_ns.classes():
##        print "Looking3:", c
##        if c.decl_string in masterList:
##            print "Found3", c
##    print "took", time.clock() - n
##    sys.exit()
##    for name in masterList:
##        print "Looking1 for:", name
##        cls = main_ns.class_( name )
##        print "found1", cls
##    print "took", time.clock() - n
##    sys.exit()

    # do all of this for a spped up and the need to keep things in order
    lookup = {}
    for cls in main_ns.classes():
        if cls.decl_string in masterList:
            lookup [cls.decl_string]=cls
    for c in masterList:
            cls = lookup[c]           
            if cls.ignore or cls.name in excludedList:
                addOutput ("// EXCULDING Class (ignored intentionally): " + cls.name)
            elif type ( cls ) != decl_wrappers.class_wrapper.class_t:
                print "NOT A CLASS", cls, type (cls), decl_wrappers.class_wrapper.class_t, decl_wrappers.class_wrapper.class_t()
            elif type ( cls.parent ) != decl_wrappers.namespace_wrapper.namespace_t:
                print "Can't support class as it's a sub class", cls
                addOutput ("// EXCLUDING Class as it's a subclass (not supported by SWIG): " + cls.name)
            elif 'Iterator' and '<' in c:
                process_template(cls)
            elif swig_process_functions ( cls, main_ns, True ) > 0: # this is to exclude empty classes
                clearAccess()
                process_class ( cls )
               
                clearAccess()
                # cls = main_ns.class_('::Ogre::SceneQuery')
                if cls.name not in nodefaultctor : # don't expose constructors for virtual classes
                    for c in cls.constructors():
                        if c.access_type == 'public' and not c.ignore:
##                            and not is_virtual(c) ) :
                            if not '[' in c.decl_string and check_args_exportable( c, main_ns ):
                                if c.parent == cls: # can't be a subclass constructor (for now)
                                    process_constructor ( c, cls.name )
                clearAccess()
                
    #            for c in cls.destructors():
    #                #if c.access_type != 'private' and not c.ignore and not '[' in c.decl_string:
    #                if  not c.ignore : # c.access_type == 'public' and
    #                    process_destructor ( c, cls.name )
                swig_process_functions ( cls, main_ns )            
                
                #sys.exit()
    #            headers.add ( cls.location.file_name )
    #            if not declarations.templates.is_instantiation( cls.name ):
    #               p=cls.parent
    #               if type(p) == type (classtype):
    #                   ignores.append ( "%ignore " + cls.decl_string +";" )
    #               continue
    #            name = declarations.templates.name( cls.name )
    #            templates.append ("%template ("+ cls.alias+") Ogre::"+cls.name+";")
    #            if name.endswith( 'Iterator' ):
    #                pass
                print "PROCESS CLASS CLOSE"
                process_class_close() 
    
     
    if NScount > 0: # we had a namespace
        addOutput ( "};" ) # make sure it gets closed
    
    # Specials
    goodtemps = ['SharedPtr','Singleton']

    template =  """    template <typename T> class Singleton
    {
    public:
        Singleton( void );
        ~Singleton( void );
        static T& getSingleton( void );
        static T* getSingletonPtr( void );
    };
    
	template<class T> class SharedPtr
	{
	protected:
		T* pRep;
		unsigned int* pUseCount;
		SharedPtrFreeMethod useFreeMethod; 
	public:
		SharedPtr();
		
      template< class Y>
		SharedPtr(Y* rep, SharedPtrFreeMethod freeMethod ) ;
		
		SharedPtr(const SharedPtr& r);
		SharedPtr& operator=(const SharedPtr& r);
		
		template< class Y>
		SharedPtr(const SharedPtr<Y>& r);
		
		template< class Y>
		SharedPtr& operator=(const SharedPtr<Y>& r);
		
		T& operator*() const;
		T* operator->() const;
		T* get() const;
		
		void bind(T* rep, SharedPtrFreeMethod freeMethod);
		
		/*inline bool unique() const { OGRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount == 1; }
		inline unsigned int useCount() const { OGRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount; }
		inline unsigned int* useCountPointer() const { return pUseCount; }

		inline T* getPointer() const { return pRep; }
		inline SharedPtrFreeMethod freeMethod() const { return useFreeMethod; }

		inline bool isNull(void) const { return pRep == 0; }

      inline void setNull(void);
      */
	};    \n
	
	"""
    if 0:
        addOutput ( template, templateOutput )
        
        for cls in main_ns.classes():
            if not declarations.templates.is_instantiation( cls.name ):
               continue
            name = declarations.templates.name( cls.name )
    #         print "//\t", name, "\t", cls.name 
            for n in goodtemps:
                if name.startswith (n):
                    process_template ( cls )
                    break
    print "111"            
    output = getOutput()
    if filename:
        print "WRITING"
        write ( filename )
        
    return output            
           