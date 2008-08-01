/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2008 Caelum team. See Contributors.txt for details.

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

#ifndef CAELUM__CAELUM_SYSTEM_H
#define CAELUM__CAELUM_SYSTEM_H

#include "CaelumPrerequisites.h"

namespace Caelum
{
    /** Root of the Caelum system.
     *  
     *  Caelum is built from several classes for different sky elements (the sun,
     *  clouds, etc). Those classes know very little about each other and are 
     *  connected through the main CaelumSystem class. This class is responsible
     *  for tracking and updating sub-components. It "owns" all of the components,
     *  using std::auto_ptr members.

        The constructor will create a standard set of components but you can
        disable some or change others. When you do something like setXxx(new Xxx())
        CaelumSystem takes control of the object's lifetime.

        This class is also reponsible for doing all per-frame and per-RenderTarget
        updates. It's better to keep that logic here instead of coupling components
        together.
     */
    class CAELUM_EXPORT CaelumSystem: public Ogre::FrameListener, public Ogre::RenderTargetListener
    {
	private:
		/// Root of the Ogre engine.
		Ogre::Root *mOgreRoot;

		/// Scene manager.
		Ogre::SceneManager *mSceneMgr;

        /// Caelum scene node for camera-bound elements (most).
		Ogre::SceneNode *mCaelumCameraNode;

        /// Caelum scene node for ground-bound elements (only clouds currently).
		Ogre::SceneNode *mCaelumGroundNode;

		/// Cleanup requested flag.
		bool mCleanup;

        /// Automatically move the camera node.
        bool mAutoMoveCameraNode;

        /// Automatically call this->notifyCameraChanged.
        bool mAutoNotifyCameraChanged;

        /// Automatically attach compositors to viewports
        bool mAutoAttachViewports;

        /// Automatically set the viewport colour to black.
        bool mAutoViewportBackground;

        /// Flag to indicate if Caelum manages standard Ogre::Scene fog.
		bool mManageSceneFog;

        /// Global fog density multiplier.
        double mGlobalFogDensityMultiplier;

        /// Scene fog density multiplier.
        double mSceneFogDensityMultiplier;

        /// Multiply the colour of the scene fog.
        Ogre::ColourValue mSceneFogColourMultiplier;

        /// Ground fog density multiplier.
        double mGroundFogDensityMultiplier;

        /// Multiply the colour of the ground fog.
        Ogre::ColourValue mGroundFogColourMultiplier;

        /// Flag for managing scene ambient light.
		bool mManageAmbientLight;

        /// Minimum ambient light; only useful if mManageAmbientLight
        Ogre::ColourValue mMinimumAmbientLight;

        /// If only one light source should enabled at a time.
        bool mEnsureSingleLightSource;

        /// Ensure only one of the light sources casts shadows.
        bool mEnsureSingleShadowSource;

		/// The sky gradients image (for lookups).
        std::auto_ptr<Ogre::Image> mSkyGradientsImage;

        /// The sun gradients image (for lookups).
        std::auto_ptr<Ogre::Image> mSunColoursImage;

        /// Observer Latitude (on the earth).
        Ogre::Degree mObserverLatitude;
        /// Observer Longitude (on the earth).
        Ogre::Degree mObserverLongitude;

        static const Ogre::Vector3 makeDirection (
                Ogre::Degree azimuth, Ogre::Degree altitude);
		
		// References to sub-components
        std::auto_ptr<UniversalClock> mUniversalClock;
        std::auto_ptr<SkyDome> mSkyDome;
        std::auto_ptr<BaseSkyLight> mSun;
        std::auto_ptr<Moon> mMoon;
        std::auto_ptr<ImageStarfield> mImageStarfield;
        std::auto_ptr<PointStarfield> mPointStarfield;
        std::auto_ptr<GroundFog> mGroundFog;
        std::auto_ptr<CloudSystem> mCloudSystem;
		std::auto_ptr<PrecipitationController> mPrecipitationController;

