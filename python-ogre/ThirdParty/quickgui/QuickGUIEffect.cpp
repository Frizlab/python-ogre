#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIEffect.h"

namespace QuickGUI
{
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	bool Effect::update(const float timeElapsed)
	{
		mCurrTime += timeElapsed;
		if (mCurrTime >= mStartTime)
		{
			const bool finished = getTimeOrIsFinished();

			if (finished)
			{
				updateValue(float(1.0));
			}
			else
			{
				mWidget->setUnderEffect(true);
				updateValue(getTimeFactor());
			}
			return finished;
		}
		// not yet started.
		return false;
	}
	//------------------------------------------------------------------------------------------------
	float Effect::getTimeFactor() 
	{          
		return (mCurrTime - mStartTime) / mDuration;
	}
	//------------------------------------------------------------------------------------------------
	bool Effect::getTimeOrIsFinished()
	{
		if (mCurrTime - mStartTime > mDuration)
		{
			if (!mRepeat)
			{
				mCurrTime = mStartTime + mDuration;
				return true;
			}
			while (mCurrTime - mStartTime > mDuration)
				mCurrTime -= mDuration;
		}
		return false;
	}
	//------------------------------------------------------------------------------------------------
	float Effect::interpolate(const float start, 
								   const float end, 
								   const float timeFactor) const
	{
		float factor;
		switch(mType)
		{
		case EFFECT_SIN:
			factor = Ogre::Math::Sin(Ogre::Radian(timeFactor * Ogre::Math::TWO_PI));
			break;
		case EFFECT_LINEARX4:
			factor = timeFactor * timeFactor * timeFactor * timeFactor;
			break;
		case EFFECT_LINEARX2:
			factor = timeFactor * timeFactor;
			break;
		case EFFECT_LINEAR:
		default:
			factor = timeFactor;
			break;
		}
		return (factor*end + (1 - factor)*start);
	}
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	void AlphaEffect::updateValue(const float factor)
	{
		//mWidget->setAlpha(interpolate(mStartAlpha, mEndAlpha, factor), true);
	}
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	void SizeEffect::updateValue(const float factor)
	{
		mWidget->setSize(
			Size(interpolate(mStartSize.width, mEndSize.width, factor),
				 interpolate(mStartSize.height, mEndSize.height, factor)
			)
			);
	}
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	void MoveEffect::updateValue(const float factor)
	{
		mWidget->setPosition(
			Point(interpolate(mStartPosition.x, mEndPosition.x, factor),
				  interpolate(mStartPosition.y, mEndPosition.y, factor)
				  )
			);
	}
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	void ActivateEffect::updateValue(const float factor)
	{
		//mWidget->setMaterialBlendFactor(interpolate (0, 1, factor), true);
	} 
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	void OverEffect::updateValue(const float factor)
	{
		//mWidget->setMaterialBlendFactor(interpolate (0, 1, factor), true);
	}
	//------------------------------------------------------------------------------------------------

}
