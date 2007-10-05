#include "QuickGUIManager.h"

namespace QuickGUI
{
	GUIManager::GUIManager(Ogre::Viewport* vp) :
		mViewport(vp),
		mActiveSheet(0),
		mWidgetContainingMouse(0),
		mActiveWidget(0),
		mClickTimeout(75),
		mAutoNameSheetCounter(0),
		mQueueID(Ogre::RENDER_QUEUE_OVERLAY),
		mMouseCursor(0),
		mSceneManager(0),
		mDraggingWidget(false),
		mDebugString("")
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

		mMouseCursor = new MouseCursor(Size(30,30),"qgui.pointer.png",this);
		mMouseCursor->setPosition(getViewportWidth()/2.0,getViewportHeight()/2.0);
		
		mDefaultSheet = createSheet();
		// Initialize all widget tracking pointers.
		mActiveWidget = mWidgetContainingMouse = mActiveSheet = mDefaultSheet;

		// by default, we support codepoints 9, and 32-166.
		mSupportedCodePoints.push_back(9);
		for(Ogre::UTFString::code_point i = 32; i < 167; ++i)
			mSupportedCodePoints.push_back(i);

		// load default skin into an SkinSet Texture - must be done before we start rendering.
		loadSkin("qgui");

		_createDefaultTextures();

