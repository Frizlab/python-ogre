#include "SimpleGUIWidget.h"
#include "SimpleGUIManager.h"

/* From OgreTextAreaOverlayElement.cpp */
#define UNICODE_ZERO 0x0030

namespace SimpleGUI
{
	Widget::Widget(const Ogre::String& instanceName, const Ogre::Vector4& relativeDimensions, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		mInstanceName(instanceName),
		mRelativeDimensions(relativeDimensions),
		mParentWidget(ParentWidget),
		mOverlayContainer(overlayContainer),
		mAbsoluteDimensions(Ogre::Vector4::ZERO),
		mVisible(false),
		mVerticalAlignment(SGUI_VA_NO_ALIGNMENT),
		mHorizontalAlignment(SGUI_HA_NO_ALIGNMENT),
		mGrabbed(false),
		mMovingEnabled(true)
	{
		mAbsoluteDimensions = getAbsoluteDimensions(mRelativeDimensions);
		mPixelDimensions = convertRelativeToPixelDimensions(mAbsoluteDimensions);
		_initEventHandlers();

		GUIManager* gm = GUIManager::getSingletonPtr();
		mFont = gm->getDefaultFont();
		mCharacterHeight = gm->getDefaultCharacterHeight();
		mText = "";
		mTextTopColor = gm->getDefaultTextColor();
		mTextBotColor = gm->getDefaultTextColor();
	}

	Widget::~Widget()
	{
		std::vector< std::vector<MemberFunctionSlot*> >::iterator it1;
		for( it1 = mUserEventHandlers.begin(); it1 != mUserEventHandlers.end(); ++it1 )
		{
			std::vector<MemberFunctionSlot*>::iterator it2;
			for( it2 = (*it1).begin(); it2 != (*it1).end(); ++it2 )
				delete (*it2);
			it1->clear();
		}
		mUserEventHandlers.clear();

		mParentWidget = NULL;
	}

	void Widget::_initEventHandlers()
	{
		int index = 0;
		mUserEventHandlers.clear();
		// 12 is the number of types of events: SGUI_ACTIVATED, SGUI_CHARACTER_KEY, etc.
		while( index < 12 )
		{
			std::vector<MemberFunctionSlot*> eventTypeHandler;
			eventTypeHandler.clear();
			mUserEventHandlers.push_back(eventTypeHandler);
			++index;
		}
	}

	void Widget::_updateDimensions(const Ogre::Vector4& relativeDimensions)
	{
		mRelativeDimensions = relativeDimensions;
		mAbsoluteDimensions = getAbsoluteDimensions(mRelativeDimensions);
		mPixelDimensions = convertRelativeToPixelDimensions(mAbsoluteDimensions);
	}

	Ogre::Vector4 Widget::convertPixelToRelativeDimensions(const Ogre::Vector4& dimensions)
	{
		GUIManager* gm = GUIManager::getSingletonPtr();
		Ogre::Real renderWindowWidth = static_cast<Ogre::Real>(gm->getRenderWindowWidth());
		Ogre::Real renderWindowHeight = static_cast<Ogre::Real>(gm->getRenderWindowHeight());

		return Ogre::Vector4(
			static_cast<Ogre::Real>(dimensions.x) / renderWindowWidth,
			static_cast<Ogre::Real>(dimensions.y) / renderWindowHeight,
			static_cast<Ogre::Real>(dimensions.z) / renderWindowWidth,
			static_cast<Ogre::Real>(dimensions.w) / renderWindowHeight
			);
	}

	Ogre::Vector2 convertPixelToRelativePoint(const Ogre::Vector2& point)
	{
		GUIManager* gm = GUIManager::getSingletonPtr();
		Ogre::Real renderWindowWidth = static_cast<Ogre::Real>(gm->getRenderWindowWidth());
		Ogre::Real renderWindowHeight = static_cast<Ogre::Real>(gm->getRenderWindowHeight());

		return Ogre::Vector2(
			static_cast<Ogre::Real>(point.x) / renderWindowWidth,
			static_cast<Ogre::Real>(point.y) / renderWindowHeight
			);
	}

	Ogre::Vector4 Widget::convertRelativeToPixelDimensions(const Ogre::Vector4& dimensions)
	{
		GUIManager* gm = GUIManager::getSingletonPtr();
		Ogre::Real renderWindowWidth = static_cast<Ogre::Real>(gm->getRenderWindowWidth());
		Ogre::Real renderWindowHeight = static_cast<Ogre::Real>(gm->getRenderWindowHeight());

		return Ogre::Vector4( 
			static_cast<int>(dimensions.x * renderWindowWidth),
			static_cast<int>(dimensions.y * renderWindowHeight),
			static_cast<int>(dimensions.z * renderWindowWidth),
			static_cast<int>(dimensions.w * renderWindowHeight)
			);
	}

	Ogre::Vector2 Widget::convertRelativeToPixelPoint(const Ogre::Vector2& point)
	{
		GUIManager* gm = GUIManager::getSingletonPtr();
		Ogre::Real renderWindowWidth = static_cast<Ogre::Real>(gm->getRenderWindowWidth());
		Ogre::Real renderWindowHeight = static_cast<Ogre::Real>(gm->getRenderWindowHeight());

		return Ogre::Vector2( 
			static_cast<int>(point.x * renderWindowWidth),
			static_cast<int>(point.y * renderWindowHeight)
			);
	}

	Ogre::BorderPanelOverlayElement* Widget::createBorderPanelOverlayElement(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Ogre::BorderPanelOverlayElement* newBorderPanelOverlayElement = NULL;

		newBorderPanelOverlayElement = static_cast<Ogre::BorderPanelOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("BorderPanel",name));
		newBorderPanelOverlayElement->setMetricsMode(Ogre::GMM_PIXELS);
		newBorderPanelOverlayElement->setPosition(dimensions.x, dimensions.y);
		newBorderPanelOverlayElement->setDimensions(dimensions.z, dimensions.w);
		if(!material.empty()) newBorderPanelOverlayElement->setMaterialName(material);

		return newBorderPanelOverlayElement;
	}

