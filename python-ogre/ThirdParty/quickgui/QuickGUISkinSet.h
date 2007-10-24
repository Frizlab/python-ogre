#ifndef QUICKGUIIMAGESET_H
#define QUICKGUIIMAGESET_H

#include "OgreCodec.h"
#include "OgreException.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgrePrerequisites.h"
#include "OgreResourceGroupManager.h"
#include "OgreTextureManager.h"
#include "OgreVector4.h"
#include "OgreLogManager.h"

#include "QuickGUIExportDLL.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

namespace QuickGUI
{
	/** 
	* Stores a generated SkinSet from the individual widget images
	*/
	class _QuickGUIExport SkinSet
	{
	public:
		// Generate a new SkinSet using the skin's image files.
		SkinSet(const Ogre::String& skinName, const Ogre::String &resourceGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
		// Delete this SkinSet
		~SkinSet();

		/*
		* Adds a texture name to the list of textures that will be included in the
		* SkinSet texture.  
		* NOTE: Texture will not be present until buildTexture function is executed.
		*/
		void addTexture(const Ogre::String& textureName);

		/*
		* Builds the resulting Image from all added Images.  If Texture has already
		* been built, it will be rebuilt.  If you have added images to the SkinSet,
		* they will not be in the texture until you call this function.
		*/
		void buildTexture();

		/*
		* Returns true if SkinSet Texture contains texture, false otherwise.
		*/
		bool containsImage(Ogre::String textureName);

		Ogre::String getSkinName();
		// Return the name of the skin for this SkinSet
		Ogre::String getTextureName();

		/*
		* Return the UV coordinates of the image, assuming the image is a part of the image set.
		* Return form is (left,top,right,bot). (left and right refer to x-coordinate, top and
		* bottom refer to y-coordinates)
		*/
		Ogre::Vector4 getTextureCoordinates(Ogre::String imageName);
		/*
		* saves skin to a .skinset file in the folder containing first 
		* place found in the resource group.
		*/
		void saveSkin();
		/*
		* loads a skin from a .skinset file return false if failing
		*/
		bool loadSkin();
	protected:
		Ogre::String mSkinName;

		// list of textures used to build the SkinSet Texture.
		std::vector<Ogre::String> mTextureNames;
		// list of textures that are currently in the SkinSet Texture.
		std::set<Ogre::String> mContainedTextures;

		// The actual texture that stores the imageset
		Ogre::TexturePtr mTexture;
		Ogre::uint mTextureWidth;
		Ogre::uint mTextureHeight;

		// This map connects individual textures to its UV coordinates, within
		// the SkinSet Texture.
		std::map<Ogre::String,Ogre::Vector4> mTextureMap;

		// Number of images that this SkinSet has
		int mNumIndividualTextures;

	private:
		void _findSkinTextures();
		bool _isImageFile(const Ogre::String& fileName);
		Ogre::String mTextureName;
		Ogre::String mResourceGroup;
	};
}

#endif
