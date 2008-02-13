/** \file    NxOgreBody.h
 *  \brief   Header for the Body class.
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

#ifndef __NXOGRE_BODY_H__
#define __NXOGRE_BODY_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreActor.h"			// For: Body inherits Actor
#include "NxOgrePose.h"
#include "NxOgreRenderableSource.h"
#include "NxOgreRenderable.h"
#include "NxOgreNodeRenderable.h"

namespace NxOgre {

	class NxPublicClass Body : public Actor, public RenderableSource {

		public:

			Body(const NxString& NameAndOrShorthandVisualIdentifier, Scene*);

			/** \brief Body constructor with limited short-hand visualisation specification.
				How the body is visualised is built into the identifier.
					
					- "myBody"
						No Visualisation

					- "myBody; cow.mesh"
						Visualisation as "cow.mesh" as the graphics model loading it from
						the rendersystems resource system.
						(Default behaviour)

					- "myBody; (reference) cow.mesh"
						Visualisation as "cow.mesh" from a pre-existing model that has
						already been loaded and in the scene already.
						(Moving it from one visualisation node to this one)
			*/
			Body(const NxString& Identifier, Scene*, Shape*, const Pose&, ActorParams);
			
			/** Body constructor with full visualisation.
					
				mSceneMgr->createBody("myBody", new CubeShape(2,2,2), Vector3(0,5,0), "model: cube.1m.mesh, scale: 2 2 2", "mass: 10");

			*/
			Body(const NxString& Identifier, Scene*, Shape*, const Pose&, NodeRenderableParams, ActorParams);
			
			/** \brief Body destructor. If you hate memory leaks and crashes, use mScene->destroyActor(...);
			*/

			virtual ~Body();

			NxShortHashIdentifier	getType() const {return NxHashes_Body;	/* "NxOgre-Example-Body" */}
			NxString				getStringType() const {return "NxOgre-Example-Body";}


			StringPairList			saveCustom();
			void					restoreCustom(StringPairList);


		protected:

			void					__renderSelf();

			NodeRenderable*			mNodeRenderable;



	};

};

#endif
