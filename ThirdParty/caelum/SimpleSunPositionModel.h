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

#ifndef SIMPLESUNPOSITIONMODEL_H
#define SIMPLESUNPOSITIONMODEL_H

#include "CaelumPrerequisites.h"
#include "SunPositionModel.h"

namespace caelum {

/** A simple circular sun position model. 
	It just creates an inclinated circular orbit.
	@author Jesús Alonso Abad.
 */
class DllExport SimpleSunPositionModel : public SunPositionModel {
// Attributes -----------------------------------------------------------------
	protected:
		/** The sun trajectory inclination. This is similar to the earth's rotation axis inclination.
		 */
		Ogre::Degree mInclination;

// Methods --------------------------------------------------------------------
	public:
		/** Basic constructor.
			@param inc The orbit inclination.
		 */
		SimpleSunPositionModel (Ogre::Degree inc);
		
		/** Changes the orbit inclination.
			@param inc The inclination in degrees.
		 */
		void setInclination (Ogre::Degree inc);

		/** Gets the orbit inclination.
			@return The orbit inclination in degrees.
		 */
		Ogre::Degree getInclination () const;

		/** @copydoc SunPositionModel::update().
		 */
		Ogre::Vector3 update (const float time);
};

} // namespace caelum

#endif // SIMPLESUNPOSITIONMODEL_H
