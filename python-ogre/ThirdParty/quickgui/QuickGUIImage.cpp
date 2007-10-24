#include "QuickGUIImage.h"

namespace QuickGUI
{
	Image::Image(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String texture, GUIManager* gm) :
		Widget(instanceName,pixelSize,texture,gm)
	{
		mWidgetType = TYPE_IMAGE;
		mSkinComponent = ".image";
	}

	Image::~Image()
	{
	}
}