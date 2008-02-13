#if 0
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

#ifndef __NXOGRE_SHAPEBLUEPRINT_WHEEL_H__
#define __NXOGRE_SHAPEBLUEPRINT_WHEEL_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreShapeBlueprint.h"		// For: These classes inherit ShapeBlueprint
#include "NxOgreShape.h"				// For: Default Params
#include "NxOgreShapeWheel.h"			// For: WheelParams
#include "NxOgreNodeRenderable.h"		// For: NodeRenderableParams

namespace NxOgre {

	class NxExport WheelShape : public ShapeBlueprint {

		public:

			WheelShape(NxReal radius, ShapeParams = ShapeParams(), WheelParams = WheelParams(), NodeRenderableParams = NodeRenderableParams());
			~WheelShape();

			NxWheelShapeDesc mShapeDescription;

	protected:

			NxString meshname;
			WheelParams wheelParams;
			NodeRenderableParams renderableParams;

			virtual bool isDynamic() {return true;}
			virtual bool isStaticOnly() {return false;}
			virtual Shape* _bindToActorDescription(Actor*, NxU32, NxArray<NxShapeDesc*>&);
			
	};

};

#endif
#endif