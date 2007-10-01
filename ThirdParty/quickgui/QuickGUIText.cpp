#include "QuickGUIText.h"
#include "QuickGUILabel.h"
#include "QuickGUIManager.h"

#include "OgreMaterialManager.h"

namespace QuickGUI
{
	Text::Text(const Ogre::String& name, QuadContainer* container, Label* owner) :
		mOwner(owner),
		mLayer(Quad::LAYER_CHILD),
		mName(name),
		mQuadContainer(container),
		mCaption(""),
		mPixelDimensions(Rect::ZERO),
		mLayout(LAYOUT_HORIZONTAL),
		mVisible(true),
		mSelectStart(-1),
		mSelectEnd(-1),
		mLineSpacing(1.0)
	{
		mOffset = owner->getOffset() + 1;
		mGUIManager = owner->getGUIManager();
		Rect ownerDimensions = mOwner->getDimensions();
		mPixelDimensions = Rect(ownerDimensions.x,ownerDimensions.y,0,0);

		if(mOwner->getParentPanel() != NULL)
			mClippingWidget = mOwner->getParentPanel();
		else if(mOwner->getParentWindow() != NULL)
			mClippingWidget = mOwner->getParentWindow();
		else if(mOwner->getParentSheet() != NULL)
			mClippingWidget = mOwner->getParentSheet();
		else
			mClippingWidget = mOwner;

		// Order dependent, this code must go before call to setFont, since that removes selection, which
		// affects the character background.
		mCharacterBackground = new Quad(name+".SelectionRectangle",mOwner);
		mCharacterBackground->setTexture("QuickGUI.TextSelection");
		mCharacterBackground->setColor(mBackgroundSelectColor);
		mCharacterBackground->setOffset(mOffset-1);
		mCharacterBackground->setVisible(false);
		mCharacterBackground->_notifyQuadContainer(mQuadContainer);

		// default font
		if(owner->getParentSheet() == NULL)
		{
			Ogre::FontManager* fm = Ogre::FontManager::getSingletonPtr();
			setFont(fm->getResourceIterator().getNext()->getName());
			mColor = Ogre::ColourValue::White;
		}
		else
		{
			setFont(owner->getParentSheet()->getDefaultFont());
			mColor = owner->getParentSheet()->getDefaultTextColor();
		}

		mSelectColor = getInverseColor(mColor);
		mBackgroundSelectColor = mColor;
	}

	Text::~Text()
	{
		_clearCharacters();
		delete mCharacterBackground;
	}

	void Text::_calculateDimensions()
	{
		if(mCharacters.empty())
		{
			mPixelDimensions.width = 0;
			mPixelDimensions.height = 0;
			return;
		}

		Ogre::Real minX = 9999;
		Ogre::Real minY = 9999;
		Ogre::Real maxX = 0;
		Ogre::Real maxY = 0;

		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			Point charPos = (*it)->getPosition();
			Size charSize = (*it)->getSize();
			if( charPos.x < minX )
				minX = charPos.x;
			if( (charPos.x + charSize.width) > maxX )
				maxX = charPos.x + charSize.width;
			if( charPos.y < minY )
				minY = charPos.y;
			if( (charPos.y + charSize.height) > maxY )
				maxY = charPos.y + charSize.height;
		}

