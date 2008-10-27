#ifndef QUICKGUIMENUPANEL_H
#define QUICKGUIMENUPANEL_H

#include "QuickGUIWindow.h"

namespace QuickGUI
{
	// forward declarations
	class MenuItem;

	class _QuickGUIExport MenuPanelDesc :
			public WindowDesc
	{
	public:
		MenuPanelDesc();

		virtual Ogre::String getClass() { return "MenuPanelDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "MenuPanel"; }

		// Factory method
		static WidgetDesc* factory() { return new MenuPanelDesc(); }

	protected:
		bool titleBar;
	};

	class _QuickGUIExport MenuPanel :
		public Window
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// Menu class creates and destroys MenuPanel
		friend class Menu;
	public:
		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Adds a MenuItem to this Widget.
		*/
		void addMenuItem(MenuItem* i);

		/**
		* Recursively searches through children and returns first widget found with name given.
		* NULL is returned if the widget is not found.
		*/
		virtual Widget* findWidget(const Ogre::String& name);
		/**
		* Checks if point p is within this widget's dimensions.
		* NULL is returned if the point is outside dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, bool ignoreDisabled = true);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Iterates through menu items and returns the y position of where the next menu item would go.
		*/
		float getNextAvailableYPosition();

		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);

		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

	protected:
		MenuPanel(const Ogre::String& name);
		virtual ~MenuPanel();

		// Windows can be used as a MenuList, but the MenuItems shouldn't be actual
		// children of the window, so we track them by this list.
		std::vector<MenuItem*> mMenuItems;

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();

		/**
		* Forcing all ListItems to be the width of client dimensions.
		*/
		void onClientSizeChanged(const EventArgs& args);
		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
		/**
		* Similar to ContainerWidget::draw, except also considers MenuItem List.
		*/
		void _draw();
	};
}

#endif
