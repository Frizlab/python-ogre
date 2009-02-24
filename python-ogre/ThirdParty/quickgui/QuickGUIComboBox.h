#ifndef QUICKGUICOMBOBOX_H
#define QUICKGUICOMBOBOX_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUIListTextItem.h"

namespace QuickGUI
{
	// forward declarations
	class MenuPanel;
	class Button;

	class _QuickGUIExport ComboBoxDesc :
		public ContainerWidgetDesc
	{
	public:
		template<typename BaseClassType>
		friend class Factory;
	//protected:
		ComboBoxDesc(const Ogre::String& id);
		virtual ~ComboBoxDesc() {}
	public:

		bool					combobox_dropDownButton;
		float					combobox_itemHeight;
		float					combobox_dropDownWidth;
		// A value of 0 means there is no Max Height.
		float					combobox_dropDownMaxHeight;
		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment	combobox_verticalTextAlignment;

		/// Describes the Text used in this Label
		TextDesc				textDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ComboBoxDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ComboBox"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ComboBox :
		public ContainerWidget
	{
	public:
		friend class ListItem;

		template<typename BaseClassType>
		friend class WidgetFactory;
	public:
		// Skin Constants
		static const Ogre::String DEFAULT;
		static const Ogre::String DOWN;
		static const Ogre::String OVER;
		static const Ogre::String DROPDOWNBUTTON;
		static const Ogre::String DROPDOWNMENUPANEL;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		template<typename BaseClassType>
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);
		/**
		* Notifies the Widget of its GUIManager. (for component/container widgets this function is recursive)
		*/
		virtual void _setGUIManager(GUIManager* gm);
		/**
		* Notifies the Widget of its Sheet. (for component/container widgets this function is recursvie)
		*/
		virtual void _setSheet(Sheet* sheet);

		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: BUTTON_EVENT_STATE_CHANGED, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::BUTTON_EVENT_STATE_CHANGED,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addComboBoxEventHandler(ComboBoxEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mComboBoxEventHandlers[EVENT].push_back(OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addComboBoxEventHandler(ComboBoxEvent EVENT, EventHandlerSlot* function);

		/**
		* Removes all ListItems from the list.
		*/
		void clearItems();
		/**
		* Clears ComboBox Selection.
		*/
		void clearSelection();
		/**
		* Adds a ListItem to the end of the DropDownList.
		* NOTE: Names are auto-generated in the form of <ComboBoxName>+<Index>.
		*/
		ListTextItem* createItem(ListTextItemDesc* d);

		/**
		* Removes an Item from the List.
		*/
		void destroyItem(unsigned int index);

		/**
		* Recursively searches through children and returns first widget found with name given.
		* NULL is returned if the widget is not found.
		*/
		Widget* findWidget(const Ogre::String& widget_name);
		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireComboBoxEvent(ComboBoxEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the Item at the index given.  If the index is invalid,
		* NULL is returned.
		*/
		ListTextItem* getItem(unsigned int index);
		/**
		* Gets the height of each ListItem within this List.
		*/
		float getItemHeight();
		/**
		* Gets the number of ListItems within this List.
		*/
		int getNumberOfItems();
		/**
		* Returns a pointer to a list of selected items.
		*/
		ListTextItem* getSelectedItem();
		/**
		* Gets the text in UTFString form.
		*/
		Ogre::UTFString getText();

		/**
		* Hides Drop Down List.
		*/
		void hideDropDownList();

		/**
		* Selects the item at index given.  If index is not valid,
		* all items will be de-selected.
		*/
		void selectItem(unsigned int index);
		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Sets all characters of the text to the specified color.
		*/
		void setColor(const Ogre::ColourValue& cv);
		/**
		* Sets the character at the index given to the specified color.
		*/
		void setColor(const Ogre::ColourValue& cv, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified color.
		*/
		void setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets the width of the Drop down MenuPanel.
		*/
		void setDropDownWidth(float width);
		/**
		* Sets all characters of the text to the specified font.
		*/
		void setFont(const Ogre::String& fontName);
		/**
		* Sets the character at the index given to the specified font.
		*/
		void setFont(const Ogre::String& fontName, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified font.
		*/
		void setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets the height of each ListItem within the List.
		*/
		void setItemHeight(float height);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s);
		/**
		* Sets the Text using Text Segments.
		*/
		void setText(std::vector<TextSegment> segments);
		/**
		* Shows the Drop Down List.
		*/
		void showDropDownList();

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

	//protected:
		ComboBox(const Ogre::String& name);
		virtual ~ComboBox();

		ComboBoxDesc* mDesc;

		Button* mDropDownButton;

		// Window containing child ListItems
		MenuPanel* mMenuPanel;

		std::list<ListTextItem*> mItems;
		ListTextItem* mSelectedItem;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mComboBoxEventHandlers[COMBOBOX_EVENT_COUNT];

		// Current settings applied to newly added text.
		Ogre::String mCurrentFontName;
		Ogre::ColourValue mCurrentColourValue;

		Text* mText;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
		/**
		* Event handler for Left Mouse Button Down event. (Show/Hide Drop Down List)
		*/
		virtual void onMouseButtonDown(const EventArgs& args);
		/**
		* Event handler for Left Mouse Button Down on Drop Down Button. (Show/Hide Drop Down List)
		*/
		void onDropDownButtonDown(const EventArgs& args);
		void onMouseEnter(const EventArgs& args);
		void onMouseLeave(const EventArgs& args);

		/**
		* Function allowing ListItems to notify List when user left clicks on them.
		*/
		void selectItem(const MouseEventArgs& mea);

		/**
		* Clears selection list by de-selecting all selected units and clearing list.
		*/
		void _clearSelection();
		/**
		* Iterates through list items and returns the y position of where the next menu item would go.
		*/
		float _getNextAvailableYPosition();
		/**
		* Iterates through items and updates their position.
		*/
		void _updateItemPositions();

		/**
		* ListItem names are in the format of <ListName>+<index>, and should always be accurate.
		*/
		void updateItemNamesAndIndices();
	};
}

#endif
