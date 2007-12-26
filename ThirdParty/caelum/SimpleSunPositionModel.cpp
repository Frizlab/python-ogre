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

#include "CaelumPrecompiled.h"
#include "SimpleSunPositionModel.h"

namespace caelum {

SimpleSunPositionModel::SimpleSunPositionModel (Ogre::Degree inc) {
	setInclination (inc);
}

void SimpleSunPositionModel::setInclination (Ogre::Degree inc) {
	mInclination = inc;
}

Ogre::Degree SimpleSunPositionModel::getInclination () const {
	return mInclination;
}

Ogre::Vector3 SimpleSunPositionModel::update (const float time) {
	// Get the inclinated axis
	Ogre::Vector3 axis = Ogre::Vector3::UNIT_Z;
	axis = Ogre::Quaternion (mInclination, Ogre::Vector3::UNIT_X) * axis;

	// Get the inclinated light direction, according to the day time
	Ogre::Vector3 dir = Ogre::Vector3::UNIT_Y;
	dir = Ogre::Quaternion (Ogre::Radian (time * 2 * Ogre::Math::PI), axis) * dir;

	mSunPosition = dir.normalisedCopy () * -1;

	return mSunPosition * -1;
}

} // namespace caelum