		mTimer = new Ogre::Timer();
	}

	GUIManager::~GUIManager()
	{
		delete mTimer;

		removeFromRenderQueue();
		clearAll();

		delete mMouseCursor;
		mMouseCursor = NULL;

		// delete imagesets
		std::map<Ogre::String,SkinSet*>::iterator it;
		for( it = mSkinSets.begin(); it != mSkinSets.end(); ++it )
			delete (it->second);
		mSkinSets.clear();
	}

	void GUIManager::_createDefaultTextures()
	{
		Ogre::TextureManager* tm = Ogre::TextureManager::getSingletonPtr();
		// QuickGUI.White - Plain White
		if( !tm->resourceExists("QuickGUI.White") )
		{
			Ogre::TexturePtr tp = 
				tm->createManual("QuickGUI.White",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,Ogre::TEX_TYPE_2D,1,1,1,0,Ogre::PF_A8B8G8R8);

			Ogre::HardwarePixelBufferSharedPtr buf = tp->getBuffer();
			buf->lock(Ogre::HardwareBuffer::HBL_NORMAL);
			const Ogre::PixelBox& pb = buf->getCurrentLock();
			Ogre::uint8* pixelData = static_cast<Ogre::uint8*>(pb.data);

			*pixelData++ = 255;	// Blue
			*pixelData++ = 255; // Green
			*pixelData++ = 255; // Red
			*pixelData++ = 255; // Alpha

			buf->unlock();
		}

		// QuickGUI.TextSelection - Used for text selections.  Has half alpha value.
		if( !tm->resourceExists("QuickGUI.TextSelection") )
		{
			Ogre::TexturePtr tp = 
				tm->createManual("QuickGUI.TextSelection",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,Ogre::TEX_TYPE_2D,1,1,1,0,Ogre::PF_A8B8G8R8);

			Ogre::HardwarePixelBufferSharedPtr buf = tp->getBuffer();
			buf->lock(Ogre::HardwareBuffer::HBL_NORMAL);
			const Ogre::PixelBox& pb = buf->getCurrentLock();
			Ogre::uint8* pixelData = static_cast<Ogre::uint8*>(pb.data);

			*pixelData++ = 255;	// Blue
			*pixelData++ = 255; // Green
			*pixelData++ = 255; // Red
			*pixelData++ = 200; // Alpha

			buf->unlock();
		}
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

		std::list<Sheet*>::iterator it;
		for( it = mSheets.begin(); it != mSheets.end(); ++it )
			delete (*it);
		mSheets.clear();

		// reset counter
		mAutoNameSheetCounter = 0;
		// create default sheet
		mDefaultSheet = createSheet();
		mActiveSheet = mDefaultSheet;

		mActiveWidget = mActiveSheet;
		mWidgetContainingMouse = mActiveSheet;
	}

	Sheet* GUIManager::createSheet()
	{
		return new Sheet("Sheet"+Ogre::StringConverter::toString(mAutoNameSheetCounter++),"",this);
	}

	void GUIManager::destroySheet(const Ogre::String& name)
	{
		// Cannot destroy active sheet!
		if( (name == "") || (mActiveSheet->getInstanceName() == name) ) 
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

		mFreeList.push_back(w);
	}

	void GUIManager::destroyWidget(const Ogre::String& widgetName)
	{
		Widget* w = mActiveSheet->getChildWidget(widgetName);
		if( w != NULL )
			destroyWidget(w);
	}

	Sheet* GUIManager::getActiveSheet()
	{
		return mActiveSheet;
	}

	Ogre::String GUIManager::getDebugString()
	{
		return mDebugString;
	}

	Sheet* GUIManager::getDefaultSheet()
	{
		return mDefaultSheet;
	}

	SkinSet* GUIManager::getSkinImageSet(const Ogre::String& name)
	{
		if(!skinLoaded(name)) return NULL;
		else return mSkinSets[name];
	}

	MouseCursor* GUIManager::getMouseCursor()
	{
		return mMouseCursor;
	}

	Widget* GUIManager::getMouseOverWidget()
	{
		return mWidgetContainingMouse;
	}

	Ogre::Viewport* GUIManager::getViewport()
	{
		return mViewport;
	}

	Ogre::Real GUIManager::getViewportWidth()
	{
		return static_cast<Ogre::Real>(mViewport->getActualWidth());
	}

	Ogre::Real GUIManager::getViewportHeight()
	{
		return static_cast<Ogre::Real>(mViewport->getActualHeight());
	}

	Sheet* GUIManager::getParentSheet(const Ogre::String& name)
	{
		if( name == "" ) return NULL;

		std::list<Sheet*>::iterator it;
		for( it = mSheets.begin(); it != mSheets.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) 
				return (*it);
		}

		return NULL;
	}

	bool GUIManager::embeddedInSkinImageset(const Ogre::String& skinName, const Ogre::String& textureName)
	{
		if(!skinLoaded(skinName)) return false;
		else return mSkinSets[skinName]->containsImage(textureName);
	}

	bool GUIManager::injectChar(Ogre::UTFString::unicode_char c)
	{
		if( std::find(mSupportedCodePoints.begin(),mSupportedCodePoints.end(),c) == mSupportedCodePoints.end() )
			return false;

		KeyEventArgs args(mActiveWidget);
		args.codepoint = c;

		return mActiveWidget->fireEvent(Widget::EVENT_CHARACTER_KEY,args);
	}

	bool GUIManager::injectKeyDown(const KeyCode& kc)
	{
		KeyEventArgs args(mActiveWidget);
		args.scancode = kc;

		return mActiveWidget->fireEvent(Widget::EVENT_KEY_DOWN,args);
	}

	bool GUIManager::injectKeyUp(const KeyCode& kc)
	{
		KeyEventArgs args(mActiveWidget);
		args.scancode = kc;

		return mActiveWidget->fireEvent(Widget::EVENT_KEY_UP,args);
	}
	
	bool GUIManager::injectMouseButtonDown(const MouseButtonID& button)
	{
		if( !mMouseCursor->isVisible() ) 
			return false;

		bool eventHandled = false;

		MouseEventArgs args(mActiveWidget);
		args.position = mMouseCursor->getPosition();
		args.button = button;

		// Feature, allowing widgets to be clicked, without transfering focus.  Widget will receive
		// Mouse Button Down Event.
		if(!mWidgetContainingMouse->getGainFocusOnClick())
		{
			return mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_BUTTON_DOWN,args);
		}

		// mActiveWidget is the last widget the user clicked on, ie TextBox, ComboBox, etc.
		if( mActiveWidget->getInstanceName() != mWidgetContainingMouse->getInstanceName() )
		{
			if(mActiveWidget->fireEvent(Widget::EVENT_LOSE_FOCUS,args))
				eventHandled = true;

			// Update active widget reference.
			mActiveWidget = mWidgetContainingMouse;
		}
		
		args.widget = mActiveWidget;
		args.position = mMouseCursor->getPosition();
		args.button = button;

		if(mActiveWidget->fireEvent(Widget::EVENT_MOUSE_BUTTON_DOWN,args))
			eventHandled = true;
		if(mActiveWidget->fireEvent(Widget::EVENT_GAIN_FOCUS,args))
			eventHandled = true;
		mActiveWidget->setGrabbed(true);
			
		// If the user clicked on a widget that is a part of a window, make sure the window is brought to front.
		Window* w = mActiveWidget->getParentWindow();
		if( w != NULL ) 
			w->bringToFront();

		// Record that the mouse button went down on this widget (non-window)
		mMouseButtonDown[args.button] = mActiveWidget;
		
		mMouseButtonTimings[button] = mTimer->getMilliseconds();

		return eventHandled;
	}

	bool GUIManager::injectMouseButtonUp(const MouseButtonID& button)
	{
		if( !mMouseCursor->isVisible() ) 
			return false;

		bool eventHandled = false;

		MouseEventArgs args(mActiveWidget);
		args.position = mMouseCursor->getPosition();
		args.button = button;

		// If dragging a widget, fire EVENT_DROPPED event
		if(mDraggingWidget)
		{
			mDraggingWidget = false;
			mActiveWidget->setGrabbed(false);
			eventHandled = mActiveWidget->fireEvent(Widget::EVENT_DROPPED,args);
			return eventHandled;
		}

		// Feature, allowing widgets to be clicked, without transfering focus.  Widget will receive
		// Mouse Button Up and Click Events, if appropriate.
		if(!mWidgetContainingMouse->getGainFocusOnClick())
		{
			if( mMouseButtonDown[args.button] == mWidgetContainingMouse )
			{
				eventHandled = mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_BUTTON_UP,args);
				// check if time elapsed it within click time.
				if( (mTimer->getMilliseconds() - mMouseButtonTimings[button]) < mClickTimeout ) 
					eventHandled = mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_CLICK,args);
			}
			return eventHandled;
		}

		// If the MouseButton was not pressed on this widget, do not register the button being released on the widget
		if( mWidgetContainingMouse->getInstanceName() != mActiveWidget->getInstanceName()  )
		{
			if(mActiveWidget->fireEvent(Widget::EVENT_LOSE_FOCUS,args))
				eventHandled = true;

			return eventHandled;
		}

		// after this point, we know that the user had mouse button down on this widget, and is now doing mouse button up

		args.widget = mActiveWidget;
		args.position = mMouseCursor->getPosition();
		args.button = button;

		if(mActiveWidget->fireEvent(Widget::EVENT_MOUSE_BUTTON_UP,args))
			eventHandled = true;
		mActiveWidget->setGrabbed(false);

		// check if time elapsed it within click time.
		if( (mTimer->getMilliseconds() - mMouseButtonTimings[button]) < mClickTimeout ) 
			if(mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_CLICK,args))
				eventHandled = true;

		return eventHandled;
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
		Widget* hitWidget;
		Widget* wItr = mActiveWidget;
		while( (hitWidget = wItr->getTargetWidget(args.position)) == NULL )
			wItr = wItr->getParentWidget();
		
		// NOTE: Widget is only detected if it is enabled.
		args.widget = hitWidget;

		bool eventHandled = false;

		if( (mWidgetContainingMouse->getInstanceName() != hitWidget->getInstanceName()) )
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

		return mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_WHEEL,args);
	}

	void GUIManager::injectTime(Ogre::Real time)
	{
		mActiveSheet->timeElapsed(time);
	}

	void GUIManager::loadSkin(const Ogre::String& skinName)
	{
		// check if imageset is already created for this skin
		if( mSkinSets.find(skinName) != mSkinSets.end() ) return;

		mSkinSets[skinName] = new SkinSet(skinName);
	}

	void GUIManager::renderQueueStarted(Ogre::uint8 id, const Ogre::String& invocation, bool& skipThisQueue)
	{
		if(mQueueID == id)
		{
			mActiveSheet->render();
			mMouseCursor->render();
		}
	}

	void GUIManager::renderQueueEnded(Ogre::uint8 id, const Ogre::String& invocation, bool& repeatThisQueue)
	{
		if(mFreeList.empty())
			return;

		mActiveWidget = mActiveSheet;
		mWidgetContainingMouse = mActiveSheet;

		std::vector<Widget*>::iterator it;
		for( it = mFreeList.begin(); it != mFreeList.end(); ++it )
			delete (*it);
		mFreeList.clear();

		injectMouseMove(0,0);
	}

	void GUIManager::removeFromRenderQueue()
	{
		if(mSceneManager != NULL)
			mSceneManager->removeRenderQueueListener(this);
	}

	void GUIManager::removeWidgetName(const Ogre::String& name)
	{
		if( name == "" ) 
			return;

		Ogre::StringVector::iterator it = std::remove(mWidgetNames.begin(),mWidgetNames.end(),name);
		if( it != mWidgetNames.end() ) 
			mWidgetNames.erase(it);
	}

	void GUIManager::setActiveSheet(Sheet* s)
	{
		if( (s == NULL) || (s->getInstanceName() == mActiveSheet->getInstanceName()) ) 
			return;

		mActiveSheet = s;

		// Make sure active widget loses focus.
		mActiveWidget->fireEvent(Widget::EVENT_LOSE_FOCUS,EventArgs());
		
		// Make sure mouse over widget has mouse leave event.
		MouseEventArgs args(mWidgetContainingMouse);
		args.position = mMouseCursor->getPosition();

		mWidgetContainingMouse->fireEvent(Widget::EVENT_MOUSE_LEAVE,args);

		// Update the active widget
		mActiveWidget = mActiveSheet;
		mWidgetContainingMouse = mActiveSheet;
		injectMouseMove(0,0);
	}

	void GUIManager::setActiveWidget(Widget* w)
	{
		if ( !w->enabled() || w == NULL )
			return;

		if( w->getInstanceName() != mActiveWidget->getInstanceName() ) 
		{
			WidgetEventArgs args(mActiveWidget);
			mActiveWidget->fireEvent(Widget::EVENT_LOSE_FOCUS,args);
		}

		mActiveWidget = w;

		WidgetEventArgs args(mActiveWidget);
		mActiveWidget->fireEvent(Widget::EVENT_GAIN_FOCUS,args);
	}

	void GUIManager::setDebugString(const Ogre::String s)
	{
		mDebugString = s;
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

	void GUIManager::setSupportedCodePoints(const std::vector<Ogre::UTFString::code_point>& list)
	{
		mSupportedCodePoints.clear();
		mSupportedCodePoints.assign(list.begin(),list.end());
	}

	void GUIManager::setViewport(Ogre::Viewport* vp)
	{
		mViewport = vp;
	}

	bool GUIManager::skinLoaded(const Ogre::String& skinName)
	{
		return (mSkinSets.find(skinName) != mSkinSets.end());
	}

	bool GUIManager::textureExists(const Ogre::String& textureName)
	{
		if(textureName == "")
			return false;

		if(Utility::textureExistsOnDisk(textureName))
			return true;

		if(Ogre::TextureManager::getSingletonPtr()->resourceExists(textureName)) 
			return true;

		std::map<Ogre::String,SkinSet*>::iterator it;
		for( it = mSkinSets.begin(); it != mSkinSets.end(); ++it )
			if( it->second->containsImage(textureName) )
				return true;

		return false;
	}

	bool GUIManager::validWidgetName(const Ogre::String& name, bool addToList)
	{
		if( name == "" ) 
			return false;

		Ogre::StringVector::iterator it;
		for( it = mWidgetNames.begin(); it != mWidgetNames.end(); ++it )
		{
			if( (*it) == name ) 
				return false;
		}

		if(addToList) mWidgetNames.push_back(name);

		return true;
	}
}
