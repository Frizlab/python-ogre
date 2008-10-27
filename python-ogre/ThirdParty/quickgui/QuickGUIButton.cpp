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
		Label(name)
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

	Ogre::String Button::getClass()
	{
		return "Button";
	}

	void Button::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect clipRegion;
		clipRegion.size = 
			Size(
				mWidgetDesc->dimensions.size.width - mDesc->padding[PADDING_RIGHT] - mDesc->padding[PADDING_LEFT],
				mWidgetDesc->dimensions.size.height - mDesc->padding[PADDING_BOTTOM] - mDesc->padding[PADDING_TOP]);
		clipRegion.position = mTexturePosition;
		clipRegion.translate(Point(mDesc->padding[PADDING_LEFT],mDesc->padding[PADDING_TOP]));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void Button::onMouseEnter(const EventArgs& args)
	{
		mSkinElementName = OVER;

		redraw();
	}

	void Button::onMouseLeave(const EventArgs& args)
	{
		mSkinElementName = DEFAULT;

		redraw();
	}

	void Button::onMouseLeftButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			mSkinElementName = DOWN;
			redraw();
		}
	}

	void Button::onMouseLeftButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			mSkinElementName = OVER;
			redraw();
		}
	}
}
