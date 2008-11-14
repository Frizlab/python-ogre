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
		verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;
	}

	void ListTextItemDesc::serialize(SerialBase* b)
	{
		ListItemDesc::serialize(b);

		b->IO("VerticalTextAlignment",&verticalTextAlignment);

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
		mDesc->verticalTextAlignment = ltid->verticalTextAlignment;
		mText = new Text(mDesc->textDesc);

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

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = 0;

		switch(mDesc->verticalTextAlignment)
		{
		case TEXT_ALIGNMENT_VERTICAL_BOTTOM:
			yPos = mDesc->dimensions.size.height - st->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_BOTTOM) - textHeight;
			break;
		case TEXT_ALIGNMENT_VERTICAL_CENTER:
			yPos = (mDesc->dimensions.size.height / 2.0) - (textHeight / 2.0);
			break;
		case TEXT_ALIGNMENT_VERTICAL_TOP:
			yPos = st->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_TOP);
			break;
		}

		// Clip to client dimensions
		Rect clipRegion(mClientDimensions);
		clipRegion.translate(mTexturePosition);

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		clipRegion = mClientDimensions;
		clipRegion.position.y = yPos;
		clipRegion.translate(mTexturePosition);		

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
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

		mText->setAllottedWidth(mDesc->dimensions.size.width);
	}
}
