import os
import environment

WRAPPER_DEFINITION_dBody=\
"""
::dReal
dBody_getPosRelPoint ( ::dBody & body, ::dReal x,::dReal y,::dReal z ) {
    static ::dReal returnValue;
    body.getPosRelPoint( x, y, z, &returnValue);
    return returnValue;
}

::dReal
dBody_getRelPointPos ( ::dBody & body, ::dReal x,::dReal y,::dReal z ) {
    static ::dReal returnValue;
    body.getRelPointPos( x, y, z, &returnValue);
    return returnValue;
}

::dReal
dBody_getPointVel ( ::dBody & body, ::dReal x,::dReal y,::dReal z ) {
    static ::dReal returnValue;
    body.getPointVel( x, y, z, &returnValue);
    return returnValue;
}

::dReal
dBody_vectorFromWorld ( ::dBody & body, ::dReal x,::dReal y,::dReal z ) {
    static ::dReal returnValue;
    body.vectorFromWorld( x, y, z, &returnValue);
    return returnValue;
}

::dReal
dBody_vectorToWorld ( ::dBody & body, ::dReal x,::dReal y,::dReal z ) {
    static ::dReal returnValue;
    body.vectorToWorld( x, y, z, &returnValue);
    return returnValue;
}
::dReal
dBody_getRelPointVel ( ::dBody & body, ::dReal x,::dReal y,::dReal z ) {
    static ::dReal returnValue;
    body.getRelPointVel( x, y, z, &returnValue);
    return returnValue;
}
        
::dReal
dBody_getFiniteRotationAxis ( ::dBody & body ) {
    static ::dReal returnValue;
    body.getFiniteRotationAxis( &returnValue);
    return returnValue;
}
"""        
            
# WRAPPER_DEFINITION_RenderWindow = \
# """
# boost::python::tuple 
# RenderWindow_getMetricsIntWrapper( Ogre::RenderWindow& rw ) {
#    unsigned int width, height, colourDepth;
#    int left, top;
#    rw.getMetrics(  width,  height, colourDepth,  left,  top );
#    return ( boost::python::make_tuple(width, height, colourDepth, left, top));
# }
# """


WRAPPER_REGISTRATION_dBody = \
"""
    def( "getPosRelPoint", &::dBody_getPosRelPoint );
    dBody_exposer.def( "getRelPointPos", &::dBody_getRelPointPos );
    dBody_exposer.def( "getPointVel", &::dBody_getPointVel );
    dBody_exposer.def( "getRelPointVel", &::dBody_getRelPointVel );
    dBody_exposer.def( "getFiniteRotationAxis", &::dBody_getFiniteRotationAxis );
    dBody_exposer.def( "vectorFromWorld", &::dBody_vectorFromWorld );
    dBody_exposer.def( "vectorToWorld", &::dBody_vectorToWorld );
"""

# WRAPPER_REGISTRATION_General =\
# """
# bp::def ("CreateBody", &CreateBody, 
#         bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
# # bp::def ("CreateBodyWithID", &CreateBodyWithID, 
# #         bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
# # bp::def ("GetID", &GetID, 
# #         bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >());
# """
def apply( mb ):
#     pass
    cs = mb.class_( 'dBody' )

    cs.add_declaration_code( WRAPPER_DEFINITION_dBody )
    cs.add_registration_code( WRAPPER_REGISTRATION_dBody )
    