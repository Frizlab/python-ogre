#ifndef CAELUM_HEADER__IMAGE_HELPER_H
#define CAELUM_HEADER__IMAGE_HELPER_H

#include "CaelumPrerequisites.h"

namespace caelum
{
	/** Gets the interpolated colour between two pixels from an image.
        Interpolate a texture pixel by hand. (fx, fy) are in texture coordinates,
        ranging [0-1] across the entire texture.
        Smooth blending is only done on the x coordinate.
        Wrapping is only supported on X as well.

		@param fx Horizontal coordinate.
		@param fy Vertical coordiate.
		@param img The lookup image.
		@param wrapX To wrap the x coordinate.
		@return The interpolated colour.
	 */
	DllExport Ogre::ColourValue getInterpolatedColour (float fx, float fy, Ogre::Image *img,
            bool wrapX = true);
}

#endif // CAELUM_HEADER__IMAGE_HELPER_H