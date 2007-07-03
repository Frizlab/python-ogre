import os
import environment

## Need wrapping and callback functionality
# ::Opcode::MeshInterface::
#	typedef void	(*RequestCallback)	(udword triangle_index, VertexPointers& triangle, void* user_data);
# 		bool			SetCallback(RequestCallback callback, void* user_data);
# 		inline_			void*				GetUserData()		const	{ return mUserData;		}
# 		inline_			RequestCallback		GetCallback()		const	{ return mObjCallback;	}
		
WRAPPER_DEFINITION_MeshInterface=\
"""
bool
Mesh_SetCallback ( ::Opcode::MeshInterface & me, 
                    PyObject* function, std::string const & method="",
                    PyObject* user_data )
{
    meshCB = new MeshCallback(subscriber, method);
    return me->SetCallback( ( ::Opcode::MeshInterface::RequestCallback*) &meshCB,  user_data );
};
    
PyObject *
Mesh_GetUserData ( ::Opcode::MeshInterface & me) {
    void *  data = me.GetUserData ();
    Py_INCREF( (PyObject *) data );     // I'm passing a reference to this object so better inc the ref :)
    return  (PyObject *) data;
    }
"""


WRAPPER_REGISTRATION_MeshInterface =[
    'def ("SetCallback", &::Mesh_SetCallback);',
    'def ("GetUserData", &::Mesh_GetUserData);'
    ]		
		
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    cs = mb.class_( 'Opcode' )
    cs.add_declaration_code( WRAPPER_DEFINITION_MeshInterface )
    apply_reg (cs,  WRAPPER_REGISTRATION_MeshInterface )

    