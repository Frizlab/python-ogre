#ifndef SIMPLEGUIWINDOW_H
#define SIMPLEGUIWINDOW_H

#include "SimpleGUIButton.h"
#include "SimpleGUIComboBox.h"
#include "SimpleGUIImage.h"
#include "SimpleGUIMenu.h"
#include "SimpleGUINStateButton.h"
#include "SimpleGUIPrerequisites.h"
#include "SimpleGUIProgressBar.h"
#include "SimpleGUIText.h"
#include "SimpleGUITextBox.h"
#include "SimpleGUITitleBar.h"
#include "SimpleGUIWidget.h"

#include <vector>

namespace SimpleGUI
{
	/** Represents a traditional Window.
		@remarks
		The Window class is able to create all other widgets,
		and for the majority of times, will be the medium used
		to display and use other widgets.  The Window class
		requires 2-6 material definitions, depending on the
		contstructor used. (Empty Window vs Default Window)
		ex: Original Material - "sample.window"
		Required:
			"sample.window.border"
			"sample.window.titlebar"
			"sample.window.titlebar.button"
			"sample.window.titlebar.button.over"
			"sample.window.titlebar.button.down"
		@note
		Windows must be created by the GUIManager.
	*/
	class Window :
		public Widget
	{
	public:
		/** Constructor - Default Window
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				show toggle visibility.
			@note
				Requires all 6 materials listed above.
			@note
				This Window will have a TitleBar
        */
		Window(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show);
		/** Constructor - Empty Window
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the widget.
			@param
				show toggle visibility.
			@note
				Requires 2 materials listed above: * and *.border.
			@note
				This Window will not have a TitleBar or defined material.
        */
		Window(const Ogre::String& name, const Ogre::Vector4& dimensions, bool show);
		virtual ~Window();

		/**
		* Internal method that sets the pixel location and size of the widget.
		*/
		void _applyDimensions();
		/**
		* Internal method that converts relative dimensions to absolute and 
		* pixel dimensions, and calls _applyDimensions().  Also notifies
		* all child widgets that the dimensions have changed.
		*/
		void _notifyDimensionsChanged();

		Button* createButton(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);
		Button* createButton(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);

		ComboBox* createComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::String highlightMaterial = "", bool show = true);
		ComboBox* createComboBox(const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::String highlightMaterial = "", bool show = true);

		Image* createImage(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture = false, bool show = true);
		Image* createImage(const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture = false, bool show = true);

