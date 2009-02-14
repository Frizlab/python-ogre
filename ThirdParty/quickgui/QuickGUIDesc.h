#ifndef QUICKGUIBASEDESC_H
#define QUICKGUIBASEDESC_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISerializable.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	class _QuickGUIExport Desc :
		public Serializable
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		Desc() {}
		virtual ~Desc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "Desc"; }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b) {}
	};
}

#endif
