#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUITextArea.h"

namespace QuickGUI
{
	TextArea::TextArea(const std::string& name, GUIManager* gm) :
		LabelArea(name,gm)
	{
		mWidgetType = TYPE_TEXTAREA;
		mSkinComponent = ".textarea";
	}

	TextArea::~TextArea()
	{
	}
}
