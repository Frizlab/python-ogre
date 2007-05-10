#ifndef SIMPLEGUIWIDGET_H
#define SIMPLEGUIWIDGET_H

#include "OgreBorderPanelOverlayElement.h"
#include "SimpleGUIEventArgs.h"
#include "SimpleGUIMemberFunctionPointer.h"
#include "SimpleGUIPrerequisites.h"
#include "OgreTextAreaOverlayElement.h"

#include <vector>

namespace SimpleGUI
{
	/**
	* Useful for widgets horizontally aligning child widgets, for example a
	* TitleBar aligning its label widget
	*/
	typedef enum HorizontalAlignment
	{
		SGUI_HA_NO_ALIGNMENT	=  0,
		SGUI_HA_LEFT				,
		SGUI_HA_MID					,
		SGUI_HA_RIGHT
	};
	/**
	* Useful for widgets vertically aligning child widgets, for example a
	* TitleBar aligning its label widget
	*/
	typedef enum VerticalAlignment
	{
		SGUI_VA_NO_ALIGNMENT	=  0,
		SGUI_VA_TOP				,
		SGUI_VA_MID				,
		SGUI_VA_BOTTOM
	};

	/** Main SimpleGUI components for creating GUI.
		@remarks
		This class outlines the interface for
		widgets, providing default functionality.
	*/
	class Widget
	{
	public:
		typedef enum Type
		{
			SGUI_BUTTON		=  0,
			SGUI_COMBOBOX		,
			SGUI_IMAGE			,
			SGUI_LABEL			,
			SGUI_LIST			,
			SGUI_LISTITEM		,
			SGUI_MENU			,
			SGUI_NSTATEBUTTON	,
			SGUI_PLAINWINDOW	,
			SGUI_PROGRESSBAR	,
			SGUI_TEXT			,
			SGUI_TEXTBOX		,
			SGUI_TITLEBAR		,
			SGUI_WINDOW
		};
		/**
		* All widgets must support these events
		*/
		typedef enum Event
		{
			SGUI_ACTIVATED      =  0,
			SGUI_CHARACTER_KEY		,
			SGUI_DEACTIVATED		,
			SGUI_KEY_DOWN			,
			SGUI_KEY_UP				,
			SGUI_MOUSE_BUTTON_DOWN	,
			SGUI_MOUSE_BUTTON_UP	,
			SGUI_MOUSE_CLICK		,
			SGUI_MOUSE_ENTER		,
			SGUI_MOUSE_LEAVE		,
			SGUI_MOUSE_MOVE			,
			SGUI_MOUSE_WHEEL			
		};
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the widget.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		Widget(const Ogre::String& instanceName, const Ogre::Vector4& relativeDimensions, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~Widget();

		// sets the widget position/size based on mPixelDimensions
		virtual void _applyDimensions()=0;
		// Recalculate the dimensions and reposition/resize the widget (calls _updateDimensions)
		virtual void _notifyDimensionsChanged()=0;
		// Realign text, if the widget has a child label widget
		virtual void _notifyTextChanged() {}
		// Calculate Absolute and Pixel dimensions
		void _updateDimensions(const Ogre::Vector4& relativeDimensions);

		/** Adds an event handler to this widget
            @note
                Only 1 event handler can be defined for an event.  Calling this for the same event will remove
				the previous handler and assign the newly provided one.
			@param
				EVENT Defined widget events, for example: SGUI_ACTIVATED, SGUI_CHARACTER_KEY, SGUI_MOUSE_BUTTON_DOWN, etc
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take SimpleGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addEventHandler(SimpleGUI::SGUI_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
        */
		template<typename T> void addEventHandler(Event EVENT, bool (T::*function)(const EventArgs&), T* obj)
		{
			mUserEventHandlers[EVENT].push_back(new MemberFunctionPointer<T>(function,obj));
		}
		
		void addEventHandler(Event EVENT, MemberFunctionSlot * function)
		{
			mUserEventHandlers[EVENT].push_back(function);
		}
		
		Ogre::Vector4 convertPixelToRelativeDimensions(const Ogre::Vector4& dimensions);
		Ogre::Vector2 convertPixelToRelativePoint(const Ogre::Vector2& point);
		Ogre::Vector4 convertRelativeToPixelDimensions(const Ogre::Vector4& dimensions);
		Ogre::Vector2 convertRelativeToPixelPoint(const Ogre::Vector2& point);

		Ogre::Vector4 getPixelDimensions();
		Ogre::Vector2 getPixelPosition();
		Ogre::Vector2 getPixelSize();

		Ogre::Vector4 getAbsoluteDimensions();
		Ogre::Vector2 getAbsolutePosition();
		Ogre::Vector2 getAbsoluteSize();

		Ogre::Vector4 getRelativeDimensions();
		Ogre::Vector2 getRelativePosition();
		Ogre::Vector2 getRelativeSize();

		Ogre::Real getCharacterHeight();
		Ogre::String getFont();
		bool getGrabbed();
		
		Ogre::String getInstanceName();
		/**
		* Returns true if window is able to be repositions, false otherwise.
		*/
		bool getMovingEnabled();
		Ogre::OverlayContainer* getOverlayContainer();
		Widget* getParentWidget();
		/**
		* Get Window this widget is attached to.  If the widget is a Window, it is returned.
		*/
		Widget* getWindow();
		
		Ogre::String getText();
		Ogre::ColourValue getTextColorTop();
		Ogre::ColourValue getTextColorBot();
		
		Type getWidgetType();

		virtual void hide();
		/**
		* Returns true if pixel point p is inside the pixel dimensions of this widget.
		*/
		virtual bool isPointWithinBounds(const Ogre::Vector2& p);
		bool isVisible();

		void move(const Ogre::Real& relativeXOffset, const Ogre::Real& relativeYOffset);

		virtual void setCharacterHeight(const Ogre::Real& relativeHeight);
		void setDimensions(const Ogre::Vector4& relativeDimensions);
		virtual void setFont(const Ogre::String& font);
		void setGrabbed(bool grabbed);
		/**
		* Sets only the height of the widget, relative to it's parent (or screen if parent does not exist)
		* NOTE: all children widgets will be notified that dimensions have changed.
		*/
		void setHeight(const Ogre::Real& relativeHeight);
		void setMovingEnabled(bool enable);
		void setPosition(const Ogre::Real& xRelativePosition, const Ogre::Real& yRelativePosition);
		virtual void setText(const Ogre::String& text);
		virtual void setTextColor(const Ogre::ColourValue& color);
		virtual void setTextColor(const Ogre::ColourValue& topColor,const Ogre::ColourValue& botColor);
		virtual void setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer=0) {}
		virtual void show();

