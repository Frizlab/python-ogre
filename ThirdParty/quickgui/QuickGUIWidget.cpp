#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIWidget.h"
#include "QuickGUIManager.h"
#include "QuickGUIPanel.h"
#include "QuickGUIWindow.h"
#include "QuickGUISheet.h"
#include "QuickGUIBorder.h"
#include "QuickGUITextArea.h"

namespace QuickGUI
{
	Widget::Widget(const std::string& name, GUIManager* gm) :
		mCanResize(false),
		mDragXOnly(false),
		mDragYOnly(false),
		mDraggingEnabled(false),
		mEnabled(true),
		mEntered(false),
		mGainFocusOnClick(true),
		mGrabbed(false),
		mGUIManager(gm),
		mHideSkin(false),
		mHideWithParent(true),
		mHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT),
		mInheritOpacity(true),
		mInheritQuadLayer(true),
		mInstanceName(name),
		mMovingEnabled(true),
		mOffset(0),
		mOpacity(1),
		mPosition(Point(0,0)),
		mParentWidget(0),
		mQuadContainer(0),
		mQuadLayer(Quad::LAYER_CHILD),
		mScrollOffset(Point::ZERO),
		mScrollPaneAccessible(true),
		mShowWithParent(true),
		mSkinComponent(""),
		mSkinName(""),
		mSkinSet(NULL),
		mTextureLocked(false),
		mUseBorders(true),
		mUseTransparencyPicking(true),
		mVerticalAnchor(ANCHOR_VERTICAL_TOP),
		mVisible(true)
	{
		_initEventHandlers();
		mQuad = _createQuad();
		mWidgetToDrag = this;

		// Add event handlers before call to setDimensions, if you want the widget handlers to be called.
		// This is important for widgets to position themself correctly.
		addEventHandler(EVENT_POSITION_CHANGED,&Widget::onPositionChanged,this);
		addEventHandler(EVENT_SIZE_CHANGED,&Widget::onSizeChanged,this);

		setPropagateEventFiring(EVENT_MOUSE_ENTER,true);
		setPropagateEventFiring(EVENT_MOUSE_LEAVE,true);
	}

	Widget::~Widget()
	{
		if(mParentWidget != NULL)
			mParentWidget->removeChild(this);

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			delete (*it);
		mComponents.clear();

		// Safest route is to destroy children first thing.
		removeAndDestroyAllChildWidgets();

		for(QuadArray::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
			delete (*it);
		mQuads.clear();

		// Cleanup Event Handlers.
		int index = 0;
		while( index < (EVENT_END_OF_LIST) )
		{
			for(EventHandlerArray::iterator it = mUserEventHandlers[index].begin(); it != mUserEventHandlers[index].end(); ++it)
				delete (*it);
			mUserEventHandlers[index].clear();

			++index;
		}

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
		Border* b;

		if( !_hasComponent(mInstanceName+".TopLeftBorder") &&
			mSkinSet->containsImage(mSkinName + mSkinComponent + ".border.topleft" + mSkinSet->getImageExtension()))
		{
			b = dynamic_cast<Border*>(_createComponent(mInstanceName+".TopLeftBorder",TYPE_BORDER));
			b->setBorderType(Border::BORDER_TYPE_TOP_LEFT);
			b->getQuad()->setClipMode(Quad::CLIPMODE_GREATGRANDPARENT);
		}

		if( !_hasComponent(mInstanceName+".TopRightBorder") &&
			mSkinSet->containsImage(mSkinName + mSkinComponent + ".border.topright" + mSkinSet->getImageExtension()))
		{
			b = dynamic_cast<Border*>(_createComponent(mInstanceName+".TopRightBorder",TYPE_BORDER));
			b->setBorderType(Border::BORDER_TYPE_TOP_RIGHT);
			b->getQuad()->setClipMode(Quad::CLIPMODE_GREATGRANDPARENT);
		}

		if( !_hasComponent(mInstanceName+".BottomLeftBorder") &&
			mSkinSet->containsImage(mSkinName + mSkinComponent + ".border.bottomleft" + mSkinSet->getImageExtension()))
		{
			b = dynamic_cast<Border*>(_createComponent(mInstanceName+".BottomLeftBorder",TYPE_BORDER));
			b->setBorderType(Border::BORDER_TYPE_BOTTOM_LEFT);
			b->getQuad()->setClipMode(Quad::CLIPMODE_GREATGRANDPARENT);
		}

		if( !_hasComponent(mInstanceName+".BottomRightBorder") &&
			mSkinSet->containsImage(mSkinName + mSkinComponent + ".border.bottomright" + mSkinSet->getImageExtension()))
		{
			b = dynamic_cast<Border*>(_createComponent(mInstanceName+".BottomRightBorder",TYPE_BORDER));
			b->setBorderType(Border::BORDER_TYPE_BOTTOM_RIGHT);
			b->getQuad()->setClipMode(Quad::CLIPMODE_GREATGRANDPARENT);
		}

		if( !_hasComponent(mInstanceName+".LeftBorder") &&
			mSkinSet->containsImage(mSkinName + mSkinComponent + ".border.left" + mSkinSet->getImageExtension()))
		{
			b = dynamic_cast<Border*>(_createComponent(mInstanceName+".LeftBorder",TYPE_BORDER));
			b->setBorderType(Border::BORDER_TYPE_LEFT);
			b->getQuad()->setClipMode(Quad::CLIPMODE_GREATGRANDPARENT);
		}

		if( !_hasComponent(mInstanceName+".TopBorder") &&
			mSkinSet->containsImage(mSkinName + mSkinComponent + ".border.top" + mSkinSet->getImageExtension()))
		{
			b = dynamic_cast<Border*>(_createComponent(mInstanceName+".TopBorder",TYPE_BORDER));
			b->setBorderType(Border::BORDER_TYPE_TOP);
			b->getQuad()->setClipMode(Quad::CLIPMODE_GREATGRANDPARENT);
		}

		if( !_hasComponent(mInstanceName+".RightBorder") &&
			mSkinSet->containsImage(mSkinName + mSkinComponent + ".border.right" + mSkinSet->getImageExtension()))
		{
			b = dynamic_cast<Border*>(_createComponent(mInstanceName+".RightBorder",TYPE_BORDER));
			b->setBorderType(Border::BORDER_TYPE_RIGHT);
			b->getQuad()->setClipMode(Quad::CLIPMODE_GREATGRANDPARENT);
		}

		if( !_hasComponent(mInstanceName+".BottomBorder") &&
			mSkinSet->containsImage(mSkinName + mSkinComponent + ".border.bottom" + mSkinSet->getImageExtension()))
		{
			b = dynamic_cast<Border*>(_createComponent(mInstanceName+".BottomBorder",TYPE_BORDER));
			b->setBorderType(Border::BORDER_TYPE_BOTTOM);
			b->getQuad()->setClipMode(Quad::CLIPMODE_GREATGRANDPARENT);
		}
	}

	Widget* Widget::_createChild(const std::string& name, Type t)
	{
		Widget* w;
		switch(t)
		{
			case TYPE_BORDER:				w = new Border(name,mGUIManager);				break;
			case TYPE_BUTTON:				w = new Button(name,mGUIManager);				break;
			case TYPE_CHECKBOX:				w = new CheckBox(name,mGUIManager);				break;
			case TYPE_COMBOBOX:				w = new ComboBox(name,mGUIManager);				break;
			case TYPE_CONSOLE:				w = new Console(name,mGUIManager);				break;
			case TYPE_IMAGE:				w = new Image(name,mGUIManager);				break;
			case TYPE_LABEL:				w = new Label(name,mGUIManager);				break;
			case TYPE_LABELAREA:			w = new LabelArea(name,mGUIManager);			break;
			case TYPE_LIST:					w = new List(name,mGUIManager);					break;
			case TYPE_MENULABEL:			w = new MenuLabel(name,mGUIManager);			break;
			case TYPE_NSTATEBUTTON:			w = new NStateButton(name,mGUIManager);			break;
			case TYPE_PANEL:				w = new Panel(name,mGUIManager);				break;
			case TYPE_PROGRESSBAR:			w = new ProgressBar(name,mGUIManager);			break;
			case TYPE_RADIOBUTTON:			w = new RadioButton(name,mGUIManager);			break;
			case TYPE_SCROLL_PANE:			w = new ScrollPane(name,mGUIManager);			break;
			case TYPE_SCROLLBAR_HORIZONTAL: w = new HorizontalScrollBar(name,mGUIManager);	break;
			case TYPE_SCROLLBAR_VERTICAL:	w = new VerticalScrollBar(name,mGUIManager);	break;
			case TYPE_TEXTAREA:				w = new TextArea(name,mGUIManager);				break;
			case TYPE_TEXTBOX:				w = new TextBox(name,mGUIManager);				break;
			case TYPE_TITLEBAR:				w = new TitleBar(name,mGUIManager);				break;
			case TYPE_TRACKBAR_HORIZONTAL:	w = new HorizontalTrackBar(name,mGUIManager);	break;
			case TYPE_TRACKBAR_VERTICAL:	w = new VerticalTrackBar(name,mGUIManager);		break;
//			case TYPE_TREE:					w = new Tree(name,mGUIManager);					break;
			case TYPE_WINDOW:				w = new Window(name,mGUIManager);				break;
			default:						w = new Widget(name,mGUIManager);				break;
		}

		addChild(w);
		w->setSize(w->getSize());
		w->setFont(mFontName,true);
		w->setPosition(0,0);
		if(mSkinName != "")
			w->setSkin(mSkinName,true);

		WidgetEventArgs args(w);
		fireEvent(EVENT_CHILD_CREATED,args);

		if(!mVisible && w->getHideWithParent())
			w->hide();

		// Widgets that are created on the sheet directly do not get clipped.
		if(mWidgetType == TYPE_SHEET)
			w->getQuad()->setClipMode(Quad::CLIPMODE_NONE);

		return w;
	}

	Widget* Widget::_createComponent(const std::string& name, Type t)
	{
		Widget* w;
		switch(t)
		{
			case TYPE_BORDER:				w = new Border(name,mGUIManager);				break;
			case TYPE_BUTTON:				w = new Button(name,mGUIManager);				break;
			case TYPE_CHECKBOX:				w = new CheckBox(name,mGUIManager);				break;
			case TYPE_COMBOBOX:				w = new ComboBox(name,mGUIManager);				break;
			case TYPE_CONSOLE:				w = new Console(name,mGUIManager);				break;
			case TYPE_IMAGE:				w = new Image(name,mGUIManager);				break;
			case TYPE_LABEL:				w = new Label(name,mGUIManager);				break;
			case TYPE_LABELAREA:			w = new LabelArea(name,mGUIManager);			break;
			case TYPE_LIST:					w = new List(name,mGUIManager);					break;
			case TYPE_MENULABEL:			w = new MenuLabel(name,mGUIManager);			break;
			case TYPE_NSTATEBUTTON:			w = new NStateButton(name,mGUIManager);			break;
			case TYPE_PANEL:				w = new Panel(name,mGUIManager);				break;
			case TYPE_PROGRESSBAR:			w = new ProgressBar(name,mGUIManager);			break;
			case TYPE_RADIOBUTTON:			w = new RadioButton(name,mGUIManager);			break;
			case TYPE_SCROLL_PANE:			w = new ScrollPane(name,mGUIManager);			break;
			case TYPE_SCROLLBAR_HORIZONTAL: w = new HorizontalScrollBar(name,mGUIManager);	break;
			case TYPE_SCROLLBAR_VERTICAL:	w = new VerticalScrollBar(name,mGUIManager);	break;
			case TYPE_TEXTAREA:				w = new TextArea(name,mGUIManager);				break;
			case TYPE_TEXTBOX:				w = new TextBox(name,mGUIManager);				break;
			case TYPE_TITLEBAR:				w = new TitleBar(name,mGUIManager);				break;
			case TYPE_TRACKBAR_HORIZONTAL:	w = new HorizontalTrackBar(name,mGUIManager);	break;
			case TYPE_TRACKBAR_VERTICAL:	w = new VerticalTrackBar(name,mGUIManager);		break;
			case TYPE_WINDOW:				w = new Window(name,mGUIManager);				break;
			default:						w = new Widget(name,mGUIManager);				break;
		}

		mComponents.push_back(w);
		w->setParent(this);

		w->setSize(w->getSize());
		w->setFont(mFontName,true);
		w->setPosition(0,0);
		// Some Composition widgets will create components before inheritting skin name.
		if(mSkinName != "")
			w->setSkin(mSkinName,true);

		if(!mVisible && w->getHideWithParent())
			w->hide();

		return w;
	}

	void Widget::_deriveAnchorValues()
	{
		if(mParentWidget == NULL)
			return;

		Size parentSize = mParentWidget->getSize();
		mPixelsFromParentRight = parentSize.width - (mPosition.x + mSize.width);
		mPixelsFromParentBottom = parentSize.height - (mPosition.y + mSize.height);
	}

	void Widget::_destroyBorders()
	{
		WidgetArray::iterator it;
		it = mComponents.begin();
		while( it != mComponents.end() )
		{
			if((*it)->getWidgetType() == TYPE_BORDER)
			{
				Widget* w = (*it);
				mComponents.erase(it);
				mGUIManager->_destroyWidget(w);
				it = mComponents.begin();
			}
			else
				++it;
		}
	}

	Quad* Widget::_createQuad()
	{
		Quad* newQuad = new Quad(this);
		mQuads.push_back(newQuad);
		return newQuad;
	}

	bool Widget::_hasComponent(const std::string& name)
	{
		for(std::vector<Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if((*it)->getInstanceName() == name)
				return true;
		}

		return false;
	}

	void Widget::_initEventHandlers()
	{
		int index = 0;
		// 22 common types of events currently
		while( index < (EVENT_END_OF_LIST) )
		{
			mUserEventHandlers[index].clear();
			mPropogateEventFiring[index] = false;
			++index;
		}
	}

	void Widget::_setScrollXOffset(float pixelXOffset)
	{
		mScrollOffset.x = pixelXOffset;
		mQuad->setXPosition(getScreenPosition().x + getScrollOffset().x);
		redraw();
	}

	void Widget::_setScrollYOffset(float pixelYOffset)
	{
		mScrollOffset.y = pixelYOffset;
		mQuad->setYPosition(getScreenPosition().y + getScrollOffset().y);
		redraw();
	}

	bool Widget::isNameUnique(const std::string& name) const
	{
		WidgetArray::const_iterator it;
		for (it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
		{
			if ((*it)->getInstanceName() == name)
				return false;
		}
		return true;
	}

	std::string Widget::generateName(Widget::Type t)
	{
		std::string s;
		switch(t)
		{
			case Widget::TYPE_BORDER:				s = "Border";			break;
			case Widget::TYPE_BUTTON:				s = "Button";			break;
			case Widget::TYPE_CHECKBOX:				s = "CheckBox";			break;
			case Widget::TYPE_COMBOBOX:				s = "ComboBox";			break;
			case Widget::TYPE_CONSOLE:				s = "Console";			break;
			case Widget::TYPE_IMAGE:				s = "Image";			break;
			case Widget::TYPE_LABEL:				s = "Label";			break;
			case Widget::TYPE_LIST:					s = "List";				break;
			case Widget::TYPE_MENULABEL:			s = "MenuLabel";		break;
			case Widget::TYPE_LABELAREA:			s = "LabelArea";		break;
			case Widget::TYPE_TEXTAREA:				s = "TextArea";			break;
			case Widget::TYPE_NSTATEBUTTON:			s = "NStateButton";		break;
			case Widget::TYPE_PANEL:				s = "Panel";			break;
			case Widget::TYPE_PROGRESSBAR:			s = "ProgressBar";		break;
			case Widget::TYPE_RADIOBUTTON:			s = "RadioButton";		break;
			case Widget::TYPE_SCROLL_PANE:			s = "ScrollPane";		break;
			case Widget::TYPE_SCROLLBAR_HORIZONTAL: s = "HScrollBar";		break;
			case Widget::TYPE_SCROLLBAR_VERTICAL:	s = "VScrollBar";		break;
			case Widget::TYPE_TEXTBOX:				s = "TextBox";			break;
			case Widget::TYPE_TITLEBAR:				s = "TitleBar";			break;
			case Widget::TYPE_TRACKBAR_HORIZONTAL:	s = "HTrackBar";		break;
			case Widget::TYPE_TRACKBAR_VERTICAL:	s = "VTrackBar";		break;
			case Widget::TYPE_WINDOW:				s = "Window";			break;
			default:								s = "Widget";			break;
		}

		int counter = 1;
		while( !isNameUnique(s + Ogre::StringConverter::toString(counter)) )
			++counter;

		return (s + Ogre::StringConverter::toString(counter));
	}

	void Widget::addChild(Widget* w)
	{
		if(w->getParentWidget() != NULL)
			return;

		if (!isNameUnique(w->getInstanceName()))
		{
			throw Ogre::Exception(Ogre::Exception::ERR_DUPLICATE_ITEM,"Name \"" + w->getInstanceName() + "\" is already used in " + getInstanceName() ,"Widget::addChild");
		}

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

	void Widget::addEventListener(MemberFunctionSlot* function)
	{
		mEventListeners.push_back(function);
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

		//setTexture(mDisabledTextureName,false);

		WidgetEventArgs args(this);
		fireEvent(EVENT_DISABLED,args);

		mEnabled = false;
	}

	void Widget::drag(const float& pixelX, const float& pixelY)
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

	bool Widget::draggingEnabled() const
	{
		return mDraggingEnabled;
	}

	void Widget::enable()
	{
		if(mWidgetType == Widget::TYPE_SHEET)
			return;

		mEnabled = true;

		//setTexture(mFullTextureName,false);

		WidgetEventArgs args(this);
		fireEvent(EVENT_ENABLED,args);
	}

	bool Widget::enabled() const
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

	Rect Widget::getActualDimensions() const
	{
		return Rect(getActualPosition(),getActualSize());
	}

	float Widget::getActualOpacity() const
	{
		if((mParentWidget == NULL) || (!mInheritOpacity))
			return mOpacity;

		return mParentWidget->getActualOpacity() * mOpacity;
	}

	Point Widget::getActualPosition() const
	{
		return getScreenPosition() + getScrollOffset();
	}

	Size Widget::getActualSize() const
	{
		if( (mParentWidget == NULL) || (mQuad->getClipMode() == Quad::CLIPMODE_NONE) )
			return mSize;

		Rect parentDimensions(mParentWidget->getActualPosition(),mParentWidget->getActualSize());
		Rect myDimensions(getActualPosition(),mSize);

		Rect r = myDimensions.getIntersection(parentDimensions);

		return Size(r.width,r.height);
	}

	Border* Widget::getBorder(BorderType t)
	{
		for(std::vector<Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if(((*it)->getWidgetType() == TYPE_BORDER) && (dynamic_cast<Border*>(*it)->getBorderType() == t))
				return dynamic_cast<Border*>(*it);
		}

		return NULL;
	}

	const Border* Widget::getBorder(BorderType t) const
	{
		for(std::vector<Widget*>::const_iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if(((*it)->getWidgetType() == TYPE_BORDER) && (dynamic_cast<Border*>(*it)->getBorderType() == t))
				return dynamic_cast<Border*>(*it);
		}

		return NULL;
	}

	WidgetArray* Widget::getChildWidgetList()
	{
		return &mChildWidgets;
	}

	const WidgetArray* Widget::getChildWidgetList() const
	{
		return &mChildWidgets;
	}

	Widget* Widget::getChildWidget(const std::string& name)
	{
		if( name.empty() )
			return NULL;

		WidgetArray::iterator it;
		for ( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if (name == (*it)->getInstanceName())
			{
				return (*it);
			}
		}

		return NULL;
	}

	const Widget* Widget::getChildWidget(const std::string& name) const
	{
		if( name.empty() )
			return NULL;

		WidgetArray::const_iterator it;
		for ( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if (name == (*it)->getInstanceName())
			{
				return (*it);
			}
		}

		return NULL;
	}

	Widget* Widget::findChildWidget(const std::string& name)
	{
		if( name.empty() )
			return NULL;
		if( mInstanceName == name )
			return this;

		Widget* w = NULL;
		WidgetArray::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (w = (*it)->findChildWidget(name)) != NULL )
			{
				return w;
			}
		}

		return NULL;
	}

	const Widget* Widget::findChildWidget(const std::string& name) const
	{
		if( name.empty() )
			return NULL;
		if( mInstanceName == name )
			return this;

		Widget* w = NULL;
		WidgetArray::const_iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (w = (*it)->findChildWidget(name)) != NULL )
			{
				return w;
			}
		}

		return NULL;
	}

	const Widget* Widget::getChildWidget(Type t, unsigned int index) const
	{
		int count = -1;
		WidgetArray::const_iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if((*it)->getWidgetType() == t)
				++count;

			if(count == index)
				return (*it);
		}

		return NULL;
	}

	bool Widget::getGainFocusOnClick() const
	{
		return mGainFocusOnClick;
	}

	bool Widget::getGrabbed() const
	{
		return mGrabbed;
	}

	float Widget::getHeight() const
	{
		return mSize.height;
	}

	bool Widget::getHideWithParent() const
	{
		return mHideWithParent;
	}

	int Widget::getHighestOffset() const
	{
		// iterate through child widgets..
		Widget* w = NULL;
		// Get the widget with the highest offset
		int widgetOffset = mOffset;
		WidgetArray::const_iterator it;
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

	Widget::HorizontalAnchor Widget::getHorizontalAnchor() const
	{
		return mHorizontalAnchor;
	}

	bool Widget::getInheritQuadLayer() const
	{
		return mInheritQuadLayer;
	}

	std::string Widget::getInstanceName() const
	{
		return mInstanceName;
	}

	bool Widget::getMovingEnabled() const
	{
		return mMovingEnabled;
	}

	int Widget::getOffset() const
	{
		return mOffset;
	}

	Panel* Widget::getParentPanel()
	{
		Widget* w = mParentWidget;
		while((w != NULL) && (w->getWidgetType() != TYPE_PANEL))
			w = w->getParentWidget();

		return dynamic_cast<Panel*>(w);
	}

	const Panel* Widget::getParentPanel() const
	{
		Widget* w = mParentWidget;
		while((w != NULL) && (w->getWidgetType() != TYPE_PANEL))
			w = w->getParentWidget();

		return dynamic_cast<const Panel*>(w);
	}

	Sheet* Widget::getParentSheet()
	{
		Widget* w = mParentWidget;
		while((w != NULL) && (w->getWidgetType() != TYPE_SHEET))
			w = w->getParentWidget();

		return dynamic_cast<Sheet*>(w);
	}

	const Sheet* Widget::getParentSheet() const
	{
		Widget* w = mParentWidget;
		while((w != NULL) && (w->getWidgetType() != TYPE_SHEET))
			w = w->getParentWidget();

		return dynamic_cast<const Sheet*>(w);
	}

	Widget* Widget::getParentWidget()
	{
		return mParentWidget;
	}

	const Widget* Widget::getParentWidget() const
	{
		return mParentWidget;
	}

	Window* Widget::getParentWindow()
	{
		Widget* w = mParentWidget;
		while((w != NULL) && (w->getWidgetType() != TYPE_WINDOW))
			w = w->getParentWidget();

		return dynamic_cast<Window*>(w);
	}

	const Window* Widget::getParentWindow() const
	{
		Widget* w = mParentWidget;
		while((w != NULL) && (w->getWidgetType() != TYPE_WINDOW))
			w = w->getParentWidget();

		return dynamic_cast<const Window*>(w);
	}

	bool Widget::getPropogateEventFiring(Event e) const
	{
		return mPropogateEventFiring[e];
	}

	Quad* Widget::getQuad()
	{
		return mQuad;
	}

	const Quad* Widget::getQuad() const
	{
		return mQuad;
	}

	QuadContainer* Widget::getQuadContainer()
	{
		return mQuadContainer;
	}

	const QuadContainer* Widget::getQuadContainer() const
	{
		return mQuadContainer;
	}

	Quad::Layer Widget::getQuadLayer()
	{
		return mQuadLayer;
	}

	const Quad::Layer Widget::getQuadLayer() const
	{
		return mQuadLayer;
	}

	Point Widget::getScreenPosition() const
	{
		if(mParentWidget == NULL)
			return Point::ZERO;

		return mParentWidget->getScreenPosition() + mPosition;
	}

	bool Widget::getScrollPaneAccessible() const
	{
		return mScrollPaneAccessible;
	}

	std::string Widget::getSkinComponent() const
	{
		return mSkinComponent;
	}

	bool Widget::getShowWithParent() const
	{
		return mShowWithParent;
	}

	std::string Widget::getSkin() const
	{
		return mSkinName;
	}

	Rect Widget::getClippedDimensions()
	{
		if( (mParentWidget == NULL) || (mQuad->getClipMode() == Quad::CLIPMODE_NONE) )
			return Rect(getActualPosition(),mSize);

		Rect parentClippedDimensions = mParentWidget->getClippedDimensions();

		Rect myActualDimensions = getActualDimensions();

		return parentClippedDimensions.getIntersection(myActualDimensions);
	}

	Rect Widget::getDimensions() const
	{
		return Rect(mPosition,mSize);
	}

	std::string Widget::getFontName() const
	{
		return mFontName;
	}

	GUIManager* Widget::getGUIManager()
	{
		return mGUIManager;
	}

	const GUIManager* Widget::getGUIManager() const
	{
		return mGUIManager;
	}

	bool Widget::getInheritOpacity() const
	{
		return mInheritOpacity;
	}

	int Widget::getNumberOfHandlers(Event e) const
	{
		return static_cast<int>(mUserEventHandlers[e].size());
	}

	float Widget::getOpacity() const
	{
		return mOpacity;
	}

	Point Widget::getPosition() const
	{
		return mPosition;
	}

	Point Widget::getScrollOffset() const
	{
		if( mParentWidget == NULL )
			return mScrollOffset;

		return mParentWidget->getScrollOffset() + mScrollOffset;
	}

	Size Widget::getSize() const
	{
		return mSize;
	}

	Widget* Widget::getTargetWidget(const Point& pixelPosition)
	{
		if( !mQuad->visible() || !mEnabled )
			return NULL;

		Widget* w = NULL;
		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if((w = (*it)->getTargetWidget(pixelPosition)) != NULL)
				return w;
		}

		// If position is not inside this widget, it can't be inside a child widget. (except menus, which are handled differently)
		if( !isPointWithinBounds(pixelPosition) ) 
			return NULL;

		// Iterate through Child Widgets and record the highest offset menu and child layer widget.
		Widget* highestMenuLayerWidget = NULL;
		int highestMenuOffset = -1;

		Widget* highestChildLayerWidget = NULL;
		int highestChildOffset = -1;

		WidgetArray::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getQuadLayer() == Quad::LAYER_CHILD )
			{
				Widget* temp = (*it)->getTargetWidget(pixelPosition);
				if( (temp != NULL) && (temp->getOffset() > highestChildOffset) )
				{
					highestChildOffset = temp->getOffset();
					highestChildLayerWidget = temp;
				}
			}
			else // LAYER_MENU
			{
				Widget* temp = (*it)->getTargetWidget(pixelPosition);
				if( (temp != NULL) && (temp->getOffset() > highestMenuOffset) )
				{
					highestMenuOffset = temp->getOffset();
					highestMenuLayerWidget = temp;
				}
			}
		}

		if(highestMenuLayerWidget != NULL)
			return highestMenuLayerWidget;
		if(highestChildLayerWidget != NULL)
			return highestChildLayerWidget;

		// If we made it here, we are inside this Widget's bounds, but not over any non-transparent child widget areas.

		if( !overTransparentPixel(pixelPosition) )
			return this;
		else // We're over a transparent pixel
			return NULL;
	}

	const Widget* Widget::getTargetWidget(const Point& pixelPosition) const
	{
		if( !mQuad->visible() || !mEnabled )
			return NULL;

		Widget* w = NULL;

		for(WidgetArray::const_iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if((w = (*it)->getTargetWidget(pixelPosition)) != NULL)
				return w;
		}

		// If position is not inside this widget, it can't be inside a child widget. (except menus, which are handled differently)
		if( !isPointWithinBounds(pixelPosition) )
			return NULL;

		// Iterate through Child Widgets and record the highest offset menu and child layer widget.
		Widget* highestMenuLayerWidget = NULL;
		int highestMenuOffset = -1;

		Widget* highestChildLayerWidget = NULL;
		int highestChildOffset = -1;

		WidgetArray::const_iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getQuadLayer() == Quad::LAYER_CHILD )
			{
				Widget* temp = (*it)->getTargetWidget(pixelPosition);
				if( (temp != NULL) && (temp->getOffset() > highestChildOffset) )
				{
					highestChildOffset = temp->getOffset();
					highestChildLayerWidget = temp;
				}
			}
			else // LAYER_MENU
			{
				Widget* temp = (*it)->getTargetWidget(pixelPosition);
				if( (temp != NULL) && (temp->getOffset() > highestMenuOffset) )
				{
					highestMenuOffset = temp->getOffset();
					highestMenuLayerWidget = temp;
				}
			}
		}

		if(highestMenuLayerWidget != NULL)
			return highestMenuLayerWidget;
		if(highestChildLayerWidget != NULL)
			return highestChildLayerWidget;

		// If we made it here, we are inside this Widget's bounds, but not over any non-transparent child widget areas.

		if( !overTransparentPixel(pixelPosition) )
			return this;
		else // We're over a transparent pixel
			return NULL;
	}

	bool Widget::getUseTransparencyPicking() const
	{
		return mUseTransparencyPicking;
	}

	Widget::Type Widget::getWidgetType() const
	{
		return mWidgetType;
	}

	Widget::VerticalAnchor Widget::getVerticalAnchor() const
	{
		return mVerticalAnchor;
	}

	float Widget::getWidth() const
	{
		return mSize.width;
	}

	float Widget::getXPosition() const
	{
		return mPosition.x;
	}

	float Widget::getYPosition() const
	{
		return mPosition.y;
	}

	bool Widget::hasMouseButtonHandlers() const
	{
		return !(mUserEventHandlers[EVENT_MOUSE_BUTTON_DOWN].empty() &&
				 mUserEventHandlers[EVENT_MOUSE_BUTTON_UP].empty() &&
				 mUserEventHandlers[EVENT_MOUSE_CLICK].empty() &&
				 mUserEventHandlers[EVENT_MOUSE_CLICK_DOUBLE].empty() &&
				 mUserEventHandlers[EVENT_MOUSE_CLICK_TRIPLE].empty());
	}

	void Widget::hide()
	{
		mGrabbed = false;

		bool currentlyVisible = mVisible;
		mVisible = false;

		for( QuadArray::iterator it = mQuads.begin(); it != mQuads.end(); ++it )
			(*it)->setVisible(false);

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it )
		{
			if((*it)->getHideWithParent())
				(*it)->hide();
		}

		// hide children
		WidgetArray::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if((*it)->getHideWithParent())
				(*it)->hide();
		}

		// if widget is the active widget at time of being hidden, set sheet to active widget.
		if(mGUIManager->getActiveWidget() == this)
			mGUIManager->setActiveWidget(mGUIManager->getActiveSheet());
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

	void Widget::hideSkin()
	{
		mHideSkin = true;

		mQuad->setMaterial("");

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it)->hideSkin();
	}

	bool Widget::isPointWithinBounds(const Point& pixelPosition) const
	{
		if(!mQuad->visible())
			return false;

		if(!mQuad->isPointWithinBounds(pixelPosition))
			return false;

		return true;
	}

	bool Widget::isVisible() const
	{
		return mVisible;
	}

	bool Widget::isChild(const Widget* w) const
	{
		WidgetArray::const_iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( w == (*it) || (*it)->isChild(w) )
				return true;
		}

		return false;
	}

	void Widget::move(const float& pixelX, const float& pixelY)
	{
		if(!mMovingEnabled)
			return;

		setPosition(mPosition.x + pixelX,mPosition.y + pixelY);
	}

	void Widget::move(const Point& pixelOffset)
	{
		move(pixelOffset.x,pixelOffset.y);
	}

	void Widget::moveX(float pixelX)
	{
		if(!mMovingEnabled)
			return;

		setXPosition(mPosition.x + pixelX);
	}

	void Widget::moveY(float pixelY)
	{
		if(!mMovingEnabled)
			return;

		setYPosition(mPosition.y + pixelY);
	}

	bool Widget::fireEvent(Event e, EventArgs& args)
	{
		if(mUserEventHandlers[e].empty() && !mPropogateEventFiring[e])
			return false;

		if(e == EVENT_MOUSE_ENTER)
		{
			if(mEntered)
				return false;
			else
				mEntered = true;
		}
		else if(e == EVENT_MOUSE_LEAVE)
		{
			if(!mEntered)
				return false;
			else
				mEntered = false;
		}

		args.eventType = e;

		// Notify any listeners
		EventHandlerArray::iterator it;
		for( it = mEventListeners.begin(); it != mEventListeners.end(); ++it )
			(*it)->execute(args);

		// Execute registered handlers
		EventHandlerArray* userEventHandlers = &(mUserEventHandlers[e]);
		for( it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mPropogateEventFiring[e] && (mParentWidget != NULL))
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
		WidgetArray::iterator it;

		for( it = mComponents.begin(); it != mComponents.end(); ++it )
			(*it)->redraw();

		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->redraw();
	}

	void Widget::onSizeChanged(const EventArgs& args)
	{
		WidgetArray::iterator it;

		for( it = mComponents.begin(); it != mComponents.end(); ++it )
			(*it)->_applyAnchors();

		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->_applyAnchors();
	}

	bool Widget::overTransparentPixel(const Point& mousePixelPosition) const
	{
		if(!mUseTransparencyPicking || (mSkinSet == NULL))
			return false;

		Point pt = mousePixelPosition - (getScreenPosition() + getScrollOffset());

		// Scale pt to skinset size
		pt.x *= mSkinSet->getImageWidth(mSkinName + mSkinComponent + mSkinSet->getImageExtension()) / mSize.width;
		pt.y *= mSkinSet->getImageHeight(mSkinName + mSkinComponent + mSkinSet->getImageExtension()) / mSize.height;

		return mSkinSet->overTransparentPixel(mSkinComponent,pt.x,pt.y);
	}

	void Widget::redraw()
	{
		mQuad->setDimensions(Rect(getScreenPosition() + getScrollOffset(),mSize));
		mQuad->setOpacity(getActualOpacity());

		WidgetArray::iterator it;

		for( it = mComponents.begin(); it != mComponents.end(); ++it )
			(*it)->redraw();

		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->redraw();
	}

	void Widget::removeChild(Widget* w)
	{
		removeChild(w->getInstanceName());
	}

	void Widget::removeChild(const std::string& widgetName)
	{
		WidgetArray::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if((*it)->getInstanceName() == widgetName)
			{
				Widget* w = (*it);

				(*it)->setQuadContainer(NULL);
				(*it)->setParent(NULL);
				mChildWidgets.erase(it);

				WidgetEventArgs args(w);
				fireEvent(EVENT_CHILD_REMOVED,args);

				return;
			}
		}
	}

	void Widget::removeAndDestroyAllChildWidgets()
	{
		WidgetArray::iterator it;
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

	void Widget::removeAndDestroyChild(Widget* w)
	{
		if((w == NULL) || (mGUIManager == NULL) || (w->getParentWidget() != this))
			return;

		removeChild(w);
		mGUIManager->_destroyWidget(w);

		WidgetEventArgs args(w);
		fireEvent(EVENT_CHILD_DESTROYED,args);
	}

	void Widget::removeAndDestroyChild(const std::string& widgetName)
	{
		removeAndDestroyChild(getChildWidget(widgetName));
	}

	bool Widget::resizingAllowed()
	{
		return mCanResize;
	}

	void Widget::timeElapsed(const float time)
	{
		if(!mEnabled)
			return;
	}

	void Widget::setDimensions(const Rect& pixelDimensions)
	{
		// Its important to set the size before the position, as this may affect scrollbars.
		setSize(pixelDimensions.width,pixelDimensions.height);
		setPosition(pixelDimensions.x,pixelDimensions.y);
	}

	void Widget::setDraggingWidget(Widget* w)
	{
		mWidgetToDrag = w;
	}

	void Widget::setFont(const std::string& fontScriptName, bool recursive)
	{
		if(fontScriptName == "")
			return;

		mFontName = fontScriptName;

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it )
			(*it)->setFont(fontScriptName,true);

		if(recursive)
		{
			for(WidgetArray::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
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

	void Widget::setHeight(float pixelHeight)
	{
		mSize.height = pixelHeight;

		// update vertical anchor
		if(mParentWidget != NULL)
			mPixelsFromParentBottom = mParentWidget->getHeight() - (mPosition.y + mSize.height);

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

	void Widget::setInheritOpacity(bool inherit)
	{
		mInheritOpacity = inherit;

		redraw();
	}

	void Widget::setInheritQuadLayer(bool inherit)
	{
		mInheritQuadLayer = inherit;
	}

	void Widget::setMovingEnabled(bool enable)
	{
		mMovingEnabled = enable;
	}

	void Widget::setOffset(int offset)
	{
		int delta = offset - mOffset;
		mOffset = offset;

		for(QuadArray::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
			(*it)->setOffset((*it)->getOffset() + delta);

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it )
			(*it)->setOffset((*it)->getOffset() + delta);

		for(WidgetArray::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->setOffset((*it)->getOffset() + delta);
	}

	void Widget::setOpacity(float opacity)
	{
		mOpacity = opacity;

		redraw();
	}

	void Widget::setPosition(const float& pixelX, const float& pixelY)
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

	void Widget::setPropagateEventFiring(Event e, bool propogate)
	{
		mPropogateEventFiring[e] = propogate;
	}

	void Widget::setScreenPosition(const float& pixelX, const float& pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		setPosition(Point(pixelX,pixelY) - mParentWidget->getScreenPosition());
	}

	void Widget::setScreenXPosition(const float& pixelX)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		setXPosition(pixelX - mParentWidget->getScreenPosition().x);
	}

	void Widget::setScreenYPosition(const float& pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		setYPosition(pixelY - mParentWidget->getScreenPosition().y);
	}

	void Widget::setScrollPaneAccessible(bool accessible)
	{
		mScrollPaneAccessible = accessible;
	}

	void Widget::setSkin(const std::string& skinName, bool recursive)
	{
		mSkinSet = SkinSetManager::getSingleton().getSkinSet(skinName);
		if(mSkinSet == NULL)
			throw Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND,"Skin \"" + skinName + "\" does not exist!  Did you forget to load it using the SkinSetManager?","Widget::setSkin");

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it)->setSkin(skinName,true);

		if(recursive)
		{
			for(WidgetArray::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
				(*it)->setSkin(skinName,recursive);
		}

		// Update reference to skin
		mSkinName = skinName;
		std::string textureName = mSkinName + mSkinComponent + mSkinSet->getImageExtension();

		if(!mSkinSet->containsImage(textureName))
			return;

		mQuad->setMaterial(mSkinSet->getMaterialName());
		mQuad->setTextureCoordinates(mSkinSet->getTextureCoordinates(textureName));

		if(mUseBorders)
			_createBorders();
		else
			_destroyBorders();

		if(mHideSkin)
			hideSkin();
	}

	void Widget::setSize(const float& pixelWidth, const float& pixelHeight)
	{
		mSize.width = pixelWidth;
		mSize.height = pixelHeight;

		// update anchor
		_deriveAnchorValues();

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

		for(QuadArray::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
		{
			if((*it)->getInheritLayer())
				(*it)->setLayer(mQuadLayer);
		}

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it )
		{
			if( (*it)->getInheritQuadLayer() )
				(*it)->setQuadLayer(mQuadLayer);
		}

		for(WidgetArray::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
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

	void Widget::setBorderThickness(float thickness)
	{
		if(mUseBorders)
		{
			for(std::vector<Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			{
				if((*it)->getWidgetType() == TYPE_BORDER)
					dynamic_cast<Border*>(*it)->setThickness(thickness);
			}
		}
	}

	void Widget::setBorderOverlap(float overlap)
	{
		if(mUseBorders)
		{
			for(std::vector<Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			{
				if((*it)->getWidgetType() == TYPE_BORDER)
					dynamic_cast<Border*>(*it)->setOverlap(overlap);
			}
		}
	}

	void Widget::setUseTransparencyPicking(bool use, bool recursive)
	{
		mUseTransparencyPicking = use;

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it )
		{
			(*it)->setUseTransparencyPicking(use,true);
		}

		if(recursive)
		{
			for(WidgetArray::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
			{
				(*it)->setUseTransparencyPicking(use,recursive);
			}
		}
	}

	void Widget::setVerticalAnchor(Widget::VerticalAnchor a)
	{
		mVerticalAnchor = a;
	}

	void Widget::setWidth(float pixelWidth)
	{
		mSize.width = pixelWidth;

		// update horizontal anchor
		if(mParentWidget != NULL)
			mPixelsFromParentRight = mParentWidget->getWidth() - (mPosition.x + mSize.width);

		WidgetEventArgs args(this);
		fireEvent(EVENT_SIZE_CHANGED,args);

		mQuad->setWidth(mSize.width);
	}

	void Widget::setXPosition(float pixelX)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mPosition.x = pixelX;

		// update horizontal anchor
		if(mParentWidget != NULL)
			mPixelsFromParentRight = mParentWidget->getWidth() - (mPosition.x + mSize.width);

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);

		mQuad->setXPosition(getScreenPosition().x + getScrollOffset().x);
	}

	void Widget::setYPosition(float pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mPosition.y = pixelY;

		// update vertical anchor
		if(mParentWidget != NULL)
			mPixelsFromParentBottom = mParentWidget->getHeight() - (mPosition.y + mSize.height);

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);

		mQuad->setYPosition(getScreenPosition().y + getScrollOffset().y);
	}

	void Widget::show()
	{
		bool currentlyVisible = mVisible;
		mVisible = true;
		for( QuadArray::iterator it = mQuads.begin(); it != mQuads.end(); ++it )
		{
			if((*it)->getShowWithOwner())
				(*it)->setVisible(true);
		}

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it )
		{
			if((*it)->getShowWithParent())
				(*it)->show();
		}

		// show children, except for Windows and lists of MenuList or ComboBox Widget.
		WidgetArray::iterator it;
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

	void Widget::showSkin()
	{
		mHideSkin = false;
		setSkin(mSkinName);
	}

	void Widget::setGUIManager(GUIManager* gm)
	{
		mGUIManager = gm;

		for(QuadArray::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
			(*it)->setGUIManager(mGUIManager);

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it)->setGUIManager(mGUIManager);

		WidgetArray::iterator it;
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

			// set the correct offset
			setOffset(mParentWidget->getOffset() + 1);
			// calculated properties
			_deriveAnchorValues();
			// inheritted properties
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

		for(QuadArray::iterator it = mQuads.begin(); it != mQuads.end(); ++it)
			(*it)->_notifyQuadContainer(mQuadContainer);

		for(WidgetArray::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it)->setQuadContainer(mQuadContainer);

		for( WidgetArray::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->setQuadContainer(mQuadContainer);
	}

	void Widget::setSkinComponent(const std::string& skinComponent)
	{
		mSkinComponent = skinComponent;

		// update widget appearance
		if(mSkinName != "")
			setSkin(mSkinName);
	}

	void Widget::unlockTexture()
	{
		mTextureLocked = false;
	}
}
