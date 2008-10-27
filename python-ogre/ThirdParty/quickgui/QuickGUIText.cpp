#include "QuickGUIText.h"
#include "QuickGUIRoot.h"
#include "QuickGUISerialReader.h"
#include "QuickGUISerialWriter.h"
#include "QuickGUIScriptWriter.h"

namespace QuickGUI
{
	TextDesc::TextDesc() :
		BaseDesc()
	{
		allottedWidth = 100;
		textAlignment = TEXT_ALIGNMENT_LEFT;
		verticalLineSpacing = 2;
	}

	float TextDesc::getTextHeight()
	{
		float maxHeight = 0;
		float height;
		for(int index = 0; index < static_cast<int>(segments.size()); ++index)
		{
			height = Text::getFontHeight(segments[index].fontName);
			if(height > maxHeight)
				maxHeight = height;
		}

		return maxHeight;
	}

	int TextDesc::getTextLength()
	{
		int length = 0;

		for(int index = 0; index < static_cast<int>(segments.size()); ++index)
			length += segments[index].text.length();

		return length;
	}

	float TextDesc::getTextWidth()
	{
		float width = 0;
		for(int index = 0; index < static_cast<int>(segments.size()); ++index)
		{
			width += Text::getTextWidth(segments[index].fontName,segments[index].text);
		}

		return width;
	}

	void TextDesc::serialize(SerialBase* b)
	{
//		b->IO("AllottedWidth",&allottedWidth);
		b->IO("VerticalLineSpacing",&verticalLineSpacing);

		b->begin("Text","segments");
		// Read in Text Segments or Write out Text Segments
		if(b->isSerialReader())
		{
			std::list<DefinitionProperty*> propList = b->getCurrentDefinition()->getProperties();
			for(std::list<DefinitionProperty*>::iterator it = propList.begin(); it != propList.end(); ++it)
			{
				Ogre::StringVector sv = (*it)->getValues();

				TextSegment s;
				s.fontName = sv[0];
				s.color.a = Ogre::StringConverter::parseReal(sv[1]);
				s.color.b = Ogre::StringConverter::parseReal(sv[2]);
				s.color.g = Ogre::StringConverter::parseReal(sv[3]);
				s.color.r = Ogre::StringConverter::parseReal(sv[4]);

				// Its possible the Text is just " ", in which case the parsing won't catch it.
				if(static_cast<int>(sv.size()) < 6)
					s.text = " ";
				else
				{
					int index = 5;
					while(index < static_cast<int>(sv.size()))
					{
						s.text.append(sv[index]);
						++index;
					}
				}

				Text::convertTextToWhiteSpace(s.text);

				segments.push_back(s);
			}
		}
		else
		{
			for(int index = 0; index < static_cast<int>(segments.size()); ++index)
			{
				Ogre::String propName = "Segment" + Ogre::StringConverter::toString(index);
				DefinitionProperty* prop = new DefinitionProperty(propName);

				TextSegment& s = segments[index];

				prop->mValues.push_back(s.fontName);

				prop->mValues.push_back(Ogre::StringConverter::toString(s.color.a));
				prop->mValues.push_back(Ogre::StringConverter::toString(s.color.b));
				prop->mValues.push_back(Ogre::StringConverter::toString(s.color.g));
				prop->mValues.push_back(Ogre::StringConverter::toString(s.color.r));

				prop->mValues.push_back(s.text);

				b->getCurrentDefinition()->mProperties[propName] = prop;
			}
		}
		b->end();
	}

	Text::Text(TextDesc& d) :
		mTextLinesDirty(true),
		mMaskText(false),
		mMaskSymbol(42)
	{
		// Create Texture used for Highlighting if it doesn't exist.
		if(!Ogre::TextureManager::getSingleton().resourceExists("TextSelection"))
		{
			Ogre::TexturePtr tp = 
				static_cast<Ogre::TexturePtr>(Ogre::TextureManager::getSingleton().createManual("TextSelection",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,Ogre::TEX_TYPE_2D,1,1,0,Ogre::PF_B8G8R8A8)); 

			Ogre::HardwarePixelBufferSharedPtr buf = tp->getBuffer(); 
			buf->lock(Ogre::HardwareBuffer::HBL_NORMAL); 
			const Ogre::PixelBox& pb = buf->getCurrentLock(); 
			Ogre::uint8* pixelData = static_cast<Ogre::uint8*>(pb.data); 

			*pixelData++ = 255; // Blue 
			*pixelData++ = 255; // Green 
			*pixelData++ = 255; // Red 
			*pixelData++ = 127; // Alpha 

			buf->unlock(); 
		}

		mTextDesc = &d;

		// Populate Characters based on Desc object
		for(int index = 0; index < static_cast<int>(mTextDesc->segments.size()); ++index)
		{
			TextSegment s = mTextDesc->segments[index];
			mText.append(s.text);

			for(Ogre::UTFString::iterator it2 = s.text.begin(); it2 != s.text.end(); ++it2)
			{
				mCharacters.push_back(new Character((*it2),static_cast<Ogre::FontPtr>(Ogre::FontManager::getSingleton().getByName(s.fontName)),s.color));
			}
		}

		// Add Null Character to List
		mCharacters.push_back(new Character(Text::UNICODE_NEL,Text::getFont(Root::getSingleton().getDefaultFontName()),Ogre::ColourValue::ZERO));
	}

