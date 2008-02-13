/** \file    NxOgreSceneRenderer.cpp
 *  \see     NxOgreSceneRenderer.h
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
	source->mRenderableSourceID = mSources.Size() + 1;
	mSources.Insert(source);
}

///////////////////////////////////////////////////////////////////////

void SceneRenderer::unregisterSource(RenderableSource* source) {
	mSources.Remove(mSources.WhereIs(source));
}

///////////////////////////////////////////////////////////////////////

void SceneRenderer::renderAll() {

}

///////////////////////////////////////////////////////////////////////

void SceneRenderer::renderNxTransform(NxActiveTransform* transform, NxU32 nbActors) {

	NxUserData* ud;
	
	for (NxU32 i=0;i < nbActors;++i) {
		
		ud = static_cast<NxUserData*>(transform[i].userData);
		
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
	mSources.Each(&RenderableSource::__renderSelf);
}

///////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
