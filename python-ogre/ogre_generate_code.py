#!/usr/bin/env python

#-------------------------------------------------------------------------------
# TODO: These are todo's that still need investigation, Roman listed these
# at one point in time.
# 1. SharedPtr
# 3. Hardware classes, I am afraid we will have to wrap them manually. They used void* as a function argumen

import os, logging
import ogre_settings, properties, shared_ptr, comparison_operator, ogre_customization_data

from pyplusplus import code_creators
from pyplusplus import module_builder
from pyplusplus.module_builder import call_policies

from pygccxml import declarations
from pygccxml.declarations import access_type_matcher_t
from pygccxml.declarations import type_traits
from pygccxml import parser

ogre_main_header = 'Ogre.h'


cached_headers = [
    parser.create_cached_source_fc(
        r""+ ogre_settings.headers_dir + "/" + ogre_main_header
        , ogre_settings.cache_dir + "/" + ogre_main_header + ".xml" ),
]


class decl_starts_with (object):
    def __init__ (self, prefix):
        self.prefix = prefix
    def __call__ (self, decl):
        return self.prefix in decl.name

def fix_unnamed_classes (mb):
    ogre_ns = mb.namespace  ('Ogre')
    print len (ogre_ns.classes ('')) / 2

    for unnamed_cls in ogre_ns.classes( '' ):
        named_parent = unnamed_cls.parent
        if not named_parent.name:
            named_parent = named_parent.parent

        for mvar in unnamed_cls.public_members:
            if not mvar.name: 
                continue

            if declarations.is_array (mvar.type):
                template = '''def_readonly("%(mvar)s", &Ogre::%(parent)s::%(mvar)s)'''
            else:
                template = '''def_readwrite("%(mvar)s", &Ogre::%(parent)s::%(mvar)s)'''
            named_parent.add_code( template % dict( mvar=mvar.name, parent=named_parent.name ) )


class private_decls_t:
    def __init__( self ):
        self.__private = {} #fname : [line ]
        for fname in os.listdir( ogre_settings.headers_dir ):
            full_name = os.path.join( ogre_settings.headers_dir, fname )
            if not os.path.isfile( full_name ):
                continue
            fobj = file( full_name, 'r' )
            for index, line in enumerate( fobj ):
                if '_OgrePrivate' in line:
                    if not self.__private.has_key( fname ):
                        self.__private[ fname ] = []
                    self.__private[ fname ].append( index + 1 ) #enumerate calcs from 0, while gccxml from 1
                line = line.strip()
                if line.startswith( '/// Internal method ' ):
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
    ogre_ns = global_ns.namespace( 'Ogre' )
    ogre_ns.include()


    startswith = [
        # Don't include, we'll never need.
        'D3D', 'GL',  'SDL', 'WIN32', 'Any', 'CompositorScriptCompiler', '_', 'Singleton', 

###     'Hardware', 'SharedPtr', 'Cmd', 'FileStreamDataStream', 'PatchSurface', 
        
        'MapIterator', 
        ## This uses a SingletonPointer however it doesn't include OgreSingleton.h or override getSingleton etc
        ## of something else strange is happening ????
        'CompositorManager','SceneManagerEnumerator','SkeletonManager',

        'ManualObject',  #Lots of Virtual Functions returing consts..
        
        ### 'VectorIterator', 'ParticleIterator', 'FileHandleDataStream', 'MemoryDataStream',
###        'PixelBox', 'StringConverter', 'MemoryManager', 'VertexElement', 'MeshSerializerImpl'
###        'PixelUtil', 
###        'DataStream', #void *
        ## now remove some as they use RenderOperation::OperationType which is defined as _OgrePrivate and hence 
        ## filtered out :(  -- of they have an enum type that doesn't seem to work..
###        'EdgeListBuilder', 'ManualObject', 'SubMesh', 'VertexCacheProfiler',
        
#        'ParticleSystem',   ## Causes missings in the link stage
        'MeshSerializerImpl', ## link problems - doesn't seem to exist at all ???
        
        ##it's defined as a const in the OrgeSceneManagerEnumerator.h file - other simular defs do not have the const??
        'DefaultSceneManagerFactory',  ## Link - FACTORY_TYPE_NAME string not found - 
        
        # this one casuses the ogre.pyd moduel not to load (fails when registering)
        'VertexCacheProfiler',
        
        # TODO: this is a really wierd one, Ogre does something funky here.
        'GpuProgramParameters',        
    ]

    ## Remove private classes , and those that are internal to Ogre...
    private_decls = private_decls_t()
    for cls in ogre_ns.classes():
        if private_decls.is_private( cls ):
            cls.exclude()
            print '{*} class "%s" is marked as private' % cls.decl_string

    for func in ogre_ns.calldefs():
        if private_decls.is_private( func ):
            if func.virtuality == declarations.VIRTUALITY_TYPES.PURE_VIRTUAL:
                continue
            func.exclude()
            print '{*} function "%s" is marked as internal' % declarations.full_name( func )

    ## Exclude protected and private that are not pure virtual
    query = ~declarations.access_type_matcher_t( 'public' ) \
            & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL )
    non_public_non_pure_virtual = ogre_ns.calldefs( query )
    non_public_non_pure_virtual.exclude()
    
    ## Exclude all public not pure virtual, that starts with '_'
    query = declarations.access_type_matcher_t( 'public' ) \
            & ~declarations.virtuality_type_matcher_t( declarations.VIRTUALITY_TYPES.PURE_VIRTUAL ) \
            & declarations.custom_matcher_t( lambda decl: decl.name.startswith( '_' ) )
    non_public_non_pure_virtual = ogre_ns.calldefs( query )
    non_public_non_pure_virtual.exclude()
   
    ## Now get rid of a wide range of classes as defined earlier in startswith...
    for prefix in startswith:
        classes = ogre_ns.classes (decl_starts_with(prefix), allow_empty=True)  ### NOTE the PREFIX is used here !!!!
        classes.exclude()
    
    ## AJM uses RenderOperation::OperationType which is defined as _OgrePrivate
    ogre_ns.class_( "EdgeListBuilder" ).member_functions( 'addIndexData' ).exclude()   
    
    ## AJM Error at compile time - errors when compiling or linking
    ogre_ns.class_( "MemoryDataStream" ).member_functions( 'getCurrentPtr' ).exclude()   
    ogre_ns.class_( "MemoryDataStream" ).member_functions( 'getPtr' ).exclude()   
    ogre_ns.calldefs ('useCountPointer').exclude () #AJM Part of OgreSharedPtr
    ogre_ns.calldefs ('peekNextPtr').exclude ()
    ogre_ns.class_( 'RenderSystemOperation' ).exclude() # AJM in OgreCompositorInstance
    ogre_ns.calldefs ('getChildIterator').exclude ()    
 
    
    #AJM Set of functions in Particle system that don't get wrapped properly..    ##TOTEST
    ogre_ns.class_( "ParticleSystem" ).calldefs ( 'CmdCull' ).exclude()   
    ogre_ns.class_( "ParticleSystem" ).calldefs ( 'CmdHeight' ).exclude()   
    ogre_ns.class_( "ParticleSystem" ).calldefs ( 'CmdIterationInterval' ).exclude()   
    ogre_ns.class_( "ParticleSystem" ).calldefs ( 'CmdLocalSpace' ).exclude()   
    ogre_ns.class_( "ParticleSystem" ).calldefs ( 'CmdMaterial' ).exclude()   
    ogre_ns.class_( "ParticleSystem" ).calldefs ( 'CmdNonvisibleTimeout' ).exclude()   
    ogre_ns.class_( "ParticleSystem" ).calldefs ( 'CmdQuota' ).exclude()   
    ogre_ns.class_( "ParticleSystem" ).calldefs ( 'CmdRenderer' ).exclude()   
    ogre_ns.class_( "ParticleSystem" ).calldefs ( 'CmdSorted' ).exclude()   
    ogre_ns.class_( "ParticleSystem" ).calldefs ( 'CmdWidth' ).exclude()   
 
    ###  Issues with various Singleton issues  