	Text::~Text()
	{
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
			delete (*it);
	}
// STATIC FUNCTIONS
	void Text::convertWhitespaceToText(Ogre::UTFString& s)
	{
		Ogre::String::size_type index = s.find('\t');
		while(index != Ogre::UTFString::npos)
		{
			s.replace(index,1,"\\t");
			index = s.find('\t');
		}
		
		index = s.find('\n');
		while(index != Ogre::UTFString::npos)
		{
			s.replace(index,1,"\\n");
			index = s.find('\n');
		}

		index = s.find('\r');
		while(index != Ogre::UTFString::npos)
		{
			s.replace(index,1,"\\n");
			index = s.find('\r');
		}
	}

	void Text::convertTextToWhiteSpace(Ogre::UTFString& s)
	{
		Ogre::String::size_type index = s.find("\\t");
		while(index != Ogre::UTFString::npos)
		{
			s.replace(index,2,"\t");
			index = s.find("\\t");
		}
		
		index = s.find("\\n");
		while(index != Ogre::UTFString::npos)
		{
			s.replace(index,2,"\n");
			index = s.find("\\n");
		}

		index = s.find("\\r");
		while(index != Ogre::UTFString::npos)
		{
			s.replace(index,2,"\n");
			index = s.find("\\r");
		}
	}

	Ogre::FontPtr Text::getFirstAvailableFont()
	{
		Ogre::ResourceManager::ResourceMapIterator rmi = Ogre::FontManager::getSingleton().getResourceIterator();
		return static_cast<Ogre::FontPtr>(rmi.getNext());
	}

	Ogre::FontPtr Text::getFont(const Ogre::String& fontName)
	{
		Ogre::FontPtr fp = static_cast<Ogre::FontPtr>(Ogre::FontManager::getSingleton().getByName(fontName));
		// Make sure the font is loaded
		fp->load();

		return fp;
	}

	float Text::getFontHeight(const Ogre::String& fontName)
	{
		return getFontHeight(getFont(fontName));
	}

	float Text::getFontHeight(Ogre::FontPtr fp)
	{
		Ogre::TexturePtr tp = getFontTexture(fp);

		Ogre::Font::UVRect uvRect = fp->getGlyphTexCoords('0');
		float height = ((uvRect.bottom - uvRect.top) * tp->getHeight());

		return height;
	}

	Ogre::TexturePtr Text::getFontTexture(const Ogre::String& fontName)
	{
		return getFontTexture(getFont(fontName));
	}

	Ogre::TexturePtr Text::getFontTexture(Ogre::FontPtr fp)
	{
		// Make sure the font is loaded
		fp->load();

		return 
			static_cast<Ogre::TexturePtr>
			(
				Ogre::TextureManager::getSingleton().getByName(fp->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName())
			);
	}

	Ogre::String Text::getFontTextureName(const Ogre::String& fontName)
	{
		return getFontTextureName(getFont(fontName));
	}

	Ogre::String Text::getFontTextureName(Ogre::FontPtr fp)
	{
		// Make sure the font is loaded
		fp->load();

		return fp->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName();
	}

	float Text::getGlyphHeight(const Ogre::String& fontName, Ogre::UTFString::code_point cp)
	{
		return getGlyphHeight(getFont(fontName),cp);
	}

	float Text::getGlyphHeight(Ogre::FontPtr fp, Ogre::UTFString::code_point cp)
	{
		Ogre::TexturePtr tp = getFontTexture(fp);

		Ogre::Font::UVRect uvRect = fp->getGlyphTexCoords(cp);

		return ((uvRect.bottom - uvRect.top) * tp->getHeight());
	}

