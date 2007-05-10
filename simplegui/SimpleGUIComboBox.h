#ifndef SIMPLEGUICOMBOBOX_H
#define SIMPLEGUICOMBOBOX_H

#include "SimpleGUIButton.h"
#include "SimpleGUILabel.h"
#include "SimpleGUIList.h"
#include "SimpleGUIPrerequisites.h"

#include <vector>

namespace SimpleGUI
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
	class ComboBox :
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
		ComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, const Ogre::String& highlightMaterial, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~ComboBox();

		/**
		* Internal method that converts relative dimensions to absolute and 
		* pixel dimensions, and calls _applyDimensions().  Also notifies
		* all child widgets that the dimensions have changed.
		*/
		virtual void _notifyDimensionsChanged();

		void addListItem(const Ogre::String& text);
		template<typename T> void addOnSelectionEventHandler(bool (T::*function)(const EventArgs&), T* obj)
		{
			mOnSelectUserEventHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void alignListItemText(HorizontalAlignment ha, VerticalAlignment va);

		int getNumberOfListItems();
		Widget* getTargetWidget(const Ogre::Vector2& p);

		// Overridden Event Handling functions
		// If user Defined Events have been created, they will be called.
		/**
		* Default Handler for the SGUI_ACTIVATED event, and activates all child widgets (if exist)
		*/
		virtual void deactivate(EventArgs& e);
		/**
		* Default Handler for the SGUI_MOUSE_ENTER event.  material.over is applied to the button
		* image. If not handled, it will be passed to the parent widget 
		* (if exists)
		*/
		bool onMouseEnters(MouseEventArgs& e);
		/**
		* Default Handler for the SGUI_MOUSE_LEAVE event.  material is applied to the button
		* image. If not handled, it will be passed to the parent widget 
		* (if exists)
		*/
		bool onMouseLeaves(MouseEventArgs& e);
		/**
		* Default Handler for the SGUI_MOUSE_BUTTON_DOWN event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonDown(MouseEventArgs& e);
		/**
		* Default Handler for the SGUI_MOUSE_BUTTON_UP event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonUp(MouseEventArgs& e);
		/**
		* Specific Handler for the ComboBox Widget.  Called when the user selects a ListItem
		*/
		bool onSelection(WidgetEventArgs& e);

		void removeListItem(unsigned int index);

		void setCharacterHeight(const Ogre::Real& relativeHeight);
		void setText(const Ogre::String& text);
		void setDropListWidth(const Ogre::Real& relativeWidth);

		void timeElapsed(Ogre::Real time);

	protected:
		Label* mLabel;
		Button* mButton;
		List* mList;

		HorizontalAlignment mListItemHorizontalAlignment;
		VerticalAlignment mListItemVerticalAlignment;

		std::vector<MemberFunctionSlot*> mOnSelectUserEventHandlers;
	};
}

#endif
