/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2008 Caelum team. See Contributors.txt for details.

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

#ifndef CAELUM__FLAT_CLOUD_LAYER_H
#define CAELUM__FLAT_CLOUD_LAYER_H

#include "CaelumPrerequisites.h"
#include "ImageHelper.h"

namespace Caelum
{
    /** A flat cloud layer; drawn as a simple plane.
     *  Supports movement and variable cloud cover.
     *  @note This is tighly integrated with LayeredCloud.cg and LayeredClouds.material.
     */
	class CAELUM_EXPORT FlatCloudLayer
	{
	public:
		FlatCloudLayer(
				Ogre::SceneManager *sceneMgr,
                Ogre::SceneNode *cloudRoot,
                Ogre::Real height);

		~FlatCloudLayer();

        /** Update function called each frame from above.
         */
	    void update (
                Ogre::Real timePassed,
		        const Ogre::Vector3 &sunDirection,
		        const Ogre::ColourValue &sunLightColour,
		        const Ogre::ColourValue &fogColour,
				const Ogre::ColourValue &sunSphereColour);

	private:
        Ogre::Real mHeight;
	    Ogre::Real mCloudCover;

	    Ogre::Vector2 mCloudSpeed;
	    Ogre::Vector2 mCloudMassOffset;
	    Ogre::Vector2 mCloudDetailOffset;

		/// Current cloud blend position; from 0 to mNoiseTextureNames.size()
	    Ogre::Real mCloudBlendPos;
        /// Current index in the set of textures.
        /// Cached to avoid setting textures every frame.
        int mCurrentTextureIndex;

        /// Time required to blend two cloud shapes.
	    Ogre::Real mCloudBlendTime;

        std::vector<String> mNoiseTextureNames;

        /// Lookup used for cloud coverage, @see setCloudCoverLookup.
        std::auto_ptr<Ogre::Image> mCloudCoverLookup;

	    Ogre::MaterialPtr mMaterial;		
        Ogre::MeshPtr mMesh;
	    Ogre::Entity *mEntity;
	    Ogre::SceneNode *mNode;
	    Ogre::SceneManager *mSceneMgr;

	    Ogre::GpuProgramParametersSharedPtr getVpParams();
	    Ogre::GpuProgramParametersSharedPtr getFpParams();

        // Set various internal parameters:
	    void setCloudMassOffset(const Ogre::Vector2 &cloudMassOffset);
	    void setCloudDetailOffset(const Ogre::Vector2 &cloudDetailOffset);
	    void setSunDirection(const Ogre::Vector3 &sunDirection);
	    void setSunLightColour(const Ogre::ColourValue &sunLightColour);
		void setSunSphereColour(const Ogre::ColourValue &sunSphereColour);
	    void setFogColour(const Ogre::ColourValue &fogColour);

    public:
        /** Set the height of the cloud layer.
         *  @param height In world units above the cloud root node.
         */
        void setHeight(Ogre::Real height);

        /** Get the height of the cloud layer.
         *  @return height In world units above the cloud root node.
         */
        Ogre::Real getHeight() const;

        /** Sets cloud movement speed.
		 *  @param cloudSpeed Cloud movement speed.
		 */
		void setCloudSpeed (const Ogre::Vector2 &cloudSpeed);

		/** Gets cloud movement speed.
		 *  @param cloudSpeed Cloud movement speed.
		 */
        Ogre::Vector2 getCloudSpeed () const { return mCloudSpeed; }

		/** Sets cloud cover, between 0 (completely clear) and 1 (completely covered)
		 *  @param cloudCover Cloud cover between 0 and 1
		 */
		void setCloudCover (const Ogre::Real cloudCover);

		/** Gets the current cloud cover.
		 *  @return Cloud cover, between 0 and 1
		 */
        inline Ogre::Real getCloudCover () const { return mCloudCover; }

        /** Set the image used to lookup the cloud coverage threshold.
         *  This image is used to calculate the cloud coverage threshold
         *  based on the desired cloud cover.
         *
         *  The cloud coverage threshold is substracted from cloud intensity
         *  at any point; to generate fewer or more clouds. That threshold is
         *  not linear, a lookup is required to ensure that setCloudCover(0.1)
         *  will actually have 10% the clouds at setCloudCover(1).
         *
         *  The lookup is the inverse of the sum on the histogram, and was
         *  calculated with a small hacky tool.
         */
	    void setCloudCoverLookup (const Ogre::String& fileName);

        /** Disable any cloud cover lookup.
         *  @see setCloudCoverLookup.
         */
        void disableCloudCoverLookup ();

	    /** Sets the time it takes to blend two cloud shaped together, in seconds.
         *  This will also reset the animation at the current time.
		 *  @param value Cloud shape blend time in seconds
		 */
		void setCloudBlendTime (const Ogre::Real value);

		/** Gets the time it takes to blend two cloud shaped together, in seconds.
		 *  @return Cloud shape blend time in seconds
		 */
		Ogre::Real getCloudBlendTime () const;

        /** Set the current blending position; between noise textures.
         *  @param value New cloud blending position
         */
	    void setCloudBlendPos (const Ogre::Real value);

        /// @see setCloudBlendPos
        Ogre::Real getCloudBlendPos () const;

    public:
        void setQueryFlags (uint flags) { mEntity->setQueryFlags (flags); }
        uint getQueryFlags () const { return mEntity->getQueryFlags (); }
        void setVisibilityFlags (uint flags) { mEntity->setVisibilityFlags (flags); }
        uint getVisibilityFlags () const { return mEntity->getVisibilityFlags (); }
	};
}

#endif // CAELUM__FLAT_CLOUD_LAYER_H
