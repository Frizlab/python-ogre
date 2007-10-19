#include "QuickGUISkinSet.h"

namespace QuickGUI
{
	SkinSet::SkinSet(const Ogre::String& skinName) :
		mSkinName(skinName),
		mNumIndividualTextures(0),
		mTextureWidth(1024),
		mTextureHeight(1024)
	{
		mTexture.setNull();
		_findSkinTextures();

		buildTexture();
	}

	SkinSet::~SkinSet()
	{
		// Remove Texture for this SkinSet from Texture Manager
		Ogre::TextureManager::getSingleton().remove(mTexture->getName());

		mTexture.setNull();
	}

	void SkinSet::_findSkinTextures()
	{
		mTextureNames.clear();

		Ogre::ResourceGroupManager* rgm = Ogre::ResourceGroupManager::getSingletonPtr();
		Ogre::StringVector resourceGroupNames = rgm->getResourceGroups();

		Ogre::StringVector::iterator groupItr;
		for( groupItr = resourceGroupNames.begin(); groupItr != resourceGroupNames.end(); ++groupItr )
		{
			Ogre::FileInfoListPtr files = rgm->findResourceFileInfo((*groupItr),mSkinName + "*");

			for( Ogre::FileInfoList::iterator fileItr = files->begin(); fileItr != files->end(); ++fileItr ) 
			{
				Ogre::String fileName = (*fileItr).filename;
				if(_isImageFile(fileName))
					mTextureNames.push_back(fileName);
			}
		}
	}

	bool SkinSet::_isImageFile(const Ogre::String& fileName)
	{
		Ogre::String::size_type index = fileName.find_last_of('.');
		if( (index != Ogre::String::npos) || (index = fileName.length() - 1) )
		{
			Ogre::String extension = fileName.substr(index + 1);
			Ogre::StringUtil::toLowerCase(extension);
			Ogre::StringVector supportedImageCodecs(Ogre::Codec::getExtensions());

			return (std::find(supportedImageCodecs.begin(),supportedImageCodecs.end(),extension) != supportedImageCodecs.end());
		}

		return false;
	}

	void SkinSet::addTexture(const Ogre::String& textureName)
	{
		// make sure texture is not already in the list.
		std::vector<Ogre::String>::iterator texNameItr;
		for( texNameItr = mTextureNames.begin(); texNameItr != mTextureNames.end(); ++texNameItr )
		{
			if( (*texNameItr) == textureName ) return;
		}

		// make sure textureName refers to a valid resource on disk.
		try
		{
			Ogre::ResourceGroupManager::getSingleton().findGroupContainingResource(textureName);
		}
		catch(...) { return; }

		// add it to the list of textures to be used to create SkinSet texture!
		mTextureNames.push_back(textureName);
	}

