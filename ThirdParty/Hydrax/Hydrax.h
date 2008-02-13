/*
--------------------------------------------------------------------------------
This source file is part of Hydrax.
Visit ---

Copyright (C) 2007 Xavier Verguín González <xavierverguin@hotmail.com>
                                           <xavyiy@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA  02111-1307, USA, or go to
http://www.gnu.org/copyleft/gpl.html.
--------------------------------------------------------------------------------
*/

#ifndef _Hydrax_H_
#define _Hydrax_H_

#include "Prerequisites.h"

#include "Enums.h"
#include "Structs.h"
#include "Help.h"
#include "Perlin.h"
#include "Mesh.h"

namespace Hydrax
{
    /** Hydrax is a realistic water rendering plugin for Ogre.
     */
    class DllExport Hydrax
    {
    public:
        /** Constructor
            @param sm Ogre SceneManager pointer
            @param c Ogre Camera pointer
         */
        Hydrax(Ogre::SceneManager *sm, Ogre::Camera *c);

        /** Destructor
         */
        ~Hydrax();

        /** Call when all params are set, creates grid water
            and perlin / normal map textures
            @remarks Call when all params are set
         */
        void create();

        /** Call every frame
            @todo Add listener interface
         */
        void update(Ogre::Real timeSinceLastFrame);

        /** Returns if the especified component is active
            @param Component Component that we want to check
         */
        bool isComponent(HydraxComponent Component);

        /** Set perlin options
            @param PerlinOptions Options
            @remarks It can be called after create(), Perlin and normal map textures will be updated
         */
        void setPerlinOptions(PerlinOptions PerlinOptions);

        /** Set mesh options
            @param MeshOptions Options
            @remarks It can be called after create(), Mesh will be updated
         */
        void setMeshOptions(MeshOptions MeshOptions);

        /** Set Rtt options
            @param RttOptions Options
            @remarks It can be called after create(), Rtt's will be updated
         */
        void setRttOptions(RttOptions RttOptions);

        /** Set Hydrax components
            @param Components Components
            @remarks It can be called after create(), Components will be updated
         */
        void setComponents(HydraxComponent Components);

        /** Set polygon mode: 1-Solid, 2-Wireframe, 3-Points
            @param Time polygon mode: 1-Solid, 2-Wireframe, 3-Points
         */
        void setPolygonMode(int Tipe);

        /** Set water position
            @param Position Water position
         */
        void setPosition(Ogre::Vector3 Position);

        /** Set clip planes error
            @param PlanesError Clip planes error
         */
        void setPlanesError(Ogre::Real PlanesError);

        /** Set water strength
            @param Strength Water strength
         */
        void setStrength(Ogre::Real Strength);

        /** Set water velocity
            @param Velocity Water velocity
         */
        void setVelocity(Ogre::Real Velocity);

        /** Set full reflection distance
            @param FullReflectionDistance Full reflection distance
         */
        void setFullReflectionDistance(Ogre::Real FullReflectionDistance);

        /** Set global transparency
            @param GlobalTransparency Global transparency distance
         */
        void setGlobalTransparency(Ogre::Real GlobalTransparency);

        /** Set normal distortion
            @param NormalDistortion Normal distortion
            @remarks Value will bi very short, like 0.025
         */
        void setNormalDistortion(Ogre::Real NormalDistortion);

        /** Set sun position
            @param SunPosition Sun position
         */
        void setSunPosition(Ogre::Vector3 SunPosition);

        /** Set sun strength
            @param SunStrength Sun strength
         */
        void setSunStrength(Ogre::Real SunStrength);

        /** Set sun area
            @param SunArea Sun area
         */
        void setSunArea(Ogre::Real SunArea);

        /** Set sun color
            @param SunColor Sun color
         */
        void setSunColor(Ogre::Vector3 SunColor);

        /** Set foam max distance
            @param FoamMaxDistance Foam max distance
         */
        void setFoamMaxDistance(Ogre::Real FoamMaxDistance);

