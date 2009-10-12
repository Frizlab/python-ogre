#include "QuickGUIImage.h"
#include "QuickGUISkinDefinitionManager.h"
#include "OgreTextureManager.h"
#include "QuickGUISkinDefinition.h"

namespace QuickGUI
{
	const Ogre::String Image::BACKGROUND = "background";

	void Image::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Image");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Image",d);
	}

	ImageDesc::ImageDesc() :
		WidgetDesc()
	{
		resetToDefault();
	}

	void ImageDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		widget_dimensions.size = Size::ZERO;
		widget_transparencyPicking = false;

		image_drawBackgroundLast = false;
		image_imageName = "";
		image_rotationInDegrees = 0;
		image_tileImage = false;
		image_updateEveryFrame = false;
	}

	void ImageDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("DrawBackgroundLast",&image_drawBackgroundLast);
		b->IO("ImageName",&image_imageName);
		b->IO("RotationInDegrees",&image_rotationInDegrees);
		b->IO("TileImage",&image_tileImage);
		b->IO("UpdateEveryFrame",&image_updateEveryFrame);
	}

	Image::Image(const Ogre::String& name) :
		Widget(name),
		mUpdateTimer(NULL)
	{
	}

	Image::~Image()
	{
		if(mUpdateTimer != NULL)
			TimerManager::getSingleton().destroyTimer(mUpdateTimer);
	}

	void Image::_initialize(WidgetDesc* d)
	{
		d->widget_transparencyPicking = false;

		// Record the size initially passed in.
		Size sizeFromDesc = d->widget_dimensions.size;

		Widget::_initialize(d);

		mDesc = dynamic_cast<ImageDesc*>(mWidgetDesc);

		ImageDesc* id = dynamic_cast<ImageDesc*>(d);

		setSkinType(id->widget_skinTypeName);
		setDrawBackgroundLast(id->image_drawBackgroundLast);
		setImage(id->image_imageName);
		setRotation(id->image_rotationInDegrees);
		setTileImage(id->image_tileImage);
		setUpdateEveryFrame(id->image_updateEveryFrame);

		if(sizeFromDesc == Size::ZERO)
		{
			if(mSkinElement != NULL)
			{
				mWidgetDesc->widget_dimensions.size.width = mSkinElement->getWidth();
				mWidgetDesc->widget_dimensions.size.height = mSkinElement->getHeight();
				redraw();
			}
		}
	}

	Ogre::String Image::getClass()
	{
		return "Image";
	}

	bool Image::getDrawBackgroundLast()
	{
		return mDesc->image_drawBackgroundLast;
	}

	Ogre::String Image::getImageName()
	{
		return mDesc->image_imageName;
	}

	int Image::getRotation()
	{
		return mDesc->image_rotationInDegrees;
	}

	bool Image::getTileImage()
	{
		return mDesc->image_tileImage;
	}

	bool Image::getUpdateEveryFrame()
	{
		return mDesc->image_updateEveryFrame;
	}

	void Image::_drawImageBackground(Brush* brush)
	{
		Point center = mTexturePosition;
		center.translate(mClientDimensions.position);
		center += (mClientDimensions.size / 2.0);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement,mDesc->image_rotationInDegrees,center);
	}

	void Image::_drawImage(Brush* brush)
	{
		if(mDesc->image_imageName != "")
		{
			ColourValue prevColor = brush->getColour();
			Rect prevClipRegion = brush->getClipRegion();

			Rect clipRegion = mClientDimensions;
			clipRegion.translate(mTexturePosition);

			brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

			brush->setTexture(mDesc->image_imageName);
			if(mDesc->image_tileImage)
				brush->drawTiledRectangle(clipRegion,UVRect(0,0,1,1),mDesc->image_rotationInDegrees,clipRegion.position + (mClientDimensions.size / 2.0));
			else
				brush->drawRectangle(clipRegion,UVRect(0,0,1,1),mDesc->image_rotationInDegrees,clipRegion.position + (mClientDimensions.size / 2.0));

			brush->setClipRegion(prevClipRegion);
			Brush::getSingleton().setColor(prevColor);
		}
	}

	void Image::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		if(mDesc->image_drawBackgroundLast)
		{
			_drawImage(brush);
			_drawImageBackground(brush);
		}
		else
		{
			_drawImageBackground(brush);
			_drawImage(brush);
		}
	}

	void Image::rotate(int numDegrees)
	{
		mDesc->image_rotationInDegrees += numDegrees;

		redraw();
	}

	void Image::setDrawBackgroundLast(bool last)
	{
		mDesc->image_drawBackgroundLast = last;
	}

	void Image::setImage(const Ogre::String& name)
	{
		mDesc->image_imageName = name;

		if(mDesc->image_imageName != "")
		{
			// If texture not loaded, load it!
			if(!Ogre::TextureManager::getSingleton().resourceExists(mDesc->image_imageName))
			{
				Ogre::Image i;
				i.load(mDesc->image_imageName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			}
		}

		redraw();
	}

	void Image::setRotation(int numDegrees)
	{
		mDesc->image_rotationInDegrees = numDegrees;

		redraw();
	}

	void Image::setTileImage(bool tile)
	{
		mDesc->image_tileImage = tile;

		redraw();
	}

	void Image::setTransparencyPicking(bool transparencyPicking)
	{
		Widget::setTransparencyPicking(transparencyPicking);
	}

	void Image::setUpdateEveryFrame(bool update)
	{
		if(mDesc->image_updateEveryFrame == update)
			return;

		mDesc->image_updateEveryFrame = update;
		if(mDesc->image_updateEveryFrame)
		{
			TimerDesc d;
			d.repeat = true;
			d.timePeriod = -1;
			mUpdateTimer = TimerManager::getSingleton().createTimer(d);
			mUpdateTimer->setCallback(&Image::updateTimerCallback,this);
			mUpdateTimer->start();
		}
		else
		{
			mUpdateTimer->stop();
			TimerManager::getSingleton().destroyTimer(mUpdateTimer);
			mUpdateTimer = NULL;
		}
	}

	void Image::updateTimerCallback()
	{
		redraw();
	}
}
