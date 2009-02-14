#ifndef QUICKGUITREEVIEWNODE_H
#define QUICKGUITREEVIEWNODE_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUIText.h"
#include "QuickGUIButton.h"

#include <list>

namespace QuickGUI
{
	// Forward declaration
	class TreeView;

	class _QuickGUIExport TreeViewNodeDesc :
		public ContainerWidgetDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
// 	protected:
		TreeViewNodeDesc(const Ogre::String& id);
		virtual ~TreeViewNodeDesc() {}
	public:

		bool treeviewnode_selected;

		/// Describes the Text used in this Label
		TextDesc textDesc;

		// Nodes should store a pointer to the Tree they belong to
		TreeView* treeView;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TreeViewNodeDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TreeViewNode"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TreeViewNode :
		public ContainerWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String ICON;
		static const Ogre::String SELECTED;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// TreeView has to set positions, toggle visibility to manage ListItems
		friend class TreeView;

		friend class Factory<Widget>;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Creates a child TreeViewNode.
		*/
		TreeViewNode* createNode(TreeViewNodeDesc* d);
		/**
		* Adds a TreeViewNode to the List at the index given.
		* NOTE: if the index is not valid, the ListItem is added to the end of the list.
		* NOTE: Names are auto-generated in the form of <TreeViewName>+<Index>.
		*/
		TreeViewNode* createNode(int index, TreeViewNodeDesc* d);

		/**
		* Expands the node, showing all immediate children. 
		* NOTE: Any parents that aren't visible will be shown.
		*/
		void expand();

		/**
		* Returns any child or component widgets under the position specified, or NULL if
		* no child or component widgets are under the position.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, bool ignoreDisabled = true);

		/**
		* Returns the total height of this branch.  Only visible Nodes are used to
		* calculate the height.
		*/
		float getBranchHeight();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the index of this node.
		*/
		int getIndex();
		/**
		* Returns node below this one at the same level, or NULL if this is the last node.
		*/
		TreeViewNode* getNextSibling();
		/**
		* Returns the number of nodes belonging to this node.
		*/
		int getNumberOfSiblings();
		/**
		* Recursive function that returns the last visible node found.
		*/
		TreeViewNode* getLastVisibleNode();
		/**
		* Returns node above this one at the same level, or NULL if this is the first node.
		*/
		TreeViewNode* getPreviousSibling();
		/**
		* Returns all sibling nodes on the same level.
		* NOTE: This node is also included in the list.
		*/
		std::list<TreeViewNode*> getSiblings();
		/**
		* Returns the position of the node relative to the TreeView.
		*/
		Point getPositionRelativeToTreeView();

		/**
		* Returns true if all child nodes are marked visible, false otherwise.
		* NOTE: Even if nodes are visible, they may not be drawn on the scene, for example if their parent is not visible.
		*/
		bool isExpanded();
		/**
		* Returns true if this node is selected, false otherwise.
		*/
		bool isSelected();

		/**
		* Minimizes the node, hiding all immediate children.
		*/
		void minimize();

		/**
		* Selects the node.
		*/
		void select();

		/**
		* If the Node is minimized it becomes expanded, and vice versa.
		*/
		void toggle();

		/**
		* Recursively updates node names and positions.
		*/
		void updateNodes();

	protected:
		TreeViewNode(const Ogre::String& name);
		virtual ~TreeViewNode();

		TreeViewNodeDesc* mDesc;

		Text* mText;

		Button* mStateButton;
		void createStateButtonIfNotExists();

		int mAutoNameCounter;

		std::list<TreeViewNode*> mNodes;

		bool mExpanded;

		int mIndex;

		/**
		* Recursively calls children and returns the largest relative x position of any child nodes.
		*/
		float _getFarthestX();

		void _selectNode(TreeViewNode* n);

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Uses Brush to draw lines to parent node.
		*/
		void drawLineToParent();
		/**
		* Recursive call telling nodes to draw lines to parent node.
		*/
		void drawLines();

		/**
		* Updates the TreeView Link;
		*/
		virtual void notifyTreeViewParent(TreeView* t);

		/**
		* If enabled, toggles the node.
		*/
		void onDoubleClick(const EventArgs& args);
		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
		/**
		* If enabled, navigates to nodes or expands/minimizes currently selected node.
		*/
		void onKeyDown(const EventArgs& args);
		/**
		* Selects the node. (LMB only)
		*/
		void onMouseDown(const EventArgs& args);
		/**
		* Expands/Minimizes node.
		*/
		void onStateButtonClicked(const EventArgs& args);

		/**
		* Sets the size and position (position relative to parent) of this Widget, respectively.
		*/
		virtual void setDimensions(const Rect& r);
		/**
		* Sets the height of the widget.
		*/
		virtual void setHeight(float pixelHeight);
		/**
		* Sets Horizontal Anchor of this widget. A Left anchor will enforce the widget to maintain
		* its distance from the left side of its parent. A right anchor will enforce the widget to maintain
		* its distance from the right side of its parent. Left and Right Anchor will grow and shrink the
		* widget to maintain distance from both sides.
		*/
		virtual void setHorizontalAnchor(HorizontalAnchor a);
		/**
		* Sets the x and y position of this widget, relative to this widget's parent.
		*/
		virtual void setPosition(const Point& position);
		/**
		* Sets the width and height of this widget.
		*/
		virtual void setSize(const Size& size);
		/**
		* Sets Vertical Anchor of this widget. A Top anchor will enforce the widget to maintain
		* its distance from the top side of its parent. A bottom anchor will enforce the widget to maintain
		* its distance from the bottom side of its parent. Top and Bottom Anchor will grow and shrink the
		* widget to maintain distance from both sides.
		*/
		virtual void setVerticalAnchor(VerticalAnchor a);
		/**
		* Sets whether or not the widget gets drawn.
		*/
		virtual void setVisible(bool visible);
		/**
		* Sets the widget of the widget.
		*/
		virtual void setWidth(float pixelWidth);

	private:
	};
}

#endif
