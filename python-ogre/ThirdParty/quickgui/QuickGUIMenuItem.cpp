#include "QuickGUIMenuItem.h"
#include "QuickGUIMenu.h"
#include "QuickGUIContextMenu.h"

namespace QuickGUI
{
	MenuItemDesc::MenuItemDesc(const Ogre::String& id) :
		ToolBarItemDesc(id),
		menu(NULL),
		contextMenu(NULL)
	{
		widget_resizeFromBottom = false;
		widget_resizeFromLeft = false;
		widget_resizeFromRight = false;
		widget_resizeFromTop = false;
	}

	MenuItem::MenuItem(const Ogre::String& name) :
		ToolBarItem(name)
	{
	}

	MenuItem::~MenuItem()
	{
	}

	void MenuItem::_initialize(WidgetDesc* d)
	{
		ToolBarItem::_initialize(d);

		MenuItemDesc* mid = dynamic_cast<MenuItemDesc*>(d);
		MenuItemDesc* mDesc = dynamic_cast<MenuItemDesc*>(mWidgetDesc);

		// The only time menu should be NULL is if the Menu is created by a ToolBar, 
		// and has no parent menu.  In this case we assign the owning menu to ourself.
		if(mid->menu == NULL)
			mDesc->menu = dynamic_cast<Menu*>(this);
		else
			mDesc->menu = mid->menu;
	}

	ContextMenu* MenuItem::getContextMenu()
	{
		return dynamic_cast<MenuItemDesc*>(mWidgetDesc)->contextMenu;
	}

	Menu* MenuItem::getMenu()
	{
		return dynamic_cast<MenuItemDesc*>(mWidgetDesc)->menu;
	}

	ToolBar* MenuItem::getToolBar()
	{
		return dynamic_cast<MenuItemDesc*>(mWidgetDesc)->toolBar;
	}

	bool MenuItem::isMenuItem()
	{
		return true;
	}

	void MenuItem::notifyContextMenuParent(ContextMenu* m)
	{
		dynamic_cast<MenuItemDesc*>(mWidgetDesc)->contextMenu = m;
	}

	void MenuItem::notifyMenuParent(Menu* m)
	{
		dynamic_cast<MenuItemDesc*>(mWidgetDesc)->menu = m;
	}

	void MenuItem::notifyToolBarParent(ToolBar* b)
	{
		dynamic_cast<MenuItemDesc*>(mWidgetDesc)->toolBar = b;
	}
}
