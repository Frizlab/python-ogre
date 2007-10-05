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
		Panel(const Ogre::String& name, const Rect& pixelDimensions, Ogre::String texture, GUIManager* gm);		

		virtual void addChild(Widget* w);

		Button* createButton();

		ComboBox* createComboBox();

		HorizontalScrollBar* createHorizontalScrollBar();

		Image* createImage();

		Label* createLabel();

		List* createList();

		Menu* createMenu();

		NStateButton* createNStateButton();

		Panel* createPanel();

		ProgressBar* createProgressBar();

		TextBox* createTextBox();

		HorizontalTrackBar* createHorizontalTrackBar();

		VerticalScrollBar* createVerticalScrollBar();

		VerticalTrackBar* createVerticalTrackBar();

		ScrollPane* getScrollPane();

	protected:
		virtual ~Panel();
		virtual void setQuadContainer(QuadContainer* container);
	protected:

		ScrollPane* mScrollPane;

		int mAutoNameWidgetCounter;
	};
}

#endif
