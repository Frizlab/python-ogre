#include "QuickGUIQuadContainer.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	QuadContainer::QuadContainer(Widget* owner) :
		mOwner(owner),
		mParentContainer(mOwner->getQuadContainer()),
		mChildrenChanged(false),
		mMenuChanged(false),
		mID(mOwner->getInstanceName() + ".QuadContainer")
	{
		mBaseQuad = mOwner->getQuad();
		mChildBufferSize.push_back(MIN_VERTEX_BUFFER_SIZE);
		mMenuBufferSize.push_back(MIN_VERTEX_BUFFER_SIZE);

		mChildVertexBuffer = new VertexBuffer(MIN_VERTEX_BUFFER_SIZE,mOwner->getGUIManager());
		mChildVertexBuffer->setData(&mChildRenderables);

		mMenuVertexBuffer = new VertexBuffer(MIN_VERTEX_BUFFER_SIZE,mOwner->getGUIManager());
		mMenuVertexBuffer->setData(&mMenuRenderables);
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

	void QuadContainer::addChildRenderable(Quad* o)
	{
		std::list<Quad*>::iterator it;
		it = mChildRenderables.begin();
		// run until:
		// 1) end of list
		// 2) zOrder Matches
		while( (it != mChildRenderables.end()) && ((*it)->getOffset() < o->getOffset()) )
			++it;

		if( it != mChildRenderables.end() )
		{
			// run until:
			// 1) end of list
			// 2) Higher zOrder found
			// 3) we find Quad with same texture
			while( (it != mChildRenderables.end()) && ((*it)->getOffset() == o->getOffset()) && ((*it)->getTextureName() != o->getTextureName()) )
				++it;
		}

		mChildRenderables.insert(it,o);
		o->_notifyChangesHandled();
		o->_notifyAddedToRenderObjectGroup();

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
		if( (mOwner != NULL) && (mOwner->getWidgetType() != Widget::TYPE_SHEET) ) return;

		std::list<QuadContainer*>::iterator it = mChildWindows.begin();
		while( (it != mChildWindows.end()) && ((*it)->getOffset() <= g->getOffset()) )
			++it;
		
		mChildWindows.insert(it,g);
	}

	void QuadContainer::addMenuRenderable(Quad* o)
	{
		std::list<Quad*>::iterator it;
		it = mMenuRenderables.begin();
		// run until:
		// 1) end of list
		// 2) zOrder Matches
		while( (it != mMenuRenderables.end()) && ((*it)->getOffset() < o->getOffset()) )
			++it;

		if( it != mMenuRenderables.end() )
		{
			// run until:
			// 1) end of list
			// 2) Higher zOrder found
			// 3) we find Quad with same texture
			while( (it != mMenuRenderables.end()) && ((*it)->getOffset() == o->getOffset()) && ((*it)->getTextureName() != o->getTextureName()) )
				++it;
		}

		mMenuRenderables.insert(it,o);

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

	Ogre::String QuadContainer::getID()
	{
		return mID;
	}

	int QuadContainer::getOffset()
	{
		return mBaseQuad->getOffset();
	}

	void QuadContainer::moveWindowGroupToEnd(QuadContainer* g)
	{
		if( (mOwner != NULL) && (mOwner->getWidgetType() != Widget::TYPE_WINDOW) ) return;

		bool windowInList = false;

		std::list<QuadContainer*>::iterator it = mChildWindows.begin();
		while( (it != mChildWindows.end()) && ((*it)->getOffset() <= g->getOffset()) )
		{
			if( (*it)->getID() == g->getID() )
			{
				mChildWindows.erase(it);
				windowInList = true;
				break;
			}
		}
		
		if(windowInList) mChildWindows.push_back(g);
	}

	void QuadContainer::notifyChildRenderableChanged(Quad* o)
	{
		if(o->offsetChanged() || o->textureChanged())
		{
			removeChildRenderable(o->getID());
			addChildRenderable(o);
		}

		mChildrenChanged = true;
	}

	void QuadContainer::notifyMenuRenderableChanged(Quad* o)
	{
		if(o->offsetChanged() || o->textureChanged())
		{
			removeMenuRenderable(o->getID());
			addMenuRenderable(o);
		}

		mMenuChanged = true;
	}

	void QuadContainer::removeChildRenderable(const Ogre::String& id)
	{
		std::list<Quad*>::iterator it;
		for( it = mChildRenderables.begin(); it != mChildRenderables.end(); ++it )
		{
			if( (*it)->getID() == id )
			{
				(*it)->_notifyRemovedFromQuadContainer();
				mChildRenderables.erase(it);
				mChildrenChanged = true;
				return;
			}
		}
	}

	void QuadContainer::removeChildPanelContainer(const Ogre::String& id)
	{
		std::list<QuadContainer*>::iterator it;
		for( it = mChildPanels.begin(); it != mChildPanels.end(); ++it )
		{
			if( (*it)->getID() == id )
			{
				mChildPanels.erase(it);
				return;
			}
		}
	}

	void QuadContainer::removeChildWindowContainer(const Ogre::String& id)
	{
		std::list<QuadContainer*>::iterator it;
		for( it = mChildWindows.begin(); it != mChildWindows.end(); ++it )
		{
			if( (*it)->getID() == id )
			{
				mChildWindows.erase(it);
				return;
			}
		}
	}

	void QuadContainer::removeMenuRenderable(const Ogre::String& id)
	{
		std::list<Quad*>::iterator it;
		for( it = mMenuRenderables.begin(); it != mMenuRenderables.end(); ++it )
		{
			if( (*it)->getID() == id )
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