	public:
        /** Flags enumeration for caelum components.
         *  This is an enumeration for the components to create by default in
         *  Caelum's constructor. You can still pass 0 and create everything
         *  by hand.
         * 
         *  CaelumSystem's constructor used to take a number of bools but now
         *  there are too many components and this is nicer.
         * 
         *  CAELUM_COMPONENT_ members are for individual components.
         *  CAELUM_COMPONENTS_ are standard bitmasks.
         *  CAELUM_COMPONENTS_DEFAULT picks elements that don't require
         *  modifications to external materials (right now it excludes ground fog).
         */
        enum CaelumComponent
        {
            CAELUM_COMPONENT_SKY_DOME           = 1 << 1,
            CAELUM_COMPONENT_MOON				= 1 << 3,
            CAELUM_COMPONENT_SUN                = 1 << 4,
            CAELUM_COMPONENT_IMAGE_STARFIELD    = 1 << 5,
            CAELUM_COMPONENT_POINT_STARFIELD    = 1 << 6,
            CAELUM_COMPONENT_CLOUDS             = 1 << 7,
            CAELUM_COMPONENT_PRECIPITATION      = 1 << 8,

            // This has nasty dependencies on materials.
            CAELUM_COMPONENT_GROUND_FOG         = 1 << (16 + 0),

            // Groups
            CAELUM_COMPONENTS_NONE              = 0,
            CAELUM_COMPONENTS_DEFAULT           = 0
                    | CAELUM_COMPONENT_SKY_DOME
                    | CAELUM_COMPONENT_MOON
                    | CAELUM_COMPONENT_SUN
                    | CAELUM_COMPONENT_POINT_STARFIELD
                    | CAELUM_COMPONENT_CLOUDS,
            CAELUM_COMPONENTS_ALL               = 0
                    | CAELUM_COMPONENTS_DEFAULT
                    | CAELUM_COMPONENT_PRECIPITATION
                    | CAELUM_COMPONENT_GROUND_FOG,
        };

        static const String DEFAULT_SKY_GRADIENTS_IMAGE;
        static const String DEFAULT_SUN_COLOURS_IMAGE;
    
		/** Constructor.
		 *  Registers itself in the Ogre engine and initialises the system.
         *
         *  @param root The Ogre root.
		 *  @param scene The Ogre scene manager.
		 *  @param componentsToCreate Default components for @see autoConfigure.
		 */
		CaelumSystem (
                Ogre::Root *root, 
				Ogre::SceneManager *sceneMgr,
                CaelumComponent componentsToCreate);

        /** Revert everything to defaults.
         *  This function will delete all subcomponents and revert everything
         *  to defaults values.
         */
        void clear ();

        /** Create some default component with resonable default settings.
         *  This results in a slightly cloudy morning sky.
         */
        void autoConfigure (
                CaelumComponent componentsToCreate);

		/** Destructor.
		 */
		~CaelumSystem ();

		/** Shuts down the system and detaches itself from the Ogre engine.
         *
         *  shutdown(true) is equivalent to deleting CaelumSystem yourself.
         *  shutdown(false) delays destruction to the next time caelum is called as
         *  a frame listener. This makes it safe to shutdown Caelum from inside
         *  another frame listener.
         *
         *  @param cleanup If this is true then detach and destroy the CaelumSystem instantly.
		 */
		void shutdown (bool cleanup);

        /** Update the whole system manually.
         *  You have to call this yourslef if you don't register CaelumSystem
         *  as an ogre frame listener. Otherwise it's called automatically.
         *
         *  @param timeSinceLastFrame: Time passed since last frame.
         */
        void updateSubcomponents (double timeSinceLastFrame);

        /** Notify subcomponents of camera changes.
         *  This function must be called after camera changes but before
         *  rendering with that camera. If multiple cameras are used it must
         *  be called for each camera before the camera is rendered with.
         *
         *  This function will move caelum's camera node to the camera
         *  position, but only if getAutoMoveCameraNode.
         *  It will also call CameraBoundElement::notifyCameraChanged
         */
        void notifyCameraChanged(Ogre::Camera* cam);

		/// Gets root scene node for camera-bound elements
        inline Ogre::SceneNode* getCaelumCameraNode(void) const { return mCaelumCameraNode; }

		/// Gets root scene node for ground-bound elements.
        inline Ogre::SceneNode* getCaelumGroundNode(void) const { return mCaelumGroundNode; }

