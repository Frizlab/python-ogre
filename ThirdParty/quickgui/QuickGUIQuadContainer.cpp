#include "QuickGUIQuadContainer.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	QuadContainer::QuadContainer(Widget* owner) :
		mOwner(owner),
		mChildrenChanged(false),
		mMenuChanged(false)
	{
		mChildBufferSize.push_back(MIN_VERTEX_BUFFER_SIZE);
		mMenuBufferSize.push_back(MIN_VERTEX_BUFFER_SIZE);

		mChildVertexBuffer = new VertexBuffer(MIN_VERTEX_BUFFER_SIZE,mOwner->getGUIManager());
		mChildVertexBuffer->setData(&mChildRenderables);

		mMenuVertexBuffer = new VertexBuffer(MIN_VERTEX_BUFFER_SIZE,mOwner->getGUIManager());
		mMenuVertexBuffer->setData(&mMenuRenderables);

		mBaseQuad = mOwner->getQuad();
		mBaseQuad->_notifyQuadContainer(this);
	}

	QuadContainer::~QuadContainer()
	{
		delete mChildVertexBuffer;
		delete mMenuVertexBuffer;

		std::list<Quad*>::iterator cIt;
		for( cIt = mChildRenderables.begin(); cIt != mChildRenderables.end(); ++cIt )
			(*cIt)->_notifyRemovedFromQuadContainer();

		mChildPanels.clear();

		mChildWindows.clear();

		std::list<Quad*>::iterator mIt;
		for( mIt = mMenuRenderables.begin(); mIt != mMenuRenderables.end(); ++mIt )
			(*mIt)->_notifyRemovedFromQuadContainer();
	}

	void QuadContainer::addChildRenderable(Quad* q)
	{
		std::list<Quad*>::iterator it;
		it = mChildRenderables.begin();
		// run until:
		// 1) end of list
		// 2) zOrder Matches
		while( (it != mChildRenderables.end()) && ((*it)->getOffset() < q->getOffset()) )
			++it;

		if( it != mChildRenderables.end() )
		{
			// run until:
			// 1) end of list
			// 2) Higher zOrder found
			// 3) we find Quad with same texture
			while( (it != mChildRenderables.end()) && ((*it)->getOffset() == q->getOffset()) && ((*it)->getTextureName() != q->getTextureName()) )
				++it;
		}

		mChildRenderables.insert(it,q);
		q->_notifyChangesHandled();
		q->_notifyAddedToRenderObjectGroup();

		mChildrenChanged = true;

		// check if buffer needs to be resized.
		size_t numQuads = static_cast<size_t>(mChildRenderables.size());
		size_t numVertices = numQuads * 6;
		if( numVertices > mChildBufferSize.back() )
		{
			Ogre::uint32 newBufferSize = Ogre::Bitwise::firstPO2From(static_cast<Ogre::uint32>(numVertices));
			mChildBufferSize.push_back(newBufferSize);
			mChildVertexBuffer->resizeVertexBuffer(mChildBufferSize.back());
		}
		else if( (static_cast<int>(mChildBufferSize.size()) > 1) && (numVertices < mChildBufferSize[static_cast<int>(mChildBufferSize.size()) - 2]) )
		{
			mChildBufferSize.pop_back();
			mChildVertexBuffer->resizeVertexBuffer(mChildBufferSize.back());
		}
	}

	void QuadContainer::addChildPanelContainer(QuadContainer* g)
	{
		std::list<QuadContainer*>::iterator it = mChildPanels.begin();
		while( (it != mChildPanels.end()) && ((*it)->getOffset() < g->getOffset()) )
			++it;
		
		mChildPanels.insert(it,g);
	}

	void QuadContainer::addChildWindowContainer(QuadContainer* g)
	{
		if( (mOwner != NULL) && (mOwner->getWidgetType() != Widget::TYPE_SHEET) ) 
			return;

		std::list<QuadContainer*>::iterator it = mChildWindows.begin();
		while( (it != mChildWindows.end()) && ((*it)->getOffset() <= g->getOffset()) )
			++it;
		
		mChildWindows.insert(it,g);
	}

	void QuadContainer::addMenuRenderable(Quad* q)
	{
		std::list<Quad*>::iterator it;
		it = mMenuRenderables.begin();
		// run until:
		// 1) end of list
		// 2) zOrder Matches
		while( (it != mMenuRenderables.end()) && ((*it)->getOffset() < q->getOffset()) )
			++it;

		if( it != mMenuRenderables.end() )
		{
			// run until:
			// 1) end of list
			// 2) Higher zOrder found
			// 3) we find Quad with same texture
			while( (it != mMenuRenderables.end()) && ((*it)->getOffset() == q->getOffset()) && ((*it)->getTextureName() != q->getTextureName()) )
				++it;
		}

		mMenuRenderables.insert(it,q);
		q->_notifyChangesHandled();
		q->_notifyAddedToRenderObjectGroup();

		mMenuChanged = true;

		// check if buffer needs to be resized.
		size_t numQuads = static_cast<size_t>(mMenuRenderables.size());
		size_t numVertices = numQuads * 6;
		if( numVertices > mMenuBufferSize.back() )
		{
			Ogre::uint32 newBufferSize = Ogre::Bitwise::firstPO2From(static_cast<Ogre::uint32>(numVertices));
			mMenuBufferSize.push_back(newBufferSize);
			mMenuVertexBuffer->resizeVertexBuffer(mMenuBufferSize.back());
		}
		else if( (static_cast<int>(mMenuBufferSize.size()) > 1) && (numVertices < mMenuBufferSize[static_cast<int>(mMenuBufferSize.size()) - 2]) )
		{
			mMenuBufferSize.pop_back();
			mMenuVertexBuffer->resizeVertexBuffer(mMenuBufferSize.back());
		}
	}

	int QuadContainer::getOffset()
	{
		return mBaseQuad->getOffset();
	}

	Widget* QuadContainer::getOwner()
	{
		return mOwner;
	}

	std::list<QuadContainer*>* QuadContainer::getPanelList()
	{
		return &mChildPanels;
	}

	std::list<QuadContainer*>* QuadContainer::getWindowList()
	{
		return &mChildWindows;
	}

	void QuadContainer::moveWindowGroupToEnd(QuadContainer* c)
	{
		if( (c->getOwner()->getWidgetType() != Widget::TYPE_WINDOW) && (c != mChildWindows.back()) ) 
			return;

		bool windowInList = false;

		for( std::list<QuadContainer*>::iterator it = mChildWindows.begin(); it != mChildWindows.end(); ++it )
		{
			if( (*it) == c )
			{
				mChildWindows.erase(it);
				windowInList = true;
				break;
			}
		}
		
		if(windowInList) 
			mChildWindows.push_back(c);
	}

	void QuadContainer::notifyChildRenderableChanged(Quad* q)
	{
		if(q->offsetChanged() || q->textureChanged())
		{
			removeChildRenderable(q);
			addChildRenderable(q);
		}

		mChildrenChanged = true;
	}

	void QuadContainer::notifyMenuRenderableChanged(Quad* q)
	{
		if(q->offsetChanged() || q->textureChanged())
		{
			removeMenuRenderable(q);
			addMenuRenderable(q);
		}

		mMenuChanged = true;
	}

	void QuadContainer::removeChildRenderable(Quad* q)
	{
		std::list<Quad*>::iterator it;
		for( it = mChildRenderables.begin(); it != mChildRenderables.end(); ++it )
		{
			if( (*it) == q )
			{
				(*it)->_notifyRemovedFromQuadContainer();
				mChildRenderables.erase(it);
				mChildrenChanged = true;
				return;
			}
		}
	}

	void QuadContainer::removeChildPanelContainer(QuadContainer* c)
	{
		std::list<QuadContainer*>::iterator it;
		for( it = mChildPanels.begin(); it != mChildPanels.end(); ++it )
		{
			if( (*it) == c )
			{
				mChildPanels.erase(it);
				return;
			}
		}
	}

	void QuadContainer::removeChildWindowContainer(QuadContainer* c)
	{
		std::list<QuadContainer*>::iterator it;
		for( it = mChildWindows.begin(); it != mChildWindows.end(); ++it )
		{
			if( (*it) == c )
			{
				mChildWindows.erase(it);
				return;
			}
		}
	}

	void QuadContainer::removeMenuRenderable(Quad* q)
	{
		std::list<Quad*>::iterator it;
		for( it = mMenuRenderables.begin(); it != mMenuRenderables.end(); ++it )
		{
			if( (*it) == q )
			{
				mMenuRenderables.erase(it);
				mMenuChanged = true;
				return;
			}
		}
	}

	void QuadContainer::render()
	{
		if(mChildrenChanged)
		{
			mChildVertexBuffer->update();
			mChildrenChanged = false;
		}
		if(mMenuChanged)
		{
			mMenuVertexBuffer->update();
			mMenuChanged = false;
		}

		mChildVertexBuffer->render();

		std::list<QuadContainer*>::iterator it;
		for( it = mChildPanels.begin(); it != mChildPanels.end(); ++it )
			(*it)->render();

		for( it = mChildWindows.begin(); it != mChildWindows.end(); ++it )
			(*it)->render();

		mMenuVertexBuffer->render();
	}
}
