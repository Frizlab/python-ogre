#include "QuickGUITab.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String Tab::FRONT = "front";
	const Ogre::String Tab::MAIN = "main";

	void Tab::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Tab");
		d->defineSkinElement(FRONT);
		d->defineSkinElement(MAIN);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Tab",d);
	}

	TabDesc::TabDesc(const Ogre::String& id) :
		LabelDesc(id)
	{
		resetToDefault();
	}

	void TabDesc::resetToDefault()
	{
		LabelDesc::resetToDefault();

		tab_frontWidth = -1;
	}

	void TabDesc::serialize(SerialBase* b)
	{
		LabelDesc::serialize(b);

		b->IO("FrontWidth",&tab_frontWidth);
	}

	Tab::Tab(const Ogre::String& name) :
		Label(name)
	{
		mSkinElementName = MAIN;
	}

	Tab::~Tab()
	{
	}

	void Tab::_adjustTabWidth()
	{
		if(mDesc->textDesc.segments.empty())
			mDesc->widget_dimensions.size.width = mDesc->tab_frontWidth + 50;
		else
			mDesc->widget_dimensions.size.width = mDesc->tab_frontWidth + mDesc->textDesc.getTextWidth() + mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_RIGHT);

		mText->setAllottedWidth(mDesc->textDesc.getTextWidth() + mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_RIGHT));
	}

	void Tab::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<TabDesc*>(mWidgetDesc);

		TabDesc* td = dynamic_cast<TabDesc*>(d);

		setSkinType(td->widget_skinTypeName);

		if(td->tab_frontWidth < 0)
			mDesc->tab_frontWidth = mSkinType->getSkinElement(FRONT)->getWidth();
		else
			mDesc->tab_frontWidth = td->tab_frontWidth;

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;

		if(mDesc->textDesc.segments.empty())
			mDesc->widget_dimensions.size.width = mDesc->tab_frontWidth + 50;
		else
			mDesc->widget_dimensions.size.width = mDesc->tab_frontWidth + mSkinType->getSkinElement(MAIN)->getBorderThickness(BORDER_LEFT) + mDesc->textDesc.getTextWidth() + mSkinType->getSkinElement(MAIN)->getBorderThickness(BORDER_RIGHT);

		if(mDesc->textDesc.segments.empty())
			mDesc->widget_dimensions.size.height = 20;
		else
			mDesc->widget_dimensions.size.height = mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_TOP) + mDesc->textDesc.getTextHeight() + mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_BOTTOM);

		mDesc->label_verticalTextAlignment = td->label_verticalTextAlignment;
		mDesc->textDesc.allottedWidth = mDesc->textDesc.getTextWidth() + mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_RIGHT);
		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);
	}

	BorderSide Tab::getBorderSide(Point p)
	{
		return BORDER_NONE;
	}

	Ogre::String Tab::getClass()
	{
		return "Tab";
	}

	void Tab::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);
		}

		Rect tabRect(mTexturePosition,mWidgetDesc->widget_dimensions.size);
		
		// Draw the front part of the tab
		tabRect.size.width = mDesc->tab_frontWidth;
		brush->drawSkinElement(tabRect,st->getSkinElement(FRONT));

		// Draw the main part of the tab
		tabRect.position.x += mDesc->tab_frontWidth;
		tabRect.size.width = mWidgetDesc->widget_dimensions.size.width - mDesc->tab_frontWidth;
		brush->drawSkinElement(tabRect,st->getSkinElement(MAIN));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Draw text

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

		Rect clipRegion;
		clipRegion.size = 
			Size(
				tabRect.size.width - st->getSkinElement(MAIN)->getBorderThickness(BORDER_RIGHT) - st->getSkinElement(MAIN)->getBorderThickness(BORDER_LEFT),
				tabRect.size.height - st->getSkinElement(MAIN)->getBorderThickness(BORDER_BOTTOM) - st->getSkinElement(MAIN)->getBorderThickness(BORDER_TOP));
		clipRegion.position = tabRect.position;
		clipRegion.translate(Point(st->getSkinElement(MAIN)->getBorderThickness(BORDER_LEFT),st->getSkinElement(MAIN)->getBorderThickness(BORDER_TOP)));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		clipRegion.position.y += yPos;
		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	bool Tab::overBorderSide(Point p)
	{
		return false;
	}

	void Tab::setFont(const Ogre::String& fontName)
	{
		Label::setFont(fontName);

		_adjustTabWidth();
	}

	void Tab::setFont(const Ogre::String& fontName, unsigned int index)
	{
		Label::setFont(fontName,index);

		_adjustTabWidth();
	}

	void Tab::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		Label::setFont(fontName,startIndex,endIndex);

		_adjustTabWidth();
	}

	void Tab::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		Label::setFont(fontName,c,allOccurrences);

		_adjustTabWidth();
	}

	void Tab::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		Label::setFont(fontName,s,allOccurrences);

		_adjustTabWidth();
	}

	void Tab::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		Label::setText(s,fp,cv);

		_adjustTabWidth();
	}

	void Tab::setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		Label::setText(s,fontName,cv);

		_adjustTabWidth();
	}

	void Tab::setText(Ogre::UTFString s)
	{
		Label::setText(s);

		_adjustTabWidth();
	}

	void Tab::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);
	}
}
