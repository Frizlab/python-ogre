#include "QuickGUIHScrollBar.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	const Ogre::String HScrollBar::BAR = "bar";
	const Ogre::String HScrollBar::LEFT1 = "left1";
	const Ogre::String HScrollBar::LEFT2 = "left2";
	const Ogre::String HScrollBar::RIGHT1 = "right1";
	const Ogre::String HScrollBar::RIGHT2 = "right2";
	const Ogre::String HScrollBar::SLIDER = "slider";

	void HScrollBar::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("HScrollBar");
		d->defineSkinElement(BAR);
		d->defineComponent(LEFT1);
		d->defineComponent(LEFT2);
		d->defineComponent(RIGHT1);
		d->defineComponent(RIGHT2);
		d->defineComponent(SLIDER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("HScrollBar",d);
	}

	HScrollBarDesc::HScrollBarDesc() :
		ComponentWidgetDesc()
	{
		buttonScrollPercent = 0.1;
		barScrollPercent = 0.2;
		scrollBarButtonLayout = HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		sliderWidth = 15;
		sliderPercentage = 0;
	}

	void HScrollBarDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);

		b->IO("ButtonScrollPercent",&buttonScrollPercent);
		b->IO("BarScrollPercent",&barScrollPercent);
		b->IO("ScrollBarButtonLayout",&scrollBarButtonLayout);
		b->IO("SliderWidth",&sliderWidth);
		b->IO("SliderPercentage",&sliderPercentage);
	}

	HScrollBar::HScrollBar(const Ogre::String& name) :
		ComponentWidget(name),
		mButton_Left1(NULL),
		mButton_Left2(NULL),
		mButton_Right1(NULL),
		mButton_Right2(NULL),
		mButton_Slider(NULL),
		mLive(false)
	{
		mSkinElementName = BAR;
	}

	HScrollBar::~HScrollBar()
	{
	}

	void HScrollBar::_initialize(WidgetDesc* d)
	{
		mDesc = dynamic_cast<HScrollBarDesc*>(mWidgetDesc);

		Widget::_initialize(d);

		HScrollBarDesc* hsd = dynamic_cast<HScrollBarDesc*>(d);

		mDesc->barScrollPercent = hsd->barScrollPercent;
		mDesc->buttonScrollPercent = hsd->buttonScrollPercent;

		ButtonDesc bd;
		bd.resizable = false;
		bd.verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
		bd.dimensions.size.width = mWidgetDesc->dimensions.size.height;
		bd.dimensions.size.height = mWidgetDesc->dimensions.size.height;

		mButton_Left1 = dynamic_cast<Button*>(mWidgetDesc->guiManager->createWidget("Button",bd));
		mButton_Left1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onLeftClicked,this);
		addComponent(LEFT1,mButton_Left1);

		bd.dimensions.position.x = 15;
		mButton_Right1 = dynamic_cast<Button*>(mWidgetDesc->guiManager->createWidget("Button",bd));
		mButton_Right1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onRightClicked,this);
		addComponent(RIGHT1,mButton_Right1);

		bd.horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
		bd.dimensions.position.x = mWidgetDesc->dimensions.size.width - (2 * bd.dimensions.size.width);
		mButton_Left2 = dynamic_cast<Button*>(mWidgetDesc->guiManager->createWidget("Button",bd));
		mButton_Left2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onLeftClicked,this);
		addComponent(LEFT2,mButton_Left2);

		bd.dimensions.position.x = mWidgetDesc->dimensions.size.width - bd.dimensions.size.width;
		mButton_Right2 = dynamic_cast<Button*>(mWidgetDesc->guiManager->createWidget("Button",bd));
		mButton_Right2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onRightClicked,this);
		addComponent(RIGHT2,mButton_Right2);

		bd.horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
		bd.dimensions.position.x = 30;
		bd.dragable = true;
		mButton_Slider = dynamic_cast<Button*>(mWidgetDesc->guiManager->createWidget("Button",bd));
		addComponent(SLIDER,mButton_Slider);

		mButton_Slider->addWidgetEventHandler(WIDGET_EVENT_DRAGGED,&HScrollBar::onSliderDragged,this);
		addWidgetEventHandler(WIDGET_EVENT_SIZE_CHANGED,&HScrollBar::onSizeChanged,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onBarClicked,this);

		setButtonLayout(hsd->scrollBarButtonLayout);
		setSkinType(hsd->skinTypeName);
		setPercentage(hsd->sliderPercentage);
	}

	void HScrollBar::_setPercentage(float percentage)
	{
		float pixelAmount = percentage * (mSliderBounds.y - mSliderBounds.x);

		if(percentage <= 0)
			mDesc->sliderPercentage = 0;
		else if(percentage > ((mSliderBounds.y - mButton_Slider->getWidth() - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x)))
			mDesc->sliderPercentage = (mSliderBounds.y - mButton_Slider->getWidth() - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x);
		else
			mDesc->sliderPercentage = percentage;

		_updateSlider();
	}

	void HScrollBar::_updateButtons()
	{
		if(mSkinType == NULL)
		{
			mSliderBounds.x = 0;
			mSliderBounds.y = mWidgetDesc->dimensions.size.width;

			mButton_Left1->setVisible(false);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(false);
			return;
		}

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);

		switch(mDesc->scrollBarButtonLayout)
		{
		case HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT:
			mSliderBounds.x = se->getBorderThickness(BORDER_LEFT);
			mSliderBounds.y = mButton_Left2->getPosition().x;
			
			mButton_Left1->setVisible(false);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(true);
			mButton_Right2->setVisible(true);
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT:
			mSliderBounds.x = (mButton_Right1->getPosition().x + mButton_Right1->getWidth());
			mSliderBounds.y = mWidgetDesc->dimensions.size.width - se->getBorderThickness(BORDER_RIGHT);
			
			mButton_Left1->setVisible(true);
			mButton_Right1->setVisible(true);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(false);
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH:
			mSliderBounds.x = (mButton_Right1->getPosition().x + mButton_Right1->getWidth());
			mSliderBounds.y = mButton_Left2->getPosition().x;

			mButton_Left1->setVisible(true);
			mButton_Right1->setVisible(true);
			mButton_Left2->setVisible(true);
			mButton_Right2->setVisible(true);
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT:
			mSliderBounds.x = (mButton_Right1->getPosition().x + mButton_Right1->getWidth());
			mSliderBounds.y = mButton_Right2->getPosition().x;

			mButton_Left1->setVisible(true);
			mButton_Right1->setVisible(true);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(true);
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT:
			mSliderBounds.x = mButton_Right1->getPosition().x;
			mSliderBounds.y = mButton_Left2->getPosition().x;

			mButton_Left1->setVisible(true);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(true);
			mButton_Right2->setVisible(true);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_NONE:
			mSliderBounds.x = se->getBorderThickness(BORDER_LEFT);
			mSliderBounds.y = mWidgetDesc->dimensions.size.width - se->getBorderThickness(BORDER_RIGHT);

			mButton_Left1->setVisible(false);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(false);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE:
			mSliderBounds.x = mButton_Right1->getPosition().x;
			mSliderBounds.y = mButton_Right2->getPosition().x;

			mButton_Left1->setVisible(true);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(true);
			break;
		}
	}

	void HScrollBar::_updateSlider()
	{
		// update position
		float yPos = 0;
		if(mSkinType != NULL)
			yPos = mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_TOP);
		mButton_Slider->setPosition(Point(mSliderBounds.x + ((mSliderBounds.y - mSliderBounds.x) * mDesc->sliderPercentage),yPos));

		// if live, updates size
		if(mLive)
			mButton_Slider->setWidth( ((*mLiveValue1) / (*mLiveValue2)) * (mSliderBounds.y - mSliderBounds.x) );

		mDesc->sliderWidth = mButton_Slider->getWidth();

		// determine if Slider is visible (if it can fit in bounds)
		if((mSliderBounds.y - mSliderBounds.x) < mButton_Slider->getWidth())
			mButton_Slider->setVisible(false);
		else
			mButton_Slider->setVisible(true);

		fireScrollBarEvent(SCROLLBAR_EVENT_ON_SCROLLED,WidgetEventArgs(this));
	}

	void HScrollBar::addScrollBarEventHandler(ScrollBarEvent EVENT, EventHandlerSlot* function)
	{
		mScrollBarEventHandlers[EVENT].push_back(function);
	}

	void HScrollBar::disableLiveSlider()
	{
		mLive = false;
	}

	void HScrollBar::enableLiveSlider(float* value1, float* value2)
	{
		mLive = true;
		mLiveValue1 = value1;
		mLiveValue2 = value2;

		_updateSlider();
	}

	Widget* HScrollBar::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new HScrollBar(widgetName);

		newWidget->_createDescObject("HScrollBarDesc");

		return newWidget;
	}

	bool HScrollBar::fireScrollBarEvent(ScrollBarEvent e, EventArgs& args)
	{
		if(mScrollBarEventHandlers[e].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mScrollBarEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	float HScrollBar::getBarScrollPercent()
	{
		return mDesc->barScrollPercent;
	}

	HScrollBarButtonLayout HScrollBar::getButtonLayout()
	{
		return mDesc->scrollBarButtonLayout;
	}

	float HScrollBar::getButtonScrollPercent()
	{
		return mDesc->buttonScrollPercent;
	}

	Ogre::String HScrollBar::getClass()
	{
		return "HScrollBar";
	}

	float HScrollBar::getPercentage()
	{
		return mDesc->sliderPercentage;
	}

	float HScrollBar::getSliderWidthPercentage()
	{
		return (mButton_Slider->getWidth() / (mSliderBounds.y - mSliderBounds.x));
	}

	void HScrollBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);
		
		mSkinElementName = BAR;
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void HScrollBar::onLeftClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			_setPercentage(mDesc->sliderPercentage - mDesc->buttonScrollPercent);
		else
			_setPercentage(mDesc->sliderPercentage + mDesc->buttonScrollPercent);
	}

	void HScrollBar::onRightClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			_setPercentage(mDesc->sliderPercentage + mDesc->buttonScrollPercent);
		else
			_setPercentage(mDesc->sliderPercentage - mDesc->buttonScrollPercent);
	}

	void HScrollBar::onBarClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mWindow == NULL)
			return;

		Point p = mea.position - mWindow->getPosition() - mTexturePosition;
		bool left = (p.x < mButton_Slider->getPosition().x);

		if(mea.button == MB_Left)
		{
			if(left)
				_setPercentage(mDesc->sliderPercentage - mDesc->barScrollPercent);
			else
				_setPercentage(mDesc->sliderPercentage + mDesc->barScrollPercent);
		}
		else if(mea.button == MB_Right)
		{
			if(left)
				_setPercentage(mDesc->sliderPercentage + mDesc->barScrollPercent);
			else
				_setPercentage(mDesc->sliderPercentage - mDesc->barScrollPercent);
		}
	}

	void HScrollBar::onSizeChanged(const EventArgs& args)
	{
		_updateButtons();
		_updateSlider();
	}

	void HScrollBar::onSliderDragged(const EventArgs& args)
	{
		_setPercentage((mButton_Slider->getPosition().x - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x));
	}

	void HScrollBar::setBarScrollPercent(float percent)
	{
		mDesc->barScrollPercent = percent;
	}

	void HScrollBar::setButtonLayout(HScrollBarButtonLayout l)
	{
		mDesc->scrollBarButtonLayout = l;

		_updateButtons();
		_updateSlider();
	}

	void HScrollBar::setButtonScrollPercent(float percent)
	{
		mDesc->buttonScrollPercent = percent;
	}

	void HScrollBar::setPercentage(float percentage)
	{
		_setPercentage(percentage);
	}

	void HScrollBar::setSkinType(const Ogre::String type)
	{
		if(!SkinTypeManager::getSingleton().hasSkinType(getClass(),type))
			throw Exception(Exception::ERR_SKINNING,"SkinType \"" + type + "\" does not exist!","Widget::setSkinType");

		mSkinType = SkinTypeManager::getSingleton().getSkinType(getClass(),type);

		mWidgetDesc->skinTypeName = type;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);

		mClientDimensions.position.x = se->getBorderThickness(BORDER_LEFT);
		mClientDimensions.position.y = se->getBorderThickness(BORDER_TOP);
		mClientDimensions.size.width = mWidgetDesc->dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
		mClientDimensions.size.height = mWidgetDesc->dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));

		redraw();

		// Update button Sizes - same width and height for each, except slider, where only height is modified
		float buttonThickness = mWidgetDesc->dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));
		Size s(buttonThickness,buttonThickness);

		mButton_Left1->setSize(s);
		mButton_Right1->setSize(s);
		mButton_Left2->setSize(s);
		mButton_Right2->setSize(s);
		mButton_Slider->setHeight(buttonThickness);

		// Update button positions
		Point p;
		p.y = se->getBorderThickness(BORDER_TOP);

		p.x = se->getBorderThickness(BORDER_LEFT);
		mButton_Left1->setPosition(p);

		p.x += buttonThickness;
		mButton_Right1->setPosition(p);

		p.x = mWidgetDesc->dimensions.size.width - se->getBorderThickness(BORDER_RIGHT) - buttonThickness - buttonThickness;
		mButton_Left2->setPosition(p);

		p.x += buttonThickness;
		mButton_Right2->setPosition(p);

		p.x = mButton_Slider->getPosition().x;
		mButton_Slider->setPosition(p);

		_updateButtons();

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getComponentType((*it).first)->typeName);
	}

	void HScrollBar::scrollLeft()
	{
		_setPercentage(mDesc->sliderPercentage - mDesc->buttonScrollPercent);
	}

	void HScrollBar::scrollToEnd()
	{
		setPercentage((mSliderBounds.y - mButton_Slider->getWidth() - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x));
	}

	void HScrollBar::scrollRight()
	{
		_setPercentage(mDesc->sliderPercentage + mDesc->buttonScrollPercent);
	}

	void HScrollBar::setSliderWidth(float width)
	{
		mButton_Slider->setWidth(width);

		mLive = false;

		_updateSlider();
	}
}
