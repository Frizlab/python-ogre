import os
import environment

WRAPPER_REGISTRATION_General=\
"""
    boost::python::def( "FMOD_System_Create", &::wrap_FMOD_System_Create,
         bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
    boost::python::def( "FMOD_System_Init", &::wrap_FMOD_System_Init );    
///        result = system->createSound("../media/drumloop.wav", FMOD_HARDWARE, 0, &sound1);
     
"""

WRAPPER_DEFINITION_General=\
"""
FMOD::System * wrap_FMOD_System_Create( void ) {
FMOD::System     *system;
FMOD_RESULT       result;
result = FMOD::System_Create(&system);
return system;
}

void wrap_FMOD_System_Init ( FMOD::System & me ) {

    me.init(32, FMOD_INIT_NORMAL, 0);
}    

"""

def apply( mb ):
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    mb.add_registration_code( WRAPPER_REGISTRATION_General )
    
