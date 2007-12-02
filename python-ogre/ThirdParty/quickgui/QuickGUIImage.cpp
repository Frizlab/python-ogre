#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIImage.h"
#include "QuickGUIUtility.h" 

namespace QuickGUI
{
	Image::Image(const Ogre::String& name, GUIManager* gm) :
		Widget(name,gm),
		mTextureName("")
	{
		mWidgetType = TYPE_IMAGE;
		mSkinComponent = ".image";
		mSize = Size(50,50);
	}

	Image::~Image()
	{
	}

	Ogre::String Image::getTextureName()
	{
		return mTextureName;
	}

	void Image::setTexture(const Ogre::String& textureName)
	{
		if(mTextureLocked)
			return;

		mTextureName = textureName;
		mQuad->setTexture(mTextureName);

		if(Utility::textureExistsOnDisk(mTextureName))
		{
			Ogre::Image i;
			i.load(mTextureName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			delete mWidgetImage;
			mWidgetImage = new Ogre::Image(i);
		}
	}
}