	Ogre::OverlayContainer* Widget::createOverlayContainer(const Ogre::String& name, const Ogre::String& material)
	{
		Ogre::OverlayContainer* newOverlayContainer = NULL;

		newOverlayContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel",name));
		newOverlayContainer->setMetricsMode(Ogre::GMM_PIXELS);
		newOverlayContainer->setPosition(0,0);
		newOverlayContainer->setDimensions(1.0,1.0);
		if(!material.empty()) newOverlayContainer->setMaterialName(material);

		return newOverlayContainer;
	}

	Ogre::PanelOverlayElement* Widget::createPanelOverlayElement(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Ogre::PanelOverlayElement* newPanelOverlayElement = NULL;

		newPanelOverlayElement = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel",name));
		newPanelOverlayElement->setMetricsMode(Ogre::GMM_PIXELS);
		newPanelOverlayElement->setPosition(dimensions.x, dimensions.y);
		newPanelOverlayElement->setDimensions(dimensions.z, dimensions.w);
		if(!material.empty()) newPanelOverlayElement->setMaterialName(material);

		return newPanelOverlayElement;
	}

	Ogre::TextAreaOverlayElement* Widget::createTextAreaOverlayElement(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Ogre::TextAreaOverlayElement* newTextAreaOverlayElement = NULL;

		newTextAreaOverlayElement = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea",name));
		newTextAreaOverlayElement->setMetricsMode(Ogre::GMM_PIXELS);
		newTextAreaOverlayElement->setPosition(dimensions.x, dimensions.y);
		newTextAreaOverlayElement->setDimensions(dimensions.z, dimensions.w);
		if(!material.empty()) newTextAreaOverlayElement->setMaterialName(material);

		return newTextAreaOverlayElement;
	}

	Ogre::Vector4 Widget::getAbsoluteDimensions(const Ogre::Vector4& relativeDimensions)
	{
		Ogre::Vector4 ParentAbsoluteDimensions;

		if( mParentWidget == NULL ) ParentAbsoluteDimensions = Ogre::Vector4(0,0,1,1);
		else ParentAbsoluteDimensions = mParentWidget->getAbsoluteDimensions();

		Ogre::Real x = ParentAbsoluteDimensions.x + (ParentAbsoluteDimensions.z * relativeDimensions.x);
		Ogre::Real y = ParentAbsoluteDimensions.y + (ParentAbsoluteDimensions.w * relativeDimensions.y);
		Ogre::Real z = ParentAbsoluteDimensions.z * relativeDimensions.z;
		Ogre::Real w = ParentAbsoluteDimensions.w * relativeDimensions.w;

		return Ogre::Vector4(x,y,z,w);
	}

