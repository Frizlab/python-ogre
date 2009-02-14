#include "QuickGUIPropertyGridBoolProperty.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIPropertyGrid.h"

namespace QuickGUI
{
	void PropertyGridBoolProperty::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridBoolProperty");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridBoolProperty",d);
	}

	PropertyGridBoolPropertyDesc::PropertyGridBoolPropertyDesc(const Ogre::String& id) :
		PropertyGridPropertyDesc(id)
	{
		resetToDefault();
	}

	void PropertyGridBoolPropertyDesc::resetToDefault()
	{
		PropertyGridPropertyDesc::resetToDefault();
	}

	void PropertyGridBoolPropertyDesc::serialize(SerialBase* b)
	{
		PropertyGridPropertyDesc::serialize(b);
	}

	PropertyGridBoolProperty::PropertyGridBoolProperty(const Ogre::String& name) :
		PropertyGridProperty(name),
		mComboBox(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	PropertyGridBoolProperty::~PropertyGridBoolProperty()
	{
	}

	void PropertyGridBoolProperty::_initialize(WidgetDesc* d)
	{		
		PropertyGridProperty::_initialize(d);

		mDesc = dynamic_cast<PropertyGridBoolPropertyDesc*>(mWidgetDesc);

		ComboBoxDesc* cbd = FactoryManager::getSingleton().getDescFactory()->getInstance<ComboBoxDesc>("DefaultComboBoxDesc");
		cbd->resetToDefault();
		cbd->widget_resizeFromLeft = true;
		cbd->widget_dimensions.position.x = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		cbd->widget_dimensions.size.width = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		cbd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		cbd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
		cbd->widget_maxSize = Size(mWidgetDesc->widget_dimensions.size.width - mWidgetDesc->widget_dimensions.size.height,mWidgetDesc->widget_dimensions.size.height);

		mComboBox = dynamic_cast<ComboBox*>(Widget::create("ComboBox",cbd));
		mComboBox->addWidgetEventHandler(WIDGET_EVENT_SIZE_CHANGED,&PropertyGridBoolProperty::onResized,this);
		addComponent("ComboBoxField",mComboBox);

		ListTextItemDesc* ltid = FactoryManager::getSingleton().getDescFactory()->getInstance<ListTextItemDesc>("DefaultListTextItemDesc");
		ltid->resetToDefault();
		ltid->textDesc.segments.push_back(TextSegment("micross.12",Ogre::ColourValue::White,"False"));
		mComboBox->createItem(ltid);

		ltid->resetToDefault();
		ltid->textDesc.segments.push_back(TextSegment("micross.12",Ogre::ColourValue::White,"True"));
		mComboBox->createItem(ltid);

		mComboBox->selectItem(0);

		mFieldWidget = mComboBox;
	}

	Ogre::String PropertyGridBoolProperty::getClass()
	{
		return "PropertyGridBoolProperty";
	}

	bool PropertyGridBoolProperty::getProperty()
	{
		if(mComboBox->getSelectedItem()->getIndex() == 0)
			return false;
		else
			return true;
	}

	void PropertyGridBoolProperty::onResized(const EventArgs& args)
	{
		if((mDesc->treeView != NULL) && mFireResizeHandler)
			dynamic_cast<PropertyGrid*>(mDesc->treeView)->setPropertyFieldWidth(mFieldWidget->getWidth());
	}

	void PropertyGridBoolProperty::setProperty(bool b)
	{
		if(b)
			mComboBox->selectItem(1);
		else
			mComboBox->selectItem(0);
	}

	void PropertyGridBoolProperty::updateFieldSkin()
	{
		if((mDesc->treeView != NULL) && (mComboBox != NULL))
			mComboBox->setSkinType(dynamic_cast<PropertyGrid*>(mDesc->treeView)->getBoolFieldSkin());
	}
}