		void setVerticalAlignment(VerticalAlignment va);
		void setHorizontalAlignment(HorizontalAlignment ha);

		// Event Handler functions - should be overriden to supply default functionality,
		// For example, the button won't look like a button unless you apply the *.over and
		// *.down material during onMouseEnters, Leaves, ButtonDown, etc. events
		/**
		* Default Handler for the SGUI_ACTIVATED event, and activates all child widgets (if exist)
		*/
		virtual void activate(EventArgs& e);
		/**
		* Default Handler for the SGUI_DEACTIVATED event, and deactivates all child widgets (if exist)
		*/
		virtual void deactivate(EventArgs& e);
		virtual bool onMouseEnters(MouseEventArgs& e);
		virtual bool onMouseLeaves(MouseEventArgs& e);
		virtual bool onMouseMoved(MouseEventArgs& e);
		virtual bool onMouseWheel(MouseEventArgs& e);
		virtual bool onMouseButtonUp(MouseEventArgs& e);
		virtual bool onMouseButtonDown(MouseEventArgs& e);
		virtual bool onMouseClicked(MouseEventArgs& e);
		virtual bool onKeyDown(KeyEventArgs& e);
		virtual bool onKeyUp(KeyEventArgs& e);
		virtual bool onCharacter(KeyEventArgs& e);
		virtual void timeElapsed(Ogre::Real time);

