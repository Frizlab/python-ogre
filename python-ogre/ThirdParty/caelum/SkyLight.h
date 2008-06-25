#ifndef SKYLIGHT_H
#define SKYLIGHT_H

namespace caelum
{
    /** Base class for sky lights (sun and moon).
     *  Contains a directional light which can be automatically disabled when too dim.
     */
    class CAELUM_EXPORT BaseSkyLight : public CameraBoundElement
    {
	protected:
		/// The main directional light.
		Ogre::Light *mMainLight;

		/// The sun scene node.
		Ogre::SceneNode *mNode;

		/// Base distance of the light.
		float mRadius;

		/// The latest normalised sun direction.
		Ogre::Vector3 mDirection;
		
		/// Body sphere colour, as set by setBodyColour
		Ogre::ColourValue mBodyColour;

		/// Sun light colour, as set by setLightColour
		Ogre::ColourValue mLightColour;

		/// Colour multiplier for light diffuse colour.
		Ogre::ColourValue mDiffuseMultiplier;

		/// Colour multiplier for light specular colour.
		Ogre::ColourValue mSpecularMultiplier;

		/** Colour multiplier for ambient light colour.
		 *  No effect, this value is only stored here.
		 */
		Ogre::ColourValue mAmbientMultiplier;

        /// If the light is automatically disabled beneath mAutoDisableThreshold
        bool mAutoDisableLight;

        /// Threshold beneath which the light is automatically disabled.
        Ogre::Real mAutoDisableThreshold;

        /// If the light is always disabled. Separate from the mAutoDisable mechanism.
        bool mForceDisableLight;

	public:
		/** Constructor.
			@param sceneMgr The scene manager where the lights will be created.
            @param caelumRootNode Root node to attach to. Should be bound to the camera.
		 */
		BaseSkyLight (
                Ogre::SceneManager *sceneMgr,
                Ogre::SceneNode *caelumRootNode);

		/// Destructor.
		virtual ~BaseSkyLight () = 0;

		/** Updates skylight parameters.
		 *  @param direction Light direction.
		 *  @param lightColour Color for the light source
		 *  @param bodyColour Color to draw the body of the light (whatever that is).
		 */
        virtual void update (
                const Ogre::Vector3& direction,
                const Ogre::ColourValue &lightColour,
                const Ogre::ColourValue &bodyColour);

		/// Retrieves the latest light direction.
		const Ogre::Vector3& getLightDirection () const;

		/// Set the sun direction.
		virtual void setLightDirection (const Ogre::Vector3 &dir);

		/// Get current body colour, as set in setBodyColour.
		Ogre::ColourValue getBodyColour () const;

		/// Sets the colour to draw the light's body with.
		virtual void setBodyColour (const Ogre::ColourValue &colour);

		/// Get current light colour, as set in setLightColour.
		Ogre::ColourValue getLightColour () const;

		/// Sets the skylight colour.
		virtual void setLightColour (const Ogre::ColourValue &colour);

		/// Set diffuse multiplier for light colour
		void setDiffuseMultiplier (const Ogre::ColourValue &diffuse);

		/// Set diffuse multiplier for light colour
		Ogre::ColourValue getDiffuseMultiplier () const;

		/// Set specular multiplier for light colour
		void setSpecularMultiplier (const Ogre::ColourValue &specular);

		/// Set specular multiplier for light colour
		Ogre::ColourValue getSpecularMultiplier () const;

		/// Set ambient multiplier for light colour
		/// This value is only stored here; the SceneManager is not touched
        /// However, CaelumSystem does use this value.
		void setAmbientMultiplier (const Ogre::ColourValue &ambient);

		/// Set ambient multiplier for light colour
		Ogre::ColourValue getAmbientMultiplier () const;

		/// Direct access to the Ogre::Light.
		Ogre::Light* getMainLight() const;

        /// Check if the light is automatically disabled.
        inline bool getAutoDisable() const { return mAutoDisableLight; }

        /** Turn on and off auto-disabling of the light when too dim.
         *  This is off by default. If you set it to true you probably also want to
         *  set the autoDisableThreshold.
         *  The "intensity" of the light for the threshold is calculated as the plain sum of r, g and b.
         */
        inline void setAutoDisable(bool value) { mAutoDisableLight = value; }

        /// Get the auto-disable threshold.
        inline Ogre::Real getAutoDisableThreshold() const { return mAutoDisableThreshold; }

        /// Set the auto-disable threshold.
        inline void setAutoDisableThreshold(Ogre::Real value) { mAutoDisableThreshold = value; }

        static const Ogre::Real DEFAULT_AUTO_DISABLE_THRESHOLD;

        /// Disable the light by force; without taking intensity into account.
        inline void setForceDisable(bool value) { mForceDisableLight = value; } 
        inline bool getForceDisable() const { return mForceDisableLight; }

    protected:
        /// Handle far radius.
	    virtual void setFarRadius (Ogre::Real radius);
		
		/// Temporary change main light color
		void setMainLightColour(const Ogre::ColourValue &colour);

        /// If the light should be enabled for a certain value.
        /// This functions takes AutoDisable and such into account.
        bool shouldEnableLight(const Ogre::ColourValue &colour);
};
}

#endif // SKYLIGHT_H
