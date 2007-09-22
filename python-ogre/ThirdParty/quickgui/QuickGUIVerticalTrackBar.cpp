#include "QuickGUIVerticalTrackBar.h"
#include "QuickGUIManager.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	VerticalTrackBar::VerticalTrackBar(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm),
		mNumRegions(1),
		mCurrentValue(0),
		mMouseDownOnSlider(false),
		mLargeChange(3)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_TRACKBAR_VERTICAL)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
		}

		addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&VerticalTrackBar::onMouseButtonDown,this);

		// Creat slider button at the beginning of the VerticalTrackBar, whether horizonal (left) or vertical (bot)
		mSliderButton = new Button(mInstanceName+".SliderButton",TYPE_BUTTON,Rect(0,0,mPixelDimensions.width,13),QGUI_GMM_RELATIVE,QGUI_GMM_PIXELS,mSliderTextureName,mQuadContainer,this,mGUIManager);
		mSliderButton->enableDragging(true);
		mSliderButton->constrainDragging(false,true);
		mSliderButton->addEventHandler(EVENT_DRAGGED,&VerticalTrackBar::onSliderDragged,this);
		mSliderButton->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&VerticalTrackBar::onMouseDownOnSlider,this);
		mSliderButton->addEventHandler(EVENT_MOUSE_BUTTON_UP,&VerticalTrackBar::onMouseUpOnSlider,this);
		mSliderButton->addEventHandler(EVENT_LOSE_FOCUS,&VerticalTrackBar::onMouseUpOnSlider,this);

		mRelativeSliderSize = mSliderButton->getSize();
		
		_getSliderPositions();

		setTexture(mFullTextureName,true);
		setValue(0);

		mOnValueChangedHandlers.clear();
	}

	VerticalTrackBar::~VerticalTrackBar()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mSliderButton = NULL;

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnValueChangedHandlers.begin(); it != mOnValueChangedHandlers.end(); ++it )
			delete (*it);
		mOnValueChangedHandlers.clear();
	}

	void VerticalTrackBar::_getSliderPositions()
	{
		mSliderPositions.clear();

		Ogre::Real trackLength = 1 - mRelativeSliderSize.height;
		mRegionLength = trackLength / mNumRegions;

		Ogre::Real yStart = 1 - (mRelativeSliderSize.height/2.0);
		mSliderPositions.push_back(yStart);
		for( int i = 0; i < mNumRegions; ++i )
		{
			mSliderPositions.push_back(yStart - ((i + 1) * mRegionLength));
		}
	}

	void VerticalTrackBar::addOnValueChangedEventHandler(MemberFunctionSlot* function)
	{
		mOnValueChangedHandlers.push_back(function);
	}

	int VerticalTrackBar::getClosestSliderPosition(const Point& pixelPoint)
	{
		Point p = pixelPoint;
		p.x = (p.x - mPixelDimensions.x) / mPixelDimensions.width;
		p.y = (p.y - mPixelDimensions.y) / mPixelDimensions.height;

		if( p.y >= mSliderPositions[0] )
			return 0;
		if( p.y <= mSliderPositions[mSliderPositions.size() - 1] )
			return (static_cast<int>(mSliderPositions.size()) - 1);

		int i;
		for( i = 1; i < static_cast<int>(mSliderPositions.size()); ++i )
		{
			if( p.y > (mSliderPositions[i] + (mRegionLength/2.0)) )
				return (i - 1);
		}

		return i;
	}

	int VerticalTrackBar::getLargeChange()
	{
		return mLargeChange;
	}

	int VerticalTrackBar::getNumRegions()
	{
		return mNumRegions;
	}
	
	int VerticalTrackBar::getNumTicks()
	{
		return (mNumRegions + 1);
	}

	int VerticalTrackBar::getValue()
	{
		return mCurrentValue;
	}

	Ogre::Real VerticalTrackBar::getPercentage()
	{
		return static_cast<Ogre::Real>(mCurrentValue) / static_cast<Ogre::Real>(mNumRegions);
	}

	void VerticalTrackBar::onMouseDownOnSlider(const EventArgs& args)
	{
		mMouseDownOnSlider = true;
		mSliderButton->lockTexture();
	}

	void VerticalTrackBar::onMouseUpOnSlider(const EventArgs& args)
	{
		mMouseDownOnSlider = false;
		mSliderButton->unlockTexture();

		if(mGUIManager->getMouseOverWidget()->getInstanceName() != mSliderButton->getInstanceName())
			mSliderButton->applyDefaultTexture();
		else
			mSliderButton->applyButtonOverTexture();
	}

	void VerticalTrackBar::onMouseButtonDown(const EventArgs& args)
	{
		Point mousePos = dynamic_cast<const MouseEventArgs&>(args).position;
		mousePos.x = (mousePos.x - mPixelDimensions.x) / mPixelDimensions.width;
		mousePos.y = (mousePos.y - mPixelDimensions.y) / mPixelDimensions.height;

		if( (mousePos.y < (mSliderPositions[mCurrentValue] - ((mRegionLength - mRelativeSliderSize.height)/2.0))) && (mCurrentValue < mNumRegions) )
			setValue(mCurrentValue + mLargeChange);
		else if( (mousePos.y > (mSliderPositions[mCurrentValue] + ((mRegionLength + mRelativeSliderSize.height)/2.0))) && (mCurrentValue > 0) )
			setValue(mCurrentValue - mLargeChange);
	}

	void VerticalTrackBar::onSliderDragged(const EventArgs& args)
	{
		int indexFromCursor = getClosestSliderPosition(mGUIManager->getMouseCursor()->getPosition());
		setValue(indexFromCursor);
	}

	void VerticalTrackBar::onValueChanged(const WidgetEventArgs& args)
	{
		if(!mEnabled)
			return;

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnValueChangedHandlers.begin(); it != mOnValueChangedHandlers.end(); ++it )
			(*it)->execute(args);
	}

	void VerticalTrackBar::setBaseTexture(const Ogre::String& textureName)
	{
		Image::setBaseTexture(textureName);

		mSliderTextureName = mTextureName + ".slider" + mTextureExtension;
	}

	void VerticalTrackBar::setLargeChange(unsigned int jumpValue)
	{
		mLargeChange = jumpValue;
	}

	void VerticalTrackBar::setNumRegions(unsigned int NumRegions)
	{
		mNumRegions = NumRegions;
		_getSliderPositions();
	}

	void VerticalTrackBar::setSliderSize(Size s, GuiMetricsMode mode)
	{
		mSliderButton->setSize(s,mode);
		mRelativeSliderSize = mSliderButton->getSize();
	}

	void VerticalTrackBar::setNumTicks(unsigned int NumTicks)
	{
		setNumRegions(NumTicks - 1);
	}

	void VerticalTrackBar::setTexture(const Ogre::String& textureName, bool updateBaseTexture)
	{
		Image::setTexture(textureName,updateBaseTexture);

		mSliderButton->setTexture(mSliderTextureName);
	}

	void VerticalTrackBar::setValue(int Value)
	{
		if( Value < 0 )
			Value = 0;
		else if( Value > mNumRegions )
			Value = mNumRegions;

		mCurrentValue = Value;

		mSliderButton->setYPosition(mSliderPositions[mCurrentValue] - (mRelativeSliderSize.height/2.0));

		WidgetEventArgs e(this);
		e.handled = false;
		onValueChanged(e);
	}
}