		Label* createLabel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);
		Label* createLabel(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);

		Menu* createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);
		Menu* createMenu(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);

		NStateButton* createNStateButton(const Ogre::String& name, const Ogre::Vector4& dimensions, bool show = true);
		NStateButton* createNStateButton(const Ogre::Vector4& dimensions, bool show = true);

		ProgressBar* createProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);
		ProgressBar* createProgressBar(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);

		TextBox* createTextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);
		TextBox* createTextBox(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);
		
		/**
		* Creates a widget that is set to the same dimensions as the window.  Relative dimensions = (0,0,1,1)
		* NOTE: Will not work if window is not an Empty Window.  In other words, if any other widgets are
		* created from this window, you cannot have a Full Size Widget.
		*/
		Button* createFullSizeButton(const Ogre::String& material, bool show = true);
		ComboBox* createFullSizeComboBox(const Ogre::String& material, Ogre::String highlightMaterial = "", bool show = true);
		Image* createFullSizeImage(const Ogre::String& material, bool texture = false, bool show = true);
		Label* createFullSizeLabel(const Ogre::String& material, bool show = true);
		Menu* createFullSizeMenu(const Ogre::String& material, bool show = true);
		NStateButton* createFullSizeNStateButton(bool show = true);
		ProgressBar* createFullSizeProgressBar(const Ogre::String& material, bool show = true);
		TextBox* createFullSizeTextBox(const Ogre::String& material, bool show = true);

		Button* getButton(unsigned int index);
		Button* getButton(const Ogre::String& name);

		ComboBox* getComboBox(unsigned int index);
		ComboBox* getComboBox(const Ogre::String& name);

		Image* getImage(unsigned int index);
		Image* getImage(const Ogre::String& name);

		Label* getLabel(unsigned int index);
		Label* getLabel(const Ogre::String& name);
		
		Menu* getMenu(unsigned int index);
		Menu* getMenu(const Ogre::String& name);

		NStateButton* getNStateButton(unsigned int index);
		NStateButton* getNStateButton(const Ogre::String& name);
		/**
		* Given pixel positions, iterate through child widgets to see if point is over them.
		*/
		Widget* getTargetWidget(const Ogre::Vector2& p);
		
		TextBox* getTextBox(unsigned int index);
		TextBox* getTextBox(const Ogre::String& name);
		
		TitleBar* getTitleBar();
		
		Ogre::ushort getZOrder();

		bool hasVisibleBorders();
		/**
		* Hides the widget, including any children widgets.
		*/
		void hide();
		/**
		* Hide, as an event handler.  Used as the default method for close button to hide window
		*/
		bool hide(const EventArgs& e);
		/**
		* More accurately, sets the border material to "transparent".
		*/
		void hideBorders();
		void hideCloseButton();
		void hideMenus();
		void hideTitlebar();

		/**
		* Returns true if the mouse's left button is down over the Window's TitleBar, false otherwise.
		*/
		bool isGrabbed();

		// Overridden Event Handling functions
		// If user Defined Events have been created, they will be called.
		/**
		* Default Handler for the SGUI_ACTIVATED event, and activates all child widgets (if exist)
		*/
		void activate(EventArgs& e);
		/**
		* Default Handler for the SGUI_DEACTIVATED event, and deactivates all child widgets (if exist)
		*/
		void deactivate(EventArgs& e);
		/**
		* Default Handler for the SGUI_MOUSE_BUTTON_DOWN event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonDown(MouseEventArgs& e);

		void setBorderSize(const Ogre::Real& left, const Ogre::Real& right, const Ogre::Real& top, const Ogre::Real& bottom);
		void setBorderSize(const Ogre::Real& sides, const Ogre::Real& topAndBottom);
		void setMaterial(const Ogre::String& material);
		void setPosition(const Ogre::Real& xRelativePosition, const Ogre::Real& yRelativePosition);
		/**
		* Convenience method to set the TitleBar's Label's caption.
		*/
		void setText(const Ogre::String& text);
		void setTextColor(const Ogre::ColourValue& color);
		void setTextColor(const Ogre::ColourValue& topColor,const Ogre::ColourValue& botColor);
		/**
		* Sets the the number of times materials should be tiled in the x and y direction on the Button's
		* PanelOverlayElement.
		*/
		void setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer);
		/**
		* Convenience method to set the TitleBar's height.
		*/
		void setTitleBarHeight(Ogre::Real height);
		void setZOrder(const Ogre::ushort& zOrder);
		/**
		* Shows the widget, including any child widgets.
		*/
		void show();
		/**
		* More accurately, sets the border material to mMaterial".border".
		*/
		void showBorders();
		void showCloseButton();
		void showMenus();
		void showTitlebar();

		void timeElapsed(Ogre::Real time);

	protected:
		// Default material, displayed in its original state.
		Ogre::String						mMaterial;

		Ogre::Overlay*						mWindowOverlay;
		Ogre::BorderPanelOverlayElement*	mWindowBorderOverlayElement;
		Ogre::ushort						mZOrder;

		bool								mTitleBarHidden;

		/**
		* The following Window containers are at different zOrders, allowing child
		* widgets, menus, titlebars and close button to have a set zOrder.  Without
		* enforcing their zOrder, the certain widgets may appear on top of others.
		* There is a max of 5 zOrder per Window Widget.
		*/
		// mOverlayContainer is considered Level0Container
		Ogre::OverlayContainer*				mLevel1Container;
		Ogre::OverlayContainer*				mLevel2Container;
		Ogre::OverlayContainer*				mLevel3Container;
		Ogre::OverlayContainer*				mLevel4Container;

		TitleBar*							mTitleBar;

		int									mAutoNameWidgetCounter;

		std::vector<Button*>				mButtons;
		std::vector<ComboBox*>				mComboBoxes;
		std::vector<Image*>					mImages;
		std::vector<Label*>					mLabels;
		std::vector<Menu*>					mMenus;
		std::vector<NStateButton*>			mNStateButtons;
		std::vector<ProgressBar*>			mProgressBars;
		std::vector<TextBox*>				mTextBoxes;

		void _init();
	};
}

#endif
