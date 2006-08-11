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

        # TODO: Write these manual wrappers ... (void *'s)
        # TODO: SharedPtr's manual wrapper is already mostly done by Roman, I should include it.
        # Don't include as they'll probably need manual wrappers
        # Reasons vary, but likely include: use of void *, or other semantics which can't be inferred.
        'Hardware', 'SharedPtr', 'Cmd', 'FileStreamDataStream', 'PatchSurface', 
        'MapIterator', 'VectorIterator', 'ParticleIterator', 'FileHandleDataStream', 'MemoryDataStream',
        'PixelBox', 'StringConverter', 'MemoryManager', 'VertexElement', 'MeshSerializerImpl', 

        # TODO: this is a really wierd one, Ogre does something funky here.
        'GpuProgramParameters',

        # TODO: Find out what the problems with thise are and fix it or ask the mailling list.
        # Don't include because they have some problems,  we'll want to solve thos problems
        # at a later date 
        #'Image' - problem with data property, returns pointer to unsigned char
        
    ]

    #Skeleton should have no_init and defaul held_type

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

    for prefix in startswith:
        classes = ogre_ns.classes (decl_starts_with(prefix), allow_empty=True)
        classes.exclude()

    # These members have Ogre::Real * methods which need to be wrapped.
    ogre_ns.class_ ('Matrix3').member_operators (symbol='[]').exclude ()
    ogre_ns.class_ ('Matrix4').member_operators (symbol='[]').exclude ()
    #returns reference to "const Real *"
    ogre_ns.class_ ('BillboardChain').calldef( 'getOtherTextureCoordRange' ).exclude()
    #all constructors in this class are private, also some of them are public.
    Skeleton = ogre_ns.class_( 'Skeleton' )
    Skeleton.constructors().exclude()

    #"properties.py" functionality exports as "data" property function, that
    #returns reference to non-const uchar*, this cuased generated code to
    #raise compilation error
    ogre_ns.class_( "Image" ).member_functions( 'getData' ).exclude()

    # TODO: These produce an error, but I can no longer remember what.
    ogre_ns.member_functions ('getCustomAttribute').exclude ()
    ogre_ns.calldefs ('getChildIterator').exclude ()

    # Methods which have void *'s but are contained in classes I need exported for the sample.
    # once the void * wrappers have been written, this should go away.
    ogre_ns.free_functions ('any_cast').exclude () #not relevant for Python
    ogre_ns.calldefs ('setOption').exclude ()
    ogre_ns.calldefs ('getOption').exclude ()
    ogre_ns.calldefs ('createBezierPatch').exclude ()
    ogre_ns.calldefs ('define').exclude ()
    ogre_ns.calldefs ('distributeControlPoints').exclude ()
    ogre_ns.calldefs ('subdivideCurve').exclude ()
    ogre_ns.calldefs ('interpolateVertexData').exclude ()
    ogre_ns.calldefs ('flipToLittleEndian').exclude ()
    ogre_ns.calldefs ('flipFromLittleEndian').exclude ()
    ogre_ns.calldefs ('flipEndian').exclude ()
    ogre_ns.calldefs ('writeData').exclude ()
    ogre_ns.calldefs ('doGet').exclude ()
    ogre_ns.calldefs ('doSet').exclude ()
    #AttribParserList is a map from string to function pointer, this class could not be exposed
    AttribParserList = ogre_ns.typedef( name="AttribParserList" )
    declarations.class_traits.get_declaration( AttribParserList ).exclude()
    #We have to expose this function, otherwise the code will not compile,
    #because the class wrapper will be abstract.
    #Roman, may it is possible to specify the body of the function as throw some
    #exception?
    #ogre_ns.class_ ('DataStream').calldefs ('read').exclude ()
    
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


module_builder.set_logger_level( logging.INFO )


def generate_code():
    #Creating an instance of class that will help you to expose your declarations
    mb = module_builder.module_builder_t( cached_headers
                                          , gccxml_path=ogre_settings.gccxml_path
                                          , working_directory=ogre_settings.working_dir
                                          , include_paths=[ogre_settings.headers_dir]
                                          , start_with_declarations=['Ogre']
                                          , indexing_suite_version=2 )
    mb.BOOST_PYTHON_MAX_ARITY = 19
    mb.classes().always_expose_using_scope = True
    filter_declarations (mb)
    fix_unnamed_classes (mb)
    shared_ptr.create (mb)
    comparison_operator.create (mb)

    #Creating code creator. After this step you should not modify/customize declarations.
    mb.build_code_creator (module_name='Ogre')

    # Create properties for accessors
    properties.create (mb)
    set_call_policies (mb)
    generate_alias (mb)

    mb.code_creator.user_defined_directories.append (ogre_settings.headers_dir)
    # TODO: make this into a list that gets added.
    for header in ogre_settings.ogre_header_list:
        mb.code_creator.add_include (header)

    mb.code_creator.add_include ("CustomSharedPtr.hpp")
    mb.code_creator.add_include ("ComparisonOperators.hpp")

    #Writing code to the following directory.
    mb.split_module (ogre_settings.build_dir)

# vim:et:ts=4:sts=4:sw=4

if __name__ == '__main__':
    generate_code()