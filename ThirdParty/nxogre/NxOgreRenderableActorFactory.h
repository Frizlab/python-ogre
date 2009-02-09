/** \file    NxOgreRenderableActorFactory.h
 *  \brief   Header for the RenderableActorFactory class.
 *  \version 1.0-22T2
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


#ifndef __NXOGRE_RENDERABLE_ACTOR_FACTORY_H__
#define __NXOGRE_RENDERABLE_ACTOR_FACTORY_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreActor.h"
#include "NxOgreNodeRenderable.h"
#include "NxOgrePose.h"

namespace NxOgre {

  class NxPublicClass RenderableActorFactory {

    public:

      virtual Actor* createRendereredActor(Scene*, const VisualIdentifier& identifier, const ActorParams&, const NxOgre::Pose&, Shape* shape, unsigned int FactoryIdentifier) = 0;
      virtual Actor* createRendereredActor(Scene*, const NxString& identifier, const ActorParams&, const NodeRenderableParams&, const NxOgre::Pose&, Shape* shape, unsigned int FactoryIdentifier) = 0;

  };

} // End of NxOgre namespace.

#endif
