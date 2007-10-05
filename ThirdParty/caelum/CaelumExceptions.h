#ifndef CAELUMEXCEPTIONS_H
#define CAELUMEXCEPTIONS_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** Exception class for unsupported features.
 * @author Jesús Alonso Abad.
 */
class DllExport UnsupportedException : public Ogre::Exception {
	public:
		/** Constructor.
		 */
		UnsupportedException (int number, const Ogre::String &description, const Ogre::String &source, const char *file, long line)
						: Ogre::Exception (number, description, source, "UnsupportedException", file, line) { }
};

} // namespace caelum

#endif // CAELUMEXCEPTIONS_H
