#include "QuickGUITreeViewNode.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUITreeView.h"
#include "QuickGUITreeViewCheckBoxNode.h"
#include "QuickGUITreeViewRadioButtonNode.h"

namespace QuickGUI
{
	const Ogre::String TreeViewNode::BACKGROUND = "background";
	const Ogre::String TreeViewNode::ICON = "icon";
	const Ogre::String TreeViewNode::SELECTED = "selected";

	void TreeViewNode::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TreeViewNode");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TreeViewNode",d);
	}

	TreeViewNodeDesc::TreeViewNodeDesc(const Ogre::String& id) :
		ContainerWidgetDesc(id),
		treeView(NULL)
	{
		resetToDefault();
	}

	void TreeViewNodeDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		textDesc.resetToDefault();

		containerwidget_clipChildrenToDimensions = false;
		containerwidget_supportScrollBars = false;

		treeviewnode_selected = false;
	}

	void TreeViewNodeDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("Selected",&treeviewnode_selected);

		textDesc.serialize(b);
	}

	TreeViewNode::TreeViewNode(const Ogre::String& name) :
		ContainerWidget(name),
		mExpanded(false),
		mIndex(-1),
		mText(NULL),
		mStateButton(NULL),
		mAutoNameCounter(0)
	{
		mSkinElementName = BACKGROUND;

		addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&TreeViewNode::onKeyDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TreeViewNode::onMouseDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_DOUBLE,&TreeViewNode::onDoubleClick,this);
	}

	TreeViewNode::~TreeViewNode()
	{
		OGRE_DELETE_T(mText,Text,Ogre::MEMCATEGORY_GENERAL);
	}

	float TreeViewNode::_getFarthestX()
	{
		if(!mDesc->widget_visible)
			return 0;

		float farthestX = mTexturePosition.x + mWidgetDesc->widget_dimensions.size.width;
		float childX = 0;

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			childX = (*it)->_getFarthestX();
			if(childX > farthestX)
				farthestX = childX;
		}

		return farthestX;
	}

	void TreeViewNode::_initialize(WidgetDesc* d)
	{		
		TreeViewNodeDesc* tvnd = dynamic_cast<TreeViewNodeDesc*>(d);
		// If Width isn't specified, derive it
		if(tvnd->widget_dimensions.size.width < 1)
			// For TreeViewNode our width will be: [State][Icon][Text][5 pixel buffer]
			tvnd->widget_dimensions.size.width = (tvnd->widget_dimensions.size.height * 2) + tvnd->textDesc.getTextWidth() + 5;
		tvnd->widget_consumeKeyboardEvents = true;

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<TreeViewNodeDesc*>(mWidgetDesc);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = tvnd->textDesc;
		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);

		setSkinType(d->widget_skinTypeName);

		if(tvnd->treeviewnode_selected)
			select();
	}

	void TreeViewNode::_selectNode(TreeViewNode* n)
	{
		if(n == this)
			mDesc->treeviewnode_selected = true;
		else
			mDesc->treeviewnode_selected = false;

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->_selectNode(n);
	}

	void TreeViewNode::addChild(Widget* w)
	{
		// Try to be efficient and only create the button once we have child nodes.
		createStateButtonIfNotExists();

		ContainerWidget::addChild(w);

		TreeViewNode* tvn = dynamic_cast<TreeViewNode*>(w);

		// Set link to TreeView
		tvn->notifyTreeViewParent(mDesc->treeView);

		// This version of addChild is only called in serialization.  Since all ListItems are named in alphabetical order,
		// we can just push each item onto the list.
		mNodes.push_back(tvn);

		tvn->mIndex = static_cast<int>(mNodes.size()) - 1;
	}

	TreeViewNode* TreeViewNode::createNode(TreeViewNodeDesc* d)
	{
		return createNode(-1,d);
	}

	TreeViewNode* TreeViewNode::createNode(int index, TreeViewNodeDesc* d)
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

		TreeViewNode* newTreeViewNode = dynamic_cast<TreeViewNode*>(Widget::create(d->getWidgetClass(),d));

		ContainerWidget::addChild(newTreeViewNode);

		// Set link to TreeView
		newTreeViewNode->notifyTreeViewParent(mDesc->treeView);

		// If we're adding to the end, things are simple - don't have to update other node names, or positions.
		if((index < 0) || (index >= static_cast<int>(mNodes.size())))
		{
			mNodes.push_back(newTreeViewNode);

			updateNodes();

			return newTreeViewNode;
		}
		
		// If we've made it here, we have to insert the node into the list, and update all node indices and positions.
		int counter = 0;
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			if(counter == index)
			{
				mNodes.insert(it,newTreeViewNode);
				break;
			}

			++counter;
		}

		updateNodes();

		return newTreeViewNode;
	}

	void TreeViewNode::createStateButtonIfNotExists()
	{
		if(mStateButton != NULL)
			return;

		ButtonDesc* bd = FactoryManager::getSingleton().getDescFactory()->getInstance<ButtonDesc>("DefaultButtonDesc");
		bd->resetToDefault();
		bd->widget_dimensions.size.width = mWidgetDesc->widget_dimensions.size.height;
		bd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;

		mStateButton = dynamic_cast<Button*>(Widget::create("Button",bd));
		mStateButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TreeViewNode::onStateButtonClicked,this);
		addComponent("StateButton",mStateButton);

		if(mDesc->treeView != NULL)
		{
			if(mExpanded)
				mStateButton->setSkinType(mDesc->treeView->getExpandSkinTypeName());
			else
				mStateButton->setSkinType(mDesc->treeView->getMinimizeSkinTypeName());
		}
	}

	void TreeViewNode::drawLineToParent()
	{
		if(mParentWidget == NULL)
			return;

		if(mParentWidget->getClass() == "TreeView")
			return;

		if(!mWidgetDesc->widget_visible)
			return;

		Brush* brush = Brush::getSingletonPtr();

		Point p1(mTexturePosition);
		p1.x += mWidgetDesc->widget_dimensions.size.height;
		p1.y += (mWidgetDesc->widget_dimensions.size.height / 2.0);
		p1.round();
		
		Point p2(p1);
		p2.x -= (mWidgetDesc->widget_dimensions.size.height / 2.0);
		p2.round();

		Point p3(p2);
		p3.y = mParentWidget->getScreenPosition().y + mParentWidget->getHeight();

		Ogre::ColourValue oldCV = brush->getColour();
		brush->setColor(Ogre::ColourValue::White);

		brush->drawLine(p1,p2);
		brush->drawLine(p2,p3);

		brush->setColor(oldCV);
	}

	void TreeViewNode::drawLines()
	{
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->drawLines();

		drawLineToParent();
	}

	void TreeViewNode::expand()
	{
		// Iterate through parents and make sure they're visible
		Widget* parent = mParentWidget;
		while((parent != NULL) && (parent->getClass() != "TreeView"))
		{
			parent->setVisible(true);

			parent = parent->getParentWidget();
		}

		if(!mNodes.empty())
		{
			for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
				(*it)->setVisible(true);

			mExpanded = true;

			if(mDesc->treeView != NULL)
				mStateButton->setSkinType(mDesc->treeView->getExpandSkinTypeName());
		}

		// Update Node positions
		if(mDesc->treeView != NULL)
			mDesc->treeView->updateNodes();
	}

	Widget* TreeViewNode::findWidgetAtPoint(const Point& p, bool ignoreDisabled)
	{
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			Widget* w = (*it).second->findWidgetAtPoint(p,ignoreDisabled);
			if(w != NULL)
				return w;
		}

		for(std::vector<Widget*>::reverse_iterator it = mChildren.rbegin(); it != mChildren.rend(); ++it)
		{
			Widget* w = (*it)->findWidgetAtPoint(p,ignoreDisabled);
			if(w != NULL)
				return w;
		}

		return Widget::findWidgetAtPoint(p,ignoreDisabled);
	}

	float TreeViewNode::getBranchHeight()
	{
		float height = 0;

		// If invisible, return 0 height
		if(!mDesc->widget_visible)
			return height;

		// If we made it here we know we're visible, so at the very least include our height
		height += mDesc->widget_dimensions.size.height;

		// Add height of child branches (recursive)
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			height += (*it)->getBranchHeight();

		return height;
	}

	Ogre::String TreeViewNode::getClass()
	{
		return "TreeViewNode";
	}

	int TreeViewNode::getIndex()
	{
		return mIndex;
	}

	TreeViewNode* TreeViewNode::getNextSibling()
	{
		if(mParentWidget == NULL)
			return NULL;

		std::list<TreeViewNode*> list;

		if(mParentWidget->getClass() == "TreeView")
			list = dynamic_cast<TreeView*>(mParentWidget)->getRootNodes();
		else
			list = dynamic_cast<TreeViewNode*>(mParentWidget)->getSiblings();

		bool returnNext = false;
		for(std::list<TreeViewNode*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			if(returnNext)
				return (*it);

			if((*it) == this)
				returnNext = true;
		}

		return NULL;
	}

	int TreeViewNode::getNumberOfSiblings()
	{
		return static_cast<int>(mNodes.size());
	}

	TreeViewNode* TreeViewNode::getLastVisibleNode()
	{
		if(!mDesc->widget_visible)
			return NULL;

		// If we have no child nodes or are minimized, return this
		if(!mExpanded || mNodes.empty())
			return this;

		// If there are visible child nodes, make recursive call using last node in list
		return mNodes.back()->getLastVisibleNode();
	}

	TreeViewNode* TreeViewNode::getPreviousSibling()
	{
		if(mParentWidget == NULL)
			return NULL;

		std::list<TreeViewNode*> list;

		if(mParentWidget->getClass() == "TreeView")
			list = dynamic_cast<TreeView*>(mParentWidget)->getRootNodes();
		else
			list = dynamic_cast<TreeViewNode*>(mParentWidget)->getSiblings();

		TreeViewNode* n = NULL;
		for(std::list<TreeViewNode*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			if((*it) == this)
				return n;

			n = (*it);
		}

		return NULL;
	}

	std::list<TreeViewNode*> TreeViewNode::getSiblings()
	{
		std::list<TreeViewNode*> list(mNodes);

		return list;
	}

	Point TreeViewNode::getPositionRelativeToTreeView()
	{
		Point p(mWidgetDesc->widget_dimensions.position);

		Widget* w = mParentWidget;
		while((w != NULL) && (w->getClass() != "TreeView"))
		{
			p.translate(w->getPosition());

			w = w->getParentWidget();
		}

		return p;
	}

	bool TreeViewNode::isExpanded()
	{
		return mExpanded;
	}

	void TreeViewNode::notifyTreeViewParent(TreeView* t)
	{
		mDesc->treeView = t;

		if((t != NULL) && (mStateButton != NULL))
		{
			if(mExpanded)
				mStateButton->setSkinType(mDesc->treeView->getExpandSkinTypeName());
			else
				mStateButton->setSkinType(mDesc->treeView->getMinimizeSkinTypeName());
		}

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->notifyTreeViewParent(t);
	}

	void TreeViewNode::minimize()
	{
		if(mNodes.empty())
			return;

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->setVisible(false);

		mExpanded = false;

		if(mDesc->treeView != NULL)
		{
			mStateButton->setSkinType(mDesc->treeView->getMinimizeSkinTypeName());
			mDesc->treeView->updateNodes();
		}
	}

	void TreeViewNode::onDoubleClick(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if((mDesc->treeView != NULL) && (mDesc->treeView->getDoubleClickToToggle()))
				toggle();
		}
	}

	void TreeViewNode::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		// Draw Background
		Point backgroundPosition(mTexturePosition);
		backgroundPosition.x += mWidgetDesc->widget_dimensions.size.height;
		brush->drawSkinElement(Rect(backgroundPosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));

		// if Selected, draw Selected SkinElement
		if(mDesc->treeviewnode_selected)
			brush->drawSkinElement(Rect(backgroundPosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(SELECTED));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Draw Icon
		Point iconPosition(mTexturePosition.x + mWidgetDesc->widget_dimensions.size.height,mTexturePosition.y);
		brush->drawSkinElement(Rect(iconPosition,Size(mWidgetDesc->widget_dimensions.size.height)),st->getSkinElement(ICON));

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = (mDesc->widget_dimensions.size.height / 2.0) - (textHeight / 2.0);

		// Clip to client dimensions
		Rect clipRegion(mTexturePosition,Size(mWidgetDesc->widget_dimensions.size.width - mWidgetDesc->widget_dimensions.size.height,mWidgetDesc->widget_dimensions.size.height));
		clipRegion.translate(Point(mWidgetDesc->widget_dimensions.size.height * 2,0));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		Point textPosition = clipRegion.position;
		textPosition.y += yPos;		

		mText->draw(textPosition);

		// Restore Clipping
		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void TreeViewNode::onKeyDown(const EventArgs& args)
	{
		if((mDesc->treeView != NULL) && (!mDesc->treeView->getKeyNavigation()))
			return;

		const KeyEventArgs& kea = dynamic_cast<const KeyEventArgs&>(args);

		switch(kea.scancode)
		{
		case KC_LEFT:
			if(mExpanded)
				minimize();
			else
			{
				if((mParentWidget != NULL) && (mParentWidget->getClass() != "TreeView"))
					mDesc->treeView->selectNode(dynamic_cast<TreeViewNode*>(mParentWidget));
			}
			break;
		case KC_RIGHT:
			if(!mExpanded)
				expand();
			else
			{
				if(static_cast<int>(mNodes.size()) > 0)
					mDesc->treeView->selectNode(mNodes.front());
			}
			break;
		case KC_UP:
			// If we are the first child node
			if(mIndex == 0) 
			{
				// If we are not root nodes, select our parent
				if(mParentWidget->getClass() != "TreeView")
					mDesc->treeView->selectNode(dynamic_cast<TreeViewNode*>(mParentWidget));
			}
			else
				mDesc->treeView->selectNode(getPreviousSibling()->getLastVisibleNode());
			break;
		case KC_DOWN:
			{
				if(mExpanded)
					mDesc->treeView->selectNode(mNodes.front());
				else
				{
					// Determine if we are the last sibling
				
					int numSiblings = 0;
					if(mParentWidget != NULL)
					{
						if(mParentWidget->getClass() == "TreeView")
							numSiblings = dynamic_cast<TreeView*>(mParentWidget)->getNumberOfRootNodes();
						else
							numSiblings = dynamic_cast<TreeViewNode*>(mParentWidget)->getNumberOfSiblings();
					}

					// If we are the last child node, iterate up through parents and select the next sibling if exists
					if(mIndex == (numSiblings - 1))
					{
						Widget* w = mParentWidget;
						while(w->getClass() != "TreeView")
						{
							TreeViewNode* n = dynamic_cast<TreeViewNode*>(w)->getNextSibling();

							if(n != NULL)
							{
								mDesc->treeView->selectNode(n);
								break;
							}

							w = w->getParentWidget();
						}
					}
					// If there are siblings below us, just select the next sibling
					else
						mDesc->treeView->selectNode(getNextSibling());
				}
			}
			break;
		case KC_SPACE:
			if(getClass() == "TreeViewCheckBoxNode")
			{
				TreeViewCheckBoxNode* tvcbn = dynamic_cast<TreeViewCheckBoxNode*>(this);
				tvcbn->setChecked(!tvcbn->getChecked());
			}
			else if(getClass() == "TreeViewRadioButtonNode")
				dynamic_cast<TreeViewRadioButtonNode*>(this)->selectRadioButton();
			break;
		}
	}

	void TreeViewNode::onMouseDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			select();
	}

	void TreeViewNode::onStateButtonClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			toggle();
	}

	void TreeViewNode::select()
	{
		if(mDesc->treeView != NULL)
			mDesc->treeView->selectNode(this);
	}

	void TreeViewNode::setDimensions(const Rect& r)
	{
		Widget::setDimensions(r);
	}

	void TreeViewNode::setHeight(float pixelHeight)
	{
		Widget::setHeight(pixelHeight);
	}

	void TreeViewNode::setHorizontalAnchor(HorizontalAnchor a)
	{
		Widget::setHorizontalAnchor(a);
	}

	void TreeViewNode::setPosition(const Point& position)
	{
		Widget::setPosition(position);
	}

	void TreeViewNode::setSize(const Size& size)
	{
		Widget::setSize(size);
	}

	void TreeViewNode::setVerticalAnchor(VerticalAnchor a)
	{
		Widget::setVerticalAnchor(a);
	}

	void TreeViewNode::setVisible(bool visible)
	{
		Widget::setVisible(visible);
	}

	void TreeViewNode::setWidth(float pixelWidth)
	{
		Widget::setWidth(pixelWidth);
	}

	void TreeViewNode::toggle()
	{
		if(mExpanded)
			minimize();
		else
			expand();
	}

	void TreeViewNode::updateNodes()
	{
		float x = mDesc->widget_dimensions.size.height;
		float y = mDesc->widget_dimensions.size.height;
		unsigned int counter = 0;
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			(*it)->setPosition(Point(x,y));
			(*it)->mIndex = counter;
			(*it)->updateNodes();

			y += (*it)->getBranchHeight();
			++counter;
		}
	}
}
