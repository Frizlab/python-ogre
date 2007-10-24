#include "QuickGUITextArea.h"

namespace QuickGUI
{
	TextArea::TextArea(const Ogre::String& instanceName, Size pixelSize, Ogre::String textureName, GUIManager* gm) :
		LabelArea(instanceName,pixelSize,textureName,gm)
	{
		mWidgetType = TYPE_TEXTAREA;
		mSkinComponent = ".textarea";
	}

	TextArea::~TextArea()
	{
	}
}
