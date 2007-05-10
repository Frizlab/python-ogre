import os
import environment

WRAPPER_DEFINITION_Utility = \
"""
SimpleGUI::EventArgs * 
Utility_castEventArgs ( SimpleGUI::EventArgs * n ) {
    if( dynamic_cast< SimpleGUI::MouseEventArgs * >( n ) ){
        return  (SimpleGUI::MouseEventArgs*) n ;
    }    
    if( dynamic_cast< SimpleGUI::KeyEventArgs * >( n ) ){
        return  (SimpleGUI::KeyEventArgs*)n  ;
    }    
    if( dynamic_cast< SimpleGUI::ButtonStateEventArgs * >( n ) ){
        return  (SimpleGUI::ButtonStateEventArgs*)n  ;
    }    
    return ( n );  
}
"""
WRAPPER_REGISTRATION_Utility = [
    'def( "CastArgs", &::Utility_castEventArgs,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());'
    ]
    
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    return
    mb.add_declaration_code( WRAPPER_DEFINITION_Utility )
    apply_reg (mb,  WRAPPER_REGISTRATION_Utility )
    