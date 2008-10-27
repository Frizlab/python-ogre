#include "QuickGUIMouseCursor.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"


namespace QuickGUI
{
	const Ogre::String MouseCursor::TEXTURE = "texture";

	MouseCursorDesc::MouseCursorDesc()
	{
		clipOnEdges = false;
		enabled = true;
		skin = "qgui";
		visible = true;
		guiManager = NULL;
	}

	void MouseCursor::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("MouseCursor");
		d->defineSkinElement(TEXTURE);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("MouseCursor",d);
	}

	MouseCursor::MouseCursor(const MouseCursorDesc& d) :
		mSkinType("default"),
		mDefaultSkinType("default")
	{
		mMouseCursorDesc.guiManager = d.guiManager;
		mSkinTypeManager = SkinTypeManager::getSingletonPtr();

		for(int i = 0; i < 4; ++i)
			mEnteredBorders[i] = false;

		// HARDCODED FOR NOW
		setSize(27,36);
		setPosition(400,300);
	}

	MouseCursor::~MouseCursor()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < MOUSE_CURSOR_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mEventHandlers[index].begin(); it != mEventHandlers[index].end(); ++it)
				delete (*it);
		}
	}

	void MouseCursor::_setSkinType(const Ogre::String type)
	{
		if(mSkinType == type)
			return;

		mSkinType = type;
		
		Ogre::Image i;
		i.load(mSkinTypeManager->getSkinType("MouseCursor",mSkinType)->getSkinElement(TEXTURE)->getTextureName(),Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		// update cursor size to match texture used
		setSize(i.getWidth(),i.getHeight());
	}

	void MouseCursor::addCursorEventHandler(MouseCursorEvent EVENT, EventHandlerSlot* function)
	{
		mEventHandlers[EVENT].push_back(function);
	}

	void MouseCursor::draw()
	{
		// If cursor is not visible, return
		if(!mMouseCursorDesc.visible)
			return;
		
		// If cursor is on a border and clipOnEdges is true, return
		if(mMouseCursorDesc.clipOnEdges)
		{
			for(int i = 0; i < 4; ++i)
			{
				if(mEnteredBorders[i])
				{
					return;
				}
			}
		}

		Brush::getSingletonPtr()->setRenderTarget(NULL);
		Brush::getSingletonPtr()->drawSkinElement(mDimensions,mSkinTypeManager->getSkinType("MouseCursor",mSkinType)->getSkinElement(TEXTURE));
	}

	bool MouseCursor::getClipOnEdges()
	{
		return mMouseCursorDesc.clipOnEdges;
	}

	bool MouseCursor::getEnabled()
	{
		return mMouseCursorDesc.enabled;
	}

	Point MouseCursor::getPosition()
	{
		return Point(mDimensions.position.x + (mDimensions.size.width/2.0),mDimensions.position.y + (mDimensions.size.height/2.0));
	}

	bool MouseCursor::getVisible()
	{
		return mMouseCursorDesc.visible;
	}

	void MouseCursor::setClipOnEdges(bool clip)
	{
		mMouseCursorDesc.clipOnEdges = clip;
	}

	void MouseCursor::setEnabled(bool enable)
	{
		if(enable == mMouseCursorDesc.enabled)
			return;

		mMouseCursorDesc.enabled = enable;

		// Fire enabled changed event.
		MouseEventArgs args(NULL);
		args.position = getPosition();

		for(std::vector<EventHandlerSlot*>::iterator it = mEventHandlers[MOUSE_CUSSOR_EVENT_ENABLED_CHANGED].begin(); it != mEventHandlers[MOUSE_CUSSOR_EVENT_ENABLED_CHANGED].end(); ++it)
			(*it)->execute(args);
	}

	void MouseCursor::setPosition(float xPosition, float yPosition)
	{
		mDimensions.position.x = xPosition - (mDimensions.size.width/2.0);
		mDimensions.position.y = yPosition - (mDimensions.size.height/2.0);

		bool fireBorderEnterEvent = false;
		bool fireBorderLeaveEvent = false;

		// If cursor horizontal position is on left edge of viewport
		if(xPosition <= 0)
		{
			// If we haven't entered the border already, fire event
			if(!mEnteredBorders[0])
			{
				mEnteredBorders[0] = true;
				fireBorderEnterEvent = true;
			}
		}
		// Else if cursor horizontal position is on right edge of viewport
		else if(xPosition >= (mMouseCursorDesc.guiManager->getViewport()->getActualWidth() - 1))
		{
			// If we haven't entered the border already, fire event
			if(!mEnteredBorders[2])
			{
				mEnteredBorders[2] = true;
				fireBorderEnterEvent = true;
			}
		}
		// Else cursor horizontal position lies in between left and right edge of viewport
		else
		{
			if(mEnteredBorders[0] || mEnteredBorders[2])
			{
				mEnteredBorders[0] = false;
				mEnteredBorders[2] = false;
				fireBorderLeaveEvent = true;
			}
		}

		// If cursor vertical position is on top edge of viewport
		if(yPosition <= 0)
		{
			// If we haven't entered the border already, fire event
			if(!mEnteredBorders[1])
			{
				mEnteredBorders[1] = true;
				fireBorderEnterEvent = true;
			}
		}
		// Else if cursor vertical position is on bottom edge of viewport
		else if(yPosition >= (mMouseCursorDesc.guiManager->getViewport()->getActualHeight() - 1))
		{
			// If we haven't entered the border already, fire event
			if(!mEnteredBorders[3])
			{
				mEnteredBorders[3] = true;
				fireBorderEnterEvent = true;
			}
		}
		// Else cursor vertical position lies in between top and bottom edge of viewport
		else
		{
			if(mEnteredBorders[1] || mEnteredBorders[3])
			{
				mEnteredBorders[1] = false;
				mEnteredBorders[3] = false;
				fireBorderLeaveEvent = true;
			}
		}

		// Fire events
		if(fireBorderEnterEvent)
		{
			MouseEventArgs args(NULL);
			args.position = getPosition();

			for(std::vector<EventHandlerSlot*>::iterator it = mEventHandlers[MOUSE_CURSOR_EVENT_BORDER_ENTER].begin(); it != mEventHandlers[MOUSE_CURSOR_EVENT_BORDER_ENTER].end(); ++it)
				(*it)->execute(args);
		}

		if(fireBorderLeaveEvent)
		{
			MouseEventArgs args(NULL);
			args.position = getPosition();

			for(std::vector<EventHandlerSlot*>::iterator it = mEventHandlers[MOUSE_CURSOR_EVENT_BORDER_LEAVE].begin(); it != mEventHandlers[MOUSE_CURSOR_EVENT_BORDER_LEAVE].end(); ++it)
				(*it)->execute(args);
		}
	}

	void MouseCursor::setPosition(const Point& p)
	{
		setPosition(p.x,p.y);
	}

	void MouseCursor::setSize(float pixelWidth, float pixelHeight)
	{
		Point currentPosition = getPosition();

		mDimensions.size.width = pixelWidth;
		mDimensions.size.height = pixelHeight;

		mDimensions.position.x = currentPosition.x - (mDimensions.size.width/2.0);
		mDimensions.position.y = currentPosition.y - (mDimensions.size.height/2.0);
	}

	void MouseCursor::setSkinType(const Ogre::String type)
	{
		mSkinType = type;
		mDefaultSkinType = type;

		Ogre::Image i;
		i.load(mSkinTypeManager->getSkinType("MouseCursor",mSkinType)->getSkinElement(TEXTURE)->getTextureName(),Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		// update cursor size to match texture used
		setSize(i.getWidth(),i.getHeight());
	}

	void MouseCursor::setVisible(bool visible)
	{
		mMouseCursorDesc.visible = visible;
	}
}
