#ifndef QUICKGUIVERTEX_H
#define QUICKGUIVERTEX_H

#include "OgreColourValue.h"
#include "OgreVector3.h"
#include "OgreVector2.h"

#include "QuickGUIExportDLL.h"

// From OpenGUI: Renderer_Ogre_Renderer.h

namespace QuickGUI
{
	const int VERTICES_PER_QUAD = 6;

	struct _QuickGUIExport Vertex
	{
		Ogre::Vector3 pos;
		Ogre::ColourValue color;
		Ogre::Vector2 uv;
	};
}

#endif
