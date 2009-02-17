import os
import string

from pyplusplus.function_transformers import transformer as transformer
from pyplusplus.function_transformers import controllers as controllers

from pygccxml import declarations
from pyplusplus import code_repository

def is_ref_or_ptr( type_ ):
    return declarations.is_pointer( type_ ) or declarations.is_reference( type_ )

def is_ptr_or_array( type_ ):
    return declarations.is_pointer( type_ ) or declarations.is_array( type_ )

def remove_ref_or_ptr( type_ ):
    if declarations.is_pointer( type_ ):
        return declarations.remove_pointer( type_ )
    elif declarations.is_reference( type_ ):
        return declarations.remove_reference( type_ )
    else:
        raise TypeError( 'Type should be reference or pointer, got %s.' % type_ )

_seq2arr = string.Template( os.linesep.join([
              'int size_$pylist = len ( $pylist );'
              'pyplus_conv::ensure_uniform_sequence< $type >( $pylist, size_$array_size );'
            , 'pyplus_conv::copy_sequence( $pylist, pyplus_conv::array_inserter( $native_array, size_$array_size ) );']))

_seq2vector = string.Template( os.linesep.join([
                 'pyplus_conv::ensure_uniform_sequence< $type >( $pylist );'
               , 'pyplus_conv::copy_sequence( $pylist, std::back_inserter( $native_array), boost::type< $type >() );']))

_arr2seq = string.Template(
            'pyplus_conv::copy_container( $native_array, $native_array + $array_size, pyplus_conv::list_inserter( $pylist ) );' )
        
class input_c_string_t(transformer.transformer_t):
    """Handles an input array with fixed size.

    void do_something(double* v) ->  do_something(object v2)

    where v2 is a Python sequence
    """

    def __init__(self, function, arg_ref, size):
        """Constructor.

        :param size: The fixed size of the input array
        :type size: int
        """
        transformer.transformer_t.__init__( self, function )

        self.arg = self.get_argument( arg_ref )
        self.arg_index = self.function.arguments.index( self.arg )

        if not is_ptr_or_array( self.arg.type ):
            raise ValueError( '%s\nin order to use "input_array" transformation, argument %s type must be a array or a pointer (got %s).' ) \
                  % ( function, self.arg.name, self.arg.type)

        self.array_size = size
        self.array_item_type = declarations.remove_const( declarations.array_item_type( self.arg.type ) )
        self.array_item_rawtype = declarations.remove_const( self.arg.type )

    def __str__(self):
        return "input_array(%s,%d)"%( self.arg.name, self.array_size)

    def required_headers( self ):
        """Returns list of header files that transformer generated code depends on."""
        return [ code_repository.convenience.file_name ]

    def __configure_sealed(self, controller):
        global _seq2arr
        w_arg = controller.find_wrapper_arg( self.arg.name )
        w_arg.type = declarations.dummy_type_t( "boost::python::object" )

        # Declare a variable that will hold the C array...
        native_array = controller.declare_variable( self.array_item_rawtype
                                                    , "native_" + self.arg.name
                                                    , '' )

        copy_pylist2arr = _seq2arr.substitute( type=self.array_item_type
                                                , pylist=w_arg.name
                                                , array_size=self.array_size
                                                , native_array=native_array )

        controller.add_pre_call_code( copy_pylist2arr )

        controller.modify_arg_expression( self.arg_index, native_array )

    def __configure_v_mem_fun_default( self, controller ):
        self.__configure_sealed( controller )

    def __configure_v_mem_fun_override( self, controller ):
        global _arr2seq
        pylist = controller.declare_py_variable( declarations.dummy_type_t( 'boost::python::list' )
                                                 , 'py_' + self.arg.name )

        copy_arr2pylist = _arr2seq.substitute( native_array=self.arg.name
                                                , array_size=self.array_size
                                                , pylist=pylist )

        controller.add_py_pre_call_code( copy_arr2pylist )

    def configure_mem_fun( self, controller ):
        self.__configure_sealed( controller )

    def configure_free_fun(self, controller ):
        self.__configure_sealed( controller )

    def configure_virtual_mem_fun( self, controller ):
        self.__configure_v_mem_fun_override( controller.override_controller )
        self.__configure_v_mem_fun_default( controller.default_controller )

        
