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

#ifndef __NXOGRE_NODERENDERABLE_H__
#define __NXOGRE_NODERENDERABLE_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreRenderable.h"
#include "NxOgreParams.h"
#include "NxOgrePose.h"

namespace NxOgre {
	
		/** NodeRenderableParams a string or class abstract way of describing how a NodeRenderable
			is with a generalisation of the RenderSystem.
		*/
		class NxExport NodeRenderableParams : public Params {

		public:


			NodeRenderableParams()				{
													setToDefault();
												}

			NodeRenderableParams(const char* p)	{
													process(p);
												}

			NodeRenderableParams(NxString p)				{
													process(p);
												}

			/** @brief Set's the Params to default based on NxActorDesc and NxBodyDesc defaults.
			*/
			void			setToDefault();

			/** Identifier for the Renderable. Leave blank for a random generated one.
				@default Blank String -> Random generated name when created.
				@string "identifier"
			*/
			NxString		Identifier;

			/** Usage of Identifier; Create implementation, or use a pre-existing one.
				@string "identifer-usage"
				@default IT_CREATE
			*/
			enum IdentifierUsage {
				// "create"
				IT_CREATE,
				// "use"
				IT_USE		
			};

			IdentifierUsage IdentifierType;

			/** Graphics Model identifier. SceneRenderer specific
				@string "model"
			*/
			NxString		GraphicsModel;

			enum GraphicsModelIdentifierType {
				// "resource"
				GMT_RESOURCE_IDENTIFIER,	// Model to fetch from RenderSystem's ResourceSystem
				//	"file"
				GMT_FILE,					// Model to load from file. RenderSystem specific
				// "reference"
				GMT_EXISTING_REFERENCE		// Model already loaded; use reference.
			};

			/** @brief Graphics Model Identifier Type.
				@string "model-type"
				@default GMT_RESOURCE_IDENTIFIER
			*/
			GraphicsModelIdentifierType GraphicsModelType;

			/** Scale of the Graphics Model.
				@string "scale"
				@default 1 1 1
			*/
			NxVec3			GraphicsModelScale;

			/** @brief The initial pose.
				@string "position", "orientation"
			*/
			Pose			GraphicsModelPose;

			/** Offset from the center of the pose.
				@string "offset-position", "offset-orientation"
				@default Pose()
			*/
			Pose			GraphicsModelOffset;

			/** RenderMaterial Override
				@string "material"
				@default Blank String
			*/
			NxString		GraphicsModelMaterial;

			/** @brief For "SubNodeRenderable behaviour, allowing you to tell the NodeRenderable
					   or the SceneRender what the NodeRenderable is intended for, which then
					   the NodeRenderable/SceneRender can be more accommodating.

					   For most classes NodeRenderable in NxOgre, they may adjust the Parameters
					   to add their type into the Intent string for you, unless you've added 
					   it yourself.

			   @default Blank String, may be adjusted when Parameters are handed to a class.
			*/ 
			NxString		Intent;

			private:
			
			//! \internal
			void			parse(Parameters);

	}; // End of NodeRenderableParams class

	/** @brief Self-contained renderable, with a model that can be frequency moved around
		Similar to the Ogre system of a SceneNode with an Entity attached. Multiple models
		are subject to the RenderSystem the Renderable is working with.
		@abstract
	*/
	class NxExport NodeRenderable : public Renderable {

		public:

			NodeRenderable(NodeRenderableParams, SceneRenderer*);

			virtual void setPose(const Pose&) = 0;
			virtual Pose getPose() const = 0;

			virtual void setMaterial(const NxString&) = 0;
			virtual NxString getMaterial() const = 0;

			virtual void setScale(const Ogre::Vector3&) = 0;
			virtual void setScale(const NxVec3&) = 0;
			virtual Ogre::Vector3 getScale() const = 0;
			virtual NxVec3 getScaleAsNxVec3() const = 0;

			virtual void setOffset(const Pose&) = 0;
			virtual Pose getOffset() const = 0;

			virtual NxString getType() {return "NxOgre-NodeRenderable";}
			virtual NxShortHashIdentifier getHashType() const {return 42719;}

	};
	
	//////////////////////////////////////////////////////////

};

#endif
