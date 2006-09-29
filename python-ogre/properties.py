#!/usr/bin/env python

from pygccxml import declarations
from pyplusplus import code_creators
from pyplusplus import module_builder

'''OGRE typically uses getX and setX for properties. However, we have to be
careful because there are methods names setX and getX which are not true
properties (in a pythonic sense), as sometimes a getX will take one or more
arguments, or the setX will take more than one argument.

TODO: there are probably exceptions to the naming scheme which should be found
and documented.

Yes: isVisible setVisible

'''

IMPROPER_GETTERS = set()
IMPROPER_SETTERS = set()

def query_functor( decl ):
    global IMPROPER_GETTERS, IMPROPER_SETTERS
    if decl.ignore:
        return False
    if len( decl.name ) <= 3:
        return False
    if not( decl.name.startswith('get') or decl.name.startswith('set') ):
        return False
    if decl.access_type != 'public':
        return False
    if decl.name[0] == 'g': #get
        if declarations.is_void( decl.return_type ):
            IMPROPER_GETTERS.add( decl )
            return False
        if decl.arguments:
            IMPROPER_GETTERS.add( decl )
            return False
    else: #set
        if 1 != len( decl.arguments ):
            IMPROPER_SETTERS.add( decl )
            return False
    return True

def find_accessor_in_base(cls, name, type):
    '''Searches in the base classes for the accessor with the right type and name.'''
    for base_class in cls.recursive_bases:
        if base_class.related_class.ignore: #don't scan excluded classes
            continue
        if 'public' != base_class.access_type: #don't scan non public hierarchy
            continue
        accessor_list = base_class.related_class.member_functions( query_functor, recursive=False, allow_empty=True )
        for mem_fun in accessor_list:
            if name == mem_fun.name[3:] and type == mem_fun.name[:3]:
                return mem_fun
    return None

def is_property_pair( fget, fset ):
    if not fset:
        return True

    normalize = lambda t: t
    normalize1 = lambda t: declarations.remove_cv( declarations.remove_pointer( t ) )
    normalize2 = lambda t: declarations.remove_cv( declarations.remove_reference( t ) )
    for n in [ normalize, normalize1, normalize2 ]:
        if declarations.is_same( n(fget.return_type), n(fset.arguments[0].type) ):
            return True
    return False

def add_properties_for_class( cls ):
    global IMPROPER_SETTERS
    accessors_db = {} # name :[ get, set ]
    for mem_fun in cls.member_functions( query_functor, recursive=False, allow_empty=True ):
        property_name = mem_fun.name[3:]
        if property_name not in accessors_db:
            accessors_db[ property_name ] = [None, None] #get, set
        accessors_db[ property_name ][ int(mem_fun.name[0] == 's') ] = mem_fun
    for name, accessors in accessors_db.items():
        if not accessors[0]:
            accessors[0] = find_accessor_in_base( cls, name, 'get' )
            if not accessors[0]:
                continue #we don't have get part
        if not accessors[1]:
            accessors[1] = find_accessor_in_base( cls, name, 'set' )
        if not is_property_pair( *accessors ):
            IMPROPER_SETTERS.add( accessors[1] )
            continue
        fname = declarations.full_name
#         print "Adding: ", name, accessors[0], accessors[1] 
        cls.add_property( name, accessors[0], accessors[1] )

def create(mb):
    for cls in mb.classes( lambda decl: decl.ignore == False ):
        add_properties_for_class( cls )
    print 'improper getters',  len( IMPROPER_GETTERS ), IMPROPER_GETTERS 
    print 'improper setters',  len( IMPROPER_SETTERS ), IMPROPER_SETTERS

