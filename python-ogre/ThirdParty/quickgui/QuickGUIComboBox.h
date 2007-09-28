#ifndef QUICKGUICOMBOBOX_H
#define QUICKGUICOMBOBOX_H

#include "QuickGUIButton.h"
#include "QuickGUILabel.h"
#include "QuickGUIList.h"

#include <vector>

namespace QuickGUI
{
	/** Represents a traditional ComboBox.
	@remarks
	The ComboBox class requires at least 3 materials to define it's image:
	Normal State, Mouse Over, and Mouse Down.  For example, if you pass
	the constructor "sample.combobox" as its arguement for the material,
	the class will expect "sample.combobox.over" and "sample.combobox.down"
	to exist.  The ComboBox supplies a list of items from which the user
	can choose.
	@note
	In order to get the most use out of ComboBox, you will need to add
	ListItems.
	@note
	ComboBoxes are meant to be created via the Window widget.
	*/
	class _QuickGUIExport ComboBox :
		public Label
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
		ComboBox(const Ogre::String& name, Type type, const Rect& pixelDimensions, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm);

		/*
		* Event Handler tied to ListItems, causing them to hide lists on mouse button up.
		*/
		void addDefaultListItemHandler(const EventArgs& args);

		/**
		* Add user defined events, that will be called when a selection is made.
		*/
		template<typename T> void addOnSelectionEventHandler(bool (T::*function)(const EventArgs&), T* obj)
		{
			mOnSelectUserEventHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void addOnSelectionEventHandler(MemberFunctionSlot* function);

		/**
		* Event Handler tied to ComboBox button.
		*/
		void applyButtonDownImage(const EventArgs& args);

		List* getDropDownList();

		/**
		* Event Handler for the EVENT_LOSE_FOCUS event.
		*/
		void onLoseFocus(const EventArgs& args);

		/**
		* Event Handler for the EVENT_MOUSE_ENTER event.
		*/
		void onMouseEnters(const EventArgs& args);
		/**
		* Default Handler for the EVENT_MOUSE_LEAVE event.
		*/
		void onMouseLeaves(const EventArgs& args);
		/**
		* Default Handler for the EVENT_MOUSE_BUTTON_DOWN event.
		*/
		void onMouseButtonDown(const EventArgs& args);
		/**
		* Default Handler for the EVENT_MOUSE_BUTTON_UP event.
		*/
		void onMouseButtonUp(const EventArgs& args);
		/**
		* Specific Handler for the ComboBox Widget.  Called when the user selects a ListItem
		*/
		void onSelection(const EventArgs& args);
		/**
		* Event Handler tied to the combobox button, for opening and closing the drop down list.
		*/
		void toggleDropDownListVisibility(const EventArgs& args);

	protected:
		virtual ~ComboBox();

		// Button that shows the drop down list.
		Button* mButton;
		// Drop down list.
		List* mList;

		// User defined event handlers that are called when a Selection is made.
		std::vector<MemberFunctionSlot*> mOnSelectUserEventHandlers;
	};
}

#endif
