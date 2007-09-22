#ifndef QUICKGUIPANEL_H
#define QUICKGUIPANEL_H

#include "OgreStringConverter.h"

#include "QuickGUIButton.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIImage.h"
#include "QuickGUIMenu.h"
#include "QuickGUINStateButton.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUIScrollPane.h"
#include "QuickGUIText.h"
#include "QuickGUITextBox.h"
#include "QuickGUIHorizontalTrackBar.h"
#include "QuickGUIVerticalTrackBar.h"
#include "QuickGUIWidget.h"

#include "QuickGUIQuadContainer.h"

namespace QuickGUI
{
	/** Represents a Widget Container.
	@remarks
	The Panel class has the ability to create the majority of defined Widgets.
	The Sheet and Window Widgets derive from this widget (Panel), giving them the
	same abilities.
	@note
	Panels cannot create the TitleBar, Window, or Sheet widget.
	@note
	Panels are meant to be created via the Window and Sheet widget.
	*/
	class _QuickGUIExport Panel :
		public Image,
		public QuadContainer
	{
	public:
		friend class QuadContainer;
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x Position, y Position, width, and height of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				material Ogre material defining the widget image.
			@param
				group QuadContainer containing this widget.
			@param
				parentWidget parent widget which created this widget.
        */
		Panel(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* parentWidget, GUIManager* gm);		

		void _removeFromChildList(Widget* w);

		Button* createButton(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Button* createButton(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Button* createButton(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Button* createButton(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		ComboBox* createComboBox(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		ComboBox* createComboBox(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		ComboBox* createComboBox(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		ComboBox* createComboBox(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		HorizontalScrollBar* createHorizontalScrollBar(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		HorizontalScrollBar* createHorizontalScrollBar(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		HorizontalScrollBar* createHorizontalScrollBar(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		HorizontalScrollBar* createHorizontalScrollBar(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		Image* createImage(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Image* createImage(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Image* createImage(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Image* createImage(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		Label* createLabel(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Label* createLabel(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Label* createLabel(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Label* createLabel(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		List* createList(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		List* createList(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		List* createList(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		List* createList(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		Menu* createMenu(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Menu* createMenu(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Menu* createMenu(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Menu* createMenu(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		NStateButton* createNStateButton(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		NStateButton* createNStateButton(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		Panel* createPanel(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Panel* createPanel(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Panel* createPanel(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		Panel* createPanel(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		ProgressBar* createProgressBar(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		ProgressBar* createProgressBar(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		ProgressBar* createProgressBar(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		ProgressBar* createProgressBar(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		TextBox* createTextBox(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		TextBox* createTextBox(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		TextBox* createTextBox(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		TextBox* createTextBox(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		HorizontalTrackBar* createHorizontalTrackBar(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		HorizontalTrackBar* createHorizontalTrackBar(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		HorizontalTrackBar* createHorizontalTrackBar(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		HorizontalTrackBar* createHorizontalTrackBar(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		VerticalScrollBar* createVerticalScrollBar(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		VerticalScrollBar* createVerticalScrollBar(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		VerticalScrollBar* createVerticalScrollBar(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		VerticalScrollBar* createVerticalScrollBar(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		VerticalTrackBar* createVerticalTrackBar(const Ogre::String& name, const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		VerticalTrackBar* createVerticalTrackBar(const Rect& dimensions, const Ogre::String& texture, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		VerticalTrackBar* createVerticalTrackBar(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);
		VerticalTrackBar* createVerticalTrackBar(const Rect& dimensions, GuiMetricsMode positionMode = QGUI_GMM_RELATIVE, GuiMetricsMode sizeMode = QGUI_GMM_RELATIVE);

		Button* getButton(unsigned int index);
		Button* getButton(const Ogre::String& name);

		ComboBox* getComboBox(unsigned int index);
		ComboBox* getComboBox(const Ogre::String& name);

		HorizontalScrollBar* getHorizontalScrollBar(unsigned int index);
		HorizontalScrollBar* getHorizontalScrollBar(const Ogre::String& name);

		HorizontalTrackBar* getHorizontalTrackBar(unsigned int index);
		HorizontalTrackBar* getHorizontalTrackBar(const Ogre::String& name);

		Image* getImage(unsigned int index);
		Image* getImage(const Ogre::String& name);

		Label* getLabel(unsigned int index);
		Label* getLabel(const Ogre::String& name);

		List* getList(unsigned int index);
		List* getList(const Ogre::String& name);
		
		Menu* getMenu(unsigned int index);
		Menu* getMenu(const Ogre::String& name);

		NStateButton* getNStateButton(unsigned int index);
		NStateButton* getNStateButton(const Ogre::String& name);

		Panel* getPanel(unsigned int index);
		Panel* getPanel(const Ogre::String& name);

		ProgressBar* getProgressBar(unsigned int index);
		ProgressBar* getProgressBar(const Ogre::String& name);

		ScrollPane* getScrollPane();
		
		TextBox* getTextBox(unsigned int index);
		TextBox* getTextBox(const Ogre::String& name);

		VerticalScrollBar* getVerticalScrollBar(unsigned int index);
		VerticalScrollBar* getVerticalScrollBar(const Ogre::String& name);

		VerticalTrackBar* getVerticalTrackBar(unsigned int index);
		VerticalTrackBar* getVerticalTrackBar(const Ogre::String& name);

	protected:
		virtual ~Panel();

		ScrollPane* mScrollPane;

		int mAutoNameWidgetCounter;

		// Recording the number of widgets created
		unsigned int mNumButtons;
		unsigned int mNumComboBoxes;
		unsigned int mNumHorizontalScrollBars;
		unsigned int mNumHorizontalTrackBars;
		unsigned int mNumImages;
		unsigned int mNumLabels;
		unsigned int mNumLists;
		unsigned int mNumMenus;
		unsigned int mNumNStateButtons;
		unsigned int mNumPanels;
		unsigned int mNumProgressBars;
		unsigned int mNumTextBoxes;
		unsigned int mNumVerticalScrollBars;
		unsigned int mNumVerticalTrackBars;

		/** 
		* Private functions preventing users from setting the Widget Instance Name.  Names
		* can be given to Windows using the "setReferenceName()" function.
        */
		Button* _createButton(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		ComboBox* _createComboBox(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		HorizontalScrollBar* _createHorizontalScrollBar(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		Image* _createImage(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		Label* _createLabel(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		List* _createList(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		// virtual, because Sheets, which contain multiple windows, need to have an overlay made for menus that are to keep highest zOrder.
		virtual Menu* _createMenu(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		NStateButton* _createNStateButton(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode);
		Panel* _createPanel(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		ProgressBar* _createProgressBar(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		TextBox* _createTextBox(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		HorizontalTrackBar* _createHorizontalTrackBar(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		VerticalScrollBar* _createVerticalScrollBar(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);
		VerticalTrackBar* _createVerticalTrackBar(const Ogre::String& name, const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& texture);

		Widget* _getWidget(Widget::Type TYPE, unsigned int index);
		Widget* _getWidget(const Ogre::String& name);
	};
}

#endif
