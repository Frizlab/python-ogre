import os
import environment

WRAPPER_REGISTRATION_General=\
"""
{ //dxWorld
        typedef bp::class_< dxWorld > dxWorld_exposer_t;
        dxWorld_exposer_t dxWorld_exposer = dxWorld_exposer_t( "dxWorld" );
        bp::scope dxWorld_scope( dxWorld_exposer );
        dxWorld_exposer.def( bp::init< >() );
    }
"""
WRAPPER_DEFINITION_General =\
"""
"""
def apply( mb ):
    pass
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    mb.add_registration_code( WRAPPER_REGISTRATION_General )
    