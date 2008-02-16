#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIRadioButtonGroup.h"

namespace QuickGUI
{
	RadioButtonGroup::RadioButtonGroup(Widget* owner) :
		mOwner(owner)
	{
		mOwner->addEventHandler(Widget::EVENT_CHILD_REMOVED,&RadioButtonGroup::onRadioButtonRemoved,this);
	}

	RadioButtonGroup::~RadioButtonGroup()
	{
	}

	RadioButton* RadioButtonGroup::createRadioButton()
	{
		return createRadioButton(mOwner->getGUIManager()->generateName(Widget::TYPE_RADIOBUTTON));
	}

	RadioButton* RadioButtonGroup::createRadioButton(const std::string& name)
	{
		GUIManager* gm = mOwner->getGUIManager();
		if(gm->isNameUnique(name))
		{
			gm->notifyNameUsed(name);
		}
		else
		{
			std::string name = gm->generateName(Widget::TYPE_RADIOBUTTON);
			gm->notifyNameUsed(name);
		}

		RadioButton* rb = dynamic_cast<RadioButton*>(mOwner->_createChild(name,Widget::TYPE_RADIOBUTTON));
		mRadioButtons.push_back(rb);

		return rb;
	}

	RadioButton* RadioButtonGroup::getSelectedRadioButton()
	{
		return mSelectedRadioButton;
	}

	void RadioButtonGroup::onRadioButtonRemoved(const EventArgs& args)
	{
		const WidgetEventArgs& wea = dynamic_cast<const WidgetEventArgs&>(args);

		for(std::vector<RadioButton*>::iterator it = mRadioButtons.begin(); it != mRadioButtons.end(); ++it)
		{
			if((*it) == wea.widget)
			{
				mRadioButtons.erase(it);
				return;
			}
		}
	}

	void RadioButtonGroup::setSelectedRadioButton(RadioButton* b)
	{
		for(std::vector<RadioButton*>::iterator it = mRadioButtons.begin(); it != mRadioButtons.end(); ++it)
		{
			(*it)->_deselect();
		}

		mSelectedRadioButton = b;

		mSelectedRadioButton->_select();
	}
}
