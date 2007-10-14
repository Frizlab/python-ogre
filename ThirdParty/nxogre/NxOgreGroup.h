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

#ifndef __NXOGRE_GROUP_H__
#define __NXOGRE_GROUP_H__

#include "NxOgrePrerequisites.h"

namespace NxOgre {

	
	//////////////////////////////////////////////////////////

	class NxExport	GroupCallback {
		
		public:

			
			//////////////////////////////////////////////////////////

			class ContactCallback {

				public:

					virtual void onStartTouch(Actor*, Actor*)	{}
					virtual void onEndTouch(Actor*, Actor*)		{}
					virtual void onTouch(Actor*, Actor*)		{}

			};

			class InheritedCallback : public ContactCallback {
				
				public:
					
					virtual void onStartTouch(Actor*, Actor*)	{}
					virtual void onEndTouch(Actor*, Actor*)		{}
					virtual void onTouch(Actor*, Actor*)		{}

			};

			template <typename T>
			class TriMethodCallback : public ContactCallback {
			
				public:

					TriMethodCallback(
					T* v,
					void (T::*Start)(Actor*, Actor*),
					void (T::*End)(Actor*, Actor*),
					void (T::*Touch)(Actor*, Actor*))
					: mInstance(v), StartTouchPtr(Start), EndTouchPtr(End), TouchPtr(Touch)
					{}

					/////////////////////////////////////////////////////

					void onStartTouch(Actor* a, Actor* b) {
						(mInstance->*StartTouchPtr)(a,b);
					}

					void onEndTouch(Actor* a, Actor* b) {
						(mInstance->*EndTouchPtr)(a,b);
					}

					void onTouch(Actor* a, Actor* b) {
						(mInstance->*TouchPtr)(a,b);
					}

					/////////////////////////////////////////////////////

					T* mInstance;
					void (T::*StartTouchPtr)(Actor*, Actor*);
					void (T::*EndTouchPtr)(Actor*, Actor*);
					void (T::*TouchPtr)(Actor*, Actor*);
			
			};

			//////////////////////////////////////////////////////////

			GroupCallback(InheritedCallback* cb) : mCallback(cb) {}

			template <typename T> explicit
			GroupCallback(T* v,
			void (T::*Start)(Actor*, Actor*),			
			void (T::*End)(Actor*, Actor*),			
			void (T::*Touch)(Actor*, Actor*))
			: mCallback(new TriMethodCallback<T>(v, Start, End, Touch)) {}
	

			//////////////////////////////////////////////////////////

			void onStartTouch(Actor* a, Actor* b) {
				mCallback->onStartTouch(a,b);
			}

			void onEndTouch(Actor* a, Actor* b) {
				mCallback->onEndTouch(a,b);
			}

			void onTouch(Actor* a, Actor* b) {
				mCallback->onTouch(a,b);
			}

			//////////////////////////////////////////////////////////

			ContactCallback*	mCallback;

	};

	//////////////////////////////////////////////////////////

	class NxExport ActorGroup {

		friend class Scene;
		friend class SceneContactController;

		public:

			ActorGroup(const NxString& identifier, Scene*);
			ActorGroup(NxActorGroup GroupID, const NxString& identifier, Scene*);
			~ActorGroup();

			NxString				getName() const		{return mName;}
			NxActorGroup			getGroupID() const	{return mGroupID;}

			void setCallback(GroupCallback::InheritedCallback* cb) {
//				std::cout << "Inherited Constructor (" << cb << ")" << std::endl;
				mCallback = new GroupCallback(cb);
				mCallbackOwned = true;
			}

			template <typename T> 
			void setCallback(
				T* v,
				void (T::*Start)(Actor*, Actor*),			
				void (T::*End)(Actor*, Actor*),			
				void (T::*Touch)(Actor*, Actor*)
			)
			{
				mCallback = new GroupCallback(v, Start, End, Touch);	
			}

			void					setCollisionCallback(ActorGroup* B, NxContactPairFlag flag, bool ForBGroup);

		protected:

			void					onStartTouch(Actor*, Actor*);
			void					onEndTouch(Actor*, Actor*);
			void					onTouch(Actor*, Actor*);

			NxString				mName;
			NxActorGroup			mGroupID;
			Scene*					mScene;
			GroupCallback*			mCallback;
			bool					mCallbackOwned;

			static					NxActorGroup mNextFreeID;
	};
	
	//////////////////////////////////////////////////////////

	class NxExport ShapeGroup {

		public:

			ShapeGroup(const NxString& identifier, Scene*);
			ShapeGroup(NxCollisionGroup GroupID, const NxString& identifier, Scene*);
			~ShapeGroup();


			enum CollisionResponse {
				CR_No_Collision = 0,
				CR_Collision
			};

			NxString				getName() const		{return mName;}
			NxActorGroup			getGroupID() const	{return mGroupID;}

			void					setCollisionResponse(ShapeGroup* B, CollisionResponse);

		protected:

			NxString			mName;
			NxCollisionGroup	mGroupID;
			Scene*				mScene;

			static				NxCollisionGroup mNextFreeID;

	};

	//////////////////////////////////////////////////////////

};

#endif
