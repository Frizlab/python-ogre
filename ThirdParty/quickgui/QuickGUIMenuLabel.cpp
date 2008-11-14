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
		verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;

		textDesc.horizontalTextAlignment = TEXT_ALIGNMENT_HORIZONTAL_LEFT;
	}

	void MenuLabelDesc::serialize(SerialBase* b)
	{
		MenuItemDesc::serialize(b);

		b->IO("VerticalTextAlignment",&verticalTextAlignment);

		textDesc.serialize(b);
	}

	MenuLabel::MenuLabel(const Ogre::String& name) :
		MenuItem(name),
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

		setSkinType(d->skinTypeName);

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
		mDesc->textDesc.allottedWidth = mld->dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));

		if(mText != NULL)
			delete mText;

		mDesc->verticalTextAlignment = mld->verticalTextAlignment;
		mText = new Text(mDesc->textDesc);
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

	float MenuLabel::getVerticalLineSpacing()
	{
		return mText->getVerticalLineSpacing();
	}

	VerticalTextAlignment MenuLabel::getVerticalTextAlignment()
	{
		return mDesc->verticalTextAlignment;
	}

	void MenuLabel::onDraw()
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

		mSkinElementName = OVER;

		redraw();
	}

	void MenuLabel::onMouseLeave(const EventArgs& args)
	{
		mSkinElementName = DEFAULT;

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

	void MenuLabel::setVerticalTextAlignment(VerticalTextAlignment a)
	{
		mDesc->verticalTextAlignment = a;

		redraw();
	}
}
