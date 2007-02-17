#ifndef generators_hpp_12_feb_2007
#define generators_hpp_12_feb_2007

#include "boost/python.hpp"
#include "OgreIteratorWrappers.h"

namespace generators{
    
template< typename VectorIterator >

struct generator_maker_vector{
        
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
        typedef generator_maker_vector< VectorIterator > maker_type;
        
        py_cls.def( "__iter__", &maker_type::iter, boost::python::return_self<>() );
        py_cls.def( "next", &maker_type::next, TNextCallPolicies() );
    }
    
};

// challenge here is the there isn't anyway to get the key if you do a python style iterator
// so I'm going to have it return a tuple instead

template< typename MapIterator >



struct generator_maker_map{
        
    typedef BOOST_DEDUCED_TYPENAME MapIterator::MappedType mapped_type;

    static void iter( const MapIterator& ){
    } //return_self call policies should be used

        
    static boost::python::tuple next( MapIterator& iter ){
        if( !iter.hasMoreElements() ){
            boost::python::objects::stop_iteration_error();
            //will not come here
        }
        return boost::python::make_tuple (
                    boost::python::str (iter.peekNextKey())
                   ,boost::python::str (iter.getNext())
                   );
//        return iter.getNext();
    }
    
    
    template< typename TNextCallPolicies, typename TPyClass>
    static void register_( TPyClass& py_cls ){
        typedef generator_maker_map< MapIterator > maker_type_map;
        
        py_cls.def( "__iter__", &maker_type_map::iter, boost::python::return_self<>() );
        py_cls.def( "next", &maker_type_map::next ); // , TNextCallPolicies() );
    }
    
};
    
}

#endif//generators_hpp_12_feb_2007
