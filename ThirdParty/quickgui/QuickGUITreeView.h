#ifndef QUICKGUITREEVIEW_H
#define QUICKGUITREEVIEW_H

#include "QuickGUIContainerWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport TreeViewDesc :
		public ContainerWidgetDesc
	{
	public:
		template<typename BaseClassType>
		friend class Factory;
	protected:
		TreeViewDesc(const Ogre::String& id);
		virtual ~TreeViewDesc() {}
	public:
		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TreeViewDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TreeView"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TreeView :
		public ContainerWidget
	{
	public:
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
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

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

	protected:
		TreeView(const Ogre::String& name);
		virtual ~TreeView();

		TreeViewDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
