#include "CaelumPrecompiled.h"
#include "SkyDome.h"
#include "CaelumSystem.h"
#include "GeometryFactory.h"
#include "CaelumExceptions.h"

namespace caelum {

const Ogre::String SkyDome::mSphericDomeResourceName = "CaelumSphericDome";
const Ogre::String SkyDome::SKY_DOME_MATERIAL_NAME = "CaelumSkyDomeMaterial";

SkyDome::SkyDome (Ogre::SceneManager *sceneMgr) {
	mAutoRadius = true;

	createSkyDomeMaterial ();

	GeometryFactory::generateSphericDome (mSphericDomeResourceName, 32);
	Ogre::Entity *ent = sceneMgr->createEntity ("Dome", mSphericDomeResourceName);
	ent->setMaterialName (SKY_DOME_MATERIAL_NAME);
	ent->setRenderQueueGroup (Ogre::RENDER_QUEUE_SKIES_EARLY + 2);
	ent->setCastShadows (false);

	mNode = sceneMgr->getRootSceneNode ()->createChildSceneNode ();
	mNode->attachObject (ent);
}

SkyDome::~SkyDome () {
	if (mNode) {
		// Detach and destroy attached entity.
		Ogre::Entity *ent = static_cast<Ogre::Entity *>(mNode->detachObject ("Dome"));
		ent->_getManager ()->destroyEntity (ent);

		// Destroy the node
		static_cast<Ogre::SceneNode *>(mNode->getParent ())->removeAndDestroyChild (mNode->getName ());
		mNode = 0;
	}

	destroySkyDomeMaterial ();
}

void SkyDome::notifyCameraChanged (Ogre::Camera *cam) {
	mNode->setPosition (cam->getRealPosition ());
	if (mAutoRadius) {
		if (cam->getFarClipDistance () > 0)
			mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getFarClipDistance () - CAMERA_DISTANCE_MODIFIER));
		else
			mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getNearClipDistance () + CAMERA_DISTANCE_MODIFIER));
	}
}

void SkyDome::setFarRadius (float radius) {
	if (radius > 0) {
		mNode->setScale (Ogre::Vector3::UNIT_SCALE * radius);
		mAutoRadius = false;
	}
	else {
		mAutoRadius = true;
	}
}

void SkyDome::setSunDirection (Ogre::Vector3 dir) {
	if (!mSkyDomeMaterial.isNull ()) {
		if (Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
			mSkyDomeMaterial->getTechnique (0)->getPass (0)->getVertexProgramParameters ()->setNamedConstant ("sunDirection", dir);
		}
	}
}

void SkyDome::setLightAbsorption (float absorption) const {
	if (absorption > 1)
		absorption = 1;
	else if (absorption < 0)
		absorption = 0;

	if (!mSkyDomeMaterial.isNull ()) {
		if (Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
			mSkyDomeMaterial->getTechnique (0)->getPass (0)->getVertexProgramParameters ()->setNamedConstant ("lightAbsorption", absorption);
		}
	}
}

void SkyDome::setLightScattering (float scattering) const {
	if (scattering <= 0)
		scattering = 0.00001;

	if (!mSkyDomeMaterial.isNull ()) {
		if (Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
			mSkyDomeMaterial->getTechnique (0)->getPass (0)->getFragmentProgramParameters ()->setNamedConstant ("lightInvScattering", 1.0f / scattering);
		}
	}
}

void SkyDome::setAtmosphereHeight (float height) const {
	if (height <= 0)
		height = 0.00001;
	else if (height > 1)
		height = 1;

	if (!mSkyDomeMaterial.isNull ()) {
		if (Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
			mSkyDomeMaterial->getTechnique (0)->getPass (0)->getFragmentProgramParameters ()->setNamedConstant ("atmosphereInvHeight", 1.0f / height);
		}
	}
}

void SkyDome::updateSkyDomeMaterialTime (SkyColourModel *skyColourModel, float time, Sun *sun) {
	skyColourModel->updateMaterial (mSkyDomeMaterial->getTechnique (0)->getPass (0)->getFragmentProgramParameters (), 
																		mSkyDomeMaterial->getTechnique (0)->getPass (0)->getVertexProgramParameters (), 
																		time, 
																		sun ? sun->getSunDirection () : Ogre::Vector3::UNIT_Y);
}

Ogre::TextureUnitState *SkyDome::getTextureUnitState () {
	if (mSkyDomeMaterial.isNull ())
		return 0;

	return mSkyDomeMaterial->getTechnique (0)->getPass (0)->getTextureUnitState (0); 
}

