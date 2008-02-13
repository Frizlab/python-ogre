#if 0
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

#ifndef __NXOGRE_SHAPEBLUEPRINT__H__
#define __NXOGRE_SHAPEBLUEPRINT__H__

#include "NxOgrePrerequisites.h"
#include "NxOgreShape.h"			// For Shape::mParams

namespace NxOgre {

	class NxExport ShapeBlueprint {

		friend class Actor;
		friend class ActorBlueprint;
		friend class Trigger;
		friend class ForceField;
		friend class CompoundShape;

		public:

			ShapeBlueprint();
			ShapeBlueprint(const ShapeParams&);
			virtual ~ShapeBlueprint() {}

			static ShapeBlueprint* getDefaultShape();
		
			bool isShared()			{return mShared;}

			/** setShared
				set's the Actor to be "shared". Allowing to be attached to multiple actors. Actors
				do not also delete shared shapes on construction of the NxActor.

				@param s	If it is shared or not.
			 */
			void setShared(bool s)	{mShared = s;}
		
			ShapeParams mParams;

		protected:

			virtual bool isDynamic()	{return false;}
			virtual bool isStaticOnly()	{return false;}
			virtual Shape* _bindToActorDescription(Actor*, NxShapeIndex id, NxArray<NxShapeDesc*>&) {return 0;}
			virtual Shape* _bindToNxActor(Actor*, NxShapeIndex) {return 0;}

			void __genericParamsToDescription(NxShapeDesc& desc, const ShapeParams& p, Scene* s);
		
			
			NxU32		mID;
			bool		mShared;

		private:

	};


};

#endif
#endif