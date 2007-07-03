#include "QuickGUIWidget.h"
#include "QuickGUIManager.h"
#include "QuickGUIWindow.h"
#include "QuickGUISheet.h"

/* From OgreTextAreaOverlayElement.cpp */
#define UNICODE_ZERO 0x0030

namespace QuickGUI
{
	Widget::Widget(const Ogre::String& instanceName, const Ogre::Vector4& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, Ogre::String WidgetMaterial, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		mInstanceName(instanceName),
		mWidgetMaterial(WidgetMaterial),
		mParentWidget(ParentWidget),
		mOverlayContainer(overlayContainer),
		mAbsoluteDimensions(Ogre::Vector4::ZERO),
		mVisible(true),
		mVerticalAlignment(QGUI_VA_NO_ALIGNMENT),
		mHorizontalAlignment(QGUI_HA_NO_ALIGNMENT),
		mGrabbed(false),
		mMovingEnabled(true),
		mBordersHidden(0),
		mZOrderOffset(0),
		mDraggingEnabled(0),
		mEnabled(1),
		mDisabledColor(Ogre::ColourValue(0.4,0.4,0.4,0.4))
	{
		mGUIManager = GUIManager::getSingletonPtr();
		mWidgetToDrag = this;

		// calculate and store dimensions, from relative to absolute to pixel
		mRelativeDimensions = _getRelativeDimensions(dimensions,positionMode,sizeMode);
		mAbsoluteDimensions = getAbsoluteDimensions(mRelativeDimensions);
		mPixelDimensions = absoluteToPixelDimensions(mAbsoluteDimensions);
		_initEventHandlers();

		//Ogre::LogManager::getSingleton().getLog("Ogre.log")->logMessage("Widget constructor - " + mInstanceName);
		mChildrenContainer = createOverlayContainer(mInstanceName+".ChildrenContainer","");
		if(mOverlayContainer != NULL) mOverlayContainer->addChildImpl(mChildrenContainer);

		// widget type has not been set yet.  If Parent is NULL,
		// we know the widget is a sheet.
		if(mParentWidget != NULL)
		{
			Sheet* s = getSheet();
			mFont = s->getDefaultFont();
			mCharacterHeight = s->getDefaultCharacterHeight();
			mText = "";
			mTextTopColor = s->getDefaultTextColor();
			mTextBotColor = s->getDefaultTextColor();
		}

		// All widgets can create borders on initialization except the Window widget, since
		// mOverlayContainer is required. mOverlayContainer is created in the Window constructor, not
		// the widget constructor.
		_createBorders();
	}

	Widget::~Widget()
	{
		removeAndDestroyAllChildWidgets();

		_destroyBorders();

		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		std::vector< std::vector<MemberFunctionSlot*> >::iterator it1;
		for( it1 = mUserEventHandlers.begin(); it1 != mUserEventHandlers.end(); ++it1 )
		{
			std::vector<MemberFunctionSlot*>::iterator it2;
			for( it2 = (*it1).begin(); it2 != (*it1).end(); ++it2 )
				delete (*it2);
			it1->clear();
		}
		mUserEventHandlers.clear();

		if( mOverlayElement != NULL )
		{
			// destroy Children container
			mOverlayContainer->removeChild(mChildrenContainer->getName());
			om->destroyOverlayElement(mChildrenContainer);
			mChildrenContainer = NULL;

			// destroy OverlayElement
			if( mOverlayElement->getParent() != NULL ) mOverlayElement->getParent()->removeChild(mOverlayElement->getName());
			om->destroyOverlayElement(mOverlayElement);
			mOverlayElement = NULL;
		}

		if((mWidgetType != Widget::QGUI_TYPE_WINDOW) && (mWidgetType != Widget::QGUI_TYPE_SHEET))
		{
			int zOrder = mParentWidget->getZOrder();
			Window* w = getWindow();
			if( w != NULL ) w->_removeZOrderValue(zOrder + mZOrderOffset);
			else getSheet()->_removeZOrderValue(zOrder + mZOrderOffset);
		}

		mParentWidget = NULL;

		GUIManager::getSingleton().removeWidgetName(mInstanceName);
	}

	void Widget::_addChildWidget(Widget* w)
	{
		mChildWidgets.push_back(w);
	}

	void Widget::_applyDimensions()
	{
		if( mInstanceName == "DefaultSheet.ChildWidget16.List" )
		{
			Ogre::Log* l = Ogre::LogManager::getSingleton().getLog("Ogre.log");
			l->logMessage("Setting ComboBox's Drop List Dimensions: " + Ogre::StringConverter::toString(mPixelDimensions));
		}

		mOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
	}

	void Widget::_initEventHandlers()
	{
		int index = 0;
		mUserEventHandlers.clear();
		// 12 is the number of types of events: QGUI_EVENT_ACTIVATED, QGUI_EVENT_CHARACTER_KEY, etc.
		while( index < 13 )
		{
			std::vector<MemberFunctionSlot*> eventTypeHandler;
			eventTypeHandler.clear();
			mUserEventHandlers.push_back(eventTypeHandler);
			++index;
		}
	}

	void Widget::_createBorders()
	{
		mBorders[QGUI_BORDER_TOP] = NULL;
		mBorders[QGUI_BORDER_BOTTOM] = NULL;
		mBorders[QGUI_BORDER_LEFT] = NULL;
		mBorders[QGUI_BORDER_RIGHT] = NULL;
		mBorderSizeInPixels[QGUI_BORDER_TOP] = 0;
		mBorderSizeInPixels[QGUI_BORDER_BOTTOM] = 0;
		mBorderSizeInPixels[QGUI_BORDER_LEFT] = 0;
		mBorderSizeInPixels[QGUI_BORDER_RIGHT] = 0;

		if(mOverlayContainer == NULL) return;

		// Making the Borders - First step, get that border material information
		Ogre::MaterialManager* mm = Ogre::MaterialManager::getSingletonPtr();
		// The material has a .border material to define all borders
		if( mm->resourceExists(mWidgetMaterial+".border") )
		{
			Ogre::String borderMaterial = mWidgetMaterial+".border";
			mBorderMaterial[QGUI_BORDER_TOP] = borderMaterial;
			mBorderMaterial[QGUI_BORDER_BOTTOM] = borderMaterial;
			mBorderMaterial[QGUI_BORDER_LEFT] = borderMaterial;
			mBorderMaterial[QGUI_BORDER_RIGHT] = borderMaterial;
		}
		else
		{
			if( mm->resourceExists(mWidgetMaterial+".border.top") ) mBorderMaterial[QGUI_BORDER_TOP] = mWidgetMaterial+".border.top";
			else mBorderMaterial[QGUI_BORDER_TOP] = "";
			if( mm->resourceExists(mWidgetMaterial+".border.bottom") ) mBorderMaterial[QGUI_BORDER_BOTTOM] = mWidgetMaterial+".border.bottom";
			else mBorderMaterial[QGUI_BORDER_BOTTOM] = "";
			if( mm->resourceExists(mWidgetMaterial+".border.left") ) mBorderMaterial[QGUI_BORDER_LEFT] = mWidgetMaterial+".border.left";
			else mBorderMaterial[QGUI_BORDER_LEFT] = "";
			if( mm->resourceExists(mWidgetMaterial+".border.right") ) mBorderMaterial[QGUI_BORDER_RIGHT] = mWidgetMaterial+".border.right";
			else mBorderMaterial[QGUI_BORDER_RIGHT] = "";
		}

		// Now border materials are stored.  Set Default Border thickness in pixels
		mBorderSizeInPixels[QGUI_BORDER_TOP] = 3;
		mBorderSizeInPixels[QGUI_BORDER_BOTTOM] = 3;
		mBorderSizeInPixels[QGUI_BORDER_LEFT] = 3;
		mBorderSizeInPixels[QGUI_BORDER_RIGHT] = 3;

		// Create the PanelOverlayElements, with a default of 3 pixel width
		Ogre::Vector4 defaultTBorderDimensions(
			mPixelDimensions.x - mBorderSizeInPixels[QGUI_BORDER_LEFT], 
			mPixelDimensions.y - mBorderSizeInPixels[QGUI_BORDER_TOP], 
			mPixelDimensions.z + mBorderSizeInPixels[QGUI_BORDER_LEFT] + mBorderSizeInPixels[QGUI_BORDER_RIGHT], 
			mBorderSizeInPixels[QGUI_BORDER_TOP]);

		Ogre::Vector4 defaultBBorderDimensions(
			mPixelDimensions.x - mBorderSizeInPixels[QGUI_BORDER_LEFT], 
			mPixelDimensions.y + mPixelDimensions.w,
			mPixelDimensions.z + mBorderSizeInPixels[QGUI_BORDER_LEFT] + mBorderSizeInPixels[QGUI_BORDER_RIGHT], 
			mBorderSizeInPixels[QGUI_BORDER_BOTTOM]);

		int lWidth = mBorderSizeInPixels[QGUI_BORDER_LEFT];

		Ogre::Vector4 defaultLBorderDimensions(
			mPixelDimensions.x - mBorderSizeInPixels[QGUI_BORDER_LEFT], 
			mPixelDimensions.y - mBorderSizeInPixels[QGUI_BORDER_TOP],
			mBorderSizeInPixels[QGUI_BORDER_LEFT], 
			mPixelDimensions.w + mBorderSizeInPixels[QGUI_BORDER_TOP] + mBorderSizeInPixels[QGUI_BORDER_BOTTOM]);

		int rWidth = mBorderSizeInPixels[QGUI_BORDER_RIGHT];

		Ogre::Vector4 defaultRBorderDimensions(
			mPixelDimensions.x + mPixelDimensions.z, 
			mPixelDimensions.y - mBorderSizeInPixels[QGUI_BORDER_TOP], 
			mBorderSizeInPixels[QGUI_BORDER_RIGHT], 
			mPixelDimensions.w + mBorderSizeInPixels[QGUI_BORDER_TOP] + mBorderSizeInPixels[QGUI_BORDER_BOTTOM]);

		mBorders[QGUI_BORDER_TOP] = createPanelOverlayElement(mInstanceName+".border.top",defaultTBorderDimensions,mBorderMaterial[QGUI_BORDER_TOP]);
		mBorders[QGUI_BORDER_BOTTOM] = createPanelOverlayElement(mInstanceName+".border.bottom",defaultBBorderDimensions,mBorderMaterial[QGUI_BORDER_BOTTOM]);
		mBorders[QGUI_BORDER_LEFT] = createPanelOverlayElement(mInstanceName+".border.left",defaultLBorderDimensions,mBorderMaterial[QGUI_BORDER_LEFT]);
		mBorders[QGUI_BORDER_RIGHT] = createPanelOverlayElement(mInstanceName+".border.right",defaultRBorderDimensions,mBorderMaterial[QGUI_BORDER_RIGHT]);

		// Attach panels and show
		mOverlayContainer->addChild(mBorders[QGUI_BORDER_TOP]);
		mBorders[QGUI_BORDER_TOP]->show();
		mOverlayContainer->addChild(mBorders[QGUI_BORDER_BOTTOM]);
		mBorders[QGUI_BORDER_BOTTOM]->show();
		mOverlayContainer->addChild(mBorders[QGUI_BORDER_LEFT]);
		mBorders[QGUI_BORDER_LEFT]->show();
		mOverlayContainer->addChild(mBorders[QGUI_BORDER_RIGHT]);
		mBorders[QGUI_BORDER_RIGHT]->show();
	}

	void Widget::_destroyBorders()
	{
		if(mOverlayContainer == NULL) return;

		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		int index;
		for( index = 0; index < 4; ++index )
		{
			if(mBorders[index] != NULL)
			{
				mOverlayContainer->removeChild(mBorders[index]->getName());
				om->destroyOverlayElement(mBorders[index]);
				mBorders[index] = NULL;
			}
		}
	}

	Ogre::Vector4 Widget::_getRelativeDimensions(const Ogre::Vector4& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode)
	{
		Ogre::Vector2 relPos = _getRelativePosition(Ogre::Vector2(dimensions.x,dimensions.y),positionMode);
		Ogre::Vector2 relSize = _getRelativeSize(Ogre::Vector2(dimensions.z,dimensions.w),sizeMode);

		return Ogre::Vector4(relPos.x,relPos.y,relSize.x,relSize.y);
	}

	Ogre::Vector2 Widget::_getRelativePosition(const Ogre::Vector2& position, GuiMetricsMode mode)
	{
		Ogre::Vector2 retVal = Ogre::Vector2::ZERO;

		Ogre::Real windowWidth = static_cast<Ogre::Real>(mGUIManager->getRenderWindowWidth());
		Ogre::Real windowHeight = static_cast<Ogre::Real>(mGUIManager->getRenderWindowHeight());
		Ogre::Vector2 parentAbsSize(1,1);
		Ogre::Vector2 parentPixelSize(windowWidth,windowHeight);
		if(mParentWidget != NULL) 
		{
			parentPixelSize = mParentWidget->getSize(QGUI_GMM_PIXELS);
			parentAbsSize = mParentWidget->getSize(QGUI_GMM_ABSOLUTE);
		}

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			retVal.x = position.x / parentAbsSize.x;
			retVal.y = position.y / parentAbsSize.y;
			break;
		case QGUI_GMM_RELATIVE:
			retVal.x = position.x;
			retVal.y = position.y;
			break;
		case QGUI_GMM_PIXELS:
			retVal.x = position.x / parentPixelSize.x;
			retVal.y = position.y / parentPixelSize.y;
			break;
		}

		return retVal;
	}

