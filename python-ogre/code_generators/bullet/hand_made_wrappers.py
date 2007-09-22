import os
import environment

WRAPPER_WRAPPER_getName =\
"""
    virtual char * getName(  ) const {
        bp::override func_getName = this->get_override( "getName" );
        return func_getName(  );
    }
"""
WRAPPER_WRAPPER_getInternalManifoldPointer =\
"""
    virtual ::btPersistentManifold * * getInternalManifoldPointer(  ){
        bp::override func_getInternalManifoldPointer = this->get_override( "getInternalManifoldPointer" );
        return func_getInternalManifoldPointer(  );
    }
    """
    
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):

    rt = mb.class_( 'btCollisionShape' )   
    rt.add_wrapper_code ( WRAPPER_WRAPPER_getName )
    rt = mb.class_( 'btConcaveShape' )   
    rt.add_wrapper_code ( WRAPPER_WRAPPER_getName )
    rt = mb.class_( 'btDispatcher' )   
    rt.add_wrapper_code ( WRAPPER_WRAPPER_getInternalManifoldPointer )
#     