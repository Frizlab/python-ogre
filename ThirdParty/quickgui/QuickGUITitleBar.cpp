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

#include "QuickGUITitleBar.h"
#include "QuickGUIWindow.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIButton.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	const Ogre::String TitleBar::BACKGROUND = "background";
	const Ogre::String TitleBar::CLOSE_BUTTON = "close_button";

	void TitleBar::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TitleBar");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinReference(CLOSE_BUTTON,"Button");
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TitleBar",d);
	}

	TitleBarDesc::TitleBarDesc() :
		ComponentWidgetDesc(),
		TextUserDesc()
	{
		resetToDefault();
	}

	void TitleBarDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		widget_dimensions.size.height = 25;
		widget_dragable = true;
		// Do not set left/right anchoring, it will interfere with manual resizing/repositioning by the Window class.

		titlebar_closeButton = false;
		titlebar_closeButtonPadding = 2;
		titlebar_closeButtonSkinType = "default.close";

		TextUserDesc::resetToDefault();
	}

	TitleBar::TitleBar(const Ogre::String& name) :
		ComponentWidget(name),
		TextUser(),
		mCloseButton(NULL),
		mHeightBuffer(3),
		mDesc(NULL)
	{
	}

	TitleBar::~TitleBar()
	{
	}

	void TitleBar::_initialize(WidgetDesc* d)
	{
		ComponentWidget::_initialize(d);

		mDesc = dynamic_cast<TitleBarDesc*>(mWidgetDesc);

		setSkinType(d->widget_skinTypeName);

		TitleBarDesc* td = dynamic_cast<TitleBarDesc*>(d);

		// Create CloseButton if property is set.
		if(td->titlebar_closeButton)
		{
			ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
			bd->resetToDefault();
			bd->widget_name = getName() + ".CloseButton";

			if((td->titlebar_closeButtonPadding * 2.0) >= mClientDimensions.size.height)
				throw Exception(Exception::ERR_INVALIDPARAMS,"CloseButtonPadding exceeds height of TitleBar, cannot create Close Button!","TitleBar::_initialize");

			bd->widget_dimensions.size.width = mClientDimensions.size.height - (td->titlebar_closeButtonPadding * 2.0);
			bd->widget_dimensions.size.height = bd->widget_dimensions.size.width;
			bd->widget_dimensions.position.x = mDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT) - td->titlebar_closeButtonPadding - bd->widget_dimensions.size.width;
			bd->widget_dimensions.position.y = td->titlebar_closeButtonPadding;
			bd->widget_skinTypeName = td->titlebar_closeButtonSkinType;
			bd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

			mCloseButton = dynamic_cast<Button*>(_createWidget(bd));
			addComponent(CLOSE_BUTTON,mCloseButton);
			
			mCloseButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&TitleBar::onCloseButtonUp,this);

			mClientDimensions.size.width = mCloseButton->getPosition().x - mSkinElement->getBorderThickness(BORDER_LEFT);
		}

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;
		TextUser::_initialize(this,mDesc);
	}

	Ogre::String TitleBar::getClass()
	{
		return "TitleBar";
	}

	void TitleBar::onCloseButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			mWindow->setVisible(false);
	}

	void TitleBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);
		}

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		if(mText->empty())
			return;

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

	void TitleBar::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinElement != NULL)
		{
			mClientDimensions.position.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = mSkinElement->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (mSkinElement->getBorderThickness(BORDER_TOP) + mSkinElement->getBorderThickness(BORDER_BOTTOM));

			if(mCloseButton)
			{
				Point p;
				p.x = mDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT) - mDesc->titlebar_closeButtonPadding - mCloseButton->getWidth();
				p.y = mDesc->titlebar_closeButtonPadding;
				mCloseButton->setPosition(p);

				mClientDimensions.size.width = mCloseButton->getPosition().x - mSkinElement->getBorderThickness(BORDER_LEFT);
			}
		}

		if(mText != NULL)
			mText->setAllottedHeight(mClientDimensions.size.height);

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);

		redraw();
	}
}
