#ifndef QUICKGUIMULTILINETEXTBOX_H
#define QUICKGUIMULTILINETEXTBOX_H

#include "QuickGUIMultiLineLabel.h"

namespace QuickGUI
{
	class _QuickGUIExport MultiLineTextBox :
		public MultiLineLabel
	{
	public:
		MultiLineTextBox(const Ogre::String& instanceName, Size pixelSize, Ogre::String textureName, GUIManager* gm);
		~MultiLineTextBox();

	protected:
	};
}

#endif
