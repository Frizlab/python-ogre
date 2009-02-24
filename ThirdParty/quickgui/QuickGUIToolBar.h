#ifndef QUICKGUITOOLBAR_H
#define QUICKGUITOOLBAR_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUIMenu.h"
#include "QuickGUIToolBarEnums.h"
#include "QuickGUIToolBarItem.h"

#include <vector>

namespace QuickGUI
{
	class _QuickGUIExport ToolBarDesc :
			public ContainerWidgetDesc
	{
	public:
		template<typename BaseClassType>
		friend class Factory;
	//protected:
		ToolBarDesc(const Ogre::String& id);
		virtual ~ToolBarDesc() {}
	public:

		ToolBarItemLayout toolbar_itemLayout;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ToolBarDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ToolBar"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ToolBar :
		public ContainerWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		template<typename BaseClassType>
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Closes all Menus that belong to this ToolBar.
		*/
		void closeMenus();
		/**
		* Creates a Menu.  Position and orientation depends on
		* toolbar_itemLayout and ToolBar orientation.
		*/
		Menu* createMenu(MenuDesc* d);
		/**
		* Creates a ToolBar item.  Position and orientation depends on
		* toolbar_itemLayout and ToolBar orientation.
		*/
		ToolBarItem* createToolBarItem(const Ogre::String& className, ToolBarItemDesc* d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the number of ToolBarItems in the ToolBar.
		*/
		int getNumberOfItems();
		/**
		* Get the orientation of the ToolBar.
		*/
		ToolBarOrientation getOrientation();

		/**
		* Returns true if a menu is open, false otherwise.
		*/
		bool isMenuOpened();

		/**
		* Opens the Menu and any Menus that are parents of the given menu.
		*/
		void openMenu(Menu* m);

	//protected:
		ToolBar(const Ogre::String& name);
		virtual ~ToolBar();

		ToolBarOrientation mOrientation;
		ToolBarState mCurrentState;

		ToolBarDesc* mDesc;

		std::vector<Menu*> mMenus;
		bool mMenuOpened;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);
		/**
		* Gets the position for the newly added Item.  Position is dependent
		* on orientation and Item Layout.
		*/
		Point getNextAvailableItemPosition(ToolBarItem* newlyAddedItem);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
