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

#ifndef __NXOGRE_CHARACTER_MODEL_H__
#define __NXOGRE_CHARACTER_MODEL_H__

#include "NxOgrePrerequisites.h"

#if (NX_USE_LEGACY_NXCONTROLLER == 0)

#include "NxOgreContainer.h"

namespace NxOgre {
	
	/////////////////////////////////////////////////////////
		
	class NxExport CharacterModel {

		friend class Character;

		public:

			CharacterModel(bool isShared = false, bool owned = false);

			bool isShared()	{return mShared;}

			bool isOwned()	{return mOwned;}

			CharacterMovementModel*	mDefaultMovementModel;
			CharacterMovementModels mMovementModels;

		protected:

			virtual void init()			{}
			
			// Shared betwen Characters.
			bool mShared;

			// Owned by NxOgre, and deleted by Character. Or owned by application and deleted by it.
			// shared CharacterModels won't be deleted.
			bool mOwned;

		private:
		
	};
		
	////////////////////////////////////////////

};// End of namespace

#endif
#endif