	Ogre::Real Widget::getCharacterHeight()
	{
		return mCharacterHeight;
	}

	Ogre::String Widget::getFont()
	{
		return mFont;
	}

	bool Widget::getGrabbed()
	{
		return mGrabbed;
	}

	Ogre::String Widget::getInstanceName()
	{
		return mInstanceName;
	}

	bool Widget::getMovingEnabled()
	{
		return mMovingEnabled;
	}

	Ogre::OverlayContainer* Widget::getOverlayContainer()
	{
		return mOverlayContainer;
	}

	Widget* Widget::getParentWidget()
	{
		return mParentWidget;
	}

	Widget* Widget::getWindow()
	{
		if( (mWidgetType == Widget::SGUI_WINDOW) || (mWidgetType == Widget::SGUI_PLAINWINDOW) )
			return this;

		Widget* w = mParentWidget;
		while( w != NULL )
		{
			if( (w->getWidgetType() == Widget::SGUI_WINDOW) || (w->getWidgetType() == Widget::SGUI_PLAINWINDOW) ) break;

			w = w->getParentWidget();
		}

		return w;
	}

	Ogre::Vector4 Widget::getPixelDimensions()
	{
		return mPixelDimensions;
	}

	Ogre::Vector2 Widget::getPixelPosition()
	{
		return Ogre::Vector2(mPixelDimensions.x,mPixelDimensions.y);
	}

	Ogre::Vector2 Widget::getPixelSize()
	{
		return Ogre::Vector2(mPixelDimensions.z,mPixelDimensions.w);
	}

	Ogre::Vector4 Widget::getAbsoluteDimensions()
	{
		return mAbsoluteDimensions;
	}

	Ogre::Vector2 Widget::getAbsolutePosition()
	{
		return Ogre::Vector2(mAbsoluteDimensions.x,mAbsoluteDimensions.y);
	}

	Ogre::Vector2 Widget::getAbsoluteSize()
	{
		return Ogre::Vector2(mAbsoluteDimensions.z,mAbsoluteDimensions.w);
	}

	Ogre::Vector4 Widget::getRelativeDimensions()
	{
		return mRelativeDimensions;
	}

	Ogre::Vector2 Widget::getRelativePosition()
	{
		return Ogre::Vector2(mRelativeDimensions.x,mRelativeDimensions.y);
	}

	Ogre::Vector2 Widget::getRelativeSize()
	{
		return Ogre::Vector2(mRelativeDimensions.z,mRelativeDimensions.w);
	}

	Ogre::String Widget::getText()
	{
		return mText;
	}

	Ogre::ColourValue Widget::getTextColorTop()
	{
		return mTextTopColor;
	}

	Ogre::ColourValue Widget::getTextColorBot()
	{
		return mTextBotColor;
	}

	Widget::Type Widget::getWidgetType()
	{
		return mWidgetType;
	}

	void Widget::hide()
	{
		mVisible = false;
		mGrabbed = false;
	}

	bool Widget::isPointWithinBounds(const Ogre::Vector2& p)
	{
		if(!mVisible) return false;

		if( p.x < mPixelDimensions.x ||
			p.x > (mPixelDimensions.x + mPixelDimensions.z) )
			return false;

		if( p.y < mPixelDimensions.y ||
			p.y > (mPixelDimensions.y + mPixelDimensions.w) )
			return false;

		return true;
	}

	bool Widget::isVisible()
	{
		return mVisible;
	}

	void Widget::move(const Ogre::Real& relativeXOffset, const Ogre::Real& relativeYOffset)
	{
		if(!mMovingEnabled) return;

		mRelativeDimensions.x += relativeXOffset;
		mRelativeDimensions.y += relativeYOffset;
		_notifyDimensionsChanged();
	}

