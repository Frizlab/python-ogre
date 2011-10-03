import os
import environment



#################################################################################################
#################################################################################################
WRAPPER_DEFINITION_General =\
"""
Hydrax::Module::ProjectedGrid * TestCreateModule ( Hydrax::Hydrax * mHydrax ) {
		// Create our projected grid module  
		Hydrax::Module::ProjectedGrid *mModule 
			= new Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
			                                    mHydrax,
												// Noise module
			                                    new Hydrax::Noise::Perlin(/*Generic one*/),
												// Base plane
			                                    Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
												// Normal mode
												Hydrax::MaterialManager::NM_VERTEX,
												// Projected grid options
										        Hydrax::Module::ProjectedGrid::Options(/*264 /*Generic one*/));
                                                
        return mModule;
}
"""

WRAPPER_REGISTRATION_General = [
    """def( "TestCreateModule", &::TestCreateModule,\\
                "Python-Ogre Helper Function: Returns the Hydrax Module.\\n\\
                Input: MyHydrax\\n\\
                Output: ProjectedGrid",\\
                bp::return_value_policy< bp::manage_new_object >());"""
]
                
def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    apply_reg (mb,  WRAPPER_REGISTRATION_General )
    