	UVRect Text::getGlyphUVCoords(const Ogre::String& fontName, Ogre::UTFString::code_point cp)
	{
		return getGlyphUVCoords(getFont(fontName),cp);
	}

	UVRect Text::getGlyphUVCoords(Ogre::FontPtr fp, Ogre::UTFString::code_point cp)
	{
		// Make sure the font is loaded
		fp->load();

		Ogre::Font::UVRect uvRect = fp->getGlyphTexCoords(cp);

		UVRect retVal;
		retVal.left = uvRect.left;
		retVal.top = uvRect.top;
		retVal.right = uvRect.right;
		retVal.bottom = uvRect.bottom;

		return retVal;
	}

	Size Text::getGlyphSize(const Ogre::String& fontName, Ogre::UTFString::code_point cp)
	{
		return getGlyphSize(getFont(fontName),cp);
	}

	Size Text::getGlyphSize(Ogre::FontPtr fp, Ogre::UTFString::code_point cp)
	{
		Ogre::TexturePtr tp = getFontTexture(fp);

		Ogre::Font::UVRect uvRect = fp->getGlyphTexCoords(cp);
		float width = ((uvRect.right - uvRect.left) * tp->getWidth());
		float height = ((uvRect.bottom - uvRect.top) * tp->getHeight());

		return Size(width,height);
	}

	float Text::getGlyphWidth(const Ogre::String& fontName, Ogre::UTFString::code_point cp)
	{
		return getGlyphWidth(getFont(fontName),cp);
	}

	float Text::getGlyphWidth(Ogre::FontPtr fp, Ogre::UTFString::code_point cp)
	{
		Ogre::TexturePtr tp = getFontTexture(fp);

		Ogre::Font::UVRect uvRect = fp->getGlyphTexCoords(cp);
		float width = ((uvRect.right - uvRect.left) * tp->getWidth());

		return width;
	}

	float Text::getTextWidth(const Ogre::String& fontName, Ogre::UTFString s)
	{
		return getTextWidth(getFont(fontName),s);
	}

	float Text::getTextWidth(Ogre::FontPtr fp, Ogre::UTFString s)
	{
		float width = 0;
		float charWidth = 0;
		Ogre::TexturePtr tp = getFontTexture(fp);

		for(Ogre::UTFString::iterator it = s.begin(); it != s.end(); ++it)
		{
			if(isSpace((*it)))
			{
				charWidth = getGlyphSize(fp,'0').width;
			}
			else if(isTab((*it)))
			{
				charWidth = (getGlyphSize(fp,'0').width * 4);
			}
			else
			{
				Ogre::Font::UVRect uvRect = fp->getGlyphTexCoords((*it));
				charWidth = ((uvRect.right - uvRect.left) * tp->getWidth());
			}

			width += charWidth;
		}

		return width;
	}

// END STATIC FUNCTIONS

	void Text::_checkTextLines()
	{
		if(mTextLinesDirty)
		{
			_destroyTextLines();
			_createTextLines();

			mTextLinesDirty = false;
			mTextDesc->segments = getTextSegments();
		}

		for(std::vector<TextLine*>::iterator it = mTextLines.begin(); it != mTextLines.end(); ++it)
			(*it)->setMaskText(mMaskText,mMaskSymbol);
	}