        /** If true; listen to preViewportUpdate and automatically notifyCameraChanged();
         *  This is on by default; but does not work with compositors.
         *  This only works for attached render targets; see @attachRenderTarget.
         */
        inline void setAutoNotifyCameraChanged(bool value) { mAutoNotifyCameraChanged = value; }
        /// @see setAutoNotifyCameraChanged
        inline bool getAutoNotifyCameraChanged() const { return mAutoNotifyCameraChanged; }

        /** If true; automatically attach viewports to subcomponents that use compositors.
         *  Compositors need to be attached to individual viewports.
         */
        inline void setAutoAttachViewports(bool value) { mAutoAttachViewports = value; }
        /// @see setAutoNotifyCameraChanged
        inline bool getAutoAttachViewports() const { return mAutoAttachViewports; }

        /** If true (default); automatically move the camera node in notifyCameraChanged.
         *  If disable you get full control of the camera node; and can attach
         *  it to the scene graph however you please.
         */
        inline void setAutoMoveCameraNode(bool value) { mAutoMoveCameraNode = value; }
        /// @see setAutoMoveCameraNode
        inline bool getAutoMoveCameraNode() { return mAutoMoveCameraNode; }

        /** If true; automatically set the viewport color to black.
         *  Caelum's domes relies on the viewport background being black.
         *  There's generally no reason to disable this.
         */
        inline void setAutoViewportBackground(bool value) { mAutoViewportBackground = value; }
        /// @see setAutoViewportBackground
        inline bool getAutoViewportBackground() const { return mAutoViewportBackground; }

        /// Get the observer's longitude. East is positive, west is negative.
        inline const Ogre::Degree getObserverLongitude () const { return mObserverLongitude; }

        /// Set the observer's longitude. East is positive, west is negative.
        inline void setObserverLongitude (Ogre::Degree value) { mObserverLongitude = value; }

        /// Get the observer's latitude. North is positive, south is negative.
        inline const Ogre::Degree getObserverLatitude () const { return mObserverLatitude; }

        /// Set the observer's latitude. North is positive, south is negative.
        inline void setObserverLatitude (Ogre::Degree value) { mObserverLatitude = value; }

		void attachRenderTarget (Ogre::RenderTarget* rt);
		void detachRenderTarget (Ogre::RenderTarget* rt);

        /** Attach CaelumSystem to a viewport.
         *  If getAutoAttachViewports() this will add Caelum's compositors.
         */
		void attachViewport (Ogre::Viewport* rt);

        /// Reverse of @see attachViewport.
		void detachViewport (Ogre::Viewport* rt);
        
    public:
		/// Gets the universal clock.
        inline UniversalClock *getUniversalClock () const { return mUniversalClock.get(); }

		/// Get the current sky dome, or null if disabled.
        inline SkyDome* getSkyDome () const { return mSkyDome.get (); }
		/// Set the skydome, or null to disable.
        inline void setSkyDome (SkyDome *obj) { mSkyDome.reset (obj); }

		/// Gets the current sun, or null if disabled.
        inline BaseSkyLight* getSun () const { return mSun.get (); }
		/// Set the sun, or null to disable.
		inline void setSun (BaseSkyLight* obj) { mSun.reset (obj); }

		/// Gets the current moon, or null if disabled.
        Moon* getMoon () const { return mMoon.get (); }
		/// Set the moon, or null to disable.
		inline void setMoon (Moon* obj) { mMoon.reset (obj); }

		/// Gets the current image starfield, or null if disabled.
        inline ImageStarfield* getImageStarfield () const { return mImageStarfield.get (); }
		/// Set image starfield, or null to disable.
        inline void setImageStarfield (ImageStarfield* obj) { mImageStarfield.reset (obj); }

		/// Gets the current point starfield, or null if disabled.
        inline PointStarfield* getPointStarfield () const { return mPointStarfield.get (); }
		/// Set image starfield, or null to disable.
        inline void setPointStarfield (PointStarfield* obj) { mPointStarfield.reset (obj); }

		/// Sets ground fog system, or null to disable.
        inline void setGroundFog (GroundFog *model) { mGroundFog.reset(model); }

		/// Get ground fog; if enabled.
        inline GroundFog* getGroundFog () const { return mGroundFog.get(); }

