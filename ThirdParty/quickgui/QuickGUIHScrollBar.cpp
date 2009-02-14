#include "QuickGUIHScrollBar.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIManager.h"
#include "QuickGUIFactoryManager.h"

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
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("HScrollBar");
		d->defineSkinElement(BAR);
		d->defineComponent(LEFT1);
		d->defineComponent(LEFT2);
		d->defineComponent(RIGHT1);
		d->defineComponent(RIGHT2);
		d->defineComponent(SLIDER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("HScrollBar",d);
	}

	HScrollBarDesc::HScrollBarDesc(const Ogre::String& id) :
		ComponentWidgetDesc(id)
	{
		resetToDefault();
	}

	void HScrollBarDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		hscrollbar_buttonScrollPercent = 0.1;
		hscrollbar_barScrollPercent = 0.2;
		hscrollbar_scrollBarButtonLayout = HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		hscrollbar_sliderWidth = 15;
		hscrollbar_sliderPercentage = 0;
	}

	void HScrollBarDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);

		b->IO("ButtonScrollPercent",&hscrollbar_buttonScrollPercent);
		b->IO("BarScrollPercent",&hscrollbar_barScrollPercent);
		b->IO("ScrollBarButtonLayout",&hscrollbar_scrollBarButtonLayout);
		b->IO("SliderWidth",&hscrollbar_sliderWidth);
		b->IO("SliderPercentage",&hscrollbar_sliderPercentage);
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
		// Clean up all user defined event handlers.
		for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mScrollBarEventHandlers[index].begin(); it != mScrollBarEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void HScrollBar::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<HScrollBarDesc*>(mWidgetDesc);

		HScrollBarDesc* hsd = dynamic_cast<HScrollBarDesc*>(d);

		mDesc->hscrollbar_barScrollPercent = hsd->hscrollbar_barScrollPercent;
		mDesc->hscrollbar_buttonScrollPercent = hsd->hscrollbar_buttonScrollPercent;

		ButtonDesc* bd = FactoryManager::getSingleton().getDescFactory()->getInstance<ButtonDesc>("DefaultButtonDesc");
		bd->resetToDefault();
		bd->widget_resizeFromBottom = false;
		bd->widget_resizeFromLeft = false;
		bd->widget_resizeFromRight = false;
		bd->widget_resizeFromTop = false;
		bd->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
		bd->widget_dimensions.size.width = mWidgetDesc->widget_dimensions.size.height;
		bd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;

		mButton_Left1 = dynamic_cast<Button*>(Widget::create("Button",bd));
		mButton_Left1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onLeftClicked,this);
		addComponent(LEFT1,mButton_Left1);

		bd->widget_dimensions.position.x = 15;
		mButton_Right1 = dynamic_cast<Button*>(Widget::create("Button",bd));
		mButton_Right1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onRightClicked,this);
		addComponent(RIGHT1,mButton_Right1);

		bd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
		bd->widget_dimensions.position.x = mWidgetDesc->widget_dimensions.size.width - (2 * bd->widget_dimensions.size.width);
		mButton_Left2 = dynamic_cast<Button*>(Widget::create("Button",bd));
		mButton_Left2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onLeftClicked,this);
		addComponent(LEFT2,mButton_Left2);

		bd->widget_dimensions.position.x = mWidgetDesc->widget_dimensions.size.width - bd->widget_dimensions.size.width;
		mButton_Right2 = dynamic_cast<Button*>(Widget::create("Button",bd));
		mButton_Right2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onRightClicked,this);
		addComponent(RIGHT2,mButton_Right2);

		bd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
		bd->widget_dimensions.position.x = 30;
		bd->widget_dragable = true;
		mButton_Slider = dynamic_cast<Button*>(Widget::create("Button",bd));
		addComponent(SLIDER,mButton_Slider);

		mButton_Slider->addWidgetEventHandler(WIDGET_EVENT_DRAGGED,&HScrollBar::onSliderDragged,this);
		addWidgetEventHandler(WIDGET_EVENT_SIZE_CHANGED,&HScrollBar::onSizeChanged,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onBarClicked,this);

		setButtonLayout(hsd->hscrollbar_scrollBarButtonLayout);
		setSkinType(hsd->widget_skinTypeName);
		setPercentage(hsd->hscrollbar_sliderPercentage);
	}

	void HScrollBar::_setPercentage(float percentage)
	{
		percentage = Ogre::Math::Floor(Ogre::Math::Ceil(percentage * 1000.0)) / 1000.0;

		// if ScrollBar is live, update size before calculating max percentage
		if(mLive)
			mButton_Slider->setWidth( ((*mLiveValue1) / (*mLiveValue2)) * (mSliderBounds.y - mSliderBounds.x) );

		// calculate the max percentage the slider can be set to.
		float sliderWidth = mButton_Slider->getWidth();
		float maxPercentage = (mSliderBounds.y - sliderWidth - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x);
		maxPercentage = Ogre::Math::Floor(Ogre::Math::Ceil(maxPercentage * 1000.0)) / 1000.0;

		if(percentage <= 0)
			mDesc->hscrollbar_sliderPercentage = 0;
		else if(percentage > maxPercentage)
			mDesc->hscrollbar_sliderPercentage = maxPercentage;
		else
			mDesc->hscrollbar_sliderPercentage = percentage;

		mDesc->hscrollbar_sliderPercentage = Ogre::Math::Floor(Ogre::Math::Ceil(mDesc->hscrollbar_sliderPercentage * 1000.0)) / 1000.0;

		_updateSlider();
	}

	void HScrollBar::_updateButtons()
	{
		if(mSkinType == NULL)
		{
			mSliderBounds.x = 0;
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.width;

			mButton_Left1->setVisible(false);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(false);
			return;
		}

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);

		switch(mDesc->hscrollbar_scrollBarButtonLayout)
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
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.width - se->getBorderThickness(BORDER_RIGHT);
			
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
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.width - se->getBorderThickness(BORDER_RIGHT);

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
		mButton_Slider->setPosition(Point(mSliderBounds.x + ((mSliderBounds.y - mSliderBounds.x) * mDesc->hscrollbar_sliderPercentage),yPos));

		// if live, updates size
		if(mLive)
			mButton_Slider->setWidth( ((*mLiveValue1) / (*mLiveValue2)) * (mSliderBounds.y - mSliderBounds.x) );

		mDesc->hscrollbar_sliderWidth = mButton_Slider->getWidth();

		// determine if Slider is visible (if it can fit in bounds)
		if((mSliderBounds.y - mSliderBounds.x) < mButton_Slider->getWidth())
			mButton_Slider->setVisible(false);
		else
			mButton_Slider->setVisible(true);

		WidgetEventArgs args(this);
		fireScrollBarEvent(SCROLLBAR_EVENT_ON_SCROLLED,args);
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
		return mDesc->hscrollbar_barScrollPercent;
	}

	HScrollBarButtonLayout HScrollBar::getButtonLayout()
	{
		return mDesc->hscrollbar_scrollBarButtonLayout;
	}

	float HScrollBar::getButtonScrollPercent()
	{
		return mDesc->hscrollbar_buttonScrollPercent;
	}

	Ogre::String HScrollBar::getClass()
	{
		return "HScrollBar";
	}

	float HScrollBar::getPercentage()
	{
		return mDesc->hscrollbar_sliderPercentage;
	}

	float HScrollBar::getSliderWidthPercentage()
	{
		return (mButton_Slider->getWidth() / (mSliderBounds.y - mSliderBounds.x));
	}

	void HScrollBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);
		
		mSkinElementName = BAR;
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void HScrollBar::onLeftClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			_setPercentage(mDesc->hscrollbar_sliderPercentage - mDesc->hscrollbar_buttonScrollPercent);
		else
			_setPercentage(mDesc->hscrollbar_sliderPercentage + mDesc->hscrollbar_buttonScrollPercent);
	}

	void HScrollBar::onRightClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			_setPercentage(mDesc->hscrollbar_sliderPercentage + mDesc->hscrollbar_buttonScrollPercent);
		else
			_setPercentage(mDesc->hscrollbar_sliderPercentage - mDesc->hscrollbar_buttonScrollPercent);
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
				_setPercentage(mDesc->hscrollbar_sliderPercentage - mDesc->hscrollbar_barScrollPercent);
			else
				_setPercentage(mDesc->hscrollbar_sliderPercentage + mDesc->hscrollbar_barScrollPercent);
		}
		else if(mea.button == MB_Right)
		{
			if(left)
				_setPercentage(mDesc->hscrollbar_sliderPercentage + mDesc->hscrollbar_barScrollPercent);
			else
				_setPercentage(mDesc->hscrollbar_sliderPercentage - mDesc->hscrollbar_barScrollPercent);
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
		mDesc->hscrollbar_barScrollPercent = percent;
	}

	void HScrollBar::setButtonLayout(HScrollBarButtonLayout l)
	{
		mDesc->hscrollbar_scrollBarButtonLayout = l;

		_updateButtons();
		_updateSlider();
	}

	void HScrollBar::setButtonScrollPercent(float percent)
	{
		mDesc->hscrollbar_buttonScrollPercent = percent;
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

		mWidgetDesc->widget_skinTypeName = type;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);

		mClientDimensions.position.x = se->getBorderThickness(BORDER_LEFT);
		mClientDimensions.position.y = se->getBorderThickness(BORDER_TOP);
		mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
		mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));

		redraw();

		// Update button Sizes - same width and height for each, except slider, where only height is modified
		float buttonThickness = mWidgetDesc->widget_dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));
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

		p.x = mWidgetDesc->widget_dimensions.size.width - se->getBorderThickness(BORDER_RIGHT) - buttonThickness - buttonThickness;
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
		_setPercentage(mDesc->hscrollbar_sliderPercentage - mDesc->hscrollbar_buttonScrollPercent);
	}

	void HScrollBar::scrollToEnd()
	{
		setPercentage((mSliderBounds.y - mButton_Slider->getWidth() - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x));
	}

	void HScrollBar::scrollRight()
	{
		_setPercentage(mDesc->hscrollbar_sliderPercentage + mDesc->hscrollbar_buttonScrollPercent);
	}

	void HScrollBar::setSliderWidth(float width)
	{
		mButton_Slider->setWidth(width);

		mLive = false;

		_updateSlider();
	}
}
