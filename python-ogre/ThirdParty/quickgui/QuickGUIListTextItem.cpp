#include "QuickGUIListTextItem.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	void ListTextItem::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("ListTextItem");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ListTextItem",d);
	}

	ListTextItemDesc::ListTextItemDesc() :
		ListItemDesc()
	{
		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			padding[i] = 5.0;
		}
	}

	void ListTextItemDesc::serialize(SerialBase* b)
	{
		ListItemDesc::serialize(b);

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			b->IO(StringConverter::toString(static_cast<Padding>(i)),&padding[i]);
		}

		textDesc.serialize(b);
	}

	ListTextItem::ListTextItem(const Ogre::String& name) :
		ListItem(name),
		mText(NULL)
	{
		mSkinElementName = DEFAULT;
	}

	ListTextItem::~ListTextItem()
	{
		delete mText;
	}

	void ListTextItem::_initialize(WidgetDesc* d)
	{
		ListItem::_initialize(d);

		mDesc = dynamic_cast<ListTextItemDesc*>(mWidgetDesc);

		ListTextItemDesc* ltid = dynamic_cast<ListTextItemDesc*>(d);

		setSkinType(d->skinTypeName);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = ltid->textDesc;

		mText = new Text(mDesc->textDesc);

		setPadding(PADDING_BOTTOM,ltid->padding[PADDING_BOTTOM]);
		setPadding(PADDING_LEFT,ltid->padding[PADDING_LEFT]);
		setPadding(PADDING_RIGHT,ltid->padding[PADDING_RIGHT]);
		setPadding(PADDING_TOP,ltid->padding[PADDING_TOP]);

		mCurrentFontName = Text::getFirstAvailableFont()->getName();
		mCurrentColourValue = Ogre::ColourValue::White;
	}

	Widget* ListTextItem::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new ListTextItem(widgetName);
		
		newWidget->_createDescObject("ListTextItemDesc");

		return newWidget;
	}

	Ogre::String ListTextItem::getClass()
	{
		return "ListTextItem";
	}

	Ogre::UTFString ListTextItem::getText()
	{
		return mText->getText();
	}

	float ListTextItem::getPadding(Padding p)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","Label::getPadding");

		return mDesc->padding[p];
	}

	void ListTextItem::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect clipRegion;
		clipRegion.size = 
			Size(
				mDesc->dimensions.size.width - mDesc->padding[PADDING_RIGHT],
				mDesc->dimensions.size.height - mDesc->padding[PADDING_BOTTOM]);
		clipRegion.position = mTexturePosition;
		clipRegion.translate(Point(mDesc->padding[PADDING_LEFT],mDesc->padding[PADDING_TOP]));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);

		Brush::getSingleton().setColor(prevColor);
	}

	void ListTextItem::setColor(const Ogre::ColourValue& cv)
	{
		mCurrentColourValue = cv;

		mText->setColor(cv);

		redraw();
	}

	void ListTextItem::setColor(const Ogre::ColourValue& cv, unsigned int index)
	{
		mText->setColor(cv,index);

		redraw();
	}

	void ListTextItem::setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setColor(cv,startIndex,endIndex);

		redraw();
	}

	void ListTextItem::setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setColor(cv,c,allOccurrences);

		redraw();
	}

	void ListTextItem::setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setColor(cv,s,allOccurrences);

		redraw();
	}

	void ListTextItem::setFont(const Ogre::String& fontName)
	{
		mCurrentFontName = fontName;

		mText->setFont(fontName);

		redraw();
	}

	void ListTextItem::setFont(const Ogre::String& fontName, unsigned int index)
	{
		mText->setFont(fontName,index);

		redraw();
	}

	void ListTextItem::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setFont(fontName,startIndex,endIndex);

		redraw();
	}

	void ListTextItem::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setFont(fontName,c,allOccurrences);

		redraw();
	}

	void ListTextItem::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setFont(fontName,s,allOccurrences);

		redraw();
	}

	void ListTextItem::setPadding(Padding p, float distance)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","Label::setPadding");

		mDesc->padding[p] = distance;
		mText->setAllottedWidth(mWidgetDesc->dimensions.size.width - (mDesc->padding[PADDING_LEFT] + mDesc->padding[PADDING_RIGHT]));

		redraw();
	}

	void ListTextItem::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		redraw();
	}

	void ListTextItem::setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		setText(s,Text::getFont(fontName),cv);
	}

	void ListTextItem::setText(Ogre::UTFString s)
	{
		setText(s,mCurrentFontName,mCurrentColourValue);
	}

	void ListTextItem::setWidth(float pixelWidth)
	{
		ListItem::setWidth(pixelWidth);

		mText->setAllottedWidth(mDesc->dimensions.size.width - (mDesc->padding[PADDING_LEFT] + mDesc->padding[PADDING_RIGHT]));
	}
}