	void Text::_createTextLines()
	{
		std::list<Character*>::iterator cItr = mCharacters.begin();

		TextLine* currentTextLine = new TextLine();
		mTextLines.push_back(currentTextLine);

		while(cItr != mCharacters.end())
		{
			if(Text::isNullCharacter((*cItr)->codePoint))
				break;

			if(isWhiteSpace((*cItr)->codePoint))
			{
				// If its a newline we can add it to the TextLine, since they have a width of 0.
				// We also want to create a New TextLine object.
				if(isNewLine((*cItr)->codePoint))
				{
					currentTextLine->addCharacter((*cItr));
					currentTextLine = new TextLine();
					mTextLines.push_back(currentTextLine);
					++cItr;
				}
				else
				{
					float availableSpace = mTextDesc->allottedWidth - currentTextLine->getWidth();

					// If the character doesn't fit on this TextLine, create a new TextLine and repeat the process
					if( (*cItr)->dimensions.size.width > availableSpace )
					{
						currentTextLine = new TextLine();
						mTextLines.push_back(currentTextLine);
					}
					else
					{
						currentTextLine->addCharacter((*cItr));
						++cItr;
					}
				}
			}
			else
			{
				float wordLength = 0;
				float availableSpace = mTextDesc->allottedWidth - currentTextLine->getWidth();
				std::list<Character*>::iterator endOfWord = cItr;

				// If we get one word that is longer than the allotted width, split it up into pieces that fit onto each line.
				if(currentTextLine->empty())
				{
					while( (endOfWord != mCharacters.end()) && !isWhiteSpace((*endOfWord)->codePoint) && (wordLength < availableSpace) )
					{
						wordLength += (*endOfWord)->dimensions.size.width;
						++endOfWord;
					}

					// add Characters from pointer to endOfWord - 1
					while(cItr != endOfWord)
					{
						currentTextLine->addCharacter((*cItr));
						++cItr;
					}
				}
				else
				{
					while( (endOfWord != mCharacters.end()) && !isWhiteSpace((*endOfWord)->codePoint) )
					{
						wordLength += (*endOfWord)->dimensions.size.width;
						++endOfWord;
					}

					// If the word doesn't fit on this TextLine, create a new TextLine and repeat the process
					if( wordLength > availableSpace )
					{
						currentTextLine = new TextLine();
						mTextLines.push_back(currentTextLine);
					}
					else
					{
						// add Characters from pointer to endOfWord - 1
						while(cItr != endOfWord)
						{
							currentTextLine->addCharacter((*cItr));
							++cItr;
						}
					}
				}
			}
		}

		currentTextLine->addCharacter(mCharacters.back());
	}

	void Text::_destroyTextLines()
	{
		for(std::vector<TextLine*>::iterator it = mTextLines.begin(); it != mTextLines.end(); ++it)
			delete (*it);
		mTextLines.clear();

		// TextLines manipulate character positions, so we need to make sure to reset them.
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
			(*it)->dimensions.position = Point::ZERO;
	}

	void Text::_drawTextLines(Point& position)
	{
		_checkTextLines();

		Point p = position;

		for(std::vector<TextLine*>::iterator it = mTextLines.begin(); it != mTextLines.end(); ++it)
		{
			switch(mTextDesc->textAlignment)
			{
			case TEXT_ALIGNMENT_LEFT:
				break;
			case TEXT_ALIGNMENT_RIGHT:
				p.x += mTextDesc->allottedWidth - (*it)->getWidth();
				break;
			case TEXT_ALIGNMENT_CENTER:
				p.x += ((mTextDesc->allottedWidth - (*it)->getWidth()) / 2.0);
				break;
			}

			(*it)->draw(p);

			p.y += (*it)->getHeight();
		}

		// Draw any remaining queued rects
		Brush::getSingleton().emptyQueue();
	}

	void Text::addCharacter(Character* c, unsigned int index)
	{
		if(index > static_cast<unsigned int>(getLength()))
			throw Exception(Exception::ERR_TEXT,"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " length=" + Ogre::StringConverter::toString(getLength()),"Text::addCharacter");

		// Inserts into mText
		mText.insert(index,1,c->codePoint);

		// Inserts into mCharacters
		int counter = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(counter == index)
			{
				mCharacters.insert(it,c);
				break;
			}

			++counter;
		}

		mTextLinesDirty = true;
	}

	void Text::addCharacter(Character* c)
	{
		addCharacter(c,getLength());
	}

