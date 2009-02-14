#include "QuickGUIColorPicker.h"
#include "QuickGUISkinDefinitionManager.h"
#include "OgreTextureManager.h"
#include "QuickGUIFactoryManager.h"

namespace QuickGUI
{
	const Ogre::String ColorPicker::BACKGROUND = "background";
	const Ogre::String ColorPicker::COLORSOURCE = "colorsource";
	const Ogre::String ColorPicker::TRANSPARENCYBACKGROUND = "transparencybackground";
	const Ogre::String ColorPicker::TRANSPARENCYSLIDER = "transparencyslider";

	void ColorPicker::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ColorPicker");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(COLORSOURCE);
		d->defineSkinElement(TRANSPARENCYBACKGROUND);
		d->defineComponent(TRANSPARENCYSLIDER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ColorPicker",d);
	}

	ColorPickerDesc::ColorPickerDesc(const Ogre::String& id) :
		ComponentWidgetDesc(id)
	{
		resetToDefault();
	}

	void ColorPickerDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		colorpicker_imageName = "";

		widget_dimensions.size = Size(250,225);
	}

	void ColorPickerDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);

		b->IO("ImageName",&colorpicker_imageName);
	}

	ColorPicker::ColorPicker(const Ogre::String& name) :
		ComponentWidget(name),
		mColorSource(NULL),
		mTransparencySlider(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	ColorPicker::~ColorPicker()
	{
	}

	void ColorPicker::_initialize(WidgetDesc* d)
	{
		ComponentWidget::_initialize(d);

		mDesc = dynamic_cast<ColorPickerDesc*>(mWidgetDesc);

		ColorPickerDesc* cpd = dynamic_cast<ColorPickerDesc*>(d);

		setSkinType(cpd->widget_skinTypeName);
		setImage(cpd->colorpicker_imageName);
	}

	Ogre::String ColorPicker::getClass()
	{
		return "ColorPicker";
	}

	Ogre::String ColorPicker::getImage()
	{
		return mDesc->colorpicker_imageName;
	}

	void ColorPicker::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		mSkinElementName = BACKGROUND;
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));

		if(mDesc->colorpicker_imageName != "")
		{
			Ogre::ColourValue prevColor = brush->getColour();
			Rect prevClipRegion = brush->getClipRegion();

			Rect clipRegion = mClientDimensions;
			clipRegion.translate(mTexturePosition);

			brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

			brush->setTexture(mDesc->colorpicker_imageName);
			brush->drawRectangle(prevClipRegion.getIntersection(clipRegion),UVRect(0,0,1,1));

			brush->setClipRegion(prevClipRegion);
			Brush::getSingleton().setColor(prevColor);
		}
	}

	void ColorPicker::setImage(const Ogre::String& name)
	{
		mDesc->colorpicker_imageName = name;

		if(mColorSource != NULL)
			mColorSource->setImage(name);
	}
}
