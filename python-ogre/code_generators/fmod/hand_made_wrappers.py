import os
import environment

WRAPPER_REGISTRATION_General=\
"""
    boost::python::def( "PYSystem_Create", &::General_FMOD_System_Create,
         bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
    boost::python::def( "PYFMOD_System_Init", &::General_FMOD_System_Init );    
    boost::python::def( "PYFMOD_System_update", &::General_FMOD_Sytem_update );    
    boost::python::def( "CastVoidPtr", &Utility_CastVoidPtr,
                bp::return_value_policy< bp::return_opaque_pointer >());
"""



WRAPPER_DEFINITION_General=\
"""
// lets try this as a global static

FMOD::System * General_FMOD_System_Create( void ) {
FMOD_RESULT       result;
FMOD::System     * Main_system;

result = FMOD::System_Create(&Main_system);
return Main_system;
}

void General_FMOD_Sytem_update( void ) {
    //Main_system->update();
    }
       
void General_FMOD_System_Init ( FMOD::System & me, int channels, FMOD_INITFLAGS flags, void * extra ) {

    me.init(channels, flags, extra);
} 

// sometimes we need to take the ctypess addressof(), an int, and recast it as a general void *
void *
Utility_CastVoidPtr ( int address )
{
    return (void *) address;
    }


"""


WRAPPER_REGISTRATION_System=\
"""
    def( "update1", &::System_FMOD_System_update );    
"""
WRAPPER_DEFINITION_System=\
"""
void System_FMOD_System_update( FMOD::System & me ) {
    me.update();
    }
"""
def apply( mb ):

    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    mb.add_registration_code( WRAPPER_REGISTRATION_General )
    ns = mb.class_( 'System' )
    ns.add_declaration_code( WRAPPER_DEFINITION_System )
    ns.add_registration_code( WRAPPER_REGISTRATION_System )
