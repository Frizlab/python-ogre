#include "QuickGUIPropertyGridTextProperty.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIPropertyGrid.h"

namespace QuickGUI
{
	void PropertyGridTextProperty::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridTextProperty");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridTextProperty",d);
	}

	PropertyGridTextPropertyDesc::PropertyGridTextPropertyDesc(const Ogre::String& id) :
		PropertyGridPropertyDesc(id)
	{
		resetToDefault();
	}

	void PropertyGridTextPropertyDesc::resetToDefault()
	{
		PropertyGridPropertyDesc::resetToDefault();
	}

	void PropertyGridTextPropertyDesc::serialize(SerialBase* b)
	{
		PropertyGridPropertyDesc::serialize(b);
	}

	PropertyGridTextProperty::PropertyGridTextProperty(const Ogre::String& name) :
		PropertyGridProperty(name),
		mTextBox(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	PropertyGridTextProperty::~PropertyGridTextProperty()
	{
	}

	void PropertyGridTextProperty::_initialize(WidgetDesc* d)
	{		
		PropertyGridProperty::_initialize(d);

		mDesc = dynamic_cast<PropertyGridTextPropertyDesc*>(mWidgetDesc);

		TextBoxDesc* tbd = FactoryManager::getSingleton().getDescFactory()->getInstance<TextBoxDesc>("DefaultTextBoxDesc");
		tbd->resetToDefault();
		tbd->widget_resizeFromLeft = true;
		tbd->widget_dimensions.position.x = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		tbd->widget_dimensions.size.width = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		tbd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		tbd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
		tbd->widget_maxSize = Size(mWidgetDesc->widget_dimensions.size.width - mWidgetDesc->widget_dimensions.size.height,mWidgetDesc->widget_dimensions.size.height);

		mTextBox = dynamic_cast<TextBox*>(Widget::create("TextBox",tbd));
		mTextBox->addWidgetEventHandler(WIDGET_EVENT_SIZE_CHANGED,&PropertyGridTextProperty::onResized,this);
		addComponent("TextBoxField",mTextBox);

		mFieldWidget = mTextBox;
	}

	Ogre::String PropertyGridTextProperty::getClass()
	{
		return "PropertyGridTextProperty";
	}

	void PropertyGridTextProperty::onResized(const EventArgs& args)
	{
		if((mDesc->treeView != NULL) && mFireResizeHandler)
			dynamic_cast<PropertyGrid*>(mDesc->treeView)->setPropertyFieldWidth(mFieldWidget->getWidth());
	}

	void PropertyGridTextProperty::updateFieldSkin()
	{
		if((mDesc->treeView != NULL) && (mTextBox != NULL))
			mTextBox->setSkinType(dynamic_cast<PropertyGrid*>(mDesc->treeView)->getTextFieldSkin());
	}
}
