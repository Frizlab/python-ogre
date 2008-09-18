#include "QuickGUIMenuItem.h"
#include "QuickGUIMenu.h"

namespace QuickGUI
{
	MenuItemDesc::MenuItemDesc() :
		ToolBarItemDesc()
	{
		resizable = false;
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

	void MenuItem::notifyMenuParent(Menu* m)
	{
		dynamic_cast<MenuItemDesc*>(mWidgetDesc)->menu = m;
	}

	void MenuItem::notifyToolBarParent(ToolBar* b)
	{
		dynamic_cast<MenuItemDesc*>(mWidgetDesc)->toolBar = b;
	}
}
