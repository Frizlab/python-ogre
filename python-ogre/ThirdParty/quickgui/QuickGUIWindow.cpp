#include "QuickGUIWindow.h"
// included to get access to default font/fontsize
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Window::Window(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material) :
		Widget(name,dimensions,material,NULL,NULL),
		mWindowOverlayElement(0),
		mTitleBar(0),
		mZOrder(0),
		mAutoNameWidgetCounter(0)
	{
		_init();
		mWidgetType = Widget::QGUI_TYPE_WINDOW;

		mWindowOverlayElement = createPanelOverlayElement(mInstanceName+"_Background",mPixelDimensions,mWidgetMaterial);
		mOverlayContainer->addChild(mWindowOverlayElement);
		mWindowOverlayElement->show();

		// Create TitleBar - tradition titlebar dimensions: across the top of the window
		Ogre::Vector4 defaultTitleBarDimensions = Ogre::Vector4(0,0,1,0.05 / getAbsoluteDimensions().w);
		mTitleBar = new TitleBar(mInstanceName+"_Titlebar",defaultTitleBarDimensions,mWidgetMaterial+".titlebar",mLevel1Container,this);
		mTitleBar->_notifyZOrder(mZOrder,1);

		// Now that mOverlayContainer has been created (via _init() function) we can create the borders
		_createBorders();
	}

	Window::Window(const Ogre::String& name, const Ogre::Vector4& dimensions) :
		Widget(name,dimensions,"",NULL,NULL),
		mWindowOverlayElement(0),
		mTitleBar(0),
		mAutoNameWidgetCounter(0)
	{
		_init();
		mWidgetType = Widget::QGUI_TYPE_PLAINWINDOW;

		mWindowOverlayElement = createPanelOverlayElement(mInstanceName+"_Background",mPixelDimensions,"");
		mOverlayContainer->addChild(mWindowOverlayElement);
		mWindowOverlayElement->show();
	}

	Window::~Window()
	{
		_destroyBorders();

		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		if(mTitleBar) delete mTitleBar;

		// Delete all child widgets before deleting the window widget

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			delete (*it);
		mChildWidgets.clear();
		
		// destroy background overlay element
		mOverlayContainer->removeChild(mWindowOverlayElement->getName());
		om->destroyOverlayElement(mWindowOverlayElement);
		mWindowOverlayElement = NULL;
	
		// destroy overlay containers
		mLevel3Container->removeChild(mLevel4Container->getName());
		om->destroyOverlayElement(mLevel4Container);
		mLevel4Container = NULL;

		mLevel2Container->removeChild(mLevel3Container->getName());
		om->destroyOverlayElement(mLevel3Container);
		mLevel3Container = NULL;

		mLevel1Container->removeChild(mLevel2Container->getName());
		om->destroyOverlayElement(mLevel2Container);
		mLevel2Container = NULL;

		mOverlayContainer->removeChild(mLevel1Container->getName());
		om->destroyOverlayElement(mLevel1Container);
		mLevel1Container = NULL;
		
		mWindowOverlay->remove2D(mOverlayContainer);
		om->destroyOverlayElement(mOverlayContainer);
		mOverlayContainer = NULL;

		// destroy overlay
		om->destroy(mWindowOverlay);
	}

	void Window::_applyDimensions()
	{
		mWindowOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mWindowOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
	}

	Button* Window::_createButton(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Button* newButton = new Button(name,dimensions,material,mLevel1Container,this);
		newButton->_notifyZOrder(mZOrder,1);
		mChildWidgets.push_back(newButton);
		// update count
		++mNumButtons;
		
		return newButton;
	}

	ComboBox* Window::_createComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::String highlightMaterial)
	{
		ComboBox* newComboBox = new ComboBox(name,dimensions,material,highlightMaterial,mLevel3Container,this);
		newComboBox->_notifyZOrder(mZOrder,3);
		mChildWidgets.push_back(newComboBox);
		// update count
		++mNumComboBoxes;
		
		return newComboBox;
	}

	Image* Window::_createImage(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture)
	{
		Ogre::String defaultMaterial = GUIManager::getSingleton().getDefaultSkin() + ".image";
		Image* newImage = new Image(name,dimensions,defaultMaterial,mLevel1Container,this);
		newImage->_notifyZOrder(mZOrder,1);
		if(texture) newImage->setMaterial(material,true);
		else newImage->setMaterial(material);
		mChildWidgets.push_back(newImage);
		// update count
		++mNumImages;

		return newImage;
	}

	Label* Window::_createLabel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Label* newLabel = new Label(name,dimensions,material,mLevel1Container,this);
		newLabel->_notifyZOrder(mZOrder,1);
		mChildWidgets.push_back(newLabel);
		// update count
		++mNumLabels;

		return newLabel;
	}

	Menu* Window::_createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Menu* newMenu = new Menu(name,dimensions,material,mLevel3Container,this);
		newMenu->_notifyZOrder(mZOrder,3);
		mChildWidgets.push_back(newMenu);
		// update count
		++mNumMenus;

		return newMenu;
	}

	NStateButton* Window::_createNStateButton(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		NStateButton* newNStateButton = new NStateButton(name,dimensions,mLevel1Container,this);
		newNStateButton->_notifyZOrder(mZOrder,1);
		mChildWidgets.push_back(newNStateButton);
		// update count
		++mNumNStateButtons;

		return newNStateButton;
	}

	ProgressBar* Window::_createProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		ProgressBar* newProgressBar = new ProgressBar(name,dimensions,material,mLevel1Container,this);
		newProgressBar->_notifyZOrder(mZOrder,1);
		mChildWidgets.push_back(newProgressBar);
		// update count
		++mNumProgressBars;

		return newProgressBar;
	}

	TextBox* Window::_createTextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		TextBox* newTextBox = new TextBox(name,dimensions,material,mLevel1Container,this);
		newTextBox->_notifyZOrder(mZOrder,1);
		mChildWidgets.push_back(newTextBox);
		// update count
		++mNumTextBoxes;

		return newTextBox;
	}

	bool Window::_destroyWidget(Widget::Type TYPE, unsigned int index)
	{
		int counter = -1;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getWidgetType() == TYPE )
			{
				++counter;
				if( counter == index )
				{
					Widget* w = (*it);
					mChildWidgets.erase(it);
					delete w;
					return true;
				}
			}
		}
		return false;
	}

	bool Window::_destroyWidget(const Ogre::String& name)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( ((*it)->getInstanceName() == name) || ((*it)->getReferenceName() == name) )
			{
				Widget* w = (*it);
				mChildWidgets.erase(it);
				delete w;
				return true;
			}
		}
		return false;
	}

	Widget* Window::_getWidget(Widget::Type TYPE, unsigned int index)
	{
		int counter = -1;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getWidgetType() == TYPE )
			{
				++counter;
				if( counter == index ) return (*it);
			}
		}
		return NULL;
	}

	Widget* Window::_getWidget(const Ogre::String& name)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( ((*it)->getInstanceName() == name) || ((*it)->getReferenceName() == name) ) return (*it);
		}
		return NULL;
	}

	void Window::_init()
	{
		mTitleBarHidden = false;
		mNumButtons = 0;
		mNumComboBoxes = 0;
		mNumImages = 0;
		mNumLabels = 0;
		mNumMenus = 0;
		mNumNStateButtons = 0;
		mNumProgressBars = 0;
		mNumTextBoxes = 0;

		mWindowOverlay = Ogre::OverlayManager::getSingleton().create(mInstanceName+"_Overlay");
		mWindowOverlay->show();
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

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->_notifyDimensionsChanged();

		Widget::_notifyDimensionsChanged();
	}

	void Window::activate(EventArgs& e)
	{
		GUIManager::getSingleton().setActiveWindow(this);

		// Assign active material, if exists

		// execute any user defined event handler callbacks
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mUserEventHandlers[QGUI_EVENT_ACTIVATED].begin(); it != mUserEventHandlers[QGUI_EVENT_ACTIVATED].end(); ++it )
			e.handled = (*it)->execute(e);

		// Fire activate event for all child widgets
		if(mTitleBar) mTitleBar->activate(e);

		std::vector<Widget*>::iterator wIt;
		for( wIt = mChildWidgets.begin(); wIt != mChildWidgets.end(); ++wIt )
		{
			(*wIt)->activate(e);
		}
	}

	Button* Window::createButton(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createButton(name,dimensions,material);
	}

	Button* Window::createButton(const Ogre::Vector4& dimensions)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".button";

		return _createButton(name,dimensions,material);
	}

	ComboBox* Window::createComboBox(const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::String highlightMaterial)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createComboBox(name,dimensions,material,highlightMaterial);
	}

	ComboBox* Window::createComboBox(const Ogre::Vector4& dimensions)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;
		
		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".combobox";

		Ogre::String highlightMaterial = GUIManager::getSingleton().getDefaultSkin() + ".listitem.highlight";

		return _createComboBox(name,dimensions,material,highlightMaterial);
	}

	Button* Window::createFullSizeButton(const Ogre::String& material)
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeButton";

		return _createButton(name,Ogre::Vector4(0,0,1,1),material);
	}

	Button* Window::createFullSizeButton()
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeButton";

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".button";

		return _createButton(name,Ogre::Vector4(0,0,1,1),material);
	}

	ComboBox* Window::createFullSizeComboBox(const Ogre::String& material, Ogre::String highlightMaterial)
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeComboBox";

		return _createComboBox(name,Ogre::Vector4(0,0,1,1),material,highlightMaterial);
	}

	ComboBox* Window::createFullSizeComboBox()
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeComboBox";

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".combobox";

		Ogre::String highlightMaterial = GUIManager::getSingleton().getDefaultSkin() + ".listitem.highlight";

		return _createComboBox(name,Ogre::Vector4(0,0,1,1),material,highlightMaterial);
	}

	Image* Window::createFullSizeImage(const Ogre::String& material, bool texture)
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeImage";		

		return _createImage(name,Ogre::Vector4(0,0,1,1),material,texture);
	}

	Image* Window::createFullSizeImage(bool texture)
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeImage";	

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".image";

		return _createImage(name,Ogre::Vector4(0,0,1,1),material,texture);
	}

	Label* Window::createFullSizeLabel(const Ogre::String& material)
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeLabel";

		return _createLabel(name,Ogre::Vector4(0,0,1,1),material);		
	}

	Label* Window::createFullSizeLabel()
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeLabel";

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".label";

		return _createLabel(name,Ogre::Vector4(0,0,1,1),material);
	}

	Menu* Window::createFullSizeMenu(const Ogre::String& material)
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeMenu";

		return _createMenu(name,Ogre::Vector4(0,0,1,1),material);
	}

	Menu* Window::createFullSizeMenu()
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeMenu";

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".menu";

		return _createMenu(name,Ogre::Vector4(0,0,1,1),material);
	}

	NStateButton* Window::createFullSizeNStateButton()
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeNStateButton";

		return _createNStateButton(name,Ogre::Vector4(0,0,1,1));
	}

	ProgressBar* Window::createFullSizeProgressBar(const Ogre::String& material)
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeProgressBar";

		return _createProgressBar(name,Ogre::Vector4(0,0,1,1),material);
	}

	ProgressBar* Window::createFullSizeProgressBar()
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeProgressBar";

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".progressbar";

		return _createProgressBar(name,Ogre::Vector4(0,0,1,1),material);
	}

	TextBox* Window::createFullSizeTextBox(const Ogre::String& material)
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeTextBox";

		return _createTextBox(name,Ogre::Vector4(0,0,1,1),material);
	}

	TextBox* Window::createFullSizeTextBox()
	{
		if( mWidgetType == QGUI_TYPE_WINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_FullSizeTextBox";

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".textbox";

		return _createTextBox(name,Ogre::Vector4(0,0,1,1),material);
	}

	Image* Window::createImage(const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createImage(name,dimensions,material,texture);
	}

	Image* Window::createImage(const Ogre::Vector4& dimensions, bool texture)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".image";

		return _createImage(name,dimensions,material,texture);
	}

	Label* Window::createLabel(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createLabel(name,dimensions,material);
	}
	
	Label* Window::createLabel(const Ogre::Vector4& dimensions)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".label";

		return _createLabel(name,dimensions,material);
	}

	Menu* Window::createMenu(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createMenu(name,dimensions,material);
	}

	Menu* Window::createMenu(const Ogre::Vector4& dimensions)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".menu";

		return _createMenu(name,dimensions,material);
	}

	NStateButton* Window::createNStateButton(const Ogre::Vector4& dimensions)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createNStateButton(name,dimensions);
	}

	ProgressBar* Window::createProgressBar(const Ogre::Vector4& dimensions,  const Ogre::String& material)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createProgressBar(name,dimensions,material);
	}

	ProgressBar* Window::createProgressBar(const Ogre::Vector4& dimensions)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".progressbar";

		return _createProgressBar(name,dimensions,material);
	}

	TextBox* Window::createTextBox(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createTextBox(name,dimensions,material);
	}

	TextBox* Window::createTextBox(const Ogre::Vector4& dimensions)
	{
		if( mWidgetType == QGUI_TYPE_PLAINWINDOW ) return NULL;

		Ogre::String name = mInstanceName + "_ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = GUIManager::getSingleton().getDefaultSkin() + ".textbox";

		return _createTextBox(name,dimensions,material);
	}

	void Window::destroyButton(unsigned int index)
	{
		if( index >= mNumButtons ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_BUTTON,index))
			--mNumButtons;
	}

	void Window::destroyButton(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumButtons;
	}

	void Window::destroyButton(Button* b)
	{
		if(_destroyWidget(b->getInstanceName()))
			--mNumButtons;
	}

	void Window::destroyComboBox(unsigned int index)
	{
		if( index >= mNumComboBoxes ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_COMBOBOX,index))
			--mNumComboBoxes;
	}

	void Window::destroyComboBox(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumComboBoxes;;
	}

	void Window::destroyComboBox(ComboBox* b)
	{
		if(_destroyWidget(b->getInstanceName()))
			--mNumComboBoxes;;
	}

	void Window::destroyImage(unsigned int index)
	{
		if( index >= mNumImages ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_IMAGE,index))
			--mNumImages;
	}

	void Window::destroyImage(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumImages;
	}

	void Window::destroyImage(Image* i)
	{
		if(_destroyWidget(i->getInstanceName()))
			--mNumImages;
	}

	void Window::destroyLabel(unsigned int index)
	{
		if( index >= mNumLabels ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_LABEL,index))
			--mNumLabels;
	}

	void Window::destroyLabel(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumLabels;
	}

	void Window::destroyLabel(Label* l)
	{
		if(_destroyWidget(l->getInstanceName()))
			--mNumLabels;
	}

	void Window::destroyMenu(unsigned int index)
	{
		if( index >= mNumMenus ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_MENU,index))
			--mNumMenus;
	}

	void Window::destroyMenu(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumMenus;
	}

	void Window::destroyMenu(Menu* m)
	{
		if(_destroyWidget(m->getInstanceName()))
			--mNumMenus;
	}

	void Window::destroyNStateButton(unsigned int index)
	{
		if( index >= mNumNStateButtons ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_NSTATEBUTTON,index))
			--mNumNStateButtons;
	}

	void Window::destroyNStateButton(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumNStateButtons;
	}

	void Window::destroyNStateButton(NStateButton* b)
	{
		if(_destroyWidget(b->getInstanceName()))
			--mNumNStateButtons;
	}

	void Window::destroyProgressBar(unsigned int index)
	{
		if( index >= mNumProgressBars ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_PROGRESSBAR,index))
			--mNumProgressBars;
	}

	void Window::destroyProgressBar(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumProgressBars;
	}

	void Window::destroyProgressBar(ProgressBar* b)
	{
		if(_destroyWidget(b->getInstanceName()))
			--mNumProgressBars;
	}

	void Window::destroyTextBox(unsigned int index)
	{
		if( index >= mNumTextBoxes ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_TEXTBOX,index))
			--mNumTextBoxes;
	}

	void Window::destroyTextBox(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumTextBoxes;
	}

	void Window::destroyTextBox(TextBox* b)
	{
		if(_destroyWidget(b->getInstanceName()))
			--mNumTextBoxes;
	}

	void Window::deactivate(EventArgs& e)
	{
		// Assign active material, if exists

		// execute any user defined event handler callbacks
		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[QGUI_EVENT_ACTIVATED];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);
		
		// Fire activate event for all child widgets
		if(mTitleBar) mTitleBar->deactivate(e);

		std::vector<Widget*>::iterator wIt;
		for( wIt = mChildWidgets.begin(); wIt != mChildWidgets.end(); ++wIt )
			(*wIt)->deactivate(e);
	}

	Button* Window::getButton(unsigned int index)
	{
		if( index >= mNumButtons ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_BUTTON,index);
		if( w != NULL ) return dynamic_cast<Button*>(w);
		else return NULL;
	}

	Button* Window::getButton(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Button*>(w);
		else return NULL;
	}

	ComboBox* Window::getComboBox(unsigned int index)
	{
		if( index >= mNumComboBoxes ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_COMBOBOX,index);
		if( w != NULL ) return dynamic_cast<ComboBox*>(w);
		else return NULL;
	}

	ComboBox* Window::getComboBox(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<ComboBox*>(w);
		else return NULL;
	}

	Image* Window::getImage(unsigned int index)
	{
		if( index >= mNumImages ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_IMAGE,index);
		if( w != NULL ) return dynamic_cast<Image*>(w);
		else return NULL;
	}

	Image* Window::getImage(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Image*>(w);
		else return NULL;
	}

	Label* Window::getLabel(unsigned int index)
	{
		if( index >= mNumLabels ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_LABEL,index);
		if( w != NULL ) return dynamic_cast<Label*>(w);
		else return NULL;
	}

	Label* Window::getLabel(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Label*>(w);
		else return NULL;
	}

	Menu* Window::getMenu( unsigned int index )
	{
		if( index >= mNumMenus ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_MENU,index);
		if( w != NULL ) return dynamic_cast<Menu*>(w);
		else return NULL;
	}

	Menu* Window::getMenu(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Menu*>(w);
		else return NULL;
	}

	NStateButton* Window::getNStateButton(unsigned int index)
	{
		if( index >= mNumNStateButtons ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_NSTATEBUTTON,index);
		if( w != NULL ) return dynamic_cast<NStateButton*>(w);
		else return NULL;
	}
		
	NStateButton* Window::getNStateButton(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<NStateButton*>(w);
		else return NULL;
	}

	Widget* Window::getTargetWidget(const Ogre::Vector2& p)
	{
		if(!mVisible) return NULL;

		if( mTitleBar && (mTitleBar->getTargetWidget(p)) ) return mTitleBar->getTargetWidget(p);

		Widget* w = NULL;

		// Get the widget with the highest zOrder
		unsigned int widgetZOrder = 0;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( ((*it)->getTargetWidget(p)) && 
				((*it)->getTargetWidget(p)->getDerivedZOrder() > widgetZOrder) )
			{
				w = (*it)->getTargetWidget(p);
				widgetZOrder = (*it)->getDerivedZOrder();
			}
		}

		if( (w == NULL) && isPointWithinBounds(p)) return this;
		else return w;
	}

	TextBox* Window::getTextBox(unsigned int index)
	{
		if( index >= mNumTextBoxes ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_TEXTBOX,index);
		if( w != NULL ) return dynamic_cast<TextBox*>(w);
		else return NULL;
	}

	TextBox* Window::getTextBox(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<TextBox*>(w);
		else return NULL;
	}

	TitleBar* Window::getTitleBar()
	{
		return mTitleBar;
	}

	Ogre::ushort Window::getZOrder()
	{
		return mZOrder;
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

	void Window::hideCloseButton()
	{
		if(mTitleBar) mTitleBar->hideCloseButton();
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

	void Window::setMaterial(const Ogre::String& material)
	{
		mWidgetMaterial = material;
		mWindowOverlayElement->setMaterialName(mWidgetMaterial);
	}

	void Window::setPosition(const Ogre::Real& xRelativePosition, const Ogre::Real& yRelativePosition)
	{
		if(!mMovingEnabled) return;

		mRelativeDimensions.x = xRelativePosition;
		mRelativeDimensions.y = yRelativePosition;
		_notifyDimensionsChanged();
	}

	void Window::setText(const Ogre::UTFString& text)
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
		mWindowOverlayElement->setTiling(horizontalRepetitions,verticalRepetitions,layer);
	}

	void Window::setTitleBarHeight(Ogre::Real height)
	{
		if( height > mWindowOverlayElement->getHeight() ) height = mWindowOverlayElement->getHeight();

		if(mTitleBar) mTitleBar->setHeight(height);
	}

	void Window::setZOrder(const Ogre::ushort& zOrder)
	{
		mZOrder = zOrder;
		mWindowOverlay->setZOrder(mZOrder);

		if(mTitleBar) mTitleBar->_notifyZOrder(mZOrder);

		// Update all child windows
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->_notifyZOrder(mZOrder);
	}

	void Window::show()
	{
		mWindowOverlay->show();
		Widget::show();
	}

	void Window::showCloseButton()
	{
		if(mTitleBar) mTitleBar->showCloseButton();
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
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->timeElapsed(time);

		if(mTitleBar) mTitleBar->timeElapsed(time);
	}
}
