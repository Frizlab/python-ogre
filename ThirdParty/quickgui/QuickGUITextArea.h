#ifndef QUICKGUIMULTILINETEXTBOX_H
#define QUICKGUIMULTILINETEXTBOX_H

#include "QuickGUILabelArea.h"

namespace QuickGUI
{
	class _QuickGUIExport TextArea :
		public LabelArea
	{
	public:
		TextArea(const Ogre::String& instanceName, Size pixelSize, Ogre::String textureName, GUIManager* gm);
		~TextArea();

	protected:
	};
}

#endif
