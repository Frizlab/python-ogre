#include "QuickGUIWindow.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"

#include "OgreImage.h"
#include "OgreLogManager.h"

namespace QuickGUI
{
	const Ogre::String Window::BACKGROUND = "background";
	const Ogre::String Window::TITLEBAR = "titlebar";

	void Window::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("Window");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(TITLEBAR);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Window",d);
	}

	WindowDesc::WindowDesc() :
		PanelDesc(),
		titleBar(true),
		titleBarCloseButton(true),
		titleBarCloseButtonPadding(2),
		titleBarSkinType("default"),
		titleBarCloseButtonSkinType("default.close")
	{
		dimensions.size = Size(100,100);
		dragable = false;

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			titleBarPadding[i] = 5.0;
		}

		resizable = true;
	}

	void WindowDesc::serialize(SerialBase* b)
	{
		PanelDesc::serialize(b);

		b->IO("TitleBar",&titleBar);
		b->IO("TitleBarCloseButton",&titleBarCloseButton);
		b->IO("TitleBarCloseButtonPadding",&titleBarCloseButtonPadding);
		b->IO("TitleBarSkinType",&titleBarSkinType);

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			b->IO(StringConverter::toString(static_cast<Padding>(i)),&titleBarPadding[i]);
		}

		titleBarTextDesc.serialize(b);
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
				delete (*it);
		}

		if(!mTexture.isNull())
			Ogre::TextureManager::getSingleton().remove(mTexture->getName());
	}

	void Window::_initialize(WidgetDesc* d)
	{
		Panel::_initialize(d);

		mDesc = dynamic_cast<WindowDesc*>(mWidgetDesc);

		WindowDesc* wd = dynamic_cast<WindowDesc*>(d);

		mDesc->titleBar = wd->titleBar;
		mDesc->titleBarCloseButton = wd->titleBarCloseButton;
		mDesc->titleBarCloseButtonPadding = wd->titleBarCloseButtonPadding;
		mDesc->titleBarCloseButtonSkinType = wd->titleBarCloseButtonSkinType;
		for(int i = 0; i < PADDING_COUNT; ++i)
			mDesc->titleBarPadding[i] = wd->titleBarPadding[i];
		mDesc->titleBarTextDesc = wd->titleBarTextDesc;		

		// Create TitleBar if property is set.
		if(mDesc->titleBar)
		{
			TitleBarDesc tbd;
			tbd.name = ".TitleBar";
			tbd.dimensions.size.width = mClientDimensions.size.width;
			tbd.closeButton = mDesc->titleBarCloseButton;
			tbd.closeButtonPadding = mDesc->titleBarCloseButtonPadding;
			tbd.skinTypeName = mDesc->titleBarSkinType;
			for(int i = 0; i < PADDING_COUNT; ++i)
			{
				tbd.padding[i] = mDesc->titleBarPadding[i];
			}
			tbd.textDesc = mDesc->titleBarTextDesc;

			mTitleBar = dynamic_cast<TitleBar*>(Widget::create("TitleBar",tbd));
			addComponent(TITLEBAR,mTitleBar);

			// Since components are not restricted to Client dimensions, offset the TitleBar to appear inside client dimensions.
			mTitleBar->setPosition(mClientDimensions.position);
		}

		if(d->getWidgetClass() == "Window")
			setSkinType(d->skinTypeName);

		createRenderTarget();
	}

	Widget* Window::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new Window(widgetName);

		newWidget->_createDescObject("WindowDesc");

		return newWidget;
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
			(int)mWidgetDesc->dimensions.size.width, 
			(int)mWidgetDesc->dimensions.size.height, 
			1, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET);

		// Add a single viewport

		mTexture->getBuffer()->getRenderTarget()->addViewport(0);

		// Restore logging detail

		log->setLogDetail(level);
	}

	ToolBar* Window::createToolBar(ToolBarDesc& d)
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

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

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
			brush->drawSkinElement(Rect(Point::ZERO,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));
		}
	}

	void Window::draw()
	{
		if(!mWidgetDesc->visible)
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
				mDesc->titleBarTextDesc = mTitleBar->mDesc->textDesc;
		}
		
		brush->setRenderTarget(NULL);
		brush->setTexture(mTexture);
		brush->drawRectangle(mWidgetDesc->dimensions,UVRect(0,0,1,1));

		WidgetEventArgs args(this);
		fireWindowEvent(WINDOW_EVENT_DRAWN,args);
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
				delete ehs;
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
			(int)mWidgetDesc->dimensions.size.width, 
			(int)mWidgetDesc->dimensions.size.height,
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
		const size_t buffSize = mWidgetDesc->dimensions.size.width * mWidgetDesc->dimensions.size.height * 4;
		unsigned char *data = new unsigned char[buffSize];
		
		// Clear buffer
		memset(data, 0, buffSize);

		// Setup Image with correct settings
		Ogre::Image i;
		i.loadDynamicImage(data, mWidgetDesc->dimensions.size.width, mWidgetDesc->dimensions.size.height, 1, Ogre::PF_R8G8B8A8, true);
		
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

		mWidgetDesc->skinTypeName = type;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);

		if(mHScrollBar)
			mHScrollBar->setPosition(Point(se->getBorderThickness(BORDER_LEFT),mWidgetDesc->dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM) - mDesc->scrollBarThickness));

		if(mVScrollBar)
		{
			float y = se->getBorderThickness(BORDER_TOP);
			
			if(mDesc->titleBar && mTitleBar->getVisible())
				y += mTitleBar->getHeight();
			
			mVScrollBar->setPosition(Point(mWidgetDesc->dimensions.size.width - se->getBorderThickness(BORDER_RIGHT) - mVScrollBar->getWidth(),y));
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

	void Window::setTitleBarTextColor(const Ogre::ColourValue& cv)
	{
		if(mTitleBar != NULL)
			mTitleBar->setColor(cv);
	}

	void Window::setTitleBarTextColor(const Ogre::ColourValue& cv, unsigned int index)
	{
		if(mTitleBar != NULL)
			mTitleBar->setColor(cv,index);
	}

	void Window::setTitleBarTextColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		if(mTitleBar != NULL)
			mTitleBar->setColor(cv,startIndex,endIndex);
	}

	void Window::setTitleBarTextColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		if(mTitleBar != NULL)
			mTitleBar->setColor(cv,c,allOccurrences);
	}

	void Window::setTitleBarTextColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		if(mTitleBar != NULL)
			mTitleBar->setColor(cv,s,allOccurrences);
	}

	void Window::setTitleBarTextFont(const Ogre::String& fontName)
	{
		if(mTitleBar != NULL)
			mTitleBar->setFont(fontName);
	}

	void Window::setTitleBarTextFont(const Ogre::String& fontName, unsigned int index)
	{
		if(mTitleBar != NULL)
			mTitleBar->setFont(fontName,index);
	}

	void Window::setTitleBarTextFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		if(mTitleBar != NULL)
			mTitleBar->setFont(fontName,startIndex,endIndex);
	}

	void Window::setTitleBarTextFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		if(mTitleBar != NULL)
			mTitleBar->setFont(fontName,c,allOccurrences);
	}
	
	void Window::setTitleBarTextFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		if(mTitleBar != NULL)
			mTitleBar->setFont(fontName,s,allOccurrences);
	}

	void Window::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->dimensions.size;

		if(mSkinType != NULL)
		{
			// Default
			SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
			mClientDimensions.position.x = se->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = se->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));

			// If titleBar exists, update its dimensions and update client dimensions
			if(mDesc->titleBar && mTitleBar)
			{
				mTitleBar->setPosition(Point(se->getBorderThickness(BORDER_LEFT),se->getBorderThickness(BORDER_TOP)));
				mTitleBar->setWidth(mWidgetDesc->dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT)));
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
