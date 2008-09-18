#include "QuickGUIMenu.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUIWidgetDescFactoryManager.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String Menu::DEFAULT = "default";
	const Ogre::String Menu::DOWN = "down";
	const Ogre::String Menu::OVER = "over";

	void Menu::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("Menu");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(DOWN);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Menu",d);
	}

	MenuDesc::MenuDesc() :
		MenuLabelDesc()
	{
		menu = NULL;
		menuWidth = 100;
		subMenuOverlap = 2;
		menuPanelSkinType = "default";
	}

	void MenuDesc::serialize(SerialBase* b)
	{
		MenuLabelDesc::serialize(b);

		b->IO("MenuWidth",&menuWidth);
		b->IO("MenuPanelSkinType",&menuPanelSkinType);
		b->IO("SubMenuOverlap",&subMenuOverlap);
	}

	Menu::Menu(const Ogre::String& name) :
		MenuLabel(name),
		mMenuPanel(NULL)
	{
		mSkinElementName= DEFAULT;
	}

	Menu::~Menu()
	{
	}

	void Menu::_initialize(WidgetDesc* d)
	{
		MenuLabel::_initialize(d);

		mDesc = dynamic_cast<MenuDesc*>(mWidgetDesc);

		setSkinType(d->skinTypeName);

		MenuDesc* md = dynamic_cast<MenuDesc*>(d);
		
		mText->setAllottedWidth(md->dimensions.size.width - (md->padding[PADDING_LEFT] + md->padding[PADDING_RIGHT]));
		setPadding(PADDING_BOTTOM,md->padding[PADDING_BOTTOM]);
		setPadding(PADDING_LEFT,md->padding[PADDING_LEFT]);
		setPadding(PADDING_RIGHT,md->padding[PADDING_RIGHT]);
		setPadding(PADDING_TOP,md->padding[PADDING_TOP]);

		mDesc->menuWidth = md->menuWidth;

		if(mMenuPanel != NULL)
			mWidgetDesc->sheet->destroyMenuPanel(mMenuPanel);

		// Create our Menu List Window
		MenuPanelDesc pd;
		pd.name = mWidgetDesc->name + ".MenuPanel";
		pd.dimensions = Rect(0,0,mDesc->menuWidth,1);
		pd.visible = false;
		pd.skinTypeName = md->menuPanelSkinType;
		mMenuPanel = mWidgetDesc->sheet->createMenuPanel(pd);
	}

	Widget* Menu::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new Menu(widgetName);

		newWidget->_createDescObject("MenuDesc");

		return newWidget;
	}

	Ogre::String Menu::getClass()
	{
		return "Menu";
	}

	void Menu::addChild(Widget* w)
	{
		if(!w->isMenuItem())
			throw Exception(Exception::ERR_INVALID_CHILD,"Cannot add non-MenuItem to a menu!","Menu::addChild");

		// Set link to parent Menu
		MenuItem* i = dynamic_cast<MenuItem*>(w);
		i->notifyMenuParent(this);
		// Set link to parent ToolBar
		i->notifyToolBarParent(mDesc->toolBar);

		// We cannot add the widget as a child, the texture position and drawing will be incorrect.
		mMenuItems.push_back(i);
		
		// Add to the windows list of MenuItems
		mMenuPanel->addMenuItem(i);

		if(w->getClass() == "Menu")
			mSubMenus.push_back(dynamic_cast<Menu*>(w));
	}

	void Menu::closeMenu()
	{
		// Close all SubMenus
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
			(*it)->closeMenu();

		mMenuPanel->setVisible(false);
	}

	void Menu::closeSubMenus()
	{
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			(*it)->closeMenu();
		}
	}

	MenuItem* Menu::createMenuItem(const Ogre::String& className, MenuItemDesc& d)
	{
		if(className == "Menu")
			return createSubMenu(dynamic_cast<MenuDesc&>(d));

		// Make sure new Menu will maintain link to owner ToolBar
		d.toolBar = mDesc->toolBar;

		// Make sure new Menu will maintain link to owner Menu
		d.menu = this;

		// Determine position of MenuItem
		d.dimensions.position.y = mMenuPanel->getNextAvailableYPosition();
		d.dimensions.size.width = mMenuPanel->getSize().width;
		
		MenuItem* newMenuItem = dynamic_cast<MenuItem*>(mWidgetDesc->guiManager->createWidget(className,d));
		addChild(newMenuItem);

		return newMenuItem;
	}

	MenuLabel* Menu::createMenuLabel(MenuLabelDesc& d)
	{
		// Make sure new Menu will maintain link to owner ToolBar
		d.toolBar = mDesc->toolBar;

		// Make sure new Menu will maintain link to owner Menu
		d.menu = this;

		// Determine position of MenuItem
		d.dimensions.position.y = mMenuPanel->getNextAvailableYPosition();
		d.dimensions.size.width = mMenuPanel->getClientDimensions().size.width;
		d.dimensions.size.height = d.textDesc.getTextHeight() + (d.padding[PADDING_TOP] + d.padding[PADDING_BOTTOM]);

		MenuLabel* newMenuLabel = dynamic_cast<MenuLabel*>(mWidgetDesc->guiManager->createWidget("MenuLabel",d));
		addChild(newMenuLabel);

		return newMenuLabel;
	}

	Menu* Menu::createSubMenu(MenuDesc& d)
	{
		// Make sure new Menu will maintain link to owner ToolBar
		d.toolBar = mDesc->toolBar;

		// Make sure new Menu will maintain link to owner Menu
		d.menu = this;

		// Determine position of Menu
		d.dimensions.position.y = mMenuPanel->getNextAvailableYPosition();
		d.dimensions.size.width = mMenuPanel->getClientDimensions().size.width;
		d.dimensions.size.height = d.textDesc.getTextHeight() + (d.padding[PADDING_TOP] + d.padding[PADDING_BOTTOM]);

		Menu* newMenu = dynamic_cast<Menu*>(mWidgetDesc->guiManager->createWidget("Menu",d));

		addChild(newMenu);

		return newMenu;
	}

	Widget* Menu::findWidget(const Ogre::String& name)
	{
		if(getName() == name)
			return this;

		Widget* w = NULL;

		// Search through menu items
		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
		{
			w = (*it)->findWidget(name);
			if(w != NULL)
				return w;
		}

		return NULL;
	}

	int Menu::getNumberOfItems()
	{
		return static_cast<int>(mMenuItems.size());
	}

	bool Menu::hasSubMenu(Menu* m)
	{
		// Check if menu is an immediate sub menu
		if(this == m)
			return true;

		// recurse submenus to see if menu is a submenu
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			if( (*it)->hasSubMenu(m) )
				return true;
		}

		return false;
	}

	bool Menu::isSubMenu()
	{
		if(mParentWidget == NULL)
			return false;

		return (mParentWidget->getClass() != "ToolBar");
	}

	void Menu::notifyMenuParent(Menu* m)
	{
		MenuItem::notifyMenuParent(m);

		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			(*it)->notifyMenuParent(this);
	}

	void Menu::notifyToolBarParent(ToolBar* b)
	{
		MenuItem::notifyToolBarParent(b);

		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			(*it)->notifyToolBarParent(b);
	}

	void Menu::onMouseButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(mParentWidget == NULL)
				throw Exception(Exception::ERR_INVALID_STATE,"Menu is not attached to any widget! (Parent is NULL)","Menu::onLeftMouseDown");

			if(mParentWidget->getClass() == "ToolBar")
			{
				if(mMenuPanel->getVisible())
					mDesc->toolBar->closeMenus();
				else
					mDesc->toolBar->openMenu(this);
			}
			else
			{
				if(mMenuPanel->getVisible())
					closeMenu();
				else
					mDesc->toolBar->openMenu(this);
			}
		}
	}

	void Menu::onMouseButtonUp(const EventArgs& args)
	{
	}

	void Menu::onMouseEnter(const EventArgs& args)
	{
		MenuLabel::onMouseEnter(args);

		if(mParentWidget == NULL)
			throw Exception(Exception::ERR_INVALID_STATE,"Menu is not attached to any widget! (Parent is NULL)","Menu::onMouseEnter");

		if(mDesc->toolBar->isMenuOpened())
			mDesc->toolBar->openMenu(this);
	}

	void Menu::openMenu()
	{
		if(getNumberOfItems() == 0)
		{
			closeMenu();
			return;
		}

		// Close all SubMenus
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
			(*it)->closeMenu();

		Point p = mDesc->dimensions.position;

		if(mParentWidget->getClass() == "ToolBar")
		{
			ToolBar* tb = mDesc->toolBar;
			if(tb == NULL)
				throw Exception(Exception::ERR_INVALID_STATE,"Parent ToolBar reference is NULL!","Menu::openMenu");

			if(tb->getOrientation() == TOOLBAR_ORIENTATION_HORIZONTAL)
			{
				// Position the MenuList above or below the MenuLabel - below is desired
				if((getScreenPosition().y + mDesc->dimensions.size.height + mMenuPanel->getSize().height) > mMenuPanel->getSheet()->getSize().height)
					p.y = mParentWidget->getScreenPosition().y - mMenuPanel->getSize().height;
				else
					p.y = mParentWidget->getScreenPosition().y + mParentWidget->getSize().height;
			}
			else // orientation is TOOLBAR_ORIENTATION_VERTICAL
			{
				// Position the MenuList to the left or right of the MenuLabel - right is desired
				if((getScreenPosition().x + mDesc->dimensions.size.width + mMenuPanel->getSize().width) > mMenuPanel->getSheet()->getSize().width)
					p.x = mParentWidget->getScreenPosition().x - mMenuPanel->getSize().width;
				else
					p.x = mParentWidget->getScreenPosition().x + mParentWidget->getSize().width;
			}
		}
		else // parent class is "Window", which means this menu is a SubMenu of another Menu
		{
			Menu* m = mDesc->menu;
			if(m == NULL)
				throw Exception(Exception::ERR_INVALID_STATE,"Parent Menu reference is NULL!","Menu::openMenu");

			// Position the MenuList to the left or right of the MenuLabel - right is desired
			if((getScreenPosition().x + getSize().width + mMenuPanel->getSize().width) > mMenuPanel->getSheet()->getSize().width)
				p.x = mParentWidget->getScreenPosition().x - mMenuPanel->getSize().width + mDesc->subMenuOverlap;
			else
				p.x = mParentWidget->getScreenPosition().x + mParentWidget->getSize().width - mDesc->subMenuOverlap;

			p.y = getScreenPosition().y;
		}

		mMenuPanel->setPosition(p);
		mMenuPanel->setVisible(true);
	}

	void Menu::openSubMenu(Menu* m)
	{
		if( this == m )
		{
			openMenu();
			return;
		}

		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			if((*it)->hasSubMenu(m))
			{
				openMenu();
				(*it)->openSubMenu(m);
			}
			else
				(*it)->closeMenu();
		}
	}

	void Menu::serialize(SerialBase* b)
	{
		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		b->begin("Child","widgets");
		// If we are reading, we need to create and populate widgets
		if(b->isSerialReader())
		{
			// Apply Desc properties
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
			for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			{
				(*it)->serialize(b);
			}
		}
		b->end();

		b->end();
	}
}
