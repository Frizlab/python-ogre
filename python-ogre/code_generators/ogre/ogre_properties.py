from pygccxml import parser
from pygccxml import declarations
from pyplusplus import module_builder

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
from pygccxml import declarations
from pyplusplus import decl_wrappers

class ogre_property_recognizer_t( decl_wrappers.name_based_recognizer_t ):
    def __init__( self ):
        decl_wrappers.name_based_recognizer_t.__init__( self )    
    
    def __is_setter( self, mem_fun ):
        if len( mem_fun.arguments ) != 1:
            return False
        if not declarations.is_void( mem_fun.return_type ):
            return False
        if mem_fun.has_const:
            return False
        return True

    def create_read_only_property( self, fget ):
        found = self.find_out_ro_prefixes( fget.name )
        if found in ( None, '' ):
            return None
            
        pure_name = fget.name[len( found ):]
        pure_name = pure_name.lower()
        is_setter = lambda decl: decl.name.lower() == pure_name and self.__is_setter( decl )
        setters = fget.parent.mem_funs( is_setter, allow_empty=True, recursive=False )
        print "1",setters
        for hierarchy_info in fget.parent.recursive_bases:
            if hierarchy_info.related_class.ignore: #don't scan excluded classes
                continue
            if 'public' != hierarchy_info.access_type: #don't scan non public hierarchy
                continue
            base_cls = hierarchy_info.related_class
            print "2",setters
            try:
                setters.extend( base_cls.mem_funs( is_setter, recursive=False  , allow_empty=True ) )
            except:
                print "Exception !!!!!!!!"
        
        if setters:
            #we found member function that could serve as setter. Py++ did not find it
            #so there is a difference in cases.
            return None
        return decl_wrappers.property_t( self.find_out_property_name( fget, found ), fget )
