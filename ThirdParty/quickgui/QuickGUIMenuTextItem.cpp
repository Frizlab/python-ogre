#include "QuickGUIMenuTextItem.h"
#include "QuickGUIToolBar.h"
#include "QuickGUIContextMenu.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIMenu.h"

namespace QuickGUI
{
	const Ogre::String MenuTextItem::DEFAULT = "default";
	const Ogre::String MenuTextItem::DOWN = "down";
	const Ogre::String MenuTextItem::OVER = "over";

	void MenuTextItem::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("MenuTextItem");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(DOWN);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("MenuTextItem",d);
	}

	MenuTextItemDesc::MenuTextItemDesc() :
		MenuItemDesc(),
		TextUserDesc()
	{
		resetToDefault();
	}

	void MenuTextItemDesc::resetToDefault()
	{
		MenuItemDesc::resetToDefault();

		TextUserDesc::resetToDefault();
		
		textDesc.horizontalTextAlignment = TEXT_ALIGNMENT_HORIZONTAL_LEFT;
	}

	void MenuTextItemDesc::serialize(SerialBase* b)
	{
		MenuItemDesc::serialize(b);

		TextUserDesc::serialize(b);
	}

	MenuTextItem::MenuTextItem(const Ogre::String& name) :
		MenuItem(name),
		TextUser()
	{
	}

	MenuTextItem::~MenuTextItem()
	{
	}

	void MenuTextItem::_initialize(WidgetDesc* d)
	{
		MenuItem::_initialize(d);

		mDesc = dynamic_cast<MenuTextItemDesc*>(mWidgetDesc);

		MenuTextItemDesc* mld = dynamic_cast<MenuTextItemDesc*>(d);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = mld->textDesc;
		mDesc->textDesc.allottedSize.height = mClientDimensions.size.height;
		TextUser::_initialize(this,mDesc);

		setSkinType(d->widget_skinTypeName);

		// Set the width of the MenuTextItem to the width of the Text by default.
		setWidth(mSkinElement->getBorderThickness(BORDER_LEFT) + getTextWidth() + mSkinElement->getBorderThickness(BORDER_RIGHT));
	}

	Ogre::String MenuTextItem::getClass()
	{
		return "MenuTextItem";
	}

	void MenuTextItem::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Clip to client dimensions
		Rect clipRegion(mClientDimensions);
		clipRegion.translate(mTexturePosition);

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		clipRegion = mClientDimensions;
		clipRegion.translate(mTexturePosition);		

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void MenuTextItem::onMouseButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			MenuItemDesc* menuItemDesc = dynamic_cast<MenuItemDesc*>(mWidgetDesc);
			
			if(menuItemDesc->toolBar != NULL)
				menuItemDesc->toolBar->closeMenus();
			else if(menuItemDesc->contextMenu != NULL)
				menuItemDesc->contextMenu->hide();
		}
	}

	void MenuTextItem::onMouseEnter(const EventArgs& args)
	{
		if(mDesc->menu != NULL)
			mDesc->menu->closeSubMenus();
		else if(mDesc->toolBar != NULL)
			mDesc->toolBar->closeMenus();
		else if(mDesc->contextMenu != NULL)
			mDesc->contextMenu->closeSubMenus();

		mSkinElement = mSkinType->getSkinElement(OVER);

		redraw();
	}

	void MenuTextItem::onMouseLeave(const EventArgs& args)
	{
		mSkinElement = mSkinType->getSkinElement(DEFAULT);

		redraw();
	}

	void MenuTextItem::updateClientDimensions()
	{
		MenuItem::updateClientDimensions();
		if(mText != NULL)
			mText->setAllottedHeight(mClientDimensions.size.height);

		redraw();
	}

	void MenuTextItem::updateSkinElement()
	{
		mSkinElement = NULL;
		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(DEFAULT);
	}
}
