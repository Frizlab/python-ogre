#include "QuickGUIManager.h"
#include "QuickGUISerialReader.h"
#include "QuickGUIRoot.h"

namespace QuickGUI
{
	GUIManagerDesc::GUIManagerDesc()
	{
		name = "";
		viewport = NULL;
		sceneManager = NULL;
		scrollLastClicked = false;
		queueID = Ogre::RENDER_QUEUE_OVERLAY;
		determineClickEvents = true;
		clickTime = 90;
		doubleClickTime = 400;
		tripleClickTime = 400;

		// by default, we support codepoints 9, and 32-166.
		supportedCodePoints.insert(9);
		for(Ogre::UTFString::code_point i = 32; i < 167; ++i)
			supportedCodePoints.insert(i);
	}

	GUIManager::GUIManager(GUIManagerDesc& d) :
		mDraggingWidget(false),
		mActiveSheet(0),
		mButtonMask(0),
		mWidgetUnderMouseCursor(0),
		mLastClickedWidget(0),
		mDownOnBorder(false),
		mKeyModifiers(0),
		mOverResizableBorder(false),
		mPreviousBorder(BORDER_NONE),
		mResizableBorder(BORDER_NONE),
		mSheetCounter(0),
		mWidgetCounter(0)
	{
		mGUIManagerDesc.name = d.name;

		mBrush = Brush::getSingletonPtr();

		// If the scenemanager argument is NULL, try to use the first available scene manager
		if(d.sceneManager == NULL)
		{
			Ogre::SceneManagerEnumerator::SceneManagerIterator it = Ogre::Root::getSingleton().getSceneManagerIterator();
			if(it.hasMoreElements())
				setSceneManager(it.getNext());
		}
		else
			setSceneManager(d.sceneManager);

		// If the viewport argument is NULL, use the first available viewport, from the scene manager chosen above.
		if(d.viewport == NULL)
		{
			if(mGUIManagerDesc.sceneManager != NULL)
			{
				Ogre::SceneManager::CameraIterator it = mGUIManagerDesc.sceneManager->getCameraIterator();
				if(it.hasMoreElements())
					setViewport(it.getNext()->getViewport());
			}
		}
		else
			setViewport(d.viewport);

		setSupportedCodePoints(d.supportedCodePoints);

		// Create Default Sheet
		{
			SheetDesc d;
			d.name = "DefaultSheet";
			d.guiManager = this;
			d.dimensions.size = Size(mGUIManagerDesc.viewport->getActualWidth(),mGUIManagerDesc.viewport->getActualHeight());
			
			createSheet(d);
		}

		d.mouseCursorDesc.guiManager = this;
		mMouseCursor = new MouseCursor(d.mouseCursorDesc);
		_clearMouseTrackingData();

		mTimer = new Ogre::Timer();
		
		TimerDesc td;
		td.repeat = false;
		td.timePeriod = 3;
		mHoverTimer = TimerManager::getSingleton().createTimer(td);
	}

