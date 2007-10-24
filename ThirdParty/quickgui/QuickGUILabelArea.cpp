#include "QuickGUILabelArea.h"

namespace QuickGUI
{
	LabelArea::LabelArea(const Ogre::String& instanceName, Size pixelSize, Ogre::String textureName, GUIManager* gm) :
		Label(instanceName,pixelSize,textureName,gm),
		mOffset(0.0),
		mCaption("")
	{
		mWidgetType = TYPE_LABELAREA;
		mSkinComponent = ".labelarea";
		mAutoSize = false;

		mTextList = new List(mInstanceName+".TextList",mSize,"",mGUIManager);
		addChild(mTextList);
		mTextList->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		mTextList->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);

		allowScrolling(true);
	}

	LabelArea::~LabelArea()
	{
	}

	int LabelArea::_getLine(int startIndex)
	{
		int end = startIndex + 1;
		
		Ogre::Real length = 0;
		Ogre::FontPtr font = mText->getFont();
		Ogre::Real fontTextureWidth = mText->getFontTextureWidth();
		Ogre::Font::UVRect uvRect;
		while( (end < static_cast<int>(mCaption.length())) && (length <= (mSize.width - mOffset)))
		{
			if(mText->isWhiteSpace(mCaption[end]))
			{
				if(mText->isSpace(mCaption[end]))
					uvRect = font->getGlyphTexCoords('r');
				else if(mText->isTab(mCaption[end]))
				{
					uvRect = font->getGlyphTexCoords('r');
					uvRect.right = uvRect.right + ((uvRect.right - uvRect.left) * (SPACES_PER_TAB - 1));
				}
			}
			else
				uvRect = font->getGlyphTexCoords(mCaption[end]);
			length += (((uvRect.right - uvRect.left) * TEXT_MULTIPLIER) * fontTextureWidth);
			++end;
		}

		// The rest of the caption is less than the width of the Widget.
		if( end == static_cast<int>(mCaption.length()) )
			return end;
		// Remaining portion of caption is larger than the width of the widget.
		else
		{
			// Iterate backwards until we find a space.
			while( !mText->isSpace(mCaption[end]) )
				--end;
			
			return end;
		}
	}

	void LabelArea::alignText()
	{
	}

	void LabelArea::allowScrolling(bool allow)
	{
		mScrollingAllowed = allow;
		mTextList->allowScrolling(mScrollingAllowed);

		if(allow)
			mOffset = mTextList->mRightScrollBar->getWidth();
		else
			mOffset = 0.0;
	}

	void LabelArea::clearText()
	{
		setText("");
	}

	void LabelArea::disable()
	{
		mTextColor = mText->getColor();

		for( int i = 0; i < mTextList->getNumberOfItems(); ++i )
		{
			dynamic_cast<TextBox*>(mTextList->getItem(i))->setTextColor(mDisabledTextColor);
		}

		Label::disable();
	}

	void LabelArea::enable()
	{
		for( int i = 0; i < mTextList->getNumberOfItems(); ++i )
		{
			dynamic_cast<TextBox*>(mTextList->getItem(i))->setTextColor(mTextColor);
		}

		Label::enable();
	}

	Ogre::UTFString LabelArea::getText()
	{
		return mCaption;
	}

	bool LabelArea::scrollingAllowed()
	{
		return mScrollingAllowed;
	}

	void LabelArea::setDisabledTextColor(const Ogre::ColourValue& c)
	{
		mDisabledTextColor = c;

		for( int i = 0; i < mTextList->getNumberOfItems(); ++i )
		{
			dynamic_cast<TextBox*>(mTextList->getItem(i))->setDisabledTextColor(mDisabledTextColor);
		}

		if(!mEnabled)
			disable();
	}

	void LabelArea::setFont(const Ogre::String& fontScriptName, bool recursive)
	{
		Label::setFont(fontScriptName,recursive);
		setText(mCaption);
	}

	void LabelArea::setHorizontalAlignment(HorizontalAlignment ha)
	{
		mHorizontalAlignment = ha;

		for( int i = 0; i < mTextList->getNumberOfItems(); ++i )
		{
			dynamic_cast<TextBox*>(mTextList->getItem(i))->setHorizontalAlignment(mHorizontalAlignment);
		}
	}

	void LabelArea::setText(const Ogre::UTFString& text)
	{
		mCaption = text;

		// Caption Iterator
		int textIndex = 0;
		// Index denoting the end of a line that can fix within the widgets width.
		int lineEnd = 0;
		// The index of the text box to use, or create and use.
		int textBoxIndex = 0;

		while( textIndex < static_cast<int>(mCaption.size()) )
		{
			lineEnd = _getLine(textIndex);

			TextBox* tb;
			if( textBoxIndex >= mTextList->getNumberOfItems() )
			{
				tb = mTextList->addTextBox();
				tb->setTexture("");
			}
			else
				tb = dynamic_cast<TextBox*>(mTextList->getItem(textBoxIndex));

			tb->setReadOnly(true);
			tb->setUseBorders(false);
			tb->setText(mCaption.substr(textIndex,lineEnd - textIndex));
			++textBoxIndex;

			textIndex = lineEnd + 1;
		}

		// destroy remaining unused textboxes.
		int numExtraTextBoxes = mTextList->getNumberOfItems() - textBoxIndex;
		while( numExtraTextBoxes > 0 )
		{
			mTextList->removeItem(textBoxIndex);
			--numExtraTextBoxes;
		}
	}

	void LabelArea::setTextColor(Ogre::ColourValue color)
	{
		for( int i = 0; i < mTextList->getNumberOfItems(); ++i )
		{
			dynamic_cast<TextBox*>(mTextList->getItem(i))->setTextColor(color);
		}
	}

	void LabelArea::setVerticalAlignment(VerticalAlignment va)
	{
		mVerticalAlignment = va;

		for( int i = 0; i < mTextList->getNumberOfItems(); ++i )
		{
			dynamic_cast<TextBox*>(mTextList->getItem(i))->setVerticalAlignment(mVerticalAlignment);
		}
	}
}
