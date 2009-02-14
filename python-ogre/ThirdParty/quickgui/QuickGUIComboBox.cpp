#include "QuickGUIComboBox.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUISheet.h"
#include "QuickGUIButton.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIScriptReader.h"

namespace QuickGUI
{
	const Ogre::String ComboBox::COMBOBOXITEM = "comboboxitem";
	const Ogre::String ComboBox::DEFAULT = "default";
	const Ogre::String ComboBox::DOWN = "down";
	const Ogre::String ComboBox::OVER = "over";
	const Ogre::String ComboBox::DROPDOWNBUTTON = "dropdownbutton";
	const Ogre::String ComboBox::DROPDOWNMENUPANEL = "dropdownmenupanel";

	void ComboBox::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ComboBox");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(DOWN);
		d->defineSkinElement(OVER);
		d->defineComponent(DROPDOWNBUTTON);
		d->defineComponent(DROPDOWNMENUPANEL);
		d->defineComponent(COMBOBOXITEM);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ComboBox",d);
	}

	ComboBoxDesc::ComboBoxDesc(const Ogre::String& id) :
		ContainerWidgetDesc(id)
	{
		resetToDefault();
	}

	void ComboBoxDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		combobox_dropDownButton = true;
		combobox_itemHeight = 25;
		combobox_dropDownWidth = 125;
		combobox_dropDownMaxHeight = 0;
		combobox_verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;

		containerwidget_supportScrollBars = false;
	}

	void ComboBoxDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("ItemHeight",&combobox_itemHeight);
		b->IO("DropDownButton",&combobox_dropDownButton);
		b->IO("DropDownWidth",&combobox_dropDownWidth);
		b->IO("DropDownMaxHeight",&combobox_dropDownMaxHeight);
		b->IO("VerticalTextAlignment",&combobox_verticalTextAlignment);

		textDesc.serialize(b);
	}

	ComboBox::ComboBox(const Ogre::String& name) :
		ContainerWidget(name),
		mMenuPanel(NULL),
		mSelectedItem(NULL),
		mDropDownButton(NULL),
		mAutoNameCounter(0)
	{
		mSkinElementName = DEFAULT;
	}

	ComboBox::~ComboBox()
	{
		OGRE_DELETE_T(mText,Text,Ogre::MEMCATEGORY_GENERAL);

		if(!mWidgetDesc->sheet->mDeleting)
		{
			if(mWidgetDesc->sheet != NULL)
				mWidgetDesc->sheet->removeWindow(mMenuPanel);

			OGRE_DELETE_T(mMenuPanel,MenuPanel,Ogre::MEMCATEGORY_GENERAL);
		}

		// Clean up all user defined event handlers.
		for(int index = 0; index < COMBOBOX_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mComboBoxEventHandlers[index].begin(); it != mComboBoxEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	float ComboBox::_getNextAvailableYPosition()
	{
		float maxY = 0;
		for(std::list<ListTextItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(((*it)->getPosition().y + (*it)->getSize().height) > maxY)
				maxY = ((*it)->getPosition().y + (*it)->getSize().height);
		}

		return maxY;
	}

	void ComboBox::_initialize(WidgetDesc* d)
	{
		ComboBoxDesc* cbd = dynamic_cast<ComboBoxDesc*>(d);
		cbd->containerwidget_supportScrollBars = false;

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<ComboBoxDesc*>(mWidgetDesc);

		setSkinType(cbd->widget_skinTypeName);

		setItemHeight(cbd->combobox_itemHeight);

		mDesc->combobox_dropDownButton = cbd->combobox_dropDownButton;
		mDesc->combobox_dropDownWidth = cbd->combobox_dropDownWidth;
		mDesc->combobox_dropDownMaxHeight = cbd->combobox_dropDownMaxHeight;

		// Create our Menu List Window
		MenuPanelDesc* lpd = FactoryManager::getSingleton().getDescFactory()->getInstance<MenuPanelDesc>("DefaultMenuPanelDesc");
		lpd->resetToDefault();
		lpd->widget_name = mWidgetDesc->widget_name + ".MenuPanel";
		lpd->widget_dimensions = Rect(0,0,mDesc->combobox_dropDownWidth,1);
		lpd->widget_visible = false;
		lpd->menupanel_owner = this;
		lpd->menupanel_maxHeight = mDesc->combobox_dropDownMaxHeight;
		mMenuPanel = dynamic_cast<MenuPanel*>(Widget::create("MenuPanel",lpd));

		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&ComboBox::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&ComboBox::onMouseLeave,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&ComboBox::onMouseEnter,this);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = cbd->textDesc;
		mDesc->combobox_verticalTextAlignment = cbd->combobox_verticalTextAlignment;
		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);

		mCurrentFontName = Text::getFirstAvailableFont()->getName();
		mCurrentColourValue = Ogre::ColourValue::White;

		if(mDesc->combobox_dropDownButton)
		{
			ButtonDesc* bd = FactoryManager::getSingleton().getDescFactory()->getInstance<ButtonDesc>("DefaultButtonDesc");
			bd->resetToDefault();
			bd->widget_name = mWidgetDesc->widget_name + ".Button";
			bd->widget_dimensions.size.width = mClientDimensions.size.height;
			bd->widget_dimensions.size.height = mClientDimensions.size.height;
			bd->widget_dimensions.position.x = mClientDimensions.position.x + mClientDimensions.size.width - mClientDimensions.size.height;
			bd->widget_dimensions.position.y = mClientDimensions.position.y;
			bd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

			mDropDownButton = dynamic_cast<Button*>(Widget::create("Button",bd));
			addComponent(DROPDOWNBUTTON,mDropDownButton);
			
			mDropDownButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&ComboBox::onDropDownButtonDown,this);

			mClientDimensions.size.width -= mClientDimensions.size.height;
		}

		setSkinType(mDesc->widget_skinTypeName);
	}

	void ComboBox::_setGUIManager(GUIManager* gm)
	{
		ContainerWidget::_setGUIManager(gm);

		if(mMenuPanel != NULL)
			mMenuPanel->_setGUIManager(gm);

		for(std::list<ListTextItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->_setGUIManager(gm);
	}

	void ComboBox::_setSheet(Sheet* sheet)
	{
		if(mWidgetDesc->sheet != NULL)
			mWidgetDesc->sheet->removeWindow(mMenuPanel);

		ContainerWidget::_setSheet(sheet);

		if(sheet != NULL)
			sheet->addWindow(mMenuPanel);

		for(std::list<ListTextItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->_setSheet(sheet);
	}

	void ComboBox::_updateItemPositions()
	{
		int y = 0;
		for(std::list<ListTextItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));
			y += mDesc->combobox_itemHeight;
		}
	}

	void ComboBox::addChild(Widget* w)
	{
		if(w->getClass() != "ListTextItem")
			throw Exception(Exception::ERR_INVALID_CHILD,"Cannot add non-ListTextItem to a ComboBox!","ComboBox::addChild");

		ListTextItem* i = dynamic_cast<ListTextItem*>(w);
		i->_setGUIManager(mWidgetDesc->guiManager);
		i->_setSheet(mWidgetDesc->sheet);

		// We cannot add the widget as a child, the texture position and drawing will be incorrect.
		mItems.push_back(i);
		
		// Add to the windows list of MenuItems
		mMenuPanel->addWidget(i);
	}

	void ComboBox::clearItems()
	{
		mMenuPanel->clearWidgets();
		mMenuPanel->setVisible(false);

		for(std::list<ListTextItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			OGRE_DELETE_T((*it),ListTextItem,Ogre::MEMCATEGORY_GENERAL);
		mItems.clear();

		mSelectedItem = NULL;
		mText->clearText();

		redraw();
	}

	void ComboBox::clearSelection()
	{
		mSelectedItem = NULL;
		mText->clearText();

		redraw();
	}

	ListTextItem* ComboBox::createItem(ListTextItemDesc* d)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.Item." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		// Determine position of MenuItem
		d->widget_dimensions.position.y = _getNextAvailableYPosition();
		d->widget_dimensions.size.width = mMenuPanel->getClientDimensions().size.width;
		d->widget_dimensions.size.height = mDesc->combobox_itemHeight;
		d->widget_skinTypeName = mSkinType->getComponentType(COMBOBOXITEM)->typeName;
		
		ListTextItem* newItem = dynamic_cast<ListTextItem*>(Widget::create(d->getWidgetClass(),d));
		addChild(newItem);

		updateIndices();

		return newItem;
	}

	void ComboBox::destroyItem(unsigned int index)
	{
		int count = 0;
		for(std::list<ListTextItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(count == index)
			{
				mMenuPanel->removeWidget((*it));

				OGRE_DELETE_T((*it),ListTextItem,Ogre::MEMCATEGORY_GENERAL);
				mItems.erase(it);
			}
		}

		// Update names and Indices
		updateIndices();
	}

	Widget* ComboBox::findWidget(const Ogre::String& widget_name)
	{
		Widget* w = ContainerWidget::findWidget(widget_name);
		if(w != NULL)
			return w;

		return mMenuPanel->findWidget(widget_name);
	}

	bool ComboBox::fireComboBoxEvent(ComboBoxEvent e, EventArgs& args)
	{
		if(mComboBoxEventHandlers[e].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mComboBoxEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	Ogre::String ComboBox::getClass()
	{
		return "ComboBox";
	}

	ListTextItem* ComboBox::getItem(unsigned int index)
	{
		if(index >= mItems.size())
			return NULL;

		unsigned int count = 0;
		for(std::list<ListTextItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(count == index)
				return (*it);

			++count;
		}

		return NULL;
	}

	float ComboBox::getItemHeight()
	{
		return mDesc->combobox_itemHeight;
	}

	int ComboBox::getNumberOfItems()
	{
		return static_cast<int>(mItems.size());
	}

	ListTextItem* ComboBox::getSelectedItem()
	{
		return mSelectedItem;
	}

	Ogre::UTFString ComboBox::getText()
	{
		return mText->getText();
	}

	void ComboBox::hideDropDownList()
	{
		if(mMenuPanel != NULL)
			mMenuPanel->setVisible(false);

		mSkinElementName = DEFAULT;

		redraw();
	}

	void ComboBox::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = 0;

		switch(mDesc->combobox_verticalTextAlignment)
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

	void ComboBox::onDropDownButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(!mMenuPanel->getVisible())
			{
				showDropDownList();
			}
			else
			{
				hideDropDownList();
				mSkinElementName = OVER;
			}

			redraw();
		}
	}

	void ComboBox::onMouseEnter(const EventArgs& args)
	{
		if(!mMenuPanel->getVisible())
			mSkinElementName = OVER;
		else
			mSkinElementName = DOWN;

		redraw();
	}

	void ComboBox::onMouseLeave(const EventArgs& args)
	{
		if(!mMenuPanel->getVisible())
			mSkinElementName = DEFAULT;
		else
			mSkinElementName = DOWN;

		redraw();
	}

	void ComboBox::onMouseButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(!mMenuPanel->getVisible())
			{
				mSkinElementName = DOWN;
				showDropDownList();
			}
			else
			{
				hideDropDownList();
				mSkinElementName = OVER;
			}

			redraw();
		}
	}

	void ComboBox::selectItem(const MouseEventArgs& mea)
	{
		mSelectedItem = dynamic_cast<ListTextItem*>(mea.widget);
		mText->setText(mSelectedItem->mText->getTextSegments());

		WidgetEventArgs args(this);
		fireComboBoxEvent(COMBOBOX_EVENT_SELECTION_CHANGED,args);

		hideDropDownList();

		redraw();
	}

	void ComboBox::selectItem(unsigned int index)
	{
		mSelectedItem = getItem(index);
		mText->setText(mSelectedItem->mText->getTextSegments());

		WidgetEventArgs args(this);
		fireComboBoxEvent(COMBOBOX_EVENT_SELECTION_CHANGED,args);

		redraw();
	}

	void ComboBox::serialize(SerialBase* b)
	{
		// Create Desc object if its not already created.
		_createDescObject();

		if(!mWidgetDesc->widget_serialize)
			return;

		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		b->begin("Child","widgets");
		// If we are reading, we need to create and populate widgets
		if(b->isSerialReader())
		{
			// Apply Desc properties
			_initialize(mWidgetDesc);

			std::list<ScriptDefinition*> defList = b->getCurrentDefinition()->getDefinitions();
			for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
			{
				// Create Empty Widget, supplying class name and widget name from script
				Widget* newWidget = FactoryManager::getSingleton().getWidgetFactory()->createInstance<Widget>((*it)->getType(),(*it)->getID());

				// Populate Desc object from Script Text, and initialize widget
				newWidget->serialize(b);

				// Add as child Widget
				addChild(newWidget);
			}
		}
		// If we are writing, we need to serialize children
		else
		{
			for(std::list<ListTextItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			{
				(*it)->serialize(b);
			}
		}
		b->end();

		b->end();
	}

	void ComboBox::setColor(const Ogre::ColourValue& cv)
	{
		mCurrentColourValue = cv;

		mText->setColor(cv);

		redraw();
	}

	void ComboBox::setColor(const Ogre::ColourValue& cv, unsigned int index)
	{
		mText->setColor(cv,index);

		redraw();
	}

	void ComboBox::setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setColor(cv,startIndex,endIndex);

		redraw();
	}

	void ComboBox::setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setColor(cv,c,allOccurrences);

		redraw();
	}

	void ComboBox::setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setColor(cv,s,allOccurrences);

		redraw();
	}

	void ComboBox::setDropDownWidth(float width)
	{
		mDesc->combobox_dropDownWidth = width;

		mMenuPanel->setWidth(mDesc->combobox_dropDownWidth);
	}

	void ComboBox::setFont(const Ogre::String& fontName)
	{
		mCurrentFontName = fontName;

		mText->setFont(fontName);

		redraw();
	}

	void ComboBox::setFont(const Ogre::String& fontName, unsigned int index)
	{
		mText->setFont(fontName,index);

		redraw();
	}

	void ComboBox::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setFont(fontName,startIndex,endIndex);

		redraw();
	}

	void ComboBox::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setFont(fontName,c,allOccurrences);

		redraw();
	}

	void ComboBox::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setFont(fontName,s,allOccurrences);

		redraw();
	}

	void ComboBox::setItemHeight(float height)
	{
		mDesc->combobox_itemHeight = height;

		_updateItemPositions();
	}

	void ComboBox::setSkinType(const Ogre::String type)
	{
		Widget::setSkinType(type);

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getComponentType((*it).first)->typeName);

		if(mMenuPanel != NULL)
			mMenuPanel->setSkinType(mSkinType->getComponentType(DROPDOWNMENUPANEL)->typeName);

		for(std::list<ListTextItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->setSkinType(mSkinType->getComponentType(COMBOBOXITEM)->typeName);
	}

	void ComboBox::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		redraw();
	}

	void ComboBox::setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		setText(s,Text::getFont(fontName),cv);
	}

	void ComboBox::setText(Ogre::UTFString s)
	{
		setText(s,mCurrentFontName,mCurrentColourValue);
	}

	void ComboBox::setText(std::vector<TextSegment> segments)
	{
		mText->setText(segments);

		redraw();
	}

	void ComboBox::showDropDownList()
	{
		// Determine if list should be shown above or below ComboBox
		Point p = getScreenPosition();

		if((p.y + mDesc->widget_dimensions.size.height + mMenuPanel->getHeight()) > mDesc->sheet->getHeight())
		{
			mMenuPanel->setPosition(Point(p.x,p.y - mMenuPanel->getHeight()));
		}
		else
		{
			mMenuPanel->setPosition(Point(p.x,p.y + mDesc->widget_dimensions.size.height));
		}

		mMenuPanel->setVisible(true);

		mSkinElementName = DOWN;
	}

	void ComboBox::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinType != NULL)
		{
			SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
			mClientDimensions.position.x = se->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = se->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));

			if(mDropDownButton)
				mClientDimensions.size.width = mDropDownButton->getPosition().x - se->getBorderThickness(BORDER_LEFT);
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);

		hideDropDownList();
	}

	void ComboBox::updateIndices()
	{
		unsigned int counter = 0;

		for(std::list<ListTextItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			(*it)->setIndex(counter);

			++counter;
		}
	}
}
