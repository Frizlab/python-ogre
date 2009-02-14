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

#ifndef __TEXTFLOWELEMENT_H__
#define __TEXTFLOWELEMENT_H__

#include "Element.h"
#include "Canvas.h"
using namespace Ogre;

namespace CanvasNS
{
struct FontState
{
	Ogre::String fontFace;
	Ogre::uint fontSize;
	Ogre::ColourValue color;

	FontState();
	FontState(const Ogre::String& fontFace, Ogre::uint fontSize, Ogre::ColourValue color);
};

struct TextBlock
{
	Ogre::DisplayString text;
	FontState font;

	TextBlock();
	TextBlock(const Ogre::DisplayString& text, const FontState& font);
};

struct GlyphNode
{
	GlyphInfo glyphInfo;
	Ogre::Real posX;
	Ogre::Real posY;
	Ogre::ColourValue color;
	bool isBreakable;

	GlyphNode();
	GlyphNode(GlyphInfo& glyphInfo, Ogre::Real posX, Ogre::Real posY, Ogre::ColourValue color, bool isBreakable);

	void draw(Canvas* canvas, Ogre::Real originX, Ogre::Real originY);
};

class TextFlowElement : public Element
{
	friend class Document;
	short alignment;
	std::pair<short, short> offset;
	std::pair<short, short> size;
	unsigned short padding;
	Fill background;
	Border border;
	FontState defaultFont;
	std::vector<TextBlock> textBlocks;
	std::vector<GlyphNode> layoutCache;
	bool needsReflow;

	TextFlowElement(Document* document, const PropertyMap& properties);
	void draw(Canvas* canvas, Atlas* atlas);
public:
	void setOffset(short left, short top);

	void setSize(short width, short height);

	void setPadding(unsigned short padding);

	void setBackground(Ogre::ColourValue color);

	void setText(const Ogre::String& markupText);

	void addText(const Ogre::DisplayString& text, const Ogre::String& fontFace, Ogre::uint fontSize, Ogre::ColourValue color);

	void clear();

	std::string getType() const;
};
}
#endif