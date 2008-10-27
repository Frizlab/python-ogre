#include "QuickGUITab.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String Tab::FRONT = "front";
	const Ogre::String Tab::MAIN = "main";

	void Tab::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("Tab");
		d->defineSkinElement(FRONT);
		d->defineSkinElement(MAIN);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Tab",d);
	}

	TabDesc::TabDesc() :
		LabelDesc()
	{
		frontWidth = -1;
	}

	void TabDesc::serialize(SerialBase* b)
	{
		LabelDesc::serialize(b);

		b->IO("FrontWidth",&frontWidth);
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
			mDesc->dimensions.size.width = mDesc->frontWidth + 50;
		else
			mDesc->dimensions.size.width = mDesc->frontWidth + mDesc->textDesc.getTextWidth() + mDesc->padding[PADDING_RIGHT];

		mText->setAllottedWidth(mDesc->textDesc.getTextWidth() + mDesc->padding[PADDING_RIGHT]);
	}

	void Tab::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<TabDesc*>(mWidgetDesc);

		TabDesc* td = dynamic_cast<TabDesc*>(d);

		setSkinType(td->skinTypeName);

		if(td->frontWidth < 0)
			mDesc->frontWidth = mSkinType->getSkinElement(FRONT)->getWidth();
		else
			mDesc->frontWidth = td->frontWidth;

		mDesc->padding[PADDING_LEFT] = td->padding[PADDING_LEFT];
		mDesc->padding[PADDING_RIGHT] = td->padding[PADDING_RIGHT];
		mDesc->padding[PADDING_BOTTOM] = td->padding[PADDING_BOTTOM];
		mDesc->padding[PADDING_TOP] = td->padding[PADDING_TOP];

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;

		if(mDesc->textDesc.segments.empty())
			mDesc->dimensions.size.width = mDesc->frontWidth + 50;
		else
			mDesc->dimensions.size.width = mDesc->frontWidth + mDesc->padding[PADDING_LEFT] + mDesc->textDesc.getTextWidth() + mDesc->padding[PADDING_RIGHT];

		if(mDesc->textDesc.segments.empty())
			mDesc->dimensions.size.height = 20;
		else
			mDesc->dimensions.size.height = mDesc->padding[PADDING_TOP] + mDesc->textDesc.getTextHeight() + mDesc->padding[PADDING_BOTTOM];

		mDesc->textDesc.allottedWidth = mDesc->textDesc.getTextWidth() + mDesc->padding[PADDING_RIGHT];
		mText = new Text(mDesc->textDesc);

		mCurrentFontName = Text::getFirstAvailableFont()->getName();
		mCurrentColourValue = Ogre::ColourValue::White;
	}

	Widget* Tab::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new Tab(widgetName);

		newWidget->_createDescObject("TabDesc");

		return newWidget;
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

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);
		}

		Rect tabRect(mTexturePosition,mWidgetDesc->dimensions.size);
		
		// Draw the front part of the tab
		tabRect.size.width = mDesc->frontWidth;
		brush->drawSkinElement(tabRect,st->getSkinElement(FRONT));

		// Draw the main part of the tab
		tabRect.position.x += mDesc->frontWidth;
		tabRect.size.width = mWidgetDesc->dimensions.size.width - mDesc->frontWidth;
		brush->drawSkinElement(tabRect,st->getSkinElement(MAIN));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect clipRegion;
		clipRegion.size = 
			Size(
				tabRect.size.width - mDesc->padding[PADDING_RIGHT] - mDesc->padding[PADDING_LEFT],
				tabRect.size.height - mDesc->padding[PADDING_BOTTOM] - mDesc->padding[PADDING_TOP]);
		clipRegion.position = tabRect.position;
		clipRegion.translate(Point(mDesc->padding[PADDING_LEFT],mDesc->padding[PADDING_TOP]));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

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

	void Tab::setPadding(Padding p, float distance)
	{
		Label::setPadding(p,distance);

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
		mClientDimensions.size = mWidgetDesc->dimensions.size;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);
	}
}