        /// Set cloud system; or null to disable.
        inline void setCloudSystem (CloudSystem *value) { mCloudSystem.reset(value); }

        /// Get cloud system; or null if disabled.
        inline CloudSystem* getCloudSystem () const { return mCloudSystem.get(); }

        /// Set precipitation controller; or null to disable.
		inline void setPrecipitationController (PrecipitationController *value) {
            mPrecipitationController.reset(value);
        }

        /// Get precipitation controller; or null if disabled.
		inline PrecipitationController* getPrecipitationController () const {
            return mPrecipitationController.get();
        }

		/** Enables/disables Caelum managing standard Ogre::Scene fog.
            This makes CaelumSystem control standard Ogre::Scene fogging. It
            will use EXP2 fog with density from SkyColourModel.

            Fog density multipliers are used; final scene fog density is:
            SceneMultiplier * GlobalMultiplier * SkyColourModel.GetFogDensity

            When this is set to false it also disables all scene fog (but you
            control it afterwards).

            @param value New value
		 */
		void setManageSceneFog (bool value);

		/** Tells if Caelum is managing the fog or not.
			@return The value set in setManageSceneFog.
		 */
		bool getManageSceneFog () const;

        /** Multiplier for scene fog density (default 1).
            This is an additional multiplier for Ogre::Scene fog density.
            This has no effect if getManagerSceneFog is false.

            Final scene fog density is:
            SceneMultiplier * GlobalMultiplier * SkyColourModel.GetFogDensity
         */
        void setSceneFogDensityMultiplier (double value);

        /** Get the value set by setSceneFogDensityMultiplier.
         */
        double getSceneFogDensityMultiplier () const;

        /** Set an additional multiplier for fog colour as it comes from SkyColourModel.
         *  This is 0.7 by default; to be compatible with previous versions.
         */
        inline void setSceneFogColourMultiplier (const Ogre::ColourValue& value) { mSceneFogColourMultiplier = value; }

        /// See setSceneFogColourMultiplier.
        inline Ogre::ColourValue getSceneFogColourMultiplier () const { return mSceneFogColourMultiplier; }

        /** Multiplier for ground fog density (default 1).
            This is an additional multiplier for Caelum::GroundFog density.
            This has no effect if GroundFog is not used.

            Final ground fog density is:
            GroundFogMultipler * GlobalMultiplier * SkyColourModel.GetFogDensity
         */
        void setGroundFogDensityMultiplier (double value);

        /** Get the value set by setGroundFogDensityMultiplier.
         */
        double getGroundFogDensityMultiplier () const;

        /** Set an additional multiplier for ground fog colour as it comes from SkyColourModel.
         *  This is OgreColour::White by default; which has no effect.
         */
        inline void setGroundFogColourMultiplier (const Ogre::ColourValue& value) { mGroundFogColourMultiplier = value; }

        /// See setGroundFogColourMultiplier.
        inline Ogre::ColourValue getGroundFogColourMultiplier () const { return mGroundFogColourMultiplier; }

        /** Multiplier for global fog density (default 1).
            This is an additional multiplier for fog density as received from
            SkyColourModel. There are other multipliers you can tweak for
            individual kinds of fog; but this is what you should change from
            whatever "game logic" you might have.
         */
        void setGlobalFogDensityMultiplier (double value);

        /** Get the value set by setSceneFogDensityMultiplier.
         */
        double getGlobalFogDensityMultiplier () const;

        /** Set this to true to have CaelumSystem manage the scene's ambient light.
         *  The colour and AmbientMultiplier of the sun and moon are used.
         *  This is false by default.
         */
        inline void setManageAmbientLight (bool value) { mManageAmbientLight = value; }

        /// Check if CaelumSystem is managing ambient lighting.
        inline bool getManageAmbientLight () const { return mManageAmbientLight; }

        /** Set the minimum value for scene ambient lighting,
         *  This is only used if getManageAmbientLight() is true.
         *  By default this value is Ogre::ColourValue::Black, so it has no effect.
         */
        inline void setMinimumAmbientLight (const Ogre::ColourValue &value) { mMinimumAmbientLight = value; }

        /// @see setMinimumAmbientLight
        inline const Ogre::ColourValue getMinimumAmbientLight () const { return mMinimumAmbientLight; }

