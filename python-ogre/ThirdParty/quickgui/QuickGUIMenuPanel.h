#ifndef QUICKGUIMENUPANEL_H
#define QUICKGUIMENUPANEL_H

#include "QuickGUIWindow.h"

namespace QuickGUI
{
	// forward declarations
	class ListItem;

	class _QuickGUIExport MenuPanelDesc :
			public WindowDesc
	{
	public:
		template<typename BaseClassType>
		friend class Factory;
	//protected:
		MenuPanelDesc(const Ogre::String& id);
		virtual ~MenuPanelDesc() {}
	public:

		Widget* menupanel_owner;
		float	menupanel_maxHeight;

		virtual Ogre::String getClass() { return "MenuPanelDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "MenuPanel"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

	//protected:
		bool window_titleBar;
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
		// ComboBox class creates and destroys MenuPanel
		friend class ComboBox;
	public:
		template<typename BaseClassType>
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Adds a ListItem to this Widget.
		*/
		void addWidget(Widget* w);

		/**
		* Clears all ListItems.
		*/
		void clearWidgets();

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the owner of the MenuPanel.
		*/
		Widget* getOwner();

		/**
		* Removes a ListItem from this widget;
		*/
		void removeWidget(Widget* w);

		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Sets the max height of the MenuPanel.
		*/
		void setMaxHeight(float height);

	//protected:
		MenuPanel(const Ogre::String& name);
		virtual ~MenuPanel();

		MenuPanelDesc* mDesc;

		void _adjustHeight();

		/**
		* Forcing all ListItems to be the width of client dimensions.
		*/
		void onClientSizeChanged(const EventArgs& args);
		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		void onLoseFocus(const EventArgs& args);
		void onVisibleChanged(const EventArgs& args);

	private:
	};
}

#endif
