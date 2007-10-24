#include "QuickGUISkinSet.h"
#include "QuickGUIConfigScriptParser.h"

#include "OgreStringConverter.h"

namespace QuickGUI
{
	SkinSet::SkinSet(const Ogre::String& skinName, const Ogre::String &resourceGroup) :
		mSkinName(skinName),
		mResourceGroup(resourceGroup),
		mNumIndividualTextures(0),
		mTextureWidth(1024),
		mTextureHeight(1024),
		mTextureName("SkinSet." + mSkinName + ".png")
	{
		mTexture.setNull();

		// Commented out so people can use latest revision
//		if (loadSkin() == false)
//		{
			_findSkinTextures();
			buildTexture();
//			saveSkin();
//		}
	}
	SkinSet::~SkinSet()
	{
		// Remove Texture for this SkinSet from Texture Manager
		Ogre::TextureManager::getSingleton().remove(mTexture->getName());

		mTexture.setNull();
	}

	bool SkinSet::loadSkin()
	{
		ConfigNode *skinRootNode = ConfigScriptLoader::getSingleton().getConfigScript("skinset", mSkinName);
		if (skinRootNode)
		{
			ConfigNode *size = skinRootNode->findChild("size");
			if (size)
			{
				mTextureWidth = Ogre::StringConverter::parseInt(size->getValues()[0]);
				mTextureHeight = Ogre::StringConverter::parseInt(size->getValues()[1]);
			}

			ConfigNode *texName = skinRootNode->findChild("texture");
			if (texName)
			{
				mTextureName = texName->getValues()[0];
			}
			try
			{
				mTexture = Ogre::TextureManager::getSingleton().load(mTextureName, mResourceGroup);
				if (!mTexture.isNull())
				{
					std::vector<ConfigNode*> children = skinRootNode->getChildren();
					std::vector<ConfigNode*>::iterator it;
					for (it = children.begin(); it != children.end(); ++it)
					{
						ConfigNode* currNode = *it;
						if (currNode->getName() == "element")
						{
							const Ogre::String elementName = currNode->getValues()[0];
							ConfigNode *dimension = currNode->findChild("dimension");
							Ogre::Vector4 texCoord;
							if (dimension)
							{
								texCoord.x = Ogre::StringConverter::parseReal(dimension->getValues()[0]);
								texCoord.y = Ogre::StringConverter::parseReal(dimension->getValues()[1]);
								texCoord.z = Ogre::StringConverter::parseReal(dimension->getValues()[2]);
								texCoord.w = Ogre::StringConverter::parseReal(dimension->getValues()[3]);
							}

							//ConfigNode *rotation = skinRootNode->findChild("rotation");
							//if (rotation)
							//{
							//   rotation.x = Ogre::StringConverter::parseReal(rotation->getValues()[0]);
							//   rotation.y = Ogre::StringConverter::parseReal(rotation->getValues()[1]);
							//   rotation.z = Ogre::StringConverter::parseReal(rotation->getValues()[2]);
							//}

							const Ogre::String texName (mSkinName + "." + elementName + ".png");

							mTextureMap[texName] = texCoord;
							mContainedTextures.insert(texName);
							mTextureNames.push_back(texName);
						}
					}
					return true;
				}
			}
			catch (...)
			{
				return false;				
			}
		}
		return false;
	}