#    ogre_ns.class_( "SkeletonManager" ).member_functions( 'getSingleton' ).exclude() 
#    ogre_ns.class_( "SkeletonManager" ).member_functions( 'getSingletonPtr' ).exclude() 
#    ogre_ns.class_( "Root" ).member_functions( 'getSingleton' ).exclude() 
#    ogre_ns.class_( "Root" ).member_functions( 'getSingletonPtr' ).exclude() 
#    ogre_ns.class_( "TextureManager" ).member_functions( 'getSingleton' ).exclude() 
#    ogre_ns.class_( "TextureManager" ).member_functions( 'getSingletonPtr' ).exclude() 
#    ogre_ns.class_( "SceneManagerEnumerator" ).member_functions( 'getSingleton' ).exclude() 
#    ogre_ns.class_( "SceneManagerEnumerator" ).member_functions( 'getSingletonPtr' ).exclude() 
       
    ## AJM These cause the python module to fail to load  -- need to recheck they are 'all' responsible
    ogre_ns.class_ ('ResourceGroupManager').calldefs( 'ResourceDeclaration' ).exclude() #AJM ????
    ogre_ns.class_( "ResourceGroupManager" ).member_functions( 'getResourceDeclarationList' ).exclude()  ##Python load issue
    ogre_ns.class_( "ResourceGroupManager" ).member_functions( 'linkWorldGeometryToResourceGroup' ).exclude()  ##Python load issue
    ogre_ns.class_( "ResourceGroupManager" ).member_functions( 'openResource' ).exclude()  ##Python load issue
    ogre_ns.class_( "ResourceGroupManager" ).member_functions( 'declareResource' ).exclude()  ##Python load issue

    # These members have Ogre::Real * methods which need to be wrapped.
    ogre_ns.class_ ('Matrix3').member_operators (symbol='[]').exclude ()
    ogre_ns.class_ ('Matrix4').member_operators (symbol='[]').exclude ()
    
    #returns reference to "const Real *"
    ogre_ns.class_ ('BillboardChain').calldef( 'getOtherTextureCoordRange' ).exclude()
    
    #all constructors in this class are private, also some of them are public.
    Skeleton = ogre_ns.class_( 'Skeleton' ).constructors().exclude()

    #"properties.py" functionality exports as "data" property function, that
    #returns reference to non-const uchar*, this cuased generated code to
    #raise compilation error
    ogre_ns.class_( "Image" ).member_functions( 'getData' ).exclude()
 
    ## STUFF that hasn't been rechecked for failure...    
    # Methods which have void *'s but are contained in classes I need exported for the sample.
    # once the void * wrappers have been written, this should go away.
    ogre_ns.free_functions ('any_cast').exclude () #not relevant for Python
     
    #AttribParserList is a map from string to function pointer, this class could not be exposed
    AttribParserList = ogre_ns.typedef( name="AttribParserList" )
    declarations.class_traits.get_declaration( AttribParserList ).exclude()
    
    #Andy, please explain why do you want/need to exclude them.
    #Also it is possible to write rule, that will exclude all variable,
    #that their names is in upper
    ogre_ns.class_('RenderQueueInvocation').variable( 'RENDER_QUEUE_INVOCATION_SHADOWS' )
    ogre_ns.class_('RibbonTrailFactory').variable( 'FACTORY_TYPE_NAME' )
    SceneManager = ogre_ns.class_( 'SceneManager' )
    var_names = [ "ENTITY_TYPE_MASK", "FX_TYPE_MASK", "LIGHT_TYPE_MASK"
                  , "STATICGEOMETRY_TYPE_MASK", "USER_TYPE_MASK_LIMIT"
                  , "WORLD_GEOMETRY_TYPE_MASK" ]
    SceneManager.variables( lambda var: var.name in var_names ).exclude()
        

    
