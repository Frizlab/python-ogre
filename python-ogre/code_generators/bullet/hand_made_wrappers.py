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
    
WRAPPER_DEFINITION_btPersistentManifold=\
"""    
btCollisionObject* getBodyAsObject0 ( btPersistentManifold & me ){
    return static_cast<btCollisionObject*>(me.getBody0());
    }
btCollisionObject* getBodyAsObject1 ( btPersistentManifold & me ){
    return static_cast<btCollisionObject*>(me.getBody1());
    }
bp::list getBodies ( btPersistentManifold & me ) {
    bp::list listout;
    listout.append( static_cast<btCollisionObject*>(me.getBody0()) );
    listout.append( static_cast<btCollisionObject*>(me.getBody1()) );
    return listout;
    }
""" 

WRAPPER_REGISTRATION_btPersistentManifold = [
    """def( "getBodyAsObject0", &::getBodyAsObject0,
    "Python-Ogre Helper Function: Get the first body (0) in a collision\\n\\
    Input: None\\n\\
    Output: btCollisionObject\\n",
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >() );""",
    """def( "getBodyAsObject1", &::getBodyAsObject1,
    "Python-Ogre Helper Function: Get the second body (1) in a collision\\n\\
    Input: None\\n\\
    Output: btCollisionObject\\n",
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >() );""",
    """def( "getBodies", &::getBodies,
    "Python-Ogre Helper Function: Get a list containing both bodies in a collision\\n\\
    Input: None\\n\\
    Output: list of 2 btCollisionObject objects\\n" );"""
    ]
    
WRAPPER_DEFINITION_btCollisionWorld=\
"""
bp::list getCollidingObjects ( btCollisionWorld & me ) {
    bp::list listout;
    // do the collisiondetection
    me.performDiscreteCollisionDetection();
    
    const unsigned int  numManifolds = me.getDispatcher()->getNumManifolds();
    for (unsigned int i=0;i < numManifolds; i++)
        {
        btPersistentManifold* contactManifold = me.getDispatcher()->getManifoldByIndexInternal(i);
        
        btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
        btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
        
        contactManifold->refreshContactPoints(obA->getWorldTransform(),obB->getWorldTransform());
        
        const unsigned int numContacts = contactManifold->getNumContacts();
        if (numContacts > 0) {
            listout.append(bp::make_tuple(obA, obB, numContacts) );
            }
        }
    return listout;
}

bp::list getCollidingObjectsAndPoints ( btCollisionWorld & me ) {
    bp::list listout;
    bp::list * sublist;
    
    // do the collisiondetection
    me.performDiscreteCollisionDetection();
    
    const unsigned int  numManifolds = me.getDispatcher()->getNumManifolds();
    for (unsigned int i=0;i < numManifolds; i++) {
        btPersistentManifold* contactManifold = me.getDispatcher()->getManifoldByIndexInternal(i);
        
        btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
        btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
        
        contactManifold->refreshContactPoints(obA->getWorldTransform(),obB->getWorldTransform());
        
        const unsigned int numContacts = contactManifold->getNumContacts();
        if (numContacts > 0) {
            sublist = new bp::list();
            for (unsigned int j = 0;j < numContacts; j++)  {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);
                sublist->append ( pt );
                }
            listout.append(bp::make_tuple(obA, obB, sublist) );
            }
        }
    return listout;
}
"""   
WRAPPER_REGISTRATION_btCollisionWorld = [
    """def( "getCollidingObjects", &::getCollidingObjects,
    "Python-Ogre Helper Function: Get a list of all colliding bodies in the world\\n\\
    Input: None\\n\\
    Output: lists of tuples, each tuple contains body0, body1 and num contact points\\n" );""",
    """def( "getCollidingObjectsAndPoints", &::getCollidingObjectsAndPoints,
    "Python-Ogre Helper Function: Get a list of all colliding bodies along with contact points\\n\\
    Input: None\\n\\
    Output: list of tuples, each tuples contains body0, body1 and a list of contact points\\n" );""",
    ]


#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):

    rt = mb.class_( 'btCollisionShape' )   
    rt.add_wrapper_code ( WRAPPER_WRAPPER_getName )
    
    rt = mb.class_( 'btDispatcher' )   
    rt.add_wrapper_code ( WRAPPER_WRAPPER_getInternalManifoldPointer )
    
    rt = mb.class_( 'btCollisionWorld' )  
    rt.add_declaration_code( WRAPPER_DEFINITION_btCollisionWorld )
    apply_reg (rt,  WRAPPER_REGISTRATION_btCollisionWorld )

    rt = mb.class_( 'btPersistentManifold' )  
    rt.add_declaration_code( WRAPPER_DEFINITION_btPersistentManifold )
    apply_reg (rt,  WRAPPER_REGISTRATION_btPersistentManifold )
    