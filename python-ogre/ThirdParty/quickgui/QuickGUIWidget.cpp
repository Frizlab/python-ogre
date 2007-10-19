#include "QuickGUIWidget.h"
#include "QuickGUIManager.h"
#include "QuickGUIPanel.h"
#include "QuickGUIWindow.h"
#include "QuickGUISheet.h"
#include "QuickGUIBorder.h"

namespace QuickGUI
{
	Widget::Widget(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String textureName, GUIManager* gm) :
		mGUIManager(gm),
		mCanResize(false),
		mDragXOnly(false),
		mDragYOnly(false),
		mDraggingEnabled(false),
		mEnabled(true),
		mGainFocusOnClick(true),
		mGrabbed(false),
		mHideWithParent(true),
		mHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT),
		mInheritClippingWidget(true),
		mInheritQuadLayer(true),
		mInstanceName(instanceName),
		mMovingEnabled(true),
		mOffset(0),
		mPosition(Point(0,0)),
		mParentPanel(0),
		mParentSheet(0),
		mParentWidget(0),
		mParentWindow(0),
		mQuadContainer(0),
		mQuadLayer(Quad::LAYER_CHILD),
		mRepositionOverTime(false),
		mResizeOverTime(false),
		mScrollOffset(Point::ZERO),
		mScrollPaneAccessible(true),
		mShowWithParent(true),
		mSkinComponent(""),
		mTextureLocked(false),
		mUseBorders(false),
		mVerticalAnchor(ANCHOR_VERTICAL_TOP),
		mVisible(true),
		mWidgetImage(NULL)
	{
		_initEventHandlers();
		mQuad = _createQuad();
		mWidgetToDrag = this;

		setTexture(textureName);
		setSize(pixelSize);

		// Add event handlers before call to setDimensions, if you want the widget handlers to be called.
		// This is important for widgets to position themself correctly.
		addEventHandler(EVENT_POSITION_CHANGED,&Widget::onPositionChanged,this);
		addEventHandler(EVENT_SIZE_CHANGED,&Widget::onSizeChanged,this);

		for(int i = 0; i < 8; ++i)
			mBorders[i] = NULL;
	}

	Widget::~Widget()
	{
		if(mParentWidget != NULL)
			mParentWidget->removeChild(this);

		// Safest route is to destroy children first thing.
		removeAndDestroyAllChildWidgets();

		for(std::vector<Quad*>::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
			delete (*it);
		mQuads.clear();

		delete mWidgetImage;

		// Cleanup Event Handlers.
		int index = 0;
		while( index < NUM_EVENTS )
		{
			for(std::vector<MemberFunctionSlot*>::iterator it = mUserEventHandlers[index].begin(); it != mUserEventHandlers[index].end(); ++it)
				delete (*it);
			mUserEventHandlers[index].clear();

			++index;
		}

		// Remove name from GUIManager name list. (So another widget can be created with this name, if desired)
		mGUIManager->notifyNameFree(mInstanceName);
	}

	void Widget::__constructor()
	{
	}

	void Widget::_applyAnchors()
	{
		if(mParentWidget == NULL)
			return;

		Size parentSize = mParentWidget->getSize();

		// Vertical anchor:
		switch(mVerticalAnchor)
		{
		case ANCHOR_VERTICAL_TOP_BOTTOM:
			setHeight(parentSize.height - mPixelsFromParentBottom - mPosition.y);
			break;
		case ANCHOR_VERTICAL_TOP:
			break;
		case ANCHOR_VERTICAL_BOTTOM:
			mPosition.y = parentSize.height - mPixelsFromParentBottom - mSize.height;
			break;
		case ANCHOR_VERTICAL_NONE:
			break;
		}

		// Horizontal anchor:
		switch(mHorizontalAnchor)
		{
		case ANCHOR_HORIZONTAL_LEFT_RIGHT:
			setWidth(parentSize.width - mPixelsFromParentRight - mPosition.x);
			break;
		case ANCHOR_HORIZONTAL_LEFT:
			break;
		case ANCHOR_HORIZONTAL_RIGHT:
			mPosition.x = parentSize.width - mPixelsFromParentRight - mSize.width;
			break;
		case ANCHOR_HORIZONTAL_NONE:
			break;
		}

		redraw();
	}

	void Widget::_createBorders()
	{
		// By default, borders have a 5 pixel thickness
		Ogre::Real overlap = 1;
		Ogre::Real thickness = 5;

		// Corners - only create them if there are textures for them.
		if(mGUIManager->textureExists(mTextureName+".border.topleft"+mTextureExtension))
		{
			mBorders[Border::BORDER_TYPE_TOP_LEFT] = new Border(mInstanceName + ".TopLeftCornerBorder",Border::BORDER_TYPE_TOP_LEFT,Size(thickness,thickness),mTextureName+".border.topleft"+mTextureExtension,mGUIManager);
			addChild(mBorders[Border::BORDER_TYPE_TOP_LEFT]);
			mBorders[Border::BORDER_TYPE_TOP_LEFT]->setPosition(-thickness + overlap,-thickness + overlap);
		}

		if(mGUIManager->textureExists(mTextureName+".border.topright"+mTextureExtension))
		{
			mBorders[Border::BORDER_TYPE_TOP_RIGHT] = new Border(mInstanceName + ".TopRightCornerBorder",Border::BORDER_TYPE_TOP_RIGHT,Size(thickness,thickness),mTextureName+".border.topright"+mTextureExtension,mGUIManager);
			addChild(mBorders[Border::BORDER_TYPE_TOP_RIGHT]);
			mBorders[Border::BORDER_TYPE_TOP_RIGHT]->setPosition(mSize.width - overlap,-thickness + overlap);
		}
		
		if(mGUIManager->textureExists(mTextureName+".border.bottomleft"+mTextureExtension))
		{
			mBorders[Border::BORDER_TYPE_BOTTOM_LEFT] = new Border(mInstanceName + ".BottomLeftCornerBorder",Border::BORDER_TYPE_BOTTOM_LEFT,Size(thickness,thickness),mTextureName+".border.bottomleft"+mTextureExtension,mGUIManager);
			addChild(mBorders[Border::BORDER_TYPE_BOTTOM_LEFT]);
			mBorders[Border::BORDER_TYPE_BOTTOM_LEFT]->setPosition(-thickness + overlap,mSize.height - overlap);
		}
		
		if(mGUIManager->textureExists(mTextureName+".border.bottomleft"+mTextureExtension))
		{
			mBorders[Border::BORDER_TYPE_BOTTOM_RIGHT] = new Border(mInstanceName + ".BottomRightCornerBorder",Border::BORDER_TYPE_BOTTOM_RIGHT,Size(thickness,thickness),mTextureName+".border.bottomright"+mTextureExtension,mGUIManager);
			addChild(mBorders[Border::BORDER_TYPE_BOTTOM_RIGHT]);
			mBorders[Border::BORDER_TYPE_BOTTOM_RIGHT]->setPosition(mSize.width - overlap,mSize.height - overlap);
		}
		
		// Border edges - only create them if there are textures for them.
		if(mGUIManager->textureExists(mTextureName+".border.left"+mTextureExtension))
		{
			mBorders[Border::BORDER_TYPE_LEFT] = new Border(mInstanceName + ".LeftBorder",Border::BORDER_TYPE_LEFT,Size(thickness,mSize.height),mTextureName+".border.left"+mTextureExtension,mGUIManager);
			addChild(mBorders[Border::BORDER_TYPE_LEFT]);
			mBorders[Border::BORDER_TYPE_LEFT]->setPosition(-thickness + overlap,0);
		}
		
		if(mGUIManager->textureExists(mTextureName+".border.top"+mTextureExtension))
		{
			mBorders[Border::BORDER_TYPE_TOP] = new Border(mInstanceName + ".TopBorder",Border::BORDER_TYPE_TOP,Size(mSize.width,thickness),mTextureName+".border.top"+mTextureExtension,mGUIManager);
			addChild(mBorders[Border::BORDER_TYPE_TOP]);
			mBorders[Border::BORDER_TYPE_TOP]->setPosition(0,-thickness + overlap);
		}
		
		if(mGUIManager->textureExists(mTextureName+".border.right"+mTextureExtension))
		{
			mBorders[Border::BORDER_TYPE_RIGHT] = new Border(mInstanceName + ".RightBorder",Border::BORDER_TYPE_RIGHT,Size(thickness,mSize.height),mTextureName+".border.right"+mTextureExtension,mGUIManager);
			addChild(mBorders[Border::BORDER_TYPE_RIGHT]);
			mBorders[Border::BORDER_TYPE_RIGHT]->setPosition(mSize.width - overlap,0);
		}
		
		if(mGUIManager->textureExists(mTextureName+".border.bottom"+mTextureExtension))
		{
			mBorders[Border::BORDER_TYPE_BOTTOM] = new Border(mInstanceName + ".BottomBorder",Border::BORDER_TYPE_BOTTOM,Size(mSize.width,thickness),mTextureName+".border.bottom"+mTextureExtension,mGUIManager);
			addChild(mBorders[Border::BORDER_TYPE_BOTTOM]);
			mBorders[Border::BORDER_TYPE_BOTTOM]->setPosition(0,mSize.height - overlap);
		}		
	}

	void Widget::_deriveAnchorValues()
	{
		Size parentSize = mParentWidget->getSize();
		mPixelsFromParentRight = parentSize.width - (mPosition.x + mSize.width);
		mPixelsFromParentBottom = parentSize.height - (mPosition.y + mSize.height);
	}

	void Widget::_destroyBorders()
	{
		for(int i = 0; i < 8; ++i)
		{
			if(mBorders[i] != NULL)
				mGUIManager->destroyWidget(mBorders[i]);
			mBorders[i] = NULL;
		}
	}

	Quad* Widget::_createQuad()
	{
		Quad* newQuad = new Quad(this);
		mQuads.push_back(newQuad);
		return newQuad;
	}

	void Widget::_detectHierarchy()
	{
		mParentSheet = NULL;
		mParentWindow = NULL;
		mParentPanel = NULL;

		Widget* w = mParentWidget;
		while( w != NULL )
		{
			switch(w->getWidgetType())
			{
			case TYPE_SHEET:
				mParentSheet = dynamic_cast<Sheet*>(w);
				break;
			case TYPE_WINDOW:
				mParentWindow = dynamic_cast<Window*>(w);
				break;
			case TYPE_PANEL:
				// It is possible to have a widget inside a panel inside a panel..
				// we need to gaurd against this, we only want the immediate parent panel.
				if( mParentPanel == NULL ) 
					mParentPanel = dynamic_cast<Panel*>(w);
				break;
			default: break;
			}

			w = w->getParentWidget();
		}
	}

	void Widget::_initEventHandlers()
	{
		int index = 0;
		// 22 common types of events currently
		while( index < NUM_EVENTS )
		{
			mUserEventHandlers[index].clear();
			mPropogateEventFiring[index] = false;
			++index;
		}
	}

	void Widget::_processFullTextureName(const Ogre::String& texture)
	{
		mFullTextureName = texture;

		if(mFullTextureName != "")
		{
			Ogre::String::size_type index = texture.find_last_of('.');
			if( index != Ogre::String::npos )
			{
				mTextureName = texture.substr(0,index);
				mTextureExtension = texture.substr(index,texture.length() - index);
			}
		}
		else
		{
			mTextureName = "";
			mTextureExtension = "";
		}
	}

	void Widget::_setScrollXOffset(Ogre::Real pixelXOffset)
	{
		mScrollOffset.x = pixelXOffset;
		mQuad->setXPosition(getScreenPosition().x + getScrollOffset().x);
		redraw();
	}

	void Widget::_setScrollYOffset(Ogre::Real pixelYOffset)
	{
		mScrollOffset.y = pixelYOffset;
		mQuad->setYPosition(getScreenPosition().y + getScrollOffset().y);
		redraw();
	}

	void Widget::addChild(Widget* w)
	{
		if(w->getParentWidget() != NULL)
			return;

		mChildWidgets.push_back(w);

		w->setParent(this);

		// Convert Widget's position to be relative to new parent.
		w->setPosition(w->getScreenPosition() - getScreenPosition());

		WidgetEventArgs args(w);
		fireEvent(EVENT_CHILD_ADDED,args);
	}

	void Widget::addEventHandler(Event EVENT, MemberFunctionSlot* function)
	{
		mUserEventHandlers[EVENT].push_back(function);
	}

	void Widget::allowResizing(bool allow)
	{
		mCanResize = allow;
	}

	void Widget::appearOverWidget(Widget* w)
	{
		if( (w->getQuadContainer() == NULL) || (mQuadContainer == NULL) )
			return;

		if( (w->getQuadContainer() != mQuadContainer) ||
			(w->getQuad()->getLayer() != mQuad->getLayer()) )
			return;

		setOffset(w->getHighestOffset() + 1);
	}

	void Widget::constrainDragging(bool DragXOnly, bool DragYOnly)
	{
		mDragXOnly = false;
		mDragYOnly = false;

		if(DragXOnly && DragYOnly)
			return;

		mDragXOnly = DragXOnly;
		mDragYOnly = DragYOnly;
	}

	void Widget::disable()
	{
		if((mWidgetType == Widget::TYPE_SHEET)) 
			return;

		setTexture(mDisabledTextureName,false);

		WidgetEventArgs args(this);
		fireEvent(EVENT_DISABLED,args);

		mEnabled = false;
	}

	void Widget::drag(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		if(!mDraggingEnabled) 
			return;

		MouseEventArgs args(this);
		args.position = mGUIManager->getMouseCursor()->getPosition();
		args.moveDelta.x = pixelX;
		args.moveDelta.y = pixelY;
		
		if(mWidgetToDrag != NULL) 
		{
			if(mDragXOnly)
				mWidgetToDrag->moveX(pixelX);
			else if(mDragYOnly)
				mWidgetToDrag->moveY(pixelY);
			else
				mWidgetToDrag->move(pixelX,pixelY);
		}

		// fire onDragged Event.		
		fireEvent(EVENT_DRAGGED,args);
	}

	bool Widget::draggingEnabled()
	{
		return mDraggingEnabled;
	}

	void Widget::enable()
	{
		if(mWidgetType == Widget::TYPE_SHEET) 
			return;

		mEnabled = true;

		setTexture(mFullTextureName,false);

		WidgetEventArgs args(this);
		fireEvent(EVENT_ENABLED,args);
	}

	bool Widget::enabled()
	{
		return mEnabled;
	}

	void Widget::enableDragging(bool enable)
	{
		mDraggingEnabled = enable;
	}

	void Widget::focus()
	{
		mGUIManager->setActiveWidget(this);
	}

	Point Widget::getActualPosition()
	{
		return getScreenPosition() + getScrollOffset();
	}

	Ogre::String Widget::getDisabledTexture()
	{
		return mDisabledTextureName;
	}

	std::vector<Widget*>* Widget::getChildWidgetList()
	{
		return &mChildWidgets;
	}

	Widget* Widget::getChildWidget(const Ogre::String& name)
	{
		if( name == "" ) 
			return NULL;
		if( mInstanceName == name ) 
			return this;

		Widget* w = NULL;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (w = (*it)->getChildWidget(name)) != NULL )
			{
				return w;
			}
		}

		return NULL;
	}

	Widget* Widget::getChildWidget(Type t, unsigned int index)
	{
		int count = -1;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if((*it)->getWidgetType() == t)
				++count;

			if(count == index)
				return (*it);
		}

		return NULL;
	}

	Ogre::String Widget::getDefaultSkin()
	{
		if(mParentSheet == NULL) return "qgui";
		else return mParentSheet->getDefaultSkin();
	}

	bool Widget::getGainFocusOnClick()
	{
		return mGainFocusOnClick;
	}

	bool Widget::getGrabbed()
	{
		return mGrabbed;
	}

	Ogre::Real Widget::getHeight()
	{
		return mSize.height;
	}

	bool Widget::getHideWithParent()
	{
		return mHideWithParent;
	}

	int Widget::getHighestOffset()
	{
		// iterate through child widgets..
		Widget* w = NULL;
		// Get the widget with the highest offset
		int widgetOffset = mOffset;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getWidgetType() == TYPE_SCROLL_PANE )
				continue;

			int temp = (*it)->getHighestOffset();
			if( temp > widgetOffset )
				widgetOffset = temp;
		}

		return widgetOffset;
	}

	Widget::HorizontalAnchor Widget::getHorizontalAnchor()
	{
		return mHorizontalAnchor;
	}

	bool Widget::getInheritClippingWidget()
	{
		return mInheritClippingWidget;
	}

	bool Widget::getInheritQuadLayer()
	{
		return mInheritQuadLayer;
	}

	Ogre::String Widget::getInstanceName()
	{
		return mInstanceName;
	}

	bool Widget::getMovingEnabled()
	{
		return mMovingEnabled;
	}

	int Widget::getOffset()
	{
		return mOffset;
	}

	Panel* Widget::getParentPanel()
	{
		return mParentPanel;
	}

	Sheet* Widget::getParentSheet()
	{
		return mParentSheet;
	}

	Widget* Widget::getParentWidget()
	{
		return mParentWidget;
	}

	Window* Widget::getParentWindow()
	{
		return mParentWindow;
	}

	bool Widget::getPropogateEventFiring(Event e)
	{
		return mPropogateEventFiring[e];
	}

	Quad* Widget::getQuad()
	{
		return mQuad;
	}

	QuadContainer* Widget::getQuadContainer()
	{
		return mQuadContainer;
	}

	Quad::Layer Widget::getQuadLayer()
	{
		return mQuadLayer;
	}

	Point Widget::getScreenPosition()
	{
		if(mParentWidget == NULL)
			return Point::ZERO;
		
		return mParentWidget->getScreenPosition() + mPosition;
	}

	bool Widget::getScrollPaneAccessible()
	{
		return mScrollPaneAccessible;
	}

	Ogre::String Widget::getSkinComponent()
	{
		return mSkinComponent;
	}

	bool Widget::getShowWithParent()
	{
		return mShowWithParent;
	}

	Rect Widget::getDimensions()
	{
		return Rect(mPosition,mSize);
	}

	Ogre::String Widget::getFontName()
	{
		return mFontName;
	}

	GUIManager* Widget::getGUIManager()
	{
		return mGUIManager;
	}

	Point Widget::getPosition()
	{
		return mPosition;
	}

	Point Widget::getScrollOffset()
	{
		if( mParentWidget == NULL )
			return mScrollOffset;

		return mParentWidget->getScrollOffset() + mScrollOffset;
	}

	Size Widget::getSize()
	{
		return mSize;
	}

	Widget* Widget::getTargetWidget(const Point& pixelPosition)
	{
		if( !mQuad->visible() || !mEnabled )
			return NULL;

		if( mCanResize )
		{
			// Check borders
			for(int i = 0; i < 8; ++i)
			{
				if((mBorders[i] != NULL) && (mBorders[i]->isPointWithinBounds(pixelPosition)))
					return mBorders[i];
			}
		}

		// If position is not inside this widget, it can't be inside a child widget. (except menus, which are handled differently)
		if( !isPointWithinBounds(pixelPosition) ) 
			return NULL;

		Widget* w = NULL;

		// Iterate through Menu Layer Child Widgets.
		int widgetOffset = 0;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getQuadLayer() == Quad::LAYER_CHILD )
				continue;

			Widget* temp = (*it)->getTargetWidget(pixelPosition);
			if( (temp != NULL) && (temp->getOffset() > widgetOffset) )
			{
				widgetOffset = temp->getOffset();
				w = temp;
			}
		}
		if(w != NULL)
			return w;

		// Iterate through Child Layer Child Widgets.
		widgetOffset = 0;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getQuadLayer() == Quad::LAYER_MENU )
				continue;

			Widget* temp = (*it)->getTargetWidget(pixelPosition);
			if( (temp != NULL) && (temp->getOffset() > widgetOffset) )
			{
				widgetOffset = temp->getOffset();
				w = temp;
			}
		}
		if(w != NULL)
			return w;

		return this;
	}

	Ogre::String Widget::getTextureName(bool includeExtension)
	{
		if(includeExtension)
			return mFullTextureName;
		else
			return mTextureName;
	}

	Widget::Type Widget::getWidgetType()
	{
		return mWidgetType;
	}

	Widget::VerticalAnchor Widget::getVerticalAnchor()
	{
		return mVerticalAnchor;
	}

	Ogre::Real Widget::getWidth()
	{
		return mSize.width;
	}

	Ogre::Real Widget::getXPosition()
	{
		return mPosition.x;
	}

	Ogre::Real Widget::getYPosition()
	{
		return mPosition.y;
	}

	void Widget::hide()
	{
		mGrabbed = false;

		bool currentlyVisible = mVisible;
		mVisible = false;

		for( std::vector<Quad*>::iterator it = mQuads.begin(); it != mQuads.end(); ++it )
			(*it)->setVisible(false);

		// hide children
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if((*it)->getHideWithParent())
				(*it)->hide();
		}

		// if widget is the active widget at time of being hidden, set sheet to active widget.
		if(mGUIManager->getActiveWidget() == this)
			mGUIManager->setActiveWidget(mParentSheet);
		// if mouse cursor is over widget at time of being hidden, tell GUIManager to find the next widget mouse is over
		if(mGUIManager->getMouseOverWidget() == this)
			mGUIManager->injectMouseMove(0,0);

		// Only fire event if we change visibility.  If we were already hidden, don't fire.
		if(currentlyVisible)
		{
			WidgetEventArgs args(this);
			fireEvent(EVENT_HIDDEN,args);
		}
	}

	bool Widget::isPointWithinBounds(const Point& pixelPosition)
	{
		if(!mQuad->visible())
			return false;

		if(!mQuad->isPointWithinBounds(pixelPosition))
			return false;

		if(overTransparentPixel(pixelPosition)) 
			return false;

		return true;
	}

	bool Widget::isVisible()
	{
		return mVisible;
	}

	bool Widget::isChild(Widget* w)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( w == (*it) )
				return true;
		}

		return false;
	}

	void Widget::move(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		if(!mMovingEnabled) 
			return;

		setPosition(mPosition.x + pixelX,mPosition.y + pixelY);
	}

	void Widget::move(const Point& pixelOffset)
	{
		move(pixelOffset.x,pixelOffset.y);
	}

	void Widget::moveX(Ogre::Real pixelX)
	{
		if(!mMovingEnabled) 
			return;

		setXPosition(mPosition.x + pixelX);
	}

	void Widget::moveY(Ogre::Real pixelY)
	{
		if(!mMovingEnabled) 
			return;

		setYPosition(mPosition.y + pixelY);
	}

	bool Widget::fireEvent(Event e, const EventArgs& args)
	{
		if(!mEnabled || (mUserEventHandlers[e].empty() && !mPropogateEventFiring[e])) 
			return false;

		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*>* userEventHandlers = &(mUserEventHandlers[e]);
		for( it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mPropogateEventFiring[e])
			mParentWidget->fireEvent(e,args);

		return true; 
	}

	void Widget::lockTexture()
	{
		mTextureLocked = true;
	}

	void Widget::onPositionChanged(const EventArgs& args)
	{
		// maintain child widget positions
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->redraw();
	}

	void Widget::onSizeChanged(const EventArgs& args)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->_applyAnchors();
	}

	bool Widget::overTransparentPixel(const Point& mousePixelPosition)
	{
		if( mWidgetImage == NULL ) 
			return false;

		Point pt = mousePixelPosition - getScreenPosition() - getScrollOffset();
		Ogre::Real relX = pt.x / mSize.width;
		Ogre::Real relY = pt.y / mSize.height;

		// Reason I subtract 1 from width and height: Cannot access pixel 10 in an image of width 10. 0-9..
		Ogre::ColourValue c = mWidgetImage->getColourAt((relX * mWidgetImage->getWidth()) - 1, (relY * mWidgetImage->getHeight()) - 1,0);
		if( c.a <= 0.0 ) 
			return true;

		return false;
	}

	void Widget::redraw()
	{
		mQuad->setDimensions(Rect(getScreenPosition() + getScrollOffset(),mSize));

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->redraw();
	}

	void Widget::removeChild(Widget* w)
	{
		removeChild(w->getInstanceName());
	}

	void Widget::removeChild(const Ogre::String& widgetName)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if((*it)->getInstanceName() == widgetName)
			{
				WidgetEventArgs args((*it));
				fireEvent(EVENT_CHILD_REMOVED,args);

				(*it)->setQuadContainer(NULL);
				(*it)->setParent(NULL);
				mChildWidgets.erase(it);
				return;
			}
		}
	}

	void Widget::removeAndDestroyAllChildWidgets()
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			WidgetEventArgs args((*it));
			fireEvent(EVENT_CHILD_REMOVED,args);

			(*it)->setQuadContainer(NULL);
			(*it)->setParent(NULL);
			delete (*it);
		}
		mChildWidgets.clear();
	}

	bool Widget::resizingAllowed()
	{
		return mCanResize;
	}

	void Widget::resizeOverTime(Ogre::Real seconds, Size finalPixelSize)
	{
		if(mParentWidget == NULL)
			return;

		mResizeOverTime = true;
		mResizeTime = seconds;
		mResizeTimer = 0;
		mInitialPixelSize = Size(mSize.width,mSize.height);

		mFinalPixelSize.width = finalPixelSize.width / mGUIManager->getViewportWidth();
		mFinalPixelSize.height = finalPixelSize.height / mGUIManager->getViewportHeight();
	}

	void Widget::setBaseTexture(const Ogre::String& textureName)
	{
		// separate extension from name, if exists.
		_processFullTextureName(textureName);

		mDisabledTextureName = mTextureName + ".disabled" + mTextureExtension;
	}

	void Widget::timeElapsed(Ogre::Real time) 
	{
		if(!mEnabled)
			return;

		if(mResizeOverTime)
		{
			Ogre::Real totalWidthChange = mFinalPixelSize.width - mInitialPixelSize.width;
			setWidth(mInitialPixelSize.width + (totalWidthChange * (mResizeTimer / mResizeTime)));
			Ogre::Real totalHeightChange = mFinalPixelSize.height - mInitialPixelSize.height;
			setHeight(mInitialPixelSize.height + (totalHeightChange * (mResizeTimer / mResizeTime)));

			mResizeTimer += time;
			if(mResizeTimer >= mResizeTime)
				mResizeOverTime = false;
		}
	}

	void Widget::setClippingWidget(Widget* w, bool recursive)
	{
		for(std::vector<Quad*>::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
		{
			if( (*it)->getInheritClippingWidget() )
				(*it)->setClippingWidget(w);
		}

		if(recursive)
		{
			for(std::vector<Widget*>::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
			{
				if( (*it)->getInheritClippingWidget() )
					(*it)->setClippingWidget(w);
			}
		}
	}

	void Widget::setDimensions(const Rect& pixelDimensions)
	{
		setPosition(pixelDimensions.x,pixelDimensions.y);
		setSize(pixelDimensions.width,pixelDimensions.height);
	}

	void Widget::setDisabledTexture(const Ogre::String& disabledTexture)
	{
		mDisabledTextureName = disabledTexture;
	}

	void Widget::setDraggingWidget(Widget* w)
	{
		mWidgetToDrag = w;
	}

	void Widget::setFont(const Ogre::String& fontScriptName, bool recursive) 
	{ 
		mFontName = fontScriptName;

		if(recursive)
		{
			for(std::vector<Widget*>::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
				(*it)->setFont(fontScriptName,recursive);
		}
	}

	void Widget::setGainFocusOnClick(bool gainFocus)
	{
		mGainFocusOnClick = gainFocus;
	}

	void Widget::setGrabbed(bool grabbed)
	{
		mGrabbed = grabbed;
	}

	void Widget::setHeight(Ogre::Real pixelHeight)
	{
		if(mWidgetType == Widget::TYPE_SHEET)
			return;

		mSize.height = pixelHeight;

		WidgetEventArgs args(this);
		fireEvent(EVENT_SIZE_CHANGED,args);

		mQuad->setHeight(mSize.height);
	}

	void Widget::setHideWithParent(bool hide)
	{
		mHideWithParent = hide;
	}

	void Widget::setHorizontalAnchor(Widget::HorizontalAnchor a)
	{
		mHorizontalAnchor = a;
	}

	void Widget::setInheritClippingWidget(bool inherit)
	{
		mInheritClippingWidget = inherit;
	}

	void Widget::setInheritQuadLayer(bool inherit)
	{
		mInheritQuadLayer = inherit;
	}

	void Widget::setMovingEnabled(bool enable)
	{
		mMovingEnabled = enable;
	}

	void Widget::setName(const Ogre::String& name)
	{
		if(!mGUIManager->isNameUnique(name))
			mInstanceName = mGUIManager->generateName(mWidgetType);
		else
			mInstanceName = name;
	}

	void Widget::setOffset(int offset)
	{
		int delta = offset - mOffset;
		mOffset = offset;

		for(std::vector<Quad*>::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
			(*it)->setOffset((*it)->getOffset() + delta);

		for(std::vector<Widget*>::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->setOffset((*it)->getOffset() + delta);
	}

	void Widget::setPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mPosition.x = pixelX;
		mPosition.y = pixelY;

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);

		// update anchors
		_deriveAnchorValues();
		
		mQuad->setPosition(getScreenPosition() + getScrollOffset());
	}

	void Widget::setPosition(const Point& pixelPoint)
	{
		setPosition(pixelPoint.x,pixelPoint.y);
	}

	void Widget::setPropogateEventFiring(Event e, bool propogate)
	{
		mPropogateEventFiring[e] = propogate;
	}

	void Widget::setScreenPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		setPosition(Point(pixelX,pixelY) - mParentWidget->getScreenPosition());
	}

	void Widget::setScreenXPosition(const Ogre::Real& pixelX)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		setXPosition(pixelX - mParentWidget->getScreenPosition().x);
	}

	void Widget::setScreenYPosition(const Ogre::Real& pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		setYPosition(pixelY - mParentWidget->getScreenPosition().y);
	}

	void Widget::setScrollPaneAccessible(bool accessible)
	{
		mScrollPaneAccessible = accessible;
	}

	void Widget::setSize(const Ogre::Real& pixelWidth, const Ogre::Real& pixelHeight)
	{
		if(mWidgetType == Widget::TYPE_SHEET)
			return;

		mSize.width = pixelWidth;
		mSize.height = pixelHeight;

		WidgetEventArgs args(this);
		fireEvent(EVENT_SIZE_CHANGED,args);

		mQuad->setSize(mSize);
	}

	void Widget::setSize(const Size& pixelSize)
	{
		Widget::setSize(pixelSize.width,pixelSize.height);
	}

	void Widget::setShowWithParent(bool show)
	{
		mShowWithParent = show;
	}

	void Widget::setQuadLayer(Quad::Layer l)
	{
		mQuadLayer = l;

		for(std::vector<Quad*>::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
		{
			if((*it)->getInheritLayer())
				(*it)->setLayer(mQuadLayer);
		}

		for(std::vector<Widget*>::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getInheritQuadLayer() )
				(*it)->setQuadLayer(mQuadLayer);
		}
	}

	void Widget::setUseBorders(bool use)
	{
		mUseBorders = use;
		if(mUseBorders)
			_createBorders();
		else
			_destroyBorders();
	}

	void Widget::setVerticalAnchor(Widget::VerticalAnchor a)
	{
		mVerticalAnchor = a;
	}

	void Widget::setWidth(Ogre::Real pixelWidth)
	{
		if(mWidgetType == Widget::TYPE_SHEET)
			return;

		mSize.width = pixelWidth;

		WidgetEventArgs args(this);
		fireEvent(EVENT_SIZE_CHANGED,args);

		mQuad->setWidth(mSize.width);
	}

	void Widget::setXPosition(Ogre::Real pixelX)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mPosition.x = pixelX;

		// update anchor
		_deriveAnchorValues();

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);
		
		mQuad->setXPosition(getScreenPosition().x + getScrollOffset().x);
	}

	void Widget::setYPosition(Ogre::Real pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mPosition.y = pixelY;

		// update anchors
		_deriveAnchorValues();

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);
		
		mQuad->setYPosition(getScreenPosition().y + getScrollOffset().y);
	}

	void Widget::show()
	{
		bool currentlyVisible = mVisible;
		mVisible = true;
		for( std::vector<Quad*>::iterator it = mQuads.begin(); it != mQuads.end(); ++it )
		{
			if((*it)->getShowWithOwner())
				(*it)->setVisible(true);
		}

		// show children, except for Windows and lists of MenuList or ComboBox Widget.
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if((*it)->getShowWithParent())
				(*it)->show();
		}

		// Only fire event if we change visibility.  If we were already visible, don't fire.
		if(!currentlyVisible)
		{
			WidgetEventArgs args(this);
			fireEvent(EVENT_SHOWN,args);
		}
	}

	void Widget::setGUIManager(GUIManager* gm)
	{
		mGUIManager = gm;

		for(std::vector<Quad*>::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
			(*it)->setGUIManager(mGUIManager);

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->setGUIManager(gm);
	}

	void Widget::setParent(Widget* parent)
	{
		mParentWidget = parent;

		if(mParentWidget != NULL)
		{
			setQuadContainer(parent->getQuadContainer());
			setGUIManager(parent->getGUIManager());

			_detectHierarchy();
			// set the correct offset
			setOffset(mParentWidget->getOffset() + 1);
			// calculated properties
			_deriveAnchorValues();
			// inheritted properties
			if(mInheritClippingWidget)
				setClippingWidget(mParentWidget,true);
			if(!mParentWidget->isVisible())
				hide();
			if(mInheritQuadLayer)
				setQuadLayer(mParentWidget->getQuadLayer());
			mGainFocusOnClick = mParentWidget->getGainFocusOnClick();
		}

		WidgetEventArgs args(this);
		fireEvent(EVENT_PARENT_CHANGED,args);
	}

	void Widget::setQuadContainer(QuadContainer* container)
	{
		mQuadContainer = container;
		
		for(std::vector<Quad*>::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
			(*it)->_notifyQuadContainer(mQuadContainer);

		for( std::vector<Widget*>::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->setQuadContainer(mQuadContainer);
	}

	void Widget::setSkinComponent(const Ogre::String& skinComponent)
	{
		mSkinComponent = skinComponent;
	}

	void Widget::setTexture(const Ogre::String& textureName, bool updateBaseTexture)
	{
		if(mTextureLocked)
			return;

		if(updateBaseTexture)
			setBaseTexture(textureName);

		mQuad->setTexture(textureName);

		if(Utility::textureExistsOnDisk(textureName))
		{
			Ogre::Image i;
			i.load(textureName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			delete mWidgetImage;
			mWidgetImage = new Ogre::Image(i);
		}
	}

	void Widget::unlockTexture()
	{
		mTextureLocked = false;
	}
}
