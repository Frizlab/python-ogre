#ifndef QUICKGUIBUTTON_H
#define QUICKGUIBUTTON_H

#include "QuickGUIButtonStates.h"
#include "QuickGUILabel.h"

namespace QuickGUI
{
	class _QuickGUIExport ButtonDesc :
			public LabelDesc
	{
	public:
		ButtonDesc();

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ButtonDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Button"; }

		// Factory method
		static WidgetDesc* factory() { return new ButtonDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Button :
		public Label
	{
	public:
		// Skin Constants
		static const Ogre::String DEFAULT;
		static const Ogre::String OVER;
		static const Ogre::String DOWN;
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
		template<typename T> void addButtonEventHandler(ButtonEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mButtonEventHandlers[EVENT].push_back(new EventHandlerPointer<T>(function,obj));
		}
		void addButtonEventHandler(ButtonEvent EVENT, EventHandlerSlot* function);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireButtonEvent(ButtonEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

		/**
		* Gets the current state of the button.
		*/
		ButtonState getState();

		/**
		* Sets the current state of the button, and fires the BUTTON_EVENT_STATE_CHANGED event.
		* NOTE: No event is fired if the state is already applied.
		*/
		void setState(ButtonState s);

	protected:
		Button(const Ogre::String& name);
		~Button();

		/// Keep track of the button state
		ButtonState mCurrentButtonState;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mButtonEventHandlers[BUTTON_EVENT_COUNT];

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
