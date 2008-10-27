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
		SkinDefinition* d = new SkinDefinition("CheckBox");
		d->defineSkinElement(CHECKED);
		d->defineSkinElement(CHECKED_DOWN);
		d->defineSkinElement(CHECKED_OVER);
		d->defineSkinElement(UNCHECKED);
		d->defineSkinElement(UNCHECKED_DOWN);
		d->defineSkinElement(UNCHECKED_OVER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("CheckBox",d);
	}

	CheckBoxDesc::CheckBoxDesc() :
		WidgetDesc()
	{
		checked = false;
	}

	void CheckBoxDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("Checked",&checked);
	}

	CheckBox::CheckBox(const Ogre::String& name) :
		Widget(name)
	{
		mSkinElementName = UNCHECKED;
	}

	CheckBox::~CheckBox()
	{
	}

	void CheckBox::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<CheckBoxDesc*>(mWidgetDesc);

		CheckBoxDesc* cbd = dynamic_cast<CheckBoxDesc*>(d);

		setSkinType(d->skinTypeName);
		setChecked(cbd->checked);

		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&CheckBox::onMouseEnter,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&CheckBox::onMouseLeave,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&CheckBox::onMouseLeftButtonDown,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&CheckBox::onMouseLeftButtonUp,this);
	}

	Widget* CheckBox::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new CheckBox(widgetName);
		
		newWidget->_createDescObject("CheckBoxDesc");

		return newWidget;
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
		return mDesc->checked;
	}

	void CheckBox::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void CheckBox::onMouseEnter(const EventArgs& args)
	{
		if(mDesc->checked)
			mSkinElementName = CHECKED_OVER;
		else
			mSkinElementName = UNCHECKED_OVER;

		redraw();
	}

	void CheckBox::onMouseLeave(const EventArgs& args)
	{
		if(mDesc->checked)
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
			if(mDesc->checked)
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
			mDesc->checked = !mDesc->checked;

			if(mDesc->checked)
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
		if(mDesc->checked == checked)
			return;

		mDesc->checked = checked;

		if(mDesc->checked)
			mSkinElementName = CHECKED;
		else
			mSkinElementName = UNCHECKED;

		redraw();

		WidgetEventArgs wea(this);
		fireCheckBoxEvent(CHECKBOX_EVENT_CHECK_CHANGED,wea);
	}
}
