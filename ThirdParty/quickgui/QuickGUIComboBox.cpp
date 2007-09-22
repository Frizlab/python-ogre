#include "QuickGUIComboBox.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	ComboBox::ComboBox(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Label(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_COMBOBOX)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
		}

		addEventHandler(EVENT_LOSE_FOCUS,&ComboBox::onLoseFocus,this);
		addEventHandler(EVENT_MOUSE_ENTER,&ComboBox::onMouseEnters,this);
		addEventHandler(EVENT_MOUSE_LEAVE,&ComboBox::onMouseLeaves,this);
		addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&ComboBox::onMouseButtonDown,this);
		addEventHandler(EVENT_MOUSE_BUTTON_UP,&ComboBox::onMouseButtonUp,this);

		// Create CloseButton - remember to position it relative to it's parent (TitleBar)
		// Height of the Title Bar
		Ogre::Real height = (mAbsoluteDimensions.height / mAbsoluteDimensions.width);
		// Button has same height as width - Make the button slightly smaller that the titlebar height
		Ogre::Real buttonHeight = 0.8;
		Ogre::Real buttonWidth = (height * buttonHeight);
		// Make a 5 pixel buffer
		Ogre::Real buffer = 5.0 / mPixelDimensions.width;
		Rect bDimensions = Rect((1 - (buttonWidth + buffer)),0.1,buttonWidth,buttonHeight);
		mButton = new Button(mInstanceName+".ComboButton",TYPE_BUTTON,bDimensions,QGUI_GMM_RELATIVE,QGUI_GMM_RELATIVE,mTextureName + ".button" + mTextureExtension,mQuadContainer,this,mGUIManager);
		mButton->addEventHandler(Widget::EVENT_MOUSE_BUTTON_DOWN,&ComboBox::toggleDropDownListVisibility,this);
		mButton->addEventHandler(Widget::EVENT_MOUSE_BUTTON_UP,&ComboBox::applyButtonDownImage,this);
		mButton->addEventHandler(Widget::EVENT_MOUSE_ENTER,&ComboBox::applyButtonDownImage,this);
		mButton->addEventHandler(Widget::EVENT_MOUSE_LEAVE,&ComboBox::applyButtonDownImage,this);

		mTextBoundsRelativeSize = Size(1 - buttonWidth,1);

		mList = new List(mInstanceName+".List",TYPE_LIST,Rect(0,1,1,0),QGUI_GMM_RELATIVE,QGUI_GMM_RELATIVE,mTextureName + ".list" + mTextureExtension,mQuadContainer,this,mGUIManager);
		mList->setShowWithParent(false);
		mList->setOffset(mOffset + 2);
		mList->hide();
		mList->addEventHandler(EVENT_CHILD_ADDED,&ComboBox::addDefaultListItemHandler,this);
	}

	ComboBox::~ComboBox()
	{
		Widget::removeAndDestroyAllChildWidgets();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnSelectUserEventHandlers.begin(); it != mOnSelectUserEventHandlers.end(); ++it )
			delete (*it);
		mOnSelectUserEventHandlers.clear();
	}

	void ComboBox::addDefaultListItemHandler(const EventArgs& args)
	{
		dynamic_cast<const WidgetEventArgs&>(args).widget->addEventHandler(Widget::EVENT_MOUSE_BUTTON_UP,&ComboBox::onSelection,this);
		dynamic_cast<const WidgetEventArgs&>(args).widget->addEventHandler(Widget::EVENT_LOSE_FOCUS,&ComboBox::onLoseFocus,this);
	}

	void ComboBox::addOnSelectionEventHandler(MemberFunctionSlot* function)
	{
		mOnSelectUserEventHandlers.push_back(function);
	}

	void ComboBox::applyButtonDownImage(const EventArgs& args)
	{
		if(mList->isVisible())
			mButton->applyButtonDownTexture();
	}

	// Called when the user clicks outside the widget
	void ComboBox::onLoseFocus(const EventArgs& args)
	{
		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);

		// Drop Down List children call this function when they lose focus, so this check is needed because you can enter this
		// function when you haven't actually clicked outside the combobox.
		if(isPointWithinBounds(mea.position))
			return;

		// if user clicked on a child of this comboBox (ie ListItem), do not hide Drop Down List.
		if(getTargetWidget(mea.position) != NULL)
			return;

		mList->hide();
	}

	List* ComboBox::getDropDownList()
	{
		return mList;
	}

	void ComboBox::onMouseEnters(const EventArgs& args)
	{
		if(!mList->isVisible())
			setTexture(mDefaultTexture + ".over" + mTextureExtension);
		if(mList->isVisible()) 
			mButton->applyButtonDownTexture();
	}

	void ComboBox::onMouseLeaves(const EventArgs& args)
	{
		if(!mList->isVisible()) 
			setTexture(mDefaultTexture + mTextureExtension);
	}

	void ComboBox::onMouseButtonDown(const EventArgs& args)
	{
		if( dynamic_cast<const MouseEventArgs&>(args).button == MB_Left )
			toggleDropDownListVisibility(args);
	}

	void ComboBox::onMouseButtonUp(const EventArgs& args)
	{
		if(mList->isVisible()) 
			mButton->applyButtonDownTexture();
	}

	void ComboBox::onSelection(const EventArgs& args)
	{
		mText->setCaption(dynamic_cast<ListItem*>(dynamic_cast<const WidgetEventArgs&>(args).widget)->getText()->getCaption());
		setTexture(mDefaultTexture + mTextureExtension);
		mButton->applyDefaultTexture();
		mList->hide();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnSelectUserEventHandlers.begin(); it != mOnSelectUserEventHandlers.end(); ++it )
			(*it)->execute(args);
	}

	void ComboBox::toggleDropDownListVisibility(const EventArgs& e)
	{
		if(!mList->isVisible())
		{
			// apply button ".down" material
			setTexture(mDefaultTexture + ".down" + mTextureExtension);
			mButton->applyButtonDownTexture();
			mList->show();
		}
		else
		{
			setTexture(mDefaultTexture + ".over" + mTextureExtension);
			mButton->applyDefaultTexture();
			mList->hide();
		}
	}
}
