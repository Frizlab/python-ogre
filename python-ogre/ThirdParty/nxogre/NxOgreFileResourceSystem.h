/** \file    NxOgreFileResourceSystem.h
 *  \brief   Header for the FileResourceSystem class.
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

#ifndef __NXOGRE_FILE_RESOURCE_SYSTEM_H__
#define __NXOGRE_FILE_RESOURCE_SYSTEM_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreResourceSystem.h"
#include "NxOgreResourceStream.h"
#include "NxStream.h"

namespace NxOgre {

	/////////////////////////////////////////////////////////////////////

	/** \brief File based resource system.
		Uses the operating system standard directory and file function to access
		standard PhysX and NxOgre files. Native file format is the Wavefront *.obj
		mesh format.
	*/
	class NxPublicClass FileResourceSystem : public ResourceSystem {
		
		public:

			FileResourceSystem();
			~FileResourceSystem();

			void init();

			//virtual NxString loadConvex(const NxString&) const = 0;

			virtual ResourceStream*		getStream(ResourceIdentifier);
			virtual ResourceStream*		getStream(ResourceIdentifier, void*);
			virtual ResourceAccessList	getAllNXS()						{ResourceAccessList list; return list;}
			virtual ResourceAccessList	getAllMESH()					{ResourceAccessList list; return list;}
			virtual ResourceAccessList	getAllNXM()						{ResourceAccessList list; return list;}
			virtual ResourceAccessList	getAllEndingWith(NxString)		{ResourceAccessList list; return list;}
			virtual void				loadThese(ResourceAccessList)	{}

	};

	/////////////////////////////////////////////////////////////////////

};

#endif
