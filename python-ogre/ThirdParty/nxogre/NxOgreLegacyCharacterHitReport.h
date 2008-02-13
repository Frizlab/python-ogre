/** \file    NxOgreLegacyCharacterHitReport.h
 *  \legacy
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

#ifndef __NXOGRE_CHARACTER_CONTROLLER_HIT_REPORT_H__
#define __NXOGRE_CHARACTER_CONTROLLER_HIT_REPORT_H__

#if (NX_USE_LEGACY_NXCONTROLLER == 1)
#if (NX_USE_CHARACTER_API == 1)

#include "NxOgrePrerequisites.h"
#include "NxController.h"

namespace NxOgre {

	class NxPublicClass CharacterHitReport {
	
	public:

		enum Response {

			// No response, Hit was just monitored or recorded. Pass the hit through any additional reporters and BaseCharacterHitReport
			RS_None = 0xFF,

			// Push the actor, and stop any additional hit reports.
			RS_Push = 0x01,
	
			// Don't push the actor, and stop any additional hit reports.
			RS_PassThrough = 0x02 

		};

		virtual Response onActor(Character*, Actor*, Shape*, ActorGroup*, const NxControllerShapeHit&) {return RS_None;}
		virtual Response onCharacterHit(Character*, Actor*, Shape*, ActorGroup*, const NxControllersHit&) {return RS_None;}

	};

	class NxPublicClass BaseCharacterHitReport : public CharacterHitReport {

	public:

		Response onActor(Character*, Actor*, Shape*, ActorGroup*, const NxControllerShapeHit&);
		Response onCharacterHit(Character*, Actor*, Shape*, ActorGroup*, const NxControllersHit&);

	};

	// Other HitReports are executed first, if they return a true, then rest are not executed.
	//
	//class myHitReport : public CharacterHitReport {
	//	
	//	CharacterHitReport::Response onActorHit(Character*, Actor*, Shape*, ActorGroup*) {
	//		std::cout << "Actor Hit!!" << std::endl;
	//	}
	//
	//	CharacterHitReport::Response onCharacterHit(Character*, Actor*, Shape*, ActorGroup*) {
	//		std::cout << "Character Hit!!" << std::endl;
	//	}
	//};
	//
	// ...
	// myHitReport* report = new myHitReport();
	// mWorld->getCharacterController()->addHitReport(report);
	// ...
	// delete report;
	//

}; // End of namespace

#endif
#endif
#endif