	void Widget::activate(EventArgs& e)
	{
		// execute any user defined event handler callbacks
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_ACTIVATED];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
	}

	void Widget::deactivate(EventArgs& e)
	{
		// execute any user defined event handler callbacks
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_DEACTIVATED];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
	}

	bool Widget::onMouseEnters(MouseEventArgs& e)
	{
		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_MOUSE_ENTER];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseEnters(e);

		return e.handled; 
	}

	bool Widget::onMouseLeaves(MouseEventArgs& e)
	{
		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_MOUSE_LEAVE];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;

		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseLeaves(e);

		return e.handled; 
	}

	bool Widget::onMouseMoved(MouseEventArgs& e)
	{
		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_MOUSE_MOVE];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseMoved(e);

		return e.handled; 
	}

	bool Widget::onMouseWheel(MouseEventArgs& e)
	{
		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_MOUSE_WHEEL];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseWheel(e);

		return e.handled; 
	}

	bool Widget::onMouseButtonUp(MouseEventArgs& e)
	{
		if( (e.button == SimpleGUI::MB_Left) && (e.widget->getInstanceName() == mInstanceName) ) mGrabbed = false;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_MOUSE_BUTTON_UP];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseButtonUp(e);

		return e.handled; 
	}

	bool Widget::onMouseButtonDown(MouseEventArgs& e)
	{
		if( (e.button == SimpleGUI::MB_Left) && (e.widget->getInstanceName() == mInstanceName) ) mGrabbed = true;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_MOUSE_BUTTON_DOWN];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseButtonDown(e);

		return e.handled; 
	}

	bool Widget::onMouseClicked(MouseEventArgs& e)
	{
		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_MOUSE_CLICK];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseClicked(e);

		return e.handled; 
	}

	bool Widget::onKeyDown(KeyEventArgs& e)
	{
		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_KEY_DOWN];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onKeyDown(e);

		return e.handled; 
	}

	bool Widget::onKeyUp(KeyEventArgs& e)
	{
		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_KEY_UP];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onKeyUp(e);

		return true; 
	}

	bool Widget::onCharacter(KeyEventArgs& e)
	{
		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_CHARACTER_KEY];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onCharacter(e);

		return e.handled;
	}

	void Widget::timeElapsed(Ogre::Real time) {}

	void Widget::setCharacterHeight(const Ogre::Real& relativeHeight)
	{
		mCharacterHeight = relativeHeight;
		_notifyTextChanged();
	}

	void Widget::setDimensions(const Ogre::Vector4& relativeDimensions)
	{
		mRelativeDimensions = relativeDimensions;
		_notifyDimensionsChanged();
	}

	void Widget::setFont(const Ogre::String& font)
	{
		mFont = font;
		_notifyTextChanged();
	}

	void Widget::setGrabbed(bool grabbed)
	{
		mGrabbed = grabbed;
	}

	void Widget::setHeight(const Ogre::Real& relativeHeight)
	{
		mRelativeDimensions.w = relativeHeight;
		_notifyDimensionsChanged();
	}

	void Widget::setMovingEnabled(bool enable)
	{
		mMovingEnabled = enable;
	}

	void Widget::setPosition(const Ogre::Real& xRelativePosition, const Ogre::Real& yRelativePosition)
	{
		mRelativeDimensions.x = xRelativePosition;
		mRelativeDimensions.y = yRelativePosition;
		_notifyDimensionsChanged();
	}

	void Widget::setText(const Ogre::String& text)
	{
		mText = text;
		_notifyTextChanged();
	}

	void Widget::setTextColor(const Ogre::ColourValue& color)
	{
		setTextColor(color,color);
	}

	void Widget::setTextColor(const Ogre::ColourValue& topColor,const Ogre::ColourValue& botColor)
	{
		mTextTopColor = topColor;
		mTextBotColor = botColor;
	}

	void Widget::show()
	{
		mVisible = true;
	}

	void Widget::setVerticalAlignment(VerticalAlignment va)
	{
		mVerticalAlignment = va;
	}

	void Widget::setHorizontalAlignment(HorizontalAlignment ha)
	{
		mHorizontalAlignment = ha;
	}
}
