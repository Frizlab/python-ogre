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

#ifndef __NXOGRE_TRIGGER_H__
#define __NXOGRE_TRIGGER_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreActor.h"
#include "NxOgreParams.h"

namespace NxOgre {

	
	//////////////////////////////////////////////////////////

	class NxExport TriggerCallback {
		
		public:

			
			//////////////////////////////////////////////////////////

			class ContactCallback {

				public:

					virtual void onEnter(Trigger*, Actor*)	{}
					virtual void onLeave(Trigger*, Actor*)	{}
					virtual void onInside(Trigger*, Actor*)	{}

#if (NX_USE_CHARACTER_API == 1)
					virtual void onEnter(Trigger*, Character*)	{}
					virtual void onLeave(Trigger*, Character*)	{}
					virtual void onInside(Trigger*, Character*)	{}
#endif

			};
			
			//////////////////////////////////////////////////////////

			class InheritedCallback : public ContactCallback {
				
				public:
					
					virtual void onEnter(Trigger*, Actor*)	{}
					virtual void onLeave(Trigger*, Actor*)	{}
					virtual void onInside(Trigger*, Actor*)	{}

#if (NX_USE_CHARACTER_API == 1)
					virtual void onEnter(Trigger*, Character*)	{}
					virtual void onLeave(Trigger*, Character*)	{}
					virtual void onInside(Trigger*, Character*)	{}
#endif

			};

			//////////////////////////////////////////////////////////

			template <typename T>
			class TriMethodCallback : public ContactCallback {
			
				public:

					TriMethodCallback(
					T* v,
					void (T::*AEnter)(Trigger*, Actor*),
					void (T::*ALeave)(Trigger*, Actor*),
					void (T::*AInside)(Trigger*, Actor*)
#if (NX_USE_CHARACTER_API == 1)
					,void (T::*CEnter)(Trigger*, Character*),
					 void (T::*CLeave)(Trigger*, Character*),
					 void (T::*CInside)(Trigger*, Character*)
#endif
					)

					: mInstance(v),
						AEnterPtr(AEnter), ALeavePtr(ALeave), AInsidePtr(AInside)
#if (NX_USE_CHARACTER_API == 1)
						,CEnterPtr(CEnter), CLeavePtr(CLeave), CInsidePtr(CInside)
#endif
					{}

					/////////////////////////////////////////////////////

					virtual void onEnter(Trigger* a, Actor* b) {
						(mInstance->*AEnterPtr)(a,b);
					}

					virtual void onLeave(Trigger* a, Actor* b) {
						(mInstance->*ALeavePtr)(a,b);
					}

					virtual void onInside(Trigger* a, Actor* b) {
						(mInstance->*AInsidePtr)(a,b);
					}

#if (NX_USE_CHARACTER_API == 1)
					virtual void onEnter(Trigger* a, Character* b) {
						(mInstance->*CEnterPtr)(a,b);
					}

					virtual void onLeave(Trigger* a, Character* b) {
						(mInstance->*CLeavePtr)(a,b);
					}

					virtual void onInside(Trigger* a, Character* b) {
						(mInstance->*CInsidePtr)(a,b);
					}
#endif
					/////////////////////////////////////////////////////

					T* mInstance;
					void (T::*AEnterPtr)(Trigger*, Actor*);
					void (T::*ALeavePtr)(Trigger*, Actor*);
					void (T::*AInsidePtr)(Trigger*, Actor*);
#if (NX_USE_CHARACTER_API == 1)
					void (T::*CEnterPtr)(Trigger*, Character*);
					void (T::*CLeavePtr)(Trigger*, Character*);
					void (T::*CInsidePtr)(Trigger*, Character*);
#endif
			};

			//////////////////////////////////////////////////////////

			TriggerCallback(InheritedCallback* cb) : mCallback(cb) {}

			template <typename T> explicit
			TriggerCallback(T* v,
			void (T::*AE)(Trigger*, Actor*),			
			void (T::*AL)(Trigger*, Actor*),			
			void (T::*AI)(Trigger*, Actor*)
#if (NX_USE_CHARACTER_API == 1)
			,void (T::*CE)(Trigger*, Character*),
			 void (T::*CL)(Trigger*, Character*),			
			 void (T::*CI)(Trigger*, Character*)
#endif			
			
			)
			: mCallback(new TriMethodCallback<T>(v, AI, AL, AI
#if (NX_USE_CHARACTER_API == 1)
			, CE,CL,CI
#endif
			)) {}
	

			//////////////////////////////////////////////////////////

			void onEnter(Trigger* a, Actor* b) {
				mCallback->onEnter(a,b);
			}

			void onLeave(Trigger* a, Actor* b) {
				mCallback->onLeave(a,b);
			}

			void onInside(Trigger* a, Actor* b) {
				mCallback->onInside(a,b);
			}

#if (NX_USE_CHARACTER_API == 1)
			void onEnter(Trigger* a, Character* b) {
				mCallback->onEnter(a,b);
			}

			void onLeave(Trigger* a, Character* b) {
				mCallback->onLeave(a,b);
			}

			void onInside(Trigger* a, Character* b) {
				mCallback->onInside(a,b);
			}
#endif
			//////////////////////////////////////////////////////////

			ContactCallback*	mCallback;

	};

	//////////////////////////////////////////////////////////

	class NxExport Trigger : public Actor {

		friend class Scene;
		friend class SceneTriggerController;

		public:

			Trigger(const NxString& identifier, Scene*, ShapeBlueprint*, const Pose&, ActorParams = "mass: 1, kinematic: yes");
			~Trigger();

			//////////////////////////////////////////////////////////

			void setCallback(TriggerCallback::InheritedCallback* cb) {
				mCallback = new TriggerCallback(cb);
				mCallbackOwned = true;
			}

			template <typename T> 
			void setCallback(
				T* v,
				void (T::*Actor_Enter)(Trigger*, Actor*),			
				void (T::*Actor_Leave)(Trigger*, Actor*),			
				void (T::*Actor_Inside)(Trigger*, Actor*)
#if (NX_USE_CHARACTER_API == 1)
				,void (T::*Character_Enter)(Trigger*, Character*),
				 void (T::*Character_Leave)(Trigger*, Character*),			
				 void (T::*Character_Inside)(Trigger*, Character*)
#endif
			)
			{

				mCallback = new TriggerCallback(
					v, 
					Actor_Enter, Actor_Leave, Actor_Inside
#if (NX_USE_CHARACTER_API == 1)
					,Character_Enter, Character_Leave, Character_Inside
#endif			
				);	
			
			}
			
			//////////////////////////////////////////////////////////
			
			virtual NxShortHashIdentifier	getType() const {return 44953;	/* "NxOgre-Trigger" */}
			virtual NxString				getStringType() const {return "NxOgre-Trigger";}
			
			//////////////////////////////////////////////////////////
			
			void _createActor(ShapeBlueprint *shape, const Pose& pose, ActorParams params);
			void _destroyActor();
			
			//////////////////////////////////////////////////////////
			
		protected:

			void onEnter(Actor*);
			void onLeave(Actor*);
			void onInside(Actor*);
#if (NX_USE_CHARACTER_API == 1)
			void onEnter(Character*);
			void onLeave(Character*);
			void onInside(Character*);
#endif
			//////////////////////////////////////////////////////////

			TriggerCallback*			mCallback;
			bool						mCallbackOwned;

	};
	
	//////////////////////////////////////////////////////////

};

#endif
