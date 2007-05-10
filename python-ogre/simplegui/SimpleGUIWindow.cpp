#include "SimpleGUIWindow.h"
// included to get access to default font/fontsize
#include "SimpleGUIManager.h"

namespace SimpleGUI
{
	Window::Window(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show) :
		Widget(name,dimensions,NULL,NULL),
		mMaterial(material),
		mWindowBorderOverlayElement(0),
		mTitleBar(0),
		mZOrder(0),
		mAutoNameWidgetCounter(0)
	{
		_init();
		mWidgetType = Widget::SGUI_WINDOW;

		// Border Overlay gives us ability to assign material to Window Borders and Panel separately.
		mWindowBorderOverlayElement = createBorderPanelOverlayElement(mInstanceName+"_Border",mPixelDimensions,mMaterial);
		mWindowBorderOverlayElement->setBorderMaterialName(mMaterial+".border");
		mWindowBorderOverlayElement->setBorderSize(3,3);
		mOverlayContainer->addChild(mWindowBorderOverlayElement);
		mWindowBorderOverlayElement->show();

		// Create TitleBar - tradition titlebar dimensions: across the top of the window
		Ogre::Vector4 defaultTitleBarDimensions = Ogre::Vector4(0,0,1,0.05 / getAbsoluteDimensions().w);
		mTitleBar = new TitleBar(mInstanceName+"_Titlebar",defaultTitleBarDimensions,mMaterial+".titlebar",true,mLevel1Container,this);

		if( show ) this->show();
		else hide();
	}

	Window::Window(const Ogre::String& name, const Ogre::Vector4& dimensions, bool show) :
		Widget(name,dimensions,NULL,NULL),
		mMaterial(""),
		mWindowBorderOverlayElement(0),
		mTitleBar(0),
		mAutoNameWidgetCounter(0)
	{
		_init();
		mWidgetType = Widget::SGUI_PLAINWINDOW;

		// Border Overlay gives us ability to assign material to window background and panels separately.
		mWindowBorderOverlayElement = createBorderPanelOverlayElement(mInstanceName+"_Border",mPixelDimensions,"");
		mOverlayContainer->addChild(mWindowBorderOverlayElement);
		mWindowBorderOverlayElement->show();

		if( show ) this->show();
		else hide();
	}

	Window::~Window()
	{
		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		// Delete all child widgets before deleting the window widget
		
		std::vector<Label*>::iterator lItr;
		for( lItr = mLabels.begin(); lItr != mLabels.end(); ++lItr )
			delete (*lItr);
		mLabels.clear();

		std::vector<NStateButton*>::iterator nIt;
		for( nIt = mNStateButtons.begin(); nIt != mNStateButtons.end(); ++nIt )
			delete (*nIt);
		mNStateButtons.clear();
		
		std::vector<Button*>::iterator bItr;
		for( bItr = mButtons.begin(); bItr != mButtons.end(); ++bItr )
			delete (*bItr);
		mButtons.clear();

		std::vector<ComboBox*>::iterator cIt;
		for( cIt = mComboBoxes.begin(); cIt != mComboBoxes.end(); ++cIt )
			delete (*cIt);
		mComboBoxes.clear();

		std::vector<Image*>::iterator iIt;
		for( iIt = mImages.begin(); iIt != mImages.end(); ++iIt )
			delete (*iIt);
		mImages.clear();

		std::vector<TextBox*>::iterator tItr;
		for( tItr = mTextBoxes.begin(); tItr != mTextBoxes.end(); ++tItr )
			delete (*tItr);
		mTextBoxes.clear();

		delete mTitleBar;

		std::vector<Menu*>::iterator mItr;
		for( mItr = mMenus.begin(); mItr != mMenus.end(); ++mItr )
			delete (*mItr);
		mMenus.clear();

		std::vector<ProgressBar*>::iterator pbItr;
		for( pbItr = mProgressBars.begin(); pbItr != mProgressBars.end(); ++pbItr )
			delete (*pbItr);
		mProgressBars.clear();
		
		// destroy border overlay element
		mOverlayContainer->removeChild(mWindowBorderOverlayElement->getName());
		om->destroyOverlayElement(mWindowBorderOverlayElement);
	
		// destroy overlay containers
		mLevel3Container->removeChild(mLevel4Container->getName());
		om->destroyOverlayElement(mLevel4Container);

		mLevel2Container->removeChild(mLevel3Container->getName());
		om->destroyOverlayElement(mLevel3Container);

		mLevel1Container->removeChild(mLevel2Container->getName());
		om->destroyOverlayElement(mLevel2Container);

		mOverlayContainer->removeChild(mLevel1Container->getName());
		om->destroyOverlayElement(mLevel1Container);
		
		mWindowOverlay->remove2D(mOverlayContainer);
		om->destroyOverlayElement(mOverlayContainer);

		// destroy overlay
		om->destroy(mWindowOverlay);
	}

