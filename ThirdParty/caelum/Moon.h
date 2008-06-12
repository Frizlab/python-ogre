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

#ifndef MOON_H
#define MOON_H

#include "CaelumPrerequisites.h"
#include "Sun.h"

namespace caelum {

/** Class representing the moon.
 *	The moon is always full.
 */
class CAELUM_EXPORT Moon : public BaseSkyLight {
// Attributes -----------------------------------------------------------------
	public:
		/// Name of the moon material.
		static const Ogre::String MOON_MATERIAL_NAME;

	private:
		/// The moon sprite.
		Ogre::BillboardSet* mMoonBillboardSet;
		
		/// Reference to the moon material.
		Ogre::MaterialPtr mMaterial;

		/// The moon sprite visible angle
		Ogre::Degree mMoonTextureAngularSize;

		Ogre::GpuProgramParametersSharedPtr getFpParams();

// Methods --------------------------------------------------------------------
	public:
		/** Constructor.
		 */
		Moon (
				Ogre::SceneManager *sceneMgr,
				Ogre::SceneNode *caelumRootNode,
				const Ogre::String& moonTextureName = "moon_disc.dds", 
				const Ogre::Degree& moonTextureAngularSize = Ogre::Degree(3.77f)); // 0.53f is real angular size of Moon and Moon, 3.77f is compatible with SphereMoon

		virtual ~Moon ();

		/** Updates the moon material.
			@param textureName The new moon texture name.
		 */
		void setMoonTexture (const Ogre::String &textureName);
		
		/** Updates the moon size.
			@param moon TextureAngularSize The new moon texture angular size.
		 */
		void setMoonTextureAngularSize(const Ogre::Degree& moonTextureAngularSize);

		/** Sets the moon sphere colour.
			@param colour The colour used to draw the moon
		 */
		void setBodyColour (const Ogre::ColourValue &colour);

		/// Set the moon's phase
		void setPhase (Ogre::Real phase);

    public:
		/// Handle camera change.
		virtual void notifyCameraChanged (Ogre::Camera *cam);
};

} // namespace caelum

#endif //MOON_H
