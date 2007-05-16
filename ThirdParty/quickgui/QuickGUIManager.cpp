#include "QuickGUIManager.h"

#include "OgreFontManager.h"

template<> QuickGUI::GUIManager* Ogre::Singleton<QuickGUI::GUIManager>::ms_Singleton = 0;

namespace QuickGUI
{
	GUIManager::GUIManager(const unsigned int& RenderWindowWidth, const unsigned int& RenderWindowHeight) :
		mRenderWindowWidth(RenderWindowWidth),
		mRenderWindowHeight(RenderWindowHeight),
		mMouseCursor(0),
		mDefaultCharacterHeight(1),
		mDefaultTextColor(Ogre::ColourValue::White),
		mWidgetContainingMouse(0),
		mActiveWidget(0),
		mClickTimeout(1),
		mAutoNameWindowCounter(0)
	{
		Ogre::FontManager* fm = Ogre::FontManager::getSingletonPtr();
		Ogre::ResourceManager::ResourceMapIterator rmi = fm->getResourceIterator();
		if(rmi.hasMoreElements()) mDefaultFont = rmi.getNext()->getName();
		else mDefaultFont = "";
		mDefaultSkin = "qgui";

		mMouseButtonDown[0] = NULL;
		mMouseButtonDown[1] = NULL;
		mMouseButtonDown[2] = NULL;
		mMouseButtonDown[3] = NULL;
		mMouseButtonDown[4] = NULL;
		mMouseButtonDown[5] = NULL;
		mMouseButtonDown[6] = NULL;
		mMouseButtonDown[7] = NULL;
	}

	GUIManager::~GUIManager()
	{
		clearAll();
		destroyMouseCursor();
	}

	GUIManager* GUIManager::getSingletonPtr(void) { return ms_Singleton; }

	GUIManager& GUIManager::getSingleton(void) { assert( ms_Singleton );  return (*ms_Singleton); }

	void GUIManager::_notifyWindowDimensions(const unsigned int& RenderWindowWidth, const unsigned int& RenderWindowHeight)
	{
		mRenderWindowWidth = RenderWindowWidth;
		mRenderWindowHeight = RenderWindowHeight;
		
		if( mMouseCursor ) mMouseCursor->_updateWindowDimensions(mRenderWindowWidth,mRenderWindowHeight);

		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
			(*it)->_notifyDimensionsChanged();
	}

	void GUIManager::clearAll()
	{
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

		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
			delete (*it);
		mWindows.clear();

		// reset counter
		mAutoNameWindowCounter = 0;

		destroyMouseCursor();
	}

	Window* GUIManager::_createEmptyWindow(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		Window* newWindow = new Window(name,dimensions);

		// All empty windows are set at 1
		newWindow->setZOrder(1);
		mWindows.push_back(newWindow);

		return newWindow;
	}

	Window* GUIManager::createEmptyWindow(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = "DefaultWindow" + Ogre::StringConverter::toString(mAutoNameWindowCounter);
		++mAutoNameWindowCounter;

		return _createEmptyWindow(name,dimensions);
	}

	MouseCursor* GUIManager::createMouseCursor(const Ogre::Vector2& dimensions, const Ogre::String& material)
	{
		mMouseCursor = new MouseCursor(dimensions,material,mRenderWindowWidth,mRenderWindowHeight);

		return mMouseCursor;
	}

	MouseCursor* GUIManager::createMouseCursor(const Ogre::Vector2& dimensions)
	{
		Ogre::String material = mDefaultSkin + ".pointer";

		return createMouseCursor(dimensions,material);
	}

	Window* GUIManager::_createWindow(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Window* newWindow = new Window(name,dimensions,material);

		// Each window is allowed up to 5 zOrders above it, for menus, titlebar, and other child widgets
		Ogre::ushort highestZOrder = (static_cast<Ogre::ushort>(mWindows.size()) * 5) + 2;

		// note that zOrder starts at 2.  0 and 1 are reserved for the screen menu widget, if the user creates it
		newWindow->setZOrder(highestZOrder+1);
		mWindows.push_front(newWindow);

		return newWindow;
	}

	Window* GUIManager::createWindow(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		// Many widgets can have no material, (label or empty window for example) but a regular window
		// must have a material!
		if( material == "" ) return NULL;

		Ogre::String name = "DefaultWindow" + Ogre::StringConverter::toString(mAutoNameWindowCounter);
		++mAutoNameWindowCounter;

		return _createWindow(name,dimensions,material);
	}

