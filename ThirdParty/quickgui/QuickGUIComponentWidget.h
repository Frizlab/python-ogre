#ifndef QUICKGUICOMPONENTWIDGET_H
#define QUICKGUICOMPONENTWIDGET_H

#include "QuickGUIWidget.h"

#include <map>
#include <vector>

namespace QuickGUI
{
	// forward declaration
	class GUIManager;
	class SerialReader;

	class _QuickGUIExport ComponentWidgetDesc :
			public WidgetDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		ComponentWidgetDesc(const Ogre::String& id);
		virtual ~ComponentWidgetDesc() {}
	public:

		/// Sets whether clipping region will be set to client area prior to component drawing
		bool componentwidget_clipComponentsToDimensions;

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ComponentWidget :
		public Widget
	{
	public:
		friend class GUIManager;
		friend class SerialReader;
	public:
		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Notifies the Widget of its GUIManager. (for component/container widgets this function is recursive)
		*/
		virtual void _setGUIManager(GUIManager* gm);
		/**
		* Notifies the Widget of its Sheet. (for component/container widgets this function is recursvie)
		*/
		virtual void _setSheet(Sheet* sheet);

		/**
		* Recursively searches through children and returns first widget found with name given.
		* NULL is returned if the widget is not found.
		*/
		virtual Widget* findWidget(const Ogre::String& name);
		/**
		* Checks if point p is within this widget's dimensions.
		* NULL is returned if the point is outside dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, bool ignoreDisabled = true);

		/**
		* Returns true if clipping region will be set to client area prior to component drawing, false otherwise.
		*/
		bool getClipComponentsToDimensions();

		/**
		* Returns true if this widget is made up of more than 1 widget.
		*/
		virtual bool isComponentWidget();

		/**
		* Returns true if clipping region will be set to client area prior to component drawing, false otherwise.
		*/
		void setClipComponentsToDimensions(bool clip);
		/**
		* Sets the height of the widget.
		*/
		virtual void setHeight(float pixelHeight);
		/**
		* Sets the width and height of this widget.
		*/
		virtual void setSize(Size size);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets the widget of the widget.
		*/
		virtual void setWidth(float pixelWidth);

		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

	protected:
		ComponentWidget(const Ogre::String& name);
		virtual ~ComponentWidget();

		std::map<Ogre::String,Widget*> mComponents;

		/**
		* Adds a component to this widget.
		*/
		void addComponent(const Ogre::String& alias, Widget* w);

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();

		/**
		* Internal function to set a widget's parent, updating its window reference and position.
		*/
		virtual void setParent(Widget* parent);
	private:
	};
}

#endif
