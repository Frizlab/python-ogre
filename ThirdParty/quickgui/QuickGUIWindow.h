#ifndef QUICKGUIWINDOW_H
#define QUICKGUIWINDOW_H

#include "QuickGUIButton.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIImage.h"
#include "QuickGUIMenu.h"
#include "QuickGUINStateButton.h"
#include "QuickGUIPrerequisites.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUIText.h"
#include "QuickGUITextBox.h"
#include "QuickGUITitleBar.h"
#include "QuickGUIWidget.h"

#include <vector>

namespace QuickGUI
{
	/** Represents a traditional Window.
		@remarks
		The Window class is able to create all other widgets,
		and for the majority of times, will be the medium used
		to display and use other widgets.  The Window class
		requires 0-5 material definitions, depending on the
		contstructor used. (Empty Window vs Default Window)
		ex: Original Material - "sample.window"
		Required:
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
			@note
				Requires all 5 materials listed above.
			@note
				This Window will have a TitleBar
        */
		Window(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		/** Constructor - Empty Window
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the widget.
			@param
				show toggle visibility.
			@note
				This Window will not have a TitleBar or defined material.
        */
		Window(const Ogre::String& name, const Ogre::Vector4& dimensions);
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

		Button* createButton(const Ogre::Vector4& dimensions, const Ogre::String& material);
		Button* createButton(const Ogre::Vector4& dimensions);

		ComboBox* createComboBox(const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::String highlightMaterial);
		ComboBox* createComboBox(const Ogre::Vector4& dimensions);

		Image* createImage(const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture);
		Image* createImage(const Ogre::Vector4& dimensions, bool texture);

		Label* createLabel(const Ogre::Vector4& dimensions, const Ogre::String& material);
		Label* createLabel(const Ogre::Vector4& dimensions);

		Menu* createMenu(const Ogre::Vector4& dimensions, const Ogre::String& material);
		Menu* createMenu(const Ogre::Vector4& dimensions);

		NStateButton* createNStateButton(const Ogre::Vector4& dimensions);

		ProgressBar* createProgressBar(const Ogre::Vector4& dimensions, const Ogre::String& material);
		ProgressBar* createProgressBar(const Ogre::Vector4& dimensions);

		TextBox* createTextBox(const Ogre::Vector4& dimensions, const Ogre::String& material);
		TextBox* createTextBox(const Ogre::Vector4& dimensions);
		
		/**
		* Creates a widget that is set to the same dimensions as the window.  Relative dimensions = (0,0,1,1)
		* NOTE: Will not work if window is not an Empty Window.  In other words, if any other widgets are
		* created from this window, you cannot have a Full Size Widget.
		*/
		Button* createFullSizeButton(const Ogre::String& material);
		Button* createFullSizeButton();

		ComboBox* createFullSizeComboBox(const Ogre::String& material, Ogre::String highlightMaterial);
		ComboBox* createFullSizeComboBox();

		Image* createFullSizeImage(const Ogre::String& material, bool texture);
		Image* createFullSizeImage(bool texture);

		Label* createFullSizeLabel(const Ogre::String& material);
		Label* createFullSizeLabel();

		Menu* createFullSizeMenu(const Ogre::String& material);
		Menu* createFullSizeMenu();

		NStateButton* createFullSizeNStateButton();

		ProgressBar* createFullSizeProgressBar(const Ogre::String& material);
		ProgressBar* createFullSizeProgressBar();

		TextBox* createFullSizeTextBox(const Ogre::String& material);
		TextBox* createFullSizeTextBox();

		void destroyButton(unsigned int index);
		void destroyButton(const Ogre::String& name);
		void destroyButton(Button* b);

		void destroyComboBox(unsigned int index);
		void destroyComboBox(const Ogre::String& name);
		void destroyComboBox(ComboBox* b);

		void destroyImage(unsigned int index);
		void destroyImage(const Ogre::String& name);
		void destroyImage(Image* i);

		void destroyLabel(unsigned int index);
		void destroyLabel(const Ogre::String& name);
		void destroyLabel(Label* b);

		void destroyMenu(unsigned int index);
		void destroyMenu(const Ogre::String& name);
		void destroyMenu(Menu* b);

		void destroyNStateButton(unsigned int index);
		void destroyNStateButton(const Ogre::String& name);
		void destroyNStateButton(NStateButton* b);

		void destroyProgressBar(unsigned int index);
		void destroyProgressBar(const Ogre::String& name);
		void destroyProgressBar(ProgressBar* b);

		void destroyTextBox(unsigned int index);
		void destroyTextBox(const Ogre::String& name);
		void destroyTextBox(TextBox* b);

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

		/**
		* Hides the widget, including any children widgets.
		*/
		void hide();
		/**
		* Hide, as an event handler.  Used as the default method for close button to hide window
		*/
		bool hide(const EventArgs& e);
		void hideCloseButton();
		void hideTitlebar();

		/**
		* Returns true if the mouse's left button is down over the Window's TitleBar, false otherwise.
		*/
		bool isGrabbed();

		// Overridden Event Handling functions
		// If user Defined Events have been created, they will be called.
		/**
		* Default Handler for the QGUI_EVENT_ACTIVATED event, and activates all child widgets (if exist)
		*/
		void activate(EventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_DEACTIVATED event, and deactivates all child widgets (if exist)
		*/
		void deactivate(EventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_BUTTON_DOWN event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonDown(MouseEventArgs& e);

		void setMaterial(const Ogre::String& material);
		void setPosition(const Ogre::Real& xRelativePosition, const Ogre::Real& yRelativePosition);
		/**
		* Convenience method to set the TitleBar's Label's caption.
		*/
		void setText(const Ogre::UTFString& text);
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
		void showCloseButton();
		void showTitlebar();

		void timeElapsed(Ogre::Real time);

	protected:
		// Default material, displayed in its original state.
		Ogre::String						mMaterial;

		Ogre::Overlay*						mWindowOverlay;
		Ogre::PanelOverlayElement*			mWindowOverlayElement;
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

		std::vector<Widget*>				mChildWidgets;

		// Recording the number of widgets created
		unsigned int						mNumButtons;
		unsigned int						mNumComboBoxes;
		unsigned int						mNumImages;
		unsigned int						mNumLabels;
		unsigned int						mNumMenus;
		unsigned int						mNumNStateButtons;
		unsigned int						mNumProgressBars;
		unsigned int						mNumTextBoxes;

		void _init();

		/** 
		* Private functions preventing users from setting the Widget Instance Name.  Names
		* can be given to Windows using the "setReferenceName()" function.
        */
		Button* _createButton(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		ComboBox* _createComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::String highlightMaterial);
		Image* _createImage(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture);
		Label* _createLabel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		Menu* _createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		NStateButton* _createNStateButton(const Ogre::String& name, const Ogre::Vector4& dimensions);
		ProgressBar* _createProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		TextBox* _createTextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);

		bool _destroyWidget(Widget::Type TYPE, unsigned int index);
		bool _destroyWidget(const Ogre::String& name);

		Widget* _getWidget(Widget::Type TYPE, unsigned int index);
		Widget* _getWidget(const Ogre::String& name);
	};
}

#endif
