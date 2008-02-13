/** \file    NxOgreTrigger.h
 *  \brief   Header for the Trigger and TriggerCallback classes.
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

#ifndef __NXOGRE_TRIGGER_H__
#define __NXOGRE_TRIGGER_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreSimpleActor.h"

namespace NxOgre {

	class NxPublicClass Trigger : public SimpleActor {

		friend class SceneTriggerController;
		friend class NxUserData;

		public:

			Trigger(SimpleShape*, TriggerContactCallback*, Scene*);
			~Trigger();


			/** \brief Get the callback associated with this trigger
			*/
			TriggerContactCallback* getCallback() {
			                                       return mCallback;
			                                      }

			void addSimpleShape(SimpleShape*);


			Actors getActors();

		protected:

			TriggerContactCallback* mCallback;
			NxUserData*             mUserData;

	};

}; // End of NxOgre namespace.

#endif
