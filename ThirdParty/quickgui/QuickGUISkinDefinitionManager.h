#ifndef QUICKGUISKINDEFINITIONMANAGER_H
#define QUICKGUISKINDEFINITIONMANAGER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISkinTypeManager.h"
#include "QuickGUISkinDefinition.h"

#include "OgreSingleton.h"

namespace QuickGUI
{
	class _QuickGUIExport SkinDefinitionManager :
		public Ogre::Singleton<SkinDefinitionManager>
	{
	public:
		SkinDefinitionManager();
		~SkinDefinitionManager();

		static SkinDefinitionManager& getSingleton(void);
		static SkinDefinitionManager* getSingletonPtr(void);

		SkinDefinition* getSkinDefinition(const Ogre::String& className);

		bool hasSkinDefinition(const Ogre::String& className);

		void registerSkinDefinition(const Ogre::String& className, SkinDefinition* d);

	protected:
		std::map<Ogre::String,SkinDefinition*> mWidgetSkinDefs;
	};
}

#endif