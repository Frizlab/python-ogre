#include "QuickGUIButton.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String Button::DEFAULT = "default";
	const Ogre::String Button::OVER = "over";
	const Ogre::String Button::DOWN = "down";

	void Button::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("Button");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(DOWN);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Button",d);
	}

	ButtonDesc::ButtonDesc() :
		LabelDesc()
	{
	}

	void ButtonDesc::serialize(SerialBase* b)
	{
		LabelDesc::serialize(b);
	}

	Button::Button(const Ogre::String& name) :
		Label(name),
		mCurrentButtonState(BUTTON_STATE_DEFAULT)
	{
		mSkinElementName = DEFAULT;
	}

	Button::~Button()
	{
	}

	void Button::_initialize(WidgetDesc* d)
	{
		Label::_initialize(d);

		mDesc = dynamic_cast<LabelDesc*>(mWidgetDesc);

		setSkinType(d->skinTypeName);

		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&Button::onMouseEnter,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&Button::onMouseLeave,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&Button::onMouseLeftButtonDown,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&Button::onMouseLeftButtonUp,this);
	}

	Widget* Button::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new Button(widgetName);
		
		newWidget->_createDescObject("ButtonDesc");

		return newWidget;
	}

	void Button::addButtonEventHandler(ButtonEvent EVENT, EventHandlerSlot* function)
	{
		mButtonEventHandlers[EVENT].push_back(function);
	}

	Ogre::String Button::getClass()
	{
		return "Button";
	}

	bool Button::fireButtonEvent(ButtonEvent e, EventArgs& args)
	{
		if(mButtonEventHandlers[e].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mButtonEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	ButtonState Button::getState()
	{
		return mCurrentButtonState;
	}

	void Button::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		switch(mCurrentButtonState)
		{
		case BUTTON_STATE_DEFAULT:		mSkinElementName = DEFAULT;		break;
		case BUTTON_STATE_DOWN:			mSkinElementName = DOWN;		break;
		case BUTTON_STATE_OVER:			mSkinElementName = OVER;		break;
		}

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect clipRegion;
		clipRegion.size = 
			Size(
				mDesc->dimensions.size.width - mDesc->padding[PADDING_RIGHT],
				mDesc->dimensions.size.height - mDesc->padding[PADDING_BOTTOM]);
		clipRegion.position = mTexturePosition;
		clipRegion.translate(Point(mDesc->padding[PADDING_LEFT],mDesc->padding[PADDING_TOP]));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);

		Brush::getSingleton().setColor(prevColor);
	}

	void Button::onMouseEnter(const EventArgs& args)
	{
		setState(BUTTON_STATE_OVER);
	}

	void Button::onMouseLeave(const EventArgs& args)
	{
		setState(BUTTON_STATE_DEFAULT);
	}

	void Button::onMouseLeftButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			setState(BUTTON_STATE_DOWN);
	}

	void Button::onMouseLeftButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			setState(BUTTON_STATE_OVER);
	}

	void Button::setState(ButtonState s)
	{
		if(mCurrentButtonState == s)
			return;

		mCurrentButtonState = s;
		
		redraw();

		WidgetEventArgs args(this);
		fireButtonEvent(BUTTON_EVENT_STATE_CHANGED, args);
	}
}
