#include "QuickGUIPropertyGridSection.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	void PropertyGridSection::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridSection");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridSection",d);
	}

	PropertyGridSectionDesc::PropertyGridSectionDesc(const Ogre::String& id) :
		TreeViewNodeDesc(id)
	{
		resetToDefault();
	}

	void PropertyGridSectionDesc::resetToDefault()
	{
		TreeViewNodeDesc::resetToDefault();
	}

	void PropertyGridSectionDesc::serialize(SerialBase* b)
	{
		TreeViewNodeDesc::serialize(b);
	}

	PropertyGridSection::PropertyGridSection(const Ogre::String& name) :
		TreeViewNode(name)
	{
		mSkinElementName = BACKGROUND;
	}

	PropertyGridSection::~PropertyGridSection()
	{
	}

	void PropertyGridSection::_initialize(WidgetDesc* d)
	{		
		TreeViewNode::_initialize(d);

		mDesc = dynamic_cast<PropertyGridSectionDesc*>(mWidgetDesc);
	}

	void PropertyGridSection::addChild(Widget* w)
	{
		TreeViewNode::addChild(w);

		mProperties.push_back(dynamic_cast<PropertyGridProperty*>(w));
	}

	TreeViewNode* PropertyGridSection::createNode(TreeViewNodeDesc* d)
	{
		return TreeViewNode::createNode(d);
	}

	TreeViewNode* PropertyGridSection::createNode(int index, TreeViewNodeDesc* d)
	{
		return TreeViewNode::createNode(index,d);
	}

	PropertyGridTextProperty* PropertyGridSection::createTextProperty(PropertyGridTextPropertyDesc* d)
	{
		return createTextProperty(-1,d);
	}

	PropertyGridTextProperty* PropertyGridSection::createTextProperty(int index, PropertyGridTextPropertyDesc* d)
	{
		// Try to be efficient and only create the button once we have child nodes.
		createStateButtonIfNotExists();

		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".Auto" + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
		d->widget_verticalAnchor = ANCHOR_VERTICAL_TOP;
		d->widget_dimensions.size.height = mDesc->widget_dimensions.size.height;
		d->widget_positionRelativeToParentClientDimensions = false;
		d->widget_visible = false;

		PropertyGridTextProperty* newTextProperty = dynamic_cast<PropertyGridTextProperty*>(Widget::create(d->getWidgetClass(),d));

		ContainerWidget::addChild(newTextProperty);

		mProperties.push_back(newTextProperty);

		// Set link to TreeView
		newTextProperty->notifyTreeViewParent(mDesc->treeView);

		// If we're adding to the end, things are simple - don't have to update other node names, or positions.
		if((index < 0) || (index >= static_cast<int>(mNodes.size())))
		{
			mNodes.push_back(newTextProperty);

			updateNodes();

			return newTextProperty;
		}
		
		// If we've made it here, we have to insert the node into the list, and update all node indices and positions.
		int counter = 0;
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			if(counter == index)
			{
				mNodes.insert(it,newTextProperty);
				break;
			}

			++counter;
		}

		updateNodes();

		return newTextProperty;
	}

	PropertyGridBoolProperty* PropertyGridSection::createBoolProperty(PropertyGridBoolPropertyDesc* d)
	{
		return createBoolProperty(-1,d);
	}

	PropertyGridBoolProperty* PropertyGridSection::createBoolProperty(int index, PropertyGridBoolPropertyDesc* d)
	{
		// Try to be efficient and only create the button once we have child nodes.
		createStateButtonIfNotExists();

		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".Auto" + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
		d->widget_verticalAnchor = ANCHOR_VERTICAL_TOP;
		d->widget_dimensions.size.height = mDesc->widget_dimensions.size.height;
		d->widget_positionRelativeToParentClientDimensions = false;
		d->widget_visible = false;

		PropertyGridBoolProperty* newBoolProperty = dynamic_cast<PropertyGridBoolProperty*>(Widget::create(d->getWidgetClass(),d));

		ContainerWidget::addChild(newBoolProperty);

		mProperties.push_back(newBoolProperty);

		// Set link to TreeView
		newBoolProperty->notifyTreeViewParent(mDesc->treeView);

		// If we're adding to the end, things are simple - don't have to update other node names, or positions.
		if((index < 0) || (index >= static_cast<int>(mNodes.size())))
		{
			mNodes.push_back(newBoolProperty);

			updateNodes();

			return newBoolProperty;
		}
		
		// If we've made it here, we have to insert the node into the list, and update all node indices and positions.
		int counter = 0;
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			if(counter == index)
			{
				mNodes.insert(it,newBoolProperty);
				break;
			}

			++counter;
		}

		updateNodes();

		return newBoolProperty;
	}

	PropertyGridComboBoxProperty* PropertyGridSection::createComboBoxProperty(PropertyGridComboBoxPropertyDesc* d)
	{
		return createComboBoxProperty(-1,d);
	}

	PropertyGridComboBoxProperty* PropertyGridSection::createComboBoxProperty(int index, PropertyGridComboBoxPropertyDesc* d)
	{
		return NULL;
	}

	Ogre::String PropertyGridSection::getClass()
	{
		return "PropertyGridSection";
	}

	void PropertyGridSection::setPropertyFieldWidth(float width)
	{
		for(std::list<PropertyGridProperty*>::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
			(*it)->setFireOnSizeChangedHandler(false);

		for(std::list<PropertyGridProperty*>::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
		{
			if((*it)->mFieldWidget == NULL)
				continue;

			(*it)->mFieldWidget->setWidth(width);
			Point p = (*it)->mFieldWidget->getPosition();
			(*it)->mFieldWidget->setPosition(Point((*it)->getWidth() - width,p.y));
		}

		for(std::list<PropertyGridProperty*>::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
			(*it)->setFireOnSizeChangedHandler(true);
	}

	void PropertyGridSection::setPropertyWidth(float width)
	{
		// Calculate Max Field width
		float maxFieldWidth = width - mWidgetDesc->widget_dimensions.size.height;
		Size maxFieldSize(maxFieldWidth,mWidgetDesc->widget_dimensions.size.height);

		for(std::list<PropertyGridProperty*>::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
		{
			(*it)->setWidth(width);

			if((*it)->mFieldWidget == NULL)
				continue;

			// Set the max width of the Field widget
			(*it)->mFieldWidget->setMaxSize(maxFieldSize);
		}
	}

	void PropertyGridSection::updatePropertySkin()
	{
		for(std::list<PropertyGridProperty*>::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
			(*it)->updateFieldSkin();
	}
}
