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

#ifndef CAELUM__PRECIPITATION_CONTROLLER_H
#define CAELUM__PRECIPITATION_CONTROLLER_H

#include "CaelumPrerequisites.h"

namespace Caelum
{
	enum PrecipitationType
	{
		PRECIPITATION_DRIZZLE		= 0,
		PRECIPITATION_RAIN			= 1,
		PRECIPITATION_SNOW			= 2,
		PRECIPITATION_SNOWGRAINS	= 3,
		PRECIPITATION_ICECRYSTALS	= 4,
		PRECIPITATION_ICEPELLETS	= 5,
		PRECIPITATION_HAIL			= 6,
		PRECIPITATION_SMALLHAIL		= 7,
	};

    class PrecipitationInstance;

    /** Compositor-based precipitation controller.
     *  This class will add and control precipitation controllers to viewports.
     *
     *  Compositors clone the composing materials. This controller will
     *  register itself as a compositor listener and change the material in notifyMaterialSetup.
     */
	class CAELUM_EXPORT PrecipitationController
	{
	private:
		Ogre::SceneManager *mSceneMgr;
		Ogre::Vector3 mWindSpeed;
		Real mCoverage;
		Real mSpeed;
		Real mTime;
		PrecipitationType mType;

	public:
        /// Called to enforce parameters on a composing material
    	void _updateMaterialParams(
                const Ogre::MaterialPtr& mat,
                const Ogre::Camera* cam,
                const Ogre::Vector3& camSpeed);

	public:
        /// Name of the compositor resource.
        static const String COMPOSITOR_NAME;

        /// Name of the compositor material.
        static const String MATERIAL_NAME;

		// Wind speed and direction
		void setWindSpeed(const Ogre::Vector3 &value);
		const Ogre::Vector3 getWindSpeed();

		// Precipitation coverage
		void setCoverage(Real value);
		Real getCoverage();

		 // Precipitation type
		void setType(PrecipitationType value);
		PrecipitationType getType();

		// Precipitation speed (affects direction)
		void setSpeed(Real value);
		Real getSpeed();

		/// Set manual camera speed for all viewports.
		void setManualCameraSpeed(const Ogre::Vector3 &value);

        /// Set auto camera speed everywhere.
		void setAutoCameraSpeed();
		
        /** Update the the precipitation controller.
         *  @param secondsSinceLastFrame Number of secods since the last frame.
         */
		void update(Real secondsSinceLastFrame);

		PrecipitationController(
				Ogre::SceneManager *sceneMgr);
		~PrecipitationController();

    public:
        typedef std::map<Ogre::Viewport*, PrecipitationInstance*> ViewportInstanceMap;
        ViewportInstanceMap mViewportInstanceMap;

    public:
        /// Add precipitation to a certain viewport.
        PrecipitationInstance* createViewportInstance(Ogre::Viewport* viewport);

        /// Remove precipitation from a certain viewport.
        void destroyViewportInstance(Ogre::Viewport* viewport);

        /// Get per-viewport instance, or null if not created yet.
        PrecipitationInstance* getViewportInstance(Ogre::Viewport* viewport);

        /// Remove from all attached viewports; clean up.
        void destroyAllViewportInstances();
	};

    /** Per-viewport instance of precipitation.
     *  This will create and control an ogre::CompositorInstance.
     */
    class PrecipitationInstance: private Ogre::CompositorInstance::Listener
    {
    private:
        PrecipitationController* mParent;
        Ogre::Viewport* mViewport;
        Ogre::CompositorInstance* mCompInst;
        Ogre::Camera* mLastCamera;
        Ogre::Vector3 mLastCameraPosition;
        Ogre::Vector3 mCameraSpeed;
        bool mAutoCameraSpeed;

        virtual void notifyMaterialSetup(uint pass_id, Ogre::MaterialPtr &mat);
        virtual void notifyMaterialRender(uint pass_id, Ogre::MaterialPtr &mat);

    public:
        inline Ogre::Viewport* getViewport() const { return mViewport; }
        inline PrecipitationController* getParent() const { return mParent; }
        inline Ogre::CompositorInstance* getCompositorInstance() const { return mCompInst; }

        /// Check if camera speed is automatically calculated (default true).
        bool getAutoCameraSpeed();

        /// Set camera speed to automatic calculation.
        void setAutoCameraSpeed();

        /// Set manual camera speed; disables automatic calculation.
        void setManualCameraSpeed(const Ogre::Vector3& value);

        /// Get current camera speed.
        const Ogre::Vector3 getCameraSpeed();

        PrecipitationInstance(PrecipitationController* parent, Ogre::Viewport* view);
        virtual ~PrecipitationInstance();
    };
}

#endif //CAELUM__PRECIPITATION_CONTROLLER_H
