#ifndef QUICKGUIWIDGET_H
#define QUICKGUIWIDGET_H

#include "OgreBorderPanelOverlayElement.h"
#include "QuickGUIEventArgs.h"
#include "QuickGUIMemberFunctionPointer.h"
#include "QuickGUIPrerequisites.h"
#include "OgreTextAreaOverlayElement.h"

#include <vector>

namespace QuickGUI
{
	// Forward declaration solely for the getWindow function
	class Window;

	/**
	* Useful for widgets horizontally aligning child widgets, for example a
	* TitleBar aligning its label widget
	*/
	typedef enum HorizontalAlignment
	{
		QGUI_HA_NO_ALIGNMENT	=  0,
		QGUI_HA_LEFT				,
		QGUI_HA_MID					,
		QGUI_HA_RIGHT
	};
	/**
	* Useful for widgets vertically aligning child widgets, for example a
	* TitleBar aligning its label widget
	*/
	typedef enum VerticalAlignment
	{
		QGUI_VA_NO_ALIGNMENT	=  0,
		QGUI_VA_TOP					,
		QGUI_VA_MID					,
		QGUI_VA_BOTTOM
	};

	/** Main QuickGUI components for creating GUI.
		@remarks
		This class outlines the interface for
		widgets, providing default functionality.
	*/
	class Widget
	{
	public:
		/**
		* Outlining Types of widgets in the library.
		*/
		typedef enum Type
		{
			QGUI_TYPE_BUTTON		=  0,
			QGUI_TYPE_COMBOBOX			,
			QGUI_TYPE_IMAGE				,
			QGUI_TYPE_LABEL				,
			QGUI_TYPE_LIST				,
			QGUI_TYPE_LISTITEM			,
			QGUI_TYPE_MENU				,
			QGUI_TYPE_NSTATEBUTTON		,
			QGUI_TYPE_PLAINWINDOW		,
			QGUI_TYPE_PROGRESSBAR		,
			QGUI_TYPE_TEXT				,
			QGUI_TYPE_TEXTBOX			,
			QGUI_TYPE_TEXTCURSOR		,
			QGUI_TYPE_TITLEBAR			,
			QGUI_TYPE_WINDOW
		};
		/**
		* All widgets must support these events
		*/
		typedef enum Event
		{
			QGUI_EVENT_ACTIVATED      =  0,
			QGUI_EVENT_CHARACTER_KEY	  ,
			QGUI_EVENT_DEACTIVATED		  ,
			QGUI_EVENT_KEY_DOWN			  ,
			QGUI_EVENT_KEY_UP			  ,
			QGUI_EVENT_MOUSE_BUTTON_DOWN  ,
			QGUI_EVENT_MOUSE_BUTTON_UP	  ,
			QGUI_EVENT_MOUSE_CLICK		  ,
			QGUI_EVENT_MOUSE_ENTER		  ,
			QGUI_EVENT_MOUSE_LEAVE		  ,
			QGUI_EVENT_MOUSE_MOVE		  ,
			QGUI_EVENT_MOUSE_WHEEL			
		};
		/**
		* Borders that every widget can use.
		*/
		typedef enum Border
		{
			QGUI_BORDER_TOP		=  0,
			QGUI_BORDER_BOTTOM		,
			QGUI_BORDER_LEFT		,
			QGUI_BORDER_RIGHT
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
		Widget(const Ogre::String& instanceName, const Ogre::Vector4& relativeDimensions, Ogre::String material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~Widget();

		// sets the widget position/size based on mPixelDimensions
		virtual void _applyDimensions()=0;
		// Internal method to create widget borders.
		void _createBorders();
		// Internal method to destroy widget borders.
		void _destroyBorders();
		// Recalculate the dimensions and reposition/resize the widget (calls _updateDimensions)
		virtual void _notifyDimensionsChanged();
		// Realign text, if the widget has a child label widget
		virtual void _notifyTextChanged() {}
		// Notify Widget of it's Window's zOrder, and it's own zOrder offset.
		virtual void _notifyZOrder(Ogre::ushort windowZOrder, Ogre::ushort offsetZOrder);
		// Notify Widget that Window's zOrder has changed. Updates child widgets also.
		virtual void _notifyZOrder(Ogre::ushort windowZOrder);
		// Calculate Absolute and Pixel dimensions
		void _updateDimensions(const Ogre::Vector4& relativeDimensions);
		// Internal method to update border sizes
		void _updateBorderSize();

		/** Adds an event handler to this widget
            @note
                Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@param
				EVENT Defined widget events, for example: QGUI_EVENT_ACTIVATED, QGUI_EVENT_CHARACTER_KEY, QGUI_EVENT_MOUSE_BUTTON_DOWN, etc
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addEventHandler(QuickGUI::QGUI_EVENT_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
        */
		template<typename T> void addEventHandler(Event EVENT, bool (T::*function)(const EventArgs&), T* obj)
		{
			mUserEventHandlers[EVENT].push_back(new MemberFunctionPointer<T>(function,obj));
		}

		void addEventHandler(Event EVENT, MemberFunctionSlot* function);

		/**
		* Internal method.  User works in relative coordinates, but internally everything is done in pixels.
		* Used to calculate the pixel size acording to the dimensions provided.
		*/
		Ogre::Vector4 convertRelativeToPixelDimensions(const Ogre::Vector4& dimensions);

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
		/**
		* Returns the Overlay Container is used to contain the Widget's OverlayElements
		*/
		Ogre::OverlayContainer* getOverlayContainer();
		/**
		* Returns the Widget's Parent Widget.  Does not throw an exception is the Parent
		* does not exist.  (Windows do not have parent widgets)
		*/
		Widget* getParentWidget();
		/**
		* Returns the user defined name for this widget.  If no reference name has been assigned to this
		* widget, "" is returned.
		*/
		Ogre::String getReferenceName();
		/**
		* Iterates through visible Children widgets to find and return the widget that is *hit* by the point.
		* Returns NULL is nothing is *hit*.
		*/
		virtual Widget* getTargetWidget(const Ogre::Vector2& p);
		/**
		* Returns the text element of this widget.  Text by default is "".
		*/
		Ogre::UTFString getText();
		Ogre::ColourValue getTextColorTop();
		Ogre::ColourValue getTextColorBot();
		
		/**
		* Returns the type of the widget, as enumerated above. ex. QGUI_TYPE_BUTTON.
		*/
		Type getWidgetType();
		/**
		* Get Window this widget is attached to.  If the widget is a Window, it is returned.
		*/
		Window* getWindow();
		/**
		* Gets the derived zOrder of this widget.
		*/
		Ogre::ushort getDerivedZOrder();

		/**
		* Sets mVisible to false.  Widgets should override this to implement how they handle
		* hiding.
		*/
		virtual void hide();
		/**
		* Hide borders if they exist.
		*/
		void hideBorders();
		/**
		* Returns true if pixel point p is inside the pixel dimensions of this widget.
		*/
		virtual bool isPointWithinBounds(const Ogre::Vector2& p);
		bool isVisible();

		/**
		* Offset the widget dimensions.  Useful for dragging/moving widgets.
		*/
		void move(const Ogre::Real& relativeXOffset, const Ogre::Real& relativeYOffset);

		/**
		* Sets top/bottom/left/right border size in pixels.
		*/
		void setBorderWidth(int borderPixelHeight);
		/**
		* Sets top/bottom, left/right border size in pixels.
		*/
		void setBorderWidth(int topBotBorderPixelHeight, int leftRightBorderPixelHeight);
		/**
		* Sets top, bottom, left, right border size in pixels.
		*/
		void setBorderWidth(int topBorderPixelHeight, int botBorderPixelHeight, int leftBorderPixelHeight, int rightBorderPixelHeight);
		/**
		* Sets character height.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.
		*/
		virtual void setCharacterHeight(const Ogre::Real& relativeHeight);
		/**
		* Manually set the relative Dimensions of the widget.
		*/
		void setDimensions(const Ogre::Vector4& relativeDimensions);
		/**
		* Sets text font.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.
		*/
		virtual void setFont(const Ogre::String& font);
		/**
		* Manually set mGrabbed to true.
		*/
		void setGrabbed(bool grabbed);
		/**
		* Sets only the height of the widget, relative to it's parent (or screen if parent does not exist)
		* NOTE: all children widgets will be notified that dimensions have changed.
		*/
		void setHeight(const Ogre::Real& relativeHeight);
		/**
		* If set to false, widget cannot be moved.
		*/
		void setMovingEnabled(bool enable);
		/**
		* Manually set position of widget relative to its parent, if exists. Window positioning is done
		* relative to the screen (Render Window).
		*/
		void setPosition(const Ogre::Real& xRelativePosition, const Ogre::Real& yRelativePosition);
		/**
		* Gives the widget a user defined name, that can be used to get access to the widget.
		*/
		void setReferenceName(const Ogre::String& ReferenceName);
		/**
		* Sets text.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.  Or in some cases where a widget does not use text,
		* this will have no visual impact.
		*/
		virtual void setText(const Ogre::UTFString& text);
		/**
		* Sets text color.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.  Or in some cases where a widget does not use text,
		* this will have no visual impact.
		*/
		virtual void setTextColor(const Ogre::ColourValue& color);
		virtual void setTextColor(const Ogre::ColourValue& topColor,const Ogre::ColourValue& botColor);
		virtual void setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer=0) {}
		/**
		* Sets mVisible to true.  Widgets should override this to implement how they handle
		* showing.
		*/
		virtual void show();
		/**
		* Shows borders, if they exist.
		*/
		void showBorders();
		/**
		* Sets text vertical alignment.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.  Or in some cases where a widget does not use text,
		* this will have no visual impact.
		*/
		void setVerticalAlignment(VerticalAlignment va);
		/**
		* Sets text horizontal alignment.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.  Or in some cases where a widget does not use text,
		* this will have no visual impact.
		*/
		void setHorizontalAlignment(HorizontalAlignment ha);

		// Event Handler functions - should be overriden to supply default functionality,
		// For example, the button won't look like a button unless you apply the *.over and
		// *.down material during onMouseEnters, Leaves, ButtonDown, etc. events
		/**
		* Default Handler for the QGUI_EVENT_ACTIVATED event, and activates all child widgets (if exist)
		*/
		virtual void activate(EventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_DEACTIVATED event, and deactivates all child widgets (if exist)
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
		Ogre::String				mReferenceName;
		Type						mWidgetType;
		bool						mVisible;

		Ogre::ushort				mWindowZOrder;
		Ogre::ushort				mOffsetZOrder;

		bool						mGrabbed;
		bool						mMovingEnabled;

		Ogre::String				mFont;
		Ogre::Real					mCharacterHeight;
		Ogre::UTFString				mText;
		Ogre::ColourValue			mTextTopColor;
		Ogre::ColourValue			mTextBotColor;
		VerticalAlignment			mVerticalAlignment;
		HorizontalAlignment			mHorizontalAlignment;

		Ogre::String				mWidgetMaterial;
		Ogre::PanelOverlayElement*	mBorders[4];
		int							mBorderSizeInPixels[4];
		Ogre::String				mBorderMaterial[4];
		bool						mBordersHidden;

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
