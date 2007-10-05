import os
import environment


WRAPPER_DEFINITION_Sun = \
"""
caelum::EarthSunPositionModel * 
Sun_castAsEarthSunPositionModel ( caelum::SunPositionModel * n ) {
    return ( (caelum::EarthSunPositionModel* ) n );
    }
caelum::SimpleSunPositionModel * 
Sun_castAsSimpleSunPositionModel ( caelum::SunPositionModel * n ) {
    return ( (caelum::SimpleSunPositionModel* ) n );
    }


caelum::SunPositionModel * Sun_castSunPositionModel(caelum::SunPositionModel * n){
    if( dynamic_cast< caelum::EarthSunPositionModel * >( n ) ){
        return  (caelum::EarthSunPositionModel*) n ;
    }    
    if( dynamic_cast< caelum::SimpleSunPositionModel * >( n ) ){
        return  (caelum::SimpleSunPositionModel*)n  ;
    }    
    return ( n );  
}


caelum::SunPositionModel * Sun_getSunPositionModel(caelum::Sun& me){
    return Sun_castSunPositionModel( me.getSunPositionModel() );
}

caelum::SunPositionModel * Sun_setSunPositionModel(caelum::Sun& me, caelum::SunPositionModel* other){
    return Sun_castSunPositionModel( me.setSunPositionModel(other) );
}
"""

WRAPPER_REGISTRATION_Sun = [
    'def( "getSunPositionModel", &::Sun_getSunPositionModel,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    'def( "setSunPositionModel", &::Sun_setSunPositionModel,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());'
    ]

    
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'Sun' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Sun )
    apply_reg (rt,  WRAPPER_REGISTRATION_Sun )

