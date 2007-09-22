#include "QuickGUIImage.h"

namespace QuickGUI
{
	Image::Image(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Widget(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm)
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