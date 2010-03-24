import os
import environment



#################################################################################################
#################################################################################################

def iter_as_generator_vector( cls ):
    print "ITER:", cls.name

    try:
        code = os.linesep.join([
                'typedef %(cls)s iter_type;'
                , 'generators::generator_maker_vector< iter_type >::register_< %(call_policies)s >( %(exposer_name)s );'])
        cls.add_registration_code(
                code % { 'cls' : cls.decl_string
                         , 'call_policies' : cls.mem_fun( 'getNext' ).call_policies.create_type()
                         , 'exposer_name' : cls.class_var_name }
                , works_on_instance=False )
        cls.include_files.append( 'generators.h' )
        print "OK"
    except:
        print "FAILED "

def iter_as_generator_map( cls ):
    print "ITER:", cls.name

    code = os.linesep.join([
            'typedef %(cls)s iter_type;'
            , 'generators::generator_maker_map< iter_type >::register_< %(call_policies)s >( %(exposer_name)s );'])
    try:
        cls.add_registration_code(
                code % { 'cls' : cls.decl_string
                         , 'call_policies' : cls.mem_fun( 'getNext' ).call_policies.create_type()
                         , 'exposer_name' : cls.class_var_name }
                , works_on_instance=False )
        cls.include_files.append( 'generators.h' )
        print "OK"
    except:
        print "FAILED"

#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    vec_iterators = mb.classes( lambda cls: cls.name.startswith( 'VectorIteratorWrapper<' ) )
    for cls in vec_iterators:
        iter_as_generator_vector( cls )

    map_iterators = mb.classes( lambda cls: cls.name.startswith( 'MapIteratorWrapper<' ) )
    for cls in map_iterators:
        iter_as_generator_map( cls )
        
    try:
        vec_iterators = mb.classes( lambda cls: cls.name.startswith( 'ConstVectorIteratorWrapper<' ) )
        for cls in vec_iterators:
            iter_as_generator_vector( cls )
    except:
        pass
    try:
        map_iterators = mb.classes( lambda cls: cls.name.startswith( 'ConstMapIteratorWrapper<' ) )
        for cls in map_iterators:
            iter_as_generator_map( cls )
    except:
        pass
        

