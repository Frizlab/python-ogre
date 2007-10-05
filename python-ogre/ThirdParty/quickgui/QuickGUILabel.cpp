#include "QuickGUILabel.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Label::Label(const Ogre::String& name, const Rect& pixelDimensions, Ogre::String texture, GUIManager* gm) :
		Image(name,pixelDimensions,texture,gm),
		mDefaultTexture(mTextureName),
		mVerticalAlignment(VA_MID),
		mHorizontalAlignment(HA_MID),
		mTextBoundsPixelOffset(Point::ZERO),
		mTextBoundsRelativeSize(Size(1,1)),
		mTextColor(Ogre::ColourValue::White),
		mDisabledTextColor(Ogre::ColourValue(0.75,0.75,0.75,1))
	{
		mWidgetType = TYPE_LABEL;
		mText = new Text(mInstanceName+".Text",mQuadContainer,this);
		mText->setQuadLayer(mQuadLayer);	
	}

	Label::~Label()
	{
		delete mText;
	}

	void Label::alignText()
	{
		Rect textDimensions = mText->getDimensions();
		// 1 pixel buffer used
		Ogre::Real horizontalBuffer = 1.0 / mGUIManager->getViewportWidth();
		Ogre::Real verticalBuffer = 1.0 / mGUIManager->getViewportHeight();

		Rect textBounds = getTextBounds();

		// Horizontal alignment
		if( mHorizontalAlignment == HA_LEFT )
			mText->setPosition(Point(textBounds.x + horizontalBuffer,textDimensions.y));
		else if( mHorizontalAlignment == HA_MID )
			mText->setPosition(Point(textBounds.x + ((textBounds.width / 2.0) - (textDimensions.width / 2.0)),textDimensions.y));
		else if( mHorizontalAlignment == HA_RIGHT )
			mText->setPosition(Point(textBounds.x + textBounds.width - (horizontalBuffer + textDimensions.width),textDimensions.y));

		textDimensions = mText->getDimensions();

		// Vertical alignment
		if( mVerticalAlignment == VA_TOP )
			mText->setPosition(Point(textDimensions.x,textBounds.y + verticalBuffer));
		else if( mVerticalAlignment == VA_MID )
			mText->setPosition(Point(textDimensions.x,textBounds.y + ((textBounds.height / 2.0) - (textDimensions.height / 2.0))));
		else if( mVerticalAlignment == VA_BOTTOM )
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
		return Rect(getScreenPosition() + getScrollOffset() + mTextBoundsPixelOffset,mTextBoundsRelativeSize * mSize);
	}

	void Label::hide()
	{
		Image::hide();
		mText->hide();
	}

	void Label::onPositionChanged(const EventArgs& args)
	{
		Image::onPositionChanged(args);

		mText->redraw();
		alignText();
	}

	void Label::onSizeChanged(const EventArgs& args)
	{
		Image::onSizeChanged(args);
		mText->redraw();
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

	void Label::setClippingWidget(Widget* w, bool recursive)
	{
		Image::setClippingWidget(w,recursive);
		mText->_clipToWidgetDimensions(w);
	}

	void Label::setGUIManager(GUIManager* gm)
	{
		Image::setGUIManager(gm);
		mText->setGUIManager(mGUIManager);
	}

	void Label::setOffset(int offset)
	{
		Image::setOffset(offset);
		mText->setOffset(mOffset+1);
	}

	void Label::setPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		Image::setPosition(pixelX,pixelY);

		mText->redraw();
		alignText();
	}

	void Label::setPosition(Point pixelPosition)
	{
		Label::setPosition(pixelPosition.x,pixelPosition.y);
	}

	void Label::setQuadContainer(QuadContainer* container)
	{
		Image::setQuadContainer(container);
		mText->_notifyQuadContainer(mQuadContainer);
	}

	void Label::setSize(const Ogre::Real& pixelWidth, const Ogre::Real& pixelHeight)
	{
		Image::setSize(pixelWidth,pixelHeight);

		mText->redraw();
		alignText();
	}

	void Label::setSize(const Size& pixelSize)
	{
		Label::setSize(pixelSize.width,pixelSize.height);
	}

	void Label::redraw()
	{
		Image::redraw();
		alignText();
	}

	void Label::setCaption(const Ogre::UTFString& caption)
	{
		mText->setCaption(caption);
	}

	void Label::setDisabledTextColor(const Ogre::ColourValue& c)
	{
		mDisabledTextColor = c;

		if(!mEnabled)
			mText->setColor(c);
	}

	void Label::setQuadLayer(Quad::Layer l)
	{
		Image::setQuadLayer(l);
		mText->setQuadLayer(l);
	}

	void Label::setTextBounds(const Point& pixelOffset, const Size& pixelSize)
	{
		mTextBoundsPixelOffset = pixelOffset;
		mTextBoundsRelativeSize = pixelSize / mSize;

		alignText();
	}

	void Label::show()
	{
		Image::show();
		mText->show();
	}
}
