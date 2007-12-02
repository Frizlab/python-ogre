#ifndef QUICKGUIQUADCONTAINER_H
#define QUICKGUIQUADCONTAINER_H

#include "OgreBitwise.h"
#include "OgrePrerequisites.h"

#include "QuickGUIPrerequisites.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIQuad.h"
#include "QuickGUIVertexBuffer.h"

#include <list>
#include <vector>


//vertices per quad = 25 quads initially
#define MIN_VERTEX_BUFFER_SIZE (25*VERTICES_PER_QUAD)

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
		std::vector<unsigned int> mChildBufferSize;
		std::vector<unsigned int> mMenuBufferSize;

		bool mChildrenChanged;
		bool mMenuChanged;

		// These variables order relate to zOrdering
		Quad* mBaseQuad;
		QuadList mChildRenderables;
		QuadContainerList mChildPanels;
		// Only Sheet widgets will make use of Window List
		QuadContainerList mChildWindows;
		QuadList mMenuRenderables;	// Highest zOrder of this group

		QuadContainerList* getPanelList();
		QuadContainerList* getWindowList();

		void _populateRenderObjectList();
		void _updateRenderQueue();

	private:
	};
}

#endif