	protected:
		Ogre::String				mInstanceName;
		Type						mWidgetType;
		bool						mVisible;

		bool						mGrabbed;
		bool						mMovingEnabled;

		Ogre::String				mFont;
		Ogre::Real					mCharacterHeight;
		Ogre::String				mText;
		Ogre::ColourValue			mTextTopColor;
		Ogre::ColourValue			mTextBotColor;
		VerticalAlignment			mVerticalAlignment;
		HorizontalAlignment			mHorizontalAlignment;

		// Everything is implemented in pixels under the hood
		Ogre::Vector4				mPixelDimensions;
		// First two values represent the absolute positions - 0,0 to 1,1 represent top left to bottom right of render window
		// Second two values represent the absolute sizes
		Ogre::Vector4				mAbsoluteDimensions;
		// First two values represent the relative positions - 0,0 to 1,1 represent top left to bottom right of parent widget
		// Second two values represent the relative sizes		
		Ogre::Vector4				mRelativeDimensions;

		Widget*						mParentWidget;
		Ogre::OverlayContainer*		mOverlayContainer;

		// Event handlers! Only one per event per widget
		std::vector< std::vector<MemberFunctionSlot*> > mUserEventHandlers;

		void _initEventHandlers();

		/* Uses parent widget's absolute dimensions with given relative dimensions to produce absolute (screen) dimensions */
		Ogre::Vector4 getAbsoluteDimensions(const Ogre::Vector4& relativeDimensions);

		// COMMENT TAKEN DIRECTLY FROM OGRE
		/** A specialisation of the PanelOverlayElement to provide a panel with a border.
		@remarks
			Whilst the standard panel can use a single tiled material, this class allows
			panels with a tileable backdrop plus a border texture. This is handy for large
			panels that are too big to use a single large texture with a border, or
			for multiple different size panels where you want the border a constant width
			but the center to repeat.
		@par
			In addition to the usual PanelOverlayElement properties, this class has a 'border
			material', which specifies the material used for the edges of the panel,
			a border width (which can either be constant all the way around, or specified
			per edge), and the texture coordinates for each of the border sections.
		*/
		Ogre::BorderPanelOverlayElement* createBorderPanelOverlayElement(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);

		// COMMENT TAKEN DIRECTLY FROM OGRE
		/** A 2D element which contains other OverlayElement instances.
		@remarks
			This is a specialisation of OverlayElement for 2D elements that contain other
			elements. These are also the smallest elements that can be attached directly
			to an Overlay.
		@remarks
			OverlayContainers should be managed using OverlayManager. This class is responsible for
			instantiating / deleting elements, and also for accepting new types of element
			from plugins etc.
		*/
		Ogre::OverlayContainer* createOverlayContainer(const Ogre::String& name, const Ogre::String& material);

		// COMMENT TAKEN DIRECTLY FROM OGRE
		/** OverlayElement representing a flat, single-material (or transparent) panel which can contain other elements.
		@remarks
			This class subclasses OverlayContainer because it can contain other elements. Like other
			containers, if hidden it's contents are also hidden, if moved it's contents also move etc. 
			The panel itself is a 2D rectangle which is either completely transparent, or is rendered 
			with a single material. The texture(s) on the panel can be tiled depending on your requirements.
		@par
			This component is suitable for backgrounds and grouping other elements. Note that because
			it has a single repeating material it cannot have a discrete border (unless the texture has one and
			the texture is tiled only once). For a bordered panel, see it's subclass BorderPanelOverlayElement.
		@par
			Note that the material can have all the usual effects applied to it like multiple texture
			layers, scrolling / animated textures etc. For multiple texture layers, you have to set 
			the tiling level for each layer.
		*/
		Ogre::PanelOverlayElement* createPanelOverlayElement(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);

		// COMMENT TAKEN FROM OGRE
		/** This class implements an overlay element which contains simple unformatted text.
		*/
		Ogre::TextAreaOverlayElement* createTextAreaOverlayElement(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
	};
}

#endif
