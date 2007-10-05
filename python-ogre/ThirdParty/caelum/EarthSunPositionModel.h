#ifndef EARTHSUNPOSITIONMODEL_H
#define EARTHSUNPOSITIONMODEL_H

#include "CaelumPrerequisites.h"
#include "SunPositionModel.h"

namespace caelum {

/** The simplified Sun position model for the earth.
	Based on experimental data and some simplifications (forumlas taken from the paper 
	"A Practical Analytic Model for Daylight", by Preetham et al.)
	@author Jesús Alonso Abad.
 */
class DllExport EarthSunPositionModel : public SunPositionModel {
// Attributes -----------------------------------------------------------------
	protected:
		/// Latitude
		Ogre::Radian mLatitude;

		/// Julian date
		Ogre::Real mJulianDate;

// Methods --------------------------------------------------------------------
	public:
		/** Basic constructor.
		 *	@param lat The latitude (in radians).
		 *	@param date The Julian date in the range [0, 1] (Jan 1st - Dec 31st respectively). 
		 *		Values above will be wrapped and below will be treated as positive.
		 */
		EarthSunPositionModel (const Ogre::Radian lat, const Ogre::Real date);
		
		/** Changes the current latitude.
		 * @param lat The new latitude in radians.
		 */
		void setLatitude (const Ogre::Radian lat);

		/** Gets the current latitude.
		 * @return The latitude in radians.
		 */
		Ogre::Radian getLatitude () const;

		/** Changes the Julian date.
		 * @param date The current Julian date in the range [0, 1] (Jan 1st - Dec 31st Respectively).
		 *		Values above will be wrapped and below will be treated as positive.
		 */
		void setJulianDate (const Ogre::Real date);

		/** Gets the Julian date.
		 * @return The Julian date in the range [0, 1].
		 */
		Ogre::Real getJulianDate () const;
		
		/** @copydoc SunPositionModel::update().
		 */
		Ogre::Vector3 update (const float time);
};

} // namespace caelum

#endif // EARTHSUNPOSITIONMODEL_H
