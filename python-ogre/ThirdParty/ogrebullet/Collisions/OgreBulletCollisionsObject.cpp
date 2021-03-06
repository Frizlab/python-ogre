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

#include "OgreBulletCollisionsObject.h"
#include "Debug/OgreBulletCollisionsDebugShape.h"

#include "OgreBulletCollisionsObjectState.h"
#include "OgreBulletCollisionsShape.h"
#include "OgreBulletCollisionsWorld.h"

using namespace Ogre;
using namespace OgreBulletCollisions;

namespace OgreBulletCollisions
{
    //-----------------------------------------------------------------------
    const Ogre::String Object::mMovableType = "OgreBullet::Object";
    // -------------------------------------------------------------------------
    Object::Object(const String &name, CollisionsWorld *world, bool init)
        :	
        MovableObject(name),
#if (OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR < 7)
        UserDefinedObject(),
#endif
        mWorld(world),
        mShape(0),
        mState(0),
        mRootNode(0),
        mBounds(Vector3::ZERO, Vector3::ZERO),
        mDebugShape(0),
        mShapeNode(0),
        mDebugNode(0)
    {
        if (init)
        {
            mObject = new btCollisionObject();
            mState = new ObjectState(this);
        }
    }
    // -------------------------------------------------------------------------
    Object::~Object()
    {
        if (mRootNode)
        {
            showDebugShape(false);
            mShapeNode->detachObject (this);
            mRootNode->removeAndDestroyChild (mShapeNode->getName ());
            //mRootNode->getParentSceneNode ()->removeAndDestroyChild (mRootNode->getName ());
        }

        getBulletCollisionWorld()->removeCollisionObject( mObject );
		getCollisionWorld()->removeObject(this);

        delete mObject;        
        //delete mShape;
        delete mState;
        delete mDebugShape;
    }
    //-----------------------------------------------------------------------
    
    btCollisionObject*  Object::getBulletObject(void) const { return mObject;};
    btCollisionWorld*  Object::getBulletCollisionWorld() const { return mWorld->getBulletCollisionWorld ();};
    CollisionsWorld*  Object::getCollisionWorld() const { return mWorld;};

    CollisionShape * Object::getShape() const{ return mShape;};
    DebugCollisionShape* Object::getDebugShape() const{ return mDebugShape;};
    const Ogre::Vector3 & Object::getWorldPosition() const {return mRootNode->_getDerivedPosition();};
    const Ogre::Quaternion & Object::getWorldOrientation() const {return mRootNode->_getDerivedOrientation();};

    void Object::setPosition(const Ogre::Vector3 &p) {mRootNode->setPosition (p);};
    void Object::setOrientation(const Ogre::Quaternion &q)  {return mRootNode->setOrientation (q);};

    void Object::setPosition(const Ogre::Real x, const Ogre::Real y, const Ogre::Real z) {mRootNode->setPosition (x,y,z);};
    void Object::setOrientation(const Ogre::Real x, const Ogre::Real y, const Ogre::Real z, const Ogre::Real w)  {return mRootNode->setOrientation (x,y,z,w);};

    
    
    
    void Object::showDebugShape(bool show)
    {
        if (show && mDebugShape == 0 && mShape)
        {
            mDebugShape = new DebugCollisionShape(mShape);
            if (mDebugShape->getIsVisual ())
            {
                assert (mDebugNode == 0);
                mDebugNode = mRootNode->createChildSceneNode(mName + "DebugShape");
                mDebugNode->attachObject (mDebugShape);
            }
        }
        else if (mDebugShape)
        {
            if (mDebugShape->getIsVisual ())
            {
                assert (mDebugNode);
                mDebugNode->detachObject (mDebugShape->getName());
                mRootNode->removeAndDestroyChild (mDebugNode->getName());
                mDebugNode = 0;
            }
            assert (mDebugNode == 0);
            delete mDebugShape;
            mDebugShape = 0;
        }
    }
    // -------------------------------------------------------------------------
    void Object::setTransform(const btVector3 &pos, const btQuaternion &quat)
    { 
        mRootNode->setPosition(pos[0], pos[1], pos[2]);
        mRootNode->setOrientation(quat.getW(),quat.getX(), quat.getY(), quat.getZ());
    }
    // -------------------------------------------------------------------------
    void Object::setPosition(const btVector3 &pos)
    {
		mRootNode->setPosition(pos[0], pos[1], pos[2]);
    }
    // -------------------------------------------------------------------------
    void Object::setOrientation(const btQuaternion &quat)
    {   
		mRootNode->setOrientation(quat.getW(),quat.getX(), quat.getY(), quat.getZ());
    }
    // -------------------------------------------------------------------------
    void Object::setTransform(const btTransform& worldTrans)
    { 
        mRootNode->setPosition(worldTrans.getOrigin()[0], worldTrans.getOrigin()[1],worldTrans.getOrigin()[2]);
        mRootNode->setOrientation(worldTrans.getRotation().getW(),worldTrans.getRotation().getX(), worldTrans.getRotation().getY(), worldTrans.getRotation().getZ());
    }
    //-----------------------------------------------------------------------
    void Object::setShape(CollisionShape *shape, 
        const Vector3 &pos, 
        const Quaternion &quat)
    {
        mShape = shape;

        mRootNode = mWorld->getSceneManager()->getRootSceneNode()->createChildSceneNode(mName, pos, quat);
        mShapeNode = mRootNode->createChildSceneNode(mName + "Shape");
        mShapeNode->attachObject(this);

        mObject->setCollisionShape(shape->getBulletShape());
        showDebugShape(mWorld->getShowDebugShapes()); 

		mObject->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
		mObject->getWorldTransform().setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));

    }
    // -------------------------------------------------------------------------
    //-----------------------------------------------------------------------
    void Object::_notifyAttached(Node* parent, bool isTagPoint)
    {
        MovableObject::_notifyAttached(parent,isTagPoint);
        if (parent)
        {
            Object* other_object = mWorld->findObject(static_cast<SceneNode*>(parent));
            if ((other_object) && (other_object != this))
            {
                static_cast<SceneNode*>(parent)->detachObject(other_object);

            }
            setPosition(parent->getPosition());
            setOrientation(parent->getOrientation());
        } 
    }
#if (OGRE_VERSION >=  ((1 << 16) | (5 << 8) | 0)) // must have at least shoggoth (1.5.0)
    //-----------------------------------------------------------------------
	void Object::visitRenderables(Renderable::Visitor* visitor, 
		bool debugRenderables)
	{
		//visitor->visit(this, 0, false);
	}
#endif
    //-----------------------------------------------------------------------
    const Ogre::String& Object::getMovableType() const
    {
        return mMovableType;
    }

    //-----------------------------------------------------------------------
    void Object::_notifyCurrentCamera(Camera* camera)
    {
    }

    //-----------------------------------------------------------------------
    const AxisAlignedBox& Object::getBoundingBox(void) const
    {
        return mBounds;
    }
    //-----------------------------------------------------------------------
    Real Object::getBoundingRadius(void) const
    {
        return Ogre::Real(0.0);
    }

    //-----------------------------------------------------------------------
    void Object::_updateRenderQueue(RenderQueue* queue)
    {
    }
}

