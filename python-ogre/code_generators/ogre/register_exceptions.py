import os
import environment
from string import Template

base_exception = \
"""
class OgreException(Exception):
    def __init__( self, app_error ):
        Exception.__init__( self )
        self._pimpl = app_error
    
    def __str__( self ):
        return self._pimpl.getFullDescription()

    def __getattribute__(self, attr):
        my_pimpl = super(OgreException, self).__getattribute__("_pimpl")
        try:
            return getattr(my_pimpl, attr)
        except AttributeError:
            return super(OgreException,self).__getattribute__(attr)

_ogre_.OgreException = OgreException
#this one for backward compatability
_ogre_.Exception = OgreException
""".strip()

derived_py_exception = \
"""
class Ogre${err_class}(OgreException):
    def __init__( self, app_error ):
        OgreException.__init__( self, app_error )
    
    def __getattribute__(self, attr):
        return super(Ogre${err_class},self).__getattribute__(attr)

_ogre_._${err_class}_.py_err_class = Ogre${err_class}
""".strip()

register_translator = """${err_class}_exception_translator();"""

exception_translator = \
"""

struct ${err_class}_exception_translator{
    
    typedef Ogre::${err_class} exception_type;
    typedef ${err_class}_exception_translator translator_type;
    
    ${err_class}_exception_translator(){
        bp::converter::registry::push_back( &translator_type::convertible
                                            , &translator_type::construct
                                            , bp::type_id<exception_type>() );
        
        bp::register_exception_translator<exception_type>(&translator_type::translate);
    }
    
    static void  
    translate( const exception_type& err ){ 
        bp::object pimpl_err( err ); 
        bp::object pyerr_class = pimpl_err.attr( "py_err_class" ); 
        bp::object pyerr = pyerr_class( pimpl_err ); 
        PyErr_SetObject( pyerr_class.ptr(), bp::incref( pyerr.ptr() ) ); 
    }

    //Sometimes, exceptions should be passed back to the library.
    static void*
    convertible(PyObject* py_obj){
        if( 1 != PyObject_IsInstance( py_obj, PyExc_Exception ) ){
            return 0;
        }
        
        if( !PyObject_HasAttrString( py_obj, "_pimpl" ) ){
            return 0;
        }
        
        bp::object pyerr( bp::handle<>( bp::borrowed( py_obj ) ) );        
        bp::object pimpl = bp::getattr( pyerr, "_pimpl" );
        bp::extract<exception_type> type_checker( pimpl );
        if( !type_checker.check() ){
            return 0;
        }
        return py_obj;
    }

    static void
    construct( PyObject* py_obj, bp::converter::rvalue_from_python_stage1_data* data){
        typedef bp::converter::rvalue_from_python_storage<exception_type> storage_t;
        
        bp::object pyerr( bp::handle<>( bp::borrowed( py_obj ) ) );        
        bp::object pimpl = bp::getattr( pyerr, "_pimpl" );
        
        storage_t* the_storage = reinterpret_cast<storage_t*>( data );
        void* memory_chunk = the_storage->storage.bytes;
        exception_type* cpp_err 
            = new (memory_chunk) exception_type( bp::extract<exception_type>(pimpl) );
        
        data->convertible = memory_chunk;
    }

};
""".strip()

def substitute( text, **keywd ):
    return Template( text ).substitute( **keywd )
    
def register( mb ):
    ogre_ns = mb.namespace( 'Ogre' )
    python_part_code = ['import _ogre_']
    
    exception = ogre_ns.class_( 'Exception' )
    exception.include()
    exception.alias = '_%s_' % exception.name
    exception.add_declaration_code( substitute( exception_translator, err_class=exception.name ) )
    exception.add_registration_code( substitute( register_translator, err_class=exception.name ), False )
    python_part_code.append( base_exception )
    
    for derived_info in exception.derived:
        derived = derived_info.related_class
        derived.include()
        derived.alias = '_%s_' % derived.name
        derived.add_declaration_code( substitute( exception_translator, err_class=derived.name ) )
        derived.add_registration_code( substitute( register_translator, err_class=derived.name ), False )
        python_part_code.append( substitute( derived_py_exception, err_class=derived.name ) )

    ogre_exceptions = os.path.join( environment.ogre.generated_dir, '_ogre_exceptions_.py' )
    f = file( ogre_exceptions, 'w+' )
    f.write( os.linesep.join( python_part_code ) )
    f.close()
