#ifndef QUICKGUITREEVIEWRADIOBUTTONNODE_H
#define QUICKGUITREEVIEWRADIOBUTTONNODE_H

#include "QuickGUITreeViewNode.h"
#include "QuickGUIRadioButton.h"

namespace QuickGUI
{
	class _QuickGUIExport TreeViewRadioButtonNodeDesc :
		public TreeViewNodeDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
// 	protected:
		TreeViewRadioButtonNodeDesc(const Ogre::String& id);
		virtual ~TreeViewRadioButtonNodeDesc() {}
	public:

		bool treeviewradiobuttonnode_radioButtonSelected;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TreeViewRadioButtonNodeDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TreeViewRadioButtonNode"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TreeViewRadioButtonNode :
		public TreeViewNode
	{
	public:
		// Skin Constants
		static const Ogre::String RADIOBUTTON;
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
		template<typename T> void addTreeViewRadioButtonNodeEventHandler(TreeViewRadioButtonNodeEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addTreeViewRadioButtonNodeEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addTreeViewRadioButtonNodeEventHandler(TreeViewRadioButtonNodeEvent EVENT, EventHandlerSlot* function);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireTreeViewRadioButtonNodeEvent(TreeViewRadioButtonNodeEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns true if the Node's RadioButton is selected, false otherwise.
		*/
		bool getRadioButtonSelected();

		/**
		* Selects the Node's RadioButton.
		*/
		void selectRadioButton();

	protected:
		TreeViewRadioButtonNode(const Ogre::String& name);
		virtual ~TreeViewRadioButtonNode();

		TreeViewRadioButtonNodeDesc* mDesc;

		RadioButton* mRadioButton;

		// TreeViewRadioButtonNode specific event handlers
		std::vector<EventHandlerSlot*> mTreeViewRadioButtonNodeEventHandlers[TREEVIEWRADIOBUTTONNODE_EVENT_COUNT];

		void onRadioButtonSelected(const EventArgs& args);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
	};
}

#endif
