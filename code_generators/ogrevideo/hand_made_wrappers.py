import os
import environment

WRAPPER_DEFINITION_General = \
"""
void General_test() {
	OgreVideoManager* mgr=(OgreVideoManager*) OgreVideoManager::getSingletonPtr();
	mgr->setInputName("konqi.ogg");
	mgr->createDefinedTexture("video_material");
	getClip("konqi.ogg")->setAutoRestart(1);
	}

"""
    
WRAPPER_REGISTRATION_General = [
    """bp::def( "testit", &General_test,
                "Python-Ogre Helper Function: Enable OpenAL Audio\\n\\
                Input: \\n\\
                Ouput: r" ) ; """ ,
#    """bp::def( "getTheoraVideoManager", &General_getTheoraVideoManager,
#                 "Python-Ogre Helper Function: Get the Single\\n\\
#                 Input: \\n\\
#                 Ouput: r",\
#                 bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());"""
#    """bp::def( "castAsTheoraVideoManager", &General_castAsTheoraVideoManager,
#                 "Python-Ogre Helper Function: Cast a external texture source manager to the theroamanager\\n\\
#                 Input: \\n\\
#                 Ouput: r",\
#                 bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());*/"""

    ]    

#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
   mb.add_declaration_code( WRAPPER_DEFINITION_General )
   apply_reg (mb,  WRAPPER_REGISTRATION_General )

    