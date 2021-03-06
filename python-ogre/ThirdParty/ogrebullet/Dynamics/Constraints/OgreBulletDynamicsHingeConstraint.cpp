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

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsConstraint.h"
#include "Constraints/OgreBulletDynamicsHingeConstraint.h"

using namespace Ogre;

namespace OgreBulletDynamics
{
    // -------------------------------------------------------------------------
    HingeConstraint::HingeConstraint(RigidBody * rbA, RigidBody * rbB, const Vector3& pivotInA,
        const Vector3& pivotInB, const Vector3& axisInA, const Vector3& axisInB):
        TypedConstraint(rbA, rbB)
    {
        btVector3 vec[4];
        vec[0] = OgreBulletCollisions::OgreBtConverter::to(pivotInA);
        vec[1] = OgreBulletCollisions::OgreBtConverter::to(pivotInB);
        vec[2] = OgreBulletCollisions::OgreBtConverter::to(axisInA);
        vec[3] = OgreBulletCollisions::OgreBtConverter::to(axisInB);
       
        mConstraint = new btHingeConstraint(
            *rbA->getBulletRigidBody (),
            *rbB->getBulletRigidBody (),
            vec[0], vec[1], vec[2], vec[3]);
/*
        mConstraint = new btHingeConstraint(
            *rbA->getBulletRigidBody (),
            *rbB->getBulletRigidBody (), 
            OgreBulletCollisions::OgreBtConverter::to(pivotInA),
            OgreBulletCollisions::OgreBtConverter::to(pivotInB), 
            OgreBulletCollisions::OgreBtConverter::to(axisInA), 
            OgreBulletCollisions::OgreBtConverter::to(axisInB));*/
    }
    // -------------------------------------------------------------------------
        HingeConstraint::HingeConstraint(RigidBody * rbA, 
            const Vector3& pivotInA, 
            const Vector3&  axisInA):
    TypedConstraint(rbA)
    {
        btVector3 vec[2];
        vec[0] = OgreBulletCollisions::OgreBtConverter::to(pivotInA);
        vec[1] = OgreBulletCollisions::OgreBtConverter::to(axisInA);

        mConstraint = new btHingeConstraint(*rbA->getBulletRigidBody (),
            vec[0], vec[1]);

/*
        mConstraint = new btHingeConstraint(*rbA->getBulletRigidBody (),
            OgreBulletCollisions::OgreBtConverter::to(pivotInA),
            OgreBulletCollisions::OgreBtConverter::to(axisInA));*/
    }
    // -------------------------------------------------------------------------
    HingeConstraint::~HingeConstraint()
    {
    }
}