        /** Set foam scale
            @param FoamScale Foam scale
         */
        void setFoamScale(Ogre::Real FoamScale);

        /** Set foam start
            @param FoamStart Foam start
         */
        void setFoamStart(Ogre::Real FoamStart);

        /** Set foam transparency
            @param FoamTransparency Foam transparency
         */
        void setFoamTransparency(Ogre::Real FoamTransparency);

        /** Set depth limit
            @param DepthLimit Depth limit
         */
        void setDepthLimit(Ogre::Real DepthLimit);

        /** Set depth color
            @param DepthColor Foam color
         */
        void setDepthColor(Ogre::Vector3 DepthColor);

        /** Set smooth power
            @param SmoothPower Smooth power
            @remarks Less values more transition distance, hight values short transition values, 1-50 range(aprox.)
         */
        void setSmoothPower(Ogre::Real SmoothPower);

        /** Set caustics scale
            @param CausticsScale Caustics scale
         */
        void setCausticsScale(Ogre::Real CausticsScale);

        /** Set caustics power
            @param CausticsPower Caustics power
         */
        void setCausticsPower(Ogre::Real CausticsPower);

        /** Set caustics end
            @param CausticsEnd Caustics end
         */
        void setCausticsEnd(Ogre::Real CausticsEnd);

        /** Has create() already called?
            @return Has create() already called?
         */
        inline bool isCreated()
        {
            return mCreated;
        }

        /** Get perlin options
            @return Hydrax perlin options
         */
        inline PerlinOptions getPerlinOptions()
        {
            return mPerlinOptions;
        }

        /** Get mesh options
            @return Hydrax mesh options
         */
        inline MeshOptions getMeshOptions()
        {
            return mMesh->getMeshOptions();
        }

        /** Get rtt options
            @return Hydrax rtt options
         */
        inline RttOptions getRttOptions()
        {
            return mRttOptions;
        }

        /** Get hydrax components selected
            @return Hydrax components
         */
        inline HydraxComponent getComponents()
        {
            return mComponents;
        }

        /** Get current polygon mode
            @return Current polygon mode: 1-Solid, 2-Wireframe, 3-Points
         */
        inline int getPolygonMode()
        {
            return mPolygonMode;
        }

        /** Get water position
            @return Water position
         */
        inline Ogre::Vector3 getPosition()
        {
            return mPosition;
        }

        /** Get water strength
            @return Hydrax water strength
         */
        inline Ogre::Real getStrength()
        {
            return mMesh->getStrength();
        }

        /** Get water transition velocity
            @return Water transition velocity
         */
        inline Ogre::Real getVelocity()
        {
            return mVelocity;
        }

        /** Get full reflection distance
            @return Hydrax water full reflection distance
         */
        inline Ogre::Real getFullReflectionDistance()
        {
            return mFullReflectionDistance;
        }

        /** Get global transparency
            @return Hydrax water global transparency
         */
        inline Ogre::Real getGlobalTransparency()
        {
            return mGlobalTransparency;
        }

        /** Get sun position
            @return Sun position
         */
        inline Ogre::Vector3 getSunPosition()
        {
            return mSunPosition;
        }

        /** Get normal distortion
            @return Hydrax normal distortion
         */
        inline Ogre::Real getNormalDistortion()
        {
            return mNormalDistortion;
        }

        /** Get water strength
            @return Hydrax water strength
         */
        inline Ogre::Real getSunStrength()
        {
            return mSunStrength;
        }

        /** Get sun area
            @return Sun area
         */
        inline Ogre::Real getSunArea()
        {
            return mSunArea;
        }

        /** Get sun color
            @return Sun color
         */
        inline Ogre::Vector3 getSunColor()
        {
            return mSunColor;
        }

        /** Get foam max distance
            @return Foam max distance
         */
        inline Ogre::Real getFoamMaxDistance()
        {
            return mFoamMaxDistance;
        }

        /** Get foam scale
            @return Foam scale
         */
        inline Ogre::Real getFoamScale()
        {
            return mFoamScale;
        }

