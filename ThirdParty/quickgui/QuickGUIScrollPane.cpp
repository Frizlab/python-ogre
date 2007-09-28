#include "QuickGUIScrollPane.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	ScrollPane::ScrollPane(const Ogre::String& instanceName, Type type, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Widget(instanceName,type,Rect(0,0,1,1),QGUI_GMM_RELATIVE,QGUI_GMM_RELATIVE,"",container,ParentWidget,gm),
		mScrollBarWidth(20),
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
		mTopBar->getQuad()->setLayer(Quad::LAYER_MENU);
		mTopBar->hide();
		mTopBar->addOnScrollEventHandler(&ScrollPane::onHorizontalScroll,this);
		// Set child widgets to also be on menu layer.
		std::vector<Widget*>* childList = mTopBar->getChildWidgetList();
		for( std::vector<Widget*>::iterator it = childList->begin(); it != childList->end(); ++it )
			(*it)->getQuad()->setLayer(Quad::LAYER_MENU);

		mBottomBar = new HorizontalScrollBar(parentName+".BottomScrollBar",TYPE_SCROLLBAR_HORIZONTAL,Rect(mScrollBarWidth,parentPixelSize.height - mScrollBarWidth,parentPixelSize.width - (mScrollBarWidth*2.0),mScrollBarWidth),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,skinSet+".scrollbar.horizontal.png",container,mParentWidget,gm);
		mBottomBar->setShowWithParent(false);
		mBottomBar->getQuad()->setLayer(Quad::LAYER_MENU);
		mBottomBar->hide();
		mBottomBar->addOnScrollEventHandler(&ScrollPane::onHorizontalScroll,this);
		// Set child widgets to also be on menu layer.
		mBottomBar->getChildWidgetList();
		for( std::vector<Widget*>::iterator it = childList->begin(); it != childList->end(); ++it )
			(*it)->getQuad()->setLayer(Quad::LAYER_MENU);

		mLeftBar = new VerticalScrollBar(parentName+".LeftScrollBar",TYPE_SCROLLBAR_VERTICAL,Rect(0,mScrollBarWidth,mScrollBarWidth,parentPixelSize.height - (mScrollBarWidth*2.0)),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,skinSet+".scrollbar.vertical.png",container,mParentWidget,gm);
		mLeftBar->setShowWithParent(false);
		mLeftBar->getQuad()->setLayer(Quad::LAYER_MENU);
		mLeftBar->hide();
		mLeftBar->addOnScrollEventHandler(&ScrollPane::onVerticalScroll,this);
		// Set child widgets to also be on menu layer.
		childList = mLeftBar->getChildWidgetList();
		for( std::vector<Widget*>::iterator it = childList->begin(); it != childList->end(); ++it )
			(*it)->getQuad()->setLayer(Quad::LAYER_MENU);

		mRightBar = new VerticalScrollBar(parentName+".RightScrollBar",TYPE_SCROLLBAR_VERTICAL,Rect(parentPixelSize.width - mScrollBarWidth,mScrollBarWidth,mScrollBarWidth,parentPixelSize.height - (mScrollBarWidth*2.0)),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,skinSet+".scrollbar.vertical.png",container,mParentWidget,gm);
		mRightBar->setShowWithParent(false);
		mRightBar->getQuad()->setLayer(Quad::LAYER_MENU);
		mRightBar->hide();
		mRightBar->addOnScrollEventHandler(&ScrollPane::onVerticalScroll,this);
		// Set child widgets to also be on menu layer.
		childList = mRightBar->getChildWidgetList();
		for( std::vector<Widget*>::iterator it = childList->begin(); it != childList->end(); ++it )
			(*it)->getQuad()->setLayer(Quad::LAYER_MENU);

		setHorizontalButtonLayout(mHorizontalButtonLayout);
		setVerticalButtonLayout(mVerticalButtonLayout);

		mParentWidget->addEventHandler(EVENT_CHILD_ADDED,&ScrollPane::onChildAddedToParent,this);
		mParentWidget->addEventHandler(EVENT_CHILD_REMOVED,&ScrollPane::onChildRemovedFromParent,this);
		mParentWidget->addEventHandler(EVENT_POSITION_CHANGED,&ScrollPane::onParentPositionChanged,this);
		mParentWidget->addEventHandler(EVENT_SIZE_CHANGED,&ScrollPane::onParentSizeChanged,this);
		addEventHandler(EVENT_SIZE_CHANGED,&ScrollPane::onSizeChanged,this);
	}

	ScrollPane::~ScrollPane()
	{
	}

	void ScrollPane::_determinePaneBounds()
	{
		if(!mEnabled)
			return;

		// Scroll Pane should not be smaller than parent width/height
		Rect parentAbsDimensions = mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE);

		// find the minimum and maximum region encompassing the managed widgets.
		Ogre::Real right = parentAbsDimensions.x + parentAbsDimensions.width;
		Ogre::Real bottom = parentAbsDimensions.y + parentAbsDimensions.height;

		Ogre::Real currentXPos = (mTopBar->getValue() * mAbsoluteDimensions.width);
		Ogre::Real currentYPos = (mLeftBar->getValue() * mAbsoluteDimensions.height);

		// Get min/max bounds for scroll pane.  By default, pane has same bounds as parent.
		// This may change depending on managed widgets that may lie above/below/left/right of current pane bounds.
		std::vector<std::pair<Widget*,Point> >::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			Rect wAbsDimensions = (*it).first->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE);
			wAbsDimensions.x += currentXPos;
			wAbsDimensions.y += currentYPos;
			
			if( (wAbsDimensions.x + wAbsDimensions.width) > right )
				right = (wAbsDimensions.x + wAbsDimensions.width);

			if( (wAbsDimensions.y + wAbsDimensions.height) > bottom )
				bottom = (wAbsDimensions.y + wAbsDimensions.height);
		}

		Size newPanelBounds(right - parentAbsDimensions.x,bottom - parentAbsDimensions.y);

		setSize(newPanelBounds,QGUI_GMM_ABSOLUTE);
		mBottomBar->setValue(currentXPos / newPanelBounds.width);
		mRightBar->setValue(currentYPos / newPanelBounds.height);
	}

	void ScrollPane::_updateWidgetOffset(const Ogre::String widgetName, const Point& offset)
	{
		std::vector<std::pair<Widget*,Point> >::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			if((*it).first->getInstanceName() == widgetName)
			{
				(*it).second = offset;
				return;
			}
		}
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
		case HORIZONTAL_BAR_LAYOUT_NONE:
			// show nothing
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
		case VERTICAL_BAR_LAYOUT_NONE:
			// show nothing`	
			break;
		}
	}

	void ScrollPane::_syncBarWithParentDimensions()
	{
		if(!mEnabled)
			return;

		if(mPixelDimensions.width <= (mParentWidget->getWidth(QGUI_GMM_PIXELS) + 0.001))
		{
			if(mTopBar->isVisible())
				mTopBar->hide();
			if(mBottomBar->isVisible())
				mBottomBar->hide();
		}
		else
			_showHScrollBars();

		if(mPixelDimensions.height <= (mParentWidget->getHeight(QGUI_GMM_PIXELS) + 0.001))
		{
			if(mLeftBar->isVisible())
				mLeftBar->hide();
			if(mRightBar->isVisible())
				mRightBar->hide();
		}
		else
			_showVScrollBars();

		// sync slider size
		mTopBar->setSliderWidth(mParentWidget->getWidth(QGUI_GMM_ABSOLUTE) / mAbsoluteDimensions.width);
		mBottomBar->setSliderWidth(mParentWidget->getWidth(QGUI_GMM_ABSOLUTE) / mAbsoluteDimensions.width);
		mLeftBar->setSliderHeight(mParentWidget->getHeight(QGUI_GMM_ABSOLUTE) / mAbsoluteDimensions.height);
		mRightBar->setSliderHeight(mParentWidget->getHeight(QGUI_GMM_ABSOLUTE) / mAbsoluteDimensions.height);
	}

	void ScrollPane::disable()
	{
		WidgetEventArgs args(this);
		fireEvent(EVENT_DISABLED,args);

		mEnabled = false;
	}

	void ScrollPane::enable()
	{
		mEnabled = true;

		_determinePaneBounds();	

		WidgetEventArgs args(this);
		fireEvent(EVENT_ENABLED,args);
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

		if((w->getWidgetType() == TYPE_TITLEBAR) || (w->getWidgetType() == TYPE_MENU))
			return;

		w->setClippingRect(mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE));

		if(w->getWidgetType() == TYPE_TEXTBOX)
			w->addEventHandler(EVENT_GAIN_FOCUS,&ScrollPane::onChildTextBoxGainedFocus,this);

		Point widgetPosition = w->getPosition(QGUI_GMM_ABSOLUTE);
		Point parentPosition = mParentWidget->getPosition(QGUI_GMM_ABSOLUTE);

		Point offset(widgetPosition.x - parentPosition.x,widgetPosition.y - parentPosition.y);
		mManagedWidgets.push_back(std::make_pair(w,offset));

		_determinePaneBounds();
	}

	void ScrollPane::onChildRemovedFromParent(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;

		// remove widget pointer from managed list
		std::vector<std::pair<Widget*,Point> >::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			if( w->getInstanceName() == (*it).first->getInstanceName() )
			{
				mManagedWidgets.erase(it);
				break;
			}
		}

		_determinePaneBounds();
	}

	void ScrollPane::onChildTextBoxGainedFocus(const EventArgs& args)
	{
	}

	void ScrollPane::onParentPositionChanged(const EventArgs& args)
	{
		if(!mEnabled)
			return;

		std::vector<std::pair<Widget*,Point> >::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
			(*it).first->setClippingRect(mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE));
	}

	void ScrollPane::onParentSizeChanged(const EventArgs& args)
	{
		if(!mEnabled)
			return;

		_syncBarWithParentDimensions();
	}

	void ScrollPane::onHorizontalScroll(const EventArgs& args)
	{
		if(!mEnabled)
			return;

		// sync both bars
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		if( w->getInstanceName() == mTopBar->getInstanceName() )
			mBottomBar->_setValue(mTopBar->getValue());
		else if( w->getInstanceName() == mBottomBar->getInstanceName() )
			mTopBar->_setValue(mBottomBar->getValue());

		// Move Scroll Pane
		setXPosition(-(mTopBar->getValue()) * mAbsoluteDimensions.width,QGUI_GMM_ABSOLUTE);

		Ogre::Real parentAbsX = mParentWidget->getXPosition(QGUI_GMM_ABSOLUTE);
		Ogre::Real parentAbsWidth = mParentWidget->getWidth(QGUI_GMM_ABSOLUTE);
		std::vector<std::pair<Widget*,Point> >::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			// In the event of scroll panes inside scroll panes, moving a scroll pane will move the inner scroll panes.
			// In this case, we need to update the clipping rect.
			(*it).first->setClippingRect(mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE));
			//(*it).first->setXPosition((mAbsoluteDimensions.x + (*it).second.x - parentAbsX) / parentAbsWidth);
			(*it).first->setXPosition(mAbsoluteDimensions.x - parentAbsX + (*it).second.x,QGUI_GMM_ABSOLUTE);
		}
	}

	void ScrollPane::onSizeChanged(const EventArgs& args)
	{
		if(!mEnabled)
			return;

		_syncBarWithParentDimensions();
	}

	void ScrollPane::onVerticalScroll(const EventArgs& args)
	{
		if(!mEnabled)
			return;

		// sync both bars
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		if( w->getInstanceName() == mLeftBar->getInstanceName() )
			mRightBar->_setValue(mLeftBar->getValue());
		else if( w->getInstanceName() == mRightBar->getInstanceName() )
			mLeftBar->_setValue(mRightBar->getValue());

		// Move Scroll Pane
		setYPosition(-(mLeftBar->getValue()) * mAbsoluteDimensions.height,QGUI_GMM_ABSOLUTE);

		Ogre::Real parentAbsY = mParentWidget->getYPosition(QGUI_GMM_ABSOLUTE);
		std::vector<std::pair<Widget*,Point> >::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			// In the event of scroll panes inside scroll panes, moving a scroll pane will move the inner scroll panes.
			// In this case, we need to update the clipping rect.
			(*it).first->setClippingRect(mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE));
			//(*it).first->setYPosition(mAbsoluteDimensions.y + (*it).second.y - parentAbsY,QGUI_GMM_ABSOLUTE);
			(*it).first->setYPosition(mAbsoluteDimensions.y - parentAbsY + (*it).second.y,QGUI_GMM_ABSOLUTE);
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
		if((mParentWidget->getWidgetType() == TYPE_LIST) && dynamic_cast<List*>(mParentWidget)->getAutoSizeHeight())
			return;

		Rect wDimensions = w->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE);
		if(!wDimensions.inside(mAbsoluteDimensions))
			return;

		Ogre::Real leftX = ((wDimensions.x - mAbsoluteDimensions.x) / mAbsoluteDimensions.width);
		//Ogre::Real leftX = (wDimensions.x - mAbsoluteDimensions.x);
		Ogre::Real rightX = (((wDimensions.x + wDimensions.width) - mAbsoluteDimensions.x) / mAbsoluteDimensions.width);
		//Ogre::Real rightX = ((wDimensions.x + wDimensions.width) / mAbsoluteDimensions.width);

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
			mTopBar->setValue(rightX * (1 - mTopBar->getSliderWidth()));
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
}
