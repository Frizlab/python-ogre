#include "QuickGUIVScrollBar.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	const Ogre::String VScrollBar::BAR = "bar";
	const Ogre::String VScrollBar::UP1 = "up1";
	const Ogre::String VScrollBar::UP2 = "up2";
	const Ogre::String VScrollBar::DOWN1 = "down1";
	const Ogre::String VScrollBar::DOWN2 = "down2";
	const Ogre::String VScrollBar::SLIDER = "slider";

	void VScrollBar::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("VScrollBar");
		d->defineSkinElement(BAR);
		d->defineComponent(UP1);
		d->defineComponent(UP2);
		d->defineComponent(DOWN1);
		d->defineComponent(DOWN2);
		d->defineComponent(SLIDER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("VScrollBar",d);
	}

	VScrollBarDesc::VScrollBarDesc() :
		ComponentWidgetDesc()
	{
		buttonScrollPercent = 0.1;
		barScrollPercent = 0.2;
		scrollBarButtonLayout = VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		sliderHeight = 15;
		sliderPercentage = 0;
	}

	void VScrollBarDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);

		b->IO("ButtonScrollPercent",&buttonScrollPercent);
		b->IO("BarScrollPercent",&barScrollPercent);
		b->IO("ScrollBarButtonLayout",&scrollBarButtonLayout);
		b->IO("SliderHeight",&sliderHeight);
		b->IO("SliderPercentage",&sliderPercentage);
	}

	VScrollBar::VScrollBar(const Ogre::String& name) :
		ComponentWidget(name),
		mButton_Up1(NULL),
		mButton_Up2(NULL),
		mButton_Down1(NULL),
		mButton_Down2(NULL),
		mButton_Slider(NULL),
		mLive(false)
	{
		mSkinElementName = BAR;
	}

	VScrollBar::~VScrollBar()
	{
	}

	void VScrollBar::_initialize(WidgetDesc* d)
	{
		mDesc = dynamic_cast<VScrollBarDesc*>(mWidgetDesc);

		Widget::_initialize(d);

		VScrollBarDesc* vsd = dynamic_cast<VScrollBarDesc*>(d);

		mDesc->barScrollPercent = vsd->barScrollPercent;
		mDesc->buttonScrollPercent = vsd->buttonScrollPercent;

		ButtonDesc bd;
		bd.resizable = false;
		bd.horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		bd.dimensions.size.width = mWidgetDesc->dimensions.size.width;
		bd.dimensions.size.height = mWidgetDesc->dimensions.size.width;

		mButton_Up1 = dynamic_cast<Button*>(Widget::create("Button",bd));
		mButton_Up1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&VScrollBar::onUpClicked,this);
		addComponent(UP1,mButton_Up1);

		bd.dimensions.position.y = 15;
		mButton_Down1 = dynamic_cast<Button*>(Widget::create("Button",bd));
		mButton_Down1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&VScrollBar::onDownClicked,this);
		addComponent(DOWN1,mButton_Down1);

		bd.verticalAnchor = ANCHOR_VERTICAL_BOTTOM;
		bd.dimensions.position.y = mWidgetDesc->dimensions.size.height - (2 * bd.dimensions.size.height);
		mButton_Up2 = dynamic_cast<Button*>(Widget::create("Button",bd));
		mButton_Up2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&VScrollBar::onUpClicked,this);
		addComponent(UP2,mButton_Up2);

		bd.dimensions.position.y = mWidgetDesc->dimensions.size.height - bd.dimensions.size.height;
		mButton_Down2 = dynamic_cast<Button*>(Widget::create("Button",bd));
		mButton_Down2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&VScrollBar::onDownClicked,this);
		addComponent(DOWN2,mButton_Down2);

		bd.dimensions.position.y = 30;
		bd.dragable = true;
		mButton_Slider = dynamic_cast<Button*>(Widget::create("Button",bd));
		addComponent(SLIDER,mButton_Slider);

		mButton_Slider->addWidgetEventHandler(WIDGET_EVENT_DRAGGED,&VScrollBar::onSliderDragged,this);
		addWidgetEventHandler(WIDGET_EVENT_SIZE_CHANGED,&VScrollBar::onSizeChanged,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&VScrollBar::onBarClicked,this);

		setButtonLayout(vsd->scrollBarButtonLayout);
		setSkinType(vsd->skinTypeName);
		setPercentage(vsd->sliderPercentage);
	}

	void VScrollBar::_setPercentage(float percentage)
	{
		float pixelAmount = percentage * (mSliderBounds.y - mSliderBounds.x);

		if(percentage <= 0)
			mDesc->sliderPercentage = 0;
		else if(percentage > ((mSliderBounds.y - mButton_Slider->getHeight() - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x)))
			mDesc->sliderPercentage = (mSliderBounds.y - mButton_Slider->getHeight() - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x);
		else
			mDesc->sliderPercentage = percentage;

		_updateSlider();
	}

	void VScrollBar::_updateButtons()
	{
		if(mSkinType == NULL)
		{
			mSliderBounds.x = 0;
			mSliderBounds.y = mWidgetDesc->dimensions.size.height;

			mButton_Up1->setVisible(false);
			mButton_Down1->setVisible(false);
			mButton_Up2->setVisible(false);
			mButton_Down2->setVisible(false);
			return;
		}

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);

		switch(mDesc->scrollBarButtonLayout)
		{
		case VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN:
			mSliderBounds.x = se->getBorderThickness(BORDER_TOP);
			mSliderBounds.y = mButton_Up2->getPosition().y;
			
			mButton_Up1->setVisible(false);
			mButton_Down1->setVisible(false);
			mButton_Up2->setVisible(true);
			mButton_Down2->setVisible(true);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP:
			mSliderBounds.x = (mButton_Down1->getPosition().y + mButton_Down1->getHeight());
			mSliderBounds.y = mWidgetDesc->dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM);
			
			mButton_Up1->setVisible(true);
			mButton_Down1->setVisible(true);
			mButton_Up2->setVisible(false);
			mButton_Down2->setVisible(false);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH:
			mSliderBounds.x = (mButton_Down1->getPosition().y + mButton_Down1->getHeight());
			mSliderBounds.y = mButton_Up2->getPosition().y;

			mButton_Up1->setVisible(true);
			mButton_Down1->setVisible(true);
			mButton_Up2->setVisible(true);
			mButton_Down2->setVisible(true);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN:
			mSliderBounds.x = (mButton_Down1->getPosition().y + mButton_Down1->getHeight());
			mSliderBounds.y = mButton_Down2->getPosition().y;

			mButton_Up1->setVisible(true);
			mButton_Down1->setVisible(true);
			mButton_Up2->setVisible(false);
			mButton_Down2->setVisible(true);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP:
			mSliderBounds.x = mButton_Down1->getPosition().y;
			mSliderBounds.y = mButton_Up2->getPosition().y;

			mButton_Up1->setVisible(true);
			mButton_Down1->setVisible(false);
			mButton_Up2->setVisible(true);
			mButton_Down2->setVisible(true);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_NONE:
			mSliderBounds.x = se->getBorderThickness(BORDER_TOP);
			mSliderBounds.y = mWidgetDesc->dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM);

			mButton_Up1->setVisible(false);
			mButton_Down1->setVisible(false);
			mButton_Up2->setVisible(false);
			mButton_Down2->setVisible(false);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE:
			mSliderBounds.x = mButton_Down1->getPosition().y;
			mSliderBounds.y = mButton_Down2->getPosition().y;

			mButton_Up1->setVisible(true);
			mButton_Down1->setVisible(false);
			mButton_Up2->setVisible(false);
			mButton_Down2->setVisible(true);
			break;
		}
	}

	void VScrollBar::_updateSlider()
	{
		// update position
		float xPos = 0;
		if(mSkinType != NULL)
			xPos = mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_LEFT);
		mButton_Slider->setPosition(Point(xPos,mSliderBounds.x + ((mSliderBounds.y - mSliderBounds.x) * mDesc->sliderPercentage)));

		// if live, updates size
		if(mLive)
			mButton_Slider->setHeight( ((*mLiveValue1) / (*mLiveValue2)) * (mSliderBounds.y - mSliderBounds.x) );

		mDesc->sliderHeight = mButton_Slider->getHeight();

		// determine if Slider is visible (if it can fit in bounds)
		if((mSliderBounds.y - mSliderBounds.x) < mButton_Slider->getHeight())
			mButton_Slider->setVisible(false);
		else
			mButton_Slider->setVisible(true);

		WidgetEventArgs args(this);
		fireScrollBarEvent(SCROLLBAR_EVENT_ON_SCROLLED,args);
	}

	void VScrollBar::addScrollBarEventHandler(ScrollBarEvent EVENT, EventHandlerSlot* function)
	{
		mScrollBarEventHandlers[EVENT].push_back(function);
	}

	void VScrollBar::disableLiveSlider()
	{
		mLive = false;
	}

	void VScrollBar::enableLiveSlider(float* value1, float* value2)
	{
		mLive = true;
		mLiveValue1 = value1;
		mLiveValue2 = value2;

		_updateSlider();
	}

	Widget* VScrollBar::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new VScrollBar(widgetName);

		newWidget->_createDescObject("VScrollBarDesc");

		return newWidget;
	}

	bool VScrollBar::fireScrollBarEvent(ScrollBarEvent e, EventArgs& args)
	{
		if(mScrollBarEventHandlers[e].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mScrollBarEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	float VScrollBar::getBarScrollPercent()
	{
		return mDesc->barScrollPercent;
	}

	VScrollBarButtonLayout VScrollBar::getButtonLayout()
	{
		return mDesc->scrollBarButtonLayout;
	}

	float VScrollBar::getButtonScrollPercent()
	{
		return mDesc->buttonScrollPercent;
	}

	Ogre::String VScrollBar::getClass()
	{
		return "VScrollBar";
	}

	float VScrollBar::getPercentage()
	{
		return mDesc->sliderPercentage;
	}

	float VScrollBar::getSliderHeightPercentage()
	{
		return (mButton_Slider->getHeight() / (mSliderBounds.y - mSliderBounds.x));
	}

	void VScrollBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);
		
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void VScrollBar::onUpClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			_setPercentage(mDesc->sliderPercentage - mDesc->buttonScrollPercent);
		else
			_setPercentage(mDesc->sliderPercentage + mDesc->buttonScrollPercent);
	}

	void VScrollBar::onDownClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			_setPercentage(mDesc->sliderPercentage + mDesc->buttonScrollPercent);
		else
			_setPercentage(mDesc->sliderPercentage - mDesc->buttonScrollPercent);
	}

	void VScrollBar::onBarClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mWindow == NULL)
			return;

		Point p = mea.position - mWindow->getPosition() - mTexturePosition;
		bool above = (p.y < mButton_Slider->getPosition().y);

		if(mea.button == MB_Left)
		{
			if(above)
				_setPercentage(mDesc->sliderPercentage - mDesc->barScrollPercent);
			else
				_setPercentage(mDesc->sliderPercentage + mDesc->barScrollPercent);
		}
		else if(mea.button == MB_Right)
		{
			if(above)
				_setPercentage(mDesc->sliderPercentage + mDesc->barScrollPercent);
			else
				_setPercentage(mDesc->sliderPercentage - mDesc->barScrollPercent);
		}
	}

	void VScrollBar::onSizeChanged(const EventArgs& args)
	{
		_updateButtons();
		_updateSlider();
	}

	void VScrollBar::onSliderDragged(const EventArgs& args)
	{
		_setPercentage((mButton_Slider->getPosition().y - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x));
	}

	void VScrollBar::scrollDown()
	{
		_setPercentage(mDesc->sliderPercentage + mDesc->buttonScrollPercent);
	}

	void VScrollBar::scrollToEnd()
	{
		setPercentage((mSliderBounds.y - mButton_Slider->getHeight() - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x));
	}

	void VScrollBar::scrollUp()
	{
		_setPercentage(mDesc->sliderPercentage - mDesc->buttonScrollPercent);
	}

	void VScrollBar::setBarScrollPercent(float percent)
	{
		mDesc->barScrollPercent = percent;
	}

	void VScrollBar::setButtonLayout(VScrollBarButtonLayout l)
	{
		mDesc->scrollBarButtonLayout = l;

		_updateButtons();
		_updateSlider();
	}

	void VScrollBar::setButtonScrollPercent(float percent)
	{
		mDesc->buttonScrollPercent = percent;
	}

	void VScrollBar::setPercentage(float percentage)
	{
		_setPercentage(percentage);
	}

	void VScrollBar::setSkinType(const Ogre::String type)
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

		// Update button Sizes - same width and height for each, except slider, where only width is modified
		float buttonThickness = mWidgetDesc->dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
		Size s(buttonThickness,buttonThickness);

		mButton_Up1->setSize(s);
		mButton_Down1->setSize(s);
		mButton_Up2->setSize(s);
		mButton_Down2->setSize(s);
		mButton_Slider->setWidth(buttonThickness);

		// Update button positions
		Point p;
		p.x = se->getBorderThickness(BORDER_LEFT);

		p.y = se->getBorderThickness(BORDER_TOP);
		mButton_Up1->setPosition(p);

		p.y += buttonThickness;
		mButton_Down1->setPosition(p);

		p.y = mWidgetDesc->dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM) - buttonThickness - buttonThickness;
		mButton_Up2->setPosition(p);

		p.y += buttonThickness;
		mButton_Down2->setPosition(p);

		p.y = mButton_Slider->getPosition().y;
		mButton_Slider->setPosition(p);

		_updateButtons();

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getComponentType((*it).first)->typeName);
	}

	void VScrollBar::setSliderHeight(float height)
	{
		mButton_Slider->setHeight(height);

		mLive = false;

		_updateSlider();
	}
}
