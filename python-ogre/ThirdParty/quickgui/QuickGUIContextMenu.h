#ifndef QUICKGUICONTEXTMENU_H
#define QUICKGUICONTEXTMENU_H

#include "QuickGUIException.h"
#include "QuickGUIMenuItem.h"
#include "QuickGUIWindow.h"

#include <vector>

namespace QuickGUI
{
	class _QuickGUIExport ContextMenuDesc :
			public WindowDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		ContextMenuDesc(const Ogre::String& id);
		virtual ~ContextMenuDesc() {}
	public:

		float			contextmenu_itemHeight;
		float			contextmenu_maxMenuHeight;
		/// Sets the amount of pixel overlap sub elements are drawn over this Menu.
		float			contextmenu_subMenuOverlap;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ContextMenuDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ContextMenu"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ContextMenu :
		public Window
	{
	public:
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class Factory<Widget>;
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
		* Hides the Menu and any visible SubMenus.
		*/
		void hide();

		/**
		* Displays the Menu and its MenuItems.  Menu is positioned to
		* display on screen.
		*/
		void openMenu(const Point& position);
		/**
		* Finds the SubMenu and opens all Menus that lead to that Menu.
		*/
		void openSubMenu(Menu* m);

		/**
		* Sets the size and position (position relative to parent) of this Widget, respectively.
		*/
		virtual void setDimensions(const Rect& r);
		/**
		* Sets the height of each ListItem within the List.
		*/
		void setItemHeight(float height);
		/**
		* Sets the width and height of this widget.
		*/
		virtual void setSize(const Size& size);
		/**
		* Shows the Menu at the specified location.  Menu may be repositioned to
		* display on screen.
		*/
		void show(const Point& position);

	protected:
		ContextMenu(const Ogre::String& name);
		virtual ~ContextMenu();

		ContextMenuDesc* mDesc;

		int mAutoNameCounter;

		// Access to child MenuItems
		std::vector<MenuItem*> mMenuItems;

		// Immediate SubMenus
		std::vector<Menu*> mSubMenus;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		float _getNextAvailableYPosition();

		/**
		* Iterates through items and updates their position.
		*/
		void _updateItemPositions();

		/**
		* Forcing all MenuItems to be the width of client dimensions.
		*/
		void onClientSizeChanged(const EventArgs& args);
		void onLoseFocus(const EventArgs& args);
		void onPositionChanged(const EventArgs& args);
		void onVisibleChanged(const EventArgs& args);

		/**
		* Sets the height of the widget.
		*/
		virtual void setHeight(float pixelHeight);

	private:
	};
}

#endif
