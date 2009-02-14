#ifndef QUICKGUIRADIOBUTTON_H
#define QUICKGUIRADIOBUTTON_H

#include "QuickGUIWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport RadioButtonDesc :
			public WidgetDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		RadioButtonDesc(const Ogre::String& id);
		virtual ~RadioButtonDesc() {}
	public:

		bool radiobutton_selected;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "RadioButtonDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "RadioButton"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport RadioButton :
		public Widget
	{
	public:
		// Skin Constants
		static const Ogre::String SELECTED;
		static const Ogre::String SELECTED_DOWN;
		static const Ogre::String SELECTED_OVER;
		static const Ogre::String UNSELECTED;
		static const Ogre::String UNSELECTED_DOWN;
		static const Ogre::String UNSELECTED_OVER;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// This class needs to be able to unselect other RadioButtons in other nodes.
		friend class TreeViewRadioButtonNode;

		friend class Factory<Widget>;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: WIDGET_EVENT_SIZE_CHANGED, WIDGET_EVENT_MOUSE_WHEEL, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::WIDGET_EVENT_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addRadioButtonEventHandler(RadioButtonEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addRadioButtonEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addRadioButtonEventHandler(RadioButtonEvent EVENT, EventHandlerSlot* function);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireRadioButtonEvent(RadioButtonEvent e, EventArgs& args);

		/**
		* Returns true if checked, false otherwise.
		*/
		bool getSelected();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

		/**
		* Selects the RadioButton.
		*/
		void select();

	protected:
		RadioButton(const Ogre::String& name);
		virtual ~RadioButton();

		RadioButtonDesc* mDesc;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mRadioButtonEventHandlers[CHECKBOX_EVENT_COUNT];

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_OVER)
		*/
		void onMouseEnter(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_DEFAULT)
		*/
		void onMouseLeave(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_DOWN)
		*/
		void onMouseLeftButtonDown(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_OVER)
		*/
		void onMouseLeftButtonUp(const EventArgs& args);

	private:
	};
}

#endif
