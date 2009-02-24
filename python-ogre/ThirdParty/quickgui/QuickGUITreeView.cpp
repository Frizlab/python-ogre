#include "QuickGUITreeView.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String TreeView::BACKGROUND = "background";

	void TreeView::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TreeView");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TreeView",d);
	}

	TreeViewDesc::TreeViewDesc(const Ogre::String& id) :
		ContainerWidgetDesc(id)
	{
		resetToDefault();
	}

	void TreeViewDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();
	}

	void TreeViewDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);
	}

	TreeView::TreeView(const Ogre::String& name) :
		ContainerWidget(name)
	{
		mSkinElementName = BACKGROUND;
	}

	TreeView::~TreeView()
	{
	}

	void TreeView::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<TreeViewDesc*>(mWidgetDesc);

		setSkinType(d->widget_skinTypeName);
	}

	Ogre::String TreeView::getClass()
	{
		return "TreeView";
	}

	void TreeView::onDraw()
	{
		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mWidgetDesc->widget_brushFilterMode);
			
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
	}
}
