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

#ifndef __NXOGRE_BODY_H__
#define __NXOGRE_BODY_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreActor.h"			// For: Body inherits Actor

namespace NxOgre {

	class NxExport Body : public Actor {

		public:

			Body(const NxString& name, Scene*);
			Body(const NxString& name, Scene*, ShapeBlueprint*, const Pose&, ActorParams = ActorParams());
			virtual ~Body();

			virtual bool			hasVisualisation() const	{return true;}
			Ogre::SceneNode*		getNode()					{return mNode;}
			Ogre::Entity*			getEntity()					{return mEntity;}
			void					setNode(Ogre::SceneNode*);
			void					setEntity(Ogre::Entity*);
			void					simulate(float);
			void					render(float);

			StringPairList			saveCustom();
			void					restoreCustom(StringPairList);

			void					disableVisualisation();

		protected:

			Ogre::SceneNode*		mNode;
			Ogre::Entity*			mEntity;

	};

};

#endif
