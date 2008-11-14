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
		verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;
	}

	void LabelDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("VerticalTextAlignment",&verticalTextAlignment);

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

		// Store pointer to SkinType
		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);
		}

		SkinElement* se = st->getSkinElement(mSkinElementName);

		if(mDesc->dimensions.size.width == 0)
		{
			if(mDesc->textDesc.segments.empty())
				mDesc->dimensions.size.width = 50;
			else
				mDesc->dimensions.size.width = se->getBorderThickness(BORDER_LEFT) + mDesc->textDesc.getTextWidth() + se->getBorderThickness(BORDER_RIGHT);
		}
		if(mDesc->dimensions.size.height == 0)
		{
			if(mDesc->textDesc.segments.empty())
				mDesc->dimensions.size.height = 20;
			else
				mDesc->dimensions.size.height = se->getBorderThickness(BORDER_TOP) + mDesc->textDesc.getTextHeight() + se->getBorderThickness(BORDER_BOTTOM);
		}

		mDesc->verticalTextAlignment = ld->verticalTextAlignment;
		mDesc->textDesc.allottedWidth = ld->dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
		mText = new Text(mDesc->textDesc);

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

	HorizontalTextAlignment Label::getHorizontalTextAlignment()
	{
		return mText->getHorizontalTextAlignment();
	}

	Ogre::UTFString Label::getText()
	{
		return mText->getText();
	}

	float Label::getVerticalLineSpacing()
	{
		return mText->getVerticalLineSpacing();
	}

	VerticalTextAlignment Label::getVerticalTextAlignment()
	{
		return mDesc->verticalTextAlignment;
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

	void Label::setHorizontalTextAlignment(HorizontalTextAlignment a)
	{
		mText->setHorizontalTextAlignment(a);

		redraw();
	}

	void Label::setVerticalTextAlignment(VerticalTextAlignment a)
	{
		mDesc->verticalTextAlignment = a;

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
