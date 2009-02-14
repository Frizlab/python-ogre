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

#include "Parser.h"
#include "Ogre.h"
using namespace Ogre;

namespace CanvasNS
{
VariantValue::VariantValue()
{
}

VariantValue::VariantValue(const std::string& value) : value(value)
{
}

const std::string& VariantValue::getString() const
{
	return value;
}

int VariantValue::getInteger() const
{
	return toNumber<int>(value);
}

Ogre::Real VariantValue::getReal() const
{
	return toNumber<Ogre::Real>(value);
}

Ogre::ColourValue VariantValue::getColor() const
{
	return parseHexColor(value);
}

void VariantValue::tokenize(std::vector<std::string>& result) const
{
	split(result, value, ",", true);
}

PropertyMap::PropertyMap()
{
}

size_t PropertyMap::size() const
{
	return internalMap.size();
}

bool PropertyMap::hasProperty(const std::string& name) const
{
	std::map<std::string, VariantValue>::const_iterator i = internalMap.find(name);

	if(i != internalMap.end())
		return true;
	else
		return false;
}

const VariantValue& PropertyMap::operator[](const std::string& name) const
{
	std::map<std::string, VariantValue>::const_iterator i = internalMap.find(name);
	
	if(i != internalMap.end())
		return i->second;

	static VariantValue empty;

	return empty;
}

VariantValue& PropertyMap::operator[](const std::string &name)
{
	return internalMap[name];
}

void parse(Definitions& result, const std::string& filename, const std::string& resourceGroup)
{
	const std::string comment = "//";
	const char delimiter = ':';
	const char braceOpen = '{';
	const char braceClose = '}';
	const char multilineBegin = '[';
	const char multilineEnd = ']';
	
	bool withinDefinition = false;

	std::string line;
	std::string currentDefinition;
	std::multimap<std::string, PropertyMap>::iterator defIter;

	result.clear();

	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, resourceGroup);
	if(stream.isNull())
		OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "Could not find definitions file.", "Demo::parse");

	while(!stream->eof())
	{
		line = stream->getLine();

		// remove comments
		size_t commIdx = line.find(comment);
		if(commIdx != std::string::npos)
			line = trim(line.substr(0, commIdx));

		if(line.empty()) continue;

		if(!withinDefinition)
		{
			currentDefinition = line;
			defIter = result.insert(std::pair<std::string, PropertyMap>(currentDefinition, PropertyMap()));
			
			// seek past brace
			while(true)
			{
				if(stream->eof())
				{
					stream->close();
					return;
				}

				line = stream->getLine();
				if(line.find(braceOpen) != std::string::npos)
					break;
			}

			withinDefinition = true;
		}
		else
		{
			size_t endBraceIdx = line.find(braceClose);
			if(endBraceIdx != std::string::npos)
			{
				withinDefinition = false;
				currentDefinition.clear();
				defIter = result.end();
				continue;
			}

			size_t delimIdx = line.find(delimiter);
			if(delimIdx == std::string::npos)
				continue;

			std::string key = trim(line.substr(0, delimIdx));
			std::string value = trim(line.substr(delimIdx + 1));

			if(value.find(multilineBegin) != 0)
			{
				if(!key.empty())
					defIter->second[key] = value;
			}
			else
			{
				value = value.substr(1) + '\n';
				char curChar;

				// concatenate multiline value
				while(true)
				{
					if(stream->eof())
					{
						stream->close();
						return;
					}
					
					stream->read(&curChar, sizeof(char));
					if(curChar == multilineEnd)
					{
						if(!key.empty())
							defIter->second[key] = value;
						break;
					}
					else
					{
						value += curChar;
					}
				}
			}				
		}
	}

	stream->close();
}
}