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

#ifndef __NXOGRE_CHARACTER_CONTROLLER_H__
#define __NXOGRE_CHARACTER_CONTROLLER_H__
#if (NX_USE_CHARACTER_API == 1)

#include "NxOgrePrerequisites.h"
#include "NxOgreContainer.h"		// For: mCharacters
#include "NxOgreParams.h"			// For: CharacterParams
#include "NxController.h"			// For: NxUserControllerHitReport

namespace NxOgre {

	class NxExport CharacterParams : public Params {

		public:

			CharacterParams() {}
			CharacterParams(const char* p)  {setToDefault();process(p);}
			CharacterParams(NxString p) {setToDefault();process(p);}

			void setToDefault();
			void parse(Parameters);

			enum CharacterType {
				CT_Box,
				CT_Capsule
			};

			CharacterType mType;
			NxVec3 mDimensions;
			NxHeightFieldAxis mUpAxis;
			NxReal mSlopeLimit;
			NxReal mStepOffset;
			NxReal mSkinWidth;
			NxU32  mActiveGroupsMask;
			NxF32  mSharpness;
			NxGroupsMask mGroupMask;


	};// End of CharacterParams class
	
	class NxExport CharacterMovementVectorController {
		public:
			
			virtual void move(NxVec3 &out, NxVec3 &moveVector, NxQuat &direction, NxVec3 &g, float t, Character*);
	};

	class NxExport CharacterController : public NxUserControllerHitReport, public CharacterMovementVectorController {

		friend class World;		
		friend class Character;


		public:
			
			Character* create(const NxString &name, Pose, CharacterParams, Scene*);
			void destroy(const NxString &name);
			Character* get(const NxString &name);

			NxControllerAction onShapeHit(const NxControllerShapeHit &);
			NxControllerAction onControllerHit(const NxControllersHit &);
			NxControllerManager* getNxControllerManager() {return mControllerManager;}

			virtual void move(NxVec3 &out, NxVec3 &moveVector, NxVec3 g, float t) {
				out = (moveVector + g) * t;
			}

			void addHitReport(CharacterHitReport* report) {
				mHitReports.insert(report);
			}

		protected:
			
			void _registerCharacter(const NxString& name, Character*);
			void _unregisterCharacter(const NxString&);

			CharacterController();
			~CharacterController();

			UserAllocator* mAllocator;

			CharacterHitReports			mHitReports;
			BaseCharacterHitReport*		mBaseHitReport;

			

		private:

			Characters mCharacters;
			NxControllerManager*	mControllerManager;

	};

};// End of namespace

#endif
#endif
