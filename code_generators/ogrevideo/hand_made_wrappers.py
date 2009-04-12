import os
import environment

WRAPPER_DEFINITION_General = \
"""
OpenAL_AudioInterfaceFactory* mAudioFactory;

void General_enableOpenALAudio( void ){  
		mAudioFactory=new Ogre::OpenAL_AudioInterfaceFactory();
		TheoraVideoManager* mgr = (Ogre::TheoraVideoManager*) Ogre::ExternalTextureSourceManager::getSingleton().getExternalTextureSource("ogg_video");
		mgr->setAudioInterfaceFactory(mAudioFactory);
    }			
"""
    
WRAPPER_REGISTRATION_General = [
    """bp::def( "enableOpenALAudio", &General_enableOpenALAudio,
                "Python-Ogre Helper Function: Enable OpenAL Audio\\n\\
                Input: \\n\\
                Ouput: r" ) ; """
                
    ]    

#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    apply_reg (mb,  WRAPPER_REGISTRATION_General )

    