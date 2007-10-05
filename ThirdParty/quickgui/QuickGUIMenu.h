#ifndef QUICKGUIMENU_H
#define QUICKGUIMENU_H

#include "QuickGUIImage.h"
#include "QuickGUIMenuList.h"

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
	class _QuickGUIExport Menu :
		public Image
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x Position, y Position, width, and height of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				material Ogre material defining the widget image.
			@param
				group QuadContainer containing this widget.
			@param
				ParentWidget parent widget which created this widget.
        */
		Menu(const Ogre::String& name, const Rect& pixelDimensions, Ogre::String texture, GUIManager* gm);

		/**
		* Adds a menu list.
		*/
		MenuList* addMenuList(const Ogre::String& name, const Ogre::UTFString& text, Ogre::Real pixelX, Ogre::Real pixelWidth, const Ogre::String& texture);
		/**
		* Adds a menu list.  Name is generated.
		*/
		MenuList* addMenuList(const Ogre::UTFString& text, Ogre::Real pixelX, Ogre::Real pixelWidth, const Ogre::String& texture);
		/**
		* Adds a menu list.  Name is generated. Default Material applied.
		*/
		MenuList* addMenuList(const Ogre::UTFString& text, Ogre::Real pixelX, Ogre::Real pixelWidth);

		/**
		* Removes and Destroys all Child Lists, and their corresponding Anchor Button.
		*/
		void clearAllMenuLists();

		/**
		* Gets a List from the Menu.  No exception is thrown
		* if the index is out of bounds.
		*/
		MenuList* getMenuList(unsigned int index);
		MenuList* getMenuList(const Ogre::String& name);

		/**
		* Hides All Child Menus.
		*/
		void hideMenuLists();

		/**
		* Removes true if point is over a MenuList Button, false otherwise.
		*/
		bool pointOverMenuListButton(Point pixelPosition);

		/**
		* When set to true, mousing over MenuLists will open them.
		*/
		void setShowMenuState(bool show);

		/**
		* Event Handler tied to MenuLists to determine if a MenuList should become visible
		* when mouse is over it.
		*/
		void showMenuList(const EventArgs& e);

		/**
		* Event Handler tied to MenuLists to open a menuList when clicked, and update Menu State.
		*/
		void toggleMenuList(const EventArgs& e);

	protected:
		virtual ~Menu();
		virtual void setParent(Widget* parent);
	protected:

		std::vector<MenuList*> mMenuLists;

		MenuList* mCurrentOpenList;

		// If user clicks on a menu, it drops down and sets this variable to true;
		// Mousing over other menu anchor buttons will switch to their menus. (Windows XP menu functionality)
		bool mShowMenus;

		int mMenuListCounter;
	};
}

#endif
