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

#include "QuickGUIWindow.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIVScrollBar.h"
#include "QuickGUITitleBar.h"
#include "QuickGUISheet.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIRoot.h"

#include "OgreImage.h"
#include "OgreLogManager.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreRenderTexture.h"
#include "OgreTextureManager.h"
#include "OgreFont.h"

namespace QuickGUI
{
	const Ogre::String Window::BACKGROUND = "background";
	const Ogre::String Window::TITLEBAR = "titlebar";

	void Window::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Window");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinReference(TITLEBAR,"TitleBar");
		d->defineSkinReference(HSCROLLBAR,"HScrollBar");
		d->defineSkinReference(VSCROLLBAR,"VScrollBar");
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Window",d);
	}

	WindowDesc::WindowDesc() :
		PanelDesc()
	{
		resetToDefault();
	}

	void WindowDesc::resetToDefault()
	{
		PanelDesc::resetToDefault();

		widget_dimensions.size = Size(300,300);
		widget_dragable = false;
		widget_resizeFromBottom = true;
		widget_resizeFromLeft = true;
		widget_resizeFromRight = true;
		widget_resizeFromTop = true;

		window_titleBar = true;
		window_titleBarDragable = true;
		window_titleBarCloseButton = true;
		window_titleBarCloseButtonPadding = 2;
		window_titleBarSkinType = "default";
		window_titleBarCloseButtonSkinType = "default.close";
		window_titlebarVerticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;

		textDesc.resetToDefault();
	}

	void WindowDesc::serialize(SerialBase* b)
	{
		PanelDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		WindowDesc* defaultValues = DescManager::getSingleton().createDesc<WindowDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("TitleBar",						&window_titleBar,						defaultValues->window_titleBar);
		b->IO("TitleBarDragable",				&window_titleBarDragable,				defaultValues->window_titleBarDragable);
		b->IO("TitleBarCloseButton",			&window_titleBarCloseButton,			defaultValues->window_titleBarCloseButton);
		b->IO("TitleBarCloseButtonPadding",		&window_titleBarCloseButtonPadding,		defaultValues->window_titleBarCloseButtonPadding);
		b->IO("TitleBarSkinType",				&window_titleBarSkinType,				defaultValues->window_titleBarSkinType);
		b->IO("TitlebarVerticalTextAlignment",	&window_titlebarVerticalTextAlignment,	defaultValues->window_titlebarVerticalTextAlignment);

		DescManager::getSingleton().destroyDesc(defaultValues);

		textDesc.serialize(b);
	}

	Window::Window(const Ogre::String& name) :
		Panel(name),
		mTitleBar(0),
		mDirty(true),
		mTexture(NULL)
	{
		mWindow = this;
	}

	Window::~Window()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < WINDOW_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mWindowEventHandlers[index].begin(); it != mWindowEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
            mWindowEventHandlers[index].clear();
		}

		if(mTexture != NULL)
		{
			Ogre::String texName = mTexture->getName();
			mTexture = NULL;
			Ogre::TextureManager::getSingleton().remove(texName);
		}
	}

	void Window::_initialize(WidgetDesc* d)
	{
		WindowDesc* wd = dynamic_cast<WindowDesc*>(d);

		Panel::_initialize(d);

		mDesc = dynamic_cast<WindowDesc*>(mWidgetDesc);

		mDesc->window_titleBar = wd->window_titleBar;
		mDesc->window_titleBarCloseButton = wd->window_titleBarCloseButton;
		mDesc->window_titleBarCloseButtonPadding = wd->window_titleBarCloseButtonPadding;
		mDesc->window_titleBarCloseButtonSkinType = wd->window_titleBarCloseButtonSkinType;
		mDesc->window_titleBarDragable = wd->window_titleBarDragable;
		mDesc->textDesc = wd->textDesc;

		// Create TitleBar if property is set.
		if(mDesc->window_titleBar)
		{
			TitleBarDesc* tbd = DescManager::getSingletonPtr()->getDefaultTitleBarDesc();
			tbd->resetToDefault();
			tbd->widget_name = getName() + ".TitleBar";
			tbd->widget_dimensions.size.width = mClientDimensions.size.width;
			if(mDesc->textDesc.segments.empty())
				tbd->widget_dimensions.size.height = Text::getFontHeight(Root::getSingleton().getDefaultFontName()) + 6;
			else
				tbd->widget_dimensions.size.height = mDesc->textDesc.getTextHeight() + 6;
			tbd->widget_dragable = mDesc->window_titleBarDragable;
			tbd->widget_moveBaseWidgetOnDrag = true;
			tbd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
			tbd->titlebar_closeButton = mDesc->window_titleBarCloseButton;
			tbd->titlebar_closeButtonPadding = mDesc->window_titleBarCloseButtonPadding;
			tbd->widget_skinTypeName = mDesc->window_titleBarSkinType;
			tbd->textDesc.verticalTextAlignment = mDesc->window_titlebarVerticalTextAlignment;
			tbd->textDesc = mDesc->textDesc;

			mTitleBar = dynamic_cast<TitleBar*>(_createWidget(tbd));
			addComponent(TITLEBAR,mTitleBar);
		}

		if(d->getWidgetClass() == "Window")
			setSkinType(d->widget_skinTypeName);

		createRenderTarget();
	}

	void Window::createRenderTarget()
	{
		// Suppress logging detail

		Ogre::Log* log = Ogre::LogManager::getSingleton().getDefaultLog();
		Ogre::LoggingLevel level = log->getLogDetail();

		log->setLogDetail(Ogre::LL_LOW);

		// Create texture

		Ogre::String texName = getName() + ".RenderTarget";

		// Its possible to have multiple Sheets with multiple Windows of the same name.
		// Lets make sure our texture name is unique
		while(Ogre::TextureManager::getSingleton().resourceExists(texName))
			texName += "_";

		mTexture = Ogre::TextureManager::getSingleton().createManual(
			texName,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D,
			(int)mWidgetDesc->widget_dimensions.size.width,
			(int)mWidgetDesc->widget_dimensions.size.height,
			0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET).getPointer();

		// Add a single viewport

		Ogre::Viewport* vp = mTexture->getBuffer()->getRenderTarget()->addViewport(0);

		// Restore logging detail

		log->setLogDetail(level);
	}

	void Window::addWindowEventHandler(WindowEvent EVENT, EventHandlerSlot* function)
	{
		mWindowEventHandlers[EVENT].push_back(function);
	}

	void Window::bringToFront()
	{
		if(mDesc->sheet != NULL)
			mDesc->sheet->bringToFront(this);
	}

	void Window::center()
	{
		if(mDesc->sheet == NULL)
			return;

		Point p;
		p.x = ((mDesc->sheet->getWidth() / 2.0) - (mDesc->widget_dimensions.size.width / 2.0));
		p.y = ((mDesc->sheet->getHeight() / 2.0) - (mDesc->widget_dimensions.size.height / 2.0));

		setPosition(p);
	}

	void Window::destroy()
	{
		mWidgetDesc->sheet->destroyWindow(this);
	}

	bool Window::fireWindowEvent(WindowEvent EVENT, EventArgs& args)
	{
		if(mWindowEventHandlers[EVENT].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mWindowEventHandlers[EVENT]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	Ogre::String Window::getClass()
	{
		return "Window";
	}

	void Window::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		// If the Window or Sheet is transparent, meaning the SkinElement is empty,
		// the texture needs to be cleared to be transparent.
		if(st->getSkinElement(mSkinElement->getName())->getTextureName() == "")
		{
			// Get currently set color used for drawing
			ColourValue cv = brush->getColour();
			brush->setColor(ColourValue::ZERO);
			brush->clear();
			// Restore color to previous setting
			brush->setColor(cv);
		}
		else
		{
			brush->drawSkinElement(Rect(Point::ZERO,mWidgetDesc->widget_dimensions.size),mSkinElement);
		}
	}

	void Window::draw()
	{
		if(!mWidgetDesc->widget_visible)
			return;

		Brush* brush = Brush::getSingletonPtr();

		if(mDirty)
		{
			//updateTexture();

			// resizeRenderTarget
			resizeRenderTarget();

			Brush* brush = Brush::getSingletonPtr();

			// setRenderTarget
			brush->setRenderTarget(mTexture);

			// clearRenderTarget
			brush->clear();

			// Draw to the render target.
			ContainerWidget::draw();

			mDirty = false;

			// Text properties are not updated until after they are drawn.  If this window
			// contains a TitleBar with text we need to stay up to date with its changes
			if(mTitleBar != NULL)
				mDesc->textDesc = mTitleBar->mDesc->textDesc;
		}

		brush->setRenderTarget(static_cast<Ogre::Viewport*>(NULL));
		brush->setTexture(mTexture);
		brush->setColor(mWidgetDesc->widget_baseColor);
		brush->setOpacity(getAbsoluteOpacity());
		brush->drawRectangle(mWidgetDesc->widget_dimensions,UVRect(0,0,1,1));

		WidgetEventArgs args(this);
		fireWindowEvent(WINDOW_EVENT_DRAWN,args);
	}

	std::string Window::getTextureName()
	{
		return mTexture->getName();
	}

	bool Window::getTitleBarDragable()
	{
		return mDesc->window_titleBarDragable;
	}

	Ogre::UTFString Window::getTitleBarText()
	{
		if(mTitleBar != NULL)
			return mTitleBar->getText();
		return "";
	}

	BrushFilterMode Window::getTitleBarTextBrushFilterMode()
	{
		if(mTitleBar != NULL)
			return mTitleBar->getTextBrushFilterMode();
		return BRUSHFILTER_LINEAR;
	}

	bool Window::hasWindowEventHandler(WindowEvent EVENT, void* obj)
	{
		for(std::vector<EventHandlerSlot*>::iterator it = mWindowEventHandlers[EVENT].begin(); it != mWindowEventHandlers[EVENT].end(); ++it)
		{
			if((*it)->getClass() == obj)
				return true;
		}

		return false;
	}

	void Window::redraw()
	{
		mDirty = true;
	}

	void Window::removeEventHandlers(void* obj)
	{
		ContainerWidget::removeEventHandlers(obj);

		for(int index = 0; index < WINDOW_EVENT_COUNT; ++index)
		{
			std::vector<EventHandlerSlot*> updatedList;
			std::vector<EventHandlerSlot*> listToCleanup;

			for(std::vector<EventHandlerSlot*>::iterator it = mWindowEventHandlers[index].begin(); it != mWindowEventHandlers[index].end(); ++it)
			{
				if((*it)->getClass() == obj)
					listToCleanup.push_back((*it));
				else
					updatedList.push_back((*it));
			}

			mWindowEventHandlers[index].clear();
			for(std::vector<EventHandlerSlot*>::iterator it = updatedList.begin(); it != updatedList.end(); ++it)
				mWindowEventHandlers[index].push_back((*it));

			for(std::vector<EventHandlerSlot*>::iterator it = listToCleanup.begin(); it != listToCleanup.end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void Window::removeWindowEventHandler(WindowEvent EVENT, void* obj)
	{
		for(std::vector<EventHandlerSlot*>::iterator it = mWindowEventHandlers[EVENT].begin(); it != mWindowEventHandlers[EVENT].end(); ++it)
		{
			if((*it)->getClass() == obj)
			{
				EventHandlerSlot* ehs = (*it);
				mWindowEventHandlers[EVENT].erase(it);
				OGRE_DELETE_T(ehs,EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
				return;
			}
		}
	}

	void Window::resize(BorderSide s, Point position)
	{
		bool changePosition = false;

		Point currentPosition = mWidgetDesc->widget_dimensions.position;

		Point newPosition = mWidgetDesc->widget_dimensions.position;

		// Record size before resize
		Size previousSize = mWidgetDesc->widget_dimensions.size;

		Point changeInPosition(position - currentPosition);

		switch(s)
		{
		case BORDER_BOTTOM:
			setHeight(changeInPosition.y);
			break;
		case BORDER_TOP_LEFT:
			setSize(Size(previousSize.width - changeInPosition.x,previousSize.height - changeInPosition.y));
			setPosition(currentPosition + Point(previousSize.width - getSize().width,previousSize.height - getSize().height));
			break;
		case BORDER_BOTTOM_LEFT:
			setSize(Size(previousSize.width - changeInPosition.x,changeInPosition.y));
			setPosition(currentPosition + Point(previousSize.width - getSize().width,0));
			break;
		case BORDER_TOP_RIGHT:
			setSize(Size(changeInPosition.x,previousSize.height - changeInPosition.y));
			setPosition(currentPosition + Point(0,previousSize.height - getSize().height));
			break;
		case BORDER_BOTTOM_RIGHT:
			setSize(Size(changeInPosition.x,changeInPosition.y));
			break;
		case BORDER_LEFT:
			setWidth(previousSize.width - changeInPosition.x);
			setPosition(currentPosition + Point(previousSize.width - getSize().width,0));
			break;
		case BORDER_RIGHT:
			setWidth(changeInPosition.x);
			break;
		case BORDER_TOP:
			setHeight(previousSize.height - changeInPosition.y);
			setPosition(currentPosition + Point(0,previousSize.height - getSize().height));
			break;
		}
	}

	void Window::resizeRenderTarget()
	{
		Ogre::TextureManager& manager = Ogre::TextureManager::getSingleton();

		// Suppress logging detail

		Ogre::Log* log = Ogre::LogManager::getSingleton().getDefaultLog();
		Ogre::LoggingLevel level = log->getLogDetail();

		log->setLogDetail(Ogre::LL_LOW);

		// Get name of old texture

		Ogre::String texName = mTexture->getName();

		// Destroy existing texture

		manager.remove(texName);
		mTexture = NULL;

		// Create new texture with the same name

		mTexture = manager.createManual(
			texName,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D,
			(int)mWidgetDesc->widget_dimensions.size.width,
			(int)mWidgetDesc->widget_dimensions.size.height,
			1, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET).getPointer();

		mTexture->load();

		// Add viewport

		mTexture->getBuffer()->getRenderTarget()->addViewport(0);

		// Restore logging detail

		log->setLogDetail(level);
	}

	void Window::saveTextureToFile(const Ogre::String& filename)
	{
		// Declare buffer
		const size_t buffSize = static_cast<size_t>(mWidgetDesc->widget_dimensions.size.width) * static_cast<size_t>(mWidgetDesc->widget_dimensions.size.height) * static_cast<size_t>(4);
		unsigned char *data = OGRE_ALLOC_T(unsigned char,buffSize,Ogre::MEMCATEGORY_GENERAL);

		// Clear buffer
		memset(data, 0, buffSize);

		// Setup Image with correct settings
		Ogre::Image i;
		i.loadDynamicImage(data, static_cast<size_t>(mWidgetDesc->widget_dimensions.size.width), static_cast<size_t>(mWidgetDesc->widget_dimensions.size.height), static_cast<size_t>(1), Ogre::PF_R8G8B8A8, true);

		// Copy Texture buffer contents to image buffer
		Ogre::HardwarePixelBufferSharedPtr buf = mTexture->getBuffer();
		const Ogre::PixelBox destBox = i.getPixelBox();
		buf->blitToMemory(destBox);

		// Save to disk!
		i.save(filename);
	}

	void Window::sendToBack()
	{
		if(mDesc->sheet != NULL)
			mDesc->sheet->sendToBack(this);
	}

	Ogre::String Window::setName(const Ogre::String& name)
	{
		// If we are attached to a sheet, make sure our name is unique
		if(mWidgetDesc->sheet != NULL)
		{
			if(mNameRegisteredWithSheet)
				mWidgetDesc->sheet->_notifyNameFreed(mWidgetDesc->widget_name);

			mWidgetDesc->widget_name = mWidgetDesc->sheet->resolveName(name);
			mWidgetDesc->sheet->_notifyNameUsed(name);
			mNameRegisteredWithSheet = true;
		}
		// Otherwise, change the name to the one we passed in
		else
			mWidgetDesc->widget_name = name;

		// Rename Texture to match new name
		if(mTexture != NULL)
		{
			Ogre::String texName = mTexture->getName();
			mTexture = NULL;
			Ogre::TextureManager::getSingleton().remove(texName);

			createRenderTarget();
		}

		// Return the new name of the Widget
		return mWidgetDesc->widget_name;
	}

	void Window::setTitleBarDragable(bool dragable)
	{
		mDesc->window_titleBarDragable = dragable;
		if(mTitleBar != NULL)
			mTitleBar->setDragable(dragable);
	}

	void Window::setTitleBarText(Ogre::UTFString s)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setText(s);
			updateClientDimensions();

			redraw();
		}
	}

	void Window::setTitleBarText(Ogre::UTFString s, const ColourValue& cv)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setText(s,cv);
			updateClientDimensions();

			redraw();
		}
	}

	void Window::setTitleBarText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setText(s,fp,cv);
			updateClientDimensions();

			redraw();
		}
	}

	void Window::setTitleBarText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv)
	{
		setTitleBarText(s,Text::getFont(fontName),cv);
	}

	void Window::setTitleBarText(std::vector<TextSegment> segments)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setText(segments);
			updateClientDimensions();

			redraw();
		}
	}

	void Window::setTitleBarTextColor(const ColourValue& cv)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setTextColor(cv);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextColor(const ColourValue& cv, unsigned int index)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setTextColor(cv,index);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextColor(const ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setTextColor(cv,startIndex,endIndex);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextColor(const ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setTextColor(cv,c,allOccurrences);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextColor(const ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setTextColor(cv,s,allOccurrences);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextFont(const Ogre::String& fontName)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setFont(fontName);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextFont(const Ogre::String& fontName, unsigned int index)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setFont(fontName,index);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setFont(fontName,startIndex,endIndex);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setFont(fontName,c,allOccurrences);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setFont(fontName,s,allOccurrences);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextBrushFilterMode(BrushFilterMode m)
	{
		if(mTitleBar != NULL)
			mTitleBar->setTextBrushFilterMode(m);
	}

	void Window::updateClientDimensions()
	{
		mUpdatingClientDimensions = true;

		Size previousSize = mClientDimensions.size;

		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinElement != NULL)
		{
			// Default
			mClientDimensions.position.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = mSkinElement->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (mSkinElement->getBorderThickness(BORDER_TOP) + mSkinElement->getBorderThickness(BORDER_BOTTOM));

			// If window_titleBar exists, update its dimensions and update client dimensions
			if(mDesc->window_titleBar && mTitleBar)
			{
				mTitleBar->setPosition(Point(mSkinElement->getBorderThickness(BORDER_LEFT),mSkinElement->getBorderThickness(BORDER_TOP)));
				mTitleBar->setWidth(mWidgetDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT)));
				mClientDimensions.position.y += mTitleBar->getHeight();
				mClientDimensions.size.height -= mTitleBar->getHeight();
			}

			// Now that client dimensions are fully updated, we can adjust ScrollBars position and size

			if(mHScrollBar)
			{
				if(mVScrollBar != NULL && mVScrollBar->getVisible())
					mHScrollBar->setWidth(mClientDimensions.size.width - mVScrollBar->getWidth());
				else
					mHScrollBar->setWidth(mClientDimensions.size.width);
				mHScrollBar->setPosition(Point(mSkinElement->getBorderThickness(BORDER_LEFT),mWidgetDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM) - mDesc->containerwidget_scrollBarThickness));
			}

			if(mVScrollBar)
			{
				if(mHScrollBar != NULL && mHScrollBar->getVisible())
					mVScrollBar->setHeight(mClientDimensions.size.height - mHScrollBar->getHeight());
				else
					mVScrollBar->setHeight(mClientDimensions.size.height);

				float y = mSkinElement->getBorderThickness(BORDER_TOP);

				if(mDesc->window_titleBar && mTitleBar->getVisible())
					y += mTitleBar->getHeight();

				mVScrollBar->setPosition(Point(mWidgetDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT) - mVScrollBar->getWidth(),y));
			}
		}

		// Get difference.  A larger size indicates a positive difference
		Size difference = mClientDimensions.size - previousSize;

		// Handle anchoring for Components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if((*it).second == mHScrollBar)
				continue;

			if((*it).second == mVScrollBar)
				continue;

			if(it->second == mTitleBar)
				continue;

			applyAnchor((*it).second,difference);
		}

		// Handle Anchoring for children
		for(std::list<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			applyAnchor((*it),difference);
		}

		mUpdatingClientDimensions = false;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);

		// Determine Virtual Size.  This function also calls _determineScrollBarVisibility, which checks if
		// ScrollBars should be shown/hidden.  If scrollBar visibility changes, so does the client area.  In
		// this case, updateClientDimensions will be called again.
		updateVirtualDimensions();

		redraw();
	}

	void Window::updateTexturePosition()
	{
		// Remember that Windows are their own texture, thus their "screen" position is zero.

		mTexturePosition = Point::ZERO;

		// Update component screen dimensions, must be done after client and screen rect have been calculated
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			(*it).second->updateTexturePosition();
		}

		// Update children screen dimensions, must be done after client and screen rect have been calculated
		for(std::list<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->updateTexturePosition();
		}

		redraw();
	}

	void Window::updateTexture()
	{
		// resizeRenderTarget
		resizeRenderTarget();

		Brush* brush = Brush::getSingletonPtr();

		// setRenderTarget
		brush->setRenderTarget(mTexture);

		// clearRenderTarget
		brush->clear();

		// Draw to the render target.
		// Make sure to set the window visible flag to true, or the texture won't be updated at all.
		bool visible = mWidgetDesc->widget_visible;
		mWidgetDesc->widget_visible = true;
		ContainerWidget::draw();
		// Restore visibility
		mWidgetDesc->widget_visible = visible;

		mDirty = false;
	}
}
