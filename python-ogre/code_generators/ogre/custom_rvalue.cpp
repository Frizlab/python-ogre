#include "python_ogre_precompiled.h"
#include "boost/python/object.hpp"  //len function
#include "boost/python/ssize_t.hpp" //ssize_t type definition
#include "boost/python/detail/none.hpp"
#include "tuples.hpp"

namespace bpl = boost::python;

namespace r_values_impl{

struct PyTuple2ColourValue{

    typedef boost::tuples::tuple< float, float, float> rgb_type;
    typedef boost::tuples::tuple< float, float, float, float> rgba_type;

    typedef bpl::from_py_sequence< rgb_type > rgb_converter_type;
    typedef bpl::from_py_sequence< rgba_type > rgba_converter_type;

    typedef Ogre::ColourValue colour_t;

    static void* convertible(PyObject* obj){
        if( rgb_converter_type::convertible( obj ) || rgba_converter_type::convertible( obj ) ){
            return obj;
        }
        else{
            return 0;
        }
    }

    static void
    construct( PyObject* obj, bpl::converter::rvalue_from_python_stage1_data* data){

        typedef bpl::converter::rvalue_from_python_storage<colour_t> colour_storage_t;
        colour_storage_t* the_storage = reinterpret_cast<colour_storage_t*>( data );
        void* memory_chunk = the_storage->storage.bytes;

        float red(1.0), green(1.0), blue(1.0), alpha(1.0);
        bpl::tuple py_tuple( bpl::handle<>( bpl::borrowed( obj ) ) );
        if( 3 == bpl::len( py_tuple ) ){
            boost::tuples::tie(red, green, blue)
                = rgb_converter_type::to_c_tuple( obj );
        }
        else{
            boost::tuples::tie(red, green, blue, alpha) 
                = rgba_converter_type::to_c_tuple( obj );
        }

        colour_t* colour = new (memory_chunk) colour_t(red, green, blue, alpha);
        data->convertible = memory_chunk;
    }
};

typedef Ogre::Real real_type;

struct PyTuple2Vector2{

    typedef Ogre::Vector2 vector_type;

    typedef boost::tuples::tuple< real_type, real_type> vector_tuple_type;

    typedef bpl::from_py_sequence< vector_tuple_type > converter_type;

    static void* convertible(PyObject* obj){
        return converter_type::convertible( obj );
    }

    static void
    construct( PyObject* obj, bpl::converter::rvalue_from_python_stage1_data* data){

        typedef bpl::converter::rvalue_from_python_storage<vector_type> vector_storage_t;
        vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data );
        void* memory_chunk = the_storage->storage.bytes;

        real_type x(0.0), y(0.0);
        boost::tuples::tie(x, y) = converter_type::to_c_tuple( obj );

        vector_type* vec = new (memory_chunk) vector_type(x,y);
        data->convertible = memory_chunk;
    }
};

struct PyTuple2Vector3{

    typedef Ogre::Vector3 vector_type;

    typedef boost::tuples::tuple< real_type, real_type, real_type> vector_tuple_type;

    typedef bpl::from_py_sequence< vector_tuple_type > converter_type;

    static void* convertible(PyObject* obj){
        return converter_type::convertible( obj );
    }

    static void
    construct( PyObject* obj, bpl::converter::rvalue_from_python_stage1_data* data){

        typedef bpl::converter::rvalue_from_python_storage<vector_type> vector_storage_t;
        vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data );
        void* memory_chunk = the_storage->storage.bytes;

        real_type x(0.0), y(0.0), z(0.0);
        boost::tuples::tie(x, y, z) = converter_type::to_c_tuple( obj );

        vector_type* vec = new (memory_chunk) vector_type(x, y, z);
        data->convertible = memory_chunk;
    }
};


struct PyTuple2Vector4{

    typedef Ogre::Vector4 vector_type;

    typedef boost::tuples::tuple< real_type, real_type, real_type, real_type> vector_tuple_type;

    typedef bpl::from_py_sequence< vector_tuple_type > converter_type;

    static void* convertible(PyObject* obj){
        return converter_type::convertible( obj );
    }