	void SkinSet::saveSkin()
	{
		ConfigScriptSerializer serializer;
		serializer.writeAttribute(0, "skinset " + mSkinName);
		serializer.beginSection(0);
		{
			{
				serializer.writeAttribute(1, "size ");
				{
					serializer.writeValue(Ogre::StringConverter::toString(mTextureWidth));
					serializer.writeValue(Ogre::StringConverter::toString(mTextureHeight));
				}
			}

			{
				serializer.writeAttribute(1, "texture ");
				{
					serializer.writeValue(mTextureName);
				}
			}

			std::map<Ogre::String, Ogre::Vector4>::iterator it;
			for (it = mTextureMap.begin(); it != mTextureMap.end(); ++it)
			{
				const Ogre::String texName (it->first);

				Ogre::StringVector nameParts = Ogre::StringUtil::split(texName, ".");
				Ogre::String elementName;
				Ogre::StringVector::iterator itTexName;
				if (nameParts.size() > 2)
				{
					elementName = nameParts[1];
					for (itTexName = nameParts.begin() + 2; itTexName < nameParts.end() - 1; ++itTexName)
						elementName = elementName +  "." + *itTexName;// sure some clever STL would do this better.

				}
				serializer.writeAttribute(1, "element " + elementName);
				serializer.beginSection(1);
				{
					serializer.writeAttribute(2, "dimension ");
					{
						const Ogre::Vector4 val (it->second);
						serializer.writeValue(Ogre::StringConverter::toString(val.x));
						serializer.writeValue(Ogre::StringConverter::toString(val.y));
						serializer.writeValue(Ogre::StringConverter::toString(val.z));
						serializer.writeValue(Ogre::StringConverter::toString(val.w));
					}
				}
				serializer.endSection(1);
			}
		}
		serializer.endSection(0);

		// Saves SkinSet texture
		Ogre::Image finalImageSet;
		//finalImageSet.load("QuickGUI.output.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		const size_t buffSize = mTextureWidth*mTextureHeight*4;
		unsigned char *data = new unsigned char[buffSize];
		finalImageSet.loadDynamicImage(data, mTextureWidth, mTextureHeight, 1, Ogre::PF_R8G8B8A8, true);
		assert (!mTexture.isNull());
		Ogre::HardwarePixelBufferSharedPtr buf = mTexture->getBuffer();
		Ogre::Image::Box textureRect(0, 0, 0, 0, 0, 1);
		textureRect.right = mTextureWidth;
		textureRect.bottom = mTextureHeight;
 		const Ogre::PixelBox srcBox = finalImageSet.getPixelBox();
 		const Ogre::PixelBox lock = buf->lock(textureRect, Ogre::HardwareBuffer::HBL_DISCARD); 
 		Ogre::PixelUtil::bulkPixelConversion(lock, srcBox); 
		buf->unlock();  	

		Ogre::String resourcePath;
		{
			Ogre::ResourceGroupManager* rgm = Ogre::ResourceGroupManager::getSingletonPtr();
			Ogre::StringVector resourceGroupNames = rgm->getResourceGroups();

			Ogre::StringVector::iterator groupItr;
			for( groupItr = resourceGroupNames.begin(); groupItr != resourceGroupNames.end(); ++groupItr )
			{
				Ogre::FileInfoListPtr files = rgm->findResourceFileInfo((*groupItr),mSkinName + "*");

				if(!files->empty()) 
				{
					const Ogre::FileInfo &fileInfo = *(files->begin());
					resourcePath = fileInfo.archive->getName();
					break;
				}
			}
		}
		resourcePath = resourcePath + '/';
		serializer.exportQueued(resourcePath + mSkinName + ".skinset");
		finalImageSet.save(resourcePath + mTextureName);
		
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
			Ogre::LogManager::getSingletonPtr()->logMessage("Quickgui : Adding  " + (*texNameItr) + "to skin" + mSkinName);

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
				mTextureName,
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
			Ogre::Image *img = &images.at(i);
			// See if we can still draw on this row, or if we need a new one
			if( (cur_x + img->getWidth() ) < mTexture->getWidth() )
			{
				// Keep track of tallest image in the row
				if( next_y < (int)img->getHeight() + cur_y )
					next_y = (int)img->getHeight() + cur_y + 1;

				// Not enough room for all the images!
				if( next_y > (int)mTexture->getHeight() )
				{
					Ogre::Image finalImageSet;
					finalImageSet.load("QuickGUI.output.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					buf->blitToMemory(finalImageSet.getPixelBox());
					finalImageSet.save(mTextureName);
		
					throw Ogre::Exception(1, "The texture" + mTextureNames.at(i)  + " is not large enough to fit all widget images.  Please view \"" + mTextureName + "\"", "QuickGUIImageSet");
				}
				
				// Blit to the specified location on the texture
				buf->blitFromMemory(img->getPixelBox(),
					Ogre::Image::Box(cur_x, cur_y, img->getWidth()+cur_x, img->getHeight()+cur_y));

				// Store the image name associated with its Texture Coordinates (UV).  
				// UV coordinates are float values between 0 and 1.
				mTextureMap[mTextureNames.at(i)] = 
					Ogre::Vector4(
						(float)cur_x / (float)mTextureWidth, 
						(float)cur_y / (float)mTextureHeight, 
						(float)(img->getWidth() + cur_x) / (float)mTextureWidth, 
						(float)(img->getHeight() + cur_y) / (float)mTextureWidth 
						);
				
				// Shift x over the correct amount
				cur_x += (int)img->getWidth() + 1;
			}
			// New Row
			else
			{
				// Reset variables for drawing the next row
				cur_x = 0;
				cur_y = next_y;

				// Keep track of the tallest image in the row
				if( next_y < (int)img->getHeight() + cur_y )
					next_y = (int)img->getHeight() + cur_y + 1;

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
				buf->blitFromMemory(img->getPixelBox(),
					Ogre::Image::Box(cur_x, cur_y, img->getWidth()+cur_x, img->getHeight()+cur_y));
				
				// Store the image name associated with its Texture Coordinates (UV).  
				// UV coordinates are float values between 0 and 1.
				mTextureMap[mTextureNames.at(i)] = 
					Ogre::Vector4(
						(float)cur_x / (float)mTextureWidth, 
						(float)cur_y / (float)mTextureHeight, 
						(float)(img->getWidth() + cur_x) / (float)mTextureWidth, 
						(float)(img->getHeight() + cur_y) / (float)mTextureWidth 
						);
				
				// Shift x over the correct amount
				cur_x += (int)img->getWidth() + 1;
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

	Ogre::String SkinSet::getSkinName()
	{
		return mSkinName;
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