	void SkinSet::buildTexture()
	{
		mContainedTextures.clear();
		mNumIndividualTextures = 0;

		// Load up each image into a vector
		std::vector<Ogre::Image> images;
		std::vector<Ogre::String>::iterator texNameItr;
		for( texNameItr = mTextureNames.begin(); texNameItr != mTextureNames.end(); ++texNameItr )
		{
			Ogre::Image tempImg;
			tempImg.load((*texNameItr),Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			images.push_back(tempImg);
			mContainedTextures.insert((*texNameItr));

			++mNumIndividualTextures;
		}

		// Sort images and names according to Height (Insertion Sort Algorithm)
		// This helps make the final texture more efficient (i.e. densely packed)
		for(int i = 1; i < mNumIndividualTextures; i++ )
		{
			Ogre::Image img = images.at(i);
			Ogre::String name = mTextureNames.at(i);
			int j = i - 1;
			while( j >= 0 && images.at(j).getHeight() > img.getHeight() )
			{
				images.at(j+1) = images.at(j);
				mTextureNames.at(j+1) = mTextureNames.at(j);
				j--;
			}
			images.at(j+1) = img;
			mTextureNames.at(j+1) = name;
		}

		// Initialize Texture
		if( mTexture.isNull() )
		{
			mTexture = Ogre::TextureManager::getSingletonPtr()->createManual(
				mSkinName,
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				Ogre::TEX_TYPE_2D,
				mTextureWidth,
				mTextureHeight,
				0,
				Ogre::PF_B8G8R8A8);
		}

		// Texture buffer
		Ogre::HardwarePixelBufferSharedPtr buf = mTexture->getBuffer();

		int cur_x = 0, cur_y = 0, next_y = 0;

		mTextureMap.clear();

		// Blit images to texture buffer
		// Minimum of 1 pixel between each image at all times
		for(int i = 0; i < mNumIndividualTextures; i++)
		{
			// See if we can still draw on this row, or if we need a new one
			if( (cur_x + images.at(i).getWidth() ) < mTexture->getWidth() )
			{
				// Keep track of tallest image in the row
				if( next_y < (int)images.at(i).getHeight() + cur_y )
					next_y = (int)images.at(i).getHeight() + cur_y + 1;

				// Not enough room for all the images!
				if( next_y > (int)mTexture->getHeight() )
				{
					Ogre::Image finalImageSet;
					finalImageSet.load("QuickGUI.output.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					buf->blitToMemory(finalImageSet.getPixelBox());
					finalImageSet.save("SkinSetImage.png");
		
					throw Ogre::Exception(1, "The texture is not large enough to fit all widget images.  Please view \"SkinSetImage.png\"", "QuickGUIImageSet");
				}
				
				// Blit to the specified location on the texture
				buf->blitFromMemory(images.at(i).getPixelBox(),
					Ogre::Image::Box(cur_x, cur_y, images.at(i).getWidth()+cur_x, images.at(i).getHeight()+cur_y));

				// Store the image name associated with its Texture Coordinates (UV).  
				// UV coordinates are float values between 0 and 1.
				mTextureMap[mTextureNames.at(i)] = 
					Ogre::Vector4(
						(float)cur_x / (float)mTextureWidth, 
						(float)cur_y / (float)mTextureHeight, 
						(float)(images.at(i).getWidth() + cur_x) / (float)mTextureWidth, 
						(float)(images.at(i).getHeight() + cur_y) / (float)mTextureWidth 
						);
				
				// Shift x over the correct amount
				cur_x += (int)images.at(i).getWidth() + 1;
			}
			// New Row
			else
			{
				// Reset variables for drawing the next row
				cur_x = 0;
				cur_y = next_y;

				// Keep track of the tallest image in the row
				if( next_y < (int)images.at(i).getHeight() + cur_y )
					next_y = (int)images.at(i).getHeight() + cur_y + 1;

				// Not enough room for all the images!
				if( next_y > (int)mTexture->getHeight() )
				{
					Ogre::Image finalImageSet;
					finalImageSet.load("QuickGUI.output.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					buf->blitToMemory(finalImageSet.getPixelBox());
					finalImageSet.save("SkinSetImage.png");

					throw Ogre::Exception(1, "The texture is not large enough to fit all widget images.  Please view \"SkinSetImage.png\"", "QuickGUIImageSet");
				}
				
				// Blit to the specified location on the texture
				buf->blitFromMemory(images.at(i).getPixelBox(),
					Ogre::Image::Box(cur_x, cur_y, images.at(i).getWidth()+cur_x, images.at(i).getHeight()+cur_y));
				
				// Store the image name associated with its Texture Coordinates (UV).  
				// UV coordinates are float values between 0 and 1.
				mTextureMap[mTextureNames.at(i)] = 
					Ogre::Vector4(
						(float)cur_x / (float)mTextureWidth, 
						(float)cur_y / (float)mTextureHeight, 
						(float)(images.at(i).getWidth() + cur_x) / (float)mTextureWidth, 
						(float)(images.at(i).getHeight() + cur_y) / (float)mTextureWidth 
						);
				
				// Shift x over the correct amount
				cur_x += (int)images.at(i).getWidth() + 1;
			}
		}

		// Following lines are unnecessary... just puts it out to a file to see :-)
/*		Ogre::Image finalImageSet;
		finalImageSet.load("QuickGUI.output.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		buf->blitToMemory(finalImageSet.getPixelBox());
		finalImageSet.save("SkinSetImage.png");
*/	}

	bool SkinSet::containsImage(Ogre::String textureName)
	{
		if(mContainedTextures.find(textureName) != mContainedTextures.end())
			return true;

		return false;
	}

	Ogre::String SkinSet::getTextureName()
	{
		return mTexture->getName();
	}

	Ogre::Vector4 SkinSet::getTextureCoordinates(Ogre::String imageName)
	{
		if( mTextureMap.find(imageName) == mTextureMap.end() )
			return Ogre::Vector4(0,0,1,1);
		else
			return mTextureMap[imageName];
	}
}
