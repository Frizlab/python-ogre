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

#include "Element.h"
#include "Document.h"
using namespace Ogre;

namespace CanvasNS
{
Element::Element(Document* document) : document(document) 
{
}

Element::~Element()
{
}

std::string Element::getName() const
{
	return name;
}

void Element::derivePosition(short alignment, int width, int height, int offsetX, int offsetY, int& resultX, int& resultY)
{
	int viewportWidth = document->getViewport()->getActualWidth();
	int viewportHeight = document->getViewport()->getActualHeight();

	if(alignment == TopLeft || alignment == Left || alignment == BottomLeft)
		resultX = offsetX;
	else if(alignment == TopCenter || alignment == Center || alignment == BottomCenter)
		resultX = (viewportWidth / 2) - (width / 2) + offsetX;
	else if(alignment == TopRight || alignment == Right || alignment == BottomRight)
		resultX = viewportWidth - width + offsetX;

	if(alignment == TopLeft || alignment == TopCenter || alignment == TopRight)
		resultY = offsetY;
	else if(alignment == Left || alignment == Center || alignment == Right)
		resultY = (viewportHeight / 2) - (height / 2) + offsetY;
	else if(alignment == BottomLeft || alignment == BottomCenter || alignment == BottomRight)
		resultY = viewportHeight - height + offsetY;
}
}