#include "QuickGUIPropertyGrid.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String PropertyGrid::TEXTFIELD = "textfield";
	const Ogre::String PropertyGrid::BOOLFIELD = "boolfield";
	const Ogre::String PropertyGrid::MULTIFIELD = "multifield";

	void PropertyGrid::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGrid");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(EXPAND);
		d->defineComponent(MINIMIZE);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGrid",d);
	}

	PropertyGridDesc::PropertyGridDesc(const Ogre::String& id) :
		TreeViewDesc(id)
	{
		resetToDefault();
	}

	void PropertyGridDesc::resetToDefault()
	{
		TreeViewDesc::resetToDefault();

		propertygrid_propertyFieldWidth = 100;
		propertygrid_propertyWidth = 200;
	}

	void PropertyGridDesc::serialize(SerialBase* b)
	{
		TreeViewDesc::serialize(b);

		b->IO("PropertyFieldWidth",&propertygrid_propertyFieldWidth);
		b->IO("PropertyWidth",&propertygrid_propertyWidth);
	}

	PropertyGrid::PropertyGrid(const Ogre::String& name) :
		TreeView(name)
	{
		mSkinElementName = BACKGROUND;
	}

	PropertyGrid::~PropertyGrid()
	{
	}

	void PropertyGrid::_initialize(WidgetDesc* d)
	{
		TreeView::_initialize(d);

		mDesc = dynamic_cast<PropertyGridDesc*>(mWidgetDesc);
	}

	void PropertyGrid::addChild(Widget* w)
	{
		TreeView::addChild(w);

		mSections.push_back(dynamic_cast<PropertyGridSection*>(w));
	}

	PropertyGridSection* PropertyGrid::createSection(PropertyGridSectionDesc* d)
	{
		PropertyGridSection* newSection = dynamic_cast<PropertyGridSection*>(TreeView::createNode(d));

		mSections.push_back(newSection);

		return newSection;
	}

	PropertyGridSection* PropertyGrid::createSection(int index, PropertyGridSectionDesc* d)
	{
		PropertyGridSection* newSection = dynamic_cast<PropertyGridSection*>(TreeView::createNode(index,d));

		mSections.push_back(newSection);

		return newSection;
	}

	TreeViewNode* PropertyGrid::createNode(TreeViewNodeDesc* d)
	{
		return TreeView::createNode(d);
	}

	TreeViewNode* PropertyGrid::createNode(int index, TreeViewNodeDesc* d)
	{
		return TreeView::createNode(index,d);
	}

	Ogre::String PropertyGrid::getBoolFieldSkin()
	{
		return mSkinType->getComponentType(BOOLFIELD)->typeName;
	}

	Ogre::String PropertyGrid::getClass()
	{
		return "PropertyGrid";
	}

	Ogre::String PropertyGrid::getComboBoxFieldSkin()
	{
		return mSkinType->getComponentType(MULTIFIELD)->typeName;
	}

	float PropertyGrid::getPropertyFieldWidth()
	{
		return mDesc->propertygrid_propertyFieldWidth;
	}

	float PropertyGrid::getPropertyWidth()
	{
		return mDesc->propertygrid_propertyWidth;
	}

	Ogre::String PropertyGrid::getTextFieldSkin()
	{
		return mSkinType->getComponentType(TEXTFIELD)->typeName;
	}

	void PropertyGrid::onDraw()
	{
		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mWidgetDesc->widget_brushFilterMode);
			
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void PropertyGrid::setPropertyFieldWidth(float width)
	{
		mDesc->propertygrid_propertyFieldWidth = width;

		// Update all Sections
		for(std::list<PropertyGridSection*>::iterator it = mSections.begin(); it != mSections.end(); ++it)
			(*it)->setPropertyFieldWidth(mDesc->propertygrid_propertyFieldWidth);

		redraw();
	}

	void PropertyGrid::setPropertyWidth(float width)
	{
		mDesc->propertygrid_propertyWidth = width;

		// Update all Sections
		for(std::list<PropertyGridSection*>::iterator it = mSections.begin(); it != mSections.end(); ++it)
			(*it)->setPropertyWidth(mDesc->propertygrid_propertyWidth);

		redraw();
	}

	void PropertyGrid::setSkinType(const Ogre::String type)
	{
		TreeView::setSkinType(type);

		// Update all Sections
		for(std::list<PropertyGridSection*>::iterator it = mSections.begin(); it != mSections.end(); ++it)
			(*it)->updatePropertySkin();
	}
}
