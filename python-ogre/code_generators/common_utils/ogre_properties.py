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
        self.extendedtypes=['Vector','ColourValue', 'Quaternion', 'Matrix']
        
    def addSetterType ( self, type_ ):
        self.extendedtypes.append ( type_ )
        
    def __get_accessors( self, mem_funs ):
        getters = []
        setters = []
        for mem_fun in mem_funs:
#            print "Checking", mem_fun
            if not self.is_accessor( mem_fun ):
#                 print "NOT ACCESSOR"
                continue 
            elif self.is_getter( mem_fun ):
 #               print "IS GETTER"
                getters.append( mem_fun )
            elif self.is_setter( mem_fun ):
#                print "IS SETTER"
                setters.append( mem_fun )
            else:
#                 print "CONTINUE"
                continue
        return ( getters, setters ) 
               
    def check_type_compatibility( self, fget, fset ):
#        print "*** IN check_type_compatibility", fset
        if decl_wrappers.name_based_recognizer_t.check_type_compatibility( self, fget, fset ):
#            print "TRUE1"
            return True
#        print fset.arguments[0].type.decl_string    
        t2 = fset.arguments[0].type
        for t in self.extendedtypes:
            if t in t2.decl_string:
#                print "TRUE2"
                return True
#        print "FALSE"
        return False
            
    def is_setter( self, mem_fun ):
#        print "IN IS_SETTER", mem_fun
        if len( mem_fun.arguments ) != 1:
            return False
        if not declarations.is_void( mem_fun.return_type ):
            return False
        if mem_fun.has_const:
            return False
#        print "RETURNING TRUE"    
        return True
        
    def create_property( self, fget, fset ):
#        print "**** IN Create Property"
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
#                 print"Overloaded Property", fget.name, fset.name, f.name
                return None
        return decl_wrappers.property_t( self.find_out_property_name( fget, found[0] ), fget, fset )         
            
    def create_read_only_property( self, fget ):
        found = self.find_out_ro_prefixes( fget.name )
#         print "Prop Check", fget.name, ' ',
        if found in ( None, '' ):
#             print "NO"
            return None
#         print "YES"            
        ## Now we need to look for overlapping function declarations
        pure_names = []
        pure_names.append( fget.name[len( found ):].lower())
        funcs =  self.class_accessors( fget.parent )[1] 
        funcs.extend( self.inherited_accessors( fget.parent )[1] )
        for f in funcs:
            if f.name.lower() in pure_names:
#                 print"Overloaded Read Only Property", fget.name, f.name
                return None
        return decl_wrappers.property_t( self.find_out_property_name( fget, found ), fget )
