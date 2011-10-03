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

#include "QuickGUIListTextItem.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"

namespace QuickGUI
{
	void ListTextItem::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ListTextItem");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ListTextItem",d);
	}

	ListTextItemDesc::ListTextItemDesc() :
		ListItemDesc(),
		TextUserDesc()
	{
		resetToDefault();
	}

	void ListTextItemDesc::resetToDefault()
	{
		ListItemDesc::resetToDefault();
		
		TextUserDesc::resetToDefault();
	}

	void ListTextItemDesc::serialize(SerialBase* b)
	{
		ListItemDesc::serialize(b);

		TextUserDesc::serialize(b);
	}

	ListTextItem::ListTextItem(const Ogre::String& name) :
		ListItem(name),
		TextUser()
	{
	}

	ListTextItem::~ListTextItem()
	{
	}

	void ListTextItem::_initialize(WidgetDesc* d)
	{
		ListItem::_initialize(d);

		mDesc = dynamic_cast<ListTextItemDesc*>(mWidgetDesc);

		ListTextItemDesc* ltid = dynamic_cast<ListTextItemDesc*>(d);

		setSkinType(d->widget_skinTypeName);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = ltid->textDesc;
		TextUser::_initialize(this,mDesc);
	}

	Ogre::String ListTextItem::getClass()
	{
		return "ListTextItem";
	}

	void ListTextItem::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Clip to client dimensions
		Rect clipRegion(mClientDimensions);
		clipRegion.translate(mTexturePosition);

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		clipRegion = mClientDimensions;
		clipRegion.translate(mTexturePosition);		

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void ListTextItem::updateClientDimensions()
	{
		ListItem::updateClientDimensions();

		if(mText != NULL)
			mText->setAllottedHeight(mClientDimensions.size.height);

		redraw();
	}
}
