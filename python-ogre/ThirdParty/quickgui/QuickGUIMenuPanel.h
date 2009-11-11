/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

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
		friend class DescFactory;
	//protected:
		MenuPanelDesc();
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
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Adds a Widget to MenuPanel.
		*/
		void addWidget(Widget* w);

		/**
		* Destroys a Widget from this MenuPanel.
		* NOTE: The widget is pushed onto a free list and deleted on next update (draw)
		*/
		void destroyWidget(Widget* w);
		/**
		* Destroys all Widgets from MenuPanel.
		* NOTE: The widgets are pushed onto a free list and deleted on next update (draw)
		*/
		void destroyWidgets();

		/**
		* Returns the opacity of this widget as it is drawn on screen.
		* NOTE: Includes parent opacity if widget_inheritOpacity is true.
		*/
		virtual float getAbsoluteOpacity();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the width of the Left and Right borders (added together).
		*/
		float getLeftRightBorderWidth();
		/**
		* Returns the owner of the MenuPanel.
		*/
		Widget* getOwner();

		/**
		* Returns true if this widget is a child of the widget given.
		*/
		virtual bool isChildOf(Widget* w);

		/**
		* Removes a Widget from this MenuPanel.
		*/
		void removeWidget(Widget* w);
		/**
		* Removes all Widgets from MenuPanel.
		*/
		void removeWidgets();

		/**
		* Sets the max height of the MenuPanel.
		*/
		void setMaxHeight(float height);

		/**
		* Recalculate Client widget_dimensions, relative to Widget's actual widget_dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Virtual dimensions, the minimum size required to encapsulate the client area and all Child widgets.
		*/
		virtual void updateVirtualDimensions();

	//protected:
		MenuPanel(const Ogre::String& name);
		virtual ~MenuPanel();

		MenuPanelDesc* mDesc;

		void _adjustHeight();

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
