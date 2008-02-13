/** \file    NxOgreLegacyCharacterHitReport.cpp
 *  \see     NxOgreLegacyCharacterHitReport.h
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

#if (NX_USE_LEGACY_NXCONTROLLER == 1)
#if (NX_USE_CHARACTER_API == 1)
#	include "NxOgreLegacyCharacterHitReport.h"
#	include "NxOgreActor.h"			// Actor::AddForceAtLocalPos and getMass
#	include "NxOgreLegacyCharacter.h"

namespace NxOgre {


////////////////////////////////////////////////////////////////////////////////////////////////

CharacterHitReport::Response BaseCharacterHitReport::onActor(Character* c, Actor* a, Shape*, ActorGroup*, const NxControllerShapeHit& h) {
	
	if (!a->isDynamic())
		return RS_Push;

	if (a->hasBodyFlag(NX_BF_KINEMATIC))
		return RS_Push;

	NxReal coeff = c->getMass() * h.length;
	a->getNxActor()->addForceAtLocalPos(h.dir * coeff, NxVec3(0,0,0), NX_IMPULSE);

	return RS_Push;
}

////////////////////////////////////////////////////////////////////////////////////////////////

CharacterHitReport::Response BaseCharacterHitReport::onCharacterHit(Character*, Actor*, Shape*, ActorGroup*, const NxControllersHit&) {

	return RS_Push;
}

////////////////////////////////////////////////////////////////////////////////////////////////

}  //End of NxOgre namespace.
#endif
#endif