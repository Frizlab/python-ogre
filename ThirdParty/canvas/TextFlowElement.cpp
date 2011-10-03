/*
	This file is part of the Canvas Demo.

	Copyright (C) 2008 Adam J. Simmons
	ajs15822@gmail.com

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "TextFlowElement.h"
#include "Document.h"
using namespace Ogre;

namespace CanvasNS
{
FontState::FontState()
: fontSize(0), color(Ogre::ColourValue::ZERO)
{

}

FontState::FontState(const Ogre::String& fontFace, Ogre::uint fontSize, Ogre::ColourValue color)
: fontFace(fontFace), fontSize(fontSize), color(color)
{

}

TextBlock::TextBlock()
{
}

TextBlock::TextBlock(const Ogre::DisplayString& text, const FontState& font)
: text(text), font(font)
{
}

GlyphNode::GlyphNode()
{
}

GlyphNode::GlyphNode(GlyphInfo& glyphInfo, Ogre::Real posX, Ogre::Real posY, Ogre::ColourValue color, bool isBreakable)
	: glyphInfo(glyphInfo), posX(posX), posY(posY), color(color), isBreakable(isBreakable)
{
}

void GlyphNode::draw(Canvas* canvas, Ogre::Real originX, Ogre::Real originY)
{
	canvas->drawGlyph(glyphInfo, posX + originX + glyphInfo.bearingX, posY + originY - glyphInfo.bearingY, 
		glyphInfo.texInfo.width, glyphInfo.texInfo.height, color);
}

TextFlowElement::TextFlowElement(Document* document, const PropertyMap& properties) : Element(document), needsReflow(false)
{
	std::vector<std::string> tokens;

	name = properties["name"].getString();
	
	properties["offset"].tokenize(tokens);
	if(tokens.size() == 2)
		offset = std::pair<short, short>(toNumber<short>(tokens[0]), toNumber<short>(tokens[1]));
	else
		offset = std::pair<short, short>(0, 0);
	
	properties["size"].tokenize(tokens);
	if(tokens.size() == 2)
		size = std::pair<short, short>(toNumber<short>(tokens[0]), toNumber<short>(tokens[1]));
	else
		size = std::pair<short, short>(0, 0);

	std::string align = properties["alignment"].getString();
	if(align == "top-left") alignment = TopLeft;
	else if(align == "top-center") alignment = TopCenter;
	else if(align == "top-right") alignment = TopRight;
	else if(align == "left") alignment = Left;
	else if(align == "center") alignment = Center;
	else if(align == "right") alignment = Right;
	else if(align == "bottom-left") alignment = BottomLeft;
	else if(align == "bottom-center") alignment = BottomCenter;
	else if(align == "bottom-right") alignment = BottomRight;
	else alignment = TopLeft;

	properties["background-color"].tokenize(tokens);
	if(tokens.size() == 2)
		background = Fill(parseHexColor(tokens[0]), parseHexColor(tokens[1]));
	else if(tokens.size() == 1)
		background = Fill(parseHexColor(tokens[0]));

	WidthRect borderWidths(0, 0, 0, 0);
	ColorRect borderColors(Ogre::ColourValue::ZERO, Ogre::ColourValue::ZERO, Ogre::ColourValue::ZERO, Ogre::ColourValue::ZERO);

	properties["border-width"].tokenize(tokens);
	if(tokens.size() == 1)
		borderWidths = WidthRect(toNumber<int>(tokens[0]), toNumber<int>(tokens[0]), toNumber<int>(tokens[0]), toNumber<int>(tokens[0]));
	else if(tokens.size() == 2)
		borderWidths = WidthRect(toNumber<int>(tokens[1]), toNumber<int>(tokens[0]), toNumber<int>(tokens[1]), toNumber<int>(tokens[0]));
	else if(tokens.size() == 3)
		borderWidths = WidthRect(toNumber<int>(tokens[1]), toNumber<int>(tokens[0]), toNumber<int>(tokens[1]), toNumber<int>(tokens[2]));
	else if(tokens.size() == 4)
		borderWidths = WidthRect(toNumber<int>(tokens[3]), toNumber<int>(tokens[0]), toNumber<int>(tokens[1]), toNumber<int>(tokens[2]));

	properties["border-color"].tokenize(tokens);
	if(tokens.size() == 1)
		borderColors = ColorRect(parseHexColor(tokens[0]), parseHexColor(tokens[0]), parseHexColor(tokens[0]), parseHexColor(tokens[0]));
	else if(tokens.size() == 2)
		borderColors = ColorRect(parseHexColor(tokens[1]), parseHexColor(tokens[0]), parseHexColor(tokens[1]), parseHexColor(tokens[0]));
	else if(tokens.size() == 3)
		borderColors = ColorRect(parseHexColor(tokens[1]), parseHexColor(tokens[0]), parseHexColor(tokens[1]), parseHexColor(tokens[2]));
	else if(tokens.size() == 4)
		borderColors = ColorRect(parseHexColor(tokens[3]), parseHexColor(tokens[0]), parseHexColor(tokens[1]), parseHexColor(tokens[2]));

	border = Border(borderWidths, borderColors);

	properties["font"].tokenize(tokens);
	if(tokens.size() == 3)
		defaultFont = FontState(document->translateFontAlias(tokens[0]), toNumber<unsigned int>(tokens[1]), parseHexColor(tokens[2]));

	padding = properties["padding"].getInteger();

	if(properties.hasProperty("text"))
		setText(properties["text"].getString());

	document->flagAsDirty();
}

inline GlyphInfo getGlyph(const GlyphMap& glyphMap, CharCode code)
{
	GlyphMap::const_iterator i = glyphMap.find(code);
	if(i == glyphMap.end())
		return GlyphInfo();

	return i->second;
}

inline bool isBreakableChar(const CharCode& code)
{
	return(code == ' ' || code == '\n' || code == '\t' || code == ',' || code == '-');
}

void TextFlowElement::draw(Canvas* canvas, Atlas* atlas)
{
	int screenX, screenY;
	derivePosition(alignment, size.first, size.second, offset.first, offset.second, screenX, screenY);

	canvas->drawRectangle(screenX, screenY, size.first, size.second, background, border);
	canvas->setClip(screenX, screenY, screenX + size.first, screenY + size.second);

	Ogre::Real originX = screenX;
	Ogre::Real originY = screenY;

	if(!needsReflow)
	{
		for(std::vector<GlyphNode>::iterator i = layoutCache.begin(); i != layoutCache.end(); i++)
			i->draw(canvas, originX, originY);
		return;
	}

	layoutCache.clear();
	needsReflow = false;

	Ogre::Real left = padding;
	Ogre::Real top = padding;
	Ogre::Real right = size.first - padding;
	Ogre::Real bottom = size.second - padding;

	Ogre::Real x = left;
	Ogre::Real y = top;

	std::vector<GlyphNode> lineBuffer;
	std::list<GlyphNode> tempBuffer;
	Ogre::Real maxLineHeight = 0;
	bool needsNewLine = false;
	bool breakpoint = true;
	bool withinParagraph = false;

	for(std::vector<TextBlock>::iterator i = textBlocks.begin(); i != textBlocks.end(); i++)
	{
		const FontMetrics& fontMetrics = atlas->getFontMetrics(i->font.fontFace, i->font.fontSize);
		const GlyphMap& glyphMap = atlas->getGlyphMap(i->font.fontFace, i->font.fontSize);

		GlyphInfo baseMetrics = getGlyph(glyphMap, 'f');
		maxLineHeight = std::max(maxLineHeight, fontMetrics.height);
		
		for(Ogre::DisplayString::iterator j = i->text.begin(); j != i->text.end(); j++)
		{
			switch(*j)
			{
			case ' ':
				if(!(!lineBuffer.size() && withinParagraph))
					x += baseMetrics.advance;
				break;
			case '\t':
				if(!(!lineBuffer.size() && withinParagraph))
					x += baseMetrics.advance * 4;
				break;
			case '\n':
				needsNewLine = true;
				withinParagraph = false;
				break;
			default:
				{
					GlyphInfo curGlyph = getGlyph(glyphMap, *j);
					lineBuffer.push_back(GlyphNode(curGlyph, x, 0, i->font.color, breakpoint));
					x += curGlyph.advance;
					withinParagraph = true;

					if(x + fontMetrics.maxAdvance > right)
						needsNewLine = true;
				}
			}

			breakpoint = isBreakableChar(*j);

			if(needsNewLine)
			{
				if(lineBuffer.size() && *j != '\n')
				{
					// Peek at next character to determine whether we should back-track to find a breakable glyph
					Ogre::DisplayString::iterator k = j + 1;
					if(k != i->text.end())
					{
						if(!isBreakableChar(*k))
						{
							tempBuffer.clear();
							GlyphNode tempGlyph;

							// Iterate backwards through our current line until we find a breakable glyph
							while(lineBuffer.size())
							{
								tempGlyph = lineBuffer.back(); 
								tempBuffer.push_front(tempGlyph);
								lineBuffer.pop_back();

								if(tempGlyph.isBreakable)
									break;
							}

							// Rare event; we've got an unbreakable line of text, so break it anyways
							if(tempBuffer.size() && !lineBuffer.size())
							{
								lineBuffer.assign(tempBuffer.begin(), tempBuffer.end());
								tempBuffer.clear();
							}

							y += maxLineHeight;

							if(y > bottom)
								return;

							for(std::vector<GlyphNode>::iterator l = lineBuffer.begin(); l != lineBuffer.end(); l++)
							{
								l->posY = y;
								l->draw(canvas, originX, originY);
								layoutCache.push_back(*l);
							}

							lineBuffer.clear();
							x = left;
							needsNewLine = false;

							if(tempBuffer.size())
							{
								for(std::list<GlyphNode>::iterator l = tempBuffer.begin(); l != tempBuffer.end(); l++)
								{
									l->posX = x;
									x += l->glyphInfo.advance;

									lineBuffer.push_back(*l);
								}
							}

							maxLineHeight = j + 1 == i->text.end() ? 0 : fontMetrics.height;
							continue;
						}
					}
				}

				y += maxLineHeight;

				if(y > bottom)
					return;

				for(std::vector<GlyphNode>::iterator k = lineBuffer.begin(); k != lineBuffer.end(); k++)
				{
					k->posY = y;
					k->draw(canvas, originX, originY);
					layoutCache.push_back(*k);
				}
				
				lineBuffer.clear();
				maxLineHeight = j + 1 == i->text.end() ? 0 : fontMetrics.height;
				x = left;
				needsNewLine = false;
			}
		}
	}

	if(lineBuffer.size())
	{
		y += maxLineHeight;

		for(std::vector<GlyphNode>::iterator k = lineBuffer.begin(); k != lineBuffer.end(); k++)
		{
			k->posY = y;
			k->draw(canvas, originX, originY);
			layoutCache.push_back(*k);
		}
	}
}
void TextFlowElement::setOffset(short left, short top)
{
	offset.first = left;
	offset.second = top;
	document->flagAsDirty();
}

void TextFlowElement::setSize(short width, short height)
{
	size.first = width;
	size.second = height;
	needsReflow = true;
	document->flagAsDirty();
}

void TextFlowElement::setPadding(unsigned short padding)
{
	this->padding = padding;
	needsReflow = true;
	document->flagAsDirty();
}

void TextFlowElement::setBackground(Ogre::ColourValue color)
{
	background = color;
	document->flagAsDirty();
}

// Helper function for TextFlowElement::setText
inline Ogre::String trimQuotes(const Ogre::String& str)
{
	Ogre::String result;

	for(Ogre::String::const_iterator i = str.begin(); i != str.end(); i++)
	{
		if(*i != '"' && *i != '\'')
			result.push_back(*i);
	}
	
	return result;
}


void TextFlowElement::setText(const Ogre::String& markupText)
{
	Ogre::String buffer;
	std::vector<FontState> fontStack;
	fontStack.push_back(defaultFont);
	Ogre::String::const_iterator i = markupText.begin();
	const Ogre::String::const_iterator end = markupText.end();

	textBlocks.clear();
	needsReflow = true;
	document->flagAsDirty();

	while(i != end)
	{
		buffer.push_back(*i);

		if(Ogre::StringUtil::endsWith(buffer, "<font"))
		{
			Ogre::String curText = buffer.substr(0, buffer.length() - 5);
			buffer.clear();

			if(!curText.empty() && fontStack.size())				
				textBlocks.push_back(TextBlock(curText, fontStack.back()));
			
			while(true)
			{
				if(++i == end)
					return;
				else if(*i == '>')
					break;
				
				buffer.push_back(*i);
			}

			FontState newState;
			if(fontStack.size())
				newState = fontStack.back();
			
			Ogre::String tag(buffer);

			Ogre::StringUtil::trim(tag);
			Ogre::StringVector tokens = Ogre::StringUtil::split(tag, " ");

			for(Ogre::StringVector::iterator i = tokens.begin(); i != tokens.end(); i++)
			{
				size_t delimIdx = i->find('=');
				if(delimIdx == Ogre::String::npos)
					continue;

				Ogre::String key = i->substr(0, delimIdx);
				Ogre::String value = i->substr(delimIdx + 1);

				if(key == "face")
					newState.fontFace = document->translateFontAlias(trimQuotes(value));
				else if(key == "size")
					newState.fontSize = Ogre::StringConverter::parseUnsignedInt(trimQuotes(value));
				else if(key == "color")
					newState.color = parseHexColor(trimQuotes(value));
			}

			fontStack.push_back(newState);

			buffer.clear();
		}
		else if(Ogre::StringUtil::endsWith(buffer, "</font>"))
		{
			Ogre::String curText = buffer.substr(0, buffer.length() - 7);
			buffer.clear();

			if(!curText.empty() && fontStack.size())				
				textBlocks.push_back(TextBlock(curText, fontStack.back()));

			fontStack.pop_back();
		}

		i++;
	}

	if(!buffer.empty() && fontStack.size())
		textBlocks.push_back(TextBlock(buffer, fontStack.back()));
}

void TextFlowElement::addText(const Ogre::DisplayString& text, const Ogre::String& fontFace, Ogre::uint fontSize, Ogre::ColourValue color)
{
	textBlocks.push_back(TextBlock(text, FontState(fontFace, fontSize, color)));
	needsReflow = true;
	document->flagAsDirty();
}

void TextFlowElement::clear()
{
	textBlocks.clear();
	layoutCache.clear();
	needsReflow = false;
	document->flagAsDirty();
}

std::string TextFlowElement::getType() const
{
	return std::string("TextFlowElement");
}
}