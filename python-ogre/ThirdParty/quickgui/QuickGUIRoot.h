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
// These widgets are not included by default due to forward declarations
#include "QuickGUITabControl.h"
#include "QuickGUITabPage.h"

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
	};
}

#endif
