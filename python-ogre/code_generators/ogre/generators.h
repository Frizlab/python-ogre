#ifndef generators_hpp_12_feb_2007
#define generators_hpp_12_feb_2007

#include "boost/python.hpp"
#include "OgreIteratorWrappers.h"

namespace generators{
    
template< typename VectorIterator >
struct generator_maker{
        
    typedef BOOST_DEDUCED_TYPENAME VectorIterator::ValueType value_type;
    
    static void iter( const VectorIterator& ){
    } //return_self call policies should be used
    
    static value_type next( VectorIterator& iter ){
        if( !iter.hasMoreElements() ){
            boost::python::objects::stop_iteration_error();
            //will not come here
        }
        return iter.getNext();
    }
    
    template< typename TNextCallPolicies, typename TPyClass>
    static void register_( TPyClass& py_cls ){
        typedef generator_maker< VectorIterator > maker_type;
        
        py_cls.def( "__iter__", &maker_type::iter, boost::python::return_self<>() );
        py_cls.def( "__next__", &maker_type::next, TNextCallPolicies() );
    }
    
};
    
}

#endif//generators_hpp_12_feb_2007
