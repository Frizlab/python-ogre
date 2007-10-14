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

namespace NxOgre {

//////////////////////////////////////////////////////////

Body::Body(const NxString& name, Scene* scene) : Actor(name, scene) {
	
}

//////////////////////////////////////////////////////////

Body::Body(const NxString& identifier, Scene* scene, ShapeBlueprint *firstShapeDescription, const Pose& pose, ActorParams params)
:	mNode(0), mEntity(0), Actor(identifier, scene, firstShapeDescription, pose, params)
	
{


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
		mNode->attachObject(mEntity);
		
	}
	else if (Ogre::StringUtil::endsWith(mVisualIdentifier, ".mesh")) {
		mEntity = mOwner->getSceneManager()->createEntity(mName + "-0", mVisualIdentifier);
		mEntity->setCastShadows(params.mNodeShadows);
		mNode->attachObject(mEntity);
	}

	if (mActor->getMass() == 0 && mOwner->getStaticGeometry()) {
		mOwner->getSceneManager()->getRootSceneNode()->removeChild(mNode);
		mOwner->getStaticGeometry()->addSceneNode(mNode);
	}

	if (params.mNodeScale != Ogre::Vector3(1,1,1)) {
		mNode->scale(params.mNodeScale);
	}

	if (mNode) {
		mNode->setPosition(pose);
		mNode->setOrientation(pose);
	}


}

//////////////////////////////////////////////////////////

Body::~Body() {

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

	
}

//////////////////////////////////////////////////////////

void Body::simulate(float dT) {
	shapeSimulate(dT);
}

//////////////////////////////////////////////////////////

void Body::render(float) {
#if (NX_UNSTABLE_USE_SCENE_ACTIVE_TRANSFORM == 1)

	NxMat34 nm = mActor->getGlobalPose();
	NxVec3 nv(nm.t);
	NxQuat nq(nm.M);
	mNode->setPosition(nv.x, nv.y, nv.z);
	mNode->setOrientation(nq.w,nq.x,nq.y,nq.z);
	shapeRender(0);
#else
	if (mActor && mNode) {
		mNode->setPosition(getGlobalPosition());
		mNode->setOrientation(getGlobalOrientation());
		shapeRender(dT);
	}	
#endif
}

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

}  //End of NxOgre namespace.
