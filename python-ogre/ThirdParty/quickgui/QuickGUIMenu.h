#ifndef QUICKGUIMENU_H
#define QUICKGUIMENU_H

#include "QuickGUIButton.h"
#include "QuickGUILabel.h"
#include "QuickGUIList.h"
#include "QuickGUIPrerequisites.h"

#include "QuickGUIComboBox.h"

#include <utility>
#include <vector>

namespace QuickGUI
{
	/** Represents a Menu.
		@remarks
		Menus are empty be default.  They do not become useful
		until adding MenuStrip widgets, which contain ListItem widgets.
		@note
		Menus must be created by the Window widget.
		@note
		Menus do not contain any text.
	*/
	class Menu :
		public Label
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		Menu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~Menu();

		/**
		* Internal method that converts relative dimensions to absolute and 
		* pixel dimensions, and calls _applyDimensions().  Also notifies
		* all child widgets that the dimensions have changed.
		*/
		void _notifyDimensionsChanged();
		// Notify Widget of it's Window's zOrder, and it's own zOrder offset.
		void _notifyZOrder(Ogre::ushort windowZOrder, Ogre::ushort offsetZOrder);
		// Notify Widget that Window's zOrder has changed. Updates child widgets also.
		void _notifyZOrder(Ogre::ushort windowZOrder);

		/**
		* Adds a menu anchor (button) and List Widget to the menu.
		*/
		List* addMenuList(const Ogre::UTFString& anchorText, const Ogre::Vector4& anchorRelativeDimensions, const Ogre::String& anchorMaterial, const Ogre::Real& relativeMenuListWidth, const Ogre::String& MenuListMaterial, Ogre::String highlightMaterial);
		/**
		* Adds a menu anchor (button) and List Widget to the menu.
		* Default Skin materials are applied to both anchor and list.
		*/
		List* addMenuList(const Ogre::UTFString& anchorText, const Ogre::Vector4& anchorRelativeDimensions, const Ogre::Real& relativeMenuListWidth);

		/**
		* Removes and Destroys all Child Lists, and their corresponding Anchor Button.
		*/
		void clearAllMenuLists();

		/**
		* Default Event Handler called when widget is deactivated.
		*/
		void deactivate(EventArgs& e);

		/**
		* Event Handler used to hide all child Lists, when a ListItem is clicked.
		*/
		bool evtHndlr_hideMenuList(const EventArgs& e);
		/**
		* Called whenever a child list creates a child list item.
		* Used to add MouseEnter and MouseLeave event handlers to List Item, 
		* for selection highlighting.
		*/
		bool evtHndlr_listItemCreated(const EventArgs& e);
		bool evtHndlr_listItemMouseEnters(const EventArgs& e);
		bool evtHndlr_listItemMouseLeaves(const EventArgs& e);
		bool evtHndlr_toggleMenuList(const EventArgs& e);

		/**
		* Checks if child Lists, ListItems, Anchor Buttons or Menu is *hit*.
		*/
		Widget* getTargetWidget(const Ogre::Vector2& p);

		/**
		* Hides the widget, including any children widgets.
		*/
		void hide();
		/**
		* Hides All Child Menus.
		*/
		void hideMenus();

		/**
		* Default Handler for the QGUI_EVENT_MOUSE_BUTTON_UP event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonUp(MouseEventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_MOVE event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseMoved(MouseEventArgs& e);

		/**
		* Shows the widget, including any child widgets.
		*/
		void show();

		/**
		* Default Handler for injecting Time.
		*/
		void timeElapsed(Ogre::Real time);

	protected:
		std::vector<Button*> mMenuListAnchors;
		std::vector<List*> mMenuLists;

		// If user clicks on a menu, it drops down and sets this variable to true;
		// Mousing over other menu anchor buttons will switch to their menus. (Windows XP menu functionality)
		bool mShowMenus;

		int mMenuListCounter;
	};
}

#endif
