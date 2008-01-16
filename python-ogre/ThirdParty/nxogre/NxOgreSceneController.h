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

#ifndef __NXOGRE_SCENE_CONTROLLER_H__
#define __NXOGRE_SCENE_CONTROLLER_H__

#include "NxOgrePrerequisites.h"

#ifdef WIN32 
	#define NOMINMAX
	#include <windows.h>
#endif

namespace NxOgre {

	class NxExport SceneController {

		friend class Scene;

		public:

			SceneController(Scene*);
			virtual ~SceneController();

			//////////////////////////////////////////////////////////////////////////////

			virtual NxString getType() const {return NxString("SceneController");}
			virtual void setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void init(NxScene*);
			virtual bool Simulate(NxReal);
			virtual NxReal	getIterationsPerSecond() {return mIPS;}
			virtual NxReal	getDeltaTime() const {return mDeltaTime;}
			virtual NxReal	getAlphaValue()	const {return 0.5f;}

			//////////////////////////////////////////////////////////////////////////////
			
			NxReal		mRenderTime;
			NxReal		mIPS;
			NxReal		mDeltaTime;

		protected:

			Scene*		mScene;
			NxScene*	mNxScene;

			//////////////////////////////////////////////////////////////////////////////

		private:

	};

	//////////////////////////////////////////////////////////////////////////////

	class NxExport NullSceneController : public SceneController {

		friend class Scene;

		public:

			NullSceneController(Scene*);
			virtual ~NullSceneController();

			//////////////////////////////////////////////////////////////////////////////

			virtual NxString getType() const {return NxString("Null");}

			virtual void setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void init(NxScene*);
			virtual bool Simulate(NxReal);
		
			//////////////////////////////////////////////////////////////////////////////

	};
	//////////////////////////////////////////////////////////////////////////////

	class NxExport VariableSceneController : public SceneController {

		friend class Scene;

		public:

			VariableSceneController(Scene*);
			virtual ~VariableSceneController();

			//////////////////////////////////////////////////////////////////////////////

			virtual NxString getType() const {return NxString("Variable");}

			virtual void setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void init(NxScene*);
			virtual bool Simulate(NxReal);
	

			//////////////////////////////////////////////////////////////////////////////

	};

	//////////////////////////////////////////////////////////////////////////////

	class NxExport FixedSceneController : public SceneController {

		friend class Scene;

		public:

			FixedSceneController(Scene*);
			virtual ~FixedSceneController();

			//////////////////////////////////////////////////////////////////////////////

			virtual NxString getType() const {return NxString("Fixed");}

			virtual void setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void init(NxScene*);
			virtual bool Simulate(NxReal);
	
			NxReal	mTiming_MaxStep;
			NxReal	mTiming_CurrentStep;
			bool	mRenderFrame;

			//////////////////////////////////////////////////////////////////////////////

	};

	//////////////////////////////////////////////////////////////////////////////

	class NxExport AccumulatorSceneController : public SceneController {

		friend class Scene;

		public:

			AccumulatorSceneController(Scene*);
			virtual ~AccumulatorSceneController();
			
			//////////////////////////////////////////////////////////////////////////////

			virtual NxString getType() const {return NxString("Accumulator");}
			virtual void setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void init(NxScene*);
			virtual bool Simulate(NxReal);
			virtual NxReal getAlphaValue() const {return mAlpha;}

			unsigned long getTime();

#ifdef WIN32
			
			LARGE_INTEGER	mFreq;
			LARGE_INTEGER	mCounter;
#else
			
#endif

		private:

			NxReal	mDt;
			NxReal	mAccumulator;
			NxReal	mSecond;
			NxU32	mSimCount;
			NxReal	mAlpha;
			NxU32	mPreviousTime;

			
			//////////////////////////////////////////////////////////////////////////////

	};


};

#endif
