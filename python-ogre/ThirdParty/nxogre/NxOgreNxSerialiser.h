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

#ifndef __NXOGRE_NxSerialiser_SERIALISER_H__
#define __NXOGRE_NxSerialiser_SERIALISER_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreSerialiser.h"

namespace NxOgre {
namespace Serialiser {
namespace NxSerialiser {

	class Serialiser {
			
		public:

			Serialiser(World*);
			~Serialiser();

#if (NX_USE_JSON == 1)
			bool importJSONFromFile(NxString filename);
#endif

#if (NX_USE_TINYXML == 1)
			bool importXMLFromFile(NxString filename);
#endif

			bool importBinaryFromFile(NxString filename);
			

		protected:

			World* mWorld;

	};
	
}; // end of NxSerialiser namespace
}; // end of Serialiser namespace
}; // end of NxOgre namespace

#endif
