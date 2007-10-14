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

#ifndef __NXOGRE_CHARACTER_H__
#define __NXOGRE_CHARACTER_H__
#if (NX_USE_CHARACTER_API == 1)

#include "NxOgrePrerequisites.h"
#include "NxOgreContainer.h"		// For: mCollisionList
#include "NxOgreParams.h"			// For: CharacterParams

namespace NxOgre {

	class NxExport Character {

		friend class CharacterHitReport;

	public:

		enum Movement {
			MV_Idle,
			MV_Walk,
			MV_Run,
			MV_Fall,
			MV_Jump_Up,
			MV_Jump_Directional,
			MV_Crouch,
			MV_Crouch_Walk,
			MV_SideStep,
			MV_Ladder,
			MV_Vehicle,
			MV_None
		};

		enum Direction {
			DR_Forward,
			DR_Backward,
			DR_StepLeft,
			DR_StepRight,
			DR_Jump_Up,
			DR_Jump_Directional,
			DR_Ladder_Up,
			DR_Ladder_Down,
			DR_None
		};

		Character(const NxString&, Scene*, CharacterController*, Pose, CharacterParams);
		~Character();

		void createNode();
		void attachMesh(const NxString&);
		void attachSkeletonMesh(const NxString&);
		void bindToAnimation(Movement, const NxString& AnimationName);
		void bindFeet(const NxString& leftBone, const NxString& rightBone);
		void unBindFeet();

		Ogre::Vector3 getGlobalPosition();
		Ogre::Quaternion getGlobalOrientation();

		void simulate(float);
		void render(float);

		void addMovement(Direction);
		void setDirection(const Ogre::Radian&);
		void setDirection(const Ogre::Quaternion&);

		void setRawNextMovementDirection(const NxVec3& direction);
		void setMovementVectorController(CharacterMovementVectorController*);
		void setPosition(const Ogre::Vector3&);

		NxString					getName() const		{
															return mName;
														}


		Ogre::SceneNode*			getNode()			{
															return mNode;
														}


		Ogre::Entity*				getEntity()			{
															return mEntity;
														}

		Scene*						getScene()			{
															return mScene;
														}

		

		NxController*				getNxController()	{
															return mCharacter;
														}

	
		NxReal						getMass()			{
															return mMass;
														}
	protected:

		NxString mName;
		Scene* mScene;
		CharacterController* mCharacterController;
		NxController* mCharacter;
		Ogre::SceneNode* mNode;
		Ogre::Entity* mEntity;

		std::pair<Ogre::Bone*, Ogre::Bone*> mFeet;
		
		RayCaster*			mFootRaycaster;
		RayCaster*			mDepthCheckCaster;

		NxVec3				mNextMovementDirection;
		NxQuat				mDirection;

		int					mType;
		NxVec3				mDimensions;
		NxHeightFieldAxis	mUpAxis;
		NxReal				mSlopeLimit;
		NxReal				mStepOffset;
		NxReal				mSkinWidth;
		NxU32				mMove_ActiveGroupsMask;
		NxF32				mMove_Sharpness;
		NxF32				mMove_MinDistance;
		NxGroupsMask		mMove_GroupMask;
		NxU32				mCollisionFlags;
		NxReal				mMass;

		NxActorUserData*	mNxActorUserData;

		CharacterMovementVectorController* mMoveVectorController;

	private:

	};


};// End of namespace
#endif
#endif
