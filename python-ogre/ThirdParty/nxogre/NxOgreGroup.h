/** \file   NxOgreGroup.h
*  \brief   Header for the ActorGroup and ShapeGroup classes.
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

#ifndef __NXOGRE_GROUP_H__
#define __NXOGRE_GROUP_H__

#include "NxOgrePrerequisites.h"

namespace NxOgre
{

  class NxPublicClass ActorGroup
  {

    friend class Scene;
    friend class SceneContactController;

  public:

    NxString          getName() const;
    NxActorGroup      getGroupID() const;

    ~ActorGroup();

  protected:

    ActorGroup(const NxString& identifier, Scene*);
    ActorGroup(NxActorGroup GroupID, const NxString& identifier, Scene*);
    

  protected:


    NxString				mName;
    NxActorGroup    mGroupID;
    Scene*          mScene;
    GroupCallback*  mCallback;

    static					NxActorGroup mNextFreeID;
  };

  //////////////////////////////////////////////////////////

  class NxPublicClass ShapeGroup {

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