	void Window::_applyDimensions()
	{
		mWindowBorderOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mWindowBorderOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
	}

	void Window::_init()
	{
		mMenus.clear();
		mButtons.clear();
		mComboBoxes.clear();
		mLabels.clear();
		mTextBoxes.clear();

		mTitleBarHidden = false;

		mWindowOverlay = Ogre::OverlayManager::getSingleton().create(mInstanceName+"_Overlay");
		// Create Window Container, note we are not setting a background.
		mOverlayContainer = createOverlayContainer(mInstanceName+"_OverlayContainer0","");
		mLevel1Container = createOverlayContainer(mInstanceName+"_OverlayContainer1","");
		mLevel2Container = createOverlayContainer(mInstanceName+"_OverlayContainer2","");
		mLevel3Container = createOverlayContainer(mInstanceName+"_OverlayContainer3","");
		mLevel4Container = createOverlayContainer(mInstanceName+"_OverlayContainer4","");
		
		mOverlayContainer->addChildImpl(mLevel1Container);
		mLevel1Container->addChildImpl(mLevel2Container);
		mLevel2Container->addChildImpl(mLevel3Container);
		mLevel3Container->addChildImpl(mLevel4Container);
		
		mWindowOverlay->add2D(mOverlayContainer);
		mOverlayContainer->show();
		mLevel1Container->show();
		mLevel2Container->show();
		mLevel3Container->show();
		mLevel4Container->show();
	}

	void Window::_notifyDimensionsChanged()
	{
		_updateDimensions(mRelativeDimensions);
		_applyDimensions();
		
		if(mTitleBar) mTitleBar->_notifyDimensionsChanged();

		std::vector<Menu*>::iterator mIt;
		for( mIt = mMenus.begin(); mIt != mMenus.end(); ++mIt )
			(*mIt)->_notifyDimensionsChanged();

		std::vector<NStateButton*>::iterator nIt;
		for( nIt = mNStateButtons.begin(); nIt != mNStateButtons.end(); ++nIt )
			(*nIt)->_notifyDimensionsChanged();

		std::vector<Button*>::iterator bIt;
		for( bIt = mButtons.begin(); bIt != mButtons.end(); ++bIt )
			(*bIt)->_notifyDimensionsChanged();

		std::vector<ComboBox*>::iterator cIt;
		for( cIt = mComboBoxes.begin(); cIt != mComboBoxes.end(); ++cIt )
			(*cIt)->_notifyDimensionsChanged();

		std::vector<Image*>::iterator iIt;
		for( iIt = mImages.begin(); iIt != mImages.end(); ++iIt )
			(*iIt)->_notifyDimensionsChanged();

		std::vector<TextBox*>::iterator tIt;
		for( tIt = mTextBoxes.begin(); tIt != mTextBoxes.end(); ++tIt )
			(*tIt)->_notifyDimensionsChanged();

		std::vector<Label*>::iterator lIt;
		for( lIt = mLabels.begin(); lIt != mLabels.end(); ++lIt )
			(*lIt)->_notifyDimensionsChanged();

		std::vector<ProgressBar*>::iterator pbItr;
		for( pbItr = mProgressBars.begin(); pbItr != mProgressBars.end(); ++pbItr )
			(*pbItr)->_notifyDimensionsChanged();
	}

	void Window::activate(EventArgs& e)
	{
		GUIManager::getSingleton().setActiveWindow(this);

		// Assign active material, if exists

		// execute any user defined event handler callbacks
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mUserEventHandlers[SGUI_ACTIVATED].begin(); it != mUserEventHandlers[SGUI_ACTIVATED].end(); ++it )
			e.handled = (*it)->execute(e);

