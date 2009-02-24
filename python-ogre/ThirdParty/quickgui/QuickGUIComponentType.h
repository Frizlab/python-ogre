#ifndef QUICKGUICOMPONENTTYPE_H
#define QUICKGUICOMPONENTTYPE_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISerializable.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	class _QuickGUIExport ComponentType :
		public Serializable
	{
	public:
		ComponentType(const Ogre::String& ComponentAlias);

		// Class of the Widget
		Ogre::String className;
		// Name reference for component
		Ogre::String componentAlias;
		// Name of the Type for this Component Widget
		Ogre::String typeName;

		/**
		* Builds the ComponentType from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
	};
}

#endif
