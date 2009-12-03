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

#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <sstream>
#include <typeinfo>
#include "OgreColourValue.h"
using namespace Ogre;

namespace CanvasNS
{
template<class NumberType>
inline NumberType toNumber(const std::string &numberString)
{
	static std::istringstream converter;
	converter.clear();
	converter.str(numberString);
	
	if(typeid(NumberType)==typeid(bool))
	{
		int result;
		return (converter >> result).fail() ? false : !!result;
	}

	NumberType result;
	return (converter >> result).fail() ? 0 : result;
}

inline std::string trim(std::string source)
{
	std::string result = source.erase(source.find_last_not_of(" \t") + 1);
	return result.erase(0, result.find_first_not_of(" \t"));
}

inline Ogre::ColourValue parseHexColor(const std::string& str)
{
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned char A = 0;

	if(str.length() == 7 || str.length() == 9)
	{
		if(str.find("#") == 0 && str.substr(1).find_first_not_of("0123456789ABCDEFabcdef") == std::string::npos)
		{
			R = (unsigned char)strtoul(str.substr(1, 2).c_str(), 0, 16);
			G = (unsigned char)strtoul(str.substr(3, 2).c_str(), 0, 16);
			B = (unsigned char)strtoul(str.substr(5, 2).c_str(), 0, 16);
			A = str.length() == 7 ? 255 : (unsigned char)strtoul(str.substr(7, 2).c_str(), 0, 16);
		}
	}

	return Ogre::ColourValue(R / 255.0, G / 255.0, B / 255.0, A / 255.0);
}

inline void split(std::vector<std::string>& result, const std::string& source, const std::string& delimiter, bool ignoreEmpty)
{
	result.clear();

	size_t idxA = 0;
	size_t idxB = source.find(delimiter);
	std::string tempStr;
	bool done = false;

	while(!done)
	{
		if(idxB != std::string::npos)
		{
			tempStr = source.substr(idxA, idxB-idxA);
			idxA = idxB + delimiter.length();
			idxB = source.find(delimiter, idxA);
		}
		else
		{
			tempStr = source.substr(idxA);
			done = true;
		}

		if(!(ignoreEmpty && tempStr.empty()))
			result.push_back(trim(tempStr));
	}
}
}
#endif
