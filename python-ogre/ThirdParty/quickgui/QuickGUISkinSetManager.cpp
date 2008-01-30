#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIForwardDeclarations.h"

#include "QuickGUISkinSetManager.h"

template<> QuickGUI::SkinSetManager* Ogre::Singleton<QuickGUI::SkinSetManager>::ms_Singleton = 0; 

namespace QuickGUI
{
	SkinSetManager::SkinSetManager()
	{
	}

	SkinSetManager::~SkinSetManager()
	{
		// delete imagesets
		std::map<std::string,SkinSet*>::iterator it;
		for( it = mSkinSets.begin(); it != mSkinSets.end(); ++it )
			delete (it->second);
		mSkinSets.clear();
		mSkinSetsbyTextureName.clear();
	}

	SkinSetManager* SkinSetManager::getSingletonPtr(void) { return ms_Singleton; } 
	SkinSetManager& SkinSetManager::getSingleton(void) { assert( ms_Singleton );  return ( *ms_Singleton ); } 

	bool SkinSetManager::embeddedInSkinSet(const std::string& skinName, const std::string& textureName)
	{
		if(!skinLoaded(skinName)) 
			return false;
		else 
			return mSkinSets[skinName]->containsImage(textureName);
	}

	bool SkinSetManager::embeddedInSkinSet(const std::string& textureName)
	{
		std::map<std::string,SkinSet*>::iterator it;
		for( it = mSkinSets.begin(); it != mSkinSets.end(); ++it )
			if( it->second->containsImage(textureName) )
				return true;

		return false;
	}

	SkinSet* SkinSetManager::getSkinSet(const std::string& name)
	{
		if(!skinLoaded(name)) 
			return NULL;
		else 
			return mSkinSets[name];
	}

	SkinSet* SkinSetManager::getSkinSetByTextureName(const std::string& texName)
	{
		std::map<std::string,SkinSet*>::iterator skinSetIt = mSkinSetsbyTextureName.find(texName);
		if (skinSetIt != mSkinSetsbyTextureName.end())
			return skinSetIt->second;
		return NULL;
	}

	std::map<std::string,SkinSet*>* SkinSetManager::getSkinSetList()
	{
		return &mSkinSets;
	}

	void SkinSetManager::loadSkin(const std::string& skinName, SkinSet::ImageType t, const std::string &resourceGroup)
	{
		// check if imageset is already created for this skin
		if( mSkinSets.find(skinName) != mSkinSets.end() )
			return;

		SkinSet *skinSet = new SkinSet(skinName, t, resourceGroup);
		mSkinSets[skinName] = skinSet;
		mSkinSetsbyTextureName[skinSet->getTextureName()] = skinSet;
	}

	bool SkinSetManager::skinLoaded(const std::string& skinName)
	{
		return (mSkinSets.find(skinName) != mSkinSets.end());
	}
}
