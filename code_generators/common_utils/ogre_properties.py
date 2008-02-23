from pygccxml import parser
from pygccxml import declarations
from pyplusplus import module_builder

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
from pygccxml import declarations
from pyplusplus import decl_wrappers

#
# class to add properties to ogre.  It follows a 'simple' set of rules
# 1. Find all functions that have a standard set of prefixes as defined in Py++
#      Currently this list is 'get', 'is'
# 2. remove the prefix
# 3. Do a case insensitive search on the non-prefixed name against all other functions 
#         ie. if original function is getPosition
#         We check 'position' against all other (lowercased) functions
#         so if 'position', or 'Position', or 'posItion' already exists we DO NOT add the property
#       This is intentionally agressive in NOT adding properties as we don't want any confusion :)
# 4. If there is NOT a conflict we can add a property



class ogre_property_recognizer_t( decl_wrappers.name_based_recognizer_t ):
    def __init__( self ):
        decl_wrappers.name_based_recognizer_t.__init__( self )
        
    def check_type_compatibility( self, fget, fset ):
        extendedtypes=['Vector','ColourValue', 'Quaternion', 'Matrix']
        if decl_wrappers.name_based_recognizer_t.check_type_compatibility( self, fget, fset ):
            return True
        t2 = fset.arguments[0].type
        for t in extendedtypes:
            if t in t2.decl_string:
                return True
        return False
            
    def is_setter( self, mem_fun ):
        if len( mem_fun.arguments ) != 1:
            return False
        if not declarations.is_void( mem_fun.return_type ):
            return False
        if mem_fun.has_const:
            return False
        return True
        
    def create_property( self, fget, fset ):
        if not self.check_type_compatibility( fget, fset ):
            return None
        found = self.find_out_prefixes( fget.name, fset.name )
        if not found:
            return None
        ## Now we need to look for overlapping function declarations
        pure_names = []
        pure_names.append( fget.name[len( found[0] ):].lower())
        pure_names.append( fset.name[len( found[1] ):].lower())
        funcs =  self.class_accessors( fget.parent )[1] 
        funcs.extend( self.inherited_accessors( fget.parent )[1] )
        for f in funcs:
            if f.name.lower() in pure_names:
                print"Overloaded Property", fget.name, fset.name, f.name
                return None
        return decl_wrappers.property_t( self.find_out_property_name( fget, found[0] ), fget, fset )         
            
    def create_read_only_property( self, fget ):
        found = self.find_out_ro_prefixes( fget.name )
        if found in ( None, '' ):
            return None
        ## Now we need to look for overlapping function declarations
        pure_names = []
        pure_names.append( fget.name[len( found ):].lower())
        funcs =  self.class_accessors( fget.parent )[1] 
        funcs.extend( self.inherited_accessors( fget.parent )[1] )
        for f in funcs:
            if f.name.lower() in pure_names:
                print"Overloaded Read Only Property", fget.name, f.name
                return None
        return decl_wrappers.property_t( self.find_out_property_name( fget, found ), fget )
