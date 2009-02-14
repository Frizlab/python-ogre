#ifndef QUICKGUIPROPERTYGRIDPROPERTY_H
#define QUICKGUIPROPERTYGRIDPROPERTY_H

#include "QuickGUITreeViewNode.h"

namespace QuickGUI
{
	class _QuickGUIExport PropertyGridPropertyDesc :
		public TreeViewNodeDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		PropertyGridPropertyDesc(const Ogre::String& id);
		virtual ~PropertyGridPropertyDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridPropertyDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGridProperty"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGridProperty :
		public TreeViewNode
	{
	public:
		// TreeView has to set positions, toggle visibility to manage ListItems
		friend class TreeView;
		// PropertyGridSection adjusts the width of the property and field
		friend class PropertyGridSection;

		friend class Factory<Widget>;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		void setFireOnSizeChangedHandler(bool fire);

	protected:
		PropertyGridProperty(const Ogre::String& name);
		virtual ~PropertyGridProperty();

		PropertyGridPropertyDesc* mDesc;

		bool mFireResizeHandler;

		// Properties consist of two part: Label and Field.
		// The field is where users can get/set data. (ie TextBox, ComboBox)
		Widget* mFieldWidget;

		/**
		* Creates a child TreeViewNode.
		*/
		TreeViewNode* createNode(TreeViewNodeDesc* d);
		/**
		* Adds a TreeViewNode to the List at the index given.
		* NOTE: if the index is not valid, the ListItem is added to the end of the list.
		* NOTE: Names are auto-generated in the form of <TreeViewName>+<Index>.
		*/
		TreeViewNode* createNode(int index, TreeViewNodeDesc* d);

		/**
		* Updates the TreeView Link;
		*/
		virtual void notifyTreeViewParent(TreeView* t);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		virtual void updateFieldSkin() = 0;

	private:
	};
}

#endif
