#ifndef QUICKGUIPROPERTYGRIDSECTION_H
#define QUICKGUIPROPERTYGRIDSECTION_H

#include "QuickGUITreeViewNode.h"
#include "QuickGUIPropertyGridProperty.h"
#include "QuickGUIPropertyGridBoolProperty.h"
#include "QuickGUIPropertyGridComboBoxProperty.h"
#include "QuickGUIPropertyGridTextProperty.h"

namespace QuickGUI
{
	class _QuickGUIExport PropertyGridSectionDesc :
		public TreeViewNodeDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		PropertyGridSectionDesc(const Ogre::String& id);
		virtual ~PropertyGridSectionDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridSectionDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGridSection"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGridSection :
		public TreeViewNode
	{
	public:
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// TreeView has to set positions, toggle visibility to manage ListItems
		friend class TreeView;
		// PropertyGrid sets Property width and field width
		friend class PropertyGrid;

		friend class Factory<Widget>;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Creates a Text Property for this Section.  A TextBox is used to get and set a property value.
		*/
		PropertyGridTextProperty* createTextProperty(PropertyGridTextPropertyDesc* d);
		/**
		* Creates a Text Property for this Section at the given index.  A TextBox is used to get and set a property value.
		* NOTE: if the index is not valid, the Property is added to the end of the Section.
		*/
		PropertyGridTextProperty* createTextProperty(int index, PropertyGridTextPropertyDesc* d);
		/**
		* Creates a Boolean Property for this Section.  A ComboBox with values "True" and "False" are used
		* to get and set a property value.
		*/
		PropertyGridBoolProperty* createBoolProperty(PropertyGridBoolPropertyDesc* d);
		/**
		* Creates a Boolean Property for this Section at the given index.  A ComboBox with values "True" and "False" are used
		* to get and set a property value.
		* NOTE: if the index is not valid, the Property is added to the end of the Section.
		*/
		PropertyGridBoolProperty* createBoolProperty(int index, PropertyGridBoolPropertyDesc* d);
		/**
		* Creates a ComboBox Property for this Section.  A ComboBox is used to get and set a property value.
		*/
		PropertyGridComboBoxProperty* createComboBoxProperty(PropertyGridComboBoxPropertyDesc* d);
		/**
		* Creates a ComboBox Property for this Section at the given index.  A ComboBox is used to get and set a property value.
		* NOTE: if the index is not valid, the Property is added to the end of the Section.
		*/
		PropertyGridComboBoxProperty* createComboBoxProperty(int index, PropertyGridComboBoxPropertyDesc* d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

	protected:
		PropertyGridSection(const Ogre::String& name);
		virtual ~PropertyGridSection();

		PropertyGridSectionDesc* mDesc;

		std::list<PropertyGridProperty*> mProperties;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

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

		void setPropertyFieldWidth(float width);

		void setPropertyWidth(float width);

		void updatePropertySkin();

	private:
	};
}

#endif
