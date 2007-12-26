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

#include "NxOgreStable.h"
#include "NxOgreBody.h"
#include "NxOgreActor.h"		// For: Body inherits Actor
#include "NxOgrePose.h"			// For: conversions
#include "NxOgreHelpers.h"		// For: conversions
#include "NxOgreScene.h"		// For: Actor::mOwner
#include "NxOgreShapeBlueprint.h"
#include "NxOgreContainer.h"
#include "NxOgreOgreNodeRenderable.h"
#include "NxOgreRenderable.h"
#include "NxOgreRenderableSource.h"
#include "NxOgreNodeRenderable.h"
#include "NxOgreSceneRenderer.h"
#include "NxOgreUserData.h"
#include "NxOgreHelpers.h"

namespace NxOgre {

//////////////////////////////////////////////////////////

Body::Body(const NxString& name, Scene* scene) : Actor(name, scene) {
	
}

//////////////////////////////////////////////////////////

Body::Body(const NxString& identifier, Scene* scene, ShapeBlueprint *firstShapeDescription, const Pose& pose, ActorParams params)
:	Actor(identifier, scene, firstShapeDescription, pose, params)
	
{

	NodeRenderableParams visualParams;

	visualParams.Identifier = identifier;
	visualParams.IdentifierType = visualParams.IT_CREATE;
	visualParams.Intent = getStringType();

	std::vector<NxString> splitID = Ogre::StringUtil::split(identifier, ";", 2);

	NxString visualIdentifier;

	if (splitID.size() == 1) {
		visualIdentifier = identifier;
	}
	else {
		visualIdentifier = splitID[1];
	}

	Ogre::StringUtil::trim(visualIdentifier);

	if (Ogre::StringUtil::startsWith(visualIdentifier, "(reference)")) {
		NxString entityName = visualIdentifier.substr(11, entityName.length() - 11);
		Ogre::StringUtil::trim(entityName);
		visualParams.GraphicsModel = entityName;
		visualParams.GraphicsModelType = visualParams.GMT_EXISTING_REFERENCE;
	}
	else {
		visualParams.GraphicsModel = visualIdentifier;
		visualParams.GraphicsModelType = visualParams.GMT_RESOURCE_IDENTIFIER;
	}

	setLevelOfDetail(LOD_High);
	mRenderable = mOwner->getSceneRenderer()->createNodeRenderable(visualParams);
	mNodeRenderable = static_cast<NodeRenderable*>(mRenderable);
	mNxActorUserData->RenderPtr = this;
	mDeltaPose = mLastPose = getGlobalPose();

	
	mOwner->getSceneRenderer()->registerSource(this);

#if 0

	if (params.mNodeName.length() == 0) {
		mNode = mOwner->getSceneManager()->createSceneNode();
	}
	else {

		if (mOwner->getSceneManager()->hasSceneNode(params.mNodeName)) {
			mNode = mOwner->getSceneManager()->getSceneNode(params.mNodeName);
			if (mNode->getParent() != NULL) {
				mNode->getParent()->removeChild(mNode);
			}
		}
		else {
			mNode = mOwner->getSceneManager()->createSceneNode(params.mNodeName);
		}
	}


	if (mActor->getMass()) {
		mOwner->getSceneManager()->getRootSceneNode()->addChild(mNode);
	}
	else {
		if (mOwner->getStaticGeometry()) {
			mOwner->getStaticGeometry()->addSceneNode(mNode);	
		}
		else {
			mOwner->getSceneManager()->getRootSceneNode()->addChild(mNode);
		}
	}

	
	if (Ogre::StringUtil::startsWith(mVisualIdentifier, "(entity)")) {
		
		NxString entityName = mVisualIdentifier.substr(8, entityName.length() - 8);
		Ogre::StringUtil::trim(entityName);
		mEntity = mOwner->getSceneManager()->getEntity(entityName);
	}
	else if (Ogre::StringUtil::endsWith(mVisualIdentifier, ".mesh")) {
		mEntity = mOwner->getSceneManager()->createEntity(mName + "-0", mVisualIdentifier);
		mEntity->setCastShadows(params.mNodeShadows);
	}

	if (mActor->getMass() == 0 && mOwner->getStaticGeometry()) {
		mOwner->getSceneManager()->getRootSceneNode()->removeChild(mNode);
		mOwner->getStaticGeometry()->addSceneNode(mNode);
	}

	if (params.mNodePose.isZero() && mEntity && mNode) {
		mNode->attachObject(mEntity);
	}
	else if (mEntity && mNode) {
		Ogre::SceneNode* childNode = mNode->createChildSceneNode(params.mNodePose.getVector3(), params.mNodePose.getQuaternion());
		childNode->attachObject(mEntity);
	}

	if (params.mNodeScale != Ogre::Vector3(1,1,1)) {
		mNode->scale(params.mNodeScale);
	}

	if (mNode) {
		mNode->setPosition(pose);
		mNode->setOrientation(pose);
	}

#endif
}

Body::Body(const NxString& identifier, Scene* scene, ShapeBlueprint *firstShapeDescription, const Pose& pose, NodeRenderableParams visualParams, ActorParams params)
:	Actor(identifier, scene, firstShapeDescription, pose, params)
{

	if (visualParams.Intent.size() == 0)
		visualParams.Intent = getStringType();

	setLevelOfDetail(LOD_High);
	setInterpolation(I_Absolute);
	mLastPoseCount = 0;
	mRenderable = mOwner->getSceneRenderer()->createNodeRenderable(visualParams);
	mNodeRenderable = static_cast<NodeRenderable*>(mRenderable);
	mNxActorUserData->RenderPtr = this;
	

	mOwner->getSceneRenderer()->registerSource(this);
}

//////////////////////////////////////////////////////////

Body::~Body() {

	mOwner->getSceneRenderer()->unregisterSource(this);

	if (mRenderable) {
		delete mRenderable;
		mRenderable = 0;
	}

#if 0
	if (mNode == 0)
		return;
	
	Ogre::SceneNode::ObjectIterator object_it = mNode->getAttachedObjectIterator();
	Ogre::MovableObject *m;
	while(object_it.hasMoreElements()) {
		m = object_it.getNext();
		if (m->getMovableType() == "Entity") {
			mOwner->getSceneManager()->destroyEntity((Ogre::Entity*) m);
		}
		else {
			mNode->detachObject(m);
			mOwner->getSceneManager()->getRootSceneNode()->attachObject(m);
		}
	}

	mNode->detachAllObjects();
	mNode->removeAndDestroyAllChildren();

	if (mNode)
		mOwner->getSceneManager()->destroySceneNode(mNode->getName());

#endif
	
}

//////////////////////////////////////////////////////////

void Body::__renderSelf() {

	switch (mInterpolation) {
		case I_Absolute:
			{
				mDeltaPose = getGlobalPose();
				mRenderable->setPose(mDeltaPose);
				mLastPose = mDeltaPose;
			}
		break;
	
		case I_Linear:
			{
				Pose blended_pose = NxInterpolate(mLastPose, getGlobalPose(), mOwner->getLastAlphaValue());
				mLastPose = getGlobalPose();
				mRenderable->setPose(blended_pose);
			}
		break;

		case I_Linear_x4:
			{
				mLastPoseCount++;

				if (mLastPoseCount == 4) {
					
					mDeltaPose = getGlobalPose();

					if (mDeltaPose.v != mLastPose.v) {
						mDeltaPose = NxInterpolate(mDeltaPose, mLastPose);
						mLastPose = getGlobalPose();
						mLastPoseCount = 0;
					}
					else {
						mLastPoseCount--;
						mDeltaPose = mLastPose;
					}

				}
				
				mRenderable->setPose(mDeltaPose);
			}
		break;

		case I_Linear_x8:
			{
				mLastPoseCount++;

				if (mLastPoseCount == 8) {
					
					mDeltaPose = getGlobalPose();

					if (mDeltaPose.v != mLastPose.v) {
						mDeltaPose = NxInterpolate(mDeltaPose, mLastPose);
						mLastPose = getGlobalPose();
						mLastPoseCount = 0;
					}
					else {
						mLastPoseCount--;
						mDeltaPose = mLastPose;
					}

				}
				
				mRenderable->setPose(mDeltaPose);
			}
		break;
	}

}

//////////////////////////////////////////////////////////

#if 0
void Body::renderTo(const Pose& pose) {
	mNode->setPosition(pose);
	mNode->setOrientation(pose);
}
#endif

//////////////////////////////////////////////////////////


/*
void Body::render(float dT) {

#if (NX_UNSTABLE_USE_SCENE_ACTIVE_TRANSFORM == 1)
#if (NX_UNSTABLE_ACCUMULATOR == 1)
   
	#define alpha dT

	assert ( alpha >= 0.0f && alpha <= 1.0f );

	NxMat34 nm = mActor->getGlobalPose();
	NxVec3 destPos = nm.t;
	NxQuat destOri = nm.M;

	NxVec3 renderPos = mPreviousPosition * ( 1.0f - alpha ) + destPos * alpha;
	NxQuat renderOri;
	renderOri.slerp( alpha , mPreviousOrientation, destOri );
	
	mNode->setPosition( NxConvert<Ogre::Vector3, NxVec3>(renderPos) );
	mNode->setOrientation( NxConvert<Ogre::Quaternion, NxQuat>(renderOri) );

	mPreviousPosition = destPos;
	mPreviousOrientation = destOri;

	shapeRender(0.0f); 

#else
	
	NxMat34 nm = mActor->getGlobalPose();
	NxVec3 nv(nm.t);
	NxQuat nq(nm.M);
	mNode->setPosition(nv.x, nv.y, nv.z);
	mNode->setOrientation(nq.w,nq.x,nq.y,nq.z);
	shapeRender(0);

#endif

#else
	if (mActor && mNode) {
		mNode->setPosition(getGlobalPosition());
		mNode->setOrientation(getGlobalOrientation());
		shapeRender(dT);
	}	
#endif

}
*/
#if 0
//////////////////////////////////////////////////////////

void Body::setNode(Ogre::SceneNode* node) {
	mNode = node;
}

//////////////////////////////////////////////////////////

void Body::setEntity(Ogre::Entity* entity) {
	mEntity = entity;
}

//////////////////////////////////////////////////////////
//
// Based of the work from http://www.ogre3d.org/wiki/index.php/DumpingNodeTree
//

void DumpNodes(StringPairList &l, Ogre::Node *n, int level,int nid) {

	Ogre::SceneNode::ObjectIterator object_it = ((Ogre::SceneNode *)n)->getAttachedObjectIterator();
	Ogre::Node::ChildNodeIterator node_it = n->getChildIterator();
	
	Ogre::MovableObject *m;
	Ogre::Entity *e;

	if (level != 0) {
		l.insert(
			"Node" + 
			Ogre::StringConverter::toString(nid) + 
			"-Position", 
			Ogre::StringConverter::toString(n->getPosition()));
	}
	
	int i=0;
	while(object_it.hasMoreElements()) {
		
		m = object_it.getNext();
		
		if (m->getMovableType() == "Entity") {
			e = static_cast<Ogre::Entity*>(m);
			NxString entitySuffix = "";
			
			
			if (i > 0)
				entitySuffix = Ogre::StringConverter::toString(i);
				
			if (level == 0) {
				l.insert("Entity" + entitySuffix, e->getMesh()->getName());
			}
			
			else {
				l.insert(
						"Node" + 
						Ogre::StringConverter::toString(nid) +
						"-Entity" +
						entitySuffix,
						e->getMesh()->getName()
					);
			}
		}
		i++;
	}

	while(node_it.hasMoreElements()) {
		DumpNodes(l, node_it.getNext(), level + 1, nid++);
	}

}

//////////////////////////////////////////////////////////

StringPairList Body::saveCustom() {
	
	StringPairList l;
	l.insert("ActorType", "Body");

	if (mNode == 0)
		return l;

	if (mNode->numChildren() > 0) {
		DumpNodes(l, mNode, 0, 1);
	}
	else {
		Ogre::Entity* entity = static_cast<Ogre::Entity*>(mNode->getAttachedObject(0));
		l.insert("Entity", entity->getMesh()->getName());
	}
	
	return l;
}

//////////////////////////////////////////////////////////

void	Body::restoreCustom(StringPairList spl) {

	mNode = mOwner->getSceneManager()->getRootSceneNode()->createChildSceneNode();

	for (StringPairList::StringPair sp = spl.begin();spl.hasNext();) {
		sp = spl.next();
/*
		std::stringstream ss;
		ss << sp.first;
		ss << " => ";
		ss << sp.second;
		ss << std::endl;

		NxDebug(ss.str());
*/
		NxString key = sp.first;
		Ogre::StringUtil::toLowerCase(key);

		if (key == "entity" || key == "node") {

			Ogre::Entity* entity = mOwner->getSceneManager()->createEntity(
				mName + "-" + sp.second + "-" + Ogre::StringConverter::toString(mNode->numAttachedObjects()), 
				sp.second
			);

			mNode->attachObject(entity);

		}

		if (key == "node-scale") {
			mNode->setScale(Ogre::StringConverter::parseVector3(sp.second));
		}

	}

	// Assign first Attached Object that is an entity to mEntity

	Ogre::SceneNode::ObjectIterator object_it = mNode->getAttachedObjectIterator();
	Ogre::MovableObject *m;
	while(object_it.hasMoreElements()) {
		m = object_it.getNext();
		if (m->getMovableType() == "Entity") {
			mEntity = (Ogre::Entity*) m;
			break;
		}
	}

	mNode->setPosition(getGlobalPosition());
	mNode->setOrientation(getGlobalOrientation());

}

//////////////////////////////////////////////////////////

void Body::disableVisualisation() {

	if (mNode->getParent() != NULL) {
		mNode->getParent()->removeChild(mNode);
	}

}

//////////////////////////////////////////////////////////

#endif 



StringPairList Body::saveCustom() {
	
	// Temp
	StringPairList l;
	return l;
}

//////////////////////////////////////////////////////////

void	Body::restoreCustom(StringPairList spl) {

	// TEMP
}

//////////////////////////////////////////////////////////

}  //End of NxOgre namespace.
