/** \file    NxOgreResourceSystem.h
 *  \brief   Header for the ResourceSystem class.
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

#ifndef __NXOGRE_RESOURCE_SYSTEM_H__
#define __NXOGRE_RESOURCE_SYSTEM_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreResourceStreamPtr.h"
#include "NxStream.h"

namespace NxOgre {

	/////////////////////////////////////////////////////////////////////

	class NxPublicClass ResourceSystem {
		
		public:
	
			ResourceSystem() {}
			virtual ~ResourceSystem() {}

			virtual void init()	{}

			virtual ResourceStream*		getStream(ResourceIdentifier);
			virtual ResourceStream*		getStream(ResourceIdentifier, void*);
			
			/** \brief		Load a convex shape from the resource system
				\param		The identifier to use.
				\returns	The modified identifier of the convex (usually stripped
							file extensions or directory names). String should be
							blank if the convex mesh could not be loaded, obvious
							errors will be thrown in this event as usual.
			*/
			virtual NxString loadConvex(const NxString&) const {return NxString();};

			// TODO: To remove, or to refine?
			virtual ResourceAccessList	getAllNXS()						{ResourceAccessList list; return list;}
			virtual ResourceAccessList	getAllMESH()					{ResourceAccessList list; return list;}
			virtual ResourceAccessList	getAllNXM()						{ResourceAccessList list; return list;}
			virtual ResourceAccessList	getAllEndingWith(NxString)		{ResourceAccessList list; return list;}
			virtual void				loadThese(ResourceAccessList)	{}

			NxString	getIdentifier(NxString ResourceSystemID, NxString ResourceName, bool write);

			Ogre::ResourceGroupManager*	mResourceManager;

	protected:

		
	};

	/////////////////////////////////////////////////////////////////////

};

#endif
