#include "QuickGUICheckBox.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String CheckBox::CHECKED = "checked";
	const Ogre::String CheckBox::CHECKED_DOWN = "checked_down";
	const Ogre::String CheckBox::CHECKED_OVER = "checked_over";
	const Ogre::String CheckBox::UNCHECKED = "unchecked";
	const Ogre::String CheckBox::UNCHECKED_DOWN = "unchecked_down";
	const Ogre::String CheckBox::UNCHECKED_OVER = "unchecked_over";

	void CheckBox::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("CheckBox");
		d->defineSkinElement(CHECKED);
		d->defineSkinElement(CHECKED_DOWN);
		d->defineSkinElement(CHECKED_OVER);
		d->defineSkinElement(UNCHECKED);
		d->defineSkinElement(UNCHECKED_DOWN);
		d->defineSkinElement(UNCHECKED_OVER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("CheckBox",d);
	}

	CheckBoxDesc::CheckBoxDesc(const Ogre::String& id) :
		WidgetDesc(id)
	{
		resetToDefault();
	}

	void CheckBoxDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		checkbox_checked = false;
	}

	void CheckBoxDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("Checked",&checkbox_checked);
	}

	CheckBox::CheckBox(const Ogre::String& name) :
		Widget(name)
	{
		mSkinElementName = UNCHECKED;

		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&CheckBox::onMouseEnter,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&CheckBox::onMouseLeave,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&CheckBox::onMouseLeftButtonDown,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&CheckBox::onMouseLeftButtonUp,this);
	}

	CheckBox::~CheckBox()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < CHECKBOX_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mCheckBoxEventHandlers[index].begin(); it != mCheckBoxEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void CheckBox::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<CheckBoxDesc*>(mWidgetDesc);

		CheckBoxDesc* cbd = dynamic_cast<CheckBoxDesc*>(d);

		setSkinType(d->widget_skinTypeName);
		setChecked(cbd->checkbox_checked);
	}

	void CheckBox::addCheckBoxEventHandler(CheckBoxEvent EVENT, EventHandlerSlot* function)
	{
		mCheckBoxEventHandlers[EVENT].push_back(function);
	}

	Ogre::String CheckBox::getClass()
	{
		return "CheckBox";
	}

	bool CheckBox::fireCheckBoxEvent(CheckBoxEvent e, EventArgs& args)
	{
		if(mCheckBoxEventHandlers[e].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mCheckBoxEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	bool CheckBox::getChecked()
	{
		return mDesc->checkbox_checked;
	}

	void CheckBox::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void CheckBox::onMouseEnter(const EventArgs& args)
	{
		if(mDesc->checkbox_checked)
			mSkinElementName = CHECKED_OVER;
		else
			mSkinElementName = UNCHECKED_OVER;

		redraw();
	}

	void CheckBox::onMouseLeave(const EventArgs& args)
	{
		if(mDesc->checkbox_checked)
			mSkinElementName = CHECKED;
		else
			mSkinElementName = UNCHECKED;

		redraw();
	}

	void CheckBox::onMouseLeftButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(mDesc->checkbox_checked)
				mSkinElementName = CHECKED_DOWN;
			else
				mSkinElementName = UNCHECKED_DOWN;

			redraw();
		}
	}

	void CheckBox::onMouseLeftButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			mDesc->checkbox_checked = !mDesc->checkbox_checked;

			if(mDesc->checkbox_checked)
				mSkinElementName = CHECKED_OVER;
			else
				mSkinElementName = UNCHECKED_OVER;

			redraw();

			WidgetEventArgs wea(this);
			fireCheckBoxEvent(CHECKBOX_EVENT_CHECK_CHANGED,wea);
		}
	}

	void CheckBox::setChecked(bool checked)
	{
		if(mDesc->checkbox_checked == checked)
			return;

		mDesc->checkbox_checked = checked;

		if(mDesc->checkbox_checked)
			mSkinElementName = CHECKED;
		else
			mSkinElementName = UNCHECKED;

		redraw();

		WidgetEventArgs wea(this);
		fireCheckBoxEvent(CHECKBOX_EVENT_CHECK_CHANGED,wea);
	}
}
