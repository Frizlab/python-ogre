#ifndef QUICKGUIPROPERTYGRIDBOOLPROPERTY_H
#define QUICKGUIPROPERTYGRIDBOOLPROPERTY_H

#include "QuickGUIPropertyGridProperty.h"
#include "QuickGUIComboBox.h"

namespace QuickGUI
{
	class _QuickGUIExport PropertyGridBoolPropertyDesc :
		public PropertyGridPropertyDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		PropertyGridBoolPropertyDesc(const Ogre::String& id);
		virtual ~PropertyGridBoolPropertyDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridBoolPropertyDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGridBoolProperty"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGridBoolProperty :
		public PropertyGridProperty
	{
	public:
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// TreeView has to set positions, toggle visibility to manage ListItems
		friend class TreeView;
		// Section updates Property TreeView pointer
		friend class PropertyGridSection;

		friend class Factory<Widget>;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* If the field is set "True", true is returned, false otherwise.
		*/
		bool getProperty();

		/**
		* Sets the field to "True" or "False".
		*/
		void setProperty(bool b);

	protected:
		PropertyGridBoolProperty(const Ogre::String& name);
		virtual ~PropertyGridBoolProperty();

		PropertyGridBoolPropertyDesc* mDesc;

		ComboBox* mComboBox;

		void onResized(const EventArgs& args);

		void updateFieldSkin();

	private:
	};
}

#endif
