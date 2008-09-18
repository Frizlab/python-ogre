#ifndef QUICKGUIPANEL_H
#define QUICKGUIPANEL_H

#include "QuickGUIContainerWidget.h"

#include "QuickGUIButton.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIImage.h"
#include "QuickGUILabel.h"
#include "QuickGUIList.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUITextArea.h"
#include "QuickGUITextBox.h"
#include "QuickGUIVScrollBar.h"

namespace QuickGUI
{
	class _QuickGUIExport PanelDesc :
			public ContainerWidgetDesc
	{
	public:
		PanelDesc();

		virtual Ogre::String getClass() { return "PanelDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Panel"; }

		// Factory method
		static WidgetDesc* factory() { return new PanelDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		//virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Panel :
		public ContainerWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class GUIManager;

		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Creates a user defined custom widget.
		*/
		Widget* createCustomWidget(const Ogre::String& className, WidgetDesc& d);

		/**
		* Creates a child Button.
		*/
		Button* createButton(ButtonDesc& d);
		/**
		* Creates a child HScrollBar.
		*/
		HScrollBar* createHScrollBar(HScrollBarDesc& d);
		/**
		* Creates a child Image.
		*/
		Image* createImage(ImageDesc& d);
		/**
		* Creates a child Label.
		*/
		Label* createLabel(LabelDesc& d);
		/**
		* Creates a child List.
		*/
		List* createList(ListDesc& d);
		/**
		* Creates a child Panel.
		*/
		Panel* createPanel(PanelDesc& d);
		/**
		* Creates a child ProgressBar.
		*/
		ProgressBar* createProgressBar(ProgressBarDesc& d);
		/**
		* Creates a child TextArea.
		*/
		TextArea* createTextArea(TextAreaDesc& d);
		/**
		* Creates a child TextBox.
		*/
		TextBox* createTextBox(TextBoxDesc& d);
		/**
		* Creates a child VScrollBar.
		*/
		VScrollBar* createVScrollBar(VScrollBarDesc& d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

	protected:
		Panel(const Ogre::String& name);
		virtual ~Panel();

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		PanelDesc* mPanelDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
