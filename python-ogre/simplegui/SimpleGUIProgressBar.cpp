#include "SimpleGUIProgressBar.h"
#include "SimpleGUIWindow.h"

namespace SimpleGUI
{
	ProgressBar::ProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,dimensions,overlayContainer,ParentWidget),
		mMaterial(material)
	{
		mWidgetType = Widget::SGUI_PROGRESSBAR;

		// Border Overlay gives us ability to assign material to TextBox border and Panel separately.
		mBackgroundOverlayElement = createPanelOverlayElement(mInstanceName+"_Background",mPixelDimensions,mMaterial);
		mOverlayContainer->addChild(mBackgroundOverlayElement);
		mBackgroundOverlayElement->show();

		mBarOverlayElement = createPanelOverlayElement(mInstanceName+"_Bar",mPixelDimensions,mMaterial+".bar");
		mOverlayContainer->addChild(mBarOverlayElement);
		mBarOverlayElement->show();

		if( show ) this->show();
		else hide();
	}

	ProgressBar::~ProgressBar()
	{
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnProgressChangedHandlers.begin(); it != mOnProgressChangedHandlers.end(); ++it )
			delete (*it);
		mOnProgressChangedHandlers.clear();

		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		// destroy bar overlay element
		mOverlayContainer->removeChild(mBarOverlayElement->getName());
		om->destroyOverlayElement(mBarOverlayElement);

		// destroy background overlay element
		mOverlayContainer->removeChild(mBackgroundOverlayElement->getName());
		om->destroyOverlayElement(mBackgroundOverlayElement);
	}

	void ProgressBar::_applyDimensions()
	{
		mBackgroundOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mBackgroundOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
		mBarOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mBarOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
	}

	void ProgressBar::_notifyDimensionsChanged()
	{
		_updateDimensions(mRelativeDimensions);
		_applyDimensions();
	}

	Ogre::Real ProgressBar::getProgress()
	{
		return mProgress;
	}

	void ProgressBar::hide()
	{
		mBackgroundOverlayElement->hide();
		mBarOverlayElement->hide();
		
		Widget::hide();
	}

	bool ProgressBar::onProgressChanged(WidgetEventArgs& e)
	{
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnProgressChangedHandlers.begin(); it != mOnProgressChangedHandlers.end(); ++it )
			e.handled = (*it)->execute(e);

		return e.handled;
	}

	void ProgressBar::setMaterial(const Ogre::String& material)
	{
		mMaterial = material;
		mBackgroundOverlayElement->setMaterialName(mMaterial);
		mBarOverlayElement->setMaterialName(mMaterial+".bar");
	}

	void ProgressBar::setProgress(Ogre::Real progress)
	{
		mProgress = progress;
		if(mProgress > 1.0) mProgress = 1.0;
		else if(mProgress < 0.0) mProgress = 0.0;

		// stretch bar image according to progress
		mBarOverlayElement->setWidth(mProgress * mPixelDimensions.z);
		// fire event and call any user defined handlers
		WidgetEventArgs e(this);
		e.handled = false;
		onProgressChanged(e);
	}

	void ProgressBar::show()
	{
		mBackgroundOverlayElement->show();
		mBarOverlayElement->show();
		
		Widget::show();
	}
}