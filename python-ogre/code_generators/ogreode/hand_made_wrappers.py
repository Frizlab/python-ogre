import os
import environment

WRAPPER_DEFINITION_General=\
"""
dBody * CreateBody ( dWorld * world )
    {
    return &dBody(world->id());
    }
# dBody * CreateBodyWithID ( dWorldID * id )
#     {
#     return &dBody(id);
#     }
# dWorldID * GetID ( dWorld * world )
# {
#     return world->id();
# }
"""
WRAPPER_REGISTRATION_General =\
"""
bp::def ("CreateBody", &CreateBody, 
        bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
# bp::def ("CreateBodyWithID", &CreateBodyWithID, 
#         bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
# bp::def ("GetID", &GetID, 
#         bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >());
"""
def apply( mb ):
    pass
#     
#     mb.add_declaration_code( WRAPPER_DEFINITION_General )
#     mb.add_registration_code( WRAPPER_REGISTRATION_General )
    