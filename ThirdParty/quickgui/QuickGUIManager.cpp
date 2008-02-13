#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIManager.h"
#include "QuickGUIEffect.h"
#include "QuickGUIConfigScriptParser.h"

namespace QuickGUI
{
	GUIManager::GUIManager(const std::string& name, Ogre::Viewport* vp, const std::string& defaultSkin) :
		mName(name),
		mViewport(vp),
		mDefaultSkin(defaultSkin),
		mActiveSheet(0),
		mWidgetContainingMouse(0),
		mActiveWidget(0),
		mQueueID(Ogre::RENDER_QUEUE_OVERLAY),
		mMouseCursor(0),
		mSceneManager(0),
		mDraggingWidget(false),
		mDebugString(""),
		mDetermineClickEvents(true),
		mClickTime(100),
		mDoubleClickTime(400),
		mTripleClickTime(400)
	{
		mSkinSetManager = SkinSetManager::getSingletonPtr();

		mWidgetNames.clear();

		for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
		{
			mMouseButtonDown[i] = NULL;
			mTimeOfButtonDown[i] = 0;
			mTimeOfClick[i] = 0;
			mTimeOfDoubleClick[i] = 0;
		}	

		// by default, we support codepoints 9, and 32-166.
		mSupportedCodePoints.insert(9);
		for(Ogre::UTFString::code_point i = 32; i < 167; ++i)
			mSupportedCodePoints.insert(i);

		mTimer = new Ogre::Timer();

		mMouseCursor = new MouseCursor(Size(24,32),mDefaultSkin,this);
		mMouseCursor->setPosition(getViewportWidth()/2.0,getViewportHeight()/2.0);

		mDefaultSheet = createSheet();
		// Initialize all widget tracking pointers.
		mActiveWidget = mWidgetContainingMouse = mActiveSheet = mDefaultSheet;
	}

	GUIManager::~GUIManager()
	{
		delete mTimer;

		mViewport = NULL;

		removeFromRenderQueue();
		clearAll();

		delete mMouseCursor;
		mMouseCursor = NULL;
	}

	void GUIManager::_destroyWidget(Widget* w)
	{
		if( w == NULL )
			return;

		// If widget already queued for destruction, return.
		if(std::find(mFreeList.begin(),mFreeList.end(),w) != mFreeList.end())
			return;

		mFreeList.push_back(w);
	}

	void GUIManager::_menuOpened(Widget* w)
	{
		if(w == NULL)
			return;

		// check if widget already in list.
		if(mOpenMenus.find(w) != mOpenMenus.end())
			return;

		mOpenMenus.insert(w);
	}

	void GUIManager::_menuClosed(Widget* w)
	{
		if(w == NULL)
			return;

		mOpenMenus.erase(mOpenMenus.find(w));
	}

	void GUIManager::_notifyViewportDimensionsChanged()
	{
		if(mViewport == NULL)
			return;

		for(std::list<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it )
			(*it)->setSize(mViewport->getActualWidth(),mViewport->getActualHeight());
	}

	void GUIManager::clearAll()
	{
		mWidgetNames.clear();

		mMouseButtonDown[0] = NULL;
		mMouseButtonDown[1] = NULL;
		mMouseButtonDown[2] = NULL;
		mMouseButtonDown[3] = NULL;
		mMouseButtonDown[4] = NULL;
		mMouseButtonDown[5] = NULL;
		mMouseButtonDown[6] = NULL;
		mMouseButtonDown[7] = NULL;
		mWidgetContainingMouse = NULL;
		mActiveWidget = NULL;

		mTimeListeners.clear();
		mOpenMenus.clear();

		std::list<Effect*>::iterator itEffect = mActiveEffects.begin();
		while (itEffect != mActiveEffects.end())        
		{
			delete *itEffect;
			++itEffect;            
		} 

		std::list<Sheet*>::iterator it;
		for( it = mSheets.begin(); it != mSheets.end(); ++it )
			delete (*it);
		mSheets.clear();

		// This function is called by the destructor, and we don't want to create a sheet at this time.
		if(mViewport != NULL)
		{
			// create default sheet
			mDefaultSheet = createSheet();
			mActiveSheet = mDefaultSheet;

			mActiveWidget = mActiveSheet;
			mWidgetContainingMouse = mActiveSheet;
		}
	}

