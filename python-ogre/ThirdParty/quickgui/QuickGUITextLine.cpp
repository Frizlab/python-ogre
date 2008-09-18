#include "QuickGUITextLine.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	TextLine::TextLine(float allottedWidth) :
		mAllottedWidth(allottedWidth),
		mAvailableSpace(allottedWidth)
	{
		mLargestFont.setNull();
	}

	TextLine::~TextLine()
	{
	}

	void TextLine::_verticallyAlignCharacters()
	{
		Ogre::TexturePtr largestFontTexture = Text::getFontTexture(mLargestFont);
		float maxBaseLine = (mLargestFont->getTrueTypeMaxBearingY() >> 6);

		for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			Ogre::TexturePtr tp = Text::getFontTexture((*it)->fontPtr);
			float baseLine = ((*it)->fontPtr->getTrueTypeMaxBearingY() >> 6);

			(*it)->dimensions.position.y = maxBaseLine - baseLine;
		}
	}

	void TextLine::addCharacter(Character* c)
	{
		// Check if character can fit within the allotted width for this TextLine
		float xCoord = 0;
		if(!mCharacters.empty())
		{
			Character* lastChar = mCharacters.back();
			xCoord = lastChar->dimensions.position.x + lastChar->dimensions.size.width;
		}

		if((xCoord + c->dimensions.size.width) > mAllottedWidth)
			return;
		
		// Add the character and set its position
		mCharacters.push_back(c);
		mAvailableSpace -= c->dimensions.size.width;

		c->dimensions.position.x += xCoord;

		// Adjust Y position to match largest baseline
		if(mLargestFont.isNull() || Text::getFontHeight(c->fontPtr) > Text::getFontHeight(mLargestFont))
		{
			mLargestFont = c->fontPtr;
			_verticallyAlignCharacters();
		}
		else
		{
			Ogre::TexturePtr largestFontTexture = Text::getFontTexture(mLargestFont);
			float maxBaseLine = (mLargestFont->getTrueTypeMaxBearingY() >> 6);
			float baseLine = (c->fontPtr->getTrueTypeMaxBearingY() >> 6);

			c->dimensions.position.y = maxBaseLine - baseLine;
		}
	}

	void TextLine::draw(Point p)
	{
		mPosition = p;
		Brush* brush = Brush::getSingletonPtr();
		Ogre::String currentTexture = "TextSelection";

		// Render any quads queued for drawing.
		brush->emptyQueue();

		// Do an initial pass to draw any quads related to text highlighting
		brush->setTexture("TextSelection");
		for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			// Ignore any newline characters
			if(Text::isNewLine((*it)->codePoint))
				continue;

			if((*it)->getHighlighted())
			{
				brush->setColor((*it)->getHighlightColor());
				brush->queueRect(Rect((*it)->dimensions.position + mPosition,(*it)->dimensions.size),(*it)->uvCoords);
			}
		}

		// Do a second pass and render the glyphs
		for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if((*it)->isWhiteSpace())
				continue;

			if( currentTexture != (*it)->texturePtr->getName() )
			{
				brush->emptyQueue();
				currentTexture = (*it)->texturePtr->getName();
				brush->setTexture(currentTexture);
			}

			if((*it)->getHighlighted())
				brush->setColor((*it)->getHighlightedTextColor());
			else
				brush->setColor((*it)->colorValue);

			brush->queueRect(Rect((*it)->dimensions.position + mPosition,(*it)->dimensions.size),(*it)->uvCoords);
		}
	}

	bool TextLine::empty()
	{
		return mCharacters.empty();
	}

	float TextLine::getAvailableSpace()
	{
		return mAvailableSpace;
	}

	int TextLine::getCharacterIndexAtPosition(Point p)
	{
		int index = 0;
		for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			// Determine if the position is to the left of a given character
			if(p.x < (*it)->dimensions.position.x)
			{
				// If we make it here we know the position is to the left of this cursor.
				// If there is previous character and the position is closer to the previous character, return its index.
				if((index > 0) && (((*it)->dimensions.position.x - p.x) > ((*it)->dimensions.size.width / 2.0)))
					return (index - 1);
				else
					return index;
			}

			++index;
		}

		return (index - 1);
	}

	float TextLine::getHeight()
	{
		return Text::getFontHeight(mLargestFont);
	}

	int TextLine::getLength()
	{
		return static_cast<int>(mCharacters.size());
	}

	float TextLine::getWidth()
	{
		return ((mCharacters.back()->dimensions.position.x + mCharacters.back()->dimensions.size.width) - mCharacters.front()->dimensions.position.x);
	}
}
