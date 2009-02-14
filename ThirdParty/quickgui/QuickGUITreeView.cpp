#include "QuickGUITreeView.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISheet.h"

#include "OgreStringConverter.h"

namespace QuickGUI
{
	const Ogre::String TreeView::BACKGROUND = "background";
	const Ogre::String TreeView::EXPAND = "expand";
	const Ogre::String TreeView::MINIMIZE = "minimize";

	void TreeView::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TreeView");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(EXPAND);
		d->defineComponent(MINIMIZE);
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

		treeview_doubleClickToToggle = true;
		treeview_keyNavigation = true;
		treeview_nodeHeight = 25;
	}

	void TreeViewDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("DoubleClickToToggle",&treeview_doubleClickToToggle);
		b->IO("KeyNavigation",&treeview_keyNavigation);
		b->IO("NodeHeight",&treeview_nodeHeight);
	}

	TreeView::TreeView(const Ogre::String& name) :
		ContainerWidget(name),
		mSelectedNode(NULL),
		mAutoNameCounter(0)
	{
		mSkinElementName = BACKGROUND;

		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TreeView::onMouseDown,this);
	}

	TreeView::~TreeView()
	{
	}

	void TreeView::_determineVirtualSize()
	{
		mVirtualSize = mClientDimensions.size;

		// Farthest X is the distance from client origin.
		float farthestX = _getFarthestX();
		// Remove left border thickness from farthestX value
		farthestX -= mClientDimensions.position.x;
		if(farthestX > mVirtualSize.width)
			mVirtualSize.width = farthestX;

		if(mTotalHeight > mVirtualSize.height)
			mVirtualSize.height = mTotalHeight;
	}

	float TreeView::_getFarthestX()
	{
		float farthestX = 0;
		float childX = 0;

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			childX = (*it)->_getFarthestX();
			if(childX > farthestX)
				farthestX = childX;
		}

		return farthestX;
	}

	void TreeView::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		TreeViewDesc* tvd = dynamic_cast<TreeViewDesc*>(d);
		mDesc = dynamic_cast<TreeViewDesc*>(mWidgetDesc);

		setSkinType(d->widget_skinTypeName);

		setDoubleClickToToggle(tvd->treeview_doubleClickToToggle);
		setKeyNavigation(tvd->treeview_keyNavigation);
		setNodeHeight(tvd->treeview_nodeHeight);
	}

	void TreeView::addChild(Widget* w)
	{
		ContainerWidget::addChild(w);

		TreeViewNode* tvn = dynamic_cast<TreeViewNode*>(w);

		// Set link to TreeView
		tvn->notifyTreeViewParent(this);

		// This version of addChild is only called in serialization.  Since all ListItems are named in alphabetical order,
		// we can just push each item onto the list.
		mNodes.push_back(tvn);

		tvn->mIndex = static_cast<int>(mNodes.size()) - 1;
	}

	void TreeView::addTreeViewEventHandler(TreeViewEvent EVENT, EventHandlerSlot* function)
	{
		mTreeViewEventHandlers[EVENT].push_back(function);
	}

	TreeViewNode* TreeView::createNode(TreeViewNodeDesc* d)
	{
		return createNode(-1,d);
	}

	TreeViewNode* TreeView::createNode(int index, TreeViewNodeDesc* d)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.Node." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
		d->widget_verticalAnchor = ANCHOR_VERTICAL_TOP;
		d->widget_dimensions.size.height = mDesc->treeview_nodeHeight;

		TreeViewNode* newTreeViewNode = dynamic_cast<TreeViewNode*>(Widget::create(d->getWidgetClass(),d));

		ContainerWidget::addChild(newTreeViewNode);

		// Set link to TreeView
		newTreeViewNode->notifyTreeViewParent(this);

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

	bool TreeView::fireTreeViewEvent(TreeViewEvent e, EventArgs& args)
	{
		if(mTreeViewEventHandlers[e].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mTreeViewEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	Ogre::String TreeView::getClass()
	{
		return "TreeView";
	}

	bool TreeView::getDoubleClickToToggle()
	{
		return mDesc->treeview_doubleClickToToggle;
	}

	Ogre::String TreeView::getExpandSkinTypeName()
	{
		return mSkinType->getComponentType(EXPAND)->typeName;
	}

	bool TreeView::getKeyNavigation()
	{
		return mDesc->treeview_keyNavigation;
	}

	float TreeView::getNodeHeight()
	{
		return mDesc->treeview_nodeHeight;
	}

	int TreeView::getNumberOfRootNodes()
	{
		return static_cast<int>(mNodes.size());
	}

	Ogre::String TreeView::getMinimizeSkinTypeName()
	{
		return mSkinType->getComponentType(MINIMIZE)->typeName;
	}

	std::list<TreeViewNode*> TreeView::getRootNodes()
	{
		std::list<TreeViewNode*> list(mNodes);

		return list;
	}

	TreeViewNode* TreeView::getSelectedNode()
	{
		return mSelectedNode;
	}

	void TreeView::onDraw()
	{
		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mWidgetDesc->widget_brushFilterMode);
			
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));

		Rect r = brush->getClipRegion();

		brush->setClipRegion(r);
	}

	void TreeView::onMouseDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			selectNode(NULL);
	}

	void TreeView::setDoubleClickToToggle(bool enable)
	{
		mDesc->treeview_doubleClickToToggle = enable;
	}

	void TreeView::setKeyNavigation(bool enable)
	{
		mDesc->treeview_keyNavigation = enable;
	}

	void TreeView::selectNode(TreeViewNode* n)
	{
		mSelectedNode = n;
		mDesc->sheet->setKeyboardListener(n);

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->_selectNode(n);

		// Scroll node into view.  Node may be above or below current view, 
		// so make sure both top and bottom are visible.
		if(n != NULL)
		{
			Point p = n->getPositionRelativeToTreeView();
			
			// scroll top into view
			scrollPointIntoView(p);

			// scroll bottom into view
			p.x += n->getWidth();
			p.y += n->getHeight();
			scrollPointIntoView(p);
		}

		redraw();

		WidgetEventArgs wea(this);
		fireTreeViewEvent(TREEVIEW_EVENT_SELECTION_CHANGED,wea);
	}

	void TreeView::setNodeHeight(float nodeHeightInPixels)
	{
		nodeHeightInPixels = Ogre::Math::Floor(nodeHeightInPixels + 0.5);

		mDesc->treeview_nodeHeight = nodeHeightInPixels;

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->setHeight(nodeHeightInPixels);

		updateNodes();
	}

	void TreeView::updateNodes()
	{
		float x = 0;
		float y = 0;
		unsigned int counter = 0;
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			(*it)->setPosition(Point(x,y));
			(*it)->mIndex = counter;
			(*it)->updateNodes();

			y += (*it)->getBranchHeight();
			++counter;
		}

		mTotalHeight = y;

		_determineVirtualSize();
		_updateScrollBars();
	}
}
