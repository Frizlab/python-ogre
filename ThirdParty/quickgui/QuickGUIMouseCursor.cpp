/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

#include "QuickGUIMouseCursor.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUISkinType.h"
#include "QuickGUISkinTypeManager.h"
#include "QuickGUIBrush.h"
#include "QuickGUIEventHandlerManager.h"
#include "QuickGUISheet.h"

#include "OgreImage.h"

namespace QuickGUI
{
	const Ogre::String MouseCursor::TEXTURE = "texture";

	MouseCursorDesc::MouseCursorDesc()
	{
		clipOnEdges = false;
		enabled = true;
		opacity = 1.0;
		queryFilter = -1;
		skin = "qgui";
		visible = true;
		guiManager = NULL;
		brushFilterMode = BRUSHFILTER_NONE;
	}

	void MouseCursor::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("MouseCursor");
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

		// Update Size of Cursor to match Skin Texture
		Ogre::Image i;
		i.load(mSkinTypeManager->getSkinType("MouseCursor",mSkinType)->getSkinElement(TEXTURE)->getTextureName(),Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		// update cursor size to match texture used
		setSize(i.getWidth(),i.getHeight());

		// Set default position
		setPosition(0,0);
	}

	MouseCursor::~MouseCursor()
	{
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

		Brush* b = Brush::getSingletonPtr();
		b->setColor(ColourValue(1,1,1,mMouseCursorDesc.opacity));
		b->setRenderTarget(static_cast<Ogre::Viewport*>(NULL));
		b->setFilterMode(mMouseCursorDesc.brushFilterMode);
		b->drawSkinElement(mDimensions,mSkinTypeManager->getSkinType("MouseCursor",mSkinType)->getSkinElement(TEXTURE));
	}

	BrushFilterMode MouseCursor::getBrushFilterMode()
	{
		return mMouseCursorDesc.brushFilterMode;
	}

	bool MouseCursor::getClipOnEdges()
	{
		return mMouseCursorDesc.clipOnEdges;
	}

	bool MouseCursor::getEnabled()
	{
		return mMouseCursorDesc.enabled;
	}

	float MouseCursor::getOpacity()
	{
		return mMouseCursorDesc.opacity;
	}

	Point MouseCursor::getPosition()
	{
		return Point(mDimensions.position.x + (mDimensions.size.width/2.0),mDimensions.position.y + (mDimensions.size.height/2.0));
	}

	unsigned int MouseCursor::getQueryFilter()
	{
		return mMouseCursorDesc.queryFilter;
	}

	Ogre::String MouseCursor::getSkinTypeName()
	{
		return mDefaultSkinType;
	}

	bool MouseCursor::getVisible()
	{
		return mMouseCursorDesc.visible;
	}

	void MouseCursor::setBrushFilterMode(BrushFilterMode m)
	{
		mMouseCursorDesc.brushFilterMode = m;
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
		Sheet* sheet = mMouseCursorDesc.guiManager->getActiveSheet();
		if(sheet != NULL)
		{
			MouseEventArgs args(sheet);
			args.position = getPosition();

			sheet->fireSheetEvent(SHEET_EVENT_MOUSE_CURSOR_ENABLED_CHANGED,args);
		}
	}

	void MouseCursor::setOpacity(float opacity)
	{
		if(opacity < 0.0)
			opacity = 0.0;
		else if (opacity > 1.0)
			opacity = 1.0;

		mMouseCursorDesc.opacity = opacity;
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
			Sheet* sheet = mMouseCursorDesc.guiManager->getActiveSheet();
			if(sheet != NULL)
			{
				MouseEventArgs args(sheet);
				args.position = getPosition();

				sheet->fireSheetEvent(SHEET_EVENT_MOUSE_CURSOR_ENTER_SHEET_BORDER,args);
			}
		}

		if(fireBorderLeaveEvent)
		{
			Sheet* sheet = mMouseCursorDesc.guiManager->getActiveSheet();
			if(sheet != NULL)
			{
				MouseEventArgs args(sheet);
				args.position = getPosition();

				sheet->fireSheetEvent(SHEET_EVENT_MOUSE_CURSOR_LEAVE_SHEET_BORDER,args);
			}
		}
	}

	void MouseCursor::setPosition(const Point& p)
	{
		setPosition(p.x,p.y);
	}

	void MouseCursor::setQueryFilter(unsigned int filter)
	{
		mMouseCursorDesc.queryFilter = filter;

		Sheet* sheet = mMouseCursorDesc.guiManager->getActiveSheet();
		if(sheet != NULL)
		{
			MouseEventArgs args(sheet);
			args.position = getPosition();
		
			sheet->fireSheetEvent(SHEET_EVENT_MOUSE_CURSOR_QUERY_FILTER_CHANGED,args);
		}
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

		Sheet* sheet = mMouseCursorDesc.guiManager->getActiveSheet();
		if(sheet != NULL)
		{
			MouseEventArgs args(sheet);
			args.position = getPosition();
			sheet->fireSheetEvent(SHEET_EVENT_MOUSE_CURSOR_SKIN_CHANGED,args);
		}
	}

	void MouseCursor::setVisible(bool visible)
	{
		mMouseCursorDesc.visible = visible;
	}
}