def set_call_policies( mb ):
    ogre_ns = mb.global_ns.namespace ('Ogre')

    # Set the default policy to deal with pointer/reference return types to reference_existing object
    # as this is the Ogre Default.
    mem_funs = ogre_ns.calldefs ()
    mem_funs.create_with_signature = True #Generated code will not compile on
    #MSVC 7.1 if function has throw modifier.
    for mem_fun in mem_funs:
        if mem_fun.call_policies:
            continue
        if declarations.is_pointer (mem_fun.return_type) \
            or declarations.is_reference (mem_fun.return_type):
            mem_fun.call_policies = call_policies.return_value_policy(
                call_policies.reference_existing_object )



def generate_alias (mb):
    for name, alias in ogre_customization_data.name2alias.items():
        print "Looking for", name
        try:
            decl = mb.class_( name )        
            decl.alias = alias
            decl.wrapper_alias = alias + '_wrapper'
        except Exception, error:
            print '{-}==>', name

    for name, alias in ogre_customization_data.name2alias_class_decl.items():
        print "Looking for", name
        decl = mb.decl( name, lambda decl: isinstance( decl, declarations.class_declaration_t ) )
        decl.alias = alias


def generate_code():
    #Creating an instance of class that will help you to expose your declarations
    mb = module_builder.module_builder_t( cached_headers
                                          , gccxml_path=ogre_settings.gccxml_path
                                          , working_directory=ogre_settings.working_dir
                                          , include_paths=[ogre_settings.headers_dir]
                                          , start_with_declarations=['Ogre']
                                          , indexing_suite_version=2 )
    mb.BOOST_PYTHON_MAX_ARITY = 25
    mb.classes().always_expose_using_scope = True
    
    global_ns = mb.global_ns
    global_ns.exclude()
    ogre_ns = global_ns.namespace( 'Ogre' )
    ogre_ns.include()
    
    filter_declarations (mb)
    fix_unnamed_classes (mb)

    shared_ptr.create (mb)
    

    for cls in ogre_ns.classes():
        print cls #, dir(cls)

    #ogre_ns.class_( 'Singleton<Ogre::>' ).exclude
    #ogre_ns.class_( 'Singleton<Ogre::TextureManager>' ).exclude
    
    comparison_operator.create (mb)

    #Creating code creator. After this step you should not modify/customize declarations.
    mb.build_code_creator (module_name='Ogre')

    # Create properties for accessors
    #properties.create (mb)
    set_call_policies (mb)
    generate_alias (mb)

    mb.code_creator.user_defined_directories.append (ogre_settings.headers_dir)
    # TODO: make this into a list that gets added.
    for header in ogre_settings.ogre_header_list:
        mb.code_creator.add_include (header)

    mb.code_creator.add_include ("CustomSharedPtr.hpp")
    mb.code_creator.add_include ("ComparisonOperators.hpp")
    mb.code_creator.add_include ("OgreHardwareOcclusionQuery.h")

    #Writing code to the following directory.
    huge_classes = []    
    huge_class_names = [ 'RenderSystem', 'StaticGeometry', 'Node', 'Pass', 'BillboardSet', 'ParticleEmitter', 
                            'ParticleSystem', 'SceneManager' ]
    for name in huge_class_names:
        huge_classes.append( mb.class_( name ) )
    
    mb.split_module(ogre_settings.build_dir, huge_classes)

# vim:et:ts=4:sts=4:sw=4

if __name__ == '__main__':
    generate_code()