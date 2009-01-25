#ifndef QUICKGUIMENU_H
#define QUICKGUIMENU_H

#include "QuickGUIException.h"
#include "QuickGUIMenuLabel.h"

#include <vector>

namespace QuickGUI
{
	// forward declarations
	class MenuPanel;

	class _QuickGUIExport MenuDesc :
			public MenuLabelDesc
	{
	public:
		friend class Menu;
		friend class ToolBar;
		template<typename BaseClassType>
		friend class Factory;
	protected:
		MenuDesc(const Ogre::String& id);
		virtual ~MenuDesc() {}
	public:

		float			menu_itemHeight;
		float			menu_maxMenuHeight;
		float			menu_menuWidth;
		/// Sets the amount of pixel overlap sub elements are drawn over this Menu.
		float			menu_subMenuOverlap;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "MenuDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Menu"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

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
		static const Ogre::String MENUPANEL;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class ToolBar;
		template<typename BaseClassType>
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);
		/**
		* Notifies the Widget of its GUIManager. (for component/container widgets this function is recursive)
		*/
		virtual void _setGUIManager(GUIManager* gm);
		/**
		* Notifies the Widget of its Sheet. (for component/container widgets this function is recursvie)
		*/
		virtual void _setSheet(Sheet* sheet);

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
		MenuItem* createMenuItem(const Ogre::String& className, MenuItemDesc* d);
		/**
		* Creates a Child MenuLabel.
		*/
		MenuLabel* createMenuLabel(MenuLabelDesc* d);
		/**
		* Creates a Child Menu.
		*/
		Menu* createSubMenu(MenuDesc* d);

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
		* Gets the height of each ListItem within this List.
		*/
		float getItemHeight();
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
		/**
		* Sets the height of each ListItem within the List.
		*/
		void setItemHeight(float height);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);

	protected:
		Menu(const Ogre::String& name);
		virtual ~Menu();

		MenuDesc* mDesc;

		// Window containing child MenuItems
		MenuPanel* mMenuPanel;
		std::vector<MenuItem*> mMenuItems;

		// Immediate SubMenus
		std::vector<Menu*> mSubMenus;

		float _getNextAvailableYPosition();
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

		/**
		* Iterates through items and updates their position.
		*/
		void _updateItemPositions();

	private:
	};
}

#endif