	Sheet* GUIManager::createSheet()
	{
		return createSheet(generateName(Widget::TYPE_SHEET));
	}

	Sheet* GUIManager::createSheet(const std::string& name)
	{
		if(!isNameUnique(name))
			throw Ogre::Exception(Ogre::Exception::ERR_DUPLICATE_ITEM,"Name \"" + name + "\" is already used!","GUIManager::createSheet");

		notifyNameUsed(name);
		Sheet* newSheet = new Sheet(name,mDefaultSkin,this);
		mSheets.push_back(newSheet);

		return newSheet;
	}

	void GUIManager::destroySheet(const std::string& name)
	{
		// Cannot destroy active sheet!
		if( (name.empty()) || (mActiveSheet->getInstanceName() == name) ) 
			return;

		std::list<Sheet*>::iterator it;
		for( it = mSheets.begin(); it != mSheets.end(); ++it )
		{
			if( (*it)->getInstanceName() == name )
			{
				Sheet* s = (*it);
				it = mSheets.erase(it);
				delete s;
				break;
			}
		}
	}

	void GUIManager::destroySheet(Sheet* sheet)
	{
		destroySheet(sheet->getInstanceName());
	}

	void GUIManager::destroyWidget(Widget* w)
	{
		if( w == NULL )
			return;

		if(w->getWidgetType() == Widget::TYPE_SHEET)
		{
			destroySheet(dynamic_cast<Sheet*>(w));
			return;
		}

		// If widget already queued for destruction, return.
		if(std::find(mFreeList.begin(),mFreeList.end(),w) != mFreeList.end())
			return;

		if(w->getParentWidget() != NULL)
			w->getParentWidget()->removeAndDestroyChild(w);
		else
			mFreeList.push_back(w);
	}

	void GUIManager::destroyWidget(const std::string& widgetName)
	{
		destroyWidget(mActiveSheet->getChildWidget(widgetName));
	}

	Sheet* GUIManager::getActiveSheet()
	{
		return mActiveSheet;
	}

	Widget* GUIManager::getActiveWidget()
	{
		return mActiveWidget;
	}

	std::string GUIManager::getDebugString()
	{
		return mDebugString;
	}

	Sheet* GUIManager::getDefaultSheet()
	{
		return mDefaultSheet;
	}

	bool GUIManager::getDetermineClickEvents()
	{
		return mDetermineClickEvents;
	}

	MouseCursor* GUIManager::getMouseCursor()
	{
		return mMouseCursor;
	}

	Widget* GUIManager::getMouseOverWidget()
	{
		return mWidgetContainingMouse;
	}

	std::string GUIManager::getName()
	{
		return mName;
	}

	Ogre::Viewport* GUIManager::getViewport()
	{
		return mViewport;
	}

	float GUIManager::getViewportWidth()
	{
		return static_cast<float>(mViewport->getActualWidth());
	}

	float GUIManager::getViewportHeight()
	{
		return static_cast<float>(mViewport->getActualHeight());
	}

	Sheet* GUIManager::getSheet(const std::string& name)
	{
		if( name.empty() ) return NULL;

		std::list<Sheet*>::iterator it;
		for( it = mSheets.begin(); it != mSheets.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) 
				return (*it);
		}

