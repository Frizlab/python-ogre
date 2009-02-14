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

#ifndef __IMAGEELEMENT_H__
#define __IMAGEELEMENT_H__

#include "Element.h"
using namespace Ogre;

namespace CanvasNS
{
class ImageElement : public Element
{
	friend class Document;
	short alignment;
	std::pair<short, short> offset;
	std::pair<short, short> size;
	Ogre::String source;

	ImageElement(Document* document, const PropertyMap& properties);
	void draw(Canvas* canvas, Atlas* atlas);
public:
	std::string getType() const;
};
}
#endif