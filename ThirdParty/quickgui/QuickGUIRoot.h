#ifndef QUICKGUIROOT_H
#define QUICKGUIROOT_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIManager.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUIScriptWriter.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUITimerManager.h"
#include "QuickGUIWidgetFactoryManager.h"
#include "QuickGUIWidgetDescFactoryManager.h"

#include "OgreFontManager.h"
#include "OgrePrerequisites.h"
#include "OgreSingleton.h"
#include "OgreStringConverter.h"
#include "OgreViewport.h"

#include <map>

namespace QuickGUI
{
	class _QuickGUIExport Root :
		public Ogre::Singleton<Root>
	{
	public:
		Root();
		~Root();

		/**
		* Internal use only.
		*/
		GUIManager* _getActiveGUIManager();
		/**
		* Internal use only.
		*/
		void _setActiveGUIManager(GUIManager* m);

		static Root& getSingleton(void); 
		static Root* getSingletonPtr(void);

		GUIManager* createGUIManager(GUIManagerDesc& d);

		void destroyGUIManager(GUIManager* gm);
		void destroyGUIManager(const std::string& name);

		GUIManager* getGUIManager(const std::string& name);
		/**
		* Gets the name of the first font found on initialization of Root.
		*/
		Ogre::String getDefaultFontName();
		/**
		* Get the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired.
		*/
		float getDefaultHoverTime();

		/**
		* Set the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired. (Commonly used for tool tips)
		*/
		void setDefaultHoverTime(float seconds);

	protected:
		Ogre::FontPtr mDefaultFont;

		int mGUIManagerCounter;

		std::map<std::string,GUIManager*> mGUIManagers;

		// The amount of time the cursor has to hover over a widget before
		// the ON_HOVER event is fired. (3 seconds by default)
		float mDefaultHoverTime;

		// Allows widgets to get access to their owning GUIManager (serialization, loading from disk)
		GUIManager* mActiveGUIManager;
	};
}

#endif
