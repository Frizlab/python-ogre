#include "SimpleGUIManager.h"

#include "OgreFontManager.h"

template<> SimpleGUI::GUIManager* Ogre::Singleton<SimpleGUI::GUIManager>::ms_Singleton = 0;

namespace SimpleGUI
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

	Window* GUIManager::createEmptyWindow(const Ogre::String& name, const Ogre::Vector4& dimensions, bool show)
	{
		Window* newWindow = new Window(name,dimensions,show);

		// All empty windows are set at 1
		newWindow->setZOrder(1);
		mWindows.push_back(newWindow);

		return newWindow;
	}

	Window* GUIManager::createEmptyWindow(const Ogre::Vector4& dimensions, bool show)
	{
		Ogre::String name = "DefaultWindow" + Ogre::StringConverter::toString(mAutoNameWindowCounter);
		++mAutoNameWindowCounter;

		Window* newWindow = new Window(name,dimensions,show);

		// All empty windows are set at 1
		newWindow->setZOrder(1);
		mWindows.push_back(newWindow);

		return newWindow;
	}

	MouseCursor* GUIManager::createMouseCursor(const Ogre::Vector2& dimensions, const Ogre::String& material, bool show)
	{
		//convert relative coordinates to pixel coordinates, since mouse works in pixels
		mMouseCursor = new MouseCursor(Ogre::Vector2(dimensions.x*mRenderWindowWidth,dimensions.y*mRenderWindowHeight),material,mRenderWindowWidth,mRenderWindowHeight);
		if(show) mMouseCursor->show();
		else mMouseCursor->hide();

		return mMouseCursor;
	}

	Window* GUIManager::createWindow(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		Window* newWindow = new Window(name,dimensions,material,show);

		// Each window is allowed up to 5 zOrders above it, for menus, titlebar, and other child widgets
		Ogre::ushort highestZOrder = (static_cast<Ogre::ushort>(mWindows.size()) * 5) + 2;

		// note that zOrder starts at 2.  0 and 1 are reserved for the screen menu widget, if the user creates it
		newWindow->setZOrder(highestZOrder+1);
		mWindows.push_front(newWindow);

		return newWindow;
	}

	Window* GUIManager::createWindow(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		Ogre::String name = "DefaultWindow" + Ogre::StringConverter::toString(mAutoNameWindowCounter);
		++mAutoNameWindowCounter;

		Window* newWindow = new Window(name,dimensions,material,show);

		// Each window is allowed up to 5 zOrders above it, for menus, titlebar, and other child widgets
		Ogre::ushort highestZOrder = (static_cast<Ogre::ushort>(mWindows.size()) * 5) + 2;

		// note that zOrder starts at 2.  0 and 1 are reserved for the screen menu widget, if the user creates it
		newWindow->setZOrder(highestZOrder+1);
		mWindows.push_front(newWindow);

		return newWindow;
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
			if( (*it)->getInstanceName() == name )
			{
				Window* w = (*it);
				it = mWindows.erase(it);
				delete w;
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
		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
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
			// mActiveWidget is the last widget the user clicked on, it TextBox, ComboBox, etc.
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
				
			mWidgetContainingMouse->getWindow()->activate(args);

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
			(mWidgetContainingMouse->getWidgetType() == Widget::SGUI_TITLEBAR) &&
			(mWidgetContainingMouse->getGrabbed()) )
		{
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
	
		// Code used to display currently focused widget.  Very helpful for debugging
//		Ogre::String s = "NULL";
//		if(mWidgetContainingMouse != NULL) s = mWidgetContainingMouse->getInstanceName();
//		Window* w = getWindow("textbox window");
//		if(w) w->getTextBox(0)->setText(s);

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
		if( w->getWidgetType() == Widget::SGUI_PLAINWINDOW ) return;

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

	void GUIManager::sortListByWindowZOrder()
	{
		// Re-organize all z-orders 
		// Each window is allowed up to 5 zOrders above it, for menus, titlebar, and other child widgets
		Ogre::ushort zOrder = (static_cast<Ogre::ushort>(mWindows.size()) * 5) + 2;
		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			if( (*it)->getWidgetType() == Widget::SGUI_WINDOW )
			{
				(*it)->setZOrder(zOrder);
				--zOrder;
			}
		}
	}
}