		// Fire activate event for all child widgets
		if(mTitleBar) mTitleBar->activate(e);

		std::vector<Menu*>::iterator mIt;
		for( mIt = mMenus.begin(); mIt != mMenus.end(); ++mIt )
			(*mIt)->activate(e);

		std::vector<NStateButton*>::iterator nIt;
		for( nIt = mNStateButtons.begin(); nIt != mNStateButtons.end(); ++nIt )
			(*nIt)->activate(e);

		std::vector<Button*>::iterator bIt;
		for( bIt = mButtons.begin(); bIt != mButtons.end(); ++bIt )
			(*bIt)->activate(e);

		std::vector<ComboBox*>::iterator cIt;
		for( cIt = mComboBoxes.begin(); cIt != mComboBoxes.end(); ++cIt )
			(*cIt)->activate(e);

		std::vector<Image*>::iterator iIt;
		for( iIt = mImages.begin(); iIt != mImages.end(); ++iIt )
			(*iIt)->activate(e);

		std::vector<TextBox*>::iterator tIt;
		for( tIt = mTextBoxes.begin(); tIt != mTextBoxes.end(); ++tIt )
			(*tIt)->activate(e);

		std::vector<Label*>::iterator lIt;
		for( lIt = mLabels.begin(); lIt != mLabels.end(); ++lIt )
			(*lIt)->activate(e);

