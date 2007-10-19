#ifndef UNIVERSALCLOCK_H
#define	UNIVERSALCLOCK_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** The system's time model.
 * This class is responsible of keeping track of the current time, transform it to the system's time model, 
 * and return the values associated (time of day, day of year...).
 * @author Jesús Alonso Abad.
 */
class DllExport UniversalClock {
// Attributes -----------------------------------------------------------------
	protected:
		/// Number of seconds per day.
		Ogre::Real mSecondsPerDay;

		/// Number of days per year.
		Ogre::Real mDaysPerYear;

		/// Number of seconds per year.
		Ogre::Real mSecondsPerYear;

		/// The current second of the year.
		Ogre::Real mCurrentSecond;

		/// The time scale.
		Ogre::Real mTimeScale;

		/// The update rate.
		Ogre::Real mUpdateRate;

		/// The time elapsed since the last update.
		Ogre::Real mTimeSinceLastUpdate;

// Methods --------------------------------------------------------------------
	public:
		/** Standard constructor.
		 * @note Will set the initial values to the earth's approximation (24 hours/day and 365 days/year).
		 */
		UniversalClock ();

		/** Sets the total number of seconds a day has in this time model.
		 * @param secs The number of seconds per day.
		 */
		void setSecondsPerDay (const Ogre::Real secs);

		/** Gets the number of seconds a day currently has.
		 * @return The seconds per day.
		 */
		Ogre::Real getSecondsPerDay () const;

		/** Sets the total number of days a year has in this time model.
		 * @param days The number of days per year.
		 */
		void setDaysPerYear (const Ogre::Real days);

		/** Gets the number of days a year currently has.
		 * @return The days per year.
		 */
		Ogre::Real getDaysPerYear () const;

		/** Sets the current time in a "second per year" fashion.
		 * @param sec The current second relative to the start of the year. 
		 * 	If the value is larger than the year length, it will be wrapped.
		 * 	Negative values will be treated as positive.
		 */
		void setCurrentTime (const Ogre::Real sec);

		/** Sets the current year time in the range [0, 1].
		 * @param time The relative year time.
		 * 	If the value is greater than 1, it will be wrapped.
		 * 	Negative values will be treated as positive.
		 */
		void setCurrentRelativeTime (const Ogre::Real time);

		/** Gets the current time.
		 * @return The second relative to the start of the year.
		 */
		Ogre::Real getCurrentTime () const;

		/** Gets the current relative year time.
		 * @return The relative time respective to the whole year length.
		 */
		Ogre::Real getCurrentRelativeTime () const;

		/** Gets the current day time.
		 * @return The second relative to the start of the current day.
		 */
		Ogre::Real getCurrentDayTime () const;

		/** Gets the current relative day time.
		 * @return The relative time respective to the whole day length.
		 */
		Ogre::Real getCurrentRelativeDayTime () const;

		/** Sets the time scale.
		 * @param scale The new time scale. If negative, time will move backwards; 2.0 means double speed...
		 */
		void setTimeScale (const Ogre::Real scale);

		/** Gets the time scale.
		 * @return The current time scale. Defaults to 1.
		 */
		Ogre::Real getTimeScale () const;

		/** Sets the update rate.
		 * @param rate A positive number representing the number of seconds to be elapsed (in real time) since 
		 * 	the last update. Negative numbers will be clamped to 0 (update every frame).
		 */
		void setUpdateRate (const Ogre::Real rate);

		/** Gets the update rate.
		 * @return The amount of time to be elapsed (at least) between updates.
		 */
		Ogre::Real getUpdateRate () const;

		/** Updates the clock.
		 * @param time The time to be added to the clock. Note this will be affected by the time scale.
		 * @return True if an update has been fired (depending on the update rate).
		 */
		bool update (const Ogre::Real time);

	private:
		/** Updates the number of seconds per year according to the seconds per day and days per year.
		 */
		void updateSecondsPerYear ();

		/** Queries an update next time the clock is updated.
		 */
		void forceUpdate ();
};

} // namespace caelum

#endif //UNIVERSALCLOCK_H
