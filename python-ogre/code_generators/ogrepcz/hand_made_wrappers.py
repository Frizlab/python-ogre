import os
import environment

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
    """bp::def( "castAsPCZSceneNode", &General_castAsPCZSceneNode,
                "Python-Ogre Helper Function: Casts a SceneNode as a PCZSceneNode.\\n\\
                Input: SceneNode\\n\\
                Ouput: PCZSceneNode",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def( "castAsPCZSceneManager", &General_castAsPCZSceneManager,
                "Python-Ogre Helper Function: Casts a SceneManager as a PCZSceneManager.\\n\\
                Input: SceneManager\\n\\
                Ouput: PCZSceneManager",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def( "castAsPCZRaySceneQuery", &General_castAsPCZRaySceneQuery,
                "Python-Ogre Helper Function: Casts a RaySceneQuery as a PCZRaySceneQuery.\\n\\
                Input: RaySceneQuery\\n\\
                Ouput: PCZRaySceneQuery",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
]

#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    apply_reg (mb,  WRAPPER_REGISTRATION_General )
    