#include "QuickGUIMenu.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"

namespace QuickGUI
{
	const Ogre::String Menu::DEFAULT = "default";
	const Ogre::String Menu::DOWN = "down";
	const Ogre::String Menu::OVER = "over";
	const Ogre::String Menu::MENUPANEL = "menupanel";

	void Menu::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Menu");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(DOWN);
		d->defineComponent(MENUPANEL);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Menu",d);
	}

	MenuDesc::MenuDesc(const Ogre::String& id) :
		MenuLabelDesc(id)
	{
		resetToDefault();
	}

	void MenuDesc::resetToDefault()
	{
		MenuLabelDesc::resetToDefault();

		menu = NULL;
		menu_itemHeight = 25;
		menu_maxMenuHeight = 0;
		menu_menuWidth = 100;
		menu_subMenuOverlap = 2;
	}

	void MenuDesc::serialize(SerialBase* b)
	{
		MenuLabelDesc::serialize(b);

		b->IO("ItemHeight",&menu_itemHeight);
		b->IO("MaxMenuHeight",&menu_maxMenuHeight);
		b->IO("MenuWidth",&menu_menuWidth);
		b->IO("SubMenuOverlap",&menu_subMenuOverlap);
	}

	Menu::Menu(const Ogre::String& name) :
		MenuLabel(name),
		mMenuPanel(NULL),
		mAutoNameCounter(0)
	{
		mSkinElementName = DEFAULT;
	}

	Menu::~Menu()
	{
		if(!mWidgetDesc->sheet->mDeleting)
		{
			if(mWidgetDesc->sheet != NULL)
				mWidgetDesc->sheet->removeWindow(mMenuPanel);

			OGRE_DELETE_T(mMenuPanel,MenuPanel,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	float Menu::_getNextAvailableYPosition()
	{
		float maxY = 0;
		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
		{
			if(((*it)->getPosition().y + (*it)->getSize().height) > maxY)
				maxY = ((*it)->getPosition().y + (*it)->getSize().height);
		}

		return maxY;
	}

	void Menu::_initialize(WidgetDesc* d)
	{
		MenuLabel::_initialize(d);

		mDesc = dynamic_cast<MenuDesc*>(mWidgetDesc);

		MenuDesc* md = dynamic_cast<MenuDesc*>(d);

		// Copy over all properties from desc param
		mDesc->menu_maxMenuHeight = md->menu_maxMenuHeight;
		mDesc->menu_menuWidth = md->menu_menuWidth;
		mDesc->menu_subMenuOverlap = md->menu_subMenuOverlap;

		// Create our Menu List Window
		MenuPanelDesc* lpd = FactoryManager::getSingleton().getDescFactory()->getInstance<MenuPanelDesc>("DefaultMenuPanelDesc");
		lpd->resetToDefault();
		lpd->widget_name = mWidgetDesc->widget_name + ".MenuPanel";
		lpd->widget_dimensions = Rect(0,0,mDesc->menu_menuWidth,1);
		lpd->widget_visible = false;
		lpd->menupanel_owner = this;
		lpd->menupanel_maxHeight = mDesc->menu_maxMenuHeight;
		mMenuPanel = dynamic_cast<MenuPanel*>(Widget::create("MenuPanel",lpd));

		setSkinType(d->widget_skinTypeName);
	}

	void Menu::_setGUIManager(GUIManager* gm)
	{
		MenuLabel::_setGUIManager(gm);

		if(mMenuPanel != NULL)
			mMenuPanel->_setGUIManager(gm);

		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			(*it)->_setGUIManager(gm);
	}

	void Menu::_setSheet(Sheet* sheet)
	{
		if(mWidgetDesc->sheet != NULL)
			mWidgetDesc->sheet->removeWindow(mMenuPanel);

		MenuLabel::_setSheet(sheet);

		if(sheet != NULL)
			sheet->addWindow(mMenuPanel);

		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			(*it)->_setSheet(sheet);
	}

	void Menu::_updateItemPositions()
	{
		int y = 0;
		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));
			y += mDesc->menu_itemHeight;
		}
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
		// Set link to parent ContextMenu
		i->notifyContextMenuParent(mDesc->contextMenu);
		i->_setGUIManager(mWidgetDesc->guiManager);
		i->_setSheet(mWidgetDesc->sheet);

		// We cannot add the widget as a child, the texture position and drawing will be incorrect.
		mMenuItems.push_back(i);
		
		// Add to the windows list of MenuItems
		mMenuPanel->addWidget(i);

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

	MenuItem* Menu::createMenuItem(const Ogre::String& className, MenuItemDesc* d)
	{
		if(className == "Menu")
			return createSubMenu(dynamic_cast<MenuDesc*>(d));

		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.MenuItem." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		// Make sure new Menu will maintain link to owner ToolBar
		d->toolBar = mDesc->toolBar;

		// Make sure new Menu will maintain link to owner Menu
		d->menu = this;

		// Determine position of MenuItem
		d->widget_dimensions.position.y = _getNextAvailableYPosition();
		d->widget_dimensions.size.width = mMenuPanel->getClientDimensions().size.width;
		d->widget_dimensions.size.height = mDesc->menu_itemHeight;
		
		MenuItem* newMenuItem = dynamic_cast<MenuItem*>(Widget::create(className,d));
		addChild(newMenuItem);

		return newMenuItem;
	}

	MenuLabel* Menu::createMenuLabel(MenuLabelDesc* d)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.MenuItem." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		// Make sure new Menu will maintain link to owner ToolBar
		d->toolBar = mDesc->toolBar;

		// Make sure new Menu will maintain link to owner Menu
		d->menu = this;

		// Determine position of MenuItem
		d->widget_dimensions.position.y = _getNextAvailableYPosition();
		d->widget_dimensions.size.width = mMenuPanel->getClientDimensions().size.width;
		d->widget_dimensions.size.height = mDesc->menu_itemHeight;

		MenuLabel* newMenuLabel = dynamic_cast<MenuLabel*>(Widget::create("MenuLabel",d));
		addChild(newMenuLabel);

		return newMenuLabel;
	}

	Menu* Menu::createSubMenu(MenuDesc* d)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.MenuItem." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		// Make sure new Menu will maintain link to owner ToolBar
		d->toolBar = mDesc->toolBar;

		// Make sure new Menu will maintain link to owner Menu
		d->menu = this;

		d->widget_verticalAnchor = ANCHOR_VERTICAL_TOP;

		// Determine position of Menu
		d->widget_dimensions.position.y = _getNextAvailableYPosition();
		d->widget_dimensions.size.width = mMenuPanel->getClientDimensions().size.width;
		d->widget_dimensions.size.height = mDesc->menu_itemHeight;

		Menu* newMenu = dynamic_cast<Menu*>(Widget::create("Menu",d));

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

		return mMenuPanel->findWidget(name);
	}

	float Menu::getItemHeight()
	{
		return mDesc->menu_itemHeight;
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

	void Menu::notifyContextMenuParent(ContextMenu* m)
	{
		MenuItem::notifyContextMenuParent(m);

		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			(*it)->notifyContextMenuParent(m);
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
				{
					if(mDesc->toolBar != NULL)
						mDesc->toolBar->openMenu(this);
					else if(mDesc->contextMenu != NULL)
						mDesc->contextMenu->openSubMenu(this);
				}
			}
		}
	}

	void Menu::onMouseButtonUp(const EventArgs& args)
	{
		// Purposefully left blank. (overrides MenuLabel::onMouseButtonUp, which is called as an event handler)
	}

	void Menu::onMouseEnter(const EventArgs& args)
	{
		MenuLabel::onMouseEnter(args);

		if(mParentWidget == NULL)
			throw Exception(Exception::ERR_INVALID_STATE,"Menu is not attached to any widget! (Parent is NULL)","Menu::onMouseEnter");

		// For standard menus, go through toolBar to open the right menu
		if((mDesc->toolBar != NULL) && mDesc->toolBar->isMenuOpened())
			mDesc->toolBar->openMenu(this);
		else if(mDesc->contextMenu != NULL)
			mDesc->contextMenu->openSubMenu(this);
	}

	void Menu::openMenu()
	{
		// Close all SubMenus
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
			(*it)->closeMenu();

		Point p = mDesc->widget_dimensions.position;

		if(mParentWidget->getClass() == "ToolBar")
		{
			ToolBar* tb = mDesc->toolBar;
			if(tb == NULL)
				throw Exception(Exception::ERR_INVALID_STATE,"Parent ToolBar reference is NULL!","Menu::openMenu");

			if(tb->getOrientation() == TOOLBAR_ORIENTATION_HORIZONTAL)
			{
				// Position the MenuList above or below the MenuLabel - below is desired
				if((getScreenPosition().y + mDesc->widget_dimensions.size.height + mMenuPanel->getSize().height) > mMenuPanel->getSheet()->getSize().height)
					p.y = mParentWidget->getScreenPosition().y - mMenuPanel->getSize().height;
				else
					p.y = mParentWidget->getScreenPosition().y + mParentWidget->getSize().height;
			}
			else // orientation is TOOLBAR_ORIENTATION_VERTICAL
			{
				// Position the MenuList to the left or right of the MenuLabel - right is desired
				if((getScreenPosition().x + mDesc->widget_dimensions.size.width + mMenuPanel->getSize().width) > mMenuPanel->getSheet()->getSize().width)
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
				p.x = mParentWidget->getScreenPosition().x - mMenuPanel->getSize().width + mDesc->menu_subMenuOverlap;
			else
				p.x = mParentWidget->getScreenPosition().x + mParentWidget->getSize().width - mDesc->menu_subMenuOverlap;

			p.y = getScreenPosition().y - mMenuPanel->mSkinType->getSkinElement(mMenuPanel->mSkinElementName)->getBorderThickness(BORDER_TOP);
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
		// Create Desc object if its not already created.
		_createDescObject();

		if(!mWidgetDesc->widget_serialize)
			return;

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
				Widget* newWidget = FactoryManager::getSingleton().getWidgetFactory()->createInstance<Widget>((*it)->getType(),(*it)->getID());

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

	void Menu::setItemHeight(float height)
	{
		mDesc->menu_itemHeight = height;

		_updateItemPositions();
	}

	void Menu::setSkinType(const Ogre::String type)
	{
		Widget::setSkinType(type);

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getComponentType((*it).first)->typeName);

		if(mMenuPanel != NULL)
			mMenuPanel->setSkinType(mSkinType->getComponentType(MENUPANEL)->typeName);
	}
}
