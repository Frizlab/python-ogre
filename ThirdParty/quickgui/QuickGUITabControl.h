#ifndef QUICKGUITABCONTROL_H
#define QUICKGUITABCONTROL_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUIForwardDeclaredDesc.h"

namespace QuickGUI
{
	// forward declarations
	class TabPage;

	class _QuickGUIExport TabControl :
		public ContainerWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
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
		template<typename T> void addTabControlEventHandler(TabControlEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mTabControlEventHandlers[EVENT].push_back(OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addTabControlEventHandler(TabControlEvent EVENT, EventHandlerSlot* function);

		/**
		* Creates a child TabPage.
		*/
		TabPage* createTabPage(TabPageDesc* d);

		/**
		* Checks if point p is within this widget's dimensions.
		* NULL is returned if the point is outside dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, bool ignoreDisabled = true);
		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireTabControlEvent(TabControlEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the currently selected TabPage.
		*/
		TabPage* getSelectedTabPage();
		/**
		* Returns the tab height of each TabPage in this TabControl.
		*/
		float getTabHeight();
		/**
		* Returns the index of the TabPage within the collection of tabs.
		*/
		int getTabIndex(TabPage* p);
		/**
		* Returns the amount of overlap each tab has over the previous one.
		*/
		float getTabOverlap();
		/**
		* Returns the TabPage at the index given.
		*/
		TabPage* getTabPage(unsigned int index);
		/**
		* Returns true if the Tabs can be reordered by mouse clicking and dragging.
		*/
		bool getTabReordering();

		/**
		* Moves a TabPage within the collection of tabs.
		*/
		void reorderTabPage(TabPage* p, unsigned int index);

		/**
		* Selects the TabPage given.
		*/
		void selectTabPage(TabPage* p);
		/**
		* Sets the tab height of each TabPage in this TabControl.
		*/
		void setTabHeight(float height);
		/**
		* Sets the amount of overlap each tab has over the previous one.
		*/
		void setTabOverlap(float overlap);
		/**
		* Sets whether tabs can be reorderd using click and drag.
		*/
		void setTabReordering(bool reordering);

		/**
		* Updates TabPage names to make sure they are based off their index,
		* and sets the position of each tab.
		*/
		void updateTabPages();

	//protected:
		TabControl(const Ogre::String& name);
		virtual ~TabControl();

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		TabControlDesc* mDesc;

		TabPage* mSelectedTab;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mTabControlEventHandlers[TABCONTROL_EVENT_COUNT];

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();
		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
	private:
	};
}

#endif
