#ifndef QUICKGUIPROPERTYGRIDCOMBOBOXPROPERTY_H
#define QUICKGUIPROPERTYGRIDCOMBOBOXPROPERTY_H

#include "QuickGUIPropertyGridProperty.h"
#include "QuickGUIComboBox.h"

namespace QuickGUI
{
	class _QuickGUIExport PropertyGridComboBoxPropertyDesc :
		public PropertyGridPropertyDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		PropertyGridComboBoxPropertyDesc(const Ogre::String& id);
		virtual ~PropertyGridComboBoxPropertyDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridComboBoxPropertyDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGridComboBoxProperty"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGridComboBoxProperty :
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

	protected:
		PropertyGridComboBoxProperty(const Ogre::String& name);
		virtual ~PropertyGridComboBoxProperty();

		PropertyGridComboBoxPropertyDesc* mDesc;

		ComboBox* mComboBox;

		void updateFieldSkin();

	private:
	};
}

#endif