		mPixelDimensions = Rect(minX,minY,maxX - minX,maxY - minY);
	}

	void Text::_clearCharacters()
	{
		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			delete (*it);
		}
		mCharacters.clear();
	}

	void Text::_clipToWidgetDimensions(Widget* w)
	{
		mClippingWidget = w;

		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			(*it)->setClippingWidget(mClippingWidget);
		}
	}

	void Text::_notifyQuadContainer(QuadContainer* container)
	{
		mQuadContainer = container;

		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			(*it)->_notifyQuadContainer(mQuadContainer);
		}
	}

	void Text::_setCaptionHorizontal(const Ogre::UTFString& text)
	{		
		int charCounter = 0;
		Rect textArea = mOwner->getTextBounds();
		Point pos(textArea.x,textArea.y);
		Ogre::UTFString::const_iterator it;
		for( it = text.begin(); it != text.end(); ++it )
		{
			Ogre::UTFString::code_point cp = it.getCharacter();
			
			if(isNewLine(cp))
			{
				pos.x = textArea.x;
				pos.y += getNewlineHeight();
				continue;
			}

			// Wrap only at a space
			if( isSpace(cp) )
			{
				Ogre::Real tempx = pos.x;
				Ogre::UTFString::code_point tempcp;
				// Find next space, or go to new line if there is no space
				// before we run out of room.
                #if defined(__x86_64__)
                for(long int i = 0; (it+i) != text.end(); i++)
                #else
                for(int i = 0; (it+i) != text.end(); i++)
				#endif
				{
					tempcp = (it+i).getCharacter();
					tempx += getGlyphSize(tempcp).width;
					if( tempx > (textArea.x + textArea.width) )
					{
						pos.x = textArea.x;
						pos.y += getNewlineHeight();
						// Ignore this space, because it is used for wrapping
						it.moveNext();
						cp = it.getCharacter(); 
						break;
					}
					if( i != 0 && isSpace(tempcp) )
						break;	// There is another space before we need a new line
				}
			}
			// check dimensions to see if wrap around should occur.
			Size size = getGlyphSize(cp);

			if( (pos.x + size.width) > (textArea.x + textArea.width) )
			{
				pos.x = textArea.x;
				pos.y += getNewlineHeight();
			}
			// Break if there isn't enough room for another line of text
			if( (pos.y + size.height) > (textArea.y + textArea.height) )
				break;

			Quad* q = new Quad(mName + ".Character." + Ogre::StringConverter::toString(charCounter),mOwner);
			q->setOffset(mOffset);
			q->setLayer(mLayer);

			// set dimensions
			q->setDimensions(Rect(pos,size));
			// update pen position
			pos.x += size.width;

			// We need to have quads that represent spaces for text indexing.  We don't need
			// to render them, however.
			if(!isspace(cp))
			{
				// set texture
				q->setTexture(mFontTexture->getName());

				// set texture coords
				q->setTextureCoordinates(mFont->getGlyphTexCoords(cp));

				// set default color
				q->setColor(mColor);

				// notify render object group
				q->_notifyQuadContainer(mQuadContainer);
			}

			mCharacters.push_back(q);
			++charCounter;
		}
	}

	void Text::_setCaptionVertical(const Ogre::UTFString& text)
	{		
		int charCounter = 0;
		Point widgetPos = mOwner->getPosition();
		Point pos = widgetPos;
		Ogre::UTFString::const_iterator it;
		for( it = text.begin(); it != text.end(); ++it )
		{
			Ogre::UTFString::code_point cp = it.getCharacter();
			
			if(isWhiteSpace(cp))
			{
				if(isSpace(cp))
					pos.y += getNewlineHeight();
				else if(isTab(cp))
					pos.y += (getNewlineHeight() * SPACES_PER_TAB);
				else if(isNewLine(cp))
				{
					pos.x = (widgetPos.x - getSpaceWidth());
					pos.y = widgetPos.y;
				}
				continue;
			}
	
			Quad* q = new Quad(mName + ".Character." + Ogre::StringConverter::toString(charCounter),mOwner);
			q->setOffset(mOffset);
			q->setLayer(mLayer);

			// derive dimensions
			Size size = getGlyphSize(cp);
			q->setDimensions(Rect(pos,size));
			// update pen position
			pos.y += size.height;

			// set texture
			q->setTexture(mFontTexture->getName());

			// set texture coords
			q->setTextureCoordinates(mFont->getGlyphTexCoords(cp));

			// notify render object group
			q->_notifyQuadContainer(mQuadContainer);

			mCharacters.push_back(q);
			++charCounter;
		}
	}

	void Text::addOnTextChangedEventHandler(MemberFunctionSlot* function)
	{
		mOnTextChangedUserEventHandlers.push_back(function);
	}

	Ogre::Real Text::calculateStringLength(const Ogre::UTFString& text)
	{
		Ogre::Real length = 0;

		float renderWindowWidth = mGUIManager->getViewportWidth();
		
		Ogre::Font::UVRect uvRect;
		unsigned int index = 0;
		while( index < text.length() )
		{
			if(isWhiteSpace(text[index]))
			{
				if(isSpace(text[index]))
					uvRect = mFont->getGlyphTexCoords('r');
				else if(isTab(text[index]))
				{
					uvRect = mFont->getGlyphTexCoords('r');
					uvRect.right = uvRect.right + ((uvRect.right - uvRect.left) * (SPACES_PER_TAB - 1));
				}
			}
			else
				uvRect = mFont->getGlyphTexCoords(text[index]);
			length += (((uvRect.right - uvRect.left) * TEXT_MULTIPLIER) * mFontTextureWidth);
			++index;
		}

		return length;
	}

	void Text::redraw()
	{
		_clearCharacters();
		clearSelection();

		// Either way we want to calculate the new dimensions, but if the caption is empty,
		// we don't want to try to align it.
		if( mCaption != "" )
		{
			if( mLayout == LAYOUT_HORIZONTAL )
				_setCaptionHorizontal(mCaption);
			else
				_setCaptionVertical(mCaption);

			_calculateDimensions();
			mOwner->alignText();
		}
		else
			_calculateDimensions();
		
		// Maintain visibility. Must be called before setting clip rect!
		if(!mVisible)
			hide();

		_clipToWidgetDimensions(mClippingWidget);
	}

	void Text::removeSelection()
	{
		if((mSelectStart == -1) || (mSelectEnd == -1))
			return;

		mCaption.erase(mSelectStart,(mSelectEnd-mSelectStart) + 1);
		clearSelection();
		setCaption(mCaption);
	}

	Rect Text::getDimensions()
	{
		return mPixelDimensions;
	}

	Ogre::UTFString Text::getCaption()
	{
		return mCaption;
	}

	Quad* Text::getCharacter(unsigned int index)
	{
		if( mCharacters.empty() )
			return NULL;
		else if(index >= mCharacters.size())
			return mCharacters[static_cast<int>(mCharacters.size())-1];

		return mCharacters[index];
	}

	Ogre::ColourValue Text::getColor()
	{
		return mColor;
	}

	Size Text::getGlyphSize(Ogre::UTFString::code_point cp)
	{
		bool tab = false;

		// Use 'r' for the space character, because its width is most similar
		if( isSpace(cp) || (tab = isTab(cp)) )
			cp = 'r';

		Ogre::Font::UVRect uvRect = mFont->getGlyphTexCoords(cp);
		float width = ((uvRect.right - uvRect.left) * mFontTextureWidth);
		float height = ((uvRect.bottom - uvRect.top) * mFontTextureHeight);

		// shrink size a little bit to increase sharpness and solve weird blurry issue.
		if(tab)
			return Size(width * SPACES_PER_TAB,height) * TEXT_MULTIPLIER;
		else
			return Size(width,height) * TEXT_MULTIPLIER;
	}

	Ogre::ColourValue Text::getInverseColor(const Ogre::ColourValue& c)
	{
		return Ogre::ColourValue(
			1.0 - c.r,
			1.0 - c.g,
			1.0 - c.b,
			c.a);
	}

	Ogre::Real Text::getLineSpacing()
	{
		return mLineSpacing;
	}

	Ogre::Real Text::getNewlineHeight()
	{
		// Take into account linespacing and the text multiplier
		return getGlyphSize('0').height * mLineSpacing * TEXT_MULTIPLIER;
	}

	int Text::getNumberOfCharacters()
	{
		return static_cast<int>(mCharacters.size());
	}

	Point Text::getPosition()
	{
		return Point(mPixelDimensions.x,mPixelDimensions.y);
	}

	Ogre::UTFString Text::getSelection()
	{
		if((mSelectStart < 0) || (mSelectEnd < 0))
			return "";

		return mCaption.substr(mSelectStart,mSelectEnd);
	}

	int Text::getSelectionEnd()
	{
		return mSelectStart;
	}

	int Text::getSelectionStart()
	{
		return mSelectEnd;
	}

	Ogre::Real Text::getSpaceWidth()
	{
		// Use 'r' for the space character, because its width is most similar
		return getGlyphSize('r').width * TEXT_MULTIPLIER;
	}

	Ogre::Real Text::getTabWidth()
	{
		return (getSpaceWidth() * SPACES_PER_TAB);
	}

	int Text::getTextIndex(const Point& pixelDimensions)
	{
		if(mCaption.length() <= 0)
				return -1;

		// check bounds
		if( pixelDimensions.x < mPixelDimensions.x )
			return 0;
		else if( pixelDimensions.x > (mPixelDimensions.x + mPixelDimensions.width) )
			return (static_cast<int>(mCaption.length()) - 1);

		int textIndex = 0;
		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			if( (*it)->isPointWithinBounds(pixelDimensions) )
				return textIndex;

			++textIndex;
		}

		// should never make it here
		return -1;
	}

	int Text::getTextCursorIndex(const Point& pixelDimensions)
	{
		if(mCaption.length() <= 0)
				return 0;

		// check bounds
		if( pixelDimensions.x < mPixelDimensions.x )
			return -1;
		else if( pixelDimensions.x > (mPixelDimensions.x + mPixelDimensions.width) )
			return (static_cast<int>(mCharacters.size()) + 1);

		int textIndex = 0;
		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			if( (*it)->isPointWithinBounds(pixelDimensions) )
			{
				float midX = (*it)->getPosition().x + ((*it)->getSize().width / 2);
				if( mPixelDimensions.x < midX )
					return textIndex;
				else return textIndex + 1;
			}

			++textIndex;
		}

		// should never make it here
		return textIndex;
	}

	int Text::getTextCursorIndex(const Rect& pixelDimensions)
	{
		if(mCaption.length() <= 0)
				return 0;

		// check bounds
		if( pixelDimensions.x < mPixelDimensions.x )
			return -1;
		else if( pixelDimensions.x > (mPixelDimensions.x + mPixelDimensions.width) )
			return (static_cast<int>(mCharacters.size()) + 1);

		int textIndex = 0;
		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			if( (*it)->getDimensions().intersectsRect(pixelDimensions) )
			{
				float midX = (*it)->getPosition().x + ((*it)->getSize().width / 2);
				if( pixelDimensions.x < midX )
					return textIndex;
				else return textIndex + 1;
			}

			++textIndex;
		}

		// should never make it here
		return textIndex;
	}

	bool Text::getVisible()
	{
		return mVisible;
	}

	void Text::hide()
	{
		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			(*it)->setVisible(false);
		}

		mVisible = false;
	}

	void Text::move(const Point& pixelDimensions)
	{
		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			Point curPos = (*it)->getPosition();
			(*it)->setPosition(Point(curPos.x + pixelDimensions.x,curPos.y + pixelDimensions.y));
		}

		mPixelDimensions.x += pixelDimensions.x;
		mPixelDimensions.y += pixelDimensions.y;
	}

	void Text::onTextChanged(const TextEventArgs& e)
	{
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnTextChangedUserEventHandlers.begin(); it != mOnTextChangedUserEventHandlers.end(); ++it )
			(*it)->execute(e);
	}

	void Text::clearSelection()
	{
		mCharacterBackground->setVisible(false);
		for(unsigned int index = 0; index < static_cast<int>(mCharacters.size()); ++index )
			mCharacters[index]->setColor(mColor);

		mSelectStart = -1;
		mSelectEnd = -1;
	}

	void Text::selectCharacters()
	{
		selectCharacters(0,static_cast<unsigned int>(mCaption.length() - 1));
	}

	void Text::selectCharacters(unsigned int startIndex, unsigned int endIndex)
	{
		if( (mCaption.length() == 0) || (startIndex >= mCaption.length()) )
			return;

		mSelectStart = startIndex;
		mSelectEnd = endIndex;

		if( endIndex >= mCaption.length() )
			mSelectEnd = static_cast<unsigned int>(mCaption.length() - 1);

		// Invert colors of selected characters, and make sure non selected characters are
		// appropriately colored.
		for(unsigned int index = 0; index < mCaption.length(); ++index )
		{
			if( (static_cast<int>(index) >= mSelectStart) && (static_cast<int>(index) <= mSelectEnd) )
				mCharacters[index]->setColor(mSelectColor);
			else
				mCharacters[index]->setColor(mColor);
		}

		// set dimensions of background selection quad.
		Rect dimensions;
		Quad* q = mCharacters[mSelectStart];
		dimensions.x = q->getPosition().x;
		dimensions.y = q->getPosition().y;
		q = mCharacters[mSelectEnd];
		dimensions.width = (q->getPosition().x + q->getSize().width) - dimensions.x;
		dimensions.height = (q->getPosition().y + q->getSize().height) - dimensions.y;
		mCharacterBackground->setDimensions(dimensions);
		mCharacterBackground->setVisible(true);
	}

	void Text::setCaption(const Ogre::UTFString& text, Layout l, Alignment a)
	{
		if(text == mCaption)
			return;

		// update Text properties
		mLayout = l;
		mCaption = text;
		mAlignment = a;

		// apply text properties
		redraw();

		TextEventArgs e(this);
		e.captionChanged = true;
		onTextChanged(e);
	}

	void Text::setFont(const Ogre::String& fontName)
	{
		mFont = static_cast<Ogre::FontPtr>(Ogre::FontManager::getSingleton().getByName(fontName));
		 
		if(mFont.isNull())
			throw Ogre::Exception( Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find font " + fontName,"Text::setFont" );

		mFont->load();
		mFontTexture = 
			static_cast<Ogre::TexturePtr>(
			Ogre::TextureManager::getSingleton().getByName(mFont->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName())
			);
		mFontTextureWidth = mFontTexture->getWidth();
		mFontTextureHeight = mFontTexture->getHeight();
		
		// update visual displaying of text
		redraw();

		TextEventArgs e(this);
		e.fontChanged = true;
		onTextChanged(e);
	}

	void Text::setLayer(Quad::Layer layer)
	{
		mLayer = layer;

		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			(*it)->setLayer(mLayer);
		}
	}

	void Text::setLineSpacing(Ogre::Real spacing)
	{
		mLineSpacing = spacing;
		redraw();
	}

	void Text::setOffset(int offset)
	{
		mOffset = offset;

		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			(*it)->setOffset(mOffset);
		}
	}

	void Text::setPosition(const Point& pixelDimensions)
	{
		Point origin(mPixelDimensions.x,mPixelDimensions.y);
		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			Point curPos = (*it)->getPosition();
			(*it)->setPosition(Point(pixelDimensions.x + (curPos.x - origin.x),pixelDimensions.y + (curPos.y - origin.y)));
		}

		mPixelDimensions.x = pixelDimensions.x;
		mPixelDimensions.y = pixelDimensions.y;
	}

	void Text::setColor(Ogre::ColourValue color)
	{
		mColor = color;

		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			(*it)->setColor(color,color);
		}

		TextEventArgs e(this);
		e.colorChanged = true;
		onTextChanged(e);
	}

	void Text::show()
	{
		std::vector<Quad*>::iterator it;
		for( it = mCharacters.begin(); it != mCharacters.end(); ++it )
		{
			(*it)->setVisible(true);
		}

		mVisible = true;
	}
}
