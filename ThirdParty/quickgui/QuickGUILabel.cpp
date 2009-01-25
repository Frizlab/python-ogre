#include "QuickGUILabel.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String Label::BACKGROUND = "background";

	void Label::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Label");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Label",d);
	}

	LabelDesc::LabelDesc(const Ogre::String& id) :
		WidgetDesc(id)
	{
		resetToDefault();
	}

	void LabelDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		label_defaultFontName = Text::getFirstAvailableFont()->getName();
		label_defaultColor = Ogre::ColourValue::White;
		label_verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;
		textDesc.resetToDefault();
	}

	void LabelDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("VerticalTextAlignment",&label_verticalTextAlignment);

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
		OGRE_DELETE_T(mText,Text,Ogre::MEMCATEGORY_GENERAL);
	}

	void Label::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<LabelDesc*>(mWidgetDesc);

		LabelDesc* ld = dynamic_cast<LabelDesc*>(d);

		setSkinType(d->widget_skinTypeName);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = ld->textDesc;

		// Store pointer to SkinType
		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);
		}

		SkinElement* se = st->getSkinElement(mSkinElementName);

		if(mDesc->widget_dimensions.size.width == 0)
		{
			if(mDesc->textDesc.segments.empty())
				mDesc->widget_dimensions.size.width = 50;
			else
				mDesc->widget_dimensions.size.width = se->getBorderThickness(BORDER_LEFT) + mDesc->textDesc.getTextWidth() + se->getBorderThickness(BORDER_RIGHT);
		}
		if(mDesc->widget_dimensions.size.height == 0)
		{
			if(mDesc->textDesc.segments.empty())
				mDesc->widget_dimensions.size.height = 20;
			else
				mDesc->widget_dimensions.size.height = se->getBorderThickness(BORDER_TOP) + mDesc->textDesc.getTextHeight() + se->getBorderThickness(BORDER_BOTTOM);
		}
		// Now that dimensions may have changed, update client dimensions
		updateClientDimensions();

		mDesc->label_verticalTextAlignment = ld->label_verticalTextAlignment;
		mDesc->textDesc.allottedWidth = mDesc->widget_dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);
	}

	void Label::addText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->addText(s,fp,cv);

		redraw();
	}

	void Label::addText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		addText(s,Text::getFont(fontName),cv);
	}

	void Label::addText(Ogre::UTFString s)
	{
		addText(s,mDesc->label_defaultFontName,mDesc->label_defaultColor);
	}

	void Label::addText(std::vector<TextSegment> segments)
	{
		mText->addText(segments);

		redraw();
	}

	void Label::clearText()
	{
		mText->clearText();

		redraw();
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
		return mDesc->label_verticalTextAlignment;
	}

	void Label::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);
		}

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = 0;

		switch(mDesc->label_verticalTextAlignment)
		{
		case TEXT_ALIGNMENT_VERTICAL_BOTTOM:
			yPos = mDesc->widget_dimensions.size.height - st->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_BOTTOM) - textHeight;
			break;
		case TEXT_ALIGNMENT_VERTICAL_CENTER:
			yPos = (mDesc->widget_dimensions.size.height / 2.0) - (textHeight / 2.0);
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
		mDesc->label_defaultColor = cv;

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

	void Label::setDefaultColor(const Ogre::ColourValue& cv)
	{
		mDesc->label_defaultColor = cv;
	}

	void Label::setDefaultFont(const Ogre::String& fontName)
	{
		mDesc->label_defaultFontName = fontName;
		mCurrentFont = Text::getFont(fontName);
	}

	void Label::setFont(const Ogre::String& fontName)
	{
		mDesc->label_defaultFontName = fontName;
		mCurrentFont = Text::getFont(fontName);

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
		mDesc->label_verticalTextAlignment = a;

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
		setText(s,mDesc->label_defaultFontName,mDesc->label_defaultColor);
	}

	void Label::setText(std::vector<TextSegment> segments)
	{
		mText->setText(segments);

		redraw();
	}

	void Label::setVerticalLineSpacing(float distance)
	{
		if(distance == mText->getVerticalLineSpacing())
			return;

		mText->setVerticalLineSpacing(distance);

		redraw();
	}

	void Label::updateClientDimensions()
	{
		Widget::updateClientDimensions();
		if(mText != NULL)
			mText->setAllottedWidth(mClientDimensions.size.width);

		redraw();
	}
}
