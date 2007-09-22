#include "QuickGUIHorizontalTrackBar.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	HorizontalTrackBar::HorizontalTrackBar(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm),
		mNumRegions(1),
		mCurrentValue(0),
		mMouseDownOnSlider(false),
		mLargeChange(3)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_TRACKBAR_HORIZONTAL)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
		}

		addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalTrackBar::onMouseButtonDown,this);

		// Creat slider button at the beginning of the HorizontalTrackBar, whether horizonal (left) or vertical (bot)
		mSliderButton = new Button(mInstanceName+".SliderButton",TYPE_BUTTON,Rect(0,0,13,mPixelDimensions.height),QGUI_GMM_RELATIVE,QGUI_GMM_PIXELS,mSliderTextureName,mQuadContainer,this,mGUIManager);
		mSliderButton->enableDragging(true);
		mSliderButton->constrainDragging(true,false);
		mSliderButton->addEventHandler(EVENT_DRAGGED,&HorizontalTrackBar::onSliderDragged,this);
		mSliderButton->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalTrackBar::onMouseDownOnSlider,this);
		mSliderButton->addEventHandler(EVENT_MOUSE_BUTTON_UP,&HorizontalTrackBar::onMouseUpOnSlider,this);
		mSliderButton->addEventHandler(EVENT_LOSE_FOCUS,&HorizontalTrackBar::onMouseUpOnSlider,this);

		mRelativeSliderSize = mSliderButton->getSize();
		
		_getSliderPositions();

		setTexture(mFullTextureName,true);
		setValue(0);

		mOnValueChangedHandlers.clear();
	}

	HorizontalTrackBar::~HorizontalTrackBar()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mSliderButton = NULL;

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnValueChangedHandlers.begin(); it != mOnValueChangedHandlers.end(); ++it )
			delete (*it);
		mOnValueChangedHandlers.clear();
	}

	void HorizontalTrackBar::_getSliderPositions()
	{
		mSliderPositions.clear();
		
		Ogre::Real trackLength = 1 - mRelativeSliderSize.width;
		mRegionLength = trackLength / mNumRegions;

		Ogre::Real xStart = (mRelativeSliderSize.width/2.0);
		mSliderPositions.push_back(xStart);
		for( int i = 0; i < mNumRegions; ++i )
		{
			mSliderPositions.push_back(xStart + ((i + 1) * mRegionLength));
		}
	}

	void HorizontalTrackBar::addOnValueChangedEventHandler(MemberFunctionSlot* function)
	{
		mOnValueChangedHandlers.push_back(function);
	}

	int HorizontalTrackBar::getClosestSliderPosition(const Point& pixelPoint)
	{
		Point p = pixelPoint;
		p.x = (p.x - mPixelDimensions.x) / mPixelDimensions.width;
		p.y = (p.y - mPixelDimensions.y) / mPixelDimensions.height;

		if( p.x <= mSliderPositions[0] )
			return 0;
		if( p.x >= mSliderPositions[mSliderPositions.size() - 1] )
			return (static_cast<int>(mSliderPositions.size()) - 1);

		int i;
		for(  i = 1; i < static_cast<int>(mSliderPositions.size()); ++i )
		{
			if( p.x < (mSliderPositions[i] - (mRegionLength/2.0)) )
				return (i - 1);
		}

		return i;
	}

	int HorizontalTrackBar::getLargeChange()
	{
		return mLargeChange;
	}

	int HorizontalTrackBar::getNumRegions()
	{
		return mNumRegions;
	}
	
	int HorizontalTrackBar::getNumTicks()
	{
		return (mNumRegions + 1);
	}

	int HorizontalTrackBar::getValue()
	{
		return mCurrentValue;
	}

	Ogre::Real HorizontalTrackBar::getPercentage()
	{
		return static_cast<Ogre::Real>(mCurrentValue) / static_cast<Ogre::Real>(mNumRegions);
	}

	void HorizontalTrackBar::onMouseDownOnSlider(const EventArgs& args)
	{
		mMouseDownOnSlider = true;
		
		mSliderButton->lockTexture();
	}

	void HorizontalTrackBar::onMouseUpOnSlider(const EventArgs& args)
	{
		mMouseDownOnSlider = false;
		mSliderButton->unlockTexture();

		if(mGUIManager->getMouseOverWidget()->getInstanceName() != mSliderButton->getInstanceName())
			mSliderButton->applyDefaultTexture();
		else
			mSliderButton->applyButtonOverTexture();
	}

	void HorizontalTrackBar::onMouseButtonDown(const EventArgs& args)
	{
		Point mousePos = dynamic_cast<const MouseEventArgs&>(args).position;
		mousePos.x = (mousePos.x - mPixelDimensions.x) / mPixelDimensions.width;
		mousePos.y = (mousePos.y - mPixelDimensions.y) / mPixelDimensions.height;

		if( (mousePos.x > (mSliderPositions[mCurrentValue] + ((mRegionLength + mRelativeSliderSize.width)/2.0))) && (mCurrentValue < mNumRegions) )
			setValue(mCurrentValue + mLargeChange);
		else if( (mousePos.x < (mSliderPositions[mCurrentValue] - ((mRegionLength - mRelativeSliderSize.width)/2.0))) && (mCurrentValue > 0) )
			setValue(mCurrentValue - mLargeChange);
	}

	void HorizontalTrackBar::onSliderDragged(const EventArgs& args)
	{
		int indexFromCursor = getClosestSliderPosition(mGUIManager->getMouseCursor()->getPosition());
		setValue(indexFromCursor);
	}

	void HorizontalTrackBar::onValueChanged(const WidgetEventArgs& args)
	{
		if(!mEnabled)
			return;

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnValueChangedHandlers.begin(); it != mOnValueChangedHandlers.end(); ++it )
			(*it)->execute(args);
	}

	void HorizontalTrackBar::setBaseTexture(const Ogre::String& textureName)
	{
		Image::setBaseTexture(textureName);

		mSliderTextureName = mTextureName + ".slider" + mTextureExtension;
	}

	void HorizontalTrackBar::setLargeChange(unsigned int LargeChange)
	{
		mLargeChange = LargeChange;
	}

	void HorizontalTrackBar::setNumRegions(unsigned int NumRegions)
	{
		mNumRegions = NumRegions;
		_getSliderPositions();
	}

	void HorizontalTrackBar::setNumTicks(unsigned int NumTicks)
	{
		setNumRegions(NumTicks - 1);
	}

	void HorizontalTrackBar::setSliderSize(Size s, GuiMetricsMode mode)
	{
		mSliderButton->setSize(s,mode);
		mRelativeSliderSize = mSliderButton->getSize();
	}

	void HorizontalTrackBar::setTexture(const Ogre::String& textureName, bool updateBaseTexture)
	{
		Image::setTexture(textureName,updateBaseTexture);

		mSliderButton->setTexture(mSliderTextureName);
	}

	void HorizontalTrackBar::setValue(int Value)
	{
		if( Value < 0 )
			Value = 0;
		else if( Value > mNumRegions )
			Value = mNumRegions;

		mCurrentValue = Value;

		mSliderButton->setXPosition(mSliderPositions[mCurrentValue] - (mRelativeSliderSize.width/2.0));

		WidgetEventArgs e(this);
		e.handled = false;
		onValueChanged(e);
	}
}