	GUIManager::~GUIManager()
	{
		delete mTimer;
		TimerManager::getSingleton().destroyTimer(mHoverTimer);

		for(std::vector<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
			delete (*it);
	}

	void GUIManager::_clearMouseTrackingData()
	{
		for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
		{
			mMouseButtonDown[i] = NULL;
		}
	}

	Sheet* GUIManager::createSheet(SheetDesc& d)
	{
		if(d.name == "")
		{
			d.name = "Sheet" + Ogre::StringConverter::toString(mSheetCounter);
			++mSheetCounter;
		}
		else
		{
			for(std::vector<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
			{
				if((*it)->getName() == d.name)
					throw Exception(Exception::ERR_DUPLICATE_ITEM,"A sheet with name \"" + d.name + "\" already exists!","GUIManager::createSheet");
			}
		}

		Root::getSingleton()._setActiveGUIManager(this);

		Ogre::String cName = d.getWidgetClass();

		if("Sheet" != d.getWidgetClass())
			throw Exception(Exception::ERR_INVALID_DESC,"Desc object for Widget of class \"" + d.getWidgetClass() + "\" does not match Widget of class \"Sheet\"!","GUIManager::createSheet");

		Sheet* s = dynamic_cast<Sheet*>(WidgetFactoryManager::getSingleton().createWidget("Sheet",d.name));
		s->_initialize(&d);

		mSheets.push_back(s);

		if(static_cast<int>(mSheets.size()) == 1)
			setActiveSheet(s);

		return s;
	}

	Widget* GUIManager::createWidget(const Ogre::String& className, WidgetDesc& d)
	{
		Ogre::String temp = d.name;

		if(d.name == "")
		{
			d.name = className + Ogre::StringConverter::toString(mWidgetCounter);
			++mWidgetCounter;
		}
		else if(mActiveSheet->findWidget(d.name) != NULL)
			throw Exception(Exception::ERR_DUPLICATE_ITEM,"A widget with name \"" + d.name + "\" already exists in Sheet \"" + mActiveSheet->getName() + "\"!","GUIManager::createWidget");

		Root::getSingleton()._setActiveGUIManager(this);

		Ogre::String cName = d.getWidgetClass();

		if(className != d.getWidgetClass())
			throw Exception(Exception::ERR_INVALID_DESC,"Desc object for Widget of class \"" + d.getWidgetClass() + "\" does not match Widget of class \"" + className + "\"!","GUIManager::createWidget");

		Widget* newWidget = WidgetFactoryManager::getSingleton().createWidget(className,d.name);
		newWidget->_initialize(&d);

		// Restore the name, in case it was changed.  This Desc could be used multiple times, so this is important!
		d.name = temp;

		return newWidget;
	}

	void GUIManager::destroySheet(Sheet* s)
	{
		if(s == mActiveSheet)
		{
			mActiveSheet->setKeyboardListener(NULL);
			mWidgetUnderMouseCursor = NULL;
			mLastClickedWidget = NULL;
			_clearMouseTrackingData();
			mActiveSheet = NULL;
		}

		for(std::vector<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
		{
			if((*it) == s)
			{
				mSheets.erase(it);
				break;
			}
		}

		delete s;
	}

	void GUIManager::destroyWidget(Widget* w)
	{
		if(w->getClass() == "Sheet")
		{
			destroySheet(dynamic_cast<Sheet*>(w));
			return;
		}

		if(std::find(mFreeList.begin(),mFreeList.end(),w) != mFreeList.end())
			return;

		if((mActiveSheet->getKeyboardListener() != NULL) && (w->findWidget(mActiveSheet->getKeyboardListener()->getName()) != NULL))
			mActiveSheet->setKeyboardListener(NULL);

		mFreeList.push_back(w);
	}

	void GUIManager::draw()
	{
		mBrush->prepareToDraw();

		mBrush->updateSceneManager(mGUIManagerDesc.sceneManager);
		mBrush->updateViewport(mGUIManagerDesc.viewport);
		mBrush->setRenderTarget(mGUIManagerDesc.viewport);

		mActiveSheet->draw();

		mMouseCursor->draw();

		mBrush->restore();
	}

	Sheet* GUIManager::getActiveSheet()
	{
		return mActiveSheet;
	}

	Widget* GUIManager::getLastClickedWidget()
	{
		return mLastClickedWidget;
	}

	MouseCursor* GUIManager::getMouseCursor()
	{
		return mMouseCursor;
	}

	Ogre::String GUIManager::getName()
	{
		return mGUIManagerDesc.name;
	}

	bool GUIManager::getScrollLastClicked()
	{
		return mGUIManagerDesc.scrollLastClicked;
	}

	Sheet* GUIManager::getSheet(const Ogre::String& name)
	{
		for(std::vector<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
		{
			if((*it)->getName() == name)
				return (*it);
		}

		throw Exception(Exception::ERR_ITEM_NOT_FOUND,"Sheet \"" + name + "\" does not exist!","GUIManager::getSheet");
	}

	Widget* GUIManager::getWidgetUnderMouseCursor()
	{
		return mWidgetUnderMouseCursor;
	}

	Ogre::Viewport* GUIManager::getViewport()
	{
		return mGUIManagerDesc.viewport;
	}

	bool GUIManager::hasSheet(const Ogre::String& name)
	{
		for(std::vector<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
		{
			if((*it)->getName() == name)
				return true;
		}

		return false;
	}

	bool GUIManager::injectChar(Ogre::UTFString::unicode_char c)
	{
		if(!isSupportedCodePoint(c))
			return false;

		Widget* w = mActiveSheet->getKeyboardListener();

		if((w == NULL) || !(w->getEnabled()))
			return false;

		KeyEventArgs args(w);
		args.keyModifiers = mKeyModifiers;
		args.keyMask = mButtonMask;
		args.codepoint = c;

		return w->fireWidgetEvent(WIDGET_EVENT_CHARACTER_KEY,args);
	}

	bool GUIManager::injectKeyDown(const KeyCode& kc)
	{
		// Turn on modifier
		if( (kc == KC_LCONTROL) || (kc == KC_RCONTROL) )
			mKeyModifiers |= CTRL;
		else if( (kc == KC_LSHIFT) || (kc == KC_RSHIFT) )
			mKeyModifiers |= SHIFT;
		else if( (kc == KC_LMENU) || (kc == KC_RMENU) )
			mKeyModifiers |= ALT;

		Widget* w = mActiveSheet->getKeyboardListener();

		if((w == NULL) || !(w->getEnabled()))
			return false;

		KeyEventArgs args(w);
		args.scancode = kc;
		args.keyMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;

		return w->fireWidgetEvent(WIDGET_EVENT_KEY_DOWN,args);
	}

	bool GUIManager::injectKeyUp(const KeyCode& kc)
	{
		//Turn off modifier
		if( (kc == KC_LCONTROL) || (kc == KC_RCONTROL) )
			mKeyModifiers &= ~CTRL;
		else if( (kc == KC_LSHIFT) || (kc == KC_RSHIFT) )
			mKeyModifiers &= ~SHIFT;
		else if( (kc == KC_LMENU) || (kc == KC_RMENU) )
			mKeyModifiers &= ~ALT;

		Widget* w = mActiveSheet->getKeyboardListener();

		if((w == NULL) || !(w->getEnabled()))
			return false;

		KeyEventArgs args(w);
		args.scancode = kc;
		args.keyModifiers = mKeyModifiers;

		return w->fireWidgetEvent(WIDGET_EVENT_KEY_UP,args);
	}

	// TODO: mDownOnBorder integration
	bool GUIManager::injectMouseButtonDown(const MouseButtonID& button)
	{
		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		// Users can manually inject single/double/triple click input, or have it detected
		// from mouse button up/down injections.
		if(mGUIManagerDesc.determineClickEvents)
		{
			// Record time of MouseDown, for single/double/triple click determination
			mTimeOfButtonDown[button] = mTimer->getMilliseconds();

			// Check if time since last double click is within triple click time
			if((mTimeOfButtonDown[button] - mTimeOfDoubleClick[button]) <= mGUIManagerDesc.tripleClickTime)
			{
				// We have passed the criteria for a triple click injection, but we have to 
				// make sure the mouse button is going down on the same widget for all clicks.
				if(mMouseButtonDown[button] == mWidgetUnderMouseCursor)
					return injectMouseTripleClick(button);
			}
			// Check if time since last click is within double click time
			if((mTimeOfButtonDown[button] - mTimeOfClick[button]) <= mGUIManagerDesc.doubleClickTime)
			{
				// We have passed the criteria for a double click injection, but we have to 
				// make sure the mouse button is going down on the same widget for both clicks.
				if(mMouseButtonDown[button] == mWidgetUnderMouseCursor)
					return injectMouseDoubleClick(button);
			}
		}

		// If we make it here, a simple mouse button down has occurred.

		// Modify the button mask
		mButtonMask |= (1 << button);

		// Determine if we need to handle closing of Menu's via clicking non menu related widgets.
		if((mLastClickedWidget != NULL) && mLastClickedWidget->isMenuItem())
		{
			if(mWidgetUnderMouseCursor != NULL)
			{
				if(!mWidgetUnderMouseCursor->isMenuItem() || (dynamic_cast<MenuItem*>(mWidgetUnderMouseCursor)->getToolBar()->findWidget(mLastClickedWidget->getName()) == NULL))
				{
					dynamic_cast<MenuItem*>(mLastClickedWidget)->getToolBar()->closeMenus();
				}
			}
		}

		// Record that the mouse button went down on this widget
		mMouseButtonDown[button] = mWidgetUnderMouseCursor;
		mLastClickedWidget = mWidgetUnderMouseCursor;

		// Check if Sheet's keyboard listener (widget receiving keyboard events) needs to be updated.
		if((mLastClickedWidget != NULL) && (mLastClickedWidget->getConsumeKeyboardEvents()))
			mActiveSheet->setKeyboardListener(mLastClickedWidget);

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.position = mMouseCursor->getPosition();
		args.button = button;
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;

		// Create a boolean to track whether or not this injection caused any significant changes.
		bool changesMade = false;

		// Get the Window under the mouse cursor.  If it is not the Sheet, but a child Window,
		// make sure it has focus. (bring to front)
		Window* win = mActiveSheet->findWindowAtPoint(args.position);
		if(win != mActiveSheet)
			// FOCUS_GAINED and FOCUS_LOST events will be fired if appropriate.
			changesMade |= mActiveSheet->focusWindow(win);

		if(mWidgetUnderMouseCursor != NULL)
		{
			mWidgetUnderMouseCursor->setGrabbed(true);
			// Fire EVENT_MOUSE_BUTTON_DOWN event to the widget in focus
			changesMade |= mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_MOUSE_BUTTON_DOWN,args);

			if((win != NULL) && (mWidgetUnderMouseCursor->getBorderSide(mMouseCursor->getPosition() - win->getPosition()) != BORDER_NONE))
				mDownOnBorder = true;
			else
				mDownOnBorder = false;

			// If Widget is grabable, change cursor
			if(mWidgetUnderMouseCursor->getDragable())
				mMouseCursor->_setSkinType("grabbed");
		}

		return changesMade;
	}

	bool GUIManager::injectMouseButtonUp(const MouseButtonID& button)
	{
		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		// Modify the button mask
		mButtonMask &= !(1 << button);

		// If the mouse button goes up and is not over the same widget
		// the mouse button went down on, disregard this injection.
		if( mMouseButtonDown[button] != mWidgetUnderMouseCursor )
			return false;

		// after this point, we know that the user had mouse button down on this widget, and is now doing mouse button up on the same widget.

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.position = mMouseCursor->getPosition();
		args.button = button;
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;

		// Create a boolean to track whether or not this injection caused any significant changes.
		bool changesMade = false;

		if(args.widget != NULL)
		{
			// Check if a widget is currently being dragged.
			if(args.widget->getGrabbed())
			{
				args.widget->setGrabbed(false);

				if(args.widget->getDragable())
				{
					// Set cursor to grabable
					mMouseCursor->_setSkinType("grabable");
				}
				else
				{
					// Restore cursor to default
					mMouseCursor->_setSkinType(mMouseCursor->mDefaultSkinType);
				}

				// We do not want the widget to receive an EVENT_MOUSE_BUTTON_UP event if we are dropping
				// the widget.  Think of Diablo II, dragging a potion to your belt.  If we sent the mouse
				// button up event the potion would be drank as soon as you dropped it into the belt.
				if(args.widget->getBeingDragged())
				{
					return args.widget->fireWidgetEvent(WIDGET_EVENT_DROPPED,args);
				}
			}

			// Fire EVENT_MOUSE_BUTTON_UP event
			changesMade |= args.widget->fireWidgetEvent(WIDGET_EVENT_MOUSE_BUTTON_UP,args);

			// Users can manually inject single/double/triple click input, or have it detected
			// from mouse button up/down injections.
			if(mGUIManagerDesc.determineClickEvents)
			{
				if((mTimer->getMilliseconds() - mTimeOfButtonDown[button]) <= mGUIManagerDesc.clickTime)
				{
					changesMade |= injectMouseClick(button);
				}
			}
		}

		return changesMade;
	}

	bool GUIManager::injectMouseClick(const MouseButtonID& button)
	{
		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		// Modify the button mask
		mButtonMask |= (1 << button);

		// Record the time the click occurred. Useful for generating double clicks.
		mTimeOfClick[button] = mTimer->getMilliseconds();

		// If the mouse button goes up and is not over the same widget
		// the mouse button went down on, disregard this injection.
		if( mMouseButtonDown[button] != mWidgetUnderMouseCursor )
			return false;

		mLastClickedWidget = mWidgetUnderMouseCursor;

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.button = button;
		args.position = mMouseCursor->getPosition();
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;

		if(args.widget != NULL)
			return args.widget->fireWidgetEvent(WIDGET_EVENT_MOUSE_CLICK,args);

		return false;
	}

	bool GUIManager::injectMouseDoubleClick(const MouseButtonID& button)
	{
		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		// Modify the button mask
		mButtonMask |= (1 << button);

		// Record the time the click occurred. Useful for generating triple clicks.
		mTimeOfDoubleClick[button] = mTimer->getMilliseconds();

		// If the mouse button goes up and is not over the same widget
		// the mouse button went down on, disregard this injection.
		if( mMouseButtonDown[button] != mWidgetUnderMouseCursor )
			return false;

		mLastClickedWidget = mWidgetUnderMouseCursor;

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.button = button;
		args.position = mMouseCursor->getPosition();
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;

		if(args.widget != NULL)
			return args.widget->fireWidgetEvent(WIDGET_EVENT_MOUSE_CLICK_DOUBLE,args);

		return false;
	}

	bool GUIManager::injectMouseTripleClick(const MouseButtonID& button)
	{
		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		// Modify the button mask
		mButtonMask |= (1 << button);

		// If the mouse button goes up and is not over the same widget
		// the mouse button went down on, disregard this injection.
		if( mMouseButtonDown[button] != mWidgetUnderMouseCursor )
			return false;

		mLastClickedWidget = mWidgetUnderMouseCursor;

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.button = button;
		args.position = mMouseCursor->getPosition();
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;

		if(args.widget != NULL)
			return args.widget->fireWidgetEvent(WIDGET_EVENT_MOUSE_CLICK_TRIPLE,args);

		return false;
	}

	bool GUIManager::injectMouseMove(const int& xPixelOffset, const int& yPixelOffset)
	{
		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		// Restart the Hover Timer whenever the mouse moves.
		mHoverTimer->start();

		// Get the widget the cursor is over.
		Window* win = mActiveSheet->findWindowAtPoint(mMouseCursor->getPosition());
		Widget* w = win->findWidgetAtPoint(mMouseCursor->getPosition() - win->getPosition());

		// See if we should be dragging or resizing a widget.
		if((mWidgetUnderMouseCursor != NULL) && (mWidgetUnderMouseCursor->getGrabbed()))
		{
			// Check resizing first
			if(mOverResizableBorder && mDownOnBorder)
			{
				// Dragging, which uses move function, works with pixel values (uninfluenced by parent dimensions!)
				mWidgetUnderMouseCursor->resize(mResizableBorder,xPixelOffset,yPixelOffset);

				return true;
			}
			// Check dragging
			else if(mWidgetUnderMouseCursor->getDragable())
			{
				// Dragging, which uses move function, works with pixel values (uninfluenced by parent dimensions!)
				mWidgetUnderMouseCursor->drag(xPixelOffset,yPixelOffset);

				return true;
			}
		}

		// Ignore disabled widgets
		if((w != NULL) && !(w->getEnabled()))
			return false;

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(w);
		args.position = mMouseCursor->getPosition();
		args.moveDelta.x = xPixelOffset;
		args.moveDelta.y = yPixelOffset;
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;

		// Create a boolean to track whether or not this injection caused any significant changes.
		bool changesMade = false;

		// The Widget underneath the mouse cursor has changed.
		if( mWidgetUnderMouseCursor != w )
		{
			if(mWidgetUnderMouseCursor != NULL)
			{
				changesMade |= mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_MOUSE_LEAVE,args);
				// Restore cursor to default
				mMouseCursor->_setSkinType(mMouseCursor->mDefaultSkinType);
			}

			// Update pointer
			mWidgetUnderMouseCursor = w;

			if(mWidgetUnderMouseCursor != NULL)
			{
				changesMade |= mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_MOUSE_ENTER,args);

				// Set the Timer to match this widgets hover time.
				mHoverTimer->setTimePeriod(mWidgetUnderMouseCursor->getHoverTime());
			}
		}

		// Notify the widget the cursor has moved.
		if(mWidgetUnderMouseCursor != NULL)
		{
			changesMade |= mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_MOUSE_MOVE,args);

			// If Widget is grabable, change cursor
			if(mWidgetUnderMouseCursor->getDragable())
				mMouseCursor->_setSkinType("grabable");
			else
				mMouseCursor->_setSkinType(mMouseCursor->mDefaultSkinType);
		}

		// Change cursor if we're over a resizable widget's border, or restore cursor if we have left resizable widget's border
		if((mWidgetUnderMouseCursor != NULL) && (mWidgetUnderMouseCursor->getResizable()))
		{
			mPreviousBorder = mResizableBorder;
			mResizableBorder = mWidgetUnderMouseCursor->getBorderSide(mMouseCursor->getPosition() - win->getPosition());
			mOverResizableBorder = true;

			switch(mResizableBorder)
			{
				case BORDER_LEFT:
				case BORDER_RIGHT:
					mMouseCursor->_setSkinType("hresize"); break;
				case BORDER_TOP:
				case BORDER_BOTTOM:
					mMouseCursor->_setSkinType("vresize"); break;
				case BORDER_TOP_LEFT:
				case BORDER_BOTTOM_RIGHT:
					mMouseCursor->_setSkinType("diag1resize"); break;
				case BORDER_TOP_RIGHT:
				case BORDER_BOTTOM_LEFT:
					mMouseCursor->_setSkinType("diag2resize"); break;
				case BORDER_NONE:
					// Revert cursor if we have just moved off a resizable border
					if(mPreviousBorder != BORDER_NONE)
					{
						mMouseCursor->_setSkinType(mMouseCursor->mDefaultSkinType);
					}
					mOverResizableBorder = false;
					break;
			}
		}
		else
			mOverResizableBorder = false;

		return changesMade;
	}

	bool GUIManager::injectMousePosition(const int& xPixelPosition, const int& yPixelPosition)
	{
		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		Point oldPos = mMouseCursor->getPosition();
		// Update cursor's position as seen on the screen.
		mMouseCursor->setPosition(xPixelPosition,yPixelPosition);

		// Determine the offset and inject a mouse movement input.
		return injectMouseMove(xPixelPosition - oldPos.x,yPixelPosition - oldPos.y);
	}

	bool GUIManager::injectMouseWheelChange(float delta)
	{
		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.position = mMouseCursor->getPosition();
		args.wheelChange = delta;
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;

		if(mGUIManagerDesc.scrollLastClicked)
		{
			Widget* w = mLastClickedWidget->getScrollableContainerWidget();
			if(w != NULL)
				return w->fireWidgetEvent(WIDGET_EVENT_MOUSE_WHEEL,args);
		}
		else
		{
			Widget* w = mWidgetUnderMouseCursor->getScrollableContainerWidget();
			if(w != NULL)
				return w->fireWidgetEvent(WIDGET_EVENT_MOUSE_WHEEL,args);
		}

		return false;
	}

	bool GUIManager::isSupportedCodePoint(Ogre::UTFString::unicode_char c)
	{
		return (mGUIManagerDesc.supportedCodePoints.find(c) != mGUIManagerDesc.supportedCodePoints.end());
	}

	Sheet* GUIManager::loadSheetFromFile(const std::string& fileName)
	{
		// Perform an isolated parsing of the one file
		ScriptReader* sc = ScriptReader::getSingletonPtr();
		sc->begin();
		sc->parseFile(fileName);

		if(sc->getDefinitions().empty())
			throw Exception(Exception::ERR_SERIALIZATION,"No definitions found in file \"" + fileName + "\"!","GUIManager::loadSheetFromFile");

		Ogre::String sheetName = sc->getDefinitions().front()->getID();
		for(std::vector<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
		{
			if((*it)->getName() == sheetName)
				throw Exception(Exception::ERR_DUPLICATE_ITEM,"A sheet with name \"" + sheetName + "\" already exists!","GUIManager::loadSheetFromFile");
		}

		Root::getSingleton()._setActiveGUIManager(this);

		Sheet* newSheet = dynamic_cast<Sheet*>(WidgetFactoryManager::getSingleton().createWidget("Sheet",sheetName));
		mSheets.push_back(newSheet);
		
		// Set the newly created sheet to the active sheet, since GUIManager::createWidget sets the
		// desc::sheet property to the active sheet.
		Sheet* temp = mActiveSheet;
		mActiveSheet = newSheet;

		newSheet->serialize(SerialReader::getSingletonPtr());

		// remove all temporary definitions found from parsing the file
		sc->end();

		// Restore Active Sheet to what it was
		mActiveSheet = temp;

		return newSheet;
	}

	Sheet* GUIManager::reloadSheetFromFile(const Ogre::String& fileName)
	{
		// Perform an isolated parsing of the one file
		ScriptReader* sc = ScriptReader::getSingletonPtr();
		sc->begin();
		sc->parseFile(fileName);

		if(sc->getDefinitions().empty())
			throw Exception(Exception::ERR_SERIALIZATION,"No definitions found in file \"" + fileName + "\"!","GUIManager::reloadSheetFromFile");

		// If sheet already exists, delete it
		Ogre::String sheetName = sc->getDefinitions().front()->getID();
		for(std::vector<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
		{
			if((*it)->getName() == sheetName)
			{
				// Delete sheet.
				destroySheet((*it));
				break;
			}
		}

		Root::getSingleton()._setActiveGUIManager(this);

		Sheet* newSheet = dynamic_cast<Sheet*>(WidgetFactoryManager::getSingleton().createWidget("Sheet",sheetName));
		mSheets.push_back(newSheet);
		
		// Set the newly created sheet to the active sheet, since GUIManager::createWidget sets the
		// desc::sheet property to the active sheet.
		Sheet* temp = mActiveSheet;
		mActiveSheet = newSheet;

		newSheet->serialize(SerialReader::getSingletonPtr());

		// remove all temporary definitions found from parsing the file
		sc->end();

		// Restore Active Sheet to what it was
		mActiveSheet = temp;

		// If we deleted the active sheet, make this sheet the active sheet.
		if(mActiveSheet == NULL)
			mActiveSheet = newSheet;

		Ogre::Viewport* v = mGUIManagerDesc.viewport;
		if(v != NULL)
			mActiveSheet->setSize(Size(v->getActualWidth(),v->getActualHeight()));

		return newSheet;
	}

	void GUIManager::hoverTimerCallback()
	{
		if(mWidgetUnderMouseCursor != NULL)
		{
			WidgetEventArgs args(mWidgetUnderMouseCursor);
			mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_ON_HOVER, args);
		}
	}

	void GUIManager::renderQueueStarted(Ogre::uint8 id, const std::string& invocation, bool &skipThisQueue)
	{
		for(std::vector<Widget*>::iterator it = mFreeList.begin(); it != mFreeList.end(); ++it)
			delete (*it);
		mFreeList.clear();
	}

	void GUIManager::renderQueueEnded(Ogre::uint8 id, const std::string& invocation, bool &repeatThisQueue)
	{
		// Perform rendering of GUI
		if(mGUIManagerDesc.queueID == id)
		{
			draw();
		}
	}

	void GUIManager::setActiveSheet(Sheet* s)
	{
		if(s == NULL)
			throw Exception(Exception::ERR_INVALIDPARAMS,"Sheet is NULL!","GUIManager::setActiveSheet");

		if(std::find(mSheets.begin(),mSheets.end(),s) == mSheets.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Sheet \"" + s->getName() + "\" does not belong to this Manager!","GUIManager::setActiveSheet");

		mActiveSheet = s;
		mWidgetUnderMouseCursor = mActiveSheet;
		mLastClickedWidget = NULL;

		Ogre::Viewport* v = mGUIManagerDesc.viewport;
		if(v != NULL)
			mActiveSheet->setSize(Size(v->getActualWidth(),v->getActualHeight()));
	}

	void GUIManager::setRenderQueueID(Ogre::uint8 id)
	{
		mGUIManagerDesc.queueID = id;
	}

	void GUIManager::setSceneManager(Ogre::SceneManager* sm)
	{
		// remove listener from previous scene manager
		if(mGUIManagerDesc.sceneManager != NULL)
			mGUIManagerDesc.sceneManager->removeRenderQueueListener(this);
		
		// update
		mGUIManagerDesc.sceneManager = sm;

		// add listener to new scene manager
		if(mGUIManagerDesc.sceneManager != NULL)
			mGUIManagerDesc.sceneManager->addRenderQueueListener(this);
	}

	void GUIManager::setScrollLastClicked(bool scroll)
	{
		mGUIManagerDesc.scrollLastClicked = scroll;
	}

	void GUIManager::setSupportedCodePoints(const std::set<Ogre::UTFString::code_point>& list)
	{
		mGUIManagerDesc.supportedCodePoints = list;
	}

	void GUIManager::setViewport(Ogre::Viewport* vp)
	{
		mGUIManagerDesc.viewport = vp;
	}
}
