#include "QuickGUILabel.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Label::Label(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm),
		mDefaultTexture(mTextureName),
		mVerticalAlignment(QGUI_VA_MID),
		mHorizontalAlignment(QGUI_HA_MID),
		mTextBoundsRelativeOffset(Point::ZERO),
		mTextBoundsRelativeSize(Size(1,1)),
		mTextBoundsAbsoluteDimensions(mAbsoluteDimensions),
		mTextColor(Ogre::ColourValue::White),
		mDisabledTextColor(Ogre::ColourValue(0.75,0.75,0.75,1))
	{
		mText = new Text(mInstanceName+".Text",mQuadContainer,this);

		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_LABEL)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
			mText->setLayer(Quad::LAYER_CHILD);
		}		
	}

	Label::~Label()
	{
		delete mText;
	}

	void Label::alignText()
	{
		//return;
		Rect textDimensions = mText->getAbsoluteDimensions();
		// 1 pixel buffer used
		//Ogre::Real horizontalBuffer = 1.0 / mPixelDimensions.width;
		//Ogre::Real verticalBuffer = 1.0 / mPixelDimensions.height;
		Ogre::Real horizontalBuffer = 1.0 / static_cast<float>(mGUIManager->getViewportWidth());
		Ogre::Real verticalBuffer = 1.0 / static_cast<float>(mGUIManager->getViewportHeight());

		// Horizontal alignment
		if( mHorizontalAlignment == QGUI_HA_LEFT )
			mText->setPosition(Point(mTextBoundsAbsoluteDimensions.x + horizontalBuffer,textDimensions.y));
		else if( mHorizontalAlignment == QGUI_HA_MID )
			mText->setPosition(Point(mTextBoundsAbsoluteDimensions.x + ((mTextBoundsAbsoluteDimensions.width / 2.0) - (textDimensions.width / 2.0)),textDimensions.y));
		else if( mHorizontalAlignment == QGUI_HA_RIGHT )
			mText->setPosition(Point(mTextBoundsAbsoluteDimensions.x + mTextBoundsAbsoluteDimensions.width - (horizontalBuffer + textDimensions.width),textDimensions.y));

		textDimensions = mText->getAbsoluteDimensions();

		// Vertical alignment
		if( mVerticalAlignment == QGUI_VA_TOP )
			mText->setPosition(Point(textDimensions.x,mTextBoundsAbsoluteDimensions.y + verticalBuffer));
		else if( mVerticalAlignment == QGUI_VA_MID )
			mText->setPosition(Point(textDimensions.x,mTextBoundsAbsoluteDimensions.y + ((mTextBoundsAbsoluteDimensions.height / 2.0) - (textDimensions.height / 2.0))));
		else if( mVerticalAlignment == QGUI_VA_BOTTOM )
			mText->setPosition(Point(textDimensions.x,mTextBoundsAbsoluteDimensions.y + mTextBoundsAbsoluteDimensions.height - (verticalBuffer + textDimensions.height)));
	}

	void Label::appearOverWidget(Widget* w)
	{
		Image::appearOverWidget(w);
		mText->setOffset(mOffset + 1);
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
		return mTextBoundsAbsoluteDimensions;
	}

	void Label::hide()
	{
		Image::hide();
		mText->hide();
	}

	void Label::onPositionChanged(const EventArgs& args)
	{
		Image::onPositionChanged(args);

		// derive TextBounds according to new position.
		mTextBoundsAbsoluteDimensions.x = mAbsoluteDimensions.x + (mTextBoundsRelativeOffset.x * mAbsoluteDimensions.width);
		mTextBoundsAbsoluteDimensions.y = mAbsoluteDimensions.y + (mTextBoundsRelativeOffset.y * mAbsoluteDimensions.height);

		mText->refresh();
		alignText();
	}

	void Label::onSizeChanged(const EventArgs& args)
	{
		// derive TextBounds according to new size.		
		mTextBoundsAbsoluteDimensions.width = mAbsoluteDimensions.width * mTextBoundsRelativeSize.width;
		mTextBoundsAbsoluteDimensions.height = mAbsoluteDimensions.height * mTextBoundsRelativeSize.height;

		mText->refresh();
		alignText();
	}

	void Label::setVerticalAlignment(VerticalAlignment va)
	{
		mVerticalAlignment = va;

		if(mText->getAbsoluteDimensions() != Rect::ZERO)
			alignText();
	}

	void Label::setHorizontalAlignment(HorizontalAlignment ha)
	{
		mHorizontalAlignment = ha;

		if(mText->getAbsoluteDimensions() != Rect::ZERO)
			alignText();
	}

	void Label::setPosition(const Ogre::Real& xVal, const Ogre::Real& yVal, GuiMetricsMode mode)
	{
		Image::setPosition(xVal,yVal,mode);

		mTextBoundsAbsoluteDimensions.x = mAbsoluteDimensions.x + (mTextBoundsRelativeOffset.x * mAbsoluteDimensions.width);
		mTextBoundsAbsoluteDimensions.y = mAbsoluteDimensions.y + (mTextBoundsRelativeOffset.y * mAbsoluteDimensions.height);

		mText->refresh();
		alignText();
	}

	void Label::setPosition(Point p, GuiMetricsMode mode)
	{
		Label::setPosition(p.x,p.y,mode);
	}

	void Label::setSize(const Ogre::Real& width, const Ogre::Real& height, GuiMetricsMode mode)
	{
		Image::setSize(width,height,mode);

		mTextBoundsAbsoluteDimensions.width = mAbsoluteDimensions.width * mTextBoundsRelativeSize.width;
		mTextBoundsAbsoluteDimensions.height = mAbsoluteDimensions.height * mTextBoundsRelativeSize.height;

		mText->refresh();
		alignText();
	}

	void Label::setSize(const Size& s, GuiMetricsMode mode)
	{
		Label::setSize(s.width,s.height,mode);
	}

	void Label::setCaption(const Ogre::UTFString& caption)
	{
		mText->setCaption(caption);
	}

	void Label::setClippingRect(const Rect& r)
	{
		Image::setClippingRect(r);
		mText->setClippingRect(r);
	}

	void Label::setDisabledTextColor(const Ogre::ColourValue& c)
	{
		mDisabledTextColor = c;

		if(!mEnabled)
			mText->setColor(c);
	}

	void Label::setTextBounds(const Point& relativeOffset, const Size& relativeSize)
	{
		mTextBoundsRelativeOffset = relativeOffset;
		mTextBoundsRelativeSize = relativeSize;

		// derive TextBounds according to new dimensions.
		mTextBoundsAbsoluteDimensions.x = mAbsoluteDimensions.x + (mTextBoundsRelativeOffset.x * mAbsoluteDimensions.width);
		mTextBoundsAbsoluteDimensions.y = mAbsoluteDimensions.y + (mTextBoundsRelativeOffset.y * mAbsoluteDimensions.height);
		mTextBoundsAbsoluteDimensions.width = mAbsoluteDimensions.width * mTextBoundsRelativeSize.width;
		mTextBoundsAbsoluteDimensions.height = mAbsoluteDimensions.height * mTextBoundsRelativeSize.height;

		alignText();
	}

	void Label::show()
	{
		Image::show();
		mText->show();
	}
}
