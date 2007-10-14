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

#ifndef __NXOGRE_USERDATA_H__
#define __NXOGRE_USERDATA_H__

#include "NxOgrePrerequisites.h"

namespace NxOgre {

	class NxExport NxActorUserData {

		public:

			enum PtrType {
				T_Actor = 0,
				T_Trigger,
#if (NX_USE_FORCEFIELD_API == 1)
				T_ForceField,
#endif
#if (NX_USE_CHARACTER_API == 1)
				T_Character,
#endif
				T_Custom
			};

			////////////////////////////////////////////////////////////////
			
			NxActorUserData(void* ptr, PtrType type) : Ptr(ptr), Type(type) {}

			PtrType getType() {
				return Type;
			}

			Actor* toActor() {
				return static_cast<Actor*>(Ptr);
			}

			Trigger* toTrigger() {
				return static_cast<Trigger*>(Ptr);
			}

#if (NX_USE_FORCEFIELD_API == 1)
			ForceField* toForceField() {
				return static_cast<ForceField*>(Ptr);
			}
#endif

#if (NX_USE_CHARACTER_API == 1)
			Character* toCharacter() {
				return static_cast<Character*>(Ptr);
			}
#endif

			////////////////////////////////////////////////////////////////

			void* Ptr;
			PtrType Type;

	};
	
};

#endif
