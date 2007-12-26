/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2007 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SUNPOSITIONMODEL_H
#define SUNPOSITIONMODEL_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** Interface for all the sun positioning models. 
		A sun position model is a configurable utility/model that will return sun's sky coordinates (direction/azimuth in the 
		form of a normalised vector) depending on various parameters. 
		These models can range from a simple circular (sin/cos) oscillation, to complex equations for accurate positioning.
		@author Jesús Alonso Abad.
 */
class DllExport SunPositionModel {
// Attributes -----------------------------------------------------------------
	protected:
		/** The last calculated sun position.
		 */
		Ogre::Vector3 mSunPosition;

// Methods --------------------------------------------------------------------
	public:
		/** Destructor
		 */
		virtual ~SunPositionModel () {};

		/** Updates the sun position.
				@param time Local time.
				@return The sun light direction (or the negated sun position).
		 */
		virtual Ogre::Vector3 update (const float time) = 0;
};

} // namespace caelum

#endif // SUNPOSITIONMODEL_H
