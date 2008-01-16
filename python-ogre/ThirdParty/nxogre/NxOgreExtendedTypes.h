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

#ifndef __NXOGRE_EXTENDED_TYPES_H__
#define __NXOGRE_EXTENDED_TYPES_H__

namespace NxOgre {

	///////////////////////////////////

	class NxVec2 {
		
		public:

			NxVec2() : x(0), y(0) {}
			NxVec2(NxReal X, NxReal Y) : x(X), y(Y) {}

			float x, y;

		protected:
			

		private:

	};

	///////////////////////////////////

};

#endif