    static void
    construct( PyObject* obj, bpl::converter::rvalue_from_python_stage1_data* data){

        typedef bpl::converter::rvalue_from_python_storage<vector_type> vector_storage_t;
        vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data );
        void* memory_chunk = the_storage->storage.bytes;

        real_type x(0.0), y(0.0), z(0.0), w(0.0);
        boost::tuples::tie(x, y, z, w) = converter_type::to_c_tuple( obj );

        vector_type* vec = new (memory_chunk) vector_type(x, y, z, w);
        data->convertible = memory_chunk;
    }
};


struct PyStringToUTFString{

    typedef Ogre::UTFString string_type;

    static void* convertible(PyObject* obj){
        if( PyString_Check(obj) || PyUnicode_Check(obj) ){
            return obj;
        }
        else{
            return 0;
        }
    }

    static void
    construct( PyObject* obj, bpl::converter::rvalue_from_python_stage1_data* data){
        typedef bpl::converter::rvalue_from_python_storage<string_type> string_storage_t;
        string_storage_t* the_storage = reinterpret_cast<string_storage_t*>( data );
        void* memory_chunk = the_storage->storage.bytes;

        bpl::object py_str( bpl::handle<>( bpl::borrowed( obj ) ) );                
        if( PyString_Check(obj) ){        
            std::string c_str = bpl::extract<std::string>( py_str );
            new (memory_chunk) string_type(c_str);
        }
        else{ //unicode
            std::wstring c_str = bpl::extract<std::wstring>( py_str );
            new (memory_chunk) string_type(c_str);
        }
        data->convertible = memory_chunk;        
    }
};

struct PyTuple2Quaternion{

    typedef Ogre::Quaternion quat_type;

    typedef boost::tuples::tuple< real_type, real_type, real_type, real_type> quat_tuple_type;

    typedef bpl::from_py_sequence< quat_tuple_type > converter_type;

    static void* convertible(PyObject* obj){
        return converter_type::convertible( obj );
    }

    static void
    construct( PyObject* obj, bpl::converter::rvalue_from_python_stage1_data* data){

        typedef bpl::converter::rvalue_from_python_storage<quat_type> quat_storage_t;
        quat_storage_t* the_storage = reinterpret_cast<quat_storage_t*>( data );
        void* memory_chunk = the_storage->storage.bytes;

        real_type fW(0.0), fX(0.0), fY(0.0), fZ(0.0);
        boost::tuples::tie(fW, fX, fY, fZ) = converter_type::to_c_tuple( obj );

        quat_type* quat = new (memory_chunk) quat_type(fW, fX, fY, fZ);
        data->convertible = memory_chunk;
    }
};

struct PyTuple2Matrix3{

    typedef Ogre::Matrix3 matrix_type;

    typedef boost::tuples::tuple< real_type, real_type, real_type, 
                                  real_type, real_type, real_type,
                                  real_type, real_type, real_type> matrix_tuple_type;

    typedef bpl::from_py_sequence< matrix_tuple_type > converter_type;

    static void* convertible(PyObject* obj){
        return converter_type::convertible( obj );
    }

    static void
    construct( PyObject* obj, bpl::converter::rvalue_from_python_stage1_data* data){

        typedef bpl::converter::rvalue_from_python_storage<matrix_type> matrix_storage_t;
        matrix_storage_t* the_storage = reinterpret_cast<matrix_storage_t*>( data );
        void* memory_chunk = the_storage->storage.bytes;

        real_type f0(0.0), f1(0.0), f2(0.0),
                  f10(0.0), f11(0.0), f12(0.0),
                  f20(0.0), f21(0.0), f22(0.0);
        boost::tuples::tie(f0,f1,f2,f10,f11,f12,f20,f21,f22) = converter_type::to_c_tuple( obj );

        matrix_type* vec = new (memory_chunk) matrix_type(f0,f1,f2,f10,f11,f12,f20,f21,f22);
        data->convertible = memory_chunk;
    }
};
    
// // struct PyTuple2Matrix4{