		return NULL;
	}

	void GUIManager::addEffect(Effect* e)
	{
		mActiveEffects.push_back(e);
	}

	std::string GUIManager::generateName(Widget::Type t)
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
			case Widget::TYPE_SHEET:				s = "Sheet";			break;
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

	bool GUIManager::injectChar(Ogre::UTFString::unicode_char c)
	{
		if( mSupportedCodePoints.find(c) == mSupportedCodePoints.end() )
			return false;

		KeyEventArgs args(mActiveWidget);
		args.codepoint = c;

		return mActiveWidget->fireEvent(Widget::EVENT_CHARACTER_KEY,args);
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

		KeyEventArgs args(mActiveWidget);
		args.scancode = kc;
		args.keyModifiers = mKeyModifiers;

		return mActiveWidget->fireEvent(Widget::EVENT_KEY_DOWN,args);
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

		KeyEventArgs args(mActiveWidget);
		args.scancode = kc;
		args.keyModifiers = mKeyModifiers;

		return mActiveWidget->fireEvent(Widget::EVENT_KEY_UP,args);
	}

	bool GUIManager::injectMouseButtonDown(const MouseButtonID& button)
	{
		if( !mMouseCursor->isVisible() ) 
			return false;

		mTimeOfButtonDown[button] = mTimer->getMilliseconds();
		if(mDetermineClickEvents)
		{
			if((mTimeOfButtonDown[button] - mTimeOfDoubleClick[button]) <= mTripleClickTime)
			{
				if(mMouseButtonDown[button] == mWidgetContainingMouse)
					return injectMouseTripleClick(button);
			}
			if((mTimeOfButtonDown[button] - mTimeOfClick[button]) <= mDoubleClickTime)
			{
				if(mMouseButtonDown[button] == mWidgetContainingMouse)
					return injectMouseDoubleClick(button);
			}
		}

		// If we make it here, a simple mouse button down has occurred.

		// Setup event args.
		MouseEventArgs args(mWidgetContainingMouse);
		args.position = mMouseCursor->getPosition();
		args.button = button;
		args.keyModifiers = mKeyModifiers;

		// Feature, allowing widgets to be clicked, without transferring focus.  Widget will receive
		// Mouse Button Down Event.  No widgets will gain or lose focus.
		if(!mWidgetContainingMouse->getGainFocusOnClick())
			return mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_BUTTON_DOWN,args);
		
		bool eventHandlerFired = false;

		// See if focus has changed from one widget to another.
		// mActiveWidget is the last widget the user clicked on, ie TextBox, ComboBox, etc.
		if( mActiveWidget != mWidgetContainingMouse )
		{
			if(mActiveWidget->fireEvent(Widget::EVENT_LOSE_FOCUS,args))
				eventHandlerFired = true;

			// Update active widget reference.
			mActiveWidget = mWidgetContainingMouse;
		}

		// Fire Gain Focus event.
		if(mActiveWidget->fireEvent(Widget::EVENT_GAIN_FOCUS,args))
			eventHandlerFired = true;
		mActiveWidget->setGrabbed(true);

		if(mActiveWidget->fireEvent(Widget::EVENT_MOUSE_BUTTON_DOWN,args))
			eventHandlerFired = true;		

		// If the user clicked on a widget that is a part of a window, make sure the window is brought to front.
		Window* w = mActiveWidget->getParentWindow();
		if( w != NULL )
		{
			w->bringToFront();
			eventHandlerFired = true;
		}

		// Record that the mouse button went down on this widget (non-window)
		mMouseButtonDown[button] = mActiveWidget;

		return eventHandlerFired;
	}

	bool GUIManager::injectMouseButtonUp(const MouseButtonID& button)
	{
		if( !mMouseCursor->isVisible() ) 
			return false;

		MouseEventArgs args(mActiveWidget);
		args.position = mMouseCursor->getPosition();
		args.button = button;
		args.keyModifiers = mKeyModifiers;

		// If dragging a widget, fire EVENT_DROPPED event
		if(mDraggingWidget)
		{
			mDraggingWidget = false;
			mActiveWidget->setGrabbed(false);
			return mActiveWidget->fireEvent(Widget::EVENT_DROPPED,args);
		}

		bool eventHandled = false;

		// Feature, allowing widgets to be clicked, without transfering focus.  Widget will receive
		// Mouse Button Up and Click Events, if appropriate.
		if(!mWidgetContainingMouse->getGainFocusOnClick())
		{
			if( mMouseButtonDown[args.button] == mWidgetContainingMouse )
			{
				if(mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_BUTTON_UP,args))
					eventHandled = true;
				if(mDetermineClickEvents)
				{
					if((mTimer->getMilliseconds() - mTimeOfButtonDown[button]) <= mClickTime)
					{
						if(injectMouseClick(button))
							eventHandled = true;
					}
				}
			}
			return eventHandled;
		}

		// If the MouseButton was not pressed on this widget, do not register the button being released on the widget
		if( mWidgetContainingMouse != mActiveWidget )
			return mActiveWidget->fireEvent(Widget::EVENT_LOSE_FOCUS,args);

		// after this point, we know that the user had mouse button down on this widget, and is now doing mouse button up

		if(mActiveWidget->fireEvent(Widget::EVENT_MOUSE_BUTTON_UP,args))
			eventHandled = true;
		mActiveWidget->setGrabbed(false);

		if(mDetermineClickEvents)
		{
			if((mTimer->getMilliseconds() - mTimeOfButtonDown[button]) <= mClickTime)
			{
				if(injectMouseClick(button))
					eventHandled = true;
			}
		}

		return eventHandled;
	}

	bool GUIManager::injectMouseClick(const MouseButtonID& button)
	{
		mTimeOfClick[button] = mTimer->getMilliseconds();

		MouseEventArgs args(mActiveWidget);
		args.position = mMouseCursor->getPosition();
		args.button = button;
		args.keyModifiers = mKeyModifiers;

		return mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_CLICK,args);
	}

	bool GUIManager::injectMouseDoubleClick(const MouseButtonID& button)
	{
		mTimeOfDoubleClick[button] = mTimer->getMilliseconds();

		MouseEventArgs args(mActiveWidget);
		args.position = mMouseCursor->getPosition();
		args.button = button;
		args.keyModifiers = mKeyModifiers;

		return mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_CLICK_DOUBLE,args);
	}

	bool GUIManager::injectMouseTripleClick(const MouseButtonID& button)
	{
		MouseEventArgs args(mActiveWidget);
		args.position = mMouseCursor->getPosition();
		args.button = button;
		args.keyModifiers = mKeyModifiers;

		return mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_CLICK_TRIPLE,args);
	}

	bool GUIManager::injectMouseLeaves(void)
	{
		if( !mMouseCursor->isVisible() ) 
			return false;

		if(mMouseCursor->getHideWhenOffScreen()) 
			mMouseCursor->_hide();

		return true;
	}

	bool GUIManager::injectMouseMove(const int& xPixelOffset, const int& yPixelOffset)
	{
		MouseEventArgs args(0);
		args.position = mMouseCursor->getPosition();
		args.moveDelta.x = xPixelOffset;
		args.moveDelta.y = yPixelOffset;
		args.keyModifiers = mKeyModifiers;

		if( mMouseCursor->mouseOnTopBorder() && yPixelOffset < 0 ) 
			args.moveDelta.y = 0;
		if( mMouseCursor->mouseOnBotBorder() && yPixelOffset > 0 ) 
			args.moveDelta.y = 0;
		if( mMouseCursor->mouseOnLeftBorder() && xPixelOffset < 0 ) 
			args.moveDelta.x = 0;
		if( mMouseCursor->mouseOnRightBorder() && xPixelOffset > 0 ) 
			args.moveDelta.x = 0;

		// Update Mouse Cursor Position
		mMouseCursor->offsetPosition(xPixelOffset,yPixelOffset);

		if(!mMouseCursor->isVisible()) 
			return false;

		// See if we should be dragging a widget.
		if( (mActiveWidget->getGrabbed()) && (mActiveWidget->draggingEnabled()) )
		{
			mDraggingWidget = true;

			// Dragging, which uses move function, works with pixel values (uninfluenced by parent dimensions!)
			mActiveWidget->drag(xPixelOffset,yPixelOffset);

			return true;
		}

		// Now get the widget the cursor is over.
		
		// Check Open Menus first, since they are not within their parent bounds.
		Widget* hitWidget = NULL;
		for(std::set<Widget*>::iterator it = mOpenMenus.begin(); it != mOpenMenus.end(); ++it)
		{
			if( (hitWidget = (*it)->getTargetWidget(args.position)) != NULL )
				break;
		}

		// If no menu's are open, or cursor not over a menu, query the Sheet for the widget the cursor is over.
		if(hitWidget == NULL)
			hitWidget = mActiveSheet->getTargetWidget(args.position);
		
		// NOTE: Widget is only detected if it is enabled.
		args.widget = hitWidget;

		bool eventHandled = false;

		if( mWidgetContainingMouse != hitWidget )
		{
			if(mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_LEAVE,args))
				eventHandled = true;

			mWidgetContainingMouse = hitWidget;
			if(mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_ENTER,args))
				eventHandled = true;
		}
		else // widget containing mouse has not changed.
			eventHandled = mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_MOVE,args);

		return eventHandled;
	}

	bool GUIManager::injectMousePosition(const int& xPixelPosition, const int& yPixelPosition)
	{
		Point pos = mMouseCursor->getPosition();

		return injectMouseMove(xPixelPosition - pos.x,yPixelPosition - pos.y);
	}

	bool GUIManager::injectMouseWheelChange(float delta)
	{
		MouseEventArgs args(mWidgetContainingMouse);
		args.wheelChange = delta;
		args.keyModifiers = mKeyModifiers;

		return mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_WHEEL,args);
	}

	void GUIManager::injectTime(const float time)
	{
		{
			WidgetArray::iterator it;
			for( it = mTimeListeners.begin(); it != mTimeListeners.end(); ++it )
				if ((*it)->isUnderTiming())
					(*it)->timeElapsed(time);
		}

		// Effects.
		{
			/*
			WidgetArray::iterator it;   
			while (itWindow != mWidgets.end())        
			{
				(*itWindow)->setUnderEffect(false);
				++itWindow;
			} 
			*/
		}
		if(!mActiveEffects.empty())
		{
			std::list<Effect*>::iterator itEffect = mActiveEffects.begin();       
			while (itEffect != mActiveEffects.end())        
			{
				if ((*itEffect)->update(time))
				{
					delete *itEffect;
					itEffect = mActiveEffects.erase(itEffect);
				}
				else
				{
					++itEffect;
				}
			}

			injectMouseMove(0,0);
		}
	}

	bool GUIManager::isKeyModifierDown(KeyModifier k)
	{
		return ((mKeyModifiers & k) > 0);
	}

	bool GUIManager::isNameUnique(const std::string& name)
	{
		if(name.empty())
			return false;

		return (mWidgetNames.find(name) == mWidgetNames.end());
	}

	void GUIManager::notifyNameFree(const std::string& name)
	{
		std::set<std::string>::iterator it = mWidgetNames.find(name);
		if( it == mWidgetNames.end() )
			return;

		mWidgetNames.erase(it);
	}

	void GUIManager::notifyNameUsed(const std::string& name)
	{
		if(!isNameUnique(name))
			return;

		mWidgetNames.insert(name);
	}

	void GUIManager::registerTimeListener(Widget* w)
	{
		if(w == NULL)
			return;

		// check if widget already in list.
		WidgetArray::iterator it;
		for( it = mTimeListeners.begin(); it != mTimeListeners.end(); ++it )
		{
			if(w == (*it))
				return;
		}

		mTimeListeners.push_back(w);
	}

	void GUIManager::renderQueueStarted(Ogre::uint8 id, const std::string& invocation, bool& skipThisQueue)
	{
		// Look for widgets queued for deletion.
		if(mFreeList.empty())
			return;

		bool activeWidgetDestroyed = false;
		if( std::find(mFreeList.begin(),mFreeList.end(),mActiveWidget) != mFreeList.end() )
		{
			activeWidgetDestroyed = true;
			mActiveWidget = mActiveSheet;
			mWidgetContainingMouse = mActiveSheet;
		}

		WidgetArray::iterator it;
		for( it = mFreeList.begin(); it != mFreeList.end(); ++it )
			delete (*it);
		mFreeList.clear();

		if(activeWidgetDestroyed)
			injectMouseMove(0,0);
	}

	void GUIManager::renderQueueEnded(Ogre::uint8 id, const std::string& invocation, bool& repeatThisQueue)
	{
		// Perform rendering of GUI
		if(mQueueID == id)
		{
			mActiveSheet->render();
			mMouseCursor->render();
		}
	}

	void GUIManager::removeFromRenderQueue()
	{
		if(mSceneManager != NULL)
			mSceneManager->removeRenderQueueListener(this);
	}

	void GUIManager::setActiveSheet(Sheet* s)
	{
		if( (s == NULL) || (s == mActiveSheet) ) 
			return;

		mActiveSheet = s;

		MouseEventArgs args(mWidgetContainingMouse);
		args.position = mMouseCursor->getPosition();

		// Make sure active widget loses focus.
		mActiveWidget->fireEvent(Widget::EVENT_LOSE_FOCUS,args);
		
		// Make sure mouse over widget has mouse leave event.
		mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_LEAVE,args);

		// Update the active widget
		mActiveWidget = mActiveSheet;
		mWidgetContainingMouse = mActiveSheet;
		injectMouseMove(0,0);
	}

	void GUIManager::setActiveWidget(Widget* w)
	{
		if ( w == NULL )
			return;

		if (!w->enabled())
			return;

		if( w != mActiveWidget ) 
		{
			WidgetEventArgs args(mActiveWidget);
			mActiveWidget->fireEvent(Widget::EVENT_LOSE_FOCUS,args);
		}

		mActiveWidget = w;

		WidgetEventArgs args(mActiveWidget);
		mActiveWidget->fireEvent(Widget::EVENT_GAIN_FOCUS,args);
	}

	void GUIManager::setDebugString(const std::string s)
	{
		mDebugString = s;
	}

	void GUIManager::setDetermineClickEvents(bool determine)
	{
		mDetermineClickEvents = determine;
	}

	void GUIManager::setRenderQueueID(Ogre::uint8 id)
	{
		mQueueID = id;
	}

	void GUIManager::setSceneManager(Ogre::SceneManager* sm)
	{
		// remove listener from previous scene manager
		if(mSceneManager != NULL)
			mSceneManager->removeRenderQueueListener(this);
		// update
		mSceneManager = sm;
		// add listener to new scene manager
		if(mSceneManager != NULL)
			mSceneManager->addRenderQueueListener(this);
	}

	void GUIManager::setSupportedCodePoints(const std::set<Ogre::UTFString::code_point>& list)
	{
		mSupportedCodePoints = list;
	}

	void GUIManager::setViewport(Ogre::Viewport* vp)
	{
		mViewport = vp;
	}

	void GUIManager::unregisterTimeListener(Widget* w)
	{
		if(w == NULL)
			return;

		WidgetArray::iterator it;
		for( it = mTimeListeners.begin(); it != mTimeListeners.end(); ++it )
		{
			if(w == (*it))
			{
				mTimeListeners.erase(it);
				return;
			}
		}
	}
}
