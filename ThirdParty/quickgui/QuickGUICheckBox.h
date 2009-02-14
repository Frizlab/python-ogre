#ifndef QUICKGUICHECKBOX_H
#define QUICKGUICHECKBOX_H

#include "QuickGUIWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport CheckBoxDesc :
			public WidgetDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		CheckBoxDesc(const Ogre::String& id);
		virtual ~CheckBoxDesc() {}
	public:

		bool checkbox_checked;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "CheckBoxDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "CheckBox"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport CheckBox :
		public Widget
	{
	public:
		// Skin Constants
		static const Ogre::String CHECKED;
		static const Ogre::String CHECKED_DOWN;
		static const Ogre::String CHECKED_OVER;
		static const Ogre::String UNCHECKED;
		static const Ogre::String UNCHECKED_DOWN;
		static const Ogre::String UNCHECKED_OVER;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
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
		template<typename T> void addCheckBoxEventHandler(CheckBoxEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addCheckBoxEventHandler(EVENT,OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addCheckBoxEventHandler(CheckBoxEvent EVENT, EventHandlerSlot* function);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireCheckBoxEvent(CheckBoxEvent e, EventArgs& args);

		/**
		* Returns true if checked, false otherwise.
		*/
		bool getChecked();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

		/**
		* Sets whether this checkbox is checked or not.
		*/
		void setChecked(bool checked);

	protected:
		CheckBox(const Ogre::String& name);
		virtual ~CheckBox();

		CheckBoxDesc* mDesc;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mCheckBoxEventHandlers[CHECKBOX_EVENT_COUNT];

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
