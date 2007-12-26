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
#include "NxOgrePrerequisites.h"
#include "NxOgreError.h"

namespace NxOgre {
	
	void NxThrow(NxString a, int b, NxString c, unsigned int d) {

//#if defined NX_DEBUG && (NX_CONSOLE_CHATTER == 1)
		std::cout << c << "#" << d << " (" << b << ")" << std::endl << a << std::endl;
//#endif

		if (Error::getInstance()) {
			ErrorReport rp = Error::getInstance()->getNewReport();
			rp.Message = a;
			rp.type = (ErrorReport::ErrorType) b;
			std::stringstream caller;
			caller << c << "#" << d;
			rp.Caller = caller.str();
			Error::getInstance()->report(rp);
		}

	}

}; //End of NxOgre namespace.
