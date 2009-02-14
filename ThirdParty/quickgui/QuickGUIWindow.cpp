#include "QuickGUIWindow.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"

#include "OgreImage.h"
#include "OgreLogManager.h"

namespace QuickGUI
{
	const Ogre::String Window::BACKGROUND = "background";
	const Ogre::String Window::TITLEBAR = "titlebar";

	void Window::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Window");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(TITLEBAR);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Window",d);
	}

	WindowDesc::WindowDesc(const Ogre::String& id) :
		PanelDesc(id)
	{
		resetToDefault();
	}

	void WindowDesc::resetToDefault()
	{
		PanelDesc::resetToDefault();

		widget_dimensions.size = Size(100,100);
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
	}

	void WindowDesc::serialize(SerialBase* b)
	{
		PanelDesc::serialize(b);

		b->IO("TitleBar",&window_titleBar);
		b->IO("TitleBarDragable",&window_titleBarDragable);
		b->IO("TitleBarCloseButton",&window_titleBarCloseButton);
		b->IO("TitleBarCloseButtonPadding",&window_titleBarCloseButtonPadding);
		b->IO("TitleBarSkinType",&window_titleBarSkinType);
		b->IO("TitlebarVerticalTextAlignment",&window_titlebarVerticalTextAlignment);

		textDesc.serialize(b);
	}

	Window::Window(const Ogre::String& name) :
		Panel(name),
		mTitleBar(0),
		mDirty(true)
	{
		mWindow = this;
		mSkinElementName = BACKGROUND;
	}

	Window::~Window()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < WINDOW_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mWindowEventHandlers[index].begin(); it != mWindowEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}

		if(!mTexture.isNull())
		{
			Ogre::String texName = mTexture->getName();
			mTexture.setNull();
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
			TitleBarDesc* tbd = FactoryManager::getSingleton().getDescFactory()->getInstance<TitleBarDesc>("DefaultTitleBarDesc");
			tbd->resetToDefault();
			tbd->widget_name = getName() + ".TitleBar";
			tbd->widget_dimensions.size.width = mClientDimensions.size.width;
			tbd->widget_dimensions.size.height = mDesc->textDesc.getTextHeight() + 6;
			tbd->widget_dragable = mDesc->window_titleBarDragable;
			tbd->titlebar_closeButton = mDesc->window_titleBarCloseButton;
			tbd->titlebar_closeButtonPadding = mDesc->window_titleBarCloseButtonPadding;
			tbd->widget_skinTypeName = mDesc->window_titleBarSkinType;
			tbd->titlebar_verticalTextAlignment = mDesc->window_titlebarVerticalTextAlignment;
			tbd->textDesc = mDesc->textDesc;

			mTitleBar = dynamic_cast<TitleBar*>(Widget::create("TitleBar",tbd));
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

		// Its possible to have multiple GUIManagers with multiple Windows of the same name.
		// Lets make sure our texture name is unique
		if(Ogre::TextureManager::getSingleton().resourceExists(texName))
			texName += "_";

		mTexture = Ogre::TextureManager::getSingleton().createManual(
			texName, 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D, 
			(int)mWidgetDesc->widget_dimensions.size.width, 
			(int)mWidgetDesc->widget_dimensions.size.height, 
			0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET);

		// Add a single viewport

		Ogre::Viewport* vp = mTexture->getBuffer()->getRenderTarget()->addViewport(0);

		// Restore logging detail

		log->setLogDetail(level);
	}

	ToolBar* Window::createToolBar(ToolBarDesc* d)
	{
		ToolBar* newToolBar = dynamic_cast<ToolBar*>(Widget::create("ToolBar",d));

		addChild(newToolBar);

		return newToolBar;
	}

	void Window::addWindowEventHandler(WindowEvent EVENT, EventHandlerSlot* function)
	{
		mWindowEventHandlers[EVENT].push_back(function);
	}

	bool Window::fireWindowEvent(WindowEvent e, EventArgs& args)
	{
		if(mWindowEventHandlers[e].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mWindowEventHandlers[e]);
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
		if(st->getSkinElement(mSkinElementName)->getTextureName() == "")
		{
			// Get currently set color used for drawing
			Ogre::ColourValue cv = brush->getColour();
			brush->setColor(Ogre::ColourValue::ZERO);
			brush->clear();
			// Restore color to previous setting
			brush->setColor(cv);
		}
		else
		{
			brush->drawSkinElement(Rect(Point::ZERO,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
		}
	}

	void Window::draw()
	{
		if(!mWidgetDesc->widget_visible)
			return;

		Brush* brush = Brush::getSingletonPtr();

		if(mDirty)
		{
			// resizeRenderTarget
			resizeRenderTarget();

			// setRenderTarget
			brush->setRenderTarget(mTexture);

			// clearRenderTarget
			brush->clear();

			// update the texture!
			ContainerWidget::draw();

			mDirty = false;

			// Text properties are not updated until after they are drawn.  If this window
			// contains a TitleBar with text we need to stay up to date with its changes
			if(mTitleBar != NULL)
				mDesc->textDesc = mTitleBar->mDesc->textDesc;
		}
		
		brush->setRenderTarget(NULL);
		brush->setTexture(mTexture);
		brush->setOpacity(getAbsoluteOpacity());
		brush->drawRectangle(mWidgetDesc->widget_dimensions,UVRect(0,0,1,1));

		WidgetEventArgs args(this);
		fireWindowEvent(WINDOW_EVENT_DRAWN,args);
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

		throw Exception(Exception::ERR_ITEM_NOT_FOUND,"No \"" + StringConverter::toString(EVENT) + "\" event handlers found that were registered by the given object!","Widget::removeEventHandler");
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
		mTexture.setNull();

		// Create new texture with the same name

		mTexture = manager.createManual( 
			texName, 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D, 
			(int)mWidgetDesc->widget_dimensions.size.width, 
			(int)mWidgetDesc->widget_dimensions.size.height,
			1, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET);

		mTexture->load();

		// Add viewport

		mTexture->getBuffer()->getRenderTarget()->addViewport(0);

		// Restore logging detail

		log->setLogDetail(level);
	}

	void Window::saveTextureToFile(const Ogre::String& filename)
	{
		// Declare buffer
		const size_t buffSize = mWidgetDesc->widget_dimensions.size.width * mWidgetDesc->widget_dimensions.size.height * 4;
		unsigned char *data = OGRE_ALLOC_T(unsigned char,buffSize,Ogre::MEMCATEGORY_GENERAL);
		
		// Clear buffer
		memset(data, 0, buffSize);

		// Setup Image with correct settings
		Ogre::Image i;
		i.loadDynamicImage(data, mWidgetDesc->widget_dimensions.size.width, mWidgetDesc->widget_dimensions.size.height, 1, Ogre::PF_R8G8B8A8, true);
		
		// Copy Texture buffer contents to image buffer
		Ogre::HardwarePixelBufferSharedPtr buf = mTexture->getBuffer();		
		const Ogre::PixelBox destBox = i.getPixelBox();
		buf->blitToMemory(destBox);
		
		// Save to disk!
		i.save(filename);
	}

	void Window::setSkinType(const Ogre::String type)
	{
		if(!SkinTypeManager::getSingleton().hasSkinType(getClass(),type))
			throw Exception(Exception::ERR_SKINNING,"SkinType \"" + type + "\" does not exist!","Widget::setSkinType");

		mSkinType = SkinTypeManager::getSingleton().getSkinType(getClass(),type);

		mWidgetDesc->widget_skinTypeName = type;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);

		if(mTitleBar)
			mTitleBar->setPosition(Point(se->getBorderThickness(BORDER_LEFT),se->getBorderThickness(BORDER_TOP)));

		if(mHScrollBar)
			mHScrollBar->setPosition(Point(se->getBorderThickness(BORDER_LEFT),mWidgetDesc->widget_dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM) - mDesc->containerwidget_scrollBarThickness));

		if(mVScrollBar)
		{
			float y = se->getBorderThickness(BORDER_TOP);
			
			if(mDesc->window_titleBar && mTitleBar->getVisible())
				y += mTitleBar->getHeight();
			
			mVScrollBar->setPosition(Point(mWidgetDesc->widget_dimensions.size.width - se->getBorderThickness(BORDER_RIGHT) - mVScrollBar->getWidth(),y));
		}

		// Initially set dimensions of ScrollBars and update client dimensions
		updateClientDimensions();

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getComponentType((*it).first)->typeName);

		WidgetEventArgs wea(this);
		fireWidgetEvent(WIDGET_EVENT_SKIN_CHANGED,wea);

		// Determine Virtual Size.
		_determineVirtualSize();
		// Check ScrollBar visibility, update client dimensions if needbe, and re-determine virtual size
		_updateScrollBars();
	}

	void Window::setTitleBarDragable(bool dragable)
	{
		mDesc->window_titleBarDragable = dragable;
		if(mTitleBar != NULL)
			mTitleBar->setDragable(dragable);
	}

	void Window::setTitleBarText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setText(s,fp,cv);
			updateClientDimensions();

			redraw();
		}
	}

	void Window::setTitleBarText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
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

	void Window::setTitleBarTextColor(const Ogre::ColourValue& cv)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setColor(cv);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextColor(const Ogre::ColourValue& cv, unsigned int index)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setColor(cv,index);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setColor(cv,startIndex,endIndex);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setColor(cv,c,allOccurrences);
			updateClientDimensions();
		}
	}

	void Window::setTitleBarTextColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		if(mTitleBar != NULL)
		{
			mTitleBar->setColor(cv,s,allOccurrences);
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

	void Window::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinType != NULL)
		{
			// Default
			SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
			mClientDimensions.position.x = se->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = se->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));

			// If window_titleBar exists, update its dimensions and update client dimensions
			if(mDesc->window_titleBar && mTitleBar)
			{
				mTitleBar->setPosition(Point(se->getBorderThickness(BORDER_LEFT),se->getBorderThickness(BORDER_TOP)));
				mTitleBar->setWidth(mWidgetDesc->widget_dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT)));
				mClientDimensions.position.y += mTitleBar->getHeight();
				mClientDimensions.size.height -= mTitleBar->getHeight();
			}

			// First we want to adjust the client dimensions, depending on whether the scrollbars are visible.

			// If HorizontalScrollBar is visible, update its dimensions and update client dimensions
			if(mHScrollBar && mHScrollBar->getVisible())
				mClientDimensions.size.height -= mHScrollBar->getHeight();

			// If VerticalScrollBar is visible, update its dimensions and update client dimensions
			if(mVScrollBar && mVScrollBar->getVisible())
				mClientDimensions.size.width -= mVScrollBar->getWidth();

			// Now that client dimensions are fully updated, we can adjust ScrollBars position and size

			if(mHScrollBar)
				mHScrollBar->setWidth(mClientDimensions.size.width);

			if(mVScrollBar)
				mVScrollBar->setHeight(mClientDimensions.size.height);
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);

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
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->updateTexturePosition();
		}

		redraw();
	}
}