		std::vector<ProgressBar*>::iterator pbItr;
		for( pbItr = mProgressBars.begin(); pbItr != mProgressBars.end(); ++pbItr )
			(*pbItr)->activate(e);
	}

	Button* Window::createButton(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Button* newButton = new Button(name,dimensions,material,show,mLevel1Container,this);
		mButtons.push_back(newButton);
		
		return newButton;
	}

	Button* Window::createButton(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Button* newButton = new Button(name,dimensions,material,show,mLevel1Container,this);
		mButtons.push_back(newButton);
		
		return newButton;
	}

	ComboBox* Window::createComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::String highlightMaterial, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		ComboBox* newComboBox = new ComboBox(name,dimensions,material,highlightMaterial,show,mLevel3Container,this);
		mComboBoxes.push_back(newComboBox);
		
		return newComboBox;
	}

	ComboBox* Window::createComboBox(const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::String highlightMaterial, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		ComboBox* newComboBox = new ComboBox(name,dimensions,material,highlightMaterial,show,mLevel3Container,this);
		mComboBoxes.push_back(newComboBox);
		
		return newComboBox;
	}

	Button* Window::createFullSizeButton(const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_WINDOW ) return NULL;

		Button* newButton = new Button(mInstanceName + "_FullSizeButton",Ogre::Vector4(0,0,1,1),material,true,mLevel1Container,this);
		mButtons.push_back(newButton);

		return newButton;
	}

	ComboBox* Window::createFullSizeComboBox(const Ogre::String& material, Ogre::String highlightMaterial, bool show)
	{
		if( mWidgetType == SGUI_WINDOW ) return NULL;

		ComboBox* newComboBox = new ComboBox(mInstanceName + "_FullSizeComboBox",Ogre::Vector4(0,0,1,1),material,highlightMaterial,true,mLevel1Container,this);
		mComboBoxes.push_back(newComboBox);

		return newComboBox;
	}

	Image* Window::createFullSizeImage(const Ogre::String& material, bool texture, bool show)
	{
		if( mWidgetType == SGUI_WINDOW ) return NULL;

		Image* newImage = new Image(mInstanceName + "_FullSizeImage",Ogre::Vector4(0,0,1,1),"",show,mLevel1Container,this);
		if(texture) newImage->setMaterial(material,true);
		else newImage->setMaterial(material);
		mImages.push_back(newImage);

		return newImage;
	}

	Label* Window::createFullSizeLabel(const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_WINDOW ) return NULL;

		GUIManager* gm = GUIManager::getSingletonPtr();
		Label* newLabel = new Label(mInstanceName + "_FullSizeLabel",Ogre::Vector4(0,0,1,1),material,show,mOverlayContainer,this);
		mLabels.push_back(newLabel);

		return newLabel;
	}

	Menu* Window::createFullSizeMenu(const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_WINDOW ) return NULL;

		Menu* newMenu = new Menu(mInstanceName + "_FullSizeMenu",Ogre::Vector4(0,0,1,1),material,show,mLevel3Container,this);
		mMenus.push_back(newMenu);

		return newMenu;
	}

	NStateButton* Window::createFullSizeNStateButton(bool show)
	{
		if( mWidgetType == SGUI_WINDOW ) return NULL;

		NStateButton* newNStateButton = new NStateButton(mInstanceName + "_FullSizeNStateButton",Ogre::Vector4(0,0,1,1),show,mLevel1Container,this);
		mNStateButtons.push_back(newNStateButton);

		return newNStateButton;
	}

	ProgressBar* Window::createFullSizeProgressBar(const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_WINDOW ) return NULL;

		ProgressBar* newProgressBar = new ProgressBar(mInstanceName + "_FullSizeProgressBar",Ogre::Vector4(0,0,1,1),material,show,mLevel1Container,this);
		mProgressBars.push_back(newProgressBar);

		return newProgressBar;
	}

	TextBox* Window::createFullSizeTextBox(const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_WINDOW ) return NULL;

		TextBox* newTextBox = new TextBox(mInstanceName + "_FullSizeTextBox",Ogre::Vector4(0,0,1,1),material,show,mLevel1Container,this);
		mTextBoxes.push_back(newTextBox);

		return newTextBox;
	}

	Image* Window::createImage(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Image* newImage = new Image(name,dimensions,"",show,mLevel1Container,this);
		if(texture) newImage->setMaterial(material,true);
		else newImage->setMaterial(material);
		mImages.push_back(newImage);

		return newImage;
	}

	Image* Window::createImage(const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Image* newImage = new Image(name,dimensions,"",show,mLevel1Container,this);
		if(texture) newImage->setMaterial(material,true);
		else newImage->setMaterial(material);
		mImages.push_back(newImage);

		return newImage;
	}

	Label* Window::createLabel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Label* newLabel = new Label(name,dimensions,material,show,mLevel1Container,this);
		mLabels.push_back(newLabel);

		return newLabel;
	}

	Label* Window::createLabel(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Label* newLabel = new Label(name,dimensions,material,show,mLevel1Container,this);
		mLabels.push_back(newLabel);

		return newLabel;
	}

	Menu* Window::createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Menu* newMenu = new Menu(name,dimensions,material,show,mLevel3Container,this);
		mMenus.push_back(newMenu);

		return newMenu;
	}

	Menu* Window::createMenu(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Menu* newMenu = new Menu(name,dimensions,material,show,mLevel3Container,this);
		mMenus.push_back(newMenu);

		return newMenu;
	}

	NStateButton* Window::createNStateButton(const Ogre::String& name, const Ogre::Vector4& dimensions, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		NStateButton* newNStateButton = new NStateButton(name,dimensions,show,mLevel1Container,this);
		mNStateButtons.push_back(newNStateButton);

		return newNStateButton;
	}

	NStateButton* Window::createNStateButton(const Ogre::Vector4& dimensions, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		NStateButton* newNStateButton = new NStateButton(name,dimensions,show,mLevel1Container,this);
		mNStateButtons.push_back(newNStateButton);

		return newNStateButton;
	}

	ProgressBar* Window::createProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		ProgressBar* newProgressBar = new ProgressBar(name,dimensions,material,show,mLevel1Container,this);
		mProgressBars.push_back(newProgressBar);

		return newProgressBar;
	}

	ProgressBar* Window::createProgressBar(const Ogre::Vector4& dimensions,  const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		ProgressBar* newProgressBar = new ProgressBar(name,dimensions,material,show,mLevel1Container,this);
		mProgressBars.push_back(newProgressBar);

		return newProgressBar;
	}

	TextBox* Window::createTextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		TextBox* newTextBox = new TextBox(name,dimensions,material,show,mLevel1Container,this);
		mTextBoxes.push_back(newTextBox);

		return newTextBox;
	}

	TextBox* Window::createTextBox(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show)
	{
		if( mWidgetType == SGUI_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		TextBox* newTextBox = new TextBox(name,dimensions,material,show,mLevel1Container,this);
		mTextBoxes.push_back(newTextBox);

		return newTextBox;
	}

	void Window::deactivate(EventArgs& e)
	{
		// Assign active material, if exists

		// execute any user defined event handler callbacks
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[SGUI_ACTIVATED];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		
		// Fire activate event for all child widgets
		if(mTitleBar) mTitleBar->deactivate(e);

		std::vector<Menu*>::iterator mIt;
		for( mIt = mMenus.begin(); mIt != mMenus.end(); ++mIt )
			(*mIt)->deactivate(e);

		std::vector<NStateButton*>::iterator nIt;
		for( nIt = mNStateButtons.begin(); nIt != mNStateButtons.end(); ++nIt )
			(*nIt)->deactivate(e);

		std::vector<Button*>::iterator bIt;
		for( bIt = mButtons.begin(); bIt != mButtons.end(); ++bIt )
			(*bIt)->deactivate(e);

		std::vector<ComboBox*>::iterator cIt;
		for( cIt = mComboBoxes.begin(); cIt != mComboBoxes.end(); ++cIt )
			(*cIt)->deactivate(e);

		std::vector<Image*>::iterator iIt;
		for( iIt = mImages.begin(); iIt != mImages.end(); ++iIt )
			(*iIt)->deactivate(e);

		std::vector<TextBox*>::iterator tIt;
		for( tIt = mTextBoxes.begin(); tIt != mTextBoxes.end(); ++tIt )
			(*tIt)->deactivate(e);

		std::vector<Label*>::iterator lIt;
		for( lIt = mLabels.begin(); lIt != mLabels.end(); ++lIt )
			(*lIt)->deactivate(e);

		std::vector<ProgressBar*>::iterator pbItr;
		for( pbItr = mProgressBars.begin(); pbItr != mProgressBars.end(); ++pbItr )
			(*pbItr)->deactivate(e);
	}

	Button* Window::getButton(unsigned int index)
	{
		if( (static_cast<int>(mButtons.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mButtons[index];
	}

	Button* Window::getButton(const Ogre::String& name)
	{
		std::vector<Button*>::iterator it;
		for( it = mButtons.begin(); it != mButtons.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}

		return NULL;
	}

	ComboBox* Window::getComboBox(unsigned int index)
	{
		if( (static_cast<int>(mComboBoxes.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mComboBoxes[index];
	}

	ComboBox* Window::getComboBox(const Ogre::String& name)
	{
		std::vector<ComboBox*>::iterator it;
		for( it = mComboBoxes.begin(); it != mComboBoxes.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}

		return NULL;
	}

	Image* Window::getImage(unsigned int index)
	{
		if( (static_cast<int>(mImages.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mImages[index];
	}

	Image* Window::getImage(const Ogre::String& name)
	{
		std::vector<Image*>::iterator it;
		for( it = mImages.begin(); it != mImages.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}

		return NULL;
	}

	Label* Window::getLabel(unsigned int index)
	{
		if( (static_cast<int>(mLabels.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mLabels[index];
	}

	Label* Window::getLabel(const Ogre::String& name)
	{
		std::vector<Label*>::iterator it;
		for( it = mLabels.begin(); it != mLabels.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}

		return NULL;
	}

	Menu* Window::getMenu( unsigned int index )
	{
		if( (static_cast<int>(mMenus.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mMenus[index];
	}

	Menu* Window::getMenu(const Ogre::String& name)
	{
		std::vector<Menu*>::iterator it;
		for( it = mMenus.begin(); it != mMenus.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}

		return NULL;
	}

	NStateButton* Window::getNStateButton(unsigned int index)
	{
		if( (static_cast<int>(mNStateButtons.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mNStateButtons[index];
	}
		
	NStateButton* Window::getNStateButton(const Ogre::String& name)
	{
		std::vector<NStateButton*>::iterator it;
		for( it = mNStateButtons.begin(); it != mNStateButtons.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}

		return NULL;
	}

	Widget* Window::getTargetWidget(const Ogre::Vector2& p)
	{
		if(!mVisible) return NULL;

		if( mTitleBar && (mTitleBar->getTargetWidget(p)) ) return mTitleBar->getTargetWidget(p);
		
		// Menu's and Combo Boxes need to be checked first, as they may have lists that lay
		// on top of other widgets
		std::vector<Menu*>::iterator mIt;
		for( mIt = mMenus.begin(); mIt != mMenus.end(); ++mIt )
		{
			if((*mIt)->getTargetWidget(p)) 
				return (*mIt)->getTargetWidget(p);
		}

		std::vector<ComboBox*>::iterator cIt;
		for( cIt = mComboBoxes.begin(); cIt != mComboBoxes.end(); ++cIt )
		{
			if((*cIt)->getTargetWidget(p)) 
				return (*cIt)->getTargetWidget(p);
		}

		std::vector<NStateButton*>::iterator nIt;
		for( nIt = mNStateButtons.begin(); nIt != mNStateButtons.end(); ++nIt )
		{
			if((*nIt)->isPointWithinBounds(p)) return (*nIt);
		}

		std::vector<Button*>::iterator bIt;
		for( bIt = mButtons.begin(); bIt != mButtons.end(); ++bIt )
		{
			if((*bIt)->isPointWithinBounds(p)) return (*bIt);
		}

		std::vector<Image*>::iterator iIt;
		for( iIt = mImages.begin(); iIt != mImages.end(); ++iIt )
		{
			if((*iIt)->isPointWithinBounds(p)) return (*iIt);
		}

		std::vector<TextBox*>::iterator tItr;
		for( tItr = mTextBoxes.begin(); tItr != mTextBoxes.end(); ++tItr )
		{
			if((*tItr)->isPointWithinBounds(p)) return (*tItr);
		}

		std::vector<Label*>::iterator lItr;
		for( lItr = mLabels.begin(); lItr != mLabels.end(); ++lItr )
		{
			if((*lItr)->isPointWithinBounds(p)) return (*lItr);
		}

		std::vector<ProgressBar*>::iterator pbItr;
		for( pbItr = mProgressBars.begin(); pbItr != mProgressBars.end(); ++pbItr )
		{
			if((*pbItr)->isPointWithinBounds(p)) return (*pbItr);
		}

		if(isPointWithinBounds(p)) return this;

		return NULL;
	}

	TextBox* Window::getTextBox(unsigned int index)
	{
		if( mTextBoxes.size() < index ) return NULL;
		return mTextBoxes[index];
	}

	TextBox* Window::getTextBox(const Ogre::String& name)
	{
		std::vector<TextBox*>::iterator it;
		for( it = mTextBoxes.begin(); it != mTextBoxes.end(); ++it )
		{
			if((*it)->getInstanceName() == name) return (*it);
		}

		return NULL;
	}

	TitleBar* Window::getTitleBar()
	{
		return mTitleBar;
	}

	Ogre::ushort Window::getZOrder()
	{
		return mZOrder;
	}

	bool Window::hasVisibleBorders()
	{
		return (mWindowBorderOverlayElement->getBorderMaterialName() == (mMaterial+".window.border"));
	}

	void Window::hide()
	{
		mWindowOverlay->hide();

		Widget::hide();
	}

	bool Window::hide(const EventArgs& e)
	{
		hide();

		return true;
	}

	void Window::hideBorders()
	{
		mWindowBorderOverlayElement->setBorderMaterialName("transparent");
	}

	void Window::hideCloseButton()
	{
		if(mTitleBar) mTitleBar->hideCloseButton();
	}

	void Window::hideMenus()
	{
		std::vector<Menu*>::iterator it;
		for( it = mMenus.begin(); it != mMenus.end(); ++it )
			(*it)->hide();
	}

	void Window::hideTitlebar()
	{
		if(mTitleBar) 
		{
			mTitleBar->hide();
			mTitleBarHidden = true;
		}
	}

	bool Window::isGrabbed()
	{
		return mGrabbed;
	}

	bool Window::onMouseButtonDown(MouseEventArgs& e) 
	{ 
		// perform any default functionality
		activate(e);

		GUIManager::getSingleton().setActiveWindow(this);

		return Widget::onMouseButtonDown(e);
	}

	void Window::setBorderSize(const Ogre::Real& left, const Ogre::Real& right, const Ogre::Real& top, const Ogre::Real& bottom)
	{
		mWindowBorderOverlayElement->setBorderSize(left,right,top,bottom);
	}

	void Window::setBorderSize(const Ogre::Real& sides, const Ogre::Real& topAndBottom)
	{
		mWindowBorderOverlayElement->setBorderSize(sides,topAndBottom);
	}

	void Window::setMaterial(const Ogre::String& material)
	{
		mMaterial = material;
		mWindowBorderOverlayElement->setMaterialName(mMaterial);
	}

	void Window::setPosition(const Ogre::Real& xRelativePosition, const Ogre::Real& yRelativePosition)
	{
		if(!mMovingEnabled) return;

		mRelativeDimensions.x = xRelativePosition;
		mRelativeDimensions.y = yRelativePosition;
		_notifyDimensionsChanged();
	}

	void Window::setText(const Ogre::String& text)
	{
		mText = text;
		mTitleBar->setFont(mFont);
		mTitleBar->setCharacterHeight(mCharacterHeight);
		mTitleBar->setTextColor(mTextTopColor,mTextBotColor);
		mTitleBar->setText(mText);
	}

	void Window::setTextColor(const Ogre::ColourValue& color)
	{
		setTextColor(color,color);
	}

	void Window::setTextColor(const Ogre::ColourValue& topColor,const Ogre::ColourValue& botColor)
	{
		mTextTopColor = topColor;
		mTextBotColor = botColor;
		mTitleBar->setFont(mFont);
		mTitleBar->setCharacterHeight(mCharacterHeight);
		mTitleBar->setTextColor(mTextTopColor,mTextBotColor);
		mTitleBar->setText(mText);
	}

	void Window::setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer)
	{
		mWindowBorderOverlayElement->setTiling(horizontalRepetitions,verticalRepetitions,layer);
	}

	void Window::setTitleBarHeight(Ogre::Real height)
	{
		if( height > mWindowBorderOverlayElement->getHeight() ) height = mWindowBorderOverlayElement->getHeight();

		if(mTitleBar) mTitleBar->setHeight(height);
	}

	void Window::setZOrder(const Ogre::ushort& zOrder)
	{
		mZOrder = zOrder;
		mWindowOverlay->setZOrder(mZOrder);
	}

	void Window::show()
	{
		mWindowOverlay->show();
		Widget::show();
	}

	void Window::showBorders()
	{
		mWindowBorderOverlayElement->setBorderMaterialName(mMaterial+".window.border");
	}

	void Window::showCloseButton()
	{
		if(mTitleBar) mTitleBar->showCloseButton();
	}

	void Window::showMenus()
	{
		std::vector<Menu*>::iterator it;
		for( it = mMenus.begin(); it != mMenus.end(); ++it )
			(*it)->show();
	}

	void Window::showTitlebar()
	{
		if(mTitleBar) 
		{
			mTitleBar->show();
			mTitleBarHidden = false;
		}
	}

	void Window::timeElapsed(Ogre::Real time)
	{
		std::vector<Label*>::iterator lItr;
		for( lItr = mLabels.begin(); lItr != mLabels.end(); ++lItr )
			(*lItr)->timeElapsed(time);

		std::vector<NStateButton*>::iterator nIt;
		for( nIt = mNStateButtons.begin(); nIt != mNStateButtons.end(); ++nIt )
			(*nIt)->timeElapsed(time);
		
		std::vector<Button*>::iterator bItr;
		for( bItr = mButtons.begin(); bItr != mButtons.end(); ++bItr )
			(*bItr)->timeElapsed(time);

		std::vector<ComboBox*>::iterator cIt;
		for( cIt = mComboBoxes.begin(); cIt != mComboBoxes.end(); ++cIt )
			(*cIt)->timeElapsed(time);

		std::vector<Image*>::iterator iIt;
		for( iIt = mImages.begin(); iIt != mImages.end(); ++iIt )
			(*iIt)->timeElapsed(time);

		std::vector<TextBox*>::iterator tItr;
		for( tItr = mTextBoxes.begin(); tItr != mTextBoxes.end(); ++tItr )
			(*tItr)->timeElapsed(time);

		if(mTitleBar) mTitleBar->timeElapsed(time);

		std::vector<Menu*>::iterator mItr;
		for( mItr = mMenus.begin(); mItr != mMenus.end(); ++mItr )
			(*mItr)->timeElapsed(time);

		std::vector<ProgressBar*>::iterator pbItr;
		for( pbItr = mProgressBars.begin(); pbItr != mProgressBars.end(); ++pbItr )
			(*pbItr)->timeElapsed(time);
	}
}
