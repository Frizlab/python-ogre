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

#ifndef __DOCUMENT_H__
#define __DOCUMENT_H__

#include <string>
#include "Element.h"
#include "ImageElement.h"
#include "TextFlowElement.h"
#include "DancingGraphElement.h"
#include "Canvas.h"
using namespace Ogre;

namespace CanvasNS
{
class Document
{
protected:
	Ogre::SceneManager* sceneMgr;
	Ogre::SceneNode* sceneNode;
	Ogre::Viewport* viewport;
	Atlas* atlas;
	Canvas* canvas;
	std::vector<Element*> elements;
	std::map<std::string, std::string> fontAliases;
	bool isDirty;

public:
	Document(Ogre::SceneManager* sceneMgr, Ogre::Viewport* viewport, const std::string& filename);
	~Document();

	Element* getElementByName(const std::string& name);

	TextFlowElement* createTextFlowElement(const PropertyMap& properties);
	ImageElement* createImageElement(const PropertyMap& properties);
	DancingGraphElement* createDancingGraphElement(const PropertyMap& properties);

	std::string translateFontAlias(const std::string& fontAlias);

	Canvas* getCanvas();

	Atlas* getAtlas();

	Ogre::Viewport* getViewport();

	void update();

	void flagAsDirty();
};
};
#endif