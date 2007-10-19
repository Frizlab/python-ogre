#include "QuickGUIMultiLineTextBox.h"

namespace QuickGUI
{
	MultiLineTextBox::MultiLineTextBox(const Ogre::String& instanceName, Size pixelSize, Ogre::String textureName, GUIManager* gm) :
		MultiLineLabel(instanceName,pixelSize,textureName,gm)
	{
		mWidgetType = TYPE_MULTILINETEXTBOX;
	}

	MultiLineTextBox::~MultiLineTextBox()
	{
	}
}
