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

					virtual void onStartTouch(Actor*, Actor*, ContactStream*)	{}
					virtual void onEndTouch(Actor*, Actor*, ContactStream*)		{}
					virtual void onTouch(Actor*, Actor*, ContactStream*)		{}

			};

			class InheritedCallback : public ContactCallback {
				
				public:
					
					virtual void onStartTouch(Actor*, Actor*, ContactStream*)	{}
					virtual void onEndTouch(Actor*, Actor*, ContactStream*)		{}
					virtual void onTouch(Actor*, Actor*, ContactStream*)		{}

			};

			template <typename T>
			class TriMethodCallback : public ContactCallback {
			
				public:

					TriMethodCallback(
					T* v,
					void (T::*Start)(Actor*, Actor*, ContactStream*),
					void (T::*End)(Actor*, Actor*, ContactStream*),
					void (T::*Touch)(Actor*, Actor*, ContactStream*))
					: mInstance(v), StartTouchPtr(Start), EndTouchPtr(End), TouchPtr(Touch)
					{}

					/////////////////////////////////////////////////////

					void onStartTouch(Actor* a, Actor* b, ContactStream* s) {
						(mInstance->*StartTouchPtr)(a,b,s);
					}

					void onEndTouch(Actor* a, Actor* b, ContactStream* s) {
						(mInstance->*EndTouchPtr)(a,b,s);
					}

					void onTouch(Actor* a, Actor* b, ContactStream* s) {
						(mInstance->*TouchPtr)(a,b,s);
					}

					/////////////////////////////////////////////////////

					T* mInstance;
					void (T::*StartTouchPtr)(Actor*, Actor*, ContactStream* s);
					void (T::*EndTouchPtr)(Actor*, Actor*, ContactStream* s);
					void (T::*TouchPtr)(Actor*, Actor*, ContactStream* s);
			
			};

			//////////////////////////////////////////////////////////

			/** @brief Inherited Class Constructor */

			GroupCallback(InheritedCallback* cb, bool ownCallback = true) : mCallback(cb), mCallbackOwned(ownCallback) {}

			//////////////////////////////////////////////////////////

			/** @brief Method Ptr Constructor */
            #ifndef GCC_XML
			template <typename T> explicit
			GroupCallback(T* v,
			void (T::*Start)(Actor*, Actor*),			
			void (T::*End)(Actor*, Actor*),			
			void (T::*Touch)(Actor*, Actor*))
			: mCallback(new TriMethodCallback<T>(v, Start, End, Touch), bool callbackOwned),mCallbackOwned(callbackOwned) {}
			#else
            template <typename T> explicit
			GroupCallback(T* v,
			void (T::*Start)(Actor*, Actor*),			
			void (T::*End)(Actor*, Actor*),			
			void (T::*Touch)(Actor*, Actor*))
			: mCallback(new TriMethodCallback<T>(v, Start, End, Touch)) {}
		    #endif
			//////////////////////////////////////////////////////////


			~GroupCallback() {
				if(mCallbackOwned) {
					NxDelete(mCallback);
				}
			} 

			//////////////////////////////////////////////////////////

			inline void onStartTouch(Actor* a, Actor* b, ContactStream* s) {
				mCallback->onStartTouch(a,b,s);
			}

			inline void onEndTouch(Actor* a, Actor* b, ContactStream* s) {
				mCallback->onEndTouch(a,b,s);
			}

			inline void onTouch(Actor* a, Actor* b, ContactStream* s) {
				mCallback->onTouch(a,b,s);
			}

			//////////////////////////////////////////////////////////

			ContactCallback*	mCallback;
			bool				mCallbackOwned;

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

			void setCallback(GroupCallback::InheritedCallback* cb, bool ownCallback = true) {
				NxDelete(mCallback);
				mCallback = new GroupCallback(cb, ownCallback);
			}

			template <typename T> 
			void setCallback(
				T* v,
				void (T::*Start)(Actor*, Actor*),			
				void (T::*End)(Actor*, Actor*),			
				void (T::*Touch)(Actor*, Actor*),
				bool ownCallback = true
			)
			{
				NxDelete(mCallback);
				mCallback = new GroupCallback(v, Start, End, Touch, ownCallback);	
			}

			void					setCollisionCallback(ActorGroup* B, NxContactPairFlag flag, bool ForBGroup);

		protected:

			void					onStartTouch(Actor*, Actor*, ContactStream*);
			void					onEndTouch(Actor*, Actor*, ContactStream*); 
			void					onTouch(Actor*, Actor*, ContactStream*);

			NxString				mName;
			NxActorGroup			mGroupID;
			Scene*					mScene;
			GroupCallback*			mCallback;

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
