#ifndef QUICKGUIMENU_H
#define QUICKGUIMENU_H

#include "QuickGUIException.h"
#include "QuickGUIMenuLabel.h"

#include <vector>

namespace QuickGUI
{
	// forward declarations
	class Window;
	class MenuPanel;

	class _QuickGUIExport MenuDesc :
			public MenuLabelDesc
	{
	public:
		friend class Menu;
		friend class ToolBar;
	public:
		MenuDesc();

		float menuWidth;
		Ogre::String menuPanelSkinType;
		/// Sets the amount of pixel overlap sub elements are drawn over this Menu.
		float subMenuOverlap;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "MenuDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Menu"; }

		// Factory method
		static WidgetDesc* factory() { return new MenuDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Menu :
		public MenuLabel
	{
	public:
		// Skin Constants
		static const Ogre::String DEFAULT;
		static const Ogre::String DOWN;
		static const Ogre::String OVER;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class ToolBar;
	public:
		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Hides Menu and any submenus.
		*/
		void closeMenu();
		/**
		* Closes any SubMenus that may be open.
		*/
		void closeSubMenus();
		/**
		* Creates a Child MenuItem.
		*/
		MenuItem* createMenuItem(const Ogre::String& className, MenuItemDesc& d);
		/**
		* Creates a Child MenuLabel.
		*/
		MenuLabel* createMenuLabel(MenuLabelDesc& d);
		/**
		* Creates a Child Menu.
		*/
		Menu* createSubMenu(MenuDesc& d);

		/**
		* Recursively searches through children and returns first widget found with name given.
		* NULL is returned if the widget is not found.
		*/
		virtual Widget* findWidget(const Ogre::String& name);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the number of MenuItems this menu has.
		*/
		int getNumberOfItems();

		/**
		* Returns true if the given menu is a submenu of this menu, false otherwise.
		* NOTE: Does not have to be a direct sub menu.
		*/
		bool hasSubMenu(Menu* m);

		/**
		* Returns true if this Menu is the child of another Menu, false otherwise.
		*/
		bool isSubMenu();

		/**
		* Displays the Menu and its MenuItems.  Menu is positioned to
		* display on screen.
		*/
		void openMenu();
		/**
		* Finds the SubMenu and opens all Menus that lead to that Menu.
		*/
		void openSubMenu(Menu* m);

		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);

	protected:
		Menu(const Ogre::String& name);
		~Menu();

		MenuDesc* mDesc;

		// Window containing child MenuItems
		MenuPanel* mMenuPanel;
		std::vector<MenuItem*> mMenuItems;

		// Immediate SubMenus
		std::vector<Menu*> mSubMenus;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);
		/**
		* Updates the Menu Link;
		*/
		virtual void notifyMenuParent(Menu* m);
		/**
		* Updates the ToolBar Link.
		*/
		virtual void notifyToolBarParent(ToolBar* b);
		/**
		* Event handler for Left Mouse Button Down event. (Show/Hide Menu List)
		*/
		virtual void onMouseButtonDown(const EventArgs& args);
		/**
		* Event handler for Mouse Button Up event.
		*/
		virtual void onMouseButtonUp(const EventArgs& args);
		/**
		* Closes all SubMenus belonging to the parent Menu, and opens this menu
		*/
		virtual void onMouseEnter(const EventArgs& args);

	private:
	};
}

#endif
