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

#ifndef __NXOGRE_SHAPE_CONVEX_H__
#define __NXOGRE_SHAPE_CONVEX_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreShape.h"				// For: Primitives inherits Shape

namespace NxOgre {
	
	//////////////////////////////////////////////////////////////////////////////////////

	class NxExport Convex : public Shape {
	
		public:

			Convex(ConvexShape&, Actor*, NxArray<NxShapeDesc*>&);
			~Convex();

		protected:

			virtual bool isDynamic() {return false;}
			virtual bool isStaticOnly(){return false;}

			void _bindNxShapeToShape(NxShape*);

			NxConvexShape *mShape;			

		private:
	
	};

	//////////////////////////////////////////////////////////////////////////////////////

	class NxExport Prism : public Shape {
	
		public:

			Prism(PrismShape&, Actor*, NxArray<NxShapeDesc*>&);
			~Prism();

		protected:

			virtual bool isDynamic() {return false;}
			virtual bool isStaticOnly(){return false;}

			void _bindNxShapeToShape(NxShape*);

			NxConvexShape *mShape;			
	

		private:
	};

	//////////////////////////////////////////////////////////////////////////////////////

};

#endif
