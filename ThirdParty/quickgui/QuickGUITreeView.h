#ifndef QUICKGUITREEVIEW_H
#define QUICKGUITREEVIEW_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUITreeViewNode.h"
#include "QuickGUITreeViewCheckBoxNode.h"
#include "QuickGUITreeViewRadioButtonNode.h"

#include <list>

namespace QuickGUI
{
	class _QuickGUIExport TreeViewDesc :
		public ContainerWidgetDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
// 	protected:
		TreeViewDesc(const Ogre::String& id);
		virtual ~TreeViewDesc() {}
	public:

		bool treeview_doubleClickToToggle;
		bool treeview_keyNavigation;
		float treeview_nodeHeight;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TreeViewDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TreeView"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TreeView :
		public ContainerWidget
	{
	public:
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String EXPAND;
		static const Ogre::String MINIMIZE;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class Factory<Widget>;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: WIDGET_EVENT_SIZE_CHANGED, WIDGET_EVENT_MOUSE_WHEEL, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::WIDGET_EVENT_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addTreeViewEventHandler(TreeViewEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addTreeViewEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addTreeViewEventHandler(TreeViewEvent EVENT, EventHandlerSlot* function);

		/**
		* Creates a child TreeViewNode.
		*/
		virtual TreeViewNode* createNode(TreeViewNodeDesc* d);
		/**
		* Adds a TreeViewNode to the List at the index given.
		* NOTE: if the index is not valid, the ListItem is added to the end of the list.
		*/
		virtual TreeViewNode* createNode(int index, TreeViewNodeDesc* d);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireTreeViewEvent(TreeViewEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns true if double clicking on a node will toggle its state, false otherwise.
		*/
		bool getDoubleClickToToggle();
		/**
		* Returns the name of the skinType used to draw nodes in expanded form.
		*/
		Ogre::String getExpandSkinTypeName();
		/**
		* Returns true if up/down keys will select previous/next node and left/right expands/minimizes node, false otherwise.
		*/
		bool getKeyNavigation();
		/**
		* Returns the name of the skinType used to draw nodes in minimized form.
		*/
		Ogre::String getMinimizeSkinTypeName();
		/**
		* Gets the height of every created or existing Node in this TreeView.
		*/
		float getNodeHeight();
		/**
		* Returns the number of root nodes in this TreeView.
		*/
		int getNumberOfRootNodes();
		/**
		* Returns a list of root nodes in this tree.
		*/
		std::list<TreeViewNode*> getRootNodes();
		/**
		* Returns the currently selected Node, or NULL if no node is selected.
		*/
		TreeViewNode* getSelectedNode();
		
		/**
		* Sets whether double clicking on a node will toggle is state.
		*/
		void setDoubleClickToToggle(bool enable);
		/**
		* If set, up/down keys will select previous/next node and left/right expands/minimizes node.
		*/
		void setKeyNavigation(bool enable);
		/**
		* Selects a Node in the tree.
		*/
		void selectNode(TreeViewNode* n);
		/**
		* Sets the height of every created or existing Node in this TreeView.
		*/
		void setNodeHeight(float nodeHeightInPixels);

		/**
		* Recursively updates node names and positions.
		*/
		void updateNodes();

	protected:
		TreeView(const Ogre::String& name);
		virtual ~TreeView();

		TreeViewDesc* mDesc;

		int mAutoNameCounter;

		float mTotalHeight;

		std::list<TreeViewNode*> mNodes;

		TreeViewNode* mSelectedNode;

		// TreeViewRadioButtonNode specific event handlers
		std::vector<EventHandlerSlot*> mTreeViewEventHandlers[TREEVIEW_EVENT_COUNT];

		// Iterates through children to determine VirtualSpace
		virtual void _determineVirtualSize();
		/**
		* Recursively calls children and returns the largest relative x position of any child nodes.
		*/
		float _getFarthestX();

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
		/**
		* Clears selection.
		*/
		void onMouseDown(const EventArgs& args);

	private:
	};
}

#endif
