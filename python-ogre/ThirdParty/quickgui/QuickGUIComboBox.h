#ifndef QUICKGUICOMBOBOX_H
#define QUICKGUICOMBOBOX_H

#include "QuickGUIButton.h"
#include "QuickGUIImage.h"
#include "QuickGUIMenuLabel.h"
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
		ComboBox(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String textureName, GUIManager* gm);

		MenuLabel* addItem();
		/**
		* Add user defined events, that will be called when a selection is made.
		*/
		template<typename T> void addOnSelectionEventHandler(bool (T::*function)(const EventArgs&), T* obj)
		{
			mOnSelectUserEventHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void addOnSelectionEventHandler(MemberFunctionSlot* function);

		void clearDropDownList();
		void clearSelection();

		int getItemIndex(MenuLabel* l);
		int getNumberOfItems();
		MenuLabel* getSelectedItem();
		int getSelectedItemIndex();
		int getVerticalPixelPadHeight();
		
		void selectItem(unsigned int index);
		void setDropDownHeight(Ogre::Real pixelHeight);
		void setDropDownWidth(Ogre::Real pixelWidth);
		virtual void setFont(const Ogre::String& fontScriptName, bool recursive = false);
		virtual void setHeight(Ogre::Real pixelHeight);
		/**
		* Sets the material displayed when the mouse moves over a list item.
		*/
		void setHighlightTexture(const Ogre::String& texture);
		void setRightToLeft(bool rightToLeft);
		/**
		* Manually set size of widget.
		*/
		virtual void setSize(const Ogre::Real& pixelWidth, const Ogre::Real& pixelHeight);
		virtual void setSize(const Size& pixelSize);
		void setVerticalPixelPadHeight(unsigned int height);

	protected:
		virtual ~ComboBox();

		Text* mTextUtilities;
		bool mAutoSize;
		int mVPixelPadHeight;

		/**
		* Manually set a ListItem to be highlighted.
		*/
		void highlightListItem(MenuLabel* l);

		void selectItem(MenuLabel* l);

		void hideDropDownList(const EventArgs& args);
		void onLoseFocus(const EventArgs& args);
		void onMouseButtonUp(const EventArgs& args);
		void onMouseEnters(const EventArgs& args);
		void onMouseLeaves(const EventArgs& args);
		/**
		* Specific Handler for the ComboBox Widget.  Called when the user selects a ListItem
		*/
		void onSelection(const EventArgs& args);

		Quad* mHighlightPanel;
		Ogre::String mHighlightTexture;
		MenuLabel* mHighlightedItem;

		// Button that shows the drop down list.
		Button* mButton;
		// Imitates the selected item.
		MenuLabel* mMenuLabel;
		// Drop down list.
		List* mList;

		Ogre::Real mDropDownHeight;
		Ogre::Real mDropDownWidth;

		bool mRightToLeft;

		// User defined event handlers that are called when a Selection is made.
		std::vector<MemberFunctionSlot*> mOnSelectUserEventHandlers;
	};
}

#endif
