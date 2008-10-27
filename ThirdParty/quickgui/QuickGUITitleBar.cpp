#include "QuickGUITitleBar.h"
#include "QuickGUIWindow.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	const Ogre::String TitleBar::BACKGROUND = "background";
	const Ogre::String TitleBar::CLOSE_BUTTON = "close_button";

	void TitleBar::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("TitleBar");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(CLOSE_BUTTON);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TitleBar",d);
	}

	TitleBarDesc::TitleBarDesc() :
		ComponentWidgetDesc()
	{
		closeButton = false;
		closeButtonPadding = 2;
		closeButtonSkinType = "default.close";
		dimensions.size.height = 25;
		dragable = true;
		horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			padding[i] = 5.0;
		}

		//resizable = false;
	}

	TitleBar::TitleBar(const Ogre::String& name) :
		ComponentWidget(name),
		mCloseButton(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	TitleBar::~TitleBar()
	{
	}

	void TitleBar::_initialize(WidgetDesc* d)
	{
		ComponentWidget::_initialize(d);

		mDesc = dynamic_cast<TitleBarDesc*>(mWidgetDesc);

		setSkinType(d->skinTypeName);

		TitleBarDesc* td = dynamic_cast<TitleBarDesc*>(d);

		// Create CloseButton if property is set.
		if(td->closeButton)
		{
			ButtonDesc bd;
			bd.name = ".CloseButton";

			if((td->closeButtonPadding * 2.0) >= mClientDimensions.size.height)
				throw Exception(Exception::ERR_INVALIDPARAMS,"CloseButtonPadding exceeds height of TitleBar, cannot create Close Button!","TitleBar::_initialize");

			bd.dimensions.size.width = mClientDimensions.size.height - (td->closeButtonPadding * 2.0);
			bd.dimensions.size.height = bd.dimensions.size.width;
			bd.dimensions.position.x = mClientDimensions.size.width - (td->closeButtonPadding + bd.dimensions.size.width);
			bd.dimensions.position.y = td->closeButtonPadding;
			bd.skinTypeName = td->closeButtonSkinType;
			bd.horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

			mCloseButton = dynamic_cast<Button*>(Widget::create("Button",bd));
			addComponent(CLOSE_BUTTON,mCloseButton);
			
			mCloseButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&TitleBar::onCloseButtonUp,this);
		}

		// Text

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;

		mDesc->textDesc.allottedWidth = td->dimensions.size.width - (td->padding[PADDING_LEFT] + td->padding[PADDING_RIGHT]);
		mText = new Text(mDesc->textDesc);

		setPadding(PADDING_BOTTOM,td->padding[PADDING_BOTTOM]);
		setPadding(PADDING_LEFT,td->padding[PADDING_LEFT]);
		setPadding(PADDING_RIGHT,td->padding[PADDING_RIGHT]);
		setPadding(PADDING_TOP,td->padding[PADDING_TOP]);
	}

	Widget* TitleBar::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new TitleBar(widgetName);

		newWidget->_createDescObject("TitleBarDesc");

		return newWidget;
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

	float TitleBar::getPadding(Padding p)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","Label::getPadding");

		return mDesc->padding[p];
	}

	Ogre::UTFString TitleBar::getText()
	{
		return mText->getText();
	}

	void TitleBar::onCloseButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			mWindow->setVisible(false);
	}

	void TitleBar::onDraw()
	{
		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

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

		redraw();
	}

	void TitleBar::setFont(const Ogre::String& fontName, unsigned int index)
	{
		mText->setFont(fontName,index);

		redraw();
	}

	void TitleBar::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setFont(fontName,startIndex,endIndex);

		redraw();
	}

	void TitleBar::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setFont(fontName,c,allOccurrences);

		redraw();
	}

	void TitleBar::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setFont(fontName,s,allOccurrences);

		redraw();
	}

	void TitleBar::setPadding(Padding p, float distance)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","Label::setPadding");

		mDesc->padding[p] = distance;
		mText->setAllottedWidth(mWidgetDesc->dimensions.size.width - (mDesc->padding[PADDING_LEFT] + mDesc->padding[PADDING_RIGHT]));

		redraw();
	}

	void TitleBar::setResizable(bool resizable)
	{
		Widget::setResizable(resizable);
	}

	void TitleBar::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		redraw();
	}
}
