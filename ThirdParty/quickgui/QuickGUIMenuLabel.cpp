#include "QuickGUIMenuLabel.h"
#include "QuickGUIToolBar.h"
#include "QuickGUIWidgetDescFactoryManager.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String MenuLabel::DEFAULT = "default";
	const Ogre::String MenuLabel::DOWN = "down";
	const Ogre::String MenuLabel::OVER = "over";

	void MenuLabel::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("MenuLabel");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(DOWN);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("MenuLabel",d);
	}

	MenuLabelDesc::MenuLabelDesc() :
		MenuItemDesc()
	{
		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			padding[i] = 5.0;
		}
	}

	void MenuLabelDesc::serialize(SerialBase* b)
	{
		MenuItemDesc::serialize(b);

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			b->IO(StringConverter::toString(static_cast<Padding>(i)),&padding[i]);
		}

		textDesc.serialize(b);
	}

	MenuLabel::MenuLabel(const Ogre::String& name) :
		MenuItem(name),
		mCurrentButtonState(BUTTON_STATE_DEFAULT),
		mText(NULL)
	{
		mSkinElementName = DEFAULT;
	}

	MenuLabel::~MenuLabel()
	{
		delete mText;
	}

	void MenuLabel::_initialize(WidgetDesc* d)
	{
		MenuItem::_initialize(d);

		mDesc = dynamic_cast<MenuLabelDesc*>(mWidgetDesc);

		MenuLabelDesc* mld = dynamic_cast<MenuLabelDesc*>(d);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = mld->textDesc;

		mDesc->textDesc.allottedWidth = mld->dimensions.size.width - (mld->padding[PADDING_LEFT] + mld->padding[PADDING_RIGHT]);

		if(mText != NULL)
			delete mText;

		mText = new Text(mDesc->textDesc);

		setPadding(PADDING_BOTTOM,mld->padding[PADDING_BOTTOM]);
		setPadding(PADDING_LEFT,mld->padding[PADDING_LEFT]);
		setPadding(PADDING_RIGHT,mld->padding[PADDING_RIGHT]);
		setPadding(PADDING_TOP,mld->padding[PADDING_TOP]);

		setSkinType(d->skinTypeName);
	}

	Widget* MenuLabel::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new MenuLabel(widgetName);

		newWidget->_createDescObject("MenuLabelDesc");

		return newWidget;
	}

	Ogre::String MenuLabel::getClass()
	{
		return "MenuLabel";
	}

	float MenuLabel::getPadding(Padding p)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","MenuLabel::getPadding");

		return mDesc->padding[p];
	}

	float MenuLabel::getVerticalLineSpacing()
	{
		return mText->getVerticalLineSpacing();
	}

	void MenuLabel::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		switch(mCurrentButtonState)
		{
		case BUTTON_STATE_DEFAULT:		mSkinElementName = DEFAULT;		break;
		case BUTTON_STATE_OVER:			mSkinElementName = OVER;		break;
		}

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

	void MenuLabel::onMouseButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			dynamic_cast<MenuItemDesc*>(mWidgetDesc)->toolBar->closeMenus();
		}
	}

	void MenuLabel::onMouseEnter(const EventArgs& args)
	{
		dynamic_cast<MenuItemDesc*>(mWidgetDesc)->menu->closeSubMenus();

		mCurrentButtonState = BUTTON_STATE_OVER;

		redraw();
	}

	void MenuLabel::onMouseLeave(const EventArgs& args)
	{
		mCurrentButtonState = BUTTON_STATE_DEFAULT;

		redraw();
	}

	void MenuLabel::setColor(const Ogre::ColourValue& cv)
	{
		mText->setColor(cv);

		redraw();
	}

	void MenuLabel::setColor(const Ogre::ColourValue& cv, unsigned int index)
	{
		mText->setColor(cv,index);

		redraw();
	}

	void MenuLabel::setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setColor(cv,startIndex,endIndex);

		redraw();
	}

	void MenuLabel::setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setColor(cv,c,allOccurrences);

		redraw();
	}

	void MenuLabel::setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setColor(cv,s,allOccurrences);

		redraw();
	}

	void MenuLabel::setFont(const Ogre::String& fontName)
	{
		mText->setFont(fontName);

		redraw();
	}

	void MenuLabel::setFont(const Ogre::String& fontName, unsigned int index)
	{
		mText->setFont(fontName,index);

		redraw();
	}

	void MenuLabel::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setFont(fontName,startIndex,endIndex);

		redraw();
	}

	void MenuLabel::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setFont(fontName,c,allOccurrences);

		redraw();
	}

	void MenuLabel::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setFont(fontName,s,allOccurrences);

		redraw();
	}

	void MenuLabel::setPadding(Padding p, float distance)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","MenuLabel::setPadding");

		mDesc->padding[p] = distance;
		mText->setAllottedWidth(mWidgetDesc->dimensions.size.width - (mDesc->padding[PADDING_LEFT] + mDesc->padding[PADDING_RIGHT]));

		redraw();
	}

	void MenuLabel::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		redraw();
	}

	void MenuLabel::setVerticalLineSpacing(float distance)
	{
		if(distance == mText->getVerticalLineSpacing())
			return;

		mText->setVerticalLineSpacing(distance);

		redraw();
	}
}
