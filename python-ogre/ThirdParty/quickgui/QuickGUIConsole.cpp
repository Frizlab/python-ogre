#include "QuickGUIConsole.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIRoot.h"
#include "QuickGUIFactoryManager.h"

namespace QuickGUI
{
	const Ogre::String Console::BACKGROUND = "background";
	const Ogre::String Console::TEXTBOX = "textbox";
	const Ogre::String Console::TEXTAREA = "textarea";

	void Console::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Console");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(TEXTBOX);
		d->defineComponent(TEXTAREA);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Console",d);
	}

	ConsoleDesc::ConsoleDesc(const Ogre::String& id) :
		ComponentWidgetDesc(id)
	{
		resetToDefault();
	}

	void ConsoleDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		console_displayAreaHorizontalAlignment = TEXT_ALIGNMENT_HORIZONTAL_LEFT;
		console_inputBoxHeight = 25;
		console_inputBoxDefaultColor = Ogre::ColourValue::White;
		console_inputBoxDefaultFontName = Root::getSingleton().getDefaultFontName();
		console_inputBoxHorizontalAlignment = TEXT_ALIGNMENT_HORIZONTAL_LEFT;
		console_layout = CONSOLE_LAYOUT_TEXT_INPUT_BOTTOM;
		console_inputBoxTextCursorDefaultSkinTypeName = "default";
	}

	void ConsoleDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);

		b->IO("DisplayAreaHorizontalAlignment",&console_displayAreaHorizontalAlignment);
		b->IO("InputBoxHeight",&console_inputBoxHeight);
		b->IO("InputBoxDefaultColor",&console_inputBoxDefaultColor);
		b->IO("InputBoxDefaultFontName",&console_inputBoxDefaultFontName);
		b->IO("InputBoxHorizontalAlignment",&console_inputBoxHorizontalAlignment);
		b->IO("InputBoxTextCursorDefaultSkinTypeName",&console_inputBoxTextCursorDefaultSkinTypeName);
		b->IO("ConsoleLayout",&console_layout);
	}

	Console::Console(const Ogre::String& name) :
		ComponentWidget(name),
		mConsoleInputHandlerSlot(NULL),
		mDesc(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	Console::~Console()
	{
		OGRE_DELETE_T(mConsoleInputHandlerSlot,ConsoleInputHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
	}

	void Console::_initialize(WidgetDesc* d)
	{
		mDesc = dynamic_cast<ConsoleDesc*>(mWidgetDesc);

		ComponentWidget::_initialize(d);

		ConsoleDesc* cd = dynamic_cast<ConsoleDesc*>(d);

		TextBoxDesc* tbd = FactoryManager::getSingleton().getDescFactory()->getInstance<TextBoxDesc>("DefaultTextBoxDesc");
		tbd->resetToDefault();
		mDesc->console_inputBoxHeight = cd->console_inputBoxHeight;
		tbd->widget_dimensions = Rect(0,0,cd->widget_dimensions.size.width,cd->console_inputBoxHeight);
		tbd->textbox_defaultColor = mDesc->console_inputBoxDefaultColor = cd->console_inputBoxDefaultColor;
		tbd->textbox_defaultFontName = mDesc->console_inputBoxDefaultFontName = cd->console_inputBoxDefaultFontName;
		tbd->textbox_textCursorDefaultSkinTypeName = mDesc->console_inputBoxTextCursorDefaultSkinTypeName = cd->console_inputBoxTextCursorDefaultSkinTypeName;
		tbd->textDesc.horizontalTextAlignment = mDesc->console_inputBoxHorizontalAlignment = cd->console_inputBoxHorizontalAlignment;
		mInputBox = dynamic_cast<TextBox*>(Widget::create("TextBox",tbd));
		addComponent(TEXTBOX,mInputBox);
		mInputBox->addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&Console::onTextSubmitted,this);

		TextAreaDesc* tad = FactoryManager::getSingleton().getDescFactory()->getInstance<TextAreaDesc>("DefaultTextAreaDesc");
		tad->resetToDefault();
		tad->widget_dimensions = Rect(0,cd->console_inputBoxHeight,cd->widget_dimensions.size.width,cd->widget_dimensions.size.height - cd->console_inputBoxHeight);
		tad->textarea_readOnly = true;
		tad->textDesc.horizontalTextAlignment = mDesc->console_displayAreaHorizontalAlignment = cd->console_displayAreaHorizontalAlignment;
		mDisplayArea = dynamic_cast<TextArea*>(Widget::create("TextArea",tad));
		addComponent(TEXTAREA,mDisplayArea);

		setSkinType(d->widget_skinTypeName);
		setLayout(cd->console_layout);
	}

	void Console::_updateComponentDimensions()
	{
		switch(mDesc->console_layout)
		{
		case CONSOLE_LAYOUT_TEXT_INPUT_BOTTOM:
			mInputBox->setPosition(Point(mClientDimensions.position.x,mClientDimensions.position.y + mClientDimensions.size.height - mDesc->console_inputBoxHeight));
			mInputBox->setWidth(mClientDimensions.size.width);
			mInputBox->setHeight(mDesc->console_inputBoxHeight);

			mDisplayArea->setPosition(mClientDimensions.position);
			mDisplayArea->setWidth(mClientDimensions.size.width);
			mDisplayArea->setHeight(mClientDimensions.size.height - mInputBox->getHeight());
			break;
		case CONSOLE_LAYOUT_TEXT_INPUT_TOP:
			mInputBox->setPosition(mClientDimensions.position);
			mInputBox->setWidth(mClientDimensions.size.width);
			mInputBox->setHeight(mDesc->console_inputBoxHeight);

			mDisplayArea->setPosition(Point(mClientDimensions.position.x,mClientDimensions.position.y + mInputBox->getHeight()));
			mDisplayArea->setWidth(mClientDimensions.size.width);
			mDisplayArea->setHeight(mClientDimensions.size.height - mInputBox->getHeight());
			break;
		}
	}

	void Console::addCharEnteredEventHandler(EventHandlerSlot* function)
	{
		mInputBox->addWidgetEventHandler(WIDGET_EVENT_CHARACTER_KEY,function);
	}

	void Console::addInputBoxText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mInputBox->addText(s,fp,cv);
	}

	void Console::addInputBoxText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		addInputBoxText(s,Text::getFont(fontName),cv);
	}

	void Console::addInputBoxText(Ogre::UTFString s)
	{
		addInputBoxText(s,mDesc->console_inputBoxDefaultFontName,mDesc->console_inputBoxDefaultColor);
	}

	void Console::addInputBoxText(std::vector<TextSegment> segments)
	{
		mInputBox->addText(segments);
	}

	void Console::clearDisplayArea()
	{
		mDisplayArea->clearText();
	}

	void Console::clearInputBox()
	{
		mInputBox->clearText();
	}

	void Console::focusInputBox()
	{
		if(mDesc->sheet != NULL)
			mDesc->sheet->setKeyboardListener(mInputBox);
	}

	Ogre::String Console::getClass()
	{
		return "Console";
	}

	HorizontalTextAlignment Console::getDisplayAreaHorizontalAlignment()
	{
		return mDesc->console_displayAreaHorizontalAlignment;
	}

	Ogre::ColourValue Console::getInputBoxDefaultColor()
	{
		return mDesc->console_inputBoxDefaultColor;
	}

	Ogre::String Console::getInputBoxDefaultFont()
	{
		return mDesc->console_inputBoxDefaultFontName;
	}

	float Console::getInputBoxHeight()
	{
		return mDesc->console_inputBoxHeight;
	}

	HorizontalTextAlignment Console::getInputBoxHorizontalAlignment()
	{
		return mDesc->console_inputBoxHorizontalAlignment;
	}

	Ogre::UTFString Console::getInputBoxText()
	{
		return mInputBox->getText();
	}

	Ogre::String Console::getInputBoxTextCursorSkinType()
	{
		return mDesc->console_inputBoxTextCursorDefaultSkinTypeName;
	}

	std::vector<TextSegment> Console::getInputBoxTextSegments()
	{
		return mInputBox->getTextSegments();
	}

	void Console::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);
		
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void Console::onTextSubmitted(const EventArgs& args)
	{
		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);

		if(kea.scancode == KC_RETURN)
		{
			if(mConsoleInputHandlerSlot != NULL)
			{
				bool addTextToDisplayArea = true;
				bool clearInputBox = true;

				mConsoleInputHandlerSlot->onTextSubmitted(this,clearInputBox,addTextToDisplayArea);

				if(addTextToDisplayArea)
					mDisplayArea->addTextLine(mInputBox->getTextSegments());
				if(clearInputBox)
					mInputBox->clearText();
			}
			else
			{
				mDisplayArea->addTextLine(mInputBox->getTextSegments());
				mInputBox->clearText();
			}
		}
	}

	void Console::setDisplayAreaHorizontalAlignment(HorizontalTextAlignment a)
	{
		mDesc->console_displayAreaHorizontalAlignment = a;

		mDisplayArea->setHorizontalAlignment(a);
	}

	void Console::setInputBoxDefaultColor(const Ogre::ColourValue& cv)
	{
		mDesc->console_inputBoxDefaultColor = cv;

		mInputBox->setDefaultColor(cv);
	}

	void Console::setInputBoxDefaultFont(const Ogre::String& fontName)
	{
		mDesc->console_inputBoxDefaultFontName = fontName;

		mInputBox->setDefaultFont(fontName);
	}

	void Console::setInputBoxHeight(float heightInPixels)
	{
		mDesc->console_inputBoxHeight = heightInPixels;

		_updateComponentDimensions();
	}

	void Console::setInputBoxHorizontalAlignment(HorizontalTextAlignment a)
	{
		mDesc->console_inputBoxHorizontalAlignment = a;

		mInputBox->setHorizontalTextAlignment(a);
	}

	void Console::setInputBoxText(Ogre::UTFString s)
	{
		setInputBoxText(s,Text::getFont(mDesc->console_inputBoxDefaultFontName),mDesc->console_inputBoxDefaultColor);
	}

	void Console::setInputBoxText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mInputBox->setText(s,fp,cv);
	}

	void Console::setInputBoxText(std::vector<TextSegment> segments)
	{
		mInputBox->setText(segments);
	}

	void Console::setInputBoxTextCursorSkinType(const Ogre::String& skinTypeName)
	{
		mDesc->console_inputBoxTextCursorDefaultSkinTypeName = skinTypeName;

		mInputBox->setTextCursorSkinType(skinTypeName);
	}

	void Console::setLayout(ConsoleLayout l)
	{
		mDesc->console_layout = l;

		_updateComponentDimensions();
	}

	void Console::setSkinType(const Ogre::String type)
	{
		ComponentWidget::setSkinType(type);

		_updateComponentDimensions();
	}
}
