#include "QuickGUITitleBar.h"
#include "QuickGUIWindow.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIManager.h"
#include "QuickGUIFactoryManager.h"

namespace QuickGUI
{
	const Ogre::String TitleBar::BACKGROUND = "background";
	const Ogre::String TitleBar::CLOSE_BUTTON = "close_button";

	void TitleBar::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TitleBar");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(CLOSE_BUTTON);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TitleBar",d);
	}

	TitleBarDesc::TitleBarDesc(const Ogre::String& id) :
		ComponentWidgetDesc(id)
	{
		resetToDefault();
	}

	void TitleBarDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		titlebar_closeButton = false;
		titlebar_closeButtonPadding = 2;
		titlebar_closeButtonSkinType = "default.close";
		widget_dimensions.size.height = 25;
		widget_dragable = true;
		widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		titlebar_verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;

		textDesc.resetToDefault();
		textDesc.horizontalTextAlignment = TEXT_ALIGNMENT_HORIZONTAL_LEFT;
	}

	TitleBar::TitleBar(const Ogre::String& name) :
		ComponentWidget(name),
		mCloseButton(NULL),
		mHeightBuffer(3),
		mText(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	TitleBar::~TitleBar()
	{
		OGRE_DELETE_T(mText,Text,Ogre::MEMCATEGORY_GENERAL);
	}

	void TitleBar::_initialize(WidgetDesc* d)
	{
		ComponentWidget::_initialize(d);

		mDesc = dynamic_cast<TitleBarDesc*>(mWidgetDesc);

		setSkinType(d->widget_skinTypeName);

		TitleBarDesc* td = dynamic_cast<TitleBarDesc*>(d);

		// Create CloseButton if property is set.
		if(td->titlebar_closeButton)
		{
			ButtonDesc* bd = dynamic_cast<ButtonDesc*>(FactoryManager::getSingleton().getWidgetDescFactory()->getInstance("DefaultButtonDesc"));
			bd->resetToDefault();
			bd->widget_name = ".CloseButton";

			if((td->titlebar_closeButtonPadding * 2.0) >= mClientDimensions.size.height)
				throw Exception(Exception::ERR_INVALIDPARAMS,"CloseButtonPadding exceeds height of TitleBar, cannot create Close Button!","TitleBar::_initialize");

			bd->widget_dimensions.size.width = mClientDimensions.size.height - (td->titlebar_closeButtonPadding * 2.0);
			bd->widget_dimensions.size.height = bd->widget_dimensions.size.width;
			bd->widget_dimensions.position.x = mDesc->widget_dimensions.size.width - mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_RIGHT) - td->titlebar_closeButtonPadding - bd->widget_dimensions.size.width;
			bd->widget_dimensions.position.y = td->titlebar_closeButtonPadding;
			bd->widget_skinTypeName = td->titlebar_closeButtonSkinType;
			bd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

			mCloseButton = dynamic_cast<Button*>(Widget::create("Button",bd));
			addComponent(CLOSE_BUTTON,mCloseButton);
			
			mCloseButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&TitleBar::onCloseButtonUp,this);

			mClientDimensions.size.width = mCloseButton->getPosition().x - mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_LEFT);
		}

		// Text

		mDesc->titlebar_verticalTextAlignment = td->titlebar_verticalTextAlignment;

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;

		mDesc->textDesc.allottedWidth = mClientDimensions.size.width;
		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);
	}

	void TitleBar::drag(int xOffset, int yOffset)
	{
		if(mWindow == NULL)
			throw Exception(Ogre::Exception::ERR_INVALID_STATE,"TitleBar has no parent Window!","TitleBar::drag");
		
		Point p = mWindow->getPosition();
		mWindow->setPosition(Point(p.x + xOffset,p.y + yOffset));
	}

	Ogre::String TitleBar::getClass()
	{
		return "TitleBar";
	}

	HorizontalTextAlignment TitleBar::getHorizontalTextAlignment()
	{
		return mText->getHorizontalTextAlignment();
	}

	Ogre::UTFString TitleBar::getText()
	{
		return mText->getText();
	}

	VerticalTextAlignment TitleBar::getVerticalTextAlignment()
	{
		return mDesc->titlebar_verticalTextAlignment;
	}

	void TitleBar::onCloseButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			mWindow->setVisible(false);
	}

	void TitleBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);
		}

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));

		if(mText->empty())
			return;

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = 0;

		switch(mDesc->titlebar_verticalTextAlignment)
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

	void TitleBar::setColor(const Ogre::ColourValue& cv)
	{
		mText->setColor(cv);

		redraw();
	}

	void TitleBar::setColor(const Ogre::ColourValue& cv, unsigned int index)
	{
		mText->setColor(cv,index);

		redraw();
	}

	void TitleBar::setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setColor(cv,startIndex,endIndex);

		redraw();
	}

	void TitleBar::setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setColor(cv,c,allOccurrences);

		redraw();
	}

	void TitleBar::setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setColor(cv,s,allOccurrences);

		redraw();
	}

	void TitleBar::setFont(const Ogre::String& fontName)
	{
		mText->setFont(fontName);
		setHeight(mDesc->textDesc.getTextHeight() + (mHeightBuffer * 2));

		redraw();
	}

	void TitleBar::setFont(const Ogre::String& fontName, unsigned int index)
	{
		mText->setFont(fontName,index);
		setHeight(mDesc->textDesc.getTextHeight() + (mHeightBuffer * 2));

		redraw();
	}

	void TitleBar::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setFont(fontName,startIndex,endIndex);
		setHeight(mDesc->textDesc.getTextHeight() + (mHeightBuffer * 2));

		redraw();
	}

	void TitleBar::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setFont(fontName,c,allOccurrences);
		setHeight(mDesc->textDesc.getTextHeight() + (mHeightBuffer * 2));

		redraw();
	}

	void TitleBar::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setFont(fontName,s,allOccurrences);
		setHeight(mDesc->textDesc.getTextHeight() + (mHeightBuffer * 2));

		redraw();
	}

	void TitleBar::setResizable(bool resizable)
	{
		Widget::setResizable(resizable);
	}

	void TitleBar::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->setText(s,fp,cv);
		setHeight(mDesc->textDesc.getTextHeight() + (mHeightBuffer * 2));

		redraw();
	}

	void TitleBar::setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		setText(s,Text::getFont(fontName),cv);
	}

	void TitleBar::setText(std::vector<TextSegment> segments)
	{
		mText->setText(segments);
		setHeight(mDesc->textDesc.getTextHeight() + (mHeightBuffer * 2));

		redraw();
	}

	void TitleBar::setHorizontalTextAlignment(HorizontalTextAlignment a)
	{
		mText->setHorizontalTextAlignment(a);

		redraw();
	}

	void TitleBar::setVerticalTextAlignment(VerticalTextAlignment a)
	{
		mDesc->titlebar_verticalTextAlignment = a;

		redraw();
	}

	void TitleBar::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;
		if(mText != NULL)
			mText->setAllottedWidth(mClientDimensions.size.width);

		if(mSkinType != NULL)
		{
			SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
			mClientDimensions.position.x = se->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = se->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));

			if(mCloseButton)
				mClientDimensions.size.width = mCloseButton->getPosition().x - se->getBorderThickness(BORDER_LEFT);
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);

		redraw();
	}
}
