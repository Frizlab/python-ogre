#ifndef QUICKGUIMENULIST_H
#define QUICKGUIMENULIST_H

#include "QuickGUIList.h"
#include "QuickGUIButton.h"

namespace QuickGUI
{
	/** Represents a MenuStrip.
		@remarks
		A button widget, with list.  When pressed, the
		list is shown.
		@note
		List Height depends on the height of its combined ListItems.
		@note
		MenuLists are created from the Menu.
	*/
	class _QuickGUIExport MenuList :
		public Button
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
				material Ogre material defining the widget image. ex. "qgui.menulist"
			@param
				group QuadContainer containing this widget.
			@param
				ParentWidget parent widget which created this widget.
        */
		MenuList(const Ogre::String& name, const Rect& pixelDimensions, Ogre::String texture, GUIManager* gm);

		/*
		* Event Handler tied to ListItems, causing them to hide lists on mouse button up.
		*/
		void addDefaultListItemHandler(const EventArgs& args);

		/**
		* Adds (and creates) a ListItem and adds it to the List.
		* Note that a pointer to the created List Item is returned.
		* ListItem's support an Image Widget and NStateButton Widget.
		*/
		ListItem* addListItem(const Ogre::UTFString& text);
		ListItem* addListItem(const Ogre::String& name, const Ogre::UTFString& text);

		/*
		* Event Handler tied to ListItems, causing them to hide lists on mouse button up.
		*/
		void hideMenuList(const EventArgs& e);

		/**
		* When user clicks outside of MenuList Button, parent Menu is notified, and hides all
		* MenuList widgets.
		*/
		void onLoseFocus(const EventArgs& args);

		List* getList();
		
	protected:
		virtual ~MenuList();

		List* mList;
		Ogre::String mButtonTexture;
		Ogre::String mListTexture;
	};
}

#endif
