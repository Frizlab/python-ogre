#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIRadioButton.h"
#include "QuickGUIRadioButtonGroup.h"

namespace QuickGUI
{
	RadioButton::RadioButton(const std::string& name, GUIManager* gm) :
		Button(name,gm),
		mSelected(false)
	{
		mWidgetType = TYPE_RADIOBUTTON;
		mSkinComponent = ".radiobutton.unselected";
		mSize = Size(75,25);

		addEventHandler(EVENT_MOUSE_BUTTON_UP,&RadioButton::onMouseButtonUp,this);
	}

	RadioButton::~RadioButton()
	{
	}

	void RadioButton::_select()
	{
		if(mSelected)
			return;

		mSelected = true;
		mSkinComponent = ".radiobutton.selected";
		setSkin(mSkinName,true);

		WidgetEventArgs args(this);
		// Event has been fired, call user defined handlers.
		EventHandlerArray::iterator it;
		for( it = mOnSelectedUserEventHandlers.begin(); it != mOnSelectedUserEventHandlers.end(); ++it )
			(*it)->execute(args);
	}

	void RadioButton::_deselect()
	{
		if(!mSelected)
			return;

		mSelected = false;
		mSkinComponent = ".radiobutton.unselected";
		setSkin(mSkinName,true);
	}

	void RadioButton::addOnSelectedEventHandler(MemberFunctionSlot* function)
	{
		mOnSelectedUserEventHandlers.push_back(function);
	}

	bool RadioButton::isSelected()
	{
		return mSelected;
	}

	void RadioButton::onMouseButtonUp(const EventArgs& args)
	{
		select();
	}

	void RadioButton::select()
	{
		dynamic_cast<RadioButtonGroup*>(mParentWidget)->setSelectedRadioButton(this);
	}
}
