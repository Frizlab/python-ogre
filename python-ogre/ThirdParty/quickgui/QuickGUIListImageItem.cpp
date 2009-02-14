#include "QuickGUIListImageItem.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	void ListImageItem::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ListImageItem");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ListImageItem",d);
	}

	ListImageItemDesc::ListImageItemDesc(const Ogre::String& id) :
		ListItemDesc(id)
	{
		resetToDefault();
	}

	void ListImageItemDesc::resetToDefault()
	{
		ListItemDesc::resetToDefault();

		listimageitem_imageName = "";
		listimageitem_tileImage = false;
	}

	void ListImageItemDesc::serialize(SerialBase* b)
	{
		ListItemDesc::serialize(b);

		b->IO("ImageName",&listimageitem_imageName);
		b->IO("TileImage",&listimageitem_tileImage);
	}

	ListImageItem::ListImageItem(const Ogre::String& name) :
		ListItem(name)
	{
		mSkinElementName = DEFAULT;
	}

	ListImageItem::~ListImageItem()
	{
	}

	void ListImageItem::_initialize(WidgetDesc* d)
	{
		ListItem::_initialize(d);

		mDesc = dynamic_cast<ListImageItemDesc*>(mWidgetDesc);

		ListImageItemDesc* ltid = dynamic_cast<ListImageItemDesc*>(d);

		setSkinType(ltid->widget_skinTypeName);
		setImage(ltid->listimageitem_imageName);
		setTileImage(ltid->listimageitem_tileImage);
	}

	Ogre::String ListImageItem::getClass()
	{
		return "ListImageItem";
	}

	Ogre::String ListImageItem::getImageName()
	{
		return mDesc->listimageitem_imageName;
	}

	bool ListImageItem::getTileImage()
	{
		return mDesc->listimageitem_tileImage;
	}

	void ListImageItem::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		if(mDesc->listimageitem_imageName != "")
		{
			Ogre::ColourValue prevColor = brush->getColour();
			Rect prevClipRegion = brush->getClipRegion();

			Rect clipRegion = mClientDimensions;
			clipRegion.translate(mTexturePosition);

			brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

			brush->setTexture(mDesc->listimageitem_imageName);
			if(mDesc->listimageitem_tileImage)
				brush->drawTiledRectangle(prevClipRegion.getIntersection(clipRegion),UVRect(0,0,1,1));
			else
				brush->drawRectangle(prevClipRegion.getIntersection(clipRegion),UVRect(0,0,1,1));

			brush->setClipRegion(prevClipRegion);
			Brush::getSingleton().setColor(prevColor);
		}
	}

	void ListImageItem::setImage(const Ogre::String& name)
	{
		mDesc->listimageitem_imageName = name;

		if(mDesc->listimageitem_imageName != "")
		{
			// If texture not loaded, load it!
			if(!Ogre::TextureManager::getSingleton().resourceExists(mDesc->listimageitem_imageName))
			{
				Ogre::Image i;
				i.load(mDesc->listimageitem_imageName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			}
		}

		redraw();
	}

	void ListImageItem::setTileImage(bool tile)
	{
		mDesc->listimageitem_tileImage = tile;

		redraw();
	}
}
