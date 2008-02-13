/** \file    NxOgreUserData.h
 *  \brief   Header for the NxUserData class.
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


#ifndef __NXOGRE_USERDATA_H__
#define __NXOGRE_USERDATA_H__

#include "NxOgrePrerequisites.h"

namespace NxOgre {

	class NxPublicClass NxUserData {

		public:

			enum PtrType {
				T_Actor       = 0,
				T_Shape       = 1,
				T_SimpleActor = 2,
				T_Character   = 3,
				T_Trigger     = 4,
				T_Custom      = 5
			};

			NxUserData(void* ptr, PtrType type, RenderableSource* render_ptr = 0)
			   : Ptr(ptr),
			     Type(type),
			     RenderPtr(render_ptr)
			     {}

			PtrType getType() {
				return Type;
			}

			Actor* toActor() {
				return static_cast<Actor*>(Ptr);
			}

			SimpleActor* toSimpleActor() {
				return static_cast<SimpleActor*>(Ptr);
			}

			Character* toCharacter() {
				return static_cast<Character*>(Ptr);
			}

			Trigger* toTrigger() {
				return static_cast<Trigger*>(Ptr);
			}

			Shape* toShape() {
				return static_cast<Shape*>(Ptr);
			}

			void* Ptr;
			PtrType Type;
			RenderableSource* RenderPtr;

	};
	
};

#endif