        /** Get foam start
            @return Foam start
         */
        inline Ogre::Real getFoamStart()
        {
            return mFoamStart;
        }

        /** Get foam transparency
            @return Foam scale
         */
        inline Ogre::Real getFoamTransparency()
        {
            return mFoamTransparency;
        }

        /** Get depth limit
            @return Depth limit
         */
        inline Ogre::Real getDepthLimit()
        {
            return mDepthLimit;
        }

        /** Get depth color
            @return Depth color
         */
        inline Ogre::Vector3 getDepthColor()
        {
            return mDepthColor;
        }

        /** Get smooth power
            @return Smooth power
         */
        inline Ogre::Real getSmoothPower()
        {
            return mSmoothPower;
        }

        /** Get caustics power
            @return Caustics power
         */
        inline Ogre::Real getCausticsPower()
        {
            return mCausticsPower;
        }

        /** Get caustics end
            @return Caustics end
         */
        inline Ogre::Real getCausticsEnd()
        {
            return mCausticsEnd;
        }

    private:

    /** Hydrax::CRefractionListener class
     */
    class CRefractionListener : public Ogre::RenderTargetListener
        {
        public:
            /// Hydrax manager pointer
            Hydrax* mHydrax;

            /** Funtion that runs each frame
                @param evt Ogre RenderTargetEvent
                @remarks We've to override it
             */
            void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

            /** Funtion that runs each frame
                @param evt Ogre RenderTargetEvent
                @remarks We've to override it
             */
            void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
        };

    /** Hydrax::CReflectionListener class
     */
    class CReflectionListener : public Ogre::RenderTargetListener
        {
        public:
            /// Hydrax manager pointer
            Hydrax* mHydrax;

            /** Funtion that runs each frame
                @param evt Ogre RenderTargetEvent
                @remarks We've to override it
             */
            void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

            /** Funtion that runs each frame
                @param evt Ogre RenderTargetEvent
                @remarks We've to override it
             */
            void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
        };

    /** Hydrax::CDepthListener class
     */
    class CDepthListener : public Ogre::RenderTargetListener
        {
        public:
            /// Hydrax manager pointer
            Hydrax* mHydrax;

            /// std::string to store entity's original materials name
            std::queue<std::string> mMaterials;

            /** Funtion that runs each frame
                @param evt Ogre RenderTargetEvent
                @remarks We've to override it
             */
            void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

            /** Funtion that runs each frame
                @param evt Ogre RenderTargetEvent
                @remarks We've to override it
             */
            void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
        };

        /** Create all perlin and normal map textures
         */
        void _createTextures();

        /** Update perlin noise textures
            @param timeSinceLastFrame Ogre RenderTargetEvent.timeSinceLastFrame
         */
        void _updateTextures(Ogre::Real timeSinceLastFrame);

        /** Update normal map textures
         */
        void _updateNM();

        /** Create all rtt listeners
         */
        void _createRttListeners();

        /** Create/Delete depth rtt listener
            @param Create Create depth rtt listener?
            @param Delete Delete depth rtt listener?
            @remarks Don't call with (Delete = true) if before we haven't call it with (Create = true)
         */
        void _createDepthRttListener(bool Create = true, bool Delete = false);

        /** Set float param in water vertex program
            @param Param Parameter
            @param Value Float value
         */
        void _setWaterVertexProgramParameter(Ogre::String Param, float Value);

        /** Set float param in water fragment program
            @param Param Parameter
            @param Value Float value
         */
        void _setWaterFragmentProgramParameter(Ogre::String Param, float Value);

        /** Set Ogre::Vector3 param in water vertex program
            @param Param Parameter
            @param Value Ogre::Vector3 value
         */
        void _setWaterVertexProgramParameter(Ogre::String Param, Ogre::Vector3 Value);

        /** Set Ogre::Vector3 param in water fragment program
            @param Param Parameter
            @param Value Ogre::Vector3 value
         */
        void _setWaterFragmentProgramParameter(Ogre::String Param, Ogre::Vector3 Value);

