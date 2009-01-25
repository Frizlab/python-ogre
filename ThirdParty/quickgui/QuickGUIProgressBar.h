#ifndef QUICKGUIPROGRESSBAR_H
#define QUICKGUIPROGRESSBAR_H

#include "QuickGUIWidget.h"
#include "QuickGUIText.h"

#include "OgreImage.h"

#include <vector>

namespace QuickGUI
{
	class _QuickGUIExport ProgressBarDesc :
			public WidgetDesc
	{
	public:
		template<typename BaseClassType>
		friend class Factory;
	protected:
		ProgressBarDesc(const Ogre::String& id);
		virtual ~ProgressBarDesc() {}
	public:

		ProgressBarFillDirection progressbar_fillDirection;
		ProgressBarLayout progressbar_layout;
		ProgressBarClippingEdge progressbar_clippingEdge;

		/// Percentage of progressbar_progress of the bar. (0-100)
		float progressbar_progress;

		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment progressbar_verticalTextAlignment;

		/// Describes the Text used in this Label
		TextDesc textDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ProgressBarDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ProgressBar"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ProgressBar :
		public Widget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String BAR;
		static const Ogre::String CLIPMAP;
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
		template<typename T> void addProgressBarEventHandler(ProgressBarEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mProgressBarEventHandlers[EVENT].push_back(OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addProgressBarEventHandler(ProgressBarEvent EVENT, EventHandlerSlot* function);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireProgressBarEvent(ProgressBarEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Gets the side of the bar texture that is clipped to simulate progressbar_progress.
		*/
		ProgressBarClippingEdge getClippingEdge();
		/**
		* Gets the direction the progressbar_progress bar fills, as it gains progressbar_progress.
		*/
		ProgressBarFillDirection getFillDirection();
		/**
		* Returns the horizontal alignment of text within this widget's client area.
		*/
		HorizontalTextAlignment getHorizontalTextAlignment();
		/**
		* Gets the axis of progressbar_progress growth, either horizontal or vertical.
		*/
		ProgressBarLayout setLayout();
		/**
		* Gets the progressbar_progress of the ProgressBar, that is a visual indicator of percent complete.
		*/
		float getProgress();
		/**
		* Gets the text in UTFString form.
		*/
		Ogre::UTFString getText();
		/**
		* Returns the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		float getVerticalLineSpacing();
		/**
		* Returns the vertical alignment of text within this widget's client area.
		*/
		VerticalTextAlignment getVerticalTextAlignment();
		
		/**
		* Sets the side of the bar texture that is clipped to simulate progressbar_progress.
		*/
		void setClippingEdge(ProgressBarClippingEdge e);
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
		* Sets the direction the progressbar_progress bar fills, as it gains progressbar_progress.
		*/
		void setFillDirection(ProgressBarFillDirection d);
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
		* Sets the Horizontal alignment of Text as displayed within the Label area.
		*/
		void setHorizontalTextAlignment(HorizontalTextAlignment a);
		/**
		* Sets the axis of progressbar_progress growth, either horizontal or vertical.
		*/
		void setLayout(ProgressBarLayout l);
		/**
		* Sets the progressbar_progress of the ProgressBar, that is a visual indicator of percent complete.
		* NOTE: values above and below 0 will be capped to 0/100%.
		*/
		void setProgress(float percent);
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
		* Sets the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		void setVerticalLineSpacing(float distance);
		/**
		* Sets the Vertical alignment of Text as displayed within the Label area.
		*/
		void setVerticalTextAlignment(VerticalTextAlignment a);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

	protected:
		ProgressBar(const Ogre::String& name);
		virtual ~ProgressBar();

		// Current settings applied to newly added text.
		Ogre::String mCurrentFontName;
		Ogre::ColourValue mCurrentColourValue;

		Text* mText;

		/// Pointer pointing to mWidgetDesc object, but casted for quick use.
		ProgressBarDesc* mDesc;

		void _processClipMap();
		/// Array of bools tracking transparent and non-transparent pixels of the clipmap.
		std::vector<bool> mClipMap;

		/// Modified texture to draw on top of background
		Ogre::TexturePtr mOutputBarTexture;

		Ogre::Image mBarImage;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mProgressBarEventHandlers[PROGRESSBAR_EVENT_COUNT];

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
