import os
import environment



WRAPPER_DEFINITION_CustomQueryResult =\
"""
    bp::tuple  CustomQueryResult_getQueryExtents( Ogre::PagingLandScapeAxisAlignedBoxSceneQuery::CustomQueryResult* me ) 
    { 
        return ( boost::python::make_tuple( me->data_.queryExtents_.width_, me->data_.queryExtents_.height_ ) );
    }
    bp::tuple  CustomQueryResult_getSubExtents( Ogre::PagingLandScapeAxisAlignedBoxSceneQuery::CustomQueryResult* me ) 
    {
        return ( boost::python::make_tuple( me->data_.subExtents_.width_, me->data_.subExtents_.height_,me->data_.subExtents_.renderLevel_ ) );
    }
    bp::tuple  CustomQueryResult_getQueryVertexData( Ogre::PagingLandScapeAxisAlignedBoxSceneQuery::CustomQueryResult* me ) 
    {
        return ( boost::python::make_tuple( me->data_.vertexData_.x_, me->data_.vertexData_.y_,me->data_.vertexData_.z_,me->data_.vertexData_.included_) );
    }
"""
WRAPPER_DEFINITION_General =\
"""
Ogre::PagingLandScapeAxisAlignedBoxSceneQuery::CustomQueryResult * General_castAsCustomQuery( Ogre::SceneQuery::WorldFragment* fragment){
       return reinterpret_cast<Ogre::PagingLandScapeAxisAlignedBoxSceneQuery::CustomQueryResult*>( fragment->geometry );
    }
    Ogre::PagingLandScapeOctreeCamera * General_castAsPLSMCamera( Ogre::Camera *in ){  
           return (Ogre::PagingLandScapeOctreeCamera*) in;  
        }
    Ogre::PagingLandScapeSceneManager * General_castAsPLSMSceneManager( Ogre::SceneManager *in ){
           return (Ogre::PagingLandScapeSceneManager*) in;  
        }
    
"""


WRAPPER_REGISTRATION_CustomQueryResult = [
    """def( "getQueryExtents", &CustomQueryResult_getQueryExtents ,
    "Python-Ogre Helper Function: returns scene query extents\\n\\
    Input: None\\n\\
    Ouput: Query Extents tuple (width_, height_)" );""",
    """def( "getQueryVertexData", &CustomQueryResult_getQueryVertexData ,
    "Python-Ogre Helper Function: returns scene query vertex data\\n\\
    Input: None\\n\\
    Ouput: Query Extents tuple (x_,y_,z_,included_)" );""",
    """def( "getSubExtents", &CustomQueryResult_getSubExtents ,
    "Python-Ogre Helper Function: returns scene query sub extents\\n\\
    Input: None\\n\\
    Ouput: Query Extents tuple (width_, height_,renderLevel_)" );"""
    ] 
