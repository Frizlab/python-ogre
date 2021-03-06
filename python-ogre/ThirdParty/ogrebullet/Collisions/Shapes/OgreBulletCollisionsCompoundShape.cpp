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
#include "Utils/OgreBulletConverter.h"

#include "Shapes/OgreBulletCollisionsCompoundShape.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"

using namespace Ogre;
using namespace OgreBulletCollisions;

namespace OgreBulletCollisions
{
    // -------------------------------------------------------------------------
    CompoundCollisionShape::CompoundCollisionShape():	
        CollisionShape()
    {
            mShape = new btCompoundShape();
	}
	//---------------------------------------------------------
	CompoundCollisionShape::CompoundCollisionShape(btCompoundShape *shape):	
		CollisionShape()
	{
		mShape = shape;
		
		// TODO : create a list of child ogre bullet collision shapes using child list, recursively		
		btCompoundShape *cShapes = static_cast <btCompoundShape *> (mShape);
		unsigned int numChildren = cShapes->getNumChildShapes();
		for (unsigned int i = 0; i < numChildren; i++)	
		{
			CollisionShape *s = 0;
			switch (cShapes->getChildList()[i].m_childShapeType)
			{
			case CONVEX_HULL_SHAPE_PROXYTYPE:
				s = new ConvexHullCollisionShape(static_cast<btConvexHullShape*> (cShapes->getChildList()[i].m_childShape));
				break;
			//case :
			//	break;
			default :
				break;
			}
			if (s)
				mShapes.push_back (s);
		}
	}
    // -------------------------------------------------------------------------
    CompoundCollisionShape::~CompoundCollisionShape()
    {
    }
    // -------------------------------------------------------------------------
    void CompoundCollisionShape::addChildShape(CollisionShape *shape, const Vector3 &pos, const Quaternion &quat)
    {
        btTransform localTrans;
        
        //localTrans.setIdentity();
        //localTrans effectively shifts the center of mass with respect to the chassis
		localTrans.setOrigin (OgreBtConverter::to(pos));
		localTrans.setRotation (OgreBtConverter::to(quat));

        static_cast <btCompoundShape *> (mShape)->addChildShape(localTrans, shape->getBulletShape());
    
        mShapes.push_back (shape);
    }
    // -------------------------------------------------------------------------
    bool CompoundCollisionShape::drawWireFrame(DebugLines *wire, 
        const Ogre::Vector3 &pos, 
        const Ogre::Quaternion &quat) const
    {
        bool isVisual = false;


        btCompoundShape * const myBtCompoundShape = static_cast <btCompoundShape *> (mShape);
        int numChildShapes = myBtCompoundShape->getNumChildShapes ();

        int i;
        for (std::vector<CollisionShape *>::const_iterator itShape = mShapes.begin(); 
            itShape != mShapes.end(); ++itShape)
        {
            const btCollisionShape * const shape = (*itShape)->getBulletShape();
            for (i = 0; i < numChildShapes; i++)
            {
                if (myBtCompoundShape->getChildShape (i) == shape)
                    break;
            }
            assert (i < numChildShapes);

            const btTransform &localTrans = myBtCompoundShape->getChildTransform (i);

            const Vector3 pos (BtOgreConverter::to(localTrans.getOrigin ()));
            const Quaternion quat( BtOgreConverter::to(localTrans.getRotation ()));

            if ((*itShape)->drawWireFrame(wire, pos, quat))
                isVisual = true;
        }
        return isVisual;
		
    }
}

