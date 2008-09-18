#ifndef QUICKGUIMENUITEM_H
#define QUICKGUIMENUITEM_H

#include "QuickGUIToolBarItem.h"

namespace QuickGUI
{
	// forward declaration
	class Menu;

	class _QuickGUIExport MenuItemDesc :
			public ToolBarItemDesc
	{
	public:
		MenuItemDesc();

		Menu* menu;

	protected:
		/// For Horizontally oriented ToolBars, this is the width of the Widget.
		/// For Vertically oriented ToolBars, this is the height of the widget.
		float length;
	};

	class _QuickGUIExport MenuItem :
		public ToolBarItem
	{
	public:
		friend class ToolBar;
		friend class Menu;
		friend class MenuPanel;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the Menu this MenuItem belongs to.
		* NOTE: This MenuItem could be a Menu, and if the Menu is directly connected to a ToolBar,
		* its parent Menu will be itself.
		*/
		Menu* getMenu();

		/**
		* Returns the ToolBar this MenuItem belongs to.
		*/
		ToolBar* getToolBar();
		
		/**
		* Returns true if this widget is a menu item, false otherwise.
		*/
		virtual bool isMenuItem();

	protected:
		MenuItem(const Ogre::String& name);
		~MenuItem();

		/**
		* Updates the Menu Link;
		*/
		virtual void notifyMenuParent(Menu* m);

		/**
		* Updates the ToolBar Link.
		*/
		virtual void notifyToolBarParent(ToolBar* b);

	private:
	};
}

#endif
