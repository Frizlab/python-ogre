#ifndef QUICKGUILIST_H
#define QUICKGUILIST_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUIListItem.h"
#include "QuickGUIListTextItem.h"

namespace QuickGUI
{
	class _QuickGUIExport ListDesc :
		public ContainerWidgetDesc
	{
	public:
		ListDesc();

		float listItemHeight;
		bool supportMultiSelect;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ListDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "List"; }

		// Factory method
		static WidgetDesc* factory() { return new ListDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport List :
		public ContainerWidget
	{
	public:
		friend class ListItem;
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

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
		template<typename T> void addListEventHandler(ListEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mListEventHandlers[EVENT].push_back(new EventHandlerPointer<T>(function,obj));
		}
		void addListEventHandler(ListEvent EVENT, EventHandlerSlot* function);

		/**
		* Adds a ListItem to the end of the List.
		*/
		ListItem* addItem(ListItemDesc& d);
		/**
		* Adds a ListItem to a position in the List.
		* NOTE: if the index is not valid, the ListItem is added to the end of the list.
		*/
		ListItem* addItem(int index, ListItemDesc& d);

		/**
		* De-selects any selected ListItems in the list.
		*/
		void clearSelection();

		/**
		* De-selects the item at the index given.  If index is invalid, nothing happens.
		*/
		void deselectItem(unsigned int index);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireListEvent(ListEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the ListItem at the index given.  If the index is invalid,
		* NULL is returned.
		*/
		ListItem* getItem(unsigned int index);
		/**
		* Gets the height of each ListItem within this List.
		*/
		float getListItemHeight();
		/**
		* Returns true if multiple items can be selected simultaneously, false otherwise.
		*/
		bool getMultiSelect();
		/**
		* Gets the number of ListItems within this List.
		*/
		int getNumberOfItems();
		/**
		* Returns a pointer to a list of selected items.
		*/
		std::list<ListItem*> getSelection();

		/**
		* Selects the item at index given.  If index is not valid,
		* all items will be de-selected.
		*/
		void selectItem(unsigned int index);
		/**
		* If set true, multiple items can be selected simultaneously, otherwise only
		* single selections can be made.
		*/
		void setMultiSelect(bool MultiSelect);
		/**
		* Sets the height of each ListItem within the List.
		*/
		void setListItemHeight(float height);

	protected:
		List(const Ogre::String& name);
		~List();

		ListDesc* mDesc;

		std::list<ListItem*> mListItems;
		std::list<ListItem*> mSelectedItems;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mListEventHandlers[LIST_EVENT_COUNT];

		unsigned int mPrevSelectedIndex;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/**
		* Forcing all ListItems to be the width of client dimensions.
		*/
		void onClientSizeChanged(const EventArgs& args);

		/**
		* Function allowing ListItems to notify List when user left clicks on them.
		*/
		void selectItem(const MouseEventArgs& mea);

		/**
		* Clears selection list by de-selecting all selected units and clearing list.
		*/
		void _clearSelection();

		/**
		* Returns the count of the list item within the list.
		*/
		unsigned int _getItemIndex(ListItem* li);

	private:
	};
}

#endif
