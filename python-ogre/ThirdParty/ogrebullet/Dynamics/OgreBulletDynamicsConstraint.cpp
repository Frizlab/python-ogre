/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



This program is free software; you can redistribute it and/or modify it under
the terms of the GPL General Public License with runtime exception as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GPL General Public License with runtime exception for more details.

You should have received a copy of the GPL General Public License with runtime exception along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
-----------------------------------------------------------------------------
*/

#include "OgreBulletDynamics.h"

#include "OgreBulletDynamicsConstraint.h"

#include "OgreBulletDynamicsWorld.h" 

#include "OgreBulletDynamicsRigidBody.h"

using namespace Ogre;

namespace OgreBulletDynamics
{
    // -------------------------------------------------------------------------
    TypedConstraint::TypedConstraint(DynamicsWorld *world) :
            mConstraint (0),
            mWorld (world),
            mBodyA (0),
            mBodyB (0)
    {
    }
    // -------------------------------------------------------------------------
    TypedConstraint::TypedConstraint(RigidBody *bodyA) :
            mConstraint(0),
            mWorld(bodyA->getDynamicsWorld()),
            mBodyA (bodyA),
            mBodyB (0)
    {
            mBodyA = bodyA;
    }
    // -------------------------------------------------------------------------
    TypedConstraint::TypedConstraint(RigidBody *bodyA, RigidBody *bodyB) :
            mConstraint(0),
            mWorld(bodyA->getDynamicsWorld()),
            mBodyA (bodyA),
            mBodyB (bodyB)
    {
        assert (bodyA->getDynamicsWorld() == bodyB->getDynamicsWorld());
        mBodyA = bodyA;
        mBodyB = bodyB;
    }
    // -------------------------------------------------------------------------
    TypedConstraint::~TypedConstraint()
    {
        if (mConstraint)
        {
            assert (mWorld->isConstraintRegistered (this) == false);
            delete mConstraint;
        }
    }
    // -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	ActionInterface::ActionInterface(DynamicsWorld *world) :
	mActionInterface (0),
		mWorld (world),
		mBodyA (0),
		mBodyB (0)
	{
	}
	// -------------------------------------------------------------------------
	ActionInterface::ActionInterface(RigidBody *bodyA) :
	mActionInterface(0),
		mWorld(bodyA->getDynamicsWorld()),
		mBodyA (bodyA),
		mBodyB (0)
	{
		mBodyA = bodyA;
	}
	// -------------------------------------------------------------------------
	ActionInterface::ActionInterface(RigidBody *bodyA, RigidBody *bodyB) :
	mActionInterface(0),
		mWorld(bodyA->getDynamicsWorld()),
		mBodyA (bodyA),
		mBodyB (bodyB)
	{
		assert (bodyA->getDynamicsWorld() == bodyB->getDynamicsWorld());
		mBodyA = bodyA;
		mBodyB = bodyB;
	}
	// -------------------------------------------------------------------------
	ActionInterface::~ActionInterface()
	{
		if (mActionInterface)
		{
			//assert (mWorld->isConstraintRegistered (this) == false);
			delete mActionInterface;
		}
	}
	// -------------------------------------------------------------------------

}