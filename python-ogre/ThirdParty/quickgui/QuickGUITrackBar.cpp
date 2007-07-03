#include "QuickGUITrackBar.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	TrackBar::TrackBar(const Ogre::String& name, const Ogre::Vector4& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,positionMode,sizeMode,"",overlayContainer,ParentWidget),
		mNumRegions(1),
		mCurrentPos(0)
	{
		mWidgetType = QGUI_TYPE_TRACKBAR;

		// setting up button dimensions
		Ogre::Real bPixelHeight, bPixelWidth;
		if( mRelativeDimensions.z > mRelativeDimensions.w ) 
		{
			mLayout = QGUI_TRACKBAR_HORIZONTAL;
			mWidgetMaterial = material + ".horizontal";
			mOverlayElement->setMaterialName(mWidgetMaterial);

			bPixelHeight = mPixelDimensions.w * 1.1;
			bPixelWidth = mPixelDimensions.z * 0.1;
			mSliderLeft = 0.0;
			mSliderTop = 0.0 - (mRelativeDimensions.w * 0.05);
			mRelSliderHeight = bPixelHeight / mPixelDimensions.w;
			mRelSliderWidth = bPixelWidth / mPixelDimensions.z;
			mRegionLength = ((mPixelDimensions.z - bPixelWidth) / mNumRegions);
		}
		else 
		{
			mLayout = QGUI_TRACKBAR_VERTICAL;
			mWidgetMaterial = material + ".vertical";
			mOverlayElement->setMaterialName(mWidgetMaterial);

			bPixelHeight = mPixelDimensions.w * 0.1;
			bPixelWidth = mPixelDimensions.z * 1.1;
			mSliderLeft = 0.0 - (mRelativeDimensions.z * 0.05);
			mSliderTop = 1.0 - (bPixelHeight / mPixelDimensions.w);
			mRelSliderHeight = bPixelHeight / mPixelDimensions.w;
			mRelSliderWidth = bPixelWidth / mPixelDimensions.z;
			mRegionLength = ((mPixelDimensions.w - bPixelHeight) / mNumRegions);
		}

		// Creat slider button at the beginning of the TrackBar, whether horizonal (left) or vertical (bot)
		mSliderButton = new Button(mInstanceName+".SliderButton",Ogre::Vector4(mSliderLeft,mSliderTop,mRelSliderWidth,mRelSliderHeight),QGUI_GMM_RELATIVE,QGUI_GMM_RELATIVE,mWidgetMaterial+".slider",mChildrenContainer,this);
		mSliderButton->setZOrderOffset(1);
		mSliderButton->enableDragging(true);
		mSliderButton->setDraggingWidget(NULL);
		mSliderButton->addEventHandler(QuickGUI::Widget::QGUI_EVENT_DRAGGED,&TrackBar::slide,this);
		Widget::_addChildWidget(mSliderButton);

		// In pixels
		mPixelInitialSliderLeft = mCurrentSliderPos.x = mSliderButton->getPosition(QGUI_GMM_PIXELS).x;
		mPixelInitialSliderBot = mCurrentSliderPos.y = mSliderButton->getPosition(QGUI_GMM_PIXELS).y;

		mOnValueChangedHandlers.clear();
	}

	TrackBar::~TrackBar()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mSliderButton = NULL;

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnValueChangedHandlers.begin(); it != mOnValueChangedHandlers.end(); ++it )
			delete (*it);
		mOnValueChangedHandlers.clear();
	}

	void TrackBar::addOnValueChangedEventHandler(MemberFunctionSlot* function)
	{
		mOnValueChangedHandlers.push_back(function);
	}

	int TrackBar::getNumRegions()
	{
		return mNumRegions;
	}
	
	int TrackBar::getNumTicks()
	{
		return (mNumRegions + 1);
	}

	int TrackBar::getTickPosition()
	{
		return mCurrentPos;
	}

	Ogre::Real TrackBar::getPercentage()
	{
		return static_cast<Ogre::Real>(mCurrentPos) / static_cast<Ogre::Real>(mNumRegions);
	}

	void TrackBar::hide()
	{
		mSliderButton->hide();
		Label::hide();
	}

	bool TrackBar::onMouseButtonDown(MouseEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		if( e.button == MB_Left )
		{
			if(mLayout == QGUI_TRACKBAR_HORIZONTAL)
			{
				// if mouse cursor simulating dragging more than 50% towards the next right position..
				if(e.position.x > (mCurrentSliderPos.x))
				{
					if(mCurrentPos < mNumRegions)
					{
						// move slider one region length to the right
						mCurrentSliderPos.x += (mRegionLength);
						++mCurrentPos;

						mSliderButton->setPosition(mCurrentSliderPos.x,mCurrentSliderPos.y,QGUI_GMM_PIXELS);
					}
				}
				else if(e.position.x < (mCurrentSliderPos.x))
				{
					if(mCurrentPos > 0)
					{
						// move slider one region length to the right
						mCurrentSliderPos.x -= (mRegionLength);
						--mCurrentPos;

						mSliderButton->setPosition(mCurrentSliderPos.x,mCurrentSliderPos.y,QGUI_GMM_PIXELS);
					}
				}
			}
			else if(mLayout == QGUI_TRACKBAR_VERTICAL)
			{
				// if mouse cursor simulating dragging more than 50% towards the next right position..
				if(e.position.y > (mCurrentSliderPos.y))
				{
					if(mCurrentPos > 0)
					{
						// move slider one region length down
						mCurrentSliderPos.y += (mRegionLength);
						--mCurrentPos;

						mSliderButton->setPosition(mCurrentSliderPos.x,mCurrentSliderPos.y,QGUI_GMM_PIXELS);
					}
				}
				else if(e.position.y < (mCurrentSliderPos.y))
				{
					if(mCurrentPos < mNumRegions)
					{
						// move slider one region length up
						mCurrentSliderPos.y -= (mRegionLength);
						++mCurrentPos;

						mSliderButton->setPosition(mCurrentSliderPos.x,mCurrentSliderPos.y,QGUI_GMM_PIXELS);
					}
				}
			}
		}

		return Label::onMouseButtonDown(e);
	}

	bool TrackBar::onValueChanged(WidgetEventArgs& e)
	{
		if(!mEnabled) return e.handled;

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnValueChangedHandlers.begin(); it != mOnValueChangedHandlers.end(); ++it )
			e.handled = (*it)->execute(e);

		return e.handled;
	}

	void TrackBar::setNumRegions(unsigned int NumRegions)
	{
		mNumRegions = NumRegions;
		if(mLayout == QGUI_TRACKBAR_HORIZONTAL)
		{
			mRegionLength = ((mPixelDimensions.z - mSliderButton->getSize(QGUI_GMM_PIXELS).x) / mNumRegions);
		}
		else if(mLayout == QGUI_TRACKBAR_VERTICAL)
		{
			mRegionLength = ((mPixelDimensions.w - mSliderButton->getSize(QGUI_GMM_PIXELS).y) / mNumRegions);
		}
	}

	void TrackBar::setNumTicks(unsigned int NumTicks)
	{
		mNumRegions = (NumTicks - 1);
		if(mLayout == QGUI_TRACKBAR_HORIZONTAL)
		{
			mRegionLength = ((mPixelDimensions.z - mSliderButton->getSize(QGUI_GMM_PIXELS).x) / mNumRegions);
		}
		else if(mLayout == QGUI_TRACKBAR_VERTICAL)
		{
			mRegionLength = ((mPixelDimensions.w - mSliderButton->getSize(QGUI_GMM_PIXELS).y) / mNumRegions);
		}
	}

	void TrackBar::setTickPosition(unsigned int Tick)
	{
		if( (Tick < 0) || (static_cast<int>(Tick) > mNumRegions) ) return;

		mCurrentPos = Tick;

		if(mLayout == QGUI_TRACKBAR_HORIZONTAL)
		{
			mCurrentSliderPos.x = mPixelInitialSliderLeft + (mCurrentPos * mRegionLength);
		}
		else if(mLayout == QGUI_TRACKBAR_VERTICAL)
		{
			mCurrentSliderPos.y = mPixelInitialSliderBot - (mCurrentPos * mRegionLength);
		}
		mSliderButton->setPosition(mCurrentSliderPos.x,mCurrentSliderPos.y,QGUI_GMM_PIXELS);
	}

	void TrackBar::show()
	{
		mSliderButton->show();
		Label::show();
	}

	bool TrackBar::slide(const EventArgs& e)
	{
		if(!mEnabled) return e.handled;

		// determine if slider was moved toward or away from current position
		Ogre::Vector2 newPos = MouseCursor::getSingleton().getPixelPosition();
		if(mLayout == QGUI_TRACKBAR_HORIZONTAL)
		{
			// if mouse cursor simulating dragging more than 50% towards the next right position..
			if(newPos.x > (mCurrentSliderPos.x + (mRegionLength / 2)))
			{
				if(mCurrentPos < mNumRegions)
				{
					// move slider one region length to the right
					mCurrentSliderPos.x += (mRegionLength);
					++mCurrentPos;

					mSliderButton->setPosition(mCurrentSliderPos.x,mCurrentSliderPos.y,QGUI_GMM_PIXELS);
				}
			}
			else if(newPos.x < (mCurrentSliderPos.x - (mRegionLength / 2)))
			{
				if(mCurrentPos > 0)
				{
					// move slider one region length to the right
					mCurrentSliderPos.x -= (mRegionLength);
					--mCurrentPos;

					mSliderButton->setPosition(mCurrentSliderPos.x,mCurrentSliderPos.y,QGUI_GMM_PIXELS);
				}
			}
		}
		else if(mLayout == QGUI_TRACKBAR_VERTICAL)
		{
			// if mouse cursor simulating dragging more than 50% towards the next right position..
			if(newPos.y > (mCurrentSliderPos.y + (mRegionLength / 2)))
			{
				if(mCurrentPos > 0)
				{
					// move slider one region length down
					mCurrentSliderPos.y += (mRegionLength);
					--mCurrentPos;

					mSliderButton->setPosition(mCurrentSliderPos.x,mCurrentSliderPos.y,QGUI_GMM_PIXELS);
				}
			}
			else if(newPos.y < (mCurrentSliderPos.y - (mRegionLength / 2)))
			{
				if(mCurrentPos < mNumRegions)
				{
					// move slider one region length up
					mCurrentSliderPos.y -= (mRegionLength);
					++mCurrentPos;

					mSliderButton->setPosition(mCurrentSliderPos.x,mCurrentSliderPos.y,QGUI_GMM_PIXELS);
				}
			}
		}

		return true;
	}
}