        /** Set float param in depth vertex program
            @param Param Parameter
            @param Value Float value
         */
        void _setDepthVertexProgramParameter(Ogre::String Param, float Value);

        /** Set float param in depth fragment program
            @param Param Parameter
            @param Value Float value
         */
        void _setDepthFragmentProgramParameter(Ogre::String Param, float Value);

        /** Set Ogre::Vector3 param in depth vertex program
            @param Param Parameter
            @param Value Ogre::Vector3 value
         */
        void _setDepthVertexProgramParameter(Ogre::String Param, Ogre::Vector3 Value);

        /** Set Ogre::Vector3 param in depth fragment program
            @param Param Parameter
            @param Value Ogre::Vector3 value
         */
        void _setDepthFragmentProgramParameter(Ogre::String Param, Ogre::Vector3 Value);

        /// Has create() already called?
        bool mCreated;

        /// Perlin options
        PerlinOptions mPerlinOptions;
        /// Rtt options
        RttOptions mRttOptions;
        /// Hydrax components
        HydraxComponent mComponents;

        /// Polygon mode: 1-Solid, 2-Wireframe, 3-Points
        int mPolygonMode;
        /// Water position
        Ogre::Vector3 mPosition;
        /// Planes error, y axis clipplanes displacement
        Ogre::Real mPlanesError;
        /// Water(displacement) velocity
        Ogre::Real mVelocity;

        /// Full reflection distance param
        Ogre::Real mFullReflectionDistance;
        /// Global transparency param
        Ogre::Real mGlobalTransparency;
        /// Normal distortion param
        Ogre::Real mNormalDistortion;

        /// Sun position
        Ogre::Vector3 mSunPosition;
        /// Sun strength param
        Ogre::Real mSunStrength;
        /// Sun area
        Ogre::Real mSunArea;
        /// Sun color
        Ogre::Vector3 mSunColor;

        /// Foam max distance param
        Ogre::Real mFoamMaxDistance;
        /// Foam scale param
        Ogre::Real mFoamScale;
        /// Foam start param
        Ogre::Real mFoamStart;
        /// Foam transparency param
        Ogre::Real mFoamTransparency;

        /// Depth limit param
        Ogre::Real mDepthLimit;
        /// Depth color param
        Ogre::Vector3 mDepthColor;

        /// Smooth power param
        Ogre::Real mSmoothPower;

        /// Caustics scale param
        Ogre::Real mCausticsScale;
        /// Caustics power
        Ogre::Real mCausticsPower;
        /// Caustics end
        Ogre::Real mCausticsEnd;

        /// NoiseBuffer for store our 3 perlin textures: Source1, Source2, Final(Final is update every frame)
        std::vector<NoiseBuffer> mNoiseBuffer;
        /// Our perlin data generation object
        Perlin mPerlin;
        /// Current texture blending position
        float mActualTexScrollPosition;

        /// Our Hydrax::Mesh pointer
        Mesh *mMesh;

        /// Refraction Plane pointer
        Ogre::MovablePlane* mRefractionPlane;
        /// Reflection Plane pointer
        Ogre::MovablePlane* mReflectionPlane;
        /// Depth Plane pointer
        Ogre::MovablePlane* mDepthPlane;

        /// Refraction texture
        Ogre::TexturePtr mTextureRefraction;
        /// Reflection texture
        Ogre::TexturePtr mTextureReflection;
        /// Depth texture
        Ogre::TexturePtr mTextureDepth;

        /// Refraction listener
        CRefractionListener mRefractionListener;
        /// Reflection listener
        CReflectionListener mReflectionListener;
        /// Depth listener
        CDepthListener mDepthListener;

        /// SceneNode to attach our Hydrax::Mesh entity
        Ogre::SceneNode *mSceneNode;
        /// SceneNode to attach our planes
        Ogre::SceneNode *mPlanesSceneNode;

        /// Pointer to Ogre::SceneManager
        Ogre::SceneManager *mSceneManager;
        /// Pointer to Ogre::Camera
        Ogre::Camera *mCamera;
    };
}

#endif
