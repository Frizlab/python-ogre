/** \file    NxOgreWhen.h
 *  \brief   Header for the namespace When and the ...
 *  \version 1.0-21
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

#ifndef __NXOGRE_WHEN_H__
#define __NXOGRE_WHEN_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreContactCallback.h"

namespace NxOgre {
namespace When {

	enum Conditions {
		Asleep,
		Awake,
		AtPosition,
		NotAtPosition,
		InArea,
		NotInArea,
		AtPose,
		NotAtPose,
		Created,
		Destroyed,
		Hit,
		HitBy,
		NewFrame,
		NewOtherFrame,
	};

	enum Nouns {
		Actor,
		Character,
		Shape,
		Scene
	};

	class Condition {

	};

	class Action {

	};

}; // End of When namespace.
}; // End of NxOgre namespace.

#endif
