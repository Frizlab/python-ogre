//This code is public domain - you can do whatever you want with it
//Original author: John Judnich
#pragma once
#ifndef __QuickGuiSkinSetParser_h_
#define __QuickGuiSkinSetParser_h_

#include "OgrePrerequisites.h"
#include "OgreScriptLoader.h"

#include "QuickGUIExportDLL.h"

namespace QuickGUI
{
	class ConfigNode;

	class _QuickGUIExport ConfigScriptLoader: public Ogre::ScriptLoader
	{
	public:
		ConfigScriptLoader();
		~ConfigScriptLoader();

		inline static ConfigScriptLoader &getSingleton() { return *singletonPtr; }
		inline static ConfigScriptLoader *getSingletonPtr() { return singletonPtr; }

		Ogre::Real getLoadingOrder() const;
		const Ogre::StringVector &getScriptPatterns() const;

		ConfigNode *getConfigScript(const Ogre::String &type, const Ogre::String &name);

		void parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName);

	private:
		static ConfigScriptLoader *singletonPtr;

		Ogre::Real mLoadOrder;
		Ogre::StringVector mScriptPatterns;

		HashMap <Ogre::String, ConfigNode*> scriptList;

		//Parsing
		char *parseBuff, *parseBuffEnd, *buffPtr;
		size_t parseBuffLen;

		enum Token
		{
			TOKEN_TEXT,
			TOKEN_NEWLINE,
			TOKEN_OPENBRACE,
			TOKEN_CLOSEBRACE,
			TOKEN_EOF,
		};

		Token tok, lastTok;
		Ogre::String tokVal, lastTokVal;
		char *lastTokPos;

		void _parseNodes(ConfigNode *parent);
		void _nextToken();
		void _prevToken();
	};


	class _QuickGUIExport ConfigNode
	{
	public:
		ConfigNode(ConfigNode *parent, const Ogre::String &name = "untitled");
		~ConfigNode();

		inline void setName(const Ogre::String &name)
		{
			this->name = name;
		}

		inline Ogre::String &getName()
		{
			return name;
		}

		inline void addValue(const Ogre::String &value)
		{
			values.push_back(value);
		}

		inline void clearValues()
		{
			values.clear();
		}

		inline std::vector<Ogre::String> &getValues()
		{
			return values;
		}

		ConfigNode *addChild(const Ogre::String &name = "untitled");
		ConfigNode *findChild(const Ogre::String &name, bool recursive = false);

		inline std::vector<ConfigNode*> &getChildren()
		{
			return children;
		}

		void setParent(ConfigNode *newParent);

		inline ConfigNode *getParent()
		{
			return parent;
		}

	private:
		Ogre::String name;
		std::vector<Ogre::String> values;
		std::vector<ConfigNode*> children;
		ConfigNode *parent;

		std::vector<ConfigNode*>::iterator _iter;
		bool _removeSelf;
	};


	class _QuickGUIExport ConfigScriptSerializer
	{
	public:
		void beginSection(const unsigned short level);
		void endSection(const unsigned short level);
		void writeAttribute(const unsigned short level, const Ogre::String& att);
		void writeValue(const Ogre::String& val);
		void clearQueue();
		void exportQueued(const Ogre::String &fileName);

	private:
		Ogre::String mBuffer;
	};
}

#endif //__QuickGuiSkinSetParser_h_
