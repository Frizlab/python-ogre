#ifndef QUICKGUIPROPERTYGRID_H
#define QUICKGUIPROPERTYGRID_H

#include "QuickGUITreeView.h"
#include "QuickGUIPropertyGridSection.h"

#include <list>

namespace QuickGUI
{
	class _QuickGUIExport PropertyGridDesc :
		public TreeViewDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		PropertyGridDesc(const Ogre::String& id);
		virtual ~PropertyGridDesc() {}
	public:

		float propertygrid_propertyFieldWidth;
		float propertygrid_propertyWidth;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGrid"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGrid :
		public TreeView
	{
	public:
	public:
		// Skin Constants
		static const Ogre::String TEXTFIELD;
		static const Ogre::String BOOLFIELD;
		static const Ogre::String MULTIFIELD;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class Factory<Widget>;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Creates a Section.
		*/
		virtual PropertyGridSection* createSection(PropertyGridSectionDesc* d);
		/**
		* Adds a Section to the PropertyGrid at the index given.
		* NOTE: if the index is not valid, the Section is added to the end of the list.
		*/
		virtual PropertyGridSection* createSection(int index, PropertyGridSectionDesc* d);

		/**
		* Returns the skin name of all Boolean ComboBox fields.
		*/
		Ogre::String getBoolFieldSkin();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the skin name of all ComboBox fields.
		*/
		Ogre::String getComboBoxFieldSkin();
		/**
		* Gets the width of all property fields.
		*/
		float getPropertyFieldWidth();
		/**
		* Gets the width of all property labels.
		*/
		float getPropertyWidth();
		/**
		* Returns the skin name of all TextBox fields.
		*/
		Ogre::String getTextFieldSkin();

		/**
		* Sets the width of all property fields.
		*/
		void setPropertyFieldWidth(float width);
		/**
		* Sets the width of all properties.
		*/
		void setPropertyWidth(float width);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);

	protected:
		PropertyGrid(const Ogre::String& name);
		virtual ~PropertyGrid();

		PropertyGridDesc* mDesc;

		std::list<PropertyGridSection*> mSections;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Creates a child TreeViewNode.
		*/
		virtual TreeViewNode* createNode(TreeViewNodeDesc* d);
		/**
		* Adds a TreeViewNode to the List at the index given.
		* NOTE: if the index is not valid, the ListItem is added to the end of the list.
		* NOTE: Names are auto-generated in the form of <TreeViewName>+<Index>.
		*/
		virtual TreeViewNode* createNode(int index, TreeViewNodeDesc* d);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
