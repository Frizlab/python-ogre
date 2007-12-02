#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUICheckBox.h"

namespace QuickGUI
{
	CheckBox::CheckBox(const Ogre::String& name, GUIManager* gm) :
		Button(name,gm),
		mChecked(false)
	{
		mWidgetType = TYPE_CHECKBOX;
		mSkinComponent = ".checkbox.unchecked";
		mSize = Size(75,25);
	}

	CheckBox::~CheckBox()
	{
	}

	void CheckBox::addOnCheckChangedEventHandler(MemberFunctionSlot* function)
	{
		mOnCheckChangedUserEventHandlers.push_back(function);
	}

	void CheckBox::onMouseButtonDown(const EventArgs& args)
	{
		Button::onMouseButtonDown(args);

		mChecked = !mChecked;
		
		// change skin component and apply skin
		if(mChecked)
			mSkinComponent = ".checkbox.checked";
		else
			mSkinComponent = ".checkbox.unchecked";

		setSkin(mSkinName,true);

		// Event has been fired, call user defined handlers.
		EventHandlerArray::iterator it;
		for( it = mOnCheckChangedUserEventHandlers.begin(); it != mOnCheckChangedUserEventHandlers.end(); ++it )
			(*it)->execute(args);
	}
}
