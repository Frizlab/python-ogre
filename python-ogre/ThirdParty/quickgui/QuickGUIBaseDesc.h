#ifndef QUICKGUIBASEDESC_H
#define QUICKGUIBASEDESC_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISerializable.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	class _QuickGUIExport BaseDesc :
		public Serializable
	{
	public:
		BaseDesc() {}

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass()=0;

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b)=0;
	};
}

#endif
