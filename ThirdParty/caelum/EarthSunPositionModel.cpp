#include "CaelumPrecompiled.h"
#include "EarthSunPositionModel.h"

namespace caelum {

EarthSunPositionModel::EarthSunPositionModel (const Ogre::Radian lat, const Ogre::Real date) {
	setLatitude (lat);
	setJulianDate (date);
}

void EarthSunPositionModel::setLatitude (const Ogre::Radian lat) {
	mLatitude = lat;
}

Ogre::Radian EarthSunPositionModel::getLatitude () const {
	return mLatitude;
}

void EarthSunPositionModel::setJulianDate (const Ogre::Real date) {
	mJulianDate = date;
}

Ogre::Real EarthSunPositionModel::getJulianDate () const {
	return mJulianDate;
}

Ogre::Vector3 EarthSunPositionModel::update (const float time) {
	Ogre::Radian declination, zenith, azimuth; // Variables
	Ogre::Real x, y, sinL, sinDelta, sinTime, cosL, cosDelta, cosTime, twoPi;	// Recurrent values (speed up calculations)

	twoPi = 2 * Ogre::Math::PI;
				
	declination = Ogre::Radian (0.4093 * Ogre::Math::Sin ((twoPi * (mJulianDate * 365 - 81)) / 368.0));

	sinL = Ogre::Math::Sin (mLatitude);
	cosL = Ogre::Math::Cos (mLatitude);
	sinDelta = Ogre::Math::Sin (declination);
	cosDelta = Ogre::Math::Cos (declination);
	sinTime = Ogre::Math::Sin (twoPi * time);
	cosTime = Ogre::Math::Cos (twoPi * time);

	x = (cosL * sinDelta - sinL * cosDelta * cosTime);
	y = -cosDelta * sinTime;
	
	zenith = Ogre::Radian (Ogre::Math::PI * 0.5) - Ogre::Math::ASin (sinL * sinDelta + cosL * cosDelta * cosTime);
	azimuth = Ogre::Math::ATan (y / x);

	if (x < 0)
		azimuth += Ogre::Radian (Ogre::Math::PI);
	else if (y < 0 && x > 0)
		azimuth += Ogre::Radian (twoPi);

	Ogre::Vector3 dir = Ogre::Vector3::UNIT_Y;
	Ogre::Quaternion rotZen, rotAzi;

	rotZen = Ogre::Quaternion (zenith, Ogre::Vector3::UNIT_X);
	rotAzi = Ogre::Quaternion (azimuth, Ogre::Vector3::UNIT_Y);

	dir = rotAzi * (rotZen * dir);

	mSunPosition = dir.normalisedCopy () * -1;

	mSunPosition.z *= -1;
	
	return mSunPosition;
}

} // namespace caelum