	void Text::clearHighlights()
	{
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			(*it)->setHighlighted(false);
		}
	}

	void Text::draw(Point& position)
	{
		_drawTextLines(position);
	}

	bool Text::empty()
	{
		return mText.empty();
	}

	float Text::getAllottedWidth()
	{
		return mTextDesc->allottedWidth;
	}

	Character* Text::getCharacter(unsigned int index)
	{
		_checkTextLines();

		if(index >= static_cast<int>(mCharacters.size()))
			throw Exception(Exception::ERR_TEXT,"Index out of bounds!","Text::getCharacter");

		unsigned int count = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(count == index)
				return (*it);

			++count;
		}

		return NULL;
	}

	float Text::getCharacterYPosition(unsigned int index)
	{
		if(index >= static_cast<int>(mText.length()))
			throw Exception(Exception::ERR_TEXT,"Index out of bounds!","Text::getCharacterYPosition");

		_checkTextLines();

		int counter = 0;
		int indexCount = 0;
		float height = 0;
		while((counter < static_cast<int>(mTextLines.size())) && ((indexCount + mTextLines[counter]->getLength()) <= static_cast<int>(index)))
		{
			// Increment height
			height += mTextLines[counter]->getHeight();
			// Increment index count
			indexCount += mTextLines[counter]->getLength();
			// Increment counter to iterate through text lines
			++counter;
		}

		return height;
	}

	int Text::getCursorIndexAtPosition(Point& p)
	{
		_checkTextLines();

		int index = 0;
		int counter = 0;
		int height = 0;
		while((counter < (static_cast<int>(mTextLines.size()) - 1)) && (height < p.y))
		{
			// Increment height
			height += mTextLines[counter]->getHeight();
			// Increment index by number of characters in current line of text
			index += mTextLines[counter]->getLength();
			// Increment counter to iterate through text lines
			++counter;
		}

		return index + mTextLines[counter]->getCursorIndexAtPosition(Point(p.x,0));
	}

	int Text::getIndexOfNextWord(unsigned int index)
	{
		if(static_cast<int>(index) >= getLength())
			return getLength();

		if(mMaskText)
			return getLength();

		unsigned int count = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(count == index)
			{
				// Skip remaining non whitespace
				while((it != mCharacters.end()) && !(*it)->isWhiteSpace())
				{
					++it;
					++count;
				}

				// Skip remaining whitespace
				while((it != mCharacters.end()) && (*it)->isWhiteSpace())
				{
					++it;
					++count;
				}

				if(it == mCharacters.end())
					return getLength();
				else
					return static_cast<int>(count);
			}

			++count;
		}

		// Should never make it here.
		return -1;
	}

	int Text::getIndexOfPreviousWord(unsigned int index)
	{
		if(index == 0)
			return 0;

		if(mMaskText)
			return 0;

		if(index >= static_cast<int>(mText.length()))
			index = static_cast<int>(mText.length());

		unsigned int count = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(count == index)
			{
				--it;
				--count;

				// Skip whitespace
				while((it != mCharacters.begin()) && (*it)->isWhiteSpace())
				{
					--it;
					--count;
				}

				// Skip non whitespace
				bool skippedNonWhiteSpace = false;
				while((it != mCharacters.begin()) && !(*it)->isWhiteSpace())
				{
					skippedNonWhiteSpace = true;

					--it;
					--count;
				}

				// At this point we have either skipped white space all the way to the beginning of the text, we have
				// skipped characters to the beginning of the text, or we have ran past the beginning of a word.  
				// If we've skipped past a word and are not at the beginning, increment counter by 1.
				if(skippedNonWhiteSpace && (count != 0))
				{
					++it;
					++count;
				}

				return static_cast<int>(count);
			}

			++count;
		}

		// Should never make it here.
		return -1;
	}

	int Text::getLength()
	{
		return static_cast<int>(mText.length());
	}

	Point Text::getPositionAtCharacterIndex(unsigned int index)
	{
		// gaurd against empty text
		if(mText.empty())
			return Point::ZERO;

		// Force index to be valid for all values given
		if(index >= static_cast<unsigned int>(mCharacters.size()))
			index = static_cast<unsigned int>(mCharacters.size()) - 1;

		_checkTextLines();

		int idx = static_cast<int>(index);
		int counter = 0;
		float height = 0;
		int numChars = 0;
		while(counter < static_cast<int>(mTextLines.size()))
		{
			if((numChars + mTextLines[counter]->getLength()) > idx)
				break;

			numChars += mTextLines[counter]->getLength();

			// Increment height
			height += mTextLines[counter]->getHeight();
			// Increment numChars by number of characters in current line of text
			numChars += mTextLines[counter]->getLength();

			// Increment counter to iterate through text lines
			++counter;
		}

		// convert index into an index relative to the TextLine
		int relIndex = idx - numChars;
		Point relTextLinePos = mTextLines[counter]->getPositionAtCharacterIndex(relIndex);
		relTextLinePos.y = mTextLines[counter]->getHeight();

		return relTextLinePos;
	}

	Size Text::getSize()
	{
		_checkTextLines();

		float height = 0;
		for(std::vector<TextLine*>::iterator it = mTextLines.begin(); it != mTextLines.end(); ++it)
		{
			height = height + (*it)->getHeight() + mTextDesc->verticalLineSpacing;
		}

		height = height - mTextDesc->verticalLineSpacing;

		return Size(mTextDesc->allottedWidth,height);
	}

	Ogre::UTFString Text::getText()
	{
		return mText;
	}

	TextDesc* Text::getTextDesc()
	{
		return mTextDesc;
	}

	float Text::getTextWidth()
	{
		_checkTextLines();

		float width = 0;

		for(std::vector<TextLine*>::iterator it = mTextLines.begin(); it != mTextLines.end(); ++it)
			width += (*it)->getWidth();

		return width;
	}

	std::vector<TextSegment> Text::getTextSegments()
	{
		std::vector<TextSegment> segments;

		Ogre::String currentFont;
		Ogre::ColourValue currentColor;
		Ogre::UTFString text;

		// initialize segment properties
		currentFont = mCharacters.front()->fontPtr->getName();
		currentColor = mCharacters.front()->colorValue;
		text = "";

		// Since the last character is always a null character, we don't need to store is as a segment.
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != --(mCharacters.end()); ++it)
		{
			// If this character has a different color or font, write out the previous text segment.
			if(((*it)->fontPtr->getName() != currentFont) || ((*it)->colorValue != currentColor))
			{
				Text::convertWhitespaceToText(text);

				TextSegment s;
				s.color = currentColor;
				s.fontName = currentFont;
				s.text = text;
				segments.push_back(s);

				currentFont = (*it)->fontPtr->getName();
				currentColor = (*it)->colorValue;
				text.clear();
			}

			text.append(1,(*it)->codePoint);
		}

		// Write out the last segment
		TextSegment s;
		s.color = currentColor;
		s.fontName = currentFont;
		s.text = text;
		segments.push_back(s);

		return segments;
	}

	float Text::getVerticalLineSpacing()
	{
		return mTextDesc->verticalLineSpacing;
	}

	void Text::highlight()
	{
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			(*it)->setHighlighted(true);
		}
	}

	void Text::highlight(unsigned int index)
	{
		if(mText.empty() || (index < 0) || (static_cast<int>(index) >= getLength()))
			throw Exception(Exception::ERR_TEXT,
					"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " Text Length=" + Ogre::StringConverter::toString(getLength()),
					"Text::highlight");

		int count = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(count == index)
			{
				(*it)->setHighlighted(true);
				return;
			}
		}
	}

	void Text::highlight(unsigned int startIndex, unsigned int endIndex)
	{
		if(mText.empty())
			throw Exception(Exception::ERR_TEXT,"Indexes are out of bounds, text is empty!","Text::highlight");

		if((startIndex < 0) || (endIndex < 0) || (static_cast<int>(startIndex) >= getLength()) || (static_cast<int>(endIndex) >= getLength()) || (startIndex > endIndex))
			throw Exception(Exception::ERR_TEXT,
					"Invalid Indices! Start=" + Ogre::StringConverter::toString(startIndex) + " End=" + Ogre::StringConverter::toString(startIndex) + " Text Length=" + Ogre::StringConverter::toString(getLength()),
					"Text::highlight");

		unsigned int count = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(count > endIndex)
				return;

			if(count >= startIndex)
				(*it)->setHighlighted(true);

			++count;
		}
	}

	void Text::highlight(Ogre::UTFString::code_point c, bool allOccurrences)
	{
		Ogre::UTFString::size_type index = mText.find(c);
		if(!allOccurrences)
		{
			if(index != Ogre::UTFString::npos)
				highlight(static_cast<int>(index));
		}
		else
		{
			int count = 0;
			std::list<Character*>::iterator it = mCharacters.begin();
			while((index != Ogre::UTFString::npos) && (it != mCharacters.end()))
			{
				if(count == index)
				{
					(*it)->setHighlighted(true);
					index = mText.find(c,count + 1);
				}

				++it;
				++count;
			}
		}
	}

	void Text::highlight(Ogre::UTFString s, bool allOccurrences)
	{
		Ogre::UTFString::size_type index = mText.find(s);
		if(!allOccurrences)
		{
			if(index != Ogre::UTFString::npos)
				highlight(static_cast<unsigned int>(index),static_cast<unsigned int>(index) + static_cast<unsigned int>(s.length()));
		}
		else
		{
			int count = 0;
			int wordLength = static_cast<int>(s.length());
			// Used to color only wordLength amount of characters
			int temp = 0;
			std::list<Character*>::iterator it = mCharacters.begin();

			while((index != Ogre::UTFString::npos) && (it != mCharacters.end()))
			{
				// If we have passed start of instance s and have not colored wordLength characters,
				// color character.
				if((count >= static_cast<int>(index)) && (temp < wordLength))
				{
					(*it)->setHighlighted(true);
					++temp;
				}

				// If we passed the first instance of s within mText, search for next instance
				if(count > static_cast<int>(index + wordLength))
				{
					temp = 0;
					index = mText.find(s,count + 1);
				}

				++it;
				++count;
			}
		}
	}

	void Text::removeCharacter(unsigned int index)
	{
		if(index > static_cast<unsigned int>(getLength()))
			throw Exception(Exception::ERR_TEXT,"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " length=" + Ogre::StringConverter::toString(getLength()),"Text::removeCharacter");

		// Remove from mText
		mText.erase(index,1);

		// Remove from mTextCharacters
		int counter = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(counter == index)
			{
				delete (*it);
				mCharacters.erase(it);
				break;
			}

			++counter;
		}

		mTextLinesDirty = true;
	}

	void Text::setAllottedWidth(float pixelWidth)
	{
		if(mTextDesc->allottedWidth == pixelWidth)
			return;

		mTextDesc->allottedWidth = pixelWidth;

		mTextLinesDirty = true;
	}

	void Text::setColor(const Ogre::ColourValue& cv)
	{
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
			(*it)->colorValue = cv;
	}

	void Text::setColor(const Ogre::ColourValue& cv, unsigned int index)
	{
		if(mText.empty() || (index < 0) || (static_cast<int>(index) >= getLength()))
			throw Exception(Exception::ERR_TEXT,
					"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " Text Length=" + Ogre::StringConverter::toString(getLength()),
					"Text::setColor");

		int count = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(count == index)
			{
				(*it)->colorValue = cv;
				return;
			}

			++count;
		}
	}

	void Text::setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		if(mText.empty())
			throw Exception(Exception::ERR_TEXT,"Indexes are out of bounds, text is empty!","Text::setColor");

		if((startIndex < 0) || (endIndex < 0) || (static_cast<int>(startIndex) >= getLength()) || (static_cast<int>(endIndex) >= getLength()) || (startIndex > endIndex))
			throw Exception(Exception::ERR_TEXT,
					"Invalid Indices! Start=" + Ogre::StringConverter::toString(startIndex) + " End=" + Ogre::StringConverter::toString(startIndex) + " Text Length=" + Ogre::StringConverter::toString(getLength()),
					"Text::setColor");

		unsigned int count = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(count > endIndex)
				return;

			if(count >= startIndex)
				(*it)->colorValue = cv;

			++count;
		}
	}

	void Text::setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		Ogre::UTFString::size_type index = mText.find(c);
		if(!allOccurrences)
		{
			if(index != Ogre::UTFString::npos)
				setColor(cv,static_cast<int>(index));
		}
		else
		{
			int count = 0;
			std::list<Character*>::iterator it = mCharacters.begin();
			while((index != Ogre::UTFString::npos) && (it != mCharacters.end()))
			{
				if(count == index)
				{
					(*it)->colorValue = cv;
					index = mText.find(c,count + 1);
				}

				++it;
				++count;
			}
		}
	}

	void Text::setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		Ogre::UTFString::size_type index = mText.find(s);
		if(!allOccurrences)
		{
			if(index != Ogre::UTFString::npos)
				setColor(cv,static_cast<unsigned int>(index),static_cast<unsigned int>(index) + static_cast<int>(s.length()));
		}
		else
		{
			int count = 0;
			int wordLength = static_cast<int>(s.length());
			// Used to color only wordLength amount of characters
			int temp = 0;
			std::list<Character*>::iterator it = mCharacters.begin();

			while((index != Ogre::UTFString::npos) && (it != mCharacters.end()))
			{
				// If we have passed start of instance s and have not colored wordLength characters,
				// color character.
				if((count >= static_cast<int>(index)) && (temp < wordLength))
				{
					(*it)->colorValue = cv;
					++temp;
				}

				// If we passed the first instance of s within mText, search for next instance
				if(count > static_cast<int>(index + wordLength))
				{
					temp = 0;
					index = mText.find(s,count + 1);
				}

				++it;
				++count;
			}
		}
	}

	void Text::setFont(const Ogre::String& fontName)
	{
		Ogre::FontPtr fp = static_cast<Ogre::FontPtr>(Ogre::FontManager::getSingleton().getByName(fontName));

		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
			(*it)->setFont(fp);

		mTextLinesDirty = true;
	}

	void Text::setFont(const Ogre::String& fontName, unsigned int index)
	{
		if(mText.empty() || (index < 0) || (static_cast<int>(index) >= getLength()))
			throw Exception(Exception::ERR_TEXT,
					"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " Text Length=" + Ogre::StringConverter::toString(getLength()),
					"Text::setFont");

		Ogre::FontPtr fp = static_cast<Ogre::FontPtr>(Ogre::FontManager::getSingleton().getByName(fontName));

		int count = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(count == index)
			{
				(*it)->setFont(fp);
				break;
			}

			++count;
		}

		mTextLinesDirty = true;
	}

	void Text::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		if(mText.empty())
			throw Exception(Exception::ERR_TEXT,"Indexes are out of bounds, text is empty!","Text::setFont");

		if((startIndex < 0) || (endIndex < 0) || (static_cast<int>(startIndex) >= getLength()) || (static_cast<int>(endIndex) >= getLength()) || (startIndex > endIndex))
			throw Exception(Exception::ERR_TEXT,
					"Invalid Indices! Start=" + Ogre::StringConverter::toString(startIndex) + " End=" + Ogre::StringConverter::toString(startIndex) + " Text Length=" + Ogre::StringConverter::toString(getLength()),
					"Text::setFont");

		Ogre::FontPtr fp = static_cast<Ogre::FontPtr>(Ogre::FontManager::getSingleton().getByName(fontName));

		unsigned int count = 0;
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			if(count > endIndex)
				break;

			if(count >= startIndex)
				(*it)->setFont(fp);

			++count;
		}

		mTextLinesDirty = true;
	}

	void Text::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		Ogre::UTFString::size_type index = mText.find(c);
		if(!allOccurrences)
		{
			if(index != Ogre::UTFString::npos)
				setFont(fontName,static_cast<int>(index));
		}
		else
		{
			Ogre::FontPtr fp = static_cast<Ogre::FontPtr>(Ogre::FontManager::getSingleton().getByName(fontName));

			int count = 0;
			std::list<Character*>::iterator it = mCharacters.begin();
			while((index != Ogre::UTFString::npos) && (it != mCharacters.end()))
			{
				if(count == index)
				{
					(*it)->setFont(fp);
					index = mText.find(c,count + 1);
				}

				++it;
				++count;
			}

			mTextLinesDirty = true;
		}
	}

	void Text::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		Ogre::UTFString::size_type index = mText.find(s);
		if(!allOccurrences)
		{
			if(index != Ogre::UTFString::npos)
				setFont(fontName,static_cast<unsigned int>(index),static_cast<unsigned int>(index) + static_cast<unsigned int>(s.length()));
		}
		else
		{
			Ogre::FontPtr fp = static_cast<Ogre::FontPtr>(Ogre::FontManager::getSingleton().getByName(fontName));

			int count = 0;
			int wordLength = static_cast<int>(s.length());
			// Used to color only wordLength amount of characters
			int temp = 0;
			std::list<Character*>::iterator it = mCharacters.begin();

			while((index != Ogre::UTFString::npos) && (it != mCharacters.end()))
			{
				// If we have passed start of instance s and have not colored wordLength characters,
				// color character.
				if((count >= static_cast<int>(index)) && (temp < wordLength))
				{
					(*it)->setFont(fp);
					++temp;
				}

				// If we passed the first instance of s within mText, search for next instance
				if(count > static_cast<int>(index + wordLength))
				{
					temp = 0;
					index = mText.find(s,count + 1);
				}

				++it;
				++count;
			}

			mTextLinesDirty = true;
		}
	}

	void Text::setMaskText(bool mask, Ogre::UTFString::code_point maskSymbol)
	{
		mMaskText = mask;
		mMaskSymbol = maskSymbol;

		_checkTextLines();
	}

	void Text::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		// set mText to s
		mText = s;

		// cleanup mTextCharacters, re-populate
		for(std::list<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
			delete (*it);
		mCharacters.clear();

		int index = 0;
		int length = static_cast<int>(mText.length());
		while(index < length)
		{
			mCharacters.push_back(new Character(mText[index],fp,cv));

			++index;
		}

		// Add Null Character to List
		mCharacters.push_back(new Character(Text::UNICODE_NEL,fp,cv));

		mTextLinesDirty = true;
	}

	void Text::setVerticalLineSpacing(float distance)
	{
		mTextDesc->verticalLineSpacing = distance;

		mTextLinesDirty = true;
	}

	void Text::update()
	{
		_checkTextLines();
	}
}