void SkyDome::createSkyDomeMaterial () {
	Ogre::MaterialPtr mat;

	// Check support
	if (Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_FRAGMENT_PROGRAM)) {
		// TODO: Check if the profiles used are supported
	}
	else {
		throw new UnsupportedException (0, "The card doesn't support the sky dome material. Fragment programs are used.", "SkyDome", "SkyDome.cpp", -1);
	}
	if (Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
		// TODO: Check if the profiles used are supported
	}
	else {
		throw new UnsupportedException (0, "The card doesn't support the sky dome material. Vertex programs are used.", "SkyDome", "SkyDome.cpp", -1);
	}

	LOG ("Generating sky dome material...");
	if (!Ogre::MaterialManager::getSingleton ().resourceExists (SKY_DOME_MATERIAL_NAME)) {
		LOG ("\tMaterial not found; creating...");
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().create (SKY_DOME_MATERIAL_NAME, CaelumSystem::RESOURCE_GROUP_NAME));
		mat->setReceiveShadows (false);
		LOG ("\t\tMaterial [OK]");
		Ogre::Pass *pass = mat->getTechnique (0)->getPass (0);
		pass->setSceneBlending (Ogre::SBT_TRANSPARENT_ALPHA);
		pass->setDepthCheckEnabled (false);
		pass->setDepthWriteEnabled (false);
		pass->setLightingEnabled (false);
		pass->setFog (true);
		// Bind the sky light absorption shader if capable to
		// TODO
		if (Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_FRAGMENT_PROGRAM)) {
			Ogre::HighLevelGpuProgramPtr fp = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram ("SkyDomeFP", CaelumSystem::RESOURCE_GROUP_NAME, "cg", Ogre::GPT_FRAGMENT_PROGRAM);
			fp->setSourceFile ("CaelumShaders.cg");
			fp->setParameter ("entry_point", "SkyDome_fp");
			fp->setParameter ("profiles", "ps_2_0 arbfp1");
			pass->setFragmentProgram ("SkyDomeFP");
			Ogre::GpuProgramParametersSharedPtr parameters = pass->getFragmentProgramParameters();
			parameters->setNamedConstant ("offset", 0.0f);
			parameters->setNamedAutoConstant ("hazeColour", Ogre::GpuProgramParameters::ACT_FOG_COLOUR);
		}
		if (Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
			Ogre::HighLevelGpuProgramPtr vp = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram ("SkyDomeVP", CaelumSystem::RESOURCE_GROUP_NAME, "cg", Ogre::GPT_VERTEX_PROGRAM);
			vp->setSourceFile ("CaelumShaders.cg");
			vp->setParameter ("entry_point", "SkyDome_vp");
			vp->setParameter ("profiles", "vs_2_0 arbvp1");
			pass->setVertexProgram ("SkyDomeVP");
			Ogre::GpuProgramParametersSharedPtr parameters = pass->getVertexProgramParameters();
			parameters->setNamedAutoConstant ("worldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
			parameters->setNamedConstant ("sunDirection", Ogre::Vector3 (1, 0, 0));
		}
		LOG ("\t\tPass [OK]");
		Ogre::TextureUnitState *tus = pass->createTextureUnitState ();
		tus->setTextureAddressingMode (Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_CLAMP, Ogre::TextureUnitState::TAM_CLAMP);
		LOG ("\t\tTextureUnit - Sky gradient [OK]");
		tus = pass->createTextureUnitState ();
		tus->setTextureName ("AtmosphereDepth.png", Ogre::TEX_TYPE_1D);
		tus->setTextureAddressingMode (Ogre::TextureUnitState::TAM_CLAMP, Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_WRAP);
		LOG ("\t\tTextureUnit - Atmosphere depth [OK]");
		mat->load ();
		LOG ("\tDONE");
	}
	else {
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().getByName (SKY_DOME_MATERIAL_NAME));
	}
	LOG ("DONE");

	mSkyDomeMaterial = mat;
}

void SkyDome::destroySkyDomeMaterial () {
	LOG ("Removing sky dome material...");
	if ((Ogre::HighLevelGpuProgramManager::getSingletonPtr()->resourceExists("SkyLightAbsorptionFP"))) {
		LOG ("Removing sky dome SkyLightAbsorptionFP...");
		Ogre::HighLevelGpuProgramManager::getSingletonPtr()->remove("SkyLightAbsorptionFP");
	}

	if ((Ogre::HighLevelGpuProgramManager::getSingletonPtr()->resourceExists("SkyLightAbsorptionVP"))) {
		LOG ("Removing sky dome SkyLightAbsorptionVP...");
		Ogre::HighLevelGpuProgramManager::getSingletonPtr()->remove("SkyLightAbsorptionVP");
	}

	if (!Ogre::MaterialManager::getSingleton ().resourceExists (SKY_DOME_MATERIAL_NAME)) {
		Ogre::MaterialManager::getSingleton ().remove (SKY_DOME_MATERIAL_NAME);
	}
	mSkyDomeMaterial.setNull ();
	LOG ("DONE");
}

} // namespace caelum
