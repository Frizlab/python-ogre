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

#ifndef __PARSER_H__
#define __PARSER_H__

#include "Utils.h"
#include <vector>
#include <map>
using namespace Ogre;

namespace CanvasNS
{
class VariantValue
{
	std::string value;
public:
	VariantValue();
	VariantValue(const std::string& value);

	const std::string& getString() const;

	int getInteger() const;

	Ogre::Real getReal() const;

	Ogre::ColourValue getColor() const;

	void tokenize(std::vector<std::string>& result) const;
};

class PropertyMap
{
	std::map<std::string, VariantValue> internalMap;
public:
	PropertyMap();

	size_t size() const;

	bool hasProperty(const std::string& name) const;

	const VariantValue& operator[](const std::string& name) const;

	VariantValue& operator[](const std::string &name);
};

typedef std::multimap<std::string, PropertyMap> Definitions;

void parse(Definitions& result, const std::string& filename, const std::string& resourceGroup);
}
#endif