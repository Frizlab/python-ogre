#include "QuickGUIComponentType.h"

namespace QuickGUI
{
	ComponentType::ComponentType(const Ogre::String& ComponentAlias) :
		className(""),
		componentAlias(ComponentAlias),
		typeName("")
	{
	}

	void ComponentType::serialize(SerialBase* b)
	{
		b->begin("ComponentType",componentAlias);

		b->IO("ClassName",&className);
		b->IO("SkinType",&typeName);

		b->end();
	}
}
