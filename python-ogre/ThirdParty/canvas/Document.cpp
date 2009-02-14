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

#include "Document.h"
using namespace Ogre;

namespace CanvasNS
{
Document::Document(Ogre::SceneManager* sceneMgr, Ogre::Viewport* viewport, const std::string& filename)
: sceneMgr(sceneMgr), viewport(viewport)
{
	Definitions defs;
	parse(defs, filename, "General");

	std::vector<Ogre::String> textureList;
	std::vector<FontFaceDefinition> fontList;
	std::string resourceGroup;

	Definitions::iterator i = defs.find("Assets");
	if(i == defs.end())
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find 'Assets' definition while loading Document.", "Document::Document");

	PropertyMap& assets = i->second;

	resourceGroup = assets["resource-group"].getString();

	std::vector<std::string> tokens;

	assets["textures"].tokenize(tokens);
	textureList.assign(tokens.begin(), tokens.end());

	assets["fonts"].tokenize(tokens);
	for(std::vector<std::string>::iterator j = tokens.begin(); j != tokens.end(); j++)
	{
		std::string fontAlias(*j);

		i = defs.find(fontAlias);
		if(i == defs.end())
			OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find the definiton for the font-alias '" + fontAlias + 
				"' while loading Document.", "Document::Document");
		
		std::string filename(i->second["filename"].getString());

		FontFaceDefinition newFont(filename, CharCodeRange::BasicLatin, FontFaceDefinition::BetterShape);
		
		static std::vector<std::string> sizeTokens;
		i->second["sizes"].tokenize(sizeTokens);

		for(std::vector<std::string>::iterator k = sizeTokens.begin(); k != sizeTokens.end(); k++)
			newFont.addSize(toNumber<unsigned int>(*k));

		fontList.push_back(newFont);
		fontAliases[fontAlias] = filename;
	}

	atlas = new Atlas(textureList, fontList, resourceGroup);
	canvas = new Canvas(atlas, viewport);

	sceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	sceneNode->attachObject(canvas);

	for(Definitions::iterator i = defs.begin(); i != defs.end(); i++)
	{
		if(i->first == "TextFlow")
			elements.push_back(createTextFlowElement(i->second));
		else if(i->first == "Image")
			elements.push_back(createImageElement(i->second));
		else if(i->first == "DancingGraph")
			elements.push_back(createDancingGraphElement(i->second));
	}
}

Document::~Document()
{
	for(std::vector<Element*>::iterator i = elements.begin(); i != elements.end(); i++)
		delete *i;

	sceneNode->detachAllObjects();
	delete canvas;
	delete atlas;
}

Element* Document::getElementByName(const std::string& name)
{
	for(std::vector<Element*>::iterator i = elements.begin(); i != elements.end(); i++)
	{
		if((*i)->getName() == name)
			return *i;
	}

	return 0;
}

TextFlowElement* Document::createTextFlowElement(const PropertyMap& properties)
{
	return new TextFlowElement(this, properties);
}

ImageElement* Document::createImageElement(const PropertyMap& properties)
{
	return new ImageElement(this, properties);
}

DancingGraphElement* Document::createDancingGraphElement(const PropertyMap& properties)
{
	return new DancingGraphElement(this, properties);
}

std::string Document::translateFontAlias(const std::string& fontAlias)
{
	std::map<std::string, std::string>::iterator i = fontAliases.find(fontAlias);

	if(i != fontAliases.end())
		return i->second;
	else
		return fontAlias;
}

Canvas* Document::getCanvas()
{
	return canvas;
}

Atlas* Document::getAtlas()
{
	return atlas;
}

Ogre::Viewport* Document::getViewport()
{
	return viewport;
}

void Document::update()
{
	if(isDirty)
	{
		canvas->clear();

		for(std::vector<Element*>::iterator i = elements.begin(); i != elements.end(); i++)
		{
			(*i)->draw(canvas, atlas);
			canvas->clearClip();
		}

		isDirty = false;
	}
}

void Document::flagAsDirty()
{
	isDirty = true;
}
};