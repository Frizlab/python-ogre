#ifndef SIMPLEGUIMENU_H
#define SIMPLEGUIMENU_H

#include "SimpleGUIButton.h"
#include "SimpleGUILabel.h"
#include "SimpleGUIList.h"
#include "SimpleGUIPrerequisites.h"

#include "SimpleGUIComboBox.h"

#include <utility>
#include <vector>

namespace SimpleGUI
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
				show toggle visibility.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		Menu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		~Menu();

		List* addMenuList(const Ogre::String& anchorText, const Ogre::Vector4& anchorRelativeDimensions, const Ogre::String& anchorMaterial, const Ogre::Real& relativeMenuListWidth, const Ogre::String& MenuListMaterial, Ogre::String highlightMaterial = "");

		void clearAllMenuLists();

		void deactivate(EventArgs& e);

		bool evtHndlr_hideMenuList(const EventArgs& e);
		/**
		* Called whenever a child list creates a child list item.
		* Used to add MouseEnter and MouseLeave event handlers to List Item, 
		* for selection highlighting.
		*/
// 		bool evtHndlr_listItemCreated(const EventArgs& e);
// 		bool evtHndlr_listItemMouseEnters(const EventArgs& e);
// 		bool evtHndlr_listItemMouseLeaves(const EventArgs& e);
		bool evtHndlr_toggleMenuList(const EventArgs& e);

		Widget* getTargetWidget(const Ogre::Vector2& p);

		void hide();
		void hideMenus();

		bool onMouseButtonUp(MouseEventArgs& e);
		bool onMouseMoved(MouseEventArgs& e);

		void show();

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
