import os
import environment


#
# these casting calls (in General) shouldn't be needed as by default Python/Boost understands that the objects
# are 'PCZ' ones and it all works.. 
#
# However adding casting functions to each class to make them into their Ogre parent types just in case
#
WRAPPER_DEFINITION_General = \
"""
// The following helper functions are a start and possibly need to be extended to include
// additional scene queries..
Ogre::PCZSceneNode * General_castAsPCZSceneNode( Ogre::SceneNode * in ){  
       return (Ogre::PCZSceneNode*) in;
    }
Ogre::PCZSceneManager * General_castAsPCZSceneManager( Ogre::SceneManager * in ){  
       return (Ogre::PCZSceneManager*) in;
    }
Ogre::PCZRaySceneQuery * General_castAsPCZRaySceneQuery( Ogre::RaySceneQuery * in ){  
       return (Ogre::PCZRaySceneQuery*) in;
    }
    
/// TO HANDLE ??
/* PCZAxisAlignedBoxSceneQuery : public DefaultAxisAlignedBoxSceneQuery
PCZRaySceneQuery : public DefaultRaySceneQuery
PCZSphereSceneQuery : public DefaultSphereSceneQuery
PCZPlaneBoundedVolumeListSceneQuery : public DefaultPlaneBoundedVolumeListSceneQuery  
*/
"""

WRAPPER_REGISTRATION_General = [
    """def( "castAsPCZSceneNode", &General_castAsPCZSceneNode,
                "Python-Ogre Helper Function: Casts a SceneNode as a PCZSceneNode.\\n\\
                Input: SceneNode\\n\\
                Ouput: PCZSceneNode",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """def( "castAsPCZSceneManager", &General_castAsPCZSceneManager,
                "Python-Ogre Helper Function: Casts a SceneManager as a PCZSceneManager.\\n\\
                Input: SceneManager\\n\\
                Ouput: PCZSceneManager",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """def( "castAsPCZRaySceneQuery", &General_castAsPCZRaySceneQuery,
                "Python-Ogre Helper Function: Casts a RaySceneQuery as a PCZRaySceneQuery.\\n\\
                Input: RaySceneQuery\\n\\
                Ouput: PCZRaySceneQuery",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
]


WRAPPER_DEFINITION_PCZSceneNode = \
"""
Ogre::SceneNode * PCZSceneNode_castAsSceneNode( Ogre::PCZSceneNode * me ){  
       return ((Ogre::SceneNode*) me);
    }
"""
WRAPPER_REGISTRATION_PCZSceneNode = [
    """def( "castAsSceneNode", &PCZSceneNode_castAsSceneNode,
                "Python-Ogre Helper Function: Casts a PCZSceneNode as a SceneNode.\\n\\
                Input: None\\n\\
                Ouput: SceneNode",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());"""
    ]


    
WRAPPER_DEFINITION_PCZSceneManager = \
"""
Ogre::SceneManager * PCZSceneManager_castAsSceneManager( Ogre::PCZSceneManager * me ){  
       return ((Ogre::SceneManager*) me);
    }
"""
WRAPPER_REGISTRATION_PCZSceneManager = [
    """def( "castAsSceneManager", &PCZSceneManager_castAsSceneManager,
                "Python-Ogre Helper Function: Casts a PCZSceneManager as a SceneManager.\\n\\
                Input: None\\n\\
                Ouput: SceneManager",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());"""
    ]

        
WRAPPER_DEFINITION_PCZRaySceneQuery = \
"""
Ogre::RaySceneQuery * PCZRaySceneQuery_castAsRaySceneQuery( Ogre::PCZRaySceneQuery * me ){  
       return ( ( Ogre::RaySceneQuery* ) me );
    }
Ogre::RaySceneQueryResult& PCZRaySceneQuery_execute( Ogre::PCZRaySceneQuery * me ){
    return ((Ogre::RaySceneQuery*) me)->execute();
    }
    
"""
WRAPPER_REGISTRATION_PCZRaySceneQuery = [
    """def( "castAsRaySceneQuery", &PCZRaySceneQuery_castAsRaySceneQuery,
                "Python-Ogre Helper Function: Casts a PCZRaySceneQuery as a RaySceneQuery.\\n\\
                Input: None\\n\\
                Ouput: RaySceneQuery",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """def( "execute", &PCZRaySceneQuery_execute,
                "Python-Ogre Helper Function: Executes a query.\\n\\
                Input: None\\n\\
                Ouput: RaySceneQueryResult",\
                bp::return_value_policy< bp::default_call_policies >());"""
    ]
    
    
WRAPPER_DEFINITION_PCZAxisAlignedBoxSceneQuery = \
"""
Ogre::AxisAlignedBoxSceneQuery * General_castAsAxisAlignedBoxSceneQuery( Ogre::PCZAxisAlignedBoxSceneQuery * me ){  
       return (Ogre::AxisAlignedBoxSceneQuery*) me;
    }
"""

WRAPPER_DEFINITION_PCZSphereSceneQuery = \
"""
Ogre::SphereSceneQuery * General_castAsSphereSceneQuery( Ogre::PCZSphereSceneQuery * me ){  
       return (Ogre::SphereSceneQuery*) me;
    }
"""

WRAPPER_DEFINITION_PCZAxisAlignedBoxSceneQuery = \
"""
Ogre::PlaneBoundedVolumeListSceneQuery * General_castAsPlaneBoundedVolumeListSceneQuery( Ogre::PCZPlaneBoundedVolumeListSceneQuery * me ){  
       return (Ogre::PlaneBoundedVolumeListSceneQuery*) me;
    }
"""





WRAPPER_DEFINITION_Portal = """
void Portal_setCorners( ::Ogre::Portal & me, boost::python::list listin )
{
	Ogre::Vector3 corners[4];
    size_t maxlen = len ( listin );
    if (maxlen > 4) maxlen = 4; // can only handle upto 4 'corners'
    size_t counter = 0;
    for (counter=0;counter < maxlen; counter ++ )
        corners[counter] =  boost::python::extract<Ogre::Vector3> (listin[counter]);
    me.setCorners ( corners);        
}
"""
WRAPPER_REGISTRATION_Portal = [
    """def( "setCorners", &::Portal_setCorners,\\
                "Python-Ogre Helper Function: Sets the corners of a portal.\\n\\
                Input: List of Vector3's - either 2 or 4 depending on portal type\\n\\
                Output: None");"""
    ]
    
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'Portal' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Portal )
    apply_reg (rt,  WRAPPER_REGISTRATION_Portal )
    
    rt = mb.class_( 'PCZSceneNode' )
    rt.add_declaration_code( WRAPPER_DEFINITION_PCZSceneNode )
    apply_reg (rt,  WRAPPER_REGISTRATION_PCZSceneNode )
    rt = mb.class_( 'PCZSceneManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_PCZSceneManager )
    apply_reg (rt,  WRAPPER_REGISTRATION_PCZSceneManager )
    rt = mb.class_( 'PCZRaySceneQuery' )
    rt.add_declaration_code( WRAPPER_DEFINITION_PCZRaySceneQuery )
    apply_reg (rt,  WRAPPER_REGISTRATION_PCZRaySceneQuery )

    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    apply_reg (mb,  WRAPPER_REGISTRATION_General )
    