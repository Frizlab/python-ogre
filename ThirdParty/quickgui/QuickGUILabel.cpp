#include "QuickGUILabel.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Label::Label(const Ogre::String& name, Type type, const Rect& pixelDimensions, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,pixelDimensions,texture,container,ParentWidget,gm),
		mDefaultTexture(mTextureName),
		mVerticalAlignment(QGUI_VA_MID),
		mHorizontalAlignment(QGUI_HA_MID),
		mTextBoundsPixelOffset(Point::ZERO),
		mTextBoundsPixelSize(Size(pixelDimensions.width,pixelDimensions.height)),
		mTextColor(Ogre::ColourValue::White),
		mDisabledTextColor(Ogre::ColourValue(0.75,0.75,0.75,1))
	{
		mText = new Text(mInstanceName+".Text",mQuadContainer,this);

		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_LABEL)
		{
			mQuad->setLayer(mParentWidget->getQuad()->getLayer());
			mText->setLayer(mQuad->getLayer());
		}		
	}

	Label::~Label()
	{
		delete mText;
	}

	void Label::alignText()
	{
		//return;
		Rect textDimensions = mText->getDimensions();
		// 1 pixel buffer used
		Ogre::Real horizontalBuffer = 1.0 / mGUIManager->getViewportWidth();
		Ogre::Real verticalBuffer = 1.0 / mGUIManager->getViewportHeight();

		Rect textBounds = getTextBounds();

		// Horizontal alignment
		if( mHorizontalAlignment == QGUI_HA_LEFT )
			mText->setPosition(Point(textBounds.x + horizontalBuffer,textDimensions.y));
		else if( mHorizontalAlignment == QGUI_HA_MID )
			mText->setPosition(Point(textBounds.x + ((textBounds.width / 2.0) - (textDimensions.width / 2.0)),textDimensions.y));
		else if( mHorizontalAlignment == QGUI_HA_RIGHT )
			mText->setPosition(Point(textBounds.x + textBounds.width - (horizontalBuffer + textDimensions.width),textDimensions.y));

		textDimensions = mText->getDimensions();

		// Vertical alignment
		if( mVerticalAlignment == QGUI_VA_TOP )
			mText->setPosition(Point(textDimensions.x,textBounds.y + verticalBuffer));
		else if( mVerticalAlignment == QGUI_VA_MID )
			mText->setPosition(Point(textDimensions.x,textBounds.y + ((textBounds.height / 2.0) - (textDimensions.height / 2.0))));
		else if( mVerticalAlignment == QGUI_VA_BOTTOM )
			mText->setPosition(Point(textDimensions.x,textBounds.y + textBounds.height - (verticalBuffer + textDimensions.height)));
	}

	void Label::disable()
	{
		mTextColor = mText->getColor();
		mText->setColor(mDisabledTextColor);

		Image::disable();
	}

	void Label::enable()
	{
		mText->setColor(mTextColor);

		Image::enable();
	}

	Ogre::UTFString Label::getCaption()
	{
		return mText->getCaption();
	}

	Text* Label::getText()
	{
		return mText;
	}

	Rect Label::getTextBounds()
	{
		return Rect(getScreenPosition() + mTextBoundsPixelOffset,mTextBoundsPixelSize);
	}

	void Label::hide()
	{
		Image::hide();
		mText->hide();
	}

	void Label::onPositionChanged(const EventArgs& args)
	{
		Image::onPositionChanged(args);

		mText->refresh();
		alignText();
	}

	void Label::onSizeChanged(const EventArgs& args)
	{
		mText->refresh();
		alignText();
	}

	void Label::setVerticalAlignment(VerticalAlignment va)
	{
		mVerticalAlignment = va;

		if(mText->getDimensions() != Rect::ZERO)
			alignText();
	}

	void Label::setHorizontalAlignment(HorizontalAlignment ha)
	{
		mHorizontalAlignment = ha;

		if(mText->getDimensions() != Rect::ZERO)
			alignText();
	}

	void Label::setOffset(int offset)
	{
		Image::setOffset(offset);
		mText->setOffset(mOffset+1);
	}

	void Label::setPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		Image::setPosition(pixelX,pixelY);

		mText->refresh();
		alignText();
	}

	void Label::setPosition(Point pixelPosition)
	{
		Label::setPosition(pixelPosition.x,pixelPosition.y);
	}

	void Label::setSize(const Ogre::Real& pixelWidth, const Ogre::Real& pixelHeight)
	{
		Image::setSize(pixelWidth,pixelHeight);

		mText->refresh();
		alignText();
	}

	void Label::setSize(const Size& pixelSize)
	{
		Label::setSize(pixelSize.width,pixelSize.height);
	}

	void Label::setCaption(const Ogre::UTFString& caption)
	{
		mText->setCaption(caption);
	}

	void Label::setClippingRect(const Rect& pixelDimensions)
	{
		Image::setClippingRect(pixelDimensions);
		mText->setClippingRect(pixelDimensions);
	}

	void Label::setDisabledTextColor(const Ogre::ColourValue& c)
	{
		mDisabledTextColor = c;

		if(!mEnabled)
			mText->setColor(c);
	}

	void Label::setTextBounds(const Point& pixelOffset, const Size& pixelSize)
	{
		mTextBoundsPixelOffset = pixelOffset;
		mTextBoundsPixelSize = pixelSize;

		alignText();
	}

	void Label::show()
	{
		Image::show();
		mText->show();
	}
}
