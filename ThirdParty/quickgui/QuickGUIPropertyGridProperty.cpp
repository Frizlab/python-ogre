#include "QuickGUIPropertyGridProperty.h"

namespace QuickGUI
{
	PropertyGridPropertyDesc::PropertyGridPropertyDesc(const Ogre::String& id) :
		TreeViewNodeDesc(id)
	{
		resetToDefault();
	}

	void PropertyGridPropertyDesc::resetToDefault()
	{
		TreeViewNodeDesc::resetToDefault();
	}

	void PropertyGridPropertyDesc::serialize(SerialBase* b)
	{
		TreeViewNodeDesc::serialize(b);
	}

	PropertyGridProperty::PropertyGridProperty(const Ogre::String& name) :
		TreeViewNode(name),
		mFieldWidget(NULL),
		mFireResizeHandler(true)
	{
	}

	PropertyGridProperty::~PropertyGridProperty()
	{
	}

	void PropertyGridProperty::_initialize(WidgetDesc* d)
	{
		d->widget_dimensions.size.width = 200;

		TreeViewNode::_initialize(d);

		mDesc = dynamic_cast<PropertyGridPropertyDesc*>(mWidgetDesc);
	}

	TreeViewNode* PropertyGridProperty::createNode(TreeViewNodeDesc* d)
	{
		return TreeViewNode::createNode(d);
	}

	TreeViewNode* PropertyGridProperty::createNode(int index, TreeViewNodeDesc* d)
	{
		return TreeViewNode::createNode(index,d);
	}

	void PropertyGridProperty::notifyTreeViewParent(TreeView* t)
	{
		TreeViewNode::notifyTreeViewParent(t);

		updateFieldSkin();
	}

	void PropertyGridProperty::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		// Draw Background
		Point backgroundPosition(mTexturePosition);
		brush->drawSkinElement(Rect(backgroundPosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));

		// if Selected, draw Selected SkinElement
		if(mDesc->treeviewnode_selected)
			brush->drawSkinElement(Rect(backgroundPosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(SELECTED));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Draw Icon
		Point iconPosition(mTexturePosition.x,mTexturePosition.y);
		brush->drawSkinElement(Rect(iconPosition,Size(mWidgetDesc->widget_dimensions.size.height)),st->getSkinElement(ICON));

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = (mDesc->widget_dimensions.size.height / 2.0) - (textHeight / 2.0);

		// Clip to client dimensions
		Rect clipRegion(mTexturePosition,Size((mWidgetDesc->widget_dimensions.size.width / 2.0) - mWidgetDesc->widget_dimensions.size.height,mWidgetDesc->widget_dimensions.size.height));
		clipRegion.translate(Point(mWidgetDesc->widget_dimensions.size.height,0));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		Point textPosition = clipRegion.position;
		textPosition.y += yPos;

		mText->draw(textPosition);

		// Restore Clipping
		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void PropertyGridProperty::setFireOnSizeChangedHandler(bool fire)
	{
		mFireResizeHandler = fire;
	}
}
