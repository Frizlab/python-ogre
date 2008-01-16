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
#include "NxOgreSceneRenderer.h"
#include "NxOgreScene.h"
#include "NxOgreRenderableSource.h"
#include "NxOgreUserData.h"

namespace NxOgre {

///////////////////////////////////////////////////////////////////////

SceneRenderer::SceneRenderer(Scene* scene, NxString rendererUserData) : mScene(scene) {
	mNxScene = mScene->getNxScene();
	mRenderAccuracy = RA_SOURCES;
}

///////////////////////////////////////////////////////////////////////

SceneRenderer::~SceneRenderer() {

}

///////////////////////////////////////////////////////////////////////

void SceneRenderer::setAccuracy(RenderAccuracy ra) {
	mRenderAccuracy = ra;
	
}

///////////////////////////////////////////////////////////////////////

void SceneRenderer::render() {

	switch (mRenderAccuracy) {
		case RA_ALL:
			renderAll();
		break;

		case RA_NX_TRANSFORM:
			{
				NxU32 nbActors = 0;
				NxActiveTransform* transform = mNxScene->getActiveTransforms(nbActors);
				if (nbActors)
					renderNxTransform(transform, nbActors);
			}
		break;

		case RA_SOURCES:
			{
				renderSources();
			}
		break;
	}
}

///////////////////////////////////////////////////////////////////////

void SceneRenderer::registerSource(NxOgre::RenderableSource *source) {
	NxRenderableSourceID id = mSources.count() + 1;
	source->mRenderableSourceID = id;
	mSources.insert(id, source);
}

///////////////////////////////////////////////////////////////////////

void SceneRenderer::unregisterSource(RenderableSource* source) {
	mSources.remove(source->mRenderableSourceID);
}

///////////////////////////////////////////////////////////////////////

void SceneRenderer::renderAll() {

}

///////////////////////////////////////////////////////////////////////

void SceneRenderer::renderNxTransform(NxActiveTransform* transform, NxU32 nbActors) {

	NxActorUserData* ud;
	
	for (NxU32 i=0;i < nbActors;++i) {
		
		ud = static_cast<NxActorUserData*>(transform[i].userData);
		
		if (ud == 0)
			continue;

		// (TODO) combine with this if with the above.
		if (ud->RenderPtr == 0)
			continue;

		ud->RenderPtr->__renderSelf();

	}

}

///////////////////////////////////////////////////////////////////////

void SceneRenderer::renderSources() {
	for (RenderableSource* r = mSources.begin();r = mSources.next();) {
		r->__renderSelf();
	}
}

///////////////////////////////////////////////////////////////////////

#if 0

///////////////////////////////////////////////////////////////////////

GenericSceneRenderer::GenericSceneRenderer(NxOgre::Scene *scene) : SceneRenderer(scene) {
}

///////////////////////////////////////////////////////////////////////

GenericSceneRenderer::~GenericSceneRenderer() {

}

///////////////////////////////////////////////////////////////////////

void GenericSceneRenderer::render() {
	
	Renderables renderable = mScene->getRenderable(Scene::RT_TRANSFORM);
	
	for (Actor* actor = renderable.actors.begin();actor = renderable.actors.next();) {
	
		actor->renderTo(actor->getGlobalPose());
		actor->shapeRender(0);
		actor->setVisualChanged(false);
/*
		if (actor->getType() == NxHashes_Actor)

		switch (actor->getType()) {

			// Actors.
			case NxHashes_Actor:
				continue;
			break;

			// Bodys
			case NxHashes_Body:
				{
				Body* body = static_cast<Body*>(actor);
				body->getNode()->setPosition(body->getGlobalPosition());
				body->getNode()->setOrientation(body->getGlobalOrientation());
				}
			break;
			
			// Inherited Actors which aren't bodies.
			default:
				actor->renderSelf();
			break;

		}
*/

	}

	
}

///////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////

AccumulatorSceneRenderer::AccumulatorSceneRenderer(NxOgre::Scene *scene) : SceneRenderer(scene) {
	
	SceneController* possibleController = mScene->getSceneController();
	
	if (possibleController->getType() == "Accumulator") {
		mController = static_cast<AccumulatorSceneController*>(possibleController);
	}
	else {
		NxThrow_Error("SceneController used with AccumulatorSceneRenderer is not a AccumulatorSceneController! NxOgre will crash.");
		mController = 0;
	}

}

///////////////////////////////////////////////////////////////////////

AccumulatorSceneRenderer::~AccumulatorSceneRenderer() {

	mAActors.destroyAndEraseAll();

}

///////////////////////////////////////////////////////////////////////

void AccumulatorSceneRenderer::render() {
	
	Renderables renderable = mScene->getRenderable(Scene::RT_TRANSFORM);
	
	NxReal alpha = mController->getAlpha();

	NxActor* nxactor;
	Pose	renderPose;
	NxMat34 destPose;
	NxQuat tQuat;
	NxQuat tQuat2;


	for (Actor* actor = renderable.actors.begin();actor = renderable.actors.next();) {
		
		nxactor = actor->getNxActor();

		AActor* aa = mAActors.get(actor);
		
		if (aa == 0) {
			aa = new AActor();
			aa->m = nxactor->getGlobalPose();
		}

		destPose = nxactor->getGlobalPose();
		destPose.t = aa->m.t * (1.0f - alpha) + destPose.t * alpha;
		destPose.M.toQuat(tQuat);
		aa->m.M.toQuat(tQuat2);
		tQuat.slerp(alpha, tQuat2, tQuat);
		destPose.M.fromQuat(tQuat);

		renderPose.v = destPose.t;
		destPose.M.toQuat(renderPose.q);

		actor->renderTo(renderPose);
		actor->shapeRender(0);

		aa->m = nxactor->getGlobalPose();

	}

	mLastKnownActorActivity = renderable.actors.count();
	mLastKnownActorCount = mScene->getNbActors();
}

#endif

}; //End of NxOgre namespace.