	Window* GUIManager::createWindow(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = "DefaultWindow" + Ogre::StringConverter::toString(mAutoNameWindowCounter);
		++mAutoNameWindowCounter;

		Ogre::String material = mDefaultSkin + ".window";

		return _createWindow(name,dimensions,material);
	}

	void GUIManager::destroyMouseCursor()
	{
		delete mMouseCursor;
		mMouseCursor = NULL;
	}

	void GUIManager::destroyWindow(const Ogre::String& name)
	{
		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			if( ((*it)->getInstanceName() == name) || ((*it)->getReferenceName() == name) )
			{
				Window* w = (*it);
				it = mWindows.erase(it);
				delete w;
				break;
			}
		}

		sortListByWindowZOrder();
	}

	void GUIManager::destroyWindow(Window* window)
	{
		destroyWindow(window->getInstanceName());
	}

	Ogre::Real GUIManager::getDefaultCharacterHeight()
	{
		return mDefaultCharacterHeight;
	}

	Ogre::String GUIManager::getDefaultFont()
	{
		return mDefaultFont;
	}

	Ogre::ColourValue GUIManager::getDefaultTextColor()
	{
		return mDefaultTextColor;
	}

	Ogre::String GUIManager::getDefaultSkin()
	{
		return mDefaultSkin;
	}

	MouseCursor* GUIManager::getMouseCursor()
	{
		return mMouseCursor;
	}

	Widget* GUIManager::getMouseOverWidget()
	{
		return mWidgetContainingMouse;
	}

	Ogre::Real GUIManager::getRenderWindowAspectRatio()
	{
		return (static_cast<Ogre::Real>(mRenderWindowWidth) / static_cast<Ogre::Real>(mRenderWindowHeight));
	}

	unsigned int GUIManager::getRenderWindowWidth()
	{
		return mRenderWindowWidth;
	}

	unsigned int GUIManager::getRenderWindowHeight()
	{
		return mRenderWindowHeight;
	}

	Window* GUIManager::getWindow(const Ogre::String& name)
	{
		if( name == "" ) return NULL;

		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			if( ((*it)->getReferenceName() == name) || ((*it)->getInstanceName() == name) ) 
				return (*it);
		}

		return NULL;
	}

	bool GUIManager::injectChar(char c)
	{
		KeyEventArgs args(0);
		args.codepoint = c;
		args.handled = false;

		if( isalpha(c) || isdigit(c) || ispunct(c) || c == ' ' )
		{
			if(mActiveWidget != NULL) 
			{
				args.widget = mActiveWidget;
				mActiveWidget->onCharacter(args);
			}
		}

		return args.handled;
	}

	bool GUIManager::injectKeyDown(const KeyCode& kc)
	{
		KeyEventArgs args(0);
		args.scancode = kc;
		args.handled = false;

		if(mActiveWidget != NULL) 
		{
			args.widget = mActiveWidget;
			mActiveWidget->onKeyDown(args);
		}

		return args.handled;
	}

	bool GUIManager::injectKeyUp(const KeyCode& kc)
	{
		KeyEventArgs args(0);
		args.scancode = kc;
		args.handled = false;

		if(mActiveWidget != NULL) 
		{
			args.widget = mActiveWidget;
			mActiveWidget->onKeyUp(args);
		}

		return args.handled;
	}
	
	bool GUIManager::injectMouseButtonDown(const MouseButtonID& button)
	{
		if( (mMouseCursor == NULL) || (!mMouseCursor->isVisible()) ) return false;

		MouseEventArgs args(0);

		args.position.x = mMouseCursor->getPixelPosition().x;
		args.position.y = mMouseCursor->getPixelPosition().y;
		args.button = button;
		args.handled = false;

		if(mWidgetContainingMouse != NULL) 
		{
			// mActiveWidget is the last widget the user clicked on, ie TextBox, ComboBox, etc.
			if( (mActiveWidget != NULL) && (mActiveWidget->getInstanceName() != mWidgetContainingMouse->getInstanceName()) )
			{
				args.widget = mActiveWidget;
				mActiveWidget->deactivate(args);
				// reset to false, becase within this context, the event (mouseButtonDown) has not been handled
				args.handled = false;
				mActiveWidget = NULL;
			}

			mActiveWidget = mWidgetContainingMouse;
			
			args.widget = mActiveWidget;
			mWidgetContainingMouse->onMouseButtonDown(args);
			// Gaurds against the scenario where a handler changes game state (clears GUI)
			if( mWidgetContainingMouse == NULL ) return args.handled;
				
			// When a window becomes active, all its child widgets except for textboxes
			mWidgetContainingMouse->getWindow()->activate(args);
			// Only one textbox can be active at a time.  In the event that the clicked widget
			// is a textbox, activate it!
			if(mActiveWidget->getWidgetType() == Widget::QGUI_TYPE_TEXTBOX) mActiveWidget->activate(args);

			// Record that the mouse button went down on this widget (non-window)
			mMouseButtonDown[args.button] = mWidgetContainingMouse;
		}
		else
		{
			if( mActiveWidget != NULL )
			{
				args.widget = mActiveWidget;
				mActiveWidget->deactivate(args);
				mActiveWidget = NULL;
			}

			mMouseButtonDown[args.button] = NULL;
		}

		mMouseButtonTimings[button] = time(NULL);

		return args.handled;
	}

	bool GUIManager::injectMouseButtonUp(const MouseButtonID& button)
	{
		if( (mMouseCursor == NULL) || (!mMouseCursor->isVisible()) ) return false;

		MouseEventArgs args(0);

		args.position.x = mMouseCursor->getPixelPosition().x;
		args.position.y = mMouseCursor->getPixelPosition().y;
		args.button = button;
		args.handled = false;

		// If the MouseButton was not pressed on this widget, do not register the button being released on the widget
		// Obviously if the recorded widget is NULL, we know that nothing will be registered
		if( mMouseButtonDown[button] == NULL ) 
		{
			if( mActiveWidget != NULL ) mActiveWidget->deactivate(args);
			return false;
		}
		if( mWidgetContainingMouse == NULL )
		{
			if( mActiveWidget != NULL ) mActiveWidget->deactivate(args);
			return false;
		}
		if( mMouseButtonDown[button]->getInstanceName() != mWidgetContainingMouse->getInstanceName() )
		{
			if( mActiveWidget != NULL ) mActiveWidget->deactivate(args);
			return false;
		}

		// after this point, we know that the user had mouse button down on this widget, and is now doing mouse button up

		args.widget = mWidgetContainingMouse;

		mWidgetContainingMouse->onMouseButtonUp(args);
		// Need to gaurd against the scenario where  mousebutton up destroys the UI
		if( (mWidgetContainingMouse != NULL) && (time(NULL) - mMouseButtonTimings[button] < mClickTimeout) ) mWidgetContainingMouse->onMouseClicked(args);

		return args.handled;
	}

	bool GUIManager::injectMouseLeaves(void)
	{
		if( (mMouseCursor == NULL) || (!mMouseCursor->isVisible()) ) return false;

		if(mMouseCursor->getHideWhenOffScreen()) mMouseCursor->_hide();

		return true;
	}

	bool GUIManager::injectMouseMove(const int& xPixelOffset, const int& yPixelOffset)
	{
		if( mMouseCursor == NULL ) return false;

		MouseEventArgs args(0);

		args.moveDelta.x = xPixelOffset;
		args.moveDelta.y = yPixelOffset;
		args.handled = false;

		if( mMouseCursor->mouseOnTopBorder() && yPixelOffset < 0 ) args.moveDelta.y = 0;
		if( mMouseCursor->mouseOnBotBorder() && yPixelOffset > 0 ) args.moveDelta.y = 0;
		if( mMouseCursor->mouseOnLeftBorder() && xPixelOffset < 0 ) args.moveDelta.x = 0;
		if( mMouseCursor->mouseOnRightBorder() && xPixelOffset > 0 ) args.moveDelta.x = 0;

		// Update Mouse Cursor Position
		mMouseCursor->offsetPosition(xPixelOffset,yPixelOffset);

		args.position.x = mMouseCursor->getPixelPosition().x;
		args.position.y = mMouseCursor->getPixelPosition().y;

		if(!mMouseCursor->isVisible()) return args.handled;

		// First off, check if we are dragging a window.
		if( (mWidgetContainingMouse != NULL) && 
			(mWidgetContainingMouse->getWidgetType() == Widget::QGUI_TYPE_TITLEBAR) &&
			(mWidgetContainingMouse->getGrabbed()) )
		{
			// If a widget is active, make in inactive
			if( mActiveWidget != NULL ) mActiveWidget->deactivate(args);

			args.widget = mWidgetContainingMouse;
			mWidgetContainingMouse->getWindow()->move(
				args.moveDelta.x / static_cast<Ogre::Real>(mRenderWindowWidth),
				args.moveDelta.y / static_cast<Ogre::Real>(mRenderWindowHeight));

			return args.handled;
		}

		// Now get the widget the cursor is over.  Since widgets can overlap,
		// we store a pointer to the widget with the highest zOrder.
		Widget* hitWidget = NULL;
		Ogre::ushort WindowZOrder = 0;
		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			if( ((*it)->getTargetWidget(args.position)) && ((*it)->getZOrder() > WindowZOrder) )
			{
				hitWidget = (*it)->getTargetWidget(args.position);
				args.widget = hitWidget;
				WindowZOrder = (*it)->getZOrder();
			}
		}

		// Take care of properly firing MouseMoved, MouseEnters, and MouseLeaves events
		if(hitWidget)
		{
			if( mWidgetContainingMouse != NULL )
			{
				if( mWidgetContainingMouse->getInstanceName() != hitWidget->getInstanceName() )
				{
					mWidgetContainingMouse->onMouseLeaves(args);
					mWidgetContainingMouse = hitWidget;
					mWidgetContainingMouse->onMouseEnters(args);
				}
				mWidgetContainingMouse->onMouseMoved(args);
			}
			else
			{
				mWidgetContainingMouse = hitWidget;
				mWidgetContainingMouse->onMouseEnters(args);
				mWidgetContainingMouse->onMouseMoved(args);
			}
		}
		else
		{
			if( mWidgetContainingMouse != NULL )
			{
				mWidgetContainingMouse->onMouseLeaves(args);
			}
			mWidgetContainingMouse = NULL;
		}

		return args.handled;
	}

	bool GUIManager::injectMousePosition(const int& xPixelPosition, const int& yPixelPosition)
	{
		if( mMouseCursor == NULL ) return false;

		MouseEventArgs args(0);
		args.handled = false;

		Ogre::Vector2 pos = mMouseCursor->getPixelPosition();

		injectMouseMove(xPixelPosition - pos.x,yPixelPosition - pos.y);

		return args.handled;
	}

	bool GUIManager::injectMouseWheelChange(float delta)
	{
		MouseEventArgs args(0);
		args.handled = false;

		args.wheelChange = delta;

		if(mWidgetContainingMouse != NULL) 
		{
			args.widget = mWidgetContainingMouse;
			mWidgetContainingMouse->onMouseWheel(args);
		}

		return args.handled;
	}

	void GUIManager::injectTime(Ogre::Real time)
	{
		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			(*it)->timeElapsed(time);
		}
	}

	void GUIManager::setActiveWindow(Window* w)
	{
		// Plain Window's have a set zOrder, and should not be drawn over default windows
		if( w->getWidgetType() == Widget::QGUI_TYPE_PLAINWINDOW ) return;

		if( static_cast<unsigned int>(mWindows.size()) < 2 ||
			mWindows.front()->getInstanceName() == w->getInstanceName() )
			return;
		
		// If we make it here, the list has at least 2 windows, and the passed in window is not the active window

		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			if( w->getInstanceName() == (*it)->getInstanceName() )
			{
				it = mWindows.erase(it);
				break;
			}
		}

		mWindows.push_front(w);

		sortListByWindowZOrder();
	}

	void GUIManager::setDefaultFont(const Ogre::String& font)
	{
		mDefaultFont = font;
	}

	void GUIManager::setDefaultTextColor(const Ogre::ColourValue& color)
	{
		mDefaultTextColor = color;
	}

	void GUIManager::setDefaultCharacterHeight(const Ogre::Real& height)
	{
		mDefaultCharacterHeight = height;
	}

	void GUIManager::setDefaultSkin(const Ogre::String& skin)
	{
		mDefaultSkin = skin;
	}

	void GUIManager::sortListByWindowZOrder()
	{
		// Re-organize all z-orders 
		// Each window is allowed up to 5 zOrders above it, for menus, titlebar, and other child widgets
		Ogre::ushort zOrder = (static_cast<Ogre::ushort>(mWindows.size()) * 5) + 2;
		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			if( (*it)->getWidgetType() == Widget::QGUI_TYPE_WINDOW )
			{
				(*it)->setZOrder(zOrder);
				--zOrder;
			}
		}
	}
}
