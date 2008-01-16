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

namespace NxOgre {

	class NxExport SceneController {

		friend class Scene;

		public:

			SceneController(Scene*);
			virtual ~SceneController();

			//////////////////////////////////////////////////////////////////////////////

			virtual void setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void init(NxScene*);
			virtual bool Simulate(NxReal);
			virtual bool Render(NxReal);

			//////////////////////////////////////////////////////////////////////////////

		protected:

			Scene*		mScene;
			NxScene*	mNxScene;

			//////////////////////////////////////////////////////////////////////////////

		private:

	};

	//////////////////////////////////////////////////////////////////////////////

	class NxExport VariableSceneController : public SceneController {

		friend class Scene;

		public:

			VariableSceneController(Scene*);
			virtual ~VariableSceneController();

			//////////////////////////////////////////////////////////////////////////////

			virtual void setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void init(NxScene*);
			virtual bool Simulate(NxReal);
			virtual bool Render(NxReal);

			//////////////////////////////////////////////////////////////////////////////

	};

	//////////////////////////////////////////////////////////////////////////////

	class NxExport FixedSceneController : public SceneController {

		friend class Scene;

		public:

			FixedSceneController(Scene*);
			virtual ~FixedSceneController();

			//////////////////////////////////////////////////////////////////////////////

			virtual void setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void init(NxScene*);
			virtual bool Simulate(NxReal);
			virtual bool Render(NxReal);

			NxReal	mTiming_MaxStep;
			NxReal	mTiming_CurrentStep;
			bool	mRenderFrame;

			//////////////////////////////////////////////////////////////////////////////

	};
#if (NX_UNSTABLE == 1)
	//////////////////////////////////////////////////////////////////////////////

	class NxExport SuperFixedSceneController : public SceneController {

		friend class Scene;

		public:

			SuperFixedSceneController(Scene*);
			virtual ~SuperFixedSceneController();

			
			//////////////////////////////////////////////////////////////////////////////

			virtual void setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void init(NxScene*);
			virtual bool Simulate(NxReal);
			virtual bool Render(NxReal);

			NxReal	mTiming_MaxStep;
			double	mTiming_CurrentStep;
			double	mMargin;
			NxU32	mFrameCount;
			NxU32	mNbSimulations;
			NxU32	mNbSimulationsLast;
			NxU32	mNbSimulationsFramesLast;
			double	mTiming_Multiplier;
			bool	mFirstFrame;
			bool	mRenderFrame;

			Ogre::Timer	*mTimer;
			Ogre::Timer	*mTimer2;
			//////////////////////////////////////////////////////////////////////////////

	};

	class NxExport ASyncSceneController : public SceneController {

		friend class Scene;

		public:

			ASyncSceneController(Scene*);
			virtual ~ASyncSceneController();

			//////////////////////////////////////////////////////////////////////////////

			virtual void setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps);
			virtual void init(NxScene*);
			virtual bool Simulate(NxReal);
			virtual bool Render(NxReal);

			//////////////////////////////////////////////////////////////////////////////

			Ogre::Timer*	mTimer;
			NxReal			mTiming_MaxStep;
			NxReal			mTiming_CurrentStep;
			NxReal			mTimingDT;
			
			bool			mRenderFrame;
			bool			mRenderNow;
			bool			mUnfinished;

	};
#endif

};

#endif
