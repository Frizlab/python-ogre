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

#include "OgreBulletCollisions.h"

#include "Shapes/OgreBulletCollisionsConeShape.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"

using namespace Ogre;
using namespace OgreBulletCollisions;

namespace OgreBulletCollisions
{
    // -------------------------------------------------------------------------
    ConeCollisionShape::ConeCollisionShape(Real radius, Real height, const Vector3 &axe):	
        CollisionShape()
    {
        if (axe == Vector3::UNIT_Y)
            mShape = new btConeShape (btScalar (radius), btScalar (height));
        else if (axe == Vector3::UNIT_X)
            mShape = new btConeShapeX (btScalar (radius), btScalar (height));
        else
             mShape = new btConeShapeZ (btScalar (radius), btScalar (height));

    }
    // -------------------------------------------------------------------------
    ConeCollisionShape::~ConeCollisionShape ()
    {
    }
}

