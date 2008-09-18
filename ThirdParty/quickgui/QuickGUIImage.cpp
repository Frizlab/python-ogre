#include "QuickGUIImage.h"
#include "QuickGUISkinDefinitionManager.h"
#include "OgreTextureManager.h"

namespace QuickGUI
{
	const Ogre::String Image::BACKGROUND = "background";

	void Image::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("Image");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Image",d);
	}

	ImageDesc::ImageDesc() :
		WidgetDesc()
	{
		transparencyPicking = false;

		imageName = "";
		tileImage = false;
		updateEveryFrame = false;
	}

	void ImageDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("ImageName",&imageName);
		b->IO("TileImage",&tileImage);
		b->IO("UpdateEveryFrame",&updateEveryFrame);
	}

	Image::Image(const Ogre::String& name) :
		Widget(name),
		mUpdateTimer(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	Image::~Image()
	{
	}

	void Image::_initialize(WidgetDesc* d)
	{
		d->transparencyPicking = false;

		Widget::_initialize(d);

		mDesc = dynamic_cast<ImageDesc*>(mWidgetDesc);

		ImageDesc* id = dynamic_cast<ImageDesc*>(d);

		setSkinType(id->skinTypeName);
		setImage(id->imageName);
		setTileImage(id->tileImage);
		setUpdateEveryFrame(id->updateEveryFrame);
	}

	Widget* Image::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new Image(widgetName);

		newWidget->_createDescObject("ImageDesc");

		return newWidget;
	}

	Ogre::String Image::getClass()
	{
		return "Image";
	}

	Ogre::String Image::getImage()
	{
		return mDesc->imageName;
	}

	bool Image::getTileImage()
	{
		return mDesc->tileImage;
	}

	bool Image::getUpdateEveryFrame()
	{
		return mDesc->updateEveryFrame;
	}

	void Image::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		mSkinElementName = BACKGROUND;
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));

		if(mDesc->imageName != "")
		{
			Ogre::ColourValue prevColor = brush->getColour();
			Rect prevClipRegion = brush->getClipRegion();

			Rect clipRegion = mClientDimensions;
			clipRegion.translate(mTexturePosition);

			brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

			brush->setTexture(mDesc->imageName);
			if(mDesc->tileImage)
				brush->drawTiledRectangle(prevClipRegion.getIntersection(clipRegion),UVRect(0,0,1,1));
			else
				brush->drawRectangle(prevClipRegion.getIntersection(clipRegion),UVRect(0,0,1,1));

			brush->setClipRegion(prevClipRegion);
			Brush::getSingleton().setColor(prevColor);
		}
	}

	void Image::setImage(const Ogre::String& name)
	{
		mDesc->imageName = name;

		if(mDesc->imageName != "")
		{
			// If texture not loaded, load it!
			if(!Ogre::TextureManager::getSingleton().resourceExists(mDesc->imageName))
			{
				Ogre::Image i;
				i.load(mDesc->imageName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			}
		}

		redraw();
	}

	void Image::setTileImage(bool tile)
	{
		mDesc->tileImage = tile;

		redraw();
	}

	void Image::setTransparencyPicking(bool transparencyPicking)
	{
		Widget::setTransparencyPicking(transparencyPicking);
	}

	void Image::setUpdateEveryFrame(bool update)
	{
		if(mDesc->updateEveryFrame == update)
			return;

		mDesc->updateEveryFrame = update;
		if(mDesc->updateEveryFrame)
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
