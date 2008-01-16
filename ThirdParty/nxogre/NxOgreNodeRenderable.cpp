//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "NxOgreStable.h"
#include "NxOgreNodeRenderable.h"
#include "NxOgreScene.h"
#include "NxOgrePose.h"
namespace NxOgre {

//////////////////////////////////////////////////////////

void NodeRenderableParams::setToDefault() {

	Identifier              = NxString();
	IdentifierType          = IT_CREATE;
	GraphicsModel           = NxString();
	GraphicsModelType       = GMT_RESOURCE_IDENTIFIER;
	GraphicsModelScale      = NxVec3(1,1,1);
	GraphicsModelPose       = Pose();
	GraphicsModelOffset     = Pose();
	GraphicsModelMaterial   = NxString();

}

//////////////////////////////////////////////////////////

void NodeRenderableParams::parse(Parameters P) {

	for (Parameters::iterator p = P.begin(); p != P.end();p++) {

		if (Set("identifier", (*p), Identifier)) continue;

		if ((*p).first == "identifier-usage") {
			
			if ((*p).second.substr(0,1) == "c" || (*p).second.substr(0,1) == "C") {
				IdentifierType = IT_CREATE;
			}
			else if ((*p).second.substr(0,1) == "u" || (*p).second.substr(0,1) == "U") {
				IdentifierType = IT_USE;
			}
			else {
				IdentifierType = IT_CREATE;
			}

		}

		if (Set("model", (*p), GraphicsModel)) continue;

		if ((*p).first == "model-type") {
			
			NxString mt = (*p).second;
			toLower(mt);

			if (mt == "resource") {
				GraphicsModelType = GMT_RESOURCE_IDENTIFIER;
			}
			else if (mt == "file") {
				GraphicsModelType = GMT_FILE;
			}
			else if (mt == "reference"){
				GraphicsModelType = GMT_EXISTING_REFERENCE;
			}
			else {
				GraphicsModelType = GMT_RESOURCE_IDENTIFIER;
			}

		}

		if (Set("scale", (*p), GraphicsModelScale)) continue;
		if (Set("position", (*p), GraphicsModelPose.v)) continue;
		if (Set("orientation", (*p), GraphicsModelPose.q)) continue;
		if (Set("offset-position", (*p), GraphicsModelOffset.v)) continue;
		if (Set("offset-orientation", (*p), GraphicsModelOffset.q)) continue;
		if (Set("material", (*p), GraphicsModelMaterial)) continue;

	}

}

//////////////////////////////////////////////////////////

NodeRenderable::NodeRenderable(NodeRenderableParams, SceneRenderer* r)
	: Renderable(r)
{}

//////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
