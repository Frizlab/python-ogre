#ifndef QUICKGUISKINREFERENCE_H
#define QUICKGUISKINREFERENCE_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISerializable.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	class _QuickGUIExport SkinReference :
		public Serializable
	{
	public:
		SkinReference(const Ogre::String& alias);

		virtual ~SkinReference() {}

		// Class of the Widget
		Ogre::String className;
		// Name reference for skin
		Ogre::String skinAlias;
		// Name of the Skin Type for this Component Widget, ie "default", "myButton", etc.
		Ogre::String typeName;

		/**
		* Builds the SkinReference from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
	};
}

#endif
