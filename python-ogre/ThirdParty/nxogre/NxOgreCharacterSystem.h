/** \file    NxOgreCharacterSystem.h
 *  \brief   Header for the CharacterSystem class.
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

#ifndef __NXOGRE_CHARACTER_SYSTEM_H__
#define __NXOGRE_CHARACTER_SYSTEM_H__

#include "NxOgrePrerequisites.h"

#if (NX_USE_LEGACY_NXCONTROLLER == 0)

#include "NxOgreContainer.h"
#include "NxOgreCharacter.h"

#if (NX_USE_CHARACTER_API == 1)
#	include "NxControllerManager.h"
#endif

namespace NxOgre {

	////////////////////////////////////////////////////////////////////////

	class NxPublicClass CharacterSystem {
	
		public:
		
			CharacterSystem(World*);
			~CharacterSystem();

#if (NX_USE_CHARACTER_API == 1)
			/** Return's a copy of the instance of the Controller Manager
				\return NxControllerManager
			*/
			NxControllerManager*	getControllerManager() {
				return mControllerManager;
			}
#endif

			void createCharacter(NxString identifier, Pose, CharacterModel*, CharacterParams = CharacterParams());
			void createPerformer(NxString visualIdentifier, Pose, CharacterModel*, CharacterParams = CharacterParams());

			void destroyCharacter(NxString identifier);
			Character* getCharacter(NxString identifier);

		protected:

			Characters				mCharacters;

		private:

#if (NX_USE_CHARACTER_API == 1)
			NxControllerManager*	mControllerManager;
#endif

			World*					mWorld;

	};

	////////////////////////////////////////////////////////////////////////

};// End of namespace

#endif
#endif