#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUIScriptWriter.h"
#include "QuickGUISerialReader.h"
#include "QuickGUIWidgetDescFactoryManager.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String Sheet::BACKGROUND = "background";

	void Sheet::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("Sheet");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Sheet",d);
	}

	SheetDesc::SheetDesc() :
		WindowDesc()
	{
		WindowDesc::titleBar = false;
		resizable = false;
		horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
	}

	void SheetDesc::serialize(SerialBase* b)
	{
		// The reason I skip calling WindowDesc::serialize is because I don't
		// want the Sheet's properties to include the Window's TitleBar 
		// properties. (Nor do I want users to manually add TitleBars to Sheets)
		PanelDesc::serialize(b);
	}

	Sheet::Sheet(SheetDesc& d) :
		Window(d.name),
		mWindowInFocus(0),
		mKeyboardListener(0)
	{
		mSkinElementName = BACKGROUND;
		_createDescObject("SheetDesc");
		_initialize(&d);
	}

	Sheet::Sheet(const Ogre::String& fileName) :
		Window(""),
		mWindowInFocus(0),
		mKeyboardListener(0)
	{
		mSkinElementName = BACKGROUND;
		_createDescObject("SheetDesc");

		// Perform an isolated parsing of the one file
		ScriptReader* sc = ScriptReader::getSingletonPtr();
		sc->begin();
		sc->parseFile(fileName);

		if(sc->getDefinitions().empty())
			throw Exception(Exception::ERR_SERIALIZATION,"No definitions found in file \"" + fileName + "\"!","GUIManager::reloadSheetFromFile");

		mWidgetDesc->name = sc->getDefinitions().front()->getID();
		serialize(SerialReader::getSingletonPtr());

		// remove all temporary definitions found from parsing the file
		sc->end();
	}

	Sheet::~Sheet()
	{
	}

	void Sheet::_initialize(WidgetDesc* d)
	{
		Window::_initialize(d);

		setSkinType(d->skinTypeName);

		mWidgetDesc->sheet = this;
	}

	void Sheet::addWindow(Window* w)
	{
		mWindows.push_back(w);
		w->mWidgetDesc->sheet = this;
		w->mWidgetDesc->guiManager = mWidgetDesc->guiManager;
	}

	void Sheet::addModalWindow(ModalWindow* w)
	{
		mModalWindows.push_back(w);
		w->mWidgetDesc->sheet = this;
		w->mWidgetDesc->guiManager = mWidgetDesc->guiManager;
	}

	void Sheet::bringToFront(Window* w)
	{
		std::list<Window*>::iterator it = std::find(mWindows.begin(),mWindows.end(),w);
		if(it == mWindows.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Window \"" + w->getName() + "\" does not belong to Sheet \"" + getName() + "\".", "Sheet::bringToFront");

		mWindows.erase(it);

		mWindows.push_back(w);
	}

	void Sheet::cleanupWidgets()
	{
		for(std::list<Widget*>::iterator it = mFreeList.begin(); it != mFreeList.end(); ++it)
		{
			delete (*it);
		}

		mFreeList.clear();
	}

	ModalWindow* Sheet::createModalWindow(ModalWindowDesc& d)
	{
		ModalWindow* w = dynamic_cast<ModalWindow*>(Widget::create("ModalWindow",d));
		addModalWindow(w);
		return w;
	}

	Window* Sheet::createWindow(WindowDesc& d)
	{
		Window* w = dynamic_cast<Window*>(Widget::create("Window",d));
		addWindow(w);
		return w;
	}

	void Sheet::destroyModalWindow(const Ogre::String& name)
	{
		destroyModalWindow(getModalWindow(name));
	}

	void Sheet::destroyModalWindow(ModalWindow* w)
	{
		if(w == NULL)
			throw Exception(Exception::ERR_INVALID_CHILD,"Window is NULL!","Sheet::destroyModalWindow");

		std::list<ModalWindow*>::iterator it = std::find(mModalWindows.begin(),mModalWindows.end(),w);
		if(it == mModalWindows.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"ModalWindow \"" + w->getName() + "\" is not a child of Sheet \"" + getName() + "\"","Sheet::destroyModalWindow");
		mModalWindows.erase(it);

		if(std::find(mFreeList.begin(),mFreeList.end(),w) != mFreeList.end())
			return;

		if((getKeyboardListener() != NULL) && (w->findWidget(getKeyboardListener()->getName()) != NULL))
			setKeyboardListener(NULL);

		mFreeList.push_back(w);
	}

	void Sheet::destroyWidget(Widget* w)
	{
		Ogre::String className = w->getClass();
		if(className == "ModalWindow")
			destroyModalWindow(dynamic_cast<ModalWindow*>(w));
		else if(className == "Window")
			destroyWindow(dynamic_cast<Window*>(w));
		else if(w->getParentWidget() != NULL)
			throw Exception(Exception::ERR_INVALID_STATE,"Widget it attached to another widget!","Sheet::destroyWidget");

		if(std::find(mFreeList.begin(),mFreeList.end(),w) != mFreeList.end())
			return;

		if((getKeyboardListener() != NULL) && (w->findWidget(getKeyboardListener()->getName()) != NULL))
			setKeyboardListener(NULL);

		mFreeList.push_back(w);
	}

	void Sheet::destroyWindow(const Ogre::String& name)
	{
		destroyWindow(getWindow(name));
	}

	void Sheet::destroyWindow(Window* w)
	{
		if(w == NULL)
			throw Exception(Exception::ERR_INVALID_CHILD,"Window is NULL!","Sheet::destroyWindow");

		std::list<Window*>::iterator it = std::find(mWindows.begin(),mWindows.end(),w);
		if(it == mWindows.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Window \"" + w->getName() + "\" is not a child of Sheet \"" + getName() + "\"","Sheet::destroyWindow");
		mWindows.erase(it);

		if(std::find(mFreeList.begin(),mFreeList.end(),w) != mFreeList.end())
			return;

		if((getKeyboardListener() != NULL) && (w->findWidget(getKeyboardListener()->getName()) != NULL))
			setKeyboardListener(NULL);

		mFreeList.push_back(w);
	}

	void Sheet::drag(unsigned int xOffset, unsigned int yOffset)
	{
		Widget::drag(xOffset,yOffset);
	}

	void Sheet::draw()
	{
		Window::draw();

		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
			(*it)->draw();

		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
			(*it)->draw();
	}

	Widget* Sheet::findWidget(const Ogre::String& name)
	{
		Widget* w = NULL;

		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
			if((w = (*it)->findWidget(name)) != NULL)
				return w;
		
		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
			if((w = (*it)->findWidget(name)) != NULL)
				return w;

		return ContainerWidget::findWidget(name);
	}

	Window* Sheet::findWindowAtPoint(const Point& p, bool ignoreDisabled)
	{
		for(std::list<ModalWindow*>::reverse_iterator it = mModalWindows.rbegin(); it != mModalWindows.rend(); ++it)
		{
			// Visible Modal Window found at point p
			if((*it)->getVisible() && (*it)->getDimensions().isPointWithinBounds(p))
			{
				// If we are ignoring disabled windows and the window is disabled, do not return it.
				if(ignoreDisabled && !((*it)->getEnabled()))
					continue;

				return (*it);
			}
		}

		for(std::list<Window*>::reverse_iterator it = mWindows.rbegin(); it != mWindows.rend(); ++it)
		{
			// Visible Window found at point p
			if((*it)->getVisible() && (*it)->getDimensions().isPointWithinBounds(p))
			{
				// If we are ignoring disabled windows and the window is disabled, do not return it.
				if(ignoreDisabled && !((*it)->getEnabled()))
					continue;

				return (*it);
			}
		}

		return this;
	}

	bool Sheet::focusWindow(Window* w)
	{
		// Cannot bring the Sheet in front of child Windows
		if(w == this)
			return false;

		std::list<Window*>::iterator it = std::find(mWindows.begin(),mWindows.end(),w);
		if(it == mWindows.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Window \"" + w->getName() + "\" does not belong to Sheet \"" + getName() + "\".", "Sheet::focusWindow");

		if(mWindowInFocus != w)
		{
			if(mWindowInFocus)
			{
				WidgetEventArgs args(mWindowInFocus);
				mWindowInFocus->fireWindowEvent(WINDOW_EVENT_FOCUS_LOST,args);
			}

			mWindowInFocus = w;

			if(mWindowInFocus)
			{
				WidgetEventArgs args(mWindowInFocus);
				mWindowInFocus->fireWindowEvent(WINDOW_EVENT_FOCUS_GAINED,args);
			}

			return true;
		}

		return false;
	}

	Ogre::String Sheet::getClass()
	{
		return "Sheet";
	}

	Widget* Sheet::getKeyboardListener()
	{
		return mKeyboardListener;
	}

	ModalWindow* Sheet::getModalWindow(const Ogre::String& name)
	{
		if(!hasModalWindow(name))
			throw Exception(Exception::ERR_INVALID_CHILD,"Sheet \"" + getName() + "\" does not have a child modal window with name \"" + name + "\".","Sheet::getModalWindow");

		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
		{
			if((*it)->getName() == name)
				return (*it);
		}

		return NULL;
	}

	Window* Sheet::getWindow(const Ogre::String& name)
	{
		if(!hasWindow(name))
			throw Exception(Exception::ERR_INVALID_CHILD,"Sheet \"" + getName() + "\" does not have a child window with name \"" + name + "\".","Sheet::getWindow");

		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			if(((*it)->getName() == name) && ((*it)->getClass() == "Window"))
				return (*it);
		}

		return NULL;
	}

	Window* Sheet::getWindowInFocus()
	{
		return mWindowInFocus;
	}

	bool Sheet::hasModalWindow(const Ogre::String& name)
	{
		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
		{
			if((*it)->getName() == name)
				return true;
		}

		return false;
	}

	bool Sheet::hasWindow(const Ogre::String& name)
	{
		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			if(((*it)->getName() == name) && ((*it)->getClass() == "Window"))
				return true;
		}

		return false;
	}

	void Sheet::removeWindow(Window* w)
	{
		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			if((*it) == w)
			{
				mWindows.erase(it);
				return;
			}
		}
	}

	void Sheet::saveToDisk(const Ogre::String fileName)
	{
		ScriptWriter::getSingletonPtr()->begin(fileName);

		serialize(SerialWriter::getSingletonPtr());

		ScriptWriter::getSingletonPtr()->end();
	}

	void Sheet::serialize(SerialBase* b)
	{
		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		b->begin("Child","widgets");
		// If we are reading, we need to create and populate widgets
		if(b->isSerialReader())
		{
			_initialize(mWidgetDesc);

			std::list<ScriptDefinition*> defList = b->getCurrentDefinition()->getDefinitions();
			for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
			{
				// Create Empty Widget, supplying class name and widget name from script
				Widget* newWidget = WidgetFactoryManager::getSingleton().createWidget((*it)->getType(),(*it)->getID());

				// Populate Desc object from Script Text, and initialize widget
				newWidget->serialize(b);

				// Add as child Widget
				addChild(newWidget);
			}
		}
		// If we are writing, we need to serialize children
		else
		{
			for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			{
				(*it)->serialize(b);
			}
		}
		b->end();

		b->begin("Child","Windows");
		// If we are reading, we need to create and populate widgets
		if(b->isSerialReader())
		{
			std::list<ScriptDefinition*> defList = b->getCurrentDefinition()->getDefinitions();
			for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
			{
				// Create Empty Widget, supplying class name and widget name from script
				Widget* newWidget = WidgetFactoryManager::getSingleton().createWidget((*it)->getType(),(*it)->getID());

				// Populate Desc object from Script Text, and initialize widget
				newWidget->serialize(b);

				// Windows are not children, make sure to add them appropriately
				addWindow(dynamic_cast<Window*>(newWidget));
			}
		}
		// If we are writing, we need to serialize children
		else
		{
			for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
			{
				(*it)->serialize(b);
			}
		}
		b->end();

		b->end();
	}

	void Sheet::setKeyboardListener(Widget* w)
	{
		if(mKeyboardListener == w)
			return;

		if(mKeyboardListener != NULL)
		{
			WidgetEventArgs args(mKeyboardListener);
			mKeyboardListener->fireWidgetEvent(WIDGET_EVENT_KEYBOARD_INPUT_LOSE,args);
		}

		mKeyboardListener = w;

		if(mKeyboardListener != NULL)
		{
			WidgetEventArgs args(mKeyboardListener);
			mKeyboardListener->fireWidgetEvent(WIDGET_EVENT_KEYBOARD_INPUT_GAIN,args);
		}
	}
}
