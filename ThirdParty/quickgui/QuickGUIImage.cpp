#include "QuickGUIImage.h"

namespace QuickGUI
{
	Image::Image(const Ogre::String& name, Type type, const Rect& pixelDimensions, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Widget(name,type,pixelDimensions,texture,container,ParentWidget,gm)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_IMAGE)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
		}
	}

	Image::~Image()
	{
	}
}