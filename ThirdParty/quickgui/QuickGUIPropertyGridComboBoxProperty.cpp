#include "QuickGUIPropertyGridComboBoxProperty.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIPropertyGrid.h"

namespace QuickGUI
{
	void PropertyGridComboBoxProperty::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridComboBoxProperty");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridComboBoxProperty",d);
	}

	PropertyGridComboBoxPropertyDesc::PropertyGridComboBoxPropertyDesc(const Ogre::String& id) :
		PropertyGridPropertyDesc(id)
	{
		resetToDefault();
	}

	void PropertyGridComboBoxPropertyDesc::resetToDefault()
	{
		PropertyGridPropertyDesc::resetToDefault();
	}

	void PropertyGridComboBoxPropertyDesc::serialize(SerialBase* b)
	{
		PropertyGridPropertyDesc::serialize(b);
	}

	PropertyGridComboBoxProperty::PropertyGridComboBoxProperty(const Ogre::String& name) :
		PropertyGridProperty(name),
		mComboBox(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	PropertyGridComboBoxProperty::~PropertyGridComboBoxProperty()
	{
	}

	void PropertyGridComboBoxProperty::_initialize(WidgetDesc* d)
	{		
		PropertyGridProperty::_initialize(d);
	}

	Ogre::String PropertyGridComboBoxProperty::getClass()
	{
		return "PropertyGridComboBoxProperty";
	}

	void PropertyGridComboBoxProperty::updateFieldSkin()
	{
		if((mDesc->treeView != NULL) && (mComboBox != NULL))
			mComboBox->setSkinType(dynamic_cast<PropertyGrid*>(mDesc->treeView)->getComboBoxFieldSkin());
	}
}