// //     typedef Ogre::Matrix4 matrix_type;
// // /// ### TODO -- boost/boost/tuple/detail/tuple_basic.hpp needs to be extended to take 16 arguments 
// // /// ### currently it's max is 10 :(
// //     typedef boost::tuples::tuple< real_type, real_type, real_type, real_type,
// //                                   real_type, real_type, real_type, real_type,
// //                                   real_type, real_type, real_type, real_type,
// //                                   real_type, real_type, real_type, real_type> matrix_tuple_type;

// //     typedef bpl::from_py_sequence< matrix_tuple_type > converter_type;

// //     static void* convertible(PyObject* obj){
// //         return converter_type::convertible( obj );
// //     }

// //     static void
// //     construct( PyObject* obj, bpl::converter::rvalue_from_python_stage1_data* data){

// //         typedef bpl::converter::rvalue_from_python_storage<matrix_type> matrix_storage_t;
// //         matrix_storage_t* the_storage = reinterpret_cast<matrix_storage_t*>( data );
// //         void* memory_chunk = the_storage->storage.bytes;

// //         real_type f0(0.0), f1(0.0), f2(0.0), f3(0.0),
// //                   f10(0.0), f11(0.0), f12(0.0), f13(0.0),
// //                   f20(0.0), f21(0.0), f22(0.0), f23(0.0),
// //                   f30(0.0), f31(0.0), f32(0.0), f33(0.0;
// //         boost::tuples::tie(f0,f1,f2,f3,f10,f11,f12,f13,
// //                         f20,f21,f22,f23,f30,f31,f32,f33) = converter_type::to_c_tuple( obj );

// //         matrix_type* vec = new (memory_chunk) matrix_type(f0,f1,f2,f3,f10,f11,f12,f13,
// //                                                         f20,f21,f22,f23,f30,f31,f32,f33);
// //         data->convertible = memory_chunk;
// //     }
// //     
// // };

} //r_values_impl


void register_pytuple_to_colour_value_conversion(){
    bpl::converter::registry::push_back(  &r_values_impl::PyTuple2ColourValue::convertible
                                         , &r_values_impl::PyTuple2ColourValue::construct
                                         , bpl::type_id<Ogre::ColourValue>() );
}

void register_pytuple_to_vector2_conversion(){
    bpl::converter::registry::push_back(  &r_values_impl::PyTuple2Vector2::convertible
                                         , &r_values_impl::PyTuple2Vector2::construct
                                         , bpl::type_id<Ogre::Vector2>() );
}

void register_pytuple_to_vector3_conversion(){
    bpl::converter::registry::push_back(  &r_values_impl::PyTuple2Vector3::convertible
                                         , &r_values_impl::PyTuple2Vector3::construct
                                         , bpl::type_id<Ogre::Vector3>() );
}

void register_pytuple_to_vector4_conversion(){
    bpl::converter::registry::push_back(  &r_values_impl::PyTuple2Vector4::convertible
                                         , &r_values_impl::PyTuple2Vector4::construct
                                         , bpl::type_id<Ogre::Vector4>() );
}

void register_pystring_to_utfstring_conversion(){
    bpl::converter::registry::push_back(  &r_values_impl::PyStringToUTFString::convertible
                                         , &r_values_impl::PyStringToUTFString::construct
                                         , bpl::type_id<Ogre::UTFString>() );
}
void register_pytuple_to_quaternion_conversion(){
    bpl::converter::registry::push_back(  &r_values_impl::PyTuple2Quaternion::convertible
                                         , &r_values_impl::PyTuple2Quaternion::construct
                                         , bpl::type_id<Ogre::Quaternion>() );
}

void register_pytuple_to_matrix3_conversion(){
    bpl::converter::registry::push_back(  &r_values_impl::PyTuple2Matrix3::convertible
                                         , &r_values_impl::PyTuple2Matrix3::construct
                                         , bpl::type_id<Ogre::Matrix3>() );
}
// // void register_pytuple_to_matrix4_conversion(){
// //     bpl::converter::registry::push_back(  &r_values_impl::PyTuple2Matrix4::convertible
// //                                          , &r_values_impl::PyTuple2Matrix4::construct
// //                                          , bpl::type_id<Ogre::Matrix4>() );
// // }