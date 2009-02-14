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

#include "ImageElement.h"
#include "Document.h"
using namespace Ogre;

namespace CanvasNS
{
ImageElement::ImageElement(Document* document, const PropertyMap& properties) : Element(document)
{
	name = properties["name"].getString();

	std::vector<std::string> tokens;

	properties["offset"].tokenize(tokens);
	if(tokens.size() == 2)
		offset = std::pair<short, short>(toNumber<short>(tokens[0]), toNumber<short>(tokens[1]));
	else
		offset = std::pair<short, short>(0, 0);

	source = properties["source"].getString();

	properties["size"].tokenize(tokens);
	if(tokens.size() == 2)
	{
		size = std::pair<short, short>(toNumber<short>(tokens[0]), toNumber<short>(tokens[1]));
	}
	else
	{
		TextureInfo texInfo = document->getAtlas()->getTextureInfo(source);
		size.first = texInfo.width;
		size.second = texInfo.height;
	}

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

	document->flagAsDirty();
}

void ImageElement::draw(Canvas* canvas, Atlas* atlas)
{
	int originX, originY;
	derivePosition(alignment, size.first, size.second, offset.first, offset.second, originX, originY);

	canvas->drawRectangle(originX, originY, size.first, size.second, Fill(source));
}

std::string ImageElement::getType() const
{
	return std::string("ImageElement");
}
}