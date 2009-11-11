/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

#include "QuickGUIScriptWriter.h"
#include "QuickGUIScriptReader.h"

template<> QuickGUI::ScriptWriter* Ogre::Singleton<QuickGUI::ScriptWriter>::ms_Singleton = 0;

namespace QuickGUI
{
	ScriptWriter::ScriptWriter() :
		mWritingToFile(false),
		mFileName(""),
		mNumSpacesPerLevel(4)
	{
	}

	ScriptWriter::~ScriptWriter()
	{
		for(std::list<ScriptDefinition*>::iterator it = mDefinitions.begin(); it != mDefinitions.end(); ++it)
			OGRE_DELETE_T((*it),ScriptDefinition,Ogre::MEMCATEGORY_GENERAL);
	}

	ScriptWriter* ScriptWriter::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	ScriptWriter& ScriptWriter::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	void ScriptWriter::addDefinition(ScriptDefinition* d)
	{
		for(std::list<ScriptDefinition*>::iterator it = mDefinitions.begin(); it != mDefinitions.end(); ++it)
		{
			// Overwrite any ScriptDefinitions that have the same type and ID
			if(((*it)->getType() == d->getType()) && ((*it)->getID() == d->getID()))
			{
				OGRE_DELETE_T((*it),ScriptDefinition,Ogre::MEMCATEGORY_GENERAL);
				(*it) = d;
				return;
			}
		}

		mDefinitions.push_back(d);
	}

	void ScriptWriter::begin(const Ogre::String& fileName)
	{
		if(mWritingToFile)
			throw Exception(Exception::ERR_SCRIPT_WRITING,"ScriptWriter already in the process of writing to a file!","ScriptWriter::begin");

		mWritingToFile = true;
		mFileName = fileName;
	}

	void ScriptWriter::end()
	{
		if(!mWritingToFile)
			throw Exception(Exception::ERR_SCRIPT_WRITING,"ScriptWriter has not been setup to write to a file! (missing call to ScripWriter::begin()?)","ScriptWriter::end");

		std::fstream file(mFileName.c_str(), std::ios::out);

		for(std::list<ScriptDefinition*>::iterator it = mDefinitions.begin(); it != mDefinitions.end(); ++it)
		{
			writeDefinition(file,(*it),0);
		}

        file.close();

		mWritingToFile = false;
	}

	void ScriptWriter::writeDefinition(std::fstream& stream, ScriptDefinition* d, int level)
	{
		// Write the definition header
		stream << getSpaces(level * mNumSpacesPerLevel) + d->getType() + " " + d->getID() << std::endl;
		// Write opening curly brace
		stream << getSpaces(level * mNumSpacesPerLevel) + "{" << std::endl;
		// Write out properties
		std::list<DefinitionProperty*> propList = d->getProperties();
		for(std::list<DefinitionProperty*>::iterator it = propList.begin(); it != propList.end(); ++it)
		{
			DefinitionProperty* p = (*it);
			stream << getSpaces((level + 1) * mNumSpacesPerLevel) + p->getPropertyName();

			Ogre::StringVector sv = p->getValues();
			for(Ogre::StringVector::iterator propertyIter = sv.begin(); propertyIter != sv.end(); ++propertyIter)
			{
				stream << " " + (*propertyIter);
			}

			stream << std::endl;
		}
		// Write out sub Definitions
		std::list<ScriptDefinition*> subDefList = d->getDefinitions();
		if(!subDefList.empty())
		{
			if(!propList.empty())
				stream << std::endl;

			for(std::list<ScriptDefinition*>::iterator it = subDefList.begin(); it != subDefList.end(); ++it)
			{
				writeDefinition(stream,(*it),(level + 1));
				stream << std::endl;
			}
		}
		// Write closing curly brace
		stream << getSpaces(level * mNumSpacesPerLevel) + "}" << std::endl;
	}

	Ogre::String ScriptWriter::getSpaces(unsigned int numberOfSpaces)
	{
		Ogre::String s = "";
		s.append(numberOfSpaces, ' ');
		return s;
	}

	bool ScriptWriter::writingToFile()
	{
		return mWritingToFile;
	}
}
