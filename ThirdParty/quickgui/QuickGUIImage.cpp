#include "QuickGUIImage.h"

namespace QuickGUI
{
	Image::Image(const Ogre::String& name, const Rect& pixelDimensions, Ogre::String texture, GUIManager* gm) :
		Widget(name,pixelDimensions,texture,gm)
	{
		mWidgetType = TYPE_IMAGE;
	}

	Image::~Image()
	{
	}
}