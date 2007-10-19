#ifndef QUICKGUIQUADCONTAINER_H
#define QUICKGUIQUADCONTAINER_H

#include "OgreBitwise.h"
#include "OgrePrerequisites.h"

#include "QuickGUIExportDLL.h"
#include "QuickGUIQuad.h"
#include "QuickGUIVertexBuffer.h"

#include <list>
#include <vector>

// 150 vertices / 6 vertices per quad = 25 quads initially
#define MIN_VERTEX_BUFFER_SIZE 150

namespace QuickGUI
{
	// Forward declarations
	class Widget;

	class _QuickGUIExport QuadContainer
	{
	public:
		QuadContainer(Widget* owner);
		~QuadContainer();
	
		void addChildRenderable(Quad* q);
		void addChildPanelContainer(QuadContainer* g);
		void addChildWindowContainer(QuadContainer* g);
		void addMenuRenderable(Quad* q);

		/**
		* Returns the number of parent iterations required to get to Sheet widget.
		*/
		int getOffset();
		Widget* getOwner();

		/**
		* Removes the Window Group from its current location and appends to the end
		* of the list.  This is essentially bringing the window to the front of the screen.
		* (highest window zOrder)
		*/
		void moveWindowGroupToEnd(QuadContainer* c);

		void notifyChildRenderableChanged(Quad* q);
		void notifyMenuRenderableChanged(Quad* q);

		void removeChildRenderable(Quad* q);
		void removeChildPanelContainer(QuadContainer* c);
		void removeChildWindowContainer(QuadContainer* c);
		void removeMenuRenderable(Quad* q);
		void render();

	protected:
		Widget* mOwner;

		VertexBuffer* mChildVertexBuffer;
		VertexBuffer* mMenuVertexBuffer;
		// stores a list of all buffer sizes, to allow easy shrinking and growing.
		std::vector<size_t> mChildBufferSize;
		std::vector<size_t> mMenuBufferSize;

		bool mChildrenChanged;
		bool mMenuChanged;

		// These variables order relate to zOrdering
		Quad* mBaseQuad;
		std::list<Quad*> mChildRenderables;
		std::list<QuadContainer*> mChildPanels;
		// Only Sheet widgets will make use of Window List
		std::list<QuadContainer*> mChildWindows;
		std::list<Quad*> mMenuRenderables;	// Highest zOrder of this group

		std::list<QuadContainer*>* getPanelList();
		std::list<QuadContainer*>* getWindowList();

		void _populateRenderObjectList();
		void _updateRenderQueue();

	private:
	};
}

#endif
