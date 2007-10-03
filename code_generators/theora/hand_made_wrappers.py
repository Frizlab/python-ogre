import os
import environment

WRAPPER_DEFINITION_General = \
"""
Ogre::TheoraVideoController * General_castAsVideoController( Ogre::ExternalTextureSource* e ){  
       return (Ogre::TheoraVideoController*) e;
    }
 
Ogre::TheoraVideoController * Get_Controller( void ) {
    Ogre::TheoraVideoController * mVideoControl =  static_cast<Ogre::TheoraVideoController*>
			(Ogre::ExternalTextureSourceManager::getSingleton().
				getExternalTextureSource("ogg_video"));
	return mVideoControl;
	}
	
Ogre::TheoraVideoController * General_Init( void ) {
    Ogre::TheoraVideoController * mVideoControl =  static_cast<Ogre::TheoraVideoController*>
			(Ogre::ExternalTextureSourceManager::getSingleton().
				getExternalTextureSource("ogg_video"));
				
        mVideoControl->setInputName( "clock.ogg" );
		//Start paused so we can have audio
		mVideoControl->setPlayMode( Ogre::TextureEffectPause );
		//! Used for attaching texture to Technique, State, and texture unit layer
		mVideoControl->setTextureTecPassStateLevel( 0, 0, 0 );

		//Set to true to allow for seeking - highly experimental though ;)
		mVideoControl->setSeekEnabled( false );
		//This is mainly for OpenAL - but applies to other audio libs which
		//use pooling instead of callbacks for updating...
		//Let TheoraMovieClip update the audioclip.
		mVideoControl->setAutoAudioUpdate( false );
		
		
		// Grab Our material, then add a new texture unit
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("Example/TheoraVideoPlayer/Play");

		//Create the material the first time through this method
		if( material.isNull() )
			material = Ogre::MaterialManager::getSingleton().create("Example/TheoraVideoPlayer/Play", "General");

		material->getTechnique(0)->getPass(0)->createTextureUnitState();

		mVideoControl->createDefinedTexture( "Example/TheoraVideoPlayer/Play", "General" );
		
		return mVideoControl;
		}
		
				
"""
    
WRAPPER_REGISTRATION_General = [
    """bp::def( "castAsVideoController", &General_castAsVideoController,
                "Python-Ogre Helper Function: Casts as Video Controller\\n\\
                Input: \\n\\
                Ouput: r",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def( "InitVideoController", &General_Init,
                "Python-Ogre Helper Function: Init Video Controller .\\n\\
                Input: None\\n\\
                Ouput: ",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());"""                
    """bp::def( "getVideoController", &Get_Controller,
                "Python-Ogre Helper Function: Get a  Video Controller .\\n\\
                Input: None\\n\\
                Ouput: ",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());"""                
    ]    

#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    return
# #     mb.add_declaration_code( WRAPPER_DEFINITION_General )
# #     apply_reg (mb,  WRAPPER_REGISTRATION_General )

    