WRAPPER_REGISTRATION_General = [
    """def( "castAsCustomQuery", &General_castAsCustomQuery,
                "Python-Ogre Helper Function: Casts a WorldFragment SceneQuery Result as a PagingLandscape CustomQuery.\\n\\
                Input: WorldFragment\\n\\
                Ouput: CustomQueryResult",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """def( "castAsCamera", &General_castAsPLSMCamera,
                "Python-Ogre Helper Function: Casts a PagingLandScapeOctreeCamera as a camera.\\n\\
                Input: None\\n\\
                Ouput: Camera",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """def( "castAsSceneManager", &General_castAsPLSMSceneManager,
                "Python-Ogre Helper Function: Casts a PagingLandScapeSceneManager as a SceneManager.\\n\\
                Input: None\\n\\
                Ouput: SceneManager",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());"""
    ]

    
    

#################################################################################################
WRAPPER_DEFINITION_PagingLandScapeSceneManager =\
"""
bool PagingLandScapeSceneManager_setOption( Ogre::PagingLandScapeSceneManager & me, const Ogre::String& strKey, bp::object pValue ) {

    bp::extract<Ogre::AxisAlignedBox> p( pValue );
    if( p.check() ){
        Ogre::AxisAlignedBox var = p();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<Ogre::Vector3> p3( pValue );
    if( p3.check() ){
        Ogre::Vector3 var = p3();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<Ogre::Matrix3> p4( pValue );
    if( p4.check() ){
        Ogre::Matrix3 var = p4();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<Ogre::Matrix4> p5( pValue );
    if( p5.check() ){
        Ogre::Matrix4 var = p5();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<Ogre::Quaternion> p6( pValue );
    if( p6.check() ){
        Ogre::Quaternion var = p6();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<int> p2( pValue );
    if( p2.check() ){
        int var = p2();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<bool> p1( pValue );
    if( p1.check() ){
        bool var = p1();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    /*
    bp::extract<Ogre::PagingLandScapeOctreeCamera> p7( pValue );
    if( p7.check() ){
        Ogre::PagingLandScapeOctreeCamera var = p7();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    */
    bp::extract<Ogre::String> p8( pValue );
    if( p8.check() ){
        Ogre::String var = p8();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    
    return false;
}


bp::tuple PagingLandScapeSceneManager_getOption( Ogre::PagingLandScapeSceneManager & me, const Ogre::String& strKey, bp::object returnType) { 

    bp::extract<Ogre::AxisAlignedBox> p( returnType );
    if( p.check() ){
        Ogre::AxisAlignedBox var = p();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<Ogre::Vector3> p3( returnType );
    if( p3.check() ){
        Ogre::Vector3 var = p3();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<Ogre::Matrix3> p4( returnType );
    if( p4.check() ){
        Ogre::Matrix3 var = p4();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<Ogre::Matrix4> p5( returnType );
    if( p5.check() ){
        Ogre::Matrix4 var = p5();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<Ogre::Quaternion> p6( returnType );
    if( p6.check() ){
        Ogre::Quaternion var = p6();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<int> p2( returnType );
    if( p2.check() ){
        int var = p2();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<bool> p1( returnType );
    if( p1.check() ){
        bool var = p1();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<Ogre::String> p7( returnType );
    if( p7.check() ){
        Ogre::String var = p7();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }

    return bp::make_tuple ( false, false ); 
}

"""

WRAPPER_REGISTRATION_PagingLandScapeSceneManager = [
    """def( "setOption", &::PagingLandScapeSceneManager_setOption ,
    "Python-Ogre Helper Function: sets an option in the SceneManager\\n\\
    Input: Option Name, Option Value\\n\\
    Ouput: True/False" );""",
    """def( "getOption", &::PagingLandScapeSceneManager_getOption ,
    "Python-Ogre Helper Function: returns an option from the PagingLandScapeSceneManager\\n\\
    Input: Option Name\\n\\
    Ouput: Tuple containing bool (True/False -- success of call) and value" );"""
    ] 

WRAPPER_DEFINITION_PagingLandScapeOptions =\
"""
bool PagingLandScapeOptions_setOption( Ogre::PagingLandScapeOptions & me, const Ogre::String& strKey, bp::object pValue ) {

    bp::extract<bool> p( pValue );
    if( p.check() ){
        bool var = p();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    // Ogre::Real to check
    bp::extract<float> p2( pValue );
    if( p2.check() ){
        bool var = p2();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<Ogre::Vector3> p3( pValue );
    if( p3.check() ){
        Ogre::Vector3 var = p3();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<Ogre::Vector2> p4( pValue );
    if( p4.check() ){
        Ogre::Vector2 var = p4();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    /*
    bp::extract<Ogre::PagingLandScapeOctreeCamera> p5( pValue );
    if( p5.check() ){
        Ogre::PagingLandScapeOctreeCamera var = p5();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    */
    bp::extract<Ogre::String> p6( pValue );
    if( p6.check() ){
        Ogre::String var = p6();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }

   
    bp::extract<int> p7( pValue );
    if( p7.check() ){
        int var = p7();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }

    return false;
}


bp::tuple PagingLandScapeOptions_getOption( Ogre::PagingLandScapeOptions & me, const Ogre::String& strKey, bp::object returnType) { 

    bp::extract<int> p( returnType );
    if( p.check() ){
        int var = p();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<bool> p1( returnType );
    if( p1.check() ){
        bool var = p1();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<float> p2( returnType );
    if( p2.check() ){
        float var = p2();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<Ogre::Vector3> p3( returnType );
    if( p3.check() ){
        Ogre::Vector3 var = p3();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    
    bp::extract<Ogre::String> p4( returnType );
    if( p4.check() ){
        Ogre::String var = p4();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }

    return bp::make_tuple ( false, false ); 
}

"""

WRAPPER_REGISTRATION_PagingLandScapeOptions = [
    """def( "setOption", &::PagingLandScapeOptions_setOption ,
    "Python-Ogre Helper Function: sets an option in the SceneManager\\n\\
    Input: Option Name, Option Value\\n\\
    Ouput: True/False" );""",
    """def( "getOption", &::PagingLandScapeOptions_getOption ,
    "Python-Ogre Helper Function: returns an option from the PagingLandScapeOptions\\n\\
    Input: Option Name\\n\\
    Ouput: Tuple containing bool (True/False -- success of call) and value" );"""
    ] 


    
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'CustomQueryResult' )
    rt.add_declaration_code( WRAPPER_DEFINITION_CustomQueryResult )
    apply_reg (rt,  WRAPPER_REGISTRATION_CustomQueryResult )

    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    apply_reg (mb,  WRAPPER_REGISTRATION_General )

    rt = mb.class_( 'PagingLandScapeSceneManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_PagingLandScapeSceneManager )
    apply_reg (rt,  WRAPPER_REGISTRATION_PagingLandScapeSceneManager)
    
    rt = mb.class_( 'PagingLandScapeOptions' )
    rt.add_declaration_code( WRAPPER_DEFINITION_PagingLandScapeOptions )
    apply_reg (rt,  WRAPPER_REGISTRATION_PagingLandScapeOptions )
    
