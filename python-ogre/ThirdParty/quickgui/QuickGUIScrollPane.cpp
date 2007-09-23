#include "QuickGUIScrollPane.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	ScrollPane::ScrollPane(const Ogre::String& instanceName, Type type, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Widget(instanceName,type,Rect(0,0,1,1),QGUI_GMM_RELATIVE,QGUI_GMM_RELATIVE,"",container,ParentWidget,gm),
		mScrollBarWidth(20),
		mAbsPosition(mAbsoluteDimensions.x,mAbsoluteDimensions.y),
		mRelPosition(mRelativeDimensions.x,mRelativeDimensions.y),
		mHorizontalButtonLayout(HorizontalScrollBar::BUTTON_LAYOUT_OPPOSITE),
		mVerticalButtonLayout(VerticalScrollBar::BUTTON_LAYOUT_OPPOSITE),
		mHorizontalBarLayout(HORIZONTAL_BAR_LAYOUT_BOTTOM),
		mVerticalBarLayout(VERTICAL_BAR_LAYOUT_RIGHT)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_SCROLL_PANE)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
		}
		
		mGainFocusOnClick = false;

		// Create 4 ScrollBars, 2 vertical, 2 horizontal.  Set the parent to this widget's parent.
		Ogre::String parentName = mParentWidget->getInstanceName();
		
		Ogre::String skinSet;
		Ogre::String parentTexture = mParentWidget->getTextureName(true);
		if(parentTexture == "")
			skinSet = mParentSheet->getDefaultSkin();
		else
			skinSet = parentTexture.substr(0,parentTexture.find_first_of('.'));
		
		Rect parentPixelSize = mParentWidget->getDimensions(QGUI_GMM_PIXELS,QGUI_GMM_PIXELS);

		mTopBar = new HorizontalScrollBar(parentName+".TopScrollBar",TYPE_SCROLLBAR_HORIZONTAL,Rect(mScrollBarWidth,0,parentPixelSize.width - (mScrollBarWidth*2.0),mScrollBarWidth),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,skinSet+".scrollbar.horizontal.png",container,mParentWidget,gm);
		mTopBar->setShowWithParent(false);
		mTopBar->hide();
		mTopBar->addOnScrollEventHandler(&ScrollPane::onHorizontalScroll,this);

		mBottomBar = new HorizontalScrollBar(parentName+".BottomScrollBar",TYPE_SCROLLBAR_HORIZONTAL,Rect(mScrollBarWidth,parentPixelSize.height - mScrollBarWidth,parentPixelSize.width - (mScrollBarWidth*2.0),mScrollBarWidth),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,skinSet+".scrollbar.horizontal.png",container,mParentWidget,gm);
		mBottomBar->setShowWithParent(false);
		mBottomBar->hide();
		mBottomBar->addOnScrollEventHandler(&ScrollPane::onHorizontalScroll,this);

		mLeftBar = new VerticalScrollBar(parentName+".LeftScrollBar",TYPE_SCROLLBAR_VERTICAL,Rect(0,mScrollBarWidth,mScrollBarWidth,parentPixelSize.height - (mScrollBarWidth*2.0)),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,skinSet+".scrollbar.vertical.png",container,mParentWidget,gm);
		mLeftBar->setShowWithParent(false);
		mLeftBar->hide();
		mLeftBar->addOnScrollEventHandler(&ScrollPane::onVerticalScroll,this);

		mRightBar = new VerticalScrollBar(parentName+".RightScrollBar",TYPE_SCROLLBAR_VERTICAL,Rect(parentPixelSize.width - mScrollBarWidth,mScrollBarWidth,mScrollBarWidth,parentPixelSize.height - (mScrollBarWidth*2.0)),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,skinSet+".scrollbar.vertical.png",container,mParentWidget,gm);
		mRightBar->setShowWithParent(false);
		mRightBar->hide();
		mRightBar->addOnScrollEventHandler(&ScrollPane::onVerticalScroll,this);

		setHorizontalButtonLayout(mHorizontalButtonLayout);
		setVerticalButtonLayout(mVerticalButtonLayout);

		mParentWidget->addEventHandler(EVENT_CHILD_ADDED,&ScrollPane::onChildAddedToParent,this);
		mParentWidget->addEventHandler(EVENT_CHILD_REMOVED,&ScrollPane::onChildRemovedFromParent,this);
		mParentWidget->addEventHandler(EVENT_POSITION_CHANGED,&ScrollPane::onParentPositionChanged,this);
		mParentWidget->addEventHandler(EVENT_SIZE_CHANGED,&ScrollPane::onParentSizeChanged,this);
	}

	ScrollPane::~ScrollPane()
	{
	}

	void ScrollPane::_showHScrollBars()
	{
		switch(mHorizontalBarLayout)
		{
		case HORIZONTAL_BAR_LAYOUT_TOP:
			if(!mTopBar->isVisible())
				mTopBar->show();
			break;
		case HORIZONTAL_BAR_LAYOUT_BOTTOM:
			if(!mBottomBar->isVisible())
				mBottomBar->show();
			break;
		case HORIZONTAL_BAR_LAYOUT_BOTH:
			if(!mTopBar->isVisible())
				mTopBar->show();
			if(!mBottomBar->isVisible())
				mBottomBar->show();
			break;
		}
	}

	void ScrollPane::_showVScrollBars()
	{
		switch(mVerticalBarLayout)
		{
		case VERTICAL_BAR_LAYOUT_LEFT:
			if(!mLeftBar->isVisible())
				mLeftBar->show();
			break;
		case VERTICAL_BAR_LAYOUT_RIGHT:
			if(!mRightBar->isVisible())
				mRightBar->show();
			break;
		case VERTICAL_BAR_LAYOUT_BOTH:
			if(!mLeftBar->isVisible())
				mLeftBar->show();
			if(!mRightBar->isVisible())
				mRightBar->show();
			break;
		}
	}

	void ScrollPane::_syncBarWithParentDimensions()
	{
		if(mPixelDimensions.width <= (mParentWidget->getWidth(QGUI_GMM_PIXELS) + 0.001))
		{
			if(mTopBar->isVisible())
				mTopBar->hide();
			if(mBottomBar->isVisible())
				mBottomBar->hide();

			if(mPixelDimensions.width != mParentWidget->getWidth(QGUI_GMM_PIXELS))
				setWidth(mParentWidget->getWidth(QGUI_GMM_PIXELS),QGUI_GMM_PIXELS);
		}
		else
			_showHScrollBars();

		if(mPixelDimensions.height <= (mParentWidget->getHeight(QGUI_GMM_PIXELS) + 0.001))
		{
			if(mLeftBar->isVisible())
				mLeftBar->hide();
			if(mRightBar->isVisible())
				mRightBar->hide();
			setHeight(mParentWidget->getHeight(QGUI_GMM_PIXELS),QGUI_GMM_PIXELS);
		}
		else
			_showVScrollBars();

		// sync slider size
		mTopBar->setSliderWidth(mParentWidget->getWidth(QGUI_GMM_ABSOLUTE) / mAbsoluteDimensions.width);
		mBottomBar->setSliderWidth(mParentWidget->getWidth(QGUI_GMM_ABSOLUTE) / mAbsoluteDimensions.width);
		mLeftBar->setSliderHeight(mParentWidget->getHeight(QGUI_GMM_ABSOLUTE) / mAbsoluteDimensions.height);
		mRightBar->setSliderHeight(mParentWidget->getHeight(QGUI_GMM_ABSOLUTE) / mAbsoluteDimensions.height);
	}

	HorizontalScrollBar* ScrollPane::getBottomScrollBar()
	{
		return mBottomBar;
	}

	HorizontalScrollBar::ButtonLayout ScrollPane::getHorizontalButtonLayout()
	{
		return mHorizontalButtonLayout;
	}

	VerticalScrollBar* ScrollPane::getLeftScrollBar()
	{
		return mLeftBar;
	}

	VerticalScrollBar* ScrollPane::getRightScrollBar()
	{
		return mRightBar;
	}

	HorizontalScrollBar* ScrollPane::getTopScrollBar()
	{
		return mTopBar;
	}

	VerticalScrollBar::ButtonLayout ScrollPane::getVerticalButtonLayout()
	{
		return mVerticalButtonLayout;
	}

	void ScrollPane::onChildAddedToParent(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;

		if(w->getWidgetType() == TYPE_TITLEBAR)
			return;

		mManagedWidgets.push_back(w);

		w->setClippingRect(mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE));

		if(w->getWidgetType() == TYPE_TEXTBOX)
			w->addEventHandler(EVENT_GAIN_FOCUS,&ScrollPane::onChildTextBoxGainedFocus,this);

		// if Widget position is outside ScrollPanel bounds, make ScrollPanel bigger.
		Rect widgetAbsDimensions = w->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE);
		if( (widgetAbsDimensions.x + widgetAbsDimensions.width) > (mAbsoluteDimensions.x + mAbsoluteDimensions.width) )
		{
			setWidth(widgetAbsDimensions.x + widgetAbsDimensions.width);
			_syncBarWithParentDimensions();
		}
		if( (widgetAbsDimensions.y + widgetAbsDimensions.height) > (mAbsoluteDimensions.y + mAbsoluteDimensions.height) )
		{
			setHeight(widgetAbsDimensions.y + widgetAbsDimensions.height);
			_syncBarWithParentDimensions();
		}
	}

	void ScrollPane::onChildRemovedFromParent(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;

		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			if( w->getInstanceName() == (*it)->getInstanceName() )
			{
				mManagedWidgets.erase(it);
				break;
			}
		}

		// see if we can resize the ScrollPane.
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			// if Widget position is outside ScrollPanel bounds, make ScrollPanel bigger.
			Rect widgetAbsDimensions = (*it)->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE);
			if( (widgetAbsDimensions.x + widgetAbsDimensions.width) > (mAbsoluteDimensions.x + mAbsoluteDimensions.width) )
			{
				setWidth(widgetAbsDimensions.x + widgetAbsDimensions.width);
				_syncBarWithParentDimensions();
			}
			if( (widgetAbsDimensions.y + widgetAbsDimensions.height) > (mAbsoluteDimensions.y + mAbsoluteDimensions.height) )
			{
				setHeight(widgetAbsDimensions.y + widgetAbsDimensions.height);
				_syncBarWithParentDimensions();
			}
		}
	}

	void ScrollPane::onChildTextBoxGainedFocus(const EventArgs& args)
	{
	}

	void ScrollPane::onParentPositionChanged(const EventArgs& args)
	{
		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
			(*it)->setClippingRect(mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE));
	}

	void ScrollPane::onParentSizeChanged(const EventArgs& args)
	{
		_syncBarWithParentDimensions();
	}

	void ScrollPane::onHorizontalScroll(const EventArgs& args)
	{
		// sync both bars
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		if( w->getInstanceName() == mTopBar->getInstanceName() )
			mBottomBar->_setValue(mTopBar->getValue());
		else if( w->getInstanceName() == mBottomBar->getInstanceName() )
			mTopBar->_setValue(mBottomBar->getValue());

		setXPosition(-(mTopBar->getValue()));
		Ogre::Real offset = getXPosition() - mRelPosition.x;
		mRelPosition.x = getXPosition();

		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			// In the event of scroll panes inside scroll panes, moving a scroll pane will move the inner scroll panes.
			// In this case, we need to update the clipping rect.
			(*it)->setClippingRect(mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE));
			(*it)->moveX(offset);
		}
	}

	void ScrollPane::onVerticalScroll(const EventArgs& args)
	{
		// sync both bars
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		if( w->getInstanceName() == mLeftBar->getInstanceName() )
			mRightBar->_setValue(mLeftBar->getValue());
		else if( w->getInstanceName() == mRightBar->getInstanceName() )
			mLeftBar->_setValue(mRightBar->getValue());

		setYPosition(-(mLeftBar->getValue()));
		Ogre::Real offset = getYPosition() - mRelPosition.y;
		mRelPosition.y = getYPosition();

		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			// In the event of scroll panes inside scroll panes, moving a scroll pane will move the inner scroll panes.
			// In this case, we need to update the clipping rect.
			(*it)->setClippingRect(mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE));
			(*it)->moveY(offset);
		}
	}

	void ScrollPane::setHorizontalBarLayout(HorizontalBarLayout layout)
	{
		mHorizontalBarLayout = layout;
		_syncBarWithParentDimensions();
	}

	void ScrollPane::setHorizontalButtonLayout(HorizontalScrollBar::ButtonLayout layout)
	{
		mHorizontalButtonLayout = layout;
		mTopBar->setButtonLayout(mHorizontalButtonLayout);
		mBottomBar->setButtonLayout(mHorizontalButtonLayout);
	}

	void ScrollPane::scrollIntoView(Widget* w)
	{
		Rect wDimensions = w->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE);
		if(!wDimensions.inside(mAbsoluteDimensions))
			return;

		Ogre::Real leftX = ((wDimensions.x - mAbsoluteDimensions.x) / mAbsoluteDimensions.width);
		Ogre::Real rightX = (((wDimensions.x + wDimensions.width) - mAbsoluteDimensions.x) / mAbsoluteDimensions.width);
		// see if we will be scrolling left, right, or not at all
		Ogre::Real hSliderValue = mTopBar->getValue();
		if( leftX < hSliderValue )
		{
			// Only need to set value of one, callbacks will sync the other scrollbar.
			mTopBar->setValue(leftX);
		}
		else if( rightX > (hSliderValue + mTopBar->getSliderWidth()) )
		{
			// Only need to set value of one, callbacks will sync the other scrollbar.
			mTopBar->setValue(rightX - mTopBar->getSliderWidth());
		}

		Ogre::Real topY = ((wDimensions.y - mAbsoluteDimensions.y) / mAbsoluteDimensions.height);
		Ogre::Real botY = (((wDimensions.y + wDimensions.height) - mAbsoluteDimensions.y) / mAbsoluteDimensions.height);
		// see if we will be scrolling up, down, or not at all
		Ogre::Real vSliderValue = mLeftBar->getValue();
		if( topY < vSliderValue )
		{
			// Only need to set value of one, callbacks will sync the other scrollbar.
			mLeftBar->setValue(topY);
		}
		else if( botY > (vSliderValue + mLeftBar->getSliderHeight()) )
		{
			// Only need to set value of one, callbacks will sync the other scrollbar.
			mLeftBar->setValue(botY - mLeftBar->getSliderHeight());
		}
	}

	void ScrollPane::setVerticalBarLayout(VerticalBarLayout layout)
	{
		mVerticalBarLayout = layout;
		_syncBarWithParentDimensions();
	}

	void ScrollPane::setVerticalButtonLayout(VerticalScrollBar::ButtonLayout layout)
	{
		mVerticalButtonLayout = layout;
		mLeftBar->setButtonLayout(mVerticalButtonLayout);
		mRightBar->setButtonLayout(mVerticalButtonLayout);
	}

	void ScrollPane::setPosition(const Ogre::Real& xVal, const Ogre::Real& yVal, GuiMetricsMode mode)
	{
		Widget::setPosition(xVal,yVal,mode);
	}

	void ScrollPane::setPosition(const Point& p, GuiMetricsMode mode)
	{
		Widget::setPosition(p,mode);
	}

	void ScrollPane::show()
	{
		mQuad->setVisible(true);

		// show children, except for Windows and lists of MenuList or ComboBox Widget.
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( ((*it)->getWidgetType() == TYPE_SCROLLBAR_HORIZONTAL) ||
				((*it)->getWidgetType() == TYPE_SCROLLBAR_VERTICAL) )
				continue;

			(*it)->show();
		}

		_syncBarWithParentDimensions();

		// Only fire event if we change visibility.  If we were already visible, don't fire.
		if(!mVisible)
		{
			WidgetEventArgs args(this);
			fireEvent(EVENT_SHOWN,args);
		}

		mVisible = true;
	}

	void ScrollPane::updateView()
	{
		setPosition(-(mTopBar->getValue()),-(mLeftBar->getValue()));

/*		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			// In the event of scroll panes inside scroll panes, moving a scroll pane will move the inner scroll panes.
			// In this case, we need to update the clipping rect.
			(*it)->setClippingRect(mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE));
			(*it)->move(getPosition());
		}
*/	}
}