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
    
    def create_read_only_property( self, fget ):
        found = self.find_out_ro_prefixes( fget.name )
        if found in ( None, '' ):
            return None

        pure_name = fget.name[len( found ):]
        pure_name = pure_name.lower()

        setters = self.class_accessors( fget.parent )[1] 
        setters.extend( self.inherited_accessors( fget.parent )[1] )
        
        setters = filter( lambda decl: decl.name.lower() == pure_name and self.is_setter( decl )
                          , setters )
        if setters:
            #we found member function that could serve as setter. Py++ did not find it
            #so there is a difference in cases.
            return None
        return decl_wrappers.property_t( self.find_out_property_name( fget, found ), fget )
