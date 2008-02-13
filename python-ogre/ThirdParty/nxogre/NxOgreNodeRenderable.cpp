/** \file    NxOgreNodeRenderable.cpp
 *  \see     NxOgreNodeRenderable.h
 *  \version 1.0-20
 *
 *  \licence NxOgre a wrapper for the PhysX physics library.
 *           Copyright (C) 2005-8 Robin Southern of NxOgre.org http://www.nxogre.org
 *           This library is free software; you can redistribute it and/or
 *           modify it under the terms of the GNU Lesser General Public
 *           License as published by the Free Software Foundation; either
 *           version 2.1 of the License, or (at your option) any later version.
 *           
 *           This library is distributed in the hope that it will be useful,
 *           but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *           Lesser General Public License for more details.
 *           
 *           You should have received a copy of the GNU Lesser General Public
 *           License along with this library; if not, write to the Free Software
 *           Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "NxOgreStable.h"
#include "NxOgreNodeRenderable.h"
#include "NxOgreScene.h"
#include "NxOgrePose.h"
#include "NxOgreHelpers.h"

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

void NodeRenderableParams::parse(Parameters params) {

	setToDefault();

	for (Parameter* parameter = params.Begin(); parameter = params.Next();) {

		if (Set("identifier", parameter, Identifier)) continue;

		if (parameter->i == "identifier-type") {
			
			if (parameter->j.substr(0,1) == "c" || parameter->j.substr(0,1) == "C") {
				IdentifierType = IT_CREATE;
			}
			else if (parameter->j.substr(0,1) == "r" || parameter->j.substr(0,1) == "R") {
				IdentifierType = IT_USE;
			}
			else {
				IdentifierType = IT_CREATE;
			}

		}

		if (Set("model", parameter, GraphicsModel)) continue;

		if (parameter->i == "model-type") {
			
			NxString mt = parameter->j;
			NxStringToLower(mt);

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

		if (Set("scale", parameter, GraphicsModelScale)) continue;
		if (Set("position", parameter, GraphicsModelPose.m.t)) continue;
		if (Set("orientation", parameter, GraphicsModelPose.m.M)) continue;
		if (Set("offset-position", parameter, GraphicsModelOffset.m.t)) continue;
		if (Set("offset-orientation", parameter, GraphicsModelOffset.m.M)) continue;
		if (Set("material", parameter, GraphicsModelMaterial)) continue;

	}

}

//////////////////////////////////////////////////////////

NodeRenderable::NodeRenderable(NodeRenderableParams, SceneRenderer* r)
	: Renderable(r)
{}

//////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
