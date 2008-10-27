#ifndef QUICKGUIHSCROLLBAR_H
#define QUICKGUIHSCROLLBAR_H

#include "QuickGUIButton.h"
#include "QuickGUIComponentWidget.h"
#include "QuickGUIEventTypes.h"
#include "QuickGUIScrollBarButtonLayouts.h"

namespace QuickGUI
{
	class _QuickGUIExport HScrollBarDesc :
			public ComponentWidgetDesc
	{
	public:
		HScrollBarDesc();

		/// The amount the slider slides when an up/down button is clicked.
		float buttonScrollPercent;
		/// The amount the slider slides when the bar is clicked.
		float barScrollPercent;

		HScrollBarButtonLayout scrollBarButtonLayout;
		/// Width of the slider in pixels.
		float sliderWidth;
		/// Position of the slider as a percentage. 0 is at the top, 100 is at the bottom. (as far down as it can go)
		float sliderPercentage;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "HScrollBarDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "HScrollBar"; }

		// Factory method
		static WidgetDesc* factory() { return new HScrollBarDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport HScrollBar :
		public ComponentWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BAR;
		static const Ogre::String LEFT1;
		static const Ogre::String LEFT2;
		static const Ogre::String RIGHT1;
		static const Ogre::String RIGHT2;
		static const Ogre::String SLIDER;
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
		/**
		* Determine Slider Bounds and toggle button visibility.
		*/
		void _updateButtons();
		/**
		* Updates the slider's position if the ScrollBar's dimensions have changed.
		* Also can update slider size, if ScrollBar is Live.
		*/
		void _updateSlider();

		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: WINDOW_EVENT_FOCUS_LOST, WINDOW_EVENT_FOCUS_GAINED, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::WINDOW_EVENT_FOCUS_LOST,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addScrollBarEventHandler(ScrollBarEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mScrollBarEventHandlers[EVENT].push_back(new EventHandlerPointer<T>(function,obj));
		}
		void addScrollBarEventHandler(ScrollBarEvent EVENT, EventHandlerSlot* function);

		/**
		* Makes the ScrollBar not live.  Refer to enableLiveSlider for more information.
		*/
		void disableLiveSlider();

		/**
		* Makes the ScrollBar Live, where the Slider grows and shrinks based on value1 / value2.
		* The normal case for this is mapping the values to the actual height / virtual height.
		*/
		void enableLiveSlider(float* value1, float* value2);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireScrollBarEvent(ScrollBarEvent e, EventArgs& args);

		/**
		* Gets the amount the sliders moves when the bar is clicked.
		*/
		float getBarScrollPercent();
		/**
		* Returns the Button layout of the ScrollBar.
		*/
		HScrollBarButtonLayout getButtonLayout();
		/**
		* Gets the amount the slider moves when the up/down arrows are clicked.
		*/
		float getButtonScrollPercent();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Gets the position of the slider relative to the bar bounds. 0 percent is at the top, 100 at the bottom.
		*/
		float getPercentage();
		/**
		* Returns the percentage of the slider width compared to the scrolling area. 1.0 means
		* the slider is the entire width of the scroll area.
		*/
		float getSliderWidthPercentage();

		/**
		* Scrolls left by ButtonScrollPercent.
		*/
		void scrollLeft();
		/**
		* If true, slider will be positioned at right-most position within scrolling area.
		* NOTE: we do not need an equivalent "scrollToBeginning", since you can simply call setPercentage(0).
		*/
		void scrollToEnd();
		/**
		* Scrolls right by ButtonScrollPercent.
		*/
		void scrollRight();
		/**
		* Sets the amount the sliders moves when the bar is clicked.
		*/
		void setBarScrollPercent(float percent);
		/**
		* Sets the Button layout of the ScrollBar.
		*/
		void setButtonLayout(HScrollBarButtonLayout l);
		/**
		* Sets the amount the slider moves when the up/down arrows are clicked.
		*/
		void setButtonScrollPercent(float percent);
		/**
		* Sets the position of the slider relative to the bar bounds. 0 percent is at the top, 100 at the bottom.
		*/
		void setPercentage(float percentage);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets the height of the slider.  Disabled ScrollBar from being Live.
		*/
		void setSliderWidth(float width);

	protected:
		HScrollBar(const Ogre::String& name);
		virtual ~HScrollBar();

		HScrollBarDesc* mDesc;

		Button* mButton_Left1;
		Button* mButton_Left2;
		Button* mButton_Right1;
		Button* mButton_Right2;
		Button* mButton_Slider;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mScrollBarEventHandlers[SCROLLBAR_EVENT_COUNT];

		/// Live ScrollBars dynamically resize the slider to represent a ratio of two value.
		/// For the most part, the two values are the actual height and the virtual height.
		bool mLive;
		/// Pointers to the values used to derive the live slider height.
		float* mLiveValue1;
		float* mLiveValue2;

		/// The area in which the slider can reside. (defines left/right)
		Point mSliderBounds;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		void onSliderDragged(const EventArgs& args);

		void onSizeChanged(const EventArgs& args);

		void _setPercentage(float percentage);

		void onLeftClicked(const EventArgs& args);
		void onRightClicked(const EventArgs& args);
		void onBarClicked(const EventArgs& args);

	private:
	};
}

#endif
