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
		std::string name = "RadioButton";
		int counter = 1;
		int beginCounter = 0;
		while (beginCounter != counter )
		{
			beginCounter = counter;
			for(std::vector<RadioButton*>::iterator it = mRadioButtons.begin(); it != mRadioButtons.end(); ++it)
			{
				if((*it)->getInstanceName() == name)
				{
					name = name + Ogre::StringConverter::toString(counter);
					counter++;
					break;
				}
			}
		}
		return createRadioButton(name);
	}

	RadioButton* RadioButtonGroup::createRadioButton(std::string& name)
	{

		int counter = 1;
		int beginCounter = 0;
		while (beginCounter != counter )
		{
			beginCounter = counter;
			for(std::vector<RadioButton*>::iterator it = mRadioButtons.begin(); it != mRadioButtons.end(); ++it)
			{
				if((*it)->getInstanceName() == name)
				{
					name = name + Ogre::StringConverter::toString(counter);
					counter++;
					break;
				}
			}
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
