#include "QuickGUILabel.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String Label::BACKGROUND = "background";

	void Label::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("Label");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Label",d);
	}

	LabelDesc::LabelDesc() :
		WidgetDesc()
	{
		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			padding[i] = 5.0;
		}
	}

	void LabelDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			b->IO(StringConverter::toString(static_cast<Padding>(i)),&padding[i]);
		}

		textDesc.serialize(b);
	}

	Label::Label(const Ogre::String& name) :
		Widget(name),
		mText(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	Label::~Label()
	{
		delete mText;
	}

	void Label::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<LabelDesc*>(mWidgetDesc);

		LabelDesc* ld = dynamic_cast<LabelDesc*>(d);

		setSkinType(d->skinTypeName);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = ld->textDesc;

		mDesc->textDesc.allottedWidth = ld->dimensions.size.width - (ld->padding[PADDING_LEFT] + ld->padding[PADDING_RIGHT]);
		mText = new Text(mDesc->textDesc);

		setPadding(PADDING_BOTTOM,ld->padding[PADDING_BOTTOM]);
		setPadding(PADDING_LEFT,ld->padding[PADDING_LEFT]);
		setPadding(PADDING_RIGHT,ld->padding[PADDING_RIGHT]);
		setPadding(PADDING_TOP,ld->padding[PADDING_TOP]);

		mCurrentFontName = Text::getFirstAvailableFont()->getName();
		mCurrentColourValue = Ogre::ColourValue::White;
	}

	Widget* Label::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new Label(widgetName);

		newWidget->_createDescObject("LabelDesc");

		return newWidget;
	}

	Ogre::String Label::getClass()
	{
		return "Label";
	}

	float Label::getPadding(Padding p)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","Label::getPadding");

		return mDesc->padding[p];
	}

	Ogre::UTFString Label::getText()
	{
		return mText->getText();
	}

	float Label::getVerticalLineSpacing()
	{
		return mText->getVerticalLineSpacing();
	}

	void Label::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);
		}

		mSkinElementName = BACKGROUND;
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

	void Label::setColor(const Ogre::ColourValue& cv)
	{
		mCurrentColourValue = cv;

		mText->setColor(cv);

		redraw();
	}

	void Label::setColor(const Ogre::ColourValue& cv, unsigned int index)
	{
		mText->setColor(cv,index);

		redraw();
	}

	void Label::setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setColor(cv,startIndex,endIndex);

		redraw();
	}

	void Label::setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setColor(cv,c,allOccurrences);

		redraw();
	}

	void Label::setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setColor(cv,s,allOccurrences);

		redraw();
	}

	void Label::setFont(const Ogre::String& fontName)
	{
		mCurrentFontName = fontName;

		mText->setFont(fontName);

		redraw();
	}

	void Label::setFont(const Ogre::String& fontName, unsigned int index)
	{
		mText->setFont(fontName,index);

		redraw();
	}

	void Label::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setFont(fontName,startIndex,endIndex);

		redraw();
	}

	void Label::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setFont(fontName,c,allOccurrences);

		redraw();
	}

	void Label::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setFont(fontName,s,allOccurrences);

		redraw();
	}

	void Label::setPadding(Padding p, float distance)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","Label::setPadding");

		mDesc->padding[p] = distance;
		mText->setAllottedWidth(mWidgetDesc->dimensions.size.width - (mDesc->padding[PADDING_LEFT] + mDesc->padding[PADDING_RIGHT]));

		redraw();
	}

	void Label::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		redraw();
	}

	void Label::setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		setText(s,Text::getFont(fontName),cv);
	}

	void Label::setText(Ogre::UTFString s)
	{
		setText(s,mCurrentFontName,mCurrentColourValue);
	}

	void Label::setVerticalLineSpacing(float distance)
	{
		if(distance == mText->getVerticalLineSpacing())
			return;

		mText->setVerticalLineSpacing(distance);

		redraw();
	}
}