        /** Ensure only one of caelum's light sources is active at a time (the brightest).
         *  This uses SkyLight::setForceDisable to disable low-intensity lightsources.
         *  Their contribution to ambient lighting is not affected.
         *  This implies a single shadow caster.
         *  This is disabled by default; and you can tweak light disabling by yourself.
         */
        inline void setEnsureSingleLightSource (bool value) { mEnsureSingleLightSource = value; }

        /// See setEnsureSingleLightSource
        inline bool getEnsureSingleLightSource () const { return mEnsureSingleLightSource; }

        /** Ensure only one of caelun's light sources casts shadows (the brightest).
         *  Disabled by default.
         */
        inline void setEnsureSingleShadowSource (bool value) { mEnsureSingleShadowSource = value; }

        /// See setEnsureSingleShadowSource
        inline bool getEnsureSingleShadowSource () const { return mEnsureSingleShadowSource; }

		/** Gets the fog colour for a certain daytime.
			@param time The current time.
			@param sunDir The sun direction.
			@return The fog colour.
		 */
		Ogre::ColourValue getFogColour (float time, const Ogre::Vector3 &sunDir);

		/** Gets the fog density for a certain daytime.
			@param time The current time.
			@param sunDir The sun direction.
			@return The fog density.
		 */
		float getFogDensity (float time, const Ogre::Vector3 &sunDir);

		/** Get the colour of the sun sphere.
         *  This colour is used to draw the sun sphere in the sky.
		 *  @return The colour of the sun.
		 */
		Ogre::ColourValue getSunSphereColour (float time, const Ogre::Vector3 &sunDir);

		/** Gets the colour of sun light.
         *  This color is used to illuminate the scene.
         *  @return The colour of the sun's light
		 */
		Ogre::ColourValue getSunLightColour (float time, const Ogre::Vector3 &sunDir);

		/// Gets the colour of moon's body.
		Ogre::ColourValue getMoonBodyColour (const Ogre::Vector3 &moonDir);

		/// Gets the colour of moon's light.
		Ogre::ColourValue getMoonLightColour (const Ogre::Vector3 &moonDir);

		/// Set the sun gradients image.
		void setSkyGradientsImage (const Ogre::String &filename = DEFAULT_SKY_GRADIENTS_IMAGE);

		/// Set the sun colours image.
		/// Sun colour is taken from this image.
		void setSunColoursImage (const Ogre::String &filename = DEFAULT_SUN_COLOURS_IMAGE);

		/** Get the sun's direction at a certain time.
         *  @param jday astronomical julian day.
         *  @see UniversalClock for julian day calculations.
		 */
		const Ogre::Vector3 getSunDirection (LongReal jday);

		/** Get the moon's direction at a certain time.
         *  @param jday astronomical julian day.
		 */
		const Ogre::Vector3 getMoonDirection (LongReal jday);

        /** Fake function to get the phase of the moon
         *  @param jday Julian day
         *  @return the phase of the moon; ranging from 0(full moon) to 2(new moon).
         *  The calculations performed by this function are completely fake.
         *  It's a triangle wave with a period of 28.5 days.
         */
		const Ogre::Real getMoonPhase (LongReal jday);

    private:
		/** Handle FrameListener::frameStarted to call updateSubcomponents every frame.
         *  If you don't register CaelumSystem as a an ogre frame listener you have to
         *  call updateSubcomponents yourself.
		 */
		virtual bool frameStarted (const Ogre::FrameEvent &e);

		/** Event trigger called just before rendering a viewport in a render target Caelum is attached to.
			Useful to make objects follow every camera that renders a viewport in a certain render target.
		 */
		virtual void preViewportUpdate (const Ogre::RenderTargetViewportEvent &e);

		virtual void viewportAdded (const Ogre::RenderTargetViewportEvent &e);
		virtual void viewportRemoved (const Ogre::RenderTargetViewportEvent &e);

        /** Free all subcomponents, but not CaelumSystem itself. Can be called multiple times.
         *  @param everything To destroy things that can't be rebuilt.
         */
        void destroySubcomponents (bool everything);
    };
} // namespace Caelum

#endif // CAELUM__CAELUM_SYSTEM_H
