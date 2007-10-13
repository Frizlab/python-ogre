#include "QuickGUIScrollPane.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	ScrollPane::ScrollPane(const Ogre::String& instanceName, const Size& pixelSize, GUIManager* gm) :
		Widget(instanceName,pixelSize,"",gm),
		mScrollBarWidth(20),
		mHorizontalButtonLayout(HorizontalScrollBar::BUTTON_LAYOUT_OPPOSITE),
		mVerticalButtonLayout(VerticalScrollBar::BUTTON_LAYOUT_OPPOSITE)
	{
		mWidgetType = TYPE_SCROLL_PANE;
		mGainFocusOnClick = false;

		mBottomBar = new HorizontalScrollBar(mInstanceName+".BottomScrollBar",Size(mSize.width - 20,20),"qgui.scrollbar.horizontal.png",gm);
		mBottomBar->setQuadLayer(Quad::LAYER_MENU);
		mBottomBar->setInheritQuadLayer(false);
		mBottomBar->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		mBottomBar->setVerticalAnchor(ANCHOR_VERTICAL_BOTTOM);
		mBottomBar->setShowWithParent(false);
		mBottomBar->addOnScrollEventHandler(&ScrollPane::onHorizontalScroll,this);

		mRightBar = new VerticalScrollBar(mInstanceName+".RightScrollBar",Size(20,mSize.height - 20),"qgui.scrollbar.vertical.png",gm);
		mRightBar->setQuadLayer(Quad::LAYER_MENU);
		mRightBar->setInheritQuadLayer(false);
		mRightBar->setHorizontalAnchor(ANCHOR_HORIZONTAL_RIGHT);
		mRightBar->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mRightBar->setShowWithParent(false);
		mRightBar->addOnScrollEventHandler(&ScrollPane::onVerticalScroll,this);
	}

	ScrollPane::~ScrollPane()
	{
	}

	void ScrollPane::_determinePaneBounds()
	{
		// Scroll Pane should not be smaller than parent width/height
		Size parentSize = mParentWidget->getSize();

		// find the minimum and maximum region encompassing the managed widgets.
		Ogre::Real width = parentSize.width;
		Ogre::Real height = parentSize.height;

		// Get min/max bounds for scroll pane.  By default, pane has same bounds as parent.
		// This may change depending on managed widgets that may lie above/below/left/right of current pane bounds.
		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			Rect wPixelDimensions = (*it)->getDimensions();
			
			if( (wPixelDimensions.x + wPixelDimensions.width) > width )
				width = (wPixelDimensions.x + wPixelDimensions.width);

			if( (wPixelDimensions.y + wPixelDimensions.height) > height )
				height = (wPixelDimensions.y + wPixelDimensions.height);
		}

		setSize(width,height);

		_syncBarWithParentDimensions();

		mBottomBar->setValue(-(mPosition.x) / mSize.width);
		mRightBar->setValue(-(mPosition.y) / mSize.height);
	}

	void ScrollPane::_syncBarWithParentDimensions()
	{
		Size parentSize = mParentWidget->getSize();

		if((mSize.width <= parentSize.width) || (!mParentWidget->isVisible()))
		{
			mBottomBar->hide();
			mRightBar->setHeight(mParentWidget->getHeight() - mRightBar->getYPosition());
		}
		else
		{
			if(mParentWidget->isVisible())
				mBottomBar->show();
			mRightBar->setHeight(mParentWidget->getHeight() - mRightBar->getYPosition() - mBottomBar->getHeight());
		}

		if((mSize.height <= parentSize.height) || (!mParentWidget->isVisible()))
		{
			mRightBar->hide();
			mBottomBar->setWidth(mParentWidget->getWidth());
		}
		else
		{
			if(mParentWidget->isVisible())
				mRightBar->show();
			mBottomBar->setWidth(mParentWidget->getWidth() - mRightBar->getWidth());
		}

		// sync slider size
		// Take into consideration that scrollbar's take up some of the view.
		if(mRightBar->isVisible())
			parentSize.width -= mRightBar->getWidth();
		if(mBottomBar->isVisible())
			parentSize.height -= mBottomBar->getHeight();
		
		mBottomBar->setSliderWidth(parentSize.width / mSize.width);
		mRightBar->setSliderHeight(parentSize.height / mSize.height);
	}

	HorizontalScrollBar::ButtonLayout ScrollPane::getHorizontalButtonLayout()
	{
		return mHorizontalButtonLayout;
	}

	Widget* ScrollPane::getTargetWidget(const Point& pixelPosition)
	{
		return NULL;
	}

	VerticalScrollBar::ButtonLayout ScrollPane::getVerticalButtonLayout()
	{
		return mVerticalButtonLayout;
	}

	void ScrollPane::manageWidget(Widget* w)
	{
		if(w->getParentWidget()->getInstanceName() != mParentWidget->getInstanceName())
			return;

		Ogre::String name = w->getInstanceName();
		Type t = w->getWidgetType();
		if( (t == TYPE_TITLEBAR) || 
			(t == TYPE_BORDER) ||
			(name == mBottomBar->getInstanceName()) ||
			(name == mRightBar->getInstanceName()) )
			return;

		mManagedWidgets.push_back(w);

		w->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&ScrollPane::onChildClicked,this);
		w->addEventHandler(EVENT_POSITION_CHANGED,&ScrollPane::onChildPositionChanged,this);
		w->addEventHandler(EVENT_SIZE_CHANGED,&ScrollPane::onChildSizeChanged,this);

		// See if ScrollPane needs to be enlarged.
		Point widgetExtents = w->getPosition() + w->getSize();
		bool resized = false;
		if(widgetExtents.x > mSize.width)
		{
			setWidth(widgetExtents.x);
			resized = true;
		}
		if(widgetExtents.y > mSize.height)
		{
			setHeight(widgetExtents.y);
			resized = true;
		}

		if(resized)
		{
			_syncBarWithParentDimensions();

			mBottomBar->setValue(-(mPosition.x) / mSize.width);
			mRightBar->setValue(-(mPosition.y) / mSize.height);
		}
	}

	void ScrollPane::manageWidgets()
	{
		if(mParentWidget == NULL)
			return;

		setSize(mParentWidget->getSize());

		// Manage Parent widgets, except for TitleBar, Borders, and 2 Scroll Bars:
		mManagedWidgets.clear();
		std::vector<Widget*>* parentChildren = mParentWidget->getChildWidgetList();
		std::vector<Widget*>::iterator it;
		for(it = parentChildren->begin(); it != parentChildren->end(); ++it)
		{
			manageWidget((*it));
		}
	}

	void ScrollPane::onChildAddedToParent(const EventArgs& args)
	{
		manageWidget(dynamic_cast<const WidgetEventArgs&>(args).widget);		
	}

	void ScrollPane::onChildRemovedFromParent(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;

		// remove widget pointer from managed list
		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			if( w->getInstanceName() == (*it)->getInstanceName() )
			{
				mManagedWidgets.erase(it);
				break;
			}
		}

		_determinePaneBounds();
	}

	void ScrollPane::onChildClicked(const EventArgs& args)
	{
		scrollIntoView(dynamic_cast<const WidgetEventArgs&>(args).widget);
	}

	void ScrollPane::onChildPositionChanged(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		Point widgetExtents = w->getPosition() + w->getSize();
		bool resized = false;
		if(widgetExtents.x > mSize.width)
		{
			setWidth(widgetExtents.x);
			resized = true;
		}
		if(widgetExtents.y > mSize.height)
		{
			setHeight(widgetExtents.y);
			resized = true;
		}

		if(resized)
		{
			_syncBarWithParentDimensions();

			mBottomBar->setValue(-(mPosition.x) / mSize.width);
			mRightBar->setValue(-(mPosition.y) / mSize.height);
		}
	}

	void ScrollPane::onChildSizeChanged(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		Point widgetExtents = w->getPosition() + w->getSize();
		bool resized = false;
		if(widgetExtents.x > mSize.width)
		{
			setWidth(widgetExtents.x);
			resized = true;
		}
		if(widgetExtents.y > mSize.height)
		{
			setHeight(widgetExtents.y);
			resized = true;
		}

		if(resized)
		{
			_syncBarWithParentDimensions();

			mBottomBar->setValue(-(mPosition.x) / mSize.width);
			mRightBar->setValue(-(mPosition.y) / mSize.height);
		}
	}

	void ScrollPane::onParentSizeChanged(const EventArgs& args)
	{
		_syncBarWithParentDimensions();
	}

	void ScrollPane::onHorizontalScroll(const EventArgs& args)
	{
		// Move Scroll Pane
		setXPosition(-(mBottomBar->getValue()) * mSize.width);

		// Get parent's on-screen dimensions.
		Rect parentDimensions(mParentWidget->getScreenPosition() + mParentWidget->getScrollOffset(),mParentWidget->getSize());

		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			(*it)->_setScrollXOffset(mPosition.x);
		}
	}

	void ScrollPane::onVerticalScroll(const EventArgs& args)
	{
		// Move Scroll Pane
		setYPosition(-(mRightBar->getValue()) * mSize.height);

		// Get parent's on-screen dimensions.
		Rect parentDimensions(mParentWidget->getScreenPosition() + mParentWidget->getScrollOffset(),mParentWidget->getSize());

		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			(*it)->_setScrollYOffset(mPosition.y);
		}
	}

	void ScrollPane::setHorizontalButtonLayout(HorizontalScrollBar::ButtonLayout layout)
	{
		mHorizontalButtonLayout = layout;
		mBottomBar->setButtonLayout(mHorizontalButtonLayout);
	}

	void ScrollPane::scrollIntoView(Widget* w)
	{
		Rect wDimensions(w->getScreenPosition(),w->getSize());
		if(!wDimensions.inside(Rect(getScreenPosition(),mSize)))
			return;

		Point parentPosition = mParentWidget->getPosition();
		Size parentSize = mParentWidget->getSize();
		Point parentScreenPos = mParentWidget->getScreenPosition() + mParentWidget->getScrollOffset();

		Point widgetPosition = w->getPosition();
		Size widgetSize = w->getSize();
		Point widgetScreenPos = w->getScreenPosition() + w->getScrollOffset();

		// see if we will be scrolling left, right, or not at all
		if( widgetScreenPos.x < parentScreenPos.x )
		{
			mBottomBar->setValue(widgetPosition.x / mSize.width);
		}
		else if( (widgetScreenPos.x + wDimensions.width) > (parentScreenPos.x + parentSize.width) )
		{
			mBottomBar->setValue((widgetPosition.x + wDimensions.width) / mSize.width);
		}

		// see if we will be scrolling up, down, or not at all
		if( widgetScreenPos.y < parentScreenPos.y )
		{
			mRightBar->setValue((parentPosition.y - widgetPosition.y) / mSize.height);
		}
		else if( (widgetScreenPos.y + wDimensions.height) > (parentScreenPos.y + parentSize.height) )
		{
			mRightBar->setValue((widgetPosition.y + wDimensions.height) / mSize.height);
		}
	}

	void ScrollPane::setVerticalButtonLayout(VerticalScrollBar::ButtonLayout layout)
	{
		mVerticalButtonLayout = layout;
		mRightBar->setButtonLayout(mVerticalButtonLayout);
	}

	void ScrollPane::setPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		Widget::setPosition(pixelX,pixelY);
	}

	void ScrollPane::setPosition(const Point& pixelPosition)
	{
		Widget::setPosition(pixelPosition);
	}
}