	Ogre::Vector2 Widget::_getRelativeSize(const Ogre::Vector2& size, GuiMetricsMode mode)
	{
		Ogre::Vector2 retVal = Ogre::Vector2::ZERO;

		Ogre::Real windowWidth = static_cast<Ogre::Real>(mGUIManager->getRenderWindowWidth());
		Ogre::Real windowHeight = static_cast<Ogre::Real>(mGUIManager->getRenderWindowHeight());
		Ogre::Vector2 parentAbsSize(1,1);
		Ogre::Vector2 parentPixelSize(windowWidth,windowHeight);
		if(mParentWidget != NULL) 
		{
			parentPixelSize = mParentWidget->getSize(QGUI_GMM_PIXELS);
			parentAbsSize = mParentWidget->getSize(QGUI_GMM_ABSOLUTE);
		}

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			retVal.x = size.x / parentAbsSize.x;
			retVal.y = size.y / parentAbsSize.y;
			break;
		case QGUI_GMM_RELATIVE:
			retVal.x = size.x;
			retVal.y = size.y;
			break;
		case QGUI_GMM_PIXELS:
			retVal.x = size.x / parentPixelSize.x;
			retVal.y = size.y / parentPixelSize.y;
			break;
		}

		return retVal;
	}

	void Widget::_notifyDimensionsChanged()
	{
		// Update Widget dimensions - done first.
		_updateDimensions(mRelativeDimensions);
		_applyDimensions(); // Widget specific

		// notify child widgets. Make sure this is done after dimensions are updated/applied.
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			(*it)->_notifyDimensionsChanged();
		}

		// update Borders. Make sure this is done after dimensions are updated/applied.
		_updateBorderSize();

		Ogre::Vector2 TBorderPos(mPixelDimensions.x - mBorderSizeInPixels[QGUI_BORDER_LEFT], mPixelDimensions.y - mBorderSizeInPixels[QGUI_BORDER_TOP]);
		Ogre::Vector2 BBorderPos(mPixelDimensions.x - mBorderSizeInPixels[QGUI_BORDER_LEFT], mPixelDimensions.y + mPixelDimensions.w);
		Ogre::Vector2 LBorderPos(mPixelDimensions.x - mBorderSizeInPixels[QGUI_BORDER_LEFT], mPixelDimensions.y - mBorderSizeInPixels[QGUI_BORDER_TOP]);
		Ogre::Vector2 RBorderPos(mPixelDimensions.x + mPixelDimensions.z, mPixelDimensions.y - mBorderSizeInPixels[QGUI_BORDER_TOP]);

		if(mBorders[QGUI_BORDER_TOP]) mBorders[QGUI_BORDER_TOP]->setPosition(TBorderPos.x,TBorderPos.y);
		if(mBorders[QGUI_BORDER_BOTTOM]) mBorders[QGUI_BORDER_BOTTOM]->setPosition(BBorderPos.x,BBorderPos.y);
		if(mBorders[QGUI_BORDER_LEFT]) mBorders[QGUI_BORDER_LEFT]->setPosition(LBorderPos.x,LBorderPos.y);
		if(mBorders[QGUI_BORDER_RIGHT]) mBorders[QGUI_BORDER_RIGHT]->setPosition(RBorderPos.x,RBorderPos.y);
	}

	void Widget::_removeChildWidget(Widget* w)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getInstanceName() == w->getInstanceName() )
			{
				mChildWidgets.erase(it);
				break;
			}
		}
	}

	void Widget::_updateDimensions(const Ogre::Vector4& relativeDimensions)
	{
		mRelativeDimensions = relativeDimensions;
		mAbsoluteDimensions = getAbsoluteDimensions(mRelativeDimensions);
		mPixelDimensions = absoluteToPixelDimensions(mAbsoluteDimensions);
	}

	void Widget::_updateBorderSize()
	{
		if(mBorders[QGUI_BORDER_TOP]) 
		{
			mBorders[QGUI_BORDER_TOP]->setHeight(mBorderSizeInPixels[QGUI_BORDER_TOP]);
			// Adjust width to incorporate width's of left and right borders
			mBorders[QGUI_BORDER_TOP]->setWidth(mPixelDimensions.z + mBorderSizeInPixels[QGUI_BORDER_LEFT] + mBorderSizeInPixels[QGUI_BORDER_RIGHT]);
		}
		
		if(mBorders[QGUI_BORDER_BOTTOM]) 
		{
			mBorders[QGUI_BORDER_BOTTOM]->setHeight(mBorderSizeInPixels[QGUI_BORDER_BOTTOM]);
			// Adjust width to incorporate width's of left and right borders
			mBorders[QGUI_BORDER_BOTTOM]->setWidth(mPixelDimensions.z + mBorderSizeInPixels[QGUI_BORDER_LEFT] + mBorderSizeInPixels[QGUI_BORDER_RIGHT]);
		}
		
		if(mBorders[QGUI_BORDER_LEFT]) 
		{
			mBorders[QGUI_BORDER_LEFT]->setWidth(mBorderSizeInPixels[QGUI_BORDER_LEFT]);
			// Adjust width to incorporate width's of top and bottom borders
			mBorders[QGUI_BORDER_LEFT]->setHeight(mPixelDimensions.w + mBorderSizeInPixels[QGUI_BORDER_TOP] + mBorderSizeInPixels[QGUI_BORDER_BOTTOM]);
		}

		if(mBorders[QGUI_BORDER_RIGHT]) 
		{
			mBorders[QGUI_BORDER_RIGHT]->setWidth(mBorderSizeInPixels[QGUI_BORDER_RIGHT]);
			// Adjust width to incorporate width's of top and bottom borders
			mBorders[QGUI_BORDER_RIGHT]->setHeight(mPixelDimensions.w + mBorderSizeInPixels[QGUI_BORDER_TOP] + mBorderSizeInPixels[QGUI_BORDER_BOTTOM]);
		}
	}

	void Widget::addEventHandler(Event EVENT, MemberFunctionSlot* function)
	{
		mUserEventHandlers[EVENT].push_back(function);
	}

	Ogre::Vector4 Widget::absoluteToPixelDimensions(const Ogre::Vector4& dimensions)
	{
		Ogre::Real renderWindowWidth = static_cast<Ogre::Real>(mGUIManager->getRenderWindowWidth());
		Ogre::Real renderWindowHeight = static_cast<Ogre::Real>(mGUIManager->getRenderWindowHeight());

		return Ogre::Vector4( 
			static_cast<int>(dimensions.x * renderWindowWidth),
			static_cast<int>(dimensions.y * renderWindowHeight),
			static_cast<int>(dimensions.z * renderWindowWidth),
			static_cast<int>(dimensions.w * renderWindowHeight)
			);
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

	void Widget::disable()
	{
		if((mWidgetType == Widget::QGUI_TYPE_TEXT) || 
			(mWidgetType == Widget::QGUI_TYPE_TEXTCURSOR) || 
			(mWidgetType == Widget::QGUI_TYPE_SHEET)) return;

		mEnabled = false;
		Ogre::MaterialPtr mp = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(mWidgetMaterial));

		Ogre::MaterialPtr disabledMaterial;
		if(Ogre::MaterialManager::getSingleton().resourceExists(mWidgetMaterial+".disabled"))
			disabledMaterial = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(mWidgetMaterial+".disabled"));
		else 
		{
			disabledMaterial = mp->clone(mWidgetMaterial+".disabled");
			Ogre::Pass* p = disabledMaterial->getTechnique(0)->getPass(0);

			if( (p != NULL) && (p->getNumTextureUnitStates() >= 1) ) 
			{
				Ogre::TextureUnitState* tus = p->createTextureUnitState();
				tus->setColourOperationEx(Ogre::LBX_MODULATE,Ogre::LBS_MANUAL,Ogre::LBS_CURRENT,mDisabledColor);
			}
		}

		setTextColor(mDisabledColor);
		mOverlayElement->setMaterialName(disabledMaterial->getName());
	}

	void Widget::drag(const Ogre::Real& xVal, const Ogre::Real& yVal,GuiMetricsMode mode)
	{
		if(!mDraggingEnabled) return;

		if(mWidgetToDrag != NULL) mWidgetToDrag->move(xVal,yVal,mode);

		// fire onDragged Event.
		WidgetEventArgs args(this);
		onDragged(args);
	}

	bool Widget::draggingEnabled()
	{
		return mDraggingEnabled;
	}

	void Widget::enable()
	{
		if((mWidgetType == Widget::QGUI_TYPE_TEXT) || 
			(mWidgetType == Widget::QGUI_TYPE_TEXTCURSOR) || 
			(mWidgetType == Widget::QGUI_TYPE_SHEET)) return;

		mEnabled = true;
		setTextColor(mTextTopColor,mTextBotColor);
		mOverlayElement->setMaterialName(mWidgetMaterial);
	}

	bool Widget::enabled()
	{
		return mEnabled;
	}

	void Widget::enableDragging(bool enable)
	{
		mDraggingEnabled = enable;
	}

	Ogre::Vector4 Widget::getAbsoluteDimensions(const Ogre::Vector4& relativeDimensions)
	{
		Ogre::Vector4 ParentAbsoluteDimensions;

		if( mParentWidget == NULL ) ParentAbsoluteDimensions = Ogre::Vector4(0,0,1,1);
		else ParentAbsoluteDimensions = mParentWidget->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE);

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

	Ogre::ColourValue Widget::getDisabledColor()
	{
		return mDisabledColor;
	}

	std::vector<Widget*>* Widget::getChildWidgetList()
	{
		return &mChildWidgets;
	}

	Widget* Widget::getChildWidget(const Ogre::String& name)
	{
		Widget* w = NULL;
		if( name == "" ) return w;

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getChildWidget(name) != NULL )
			{
				w = (*it);
				break;
			}
		}

		if( w != NULL ) return w;
		else if( mInstanceName == name ) return this;
		else return NULL;
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

	Ogre::Vector4 Widget::getDimensions(GuiMetricsMode position, GuiMetricsMode size)
	{
		Ogre::Vector4 retVal = Ogre::Vector4::ZERO;

		switch(position)
		{
		case QGUI_GMM_ABSOLUTE:
			retVal.x = mAbsoluteDimensions.x;
			retVal.y = mAbsoluteDimensions.y;
			break;
		case QGUI_GMM_RELATIVE:
			retVal.x = mRelativeDimensions.x;
			retVal.y = mRelativeDimensions.y;
			break;
		case QGUI_GMM_PIXELS:
			retVal.x = mPixelDimensions.x;
			retVal.y = mPixelDimensions.y;
			break;
		}

		switch(size)
		{
		case QGUI_GMM_ABSOLUTE:
			retVal.z = mAbsoluteDimensions.z;
			retVal.w = mAbsoluteDimensions.w;
			break;
		case QGUI_GMM_RELATIVE:
			retVal.z = mRelativeDimensions.z;
			retVal.w = mRelativeDimensions.w;
			break;
		case QGUI_GMM_PIXELS:
			retVal.z = mPixelDimensions.z;
			retVal.w = mPixelDimensions.w;
			break;
		}

		return retVal;
	}

	Ogre::Vector2 Widget::getPosition(GuiMetricsMode mode)
	{
		Ogre::Vector2 retVal = Ogre::Vector2::ZERO;

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			retVal.x = mAbsoluteDimensions.x;
			retVal.y = mAbsoluteDimensions.y;
			break;
		case QGUI_GMM_RELATIVE:
			retVal.x = mRelativeDimensions.x;
			retVal.y = mRelativeDimensions.y;
			break;
		case QGUI_GMM_PIXELS:
			retVal.x = mPixelDimensions.x;
			retVal.y = mPixelDimensions.y;
			break;
		}

		return retVal;
	}

	Ogre::Vector2 Widget::getSize(GuiMetricsMode mode)
	{
		Ogre::Vector2 retVal = Ogre::Vector2::ZERO;

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			retVal.x = mAbsoluteDimensions.z;
			retVal.y = mAbsoluteDimensions.w;
			break;
		case QGUI_GMM_RELATIVE:
			retVal.x = mRelativeDimensions.z;
			retVal.y = mRelativeDimensions.w;
			break;
		case QGUI_GMM_PIXELS:
			retVal.x = mPixelDimensions.z;
			retVal.y = mPixelDimensions.w;
			break;
		}

		return retVal;
	}

	Sheet* Widget::getSheet()
	{
		if( mWidgetType == Widget::QGUI_TYPE_SHEET )
			return dynamic_cast<Sheet*>(this);

		Widget* w = mParentWidget;
		while( w != NULL )
		{
			if( w->getWidgetType() == Widget::QGUI_TYPE_SHEET ) break;

			w = w->getParentWidget();
		}

		if( w != NULL ) return dynamic_cast<Sheet*>(w);
		else return NULL;
	}

	Widget* Widget::getTargetWidget(const Ogre::Vector2& p)
	{
		if( !mVisible || !mEnabled ) return NULL;

		// iterate through child widgets..
		Widget* w = NULL;
		// Get the widget with the highest zOrder
		int widgetZOrder = 0;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			Widget* temp = (*it)->getTargetWidget(p);
			if( (temp != NULL) && (temp->getZOrder() > widgetZOrder) )
			{
				w = temp;
				widgetZOrder = temp->getZOrder();
			}
		}

		if( w != NULL ) return w;
		else if( isPointWithinBounds(p) ) return this;
		else return NULL;
	}

	Ogre::UTFString Widget::getText()
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

	Window* Widget::getWindow()
	{
		if( mWidgetType == Widget::QGUI_TYPE_WINDOW )
			return dynamic_cast<Window*>(this);

		Widget* w = mParentWidget;
		while( w != NULL )
		{
			if( w->getWidgetType() == Widget::QGUI_TYPE_WINDOW )
				break;

			w = w->getParentWidget();
		}

		if( w != NULL ) return dynamic_cast<Window*>(w);
		else return NULL;
	}

	int Widget::getZOrder()
	{
		int zOrder = 0;
		if( mParentWidget != NULL ) zOrder = mParentWidget->getZOrder();
		return zOrder + mZOrderOffset;
	}

	int Widget::getZOrderOffset()
	{
		return mZOrderOffset;
	}

	void Widget::hide()
	{
		mVisible = false;
		mGrabbed = false;

		if( mBorders[QGUI_BORDER_TOP] ) mBorders[QGUI_BORDER_TOP]->hide();
		if( mBorders[QGUI_BORDER_BOTTOM] ) mBorders[QGUI_BORDER_BOTTOM]->hide();
		if( mBorders[QGUI_BORDER_LEFT] ) mBorders[QGUI_BORDER_LEFT]->hide();
		if( mBorders[QGUI_BORDER_RIGHT] ) mBorders[QGUI_BORDER_RIGHT]->hide();

		if( mOverlayElement ) mOverlayElement->hide();

		// hide children
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->hide();
	}

	void Widget::hideBorders()
	{
		mBordersHidden = true;
		if( mBorders[QGUI_BORDER_TOP] ) mBorders[QGUI_BORDER_TOP]->hide();
		if( mBorders[QGUI_BORDER_BOTTOM] ) mBorders[QGUI_BORDER_BOTTOM]->hide();
		if( mBorders[QGUI_BORDER_LEFT] ) mBorders[QGUI_BORDER_LEFT]->hide();
		if( mBorders[QGUI_BORDER_RIGHT] ) mBorders[QGUI_BORDER_RIGHT]->hide();
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

		if(overTransparentPixel(p)) return false;

		return true;
	}

	bool Widget::isVisible()
	{
		return mVisible;
	}

	void Widget::move(const Ogre::Real& xVal, const Ogre::Real& yVal, GuiMetricsMode mode)
	{
		if(!mMovingEnabled) return;

		
		Ogre::Vector2 relPos = _getRelativePosition(Ogre::Vector2(xVal,yVal),mode);
		mRelativeDimensions.x += relPos.x;
		mRelativeDimensions.y += relPos.y;

		_notifyDimensionsChanged();
		
		/*
		Ogre::Vector2 parentPosition(0,0);
		Ogre::Vector2 parentSize(1,1);
		if(mParentWidget != NULL) 
		{
			parentSize = mParentWidget->getSize(QGUI_GMM_PIXELS);
			parentPosition = mParentWidget->getPosition(QGUI_GMM_PIXELS);
		}

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mRelativeDimensions.x += xVal / parentSize.x;
			mRelativeDimensions.y += yVal / parentSize.y;
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.x += xVal;
			mRelativeDimensions.y += yVal;
			break;
		case QGUI_GMM_PIXELS:
			Ogre::Real deltaX = xVal - parentPosition.x;
			Ogre::Real deltaY = yVal - parentPosition.y;

			mRelativeDimensions.x += xVal / parentSize.x;
			mRelativeDimensions.y += yVal / parentSize.y;
			break;
		}

		_notifyDimensionsChanged();
		*/
	}

	bool Widget::overTransparentPixel(const Ogre::Vector2& mousePosition)
	{
		if( mWidgetMaterial == "" ) return false;

		Ogre::MaterialManager* mm = Ogre::MaterialManager::getSingletonPtr();
		if(!mm->resourceExists(mWidgetMaterial)) return false;

		Ogre::MaterialPtr mp = static_cast<Ogre::MaterialPtr>(mm->getByName(mWidgetMaterial));
		Ogre::Pass* p = mp->getTechnique(0)->getPass(0);
		
		if( (p == NULL) || (p->getNumTextureUnitStates() < 1) ) return false;

		Ogre::String name = p->getTextureUnitState(0)->getTextureName();

		// exempt from transparent checks, like "" material.
		if( name == "transparent.png" ) return false;

		Ogre::Image i;
		i.load(name,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		Ogre::Vector2 pt = Ogre::Vector2(mousePosition.x - mPixelDimensions.x,mousePosition.y - mPixelDimensions.y);
		Ogre::Real relX = pt.x / mPixelDimensions.z;
		Ogre::Real relY = pt.y / mPixelDimensions.w;

		Ogre::ColourValue c = i.getColourAt(relX * i.getWidth(), relY * i.getHeight(),0);
		if( c.a <= 0.0 ) return true;

		return false;
	}

	void Widget::removeAndDestroyAllChildWidgets()
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			delete (*it);
		mChildWidgets.clear();
	}

	void Widget::setBorderWidth(int borderPixelHeight)
	{
		setBorderWidth(borderPixelHeight,borderPixelHeight,borderPixelHeight,borderPixelHeight);
	}

	void Widget::setBorderWidth(int topBotBorderPixelHeight, int leftRightBorderPixelHeight)
	{
		setBorderWidth(topBotBorderPixelHeight,topBotBorderPixelHeight,leftRightBorderPixelHeight,leftRightBorderPixelHeight);
	}

	void Widget::setBorderWidth(int topBorderPixelHeight, int botBorderPixelHeight, int leftBorderPixelHeight, int rightBorderPixelHeight)
	{
		// Update widths
		mBorderSizeInPixels[QGUI_BORDER_TOP] = topBorderPixelHeight;
		mBorderSizeInPixels[QGUI_BORDER_BOTTOM] = botBorderPixelHeight;
		mBorderSizeInPixels[QGUI_BORDER_LEFT] = leftBorderPixelHeight;
		mBorderSizeInPixels[QGUI_BORDER_RIGHT] = rightBorderPixelHeight;

		// This function repositions and resizes the borders
		_notifyDimensionsChanged();
	}

	void Widget::activate(EventArgs& e)
	{
		if(!mEnabled) return;

		// execute any user defined event handler callbacks
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_ACTIVATED];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
	}

	void Widget::deactivate(EventArgs& e)
	{
		if(!mEnabled) return;

		mGrabbed = false;

		// execute any user defined event handler callbacks
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_DEACTIVATED];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
	}

	bool Widget::onDragged(EventArgs& e)
	{
		if(!mEnabled) return e.handled;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_DRAGGED];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);

		return e.handled;
	}

	bool Widget::onMouseEnters(MouseEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_MOUSE_ENTER];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseEnters(e);

		return e.handled; 
	}

	bool Widget::onMouseLeaves(MouseEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_MOUSE_LEAVE];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;

		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseLeaves(e);

		return e.handled; 
	}

	bool Widget::onMouseMoved(MouseEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_MOUSE_MOVE];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseMoved(e);

		return e.handled; 
	}

	bool Widget::onMouseWheel(MouseEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_MOUSE_WHEEL];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseWheel(e);

		return e.handled; 
	}

	bool Widget::onMouseButtonUp(MouseEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		mGrabbed = false;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_MOUSE_BUTTON_UP];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseButtonUp(e);

		return e.handled; 
	}

	bool Widget::onMouseButtonDown(MouseEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		if( (e.button == QuickGUI::MB_Left) && (e.widget->getInstanceName() == mInstanceName) ) mGrabbed = true;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_MOUSE_BUTTON_DOWN];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseButtonDown(e);

		return e.handled; 
	}

	bool Widget::onMouseClicked(MouseEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_MOUSE_CLICK];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onMouseClicked(e);

		return e.handled; 
	}

	bool Widget::onKeyDown(KeyEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_KEY_DOWN];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onKeyDown(e);

		return e.handled; 
	}

	bool Widget::onKeyUp(KeyEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_KEY_UP];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onKeyUp(e);

		return true; 
	}

	bool Widget::onCharacter(KeyEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		// execute any user defined event handler callbacks.
		// NOTE: it is important to return right away, for example pressing
		// a button might toggle another game state which clears the current GUI,
		// destroying this button! Exit immediately!
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_CHARACTER_KEY];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		if(e.handled) return e.handled;
		
		// if event not handled, pass it up to parent widget
		if( (mParentWidget != NULL) && !e.handled ) e.handled = mParentWidget->onCharacter(e);

		return e.handled;
	}

	void Widget::timeElapsed(Ogre::Real time) 
	{
		if(!mEnabled) return;

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			(*it)->timeElapsed(time);
		}
	}

	void Widget::setCharacterHeight(const Ogre::Real& relativeHeight)
	{
		mCharacterHeight = relativeHeight;
		_notifyTextChanged();
	}

	void Widget::setDimensions(const Ogre::Vector4& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode)
	{
		mRelativeDimensions = _getRelativeDimensions(dimensions,positionMode,sizeMode);

		_notifyDimensionsChanged();
	}

	void Widget::setDisabledColor(const Ogre::ColourValue& c)
	{
		mDisabledColor = c;
	}

	void Widget::setDraggingWidget(Widget* w)
	{
		mWidgetToDrag = w;
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

	void Widget::setPosition(const Ogre::Real& xVal, const Ogre::Real& yVal, GuiMetricsMode mode)
	{
		Ogre::Vector2 parentPosition(0,0);
		Ogre::Vector2 parentSize(1,1);
		if(mParentWidget != NULL) 
		{
			parentSize = mParentWidget->getSize(QGUI_GMM_PIXELS);
			parentPosition = mParentWidget->getPosition(QGUI_GMM_PIXELS);
		}

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mRelativeDimensions.x = xVal / parentSize.x;
			mRelativeDimensions.y = yVal / parentSize.y;
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.x = xVal;
			mRelativeDimensions.y = yVal;
			break;
		case QGUI_GMM_PIXELS:
			Ogre::Real deltaX = static_cast<Ogre::Real>(xVal) - parentPosition.x;
			Ogre::Real deltaY = static_cast<Ogre::Real>(yVal) - parentPosition.y;

			mRelativeDimensions.x = (deltaX / parentSize.x);
			mRelativeDimensions.y = (deltaY / parentSize.y);
			break;
		}

		_notifyDimensionsChanged();
	}

	void Widget::setText(const Ogre::UTFString& text)
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
		_notifyTextChanged();
	}

	void Widget::setZOrderOffset(int offset, bool updateWindowZList)
	{
		mZOrderOffset = offset;

		if(updateWindowZList)
		{
			int zOrder = 0;
			if( mParentWidget != NULL ) zOrder = mParentWidget->getZOrder();

			Window* w = getWindow();
			if( w != NULL ) w->_addZOrderValue(zOrder + mZOrderOffset);
			else getSheet()->_addZOrderValue(zOrder + mZOrderOffset);
		}
	}

	void Widget::show()
	{
		mVisible = true;
		if( !mBordersHidden )
		{
			if( mBorders[QGUI_BORDER_TOP] ) mBorders[QGUI_BORDER_TOP]->show();
			if( mBorders[QGUI_BORDER_BOTTOM] ) mBorders[QGUI_BORDER_BOTTOM]->show();
			if( mBorders[QGUI_BORDER_LEFT] ) mBorders[QGUI_BORDER_LEFT]->show();
			if( mBorders[QGUI_BORDER_RIGHT] ) mBorders[QGUI_BORDER_RIGHT]->show();
		}

		if( mOverlayElement ) mOverlayElement->show();

		// show children
		// hide children
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->show();
	}

	void Widget::showBorders()
	{
		mBordersHidden = false;
		if( mBorders[QGUI_BORDER_TOP] ) mBorders[QGUI_BORDER_TOP]->show();
		if( mBorders[QGUI_BORDER_BOTTOM] ) mBorders[QGUI_BORDER_BOTTOM]->show();
		if( mBorders[QGUI_BORDER_LEFT] ) mBorders[QGUI_BORDER_LEFT]->show();
		if( mBorders[QGUI_BORDER_RIGHT] ) mBorders[QGUI_BORDER_RIGHT]->show();
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
