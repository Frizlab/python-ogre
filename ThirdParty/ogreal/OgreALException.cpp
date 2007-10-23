#include "OgreALException.h"
#include "OgreException.h"

namespace OgreAL {
	void check(bool condition, const int errorNumber, const Ogre::String& description, const Ogre::String& source)
	{
		if (!condition)
		{
			throw Ogre::Exception(errorNumber, description, source);
		}
	}
} // Namespace
