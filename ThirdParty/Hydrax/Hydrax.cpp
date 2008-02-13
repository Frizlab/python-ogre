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

#include "Hydrax.h"

namespace Hydrax
{

    Hydrax::Hydrax(Ogre::SceneManager *sm, Ogre::Camera *c)
            : mSceneManager(sm)
            , mCamera(c)
            , mCreated(false)
            , mPolygonMode(0)
            , mPosition(Ogre::Vector3(0,0,0))
            , mPlanesError(0)
            , mVelocity(9.75)
            , mFullReflectionDistance(90000000)
            , mGlobalTransparency(0.05)
            , mNormalDistortion(0.09)
            , mSunPosition(Ogre::Vector3(5000,3000,1))
            , mSunStrength(1.75)
            , mSunArea(150)
            , mSunColor(Ogre::Vector3(1,0.75,0.25))
            , mFoamMaxDistance(75000000)
            , mFoamScale(50)
            , mFoamStart(0.5)
            , mFoamTransparency(1.275)
            , mDepthLimit(0)
            , mDepthColor(Ogre::Vector3(0,0.1,0.172))
            , mSmoothPower(30)
            , mCausticsScale(20)
            , mCausticsPower(15)
            , mCausticsEnd(0.55)
            , mActualTexScrollPosition(0)
            , mMesh(new Mesh(sm))
            , mSceneNode(0)
            , mRefractionPlane(0)
            , mReflectionPlane(0)
            , mDepthPlane(0)
            , mTextureRefraction(0)
            , mTextureReflection(0)
            , mTextureDepth(0)
            , mPlanesSceneNode(0)
            , mComponents(HYDRAX_COMPONENTS_NONE)
    {
        LOG("[Hydrax] Hydrax object created.");
    }

    Hydrax::~Hydrax()
    {
        if (mSceneNode)
        {
            mSceneNode->detachAllObjects();
            mSceneNode->getParentSceneNode()->removeAndDestroyChild(mSceneNode->getName());

            Ogre::TextureManager::getSingleton().remove("HydraxHeight");
            Ogre::TextureManager::getSingleton().remove("HydraxNormalMap");

            delete mMesh;
        }

        if (mPlanesSceneNode)
        {
            mPlanesSceneNode->detachAllObjects();
            mPlanesSceneNode->getParentSceneNode()->removeAndDestroyChild(mPlanesSceneNode->getName());

            Ogre::RenderTarget* mRT_TextureRefraction = mTextureRefraction->getBuffer()->getRenderTarget();
            mRT_TextureRefraction->removeAllListeners();
            mRT_TextureRefraction->removeAllViewports();

            Ogre::RenderTarget* mRT_TextureReflection = mTextureReflection->getBuffer()->getRenderTarget();
            mRT_TextureReflection->removeAllListeners();
            mRT_TextureReflection->removeAllViewports();

            if (isComponent(HYDRAX_COMPONENT_DEPTH))
            {
                Ogre::RenderTarget* mRT_TextureDepth = mTextureDepth->getBuffer()->getRenderTarget();
                mRT_TextureDepth->removeAllListeners();
                mRT_TextureDepth->removeAllViewports();

                Ogre::TextureManager::getSingleton().remove("Depth");

                Ogre::MeshManager::getSingleton().remove("DepthClipPlane");
            }

            Ogre::TextureManager::getSingleton().remove("Reflection");
            Ogre::TextureManager::getSingleton().remove("Refraction");

            Ogre::MeshManager::getSingleton().remove("RefractionClipPlane");
            Ogre::MeshManager::getSingleton().remove("ReflectionClipPlane");
        }

        mNoiseBuffer[2].Set(mPerlin.getFreeData(), mPerlin.mSize);
        mNoiseBuffer.clear();

        LOG("[Hydrax] Hydrax object removed.");
    }

    void Hydrax::create()
    {
        if (isCreated())
        {
            LOG("[Hydrax] Hydrax alredy created, skipping...");

            return;
        }

        LOG("[Hydrax] Creating perlin noise textures and normal maps.");
        _createTextures();
        _updateTextures(0);
        LOG("[Hydrax] Perlin noise textures and normal maps created.");

        LOG("[Hydrax] Creating water mesh.");
        mSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
        mMesh->create(mSceneNode);
        mMesh->update(mNoiseBuffer[2]);
        LOG("[Hydrax] Water mesh created");

        LOG("[Hydrax] Creating RTListeners.");
        _createRttListeners();
        LOG("[Hydrax] RTListeners created");

        mCreated = true;
    }

    void Hydrax::_createTextures()
    {
        mPerlin.setSize(mPerlinOptions.TexQuality, mPerlinOptions.TexQuality);

        mNoiseBuffer.reserve(3);
        mNoiseBuffer.resize(3);

        mPerlin.mPerlinModule.SetFrequency(mPerlinOptions.Frecuency);
        mPerlin.mPerlinModule.SetPersistence(mPerlinOptions.Persistence);
        mPerlin.mPerlinModule.SetOctaveCount(mPerlinOptions.Octave);
        mPerlin.mPerlinModule.SetLacunarity(mPerlinOptions.Lacunarity);

        mPerlin.createPerlinTexture(Ogre::String("HydraxHeight"));
        mPerlin.createNMTexture(Ogre::String("HydraxNormalMap"));

        mPerlin.mPerlinModule.SetSeed(0);
        mNoiseBuffer[0].Set(mPerlin.getData(true), mPerlin.mSize, mPerlin.getNormalMap(mPerlinOptions.NMHeight, true));
        mPerlin.mPerlinModule.SetSeed(1);
        mNoiseBuffer[1].Set(mPerlin.getData(true), mPerlin.mSize, mPerlin.getNormalMap(mPerlinOptions.NMHeight, true));
        mNoiseBuffer[2].Set(mPerlin.getFreeData(), mPerlin.mSize);
        mNoiseBuffer[2].mNormalMap.SetSize(mPerlin.mSize.Width, mPerlin.mSize.Height);
    }

    void Hydrax::_updateTextures(Ogre::Real timeSinceLastFrame)
    {
        int x, y, newY1, newY1a, newY2, newY2a;
        float a, b, c, d, diff, diffb;

        float** OriginalA = mNoiseBuffer[0].mData;
        float** OriginalB = mNoiseBuffer[1].mData;

        diff  = (mActualTexScrollPosition - static_cast<int>(mActualTexScrollPosition))/2; // Divide by 2 to avoid it in: mNoiseBuffer[2].mData[x][y] = (c+d)/2;
        diffb = 0.5 - diff;

        for (y = 0; y < mNoiseBuffer[0].mSize.Height; y ++)
        {
            newY1 = y + static_cast<int>(mActualTexScrollPosition);
            if (newY1>mNoiseBuffer[0].mSize.Height-1)
            {
                newY1-=mNoiseBuffer[0].mSize.Height;
            }

            newY1a = (y+1) + static_cast<int>(mActualTexScrollPosition);
            if (newY1a>mNoiseBuffer[0].mSize.Height-1)
            {
                newY1a-=mNoiseBuffer[0].mSize.Height;
            }

            newY2 = y - static_cast<int>(mActualTexScrollPosition);
            if (newY2<0)
            {
                newY2+=mNoiseBuffer[0].mSize.Height-1;
            }

            newY2a = (y+1) - static_cast<int>(mActualTexScrollPosition);
            if (newY2a<0)
            {
                newY2a+=mNoiseBuffer[0].mSize.Height-1;
            }
            if (newY2a>mNoiseBuffer[0].mSize.Height-1)
            {
                newY2a-=mNoiseBuffer[0].mSize.Height;
            }

            for (x = 0; x < mNoiseBuffer[0].mSize.Width; x ++)
            {
                a = OriginalA[newY1][x];
                b = OriginalA[newY1a][x];

                c=a*diffb+ b*diff;

                a = OriginalB[newY2][x];
                b = OriginalB[newY2a][x];

                d=a*diff + b*diffb;

                mNoiseBuffer[2].mData[y][x] = (c+d);
            }
        }

        mActualTexScrollPosition += mVelocity*timeSinceLastFrame;

        if (mActualTexScrollPosition>mNoiseBuffer[0].mSize.Height)
        {
            mActualTexScrollPosition = 0;
        }

        mPerlin.updatePerlinTexture(mNoiseBuffer[2].mData);
        _updateNM();
    }

    void Hydrax::_updateNM()
    {
        int x, y, newY1, newY1a, newY2, newY2a;
        float diff, diffb;

        diff  = (mActualTexScrollPosition - static_cast<int>(mActualTexScrollPosition))/2; // Divide by 2 to avoid it in the bucle
        diffb = 0.5 - diff;

        for (y = 0; y < mNoiseBuffer[0].mSize.Height; y ++)
        {
            newY1 = y + static_cast<int>(mActualTexScrollPosition);
            if (newY1>mNoiseBuffer[0].mSize.Height-1)
            {
                newY1-=mNoiseBuffer[0].mSize.Height;
            }

            newY1a = y+1 + static_cast<int>(mActualTexScrollPosition);
            if (newY1a>mNoiseBuffer[0].mSize.Height-1)
            {
                newY1a-=mNoiseBuffer[0].mSize.Height;
            }

            newY2 = y - static_cast<int>(mActualTexScrollPosition);
            if (newY2<0)
            {
                newY2+=mNoiseBuffer[0].mSize.Height-1;
            }

            newY2a = (y+1) - static_cast<int>(mActualTexScrollPosition);
            if (newY2a<0)
            {
                newY2a+=mNoiseBuffer[0].mSize.Height-1;
            }
            if (newY2a>mNoiseBuffer[0].mSize.Height-1)
            {
                newY2a-=mNoiseBuffer[0].mSize.Height;
            }

            for (x = 0; x < mNoiseBuffer[0].mSize.Width; x ++)
            {
                noise::utils::Color c =
                    noise::utils::Color(
                        // Red
                        (( mNoiseBuffer[1].mNormalMap.GetValue(newY1,x).red   *diffb + mNoiseBuffer[1].mNormalMap.GetValue(newY1a,x).red   *diff )
                         +(mNoiseBuffer[0].mNormalMap.GetValue(newY2,x).red   *diff  + mNoiseBuffer[0].mNormalMap.GetValue(newY2a,x).red   *diffb)),
                        // Green
                        (( mNoiseBuffer[1].mNormalMap.GetValue(newY1,x).green *diffb + mNoiseBuffer[1].mNormalMap.GetValue(newY1a,x).green *diff )
                         +(mNoiseBuffer[0].mNormalMap.GetValue(newY2,x).green *diff  + mNoiseBuffer[0].mNormalMap.GetValue(newY2a,x).green *diffb)),
                        // Blue
                        (( mNoiseBuffer[1].mNormalMap.GetValue(newY1,x).blue  *diffb + mNoiseBuffer[1].mNormalMap.GetValue(newY1a,x).blue  *diff )
                         +(mNoiseBuffer[0].mNormalMap.GetValue(newY2,x).blue  *diff  + mNoiseBuffer[0].mNormalMap.GetValue(newY2a,x).blue  *diffb)),
                        // Alpha
                        255);

                mNoiseBuffer[2].mNormalMap.SetValue(y,x,c);
            }
        }

        mPerlin.updateNMTexture(mNoiseBuffer[2].mNormalMap);
    }

    void Hydrax::_createRttListeners()
    {
        if (!isCreated())
        {
            mRefractionPlane = new Ogre::MovablePlane("RefractionPlane");
            mReflectionPlane = new Ogre::MovablePlane("ReflectionPlane");

            mRefractionPlane->d = 0;
            mReflectionPlane->d = 0;

            mRefractionPlane->normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
            mReflectionPlane->normal = Ogre::Vector3::UNIT_Y;

            Ogre::MeshManager::getSingleton().createPlane(
                "RefractionClipPlane",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                *mRefractionPlane,
                mMesh->getSize().Width,mMesh->getSize().Height,
                10,10,true,1,5,5,Ogre::Vector3::UNIT_Z);

            Ogre::MeshManager::getSingleton().createPlane(
                "ReflectionClipPlane",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                *mReflectionPlane,
                mMesh->getSize().Width,mMesh->getSize().Height,
                10,10,true,1,5,5,Ogre::Vector3::UNIT_Z);

            mRefractionPlane->setCastShadows(false);
            mReflectionPlane->setCastShadows(false);

            mPlanesSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
            mPlanesSceneNode->attachObject(mRefractionPlane);
            mPlanesSceneNode->attachObject(mReflectionPlane);

            mReflectionListener.mHydrax = this;
            mRefractionListener.mHydrax = this;
        }

        Ogre::TextureManager::getSingleton().remove("Reflection");
        Ogre::TextureManager::getSingleton().remove("Refraction");

        mTextureRefraction = Ogre::TextureManager::getSingleton().createManual(
                                 "Refraction",
                                 Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                 Ogre::TEX_TYPE_2D,
                                 mRttOptions.RefractionQuality, mRttOptions.RefractionQuality, 0,
                                 Ogre::PF_R8G8B8,
                                 Ogre::TU_RENDERTARGET);

        Ogre::RenderTarget* mRT_TextureRefraction = mTextureRefraction->getBuffer()->getRenderTarget();
        {
            Ogre::Viewport *v = mRT_TextureRefraction->addViewport(mCamera);
            v->setClearEveryFrame(true);
            v->setBackgroundColour(Ogre::ColourValue::White);
            v->setOverlaysEnabled(false);

            mRT_TextureRefraction->addListener(&mRefractionListener);

            Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(mMesh->getMaterialName());
            Ogre::TextureUnitState* t = mat->getTechnique(0)->getPass(0)->getTextureUnitState(2);
            t->setTextureName("Refraction");
        }

        mTextureReflection = Ogre::TextureManager::getSingleton().createManual(
                                 "Reflection",
                                 Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                 Ogre::TEX_TYPE_2D,
                                 mRttOptions.ReflectionQuality, mRttOptions.ReflectionQuality, 0,
                                 Ogre::PF_R8G8B8,
                                 Ogre::TU_RENDERTARGET);

        Ogre::RenderTarget* mRT_TextureReflection = mTextureReflection->getBuffer()->getRenderTarget();
        {
            Ogre::Viewport *v = mRT_TextureReflection->addViewport(mCamera);
            v->setClearEveryFrame(true);
            v->setBackgroundColour(Ogre::ColourValue::White);
            v->setOverlaysEnabled(false);

            mRT_TextureReflection->addListener(&mReflectionListener);

            Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(mMesh->getMaterialName());
            Ogre::TextureUnitState* t = mat->getTechnique(0)->getPass(0)->getTextureUnitState(1);
            t->setTextureName("Reflection");
        }

        if (isComponent(HYDRAX_COMPONENT_DEPTH))
        {
            _createDepthRttListener();
        }
    }

    void Hydrax::_createDepthRttListener(bool Create, bool Delete)
    {
        if (Create)
        {
            Ogre::TextureManager::getSingleton().remove("Depth");

            if (!isCreated())
            {
                mDepthPlane = new Ogre::MovablePlane("DepthPlane");

                mDepthPlane->d = 0;
                mDepthPlane->normal = Ogre::Vector3::NEGATIVE_UNIT_Y;

                Ogre::MeshManager::getSingleton().createPlane(
                    "DepthClipPlane",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                    *mDepthPlane,
                    mMesh->getSize().Width,mMesh->getSize().Height,
                    10,10,true,1,5,5,Ogre::Vector3::UNIT_Z);

                mDepthPlane->setCastShadows(false);

                mPlanesSceneNode->attachObject(mDepthPlane);

                mDepthListener.mHydrax = this;
            }

            mTextureDepth = Ogre::TextureManager::getSingleton().createManual(
                                "Depth",
                                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                Ogre::TEX_TYPE_2D,
                                mRttOptions.DepthQuality, mRttOptions.DepthQuality, 0,
                                Ogre::PF_R8G8B8,
                                Ogre::TU_RENDERTARGET);

            Ogre::RenderTarget* mRT_TextureDepth = mTextureDepth->getBuffer()->getRenderTarget();
            {
                Ogre::Viewport *v = mRT_TextureDepth->addViewport(mCamera);
                v->setClearEveryFrame(true);
                v->setBackgroundColour(Ogre::ColourValue::White);
                v->setOverlaysEnabled(false);
                v->setMaterialScheme("Depth");

                mRT_TextureDepth->addListener(&mDepthListener);

                Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(mMesh->getMaterialName());
                Ogre::TextureUnitState* t = mat->getTechnique(0)->getPass(0)->getTextureUnitState(3);
                t->setTextureName("Depth");
            }

        }
        if (Delete)
        {
            Ogre::RenderTarget* mRT_TextureDepth = mTextureDepth->getBuffer()->getRenderTarget();
            mRT_TextureDepth->removeAllListeners();
            mRT_TextureDepth->removeAllViewports();

            Ogre::TextureManager::getSingleton().remove("Depth");

            Ogre::MeshManager::getSingleton().remove("DepthClipPlane");
        }
    }

    void Hydrax::CReflectionListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        mHydrax->mMesh->getEntity()->setVisible(false);
        mHydrax->mReflectionPlane->getParentNode()->translate(0,mHydrax->mPlanesError,0);

        mHydrax->mCamera->enableReflection(mHydrax->mReflectionPlane);
        mHydrax->mCamera->enableCustomNearClipPlane(mHydrax->mReflectionPlane);
    }

    void Hydrax::CReflectionListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        mHydrax->mMesh->getEntity()->setVisible(true);
        mHydrax->mReflectionPlane->getParentNode()->translate(0,-mHydrax->mPlanesError,0);

        mHydrax->mCamera->disableReflection();
        mHydrax->mCamera->disableCustomNearClipPlane();
    }

    void Hydrax::CRefractionListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        mHydrax->mMesh->getEntity()->setVisible(false);
        mHydrax->mRefractionPlane->getParentNode()->translate(0,mHydrax->mPlanesError,0);

        mHydrax->mCamera->enableCustomNearClipPlane(mHydrax->mRefractionPlane);
    }

    void Hydrax::CRefractionListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        mHydrax->mMesh->getEntity()->setVisible(true);
        mHydrax->mRefractionPlane->getParentNode()->translate(0,-mHydrax->mPlanesError,0);

        mHydrax->mCamera->disableCustomNearClipPlane();
    }

    void Hydrax::CDepthListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        mHydrax->mMesh->getEntity()->setVisible(false);

        Ogre::SceneManager::MovableObjectIterator it = mHydrax->mSceneManager->getMovableObjectIterator("Entity");

        Ogre::Entity* cur;
        mMaterials.empty();

        while (it.hasMoreElements())
        {
            cur = dynamic_cast<Ogre::Entity*>(it.peekNextValue());
            mMaterials.push(cur->getSubEntity(0)->getMaterialName());

            if (mHydrax->isComponent(HYDRAX_COMPONENT_CAUSTICS))
            {
                cur->getSubEntity(0)->setMaterialName("HydraxShaderDepthCaustics");
            }
            else
            {
                cur->getSubEntity(0)->setMaterialName("HydraxShaderDepth");
            }

            it.moveNext();
        }

        mHydrax->mDepthPlane->getParentNode()->translate(0,mHydrax->mPlanesError,0);
        mHydrax->mCamera->enableCustomNearClipPlane(mHydrax->mDepthPlane);
    }

    void Hydrax::CDepthListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        Ogre::SceneManager::MovableObjectIterator it = mHydrax->mSceneManager->getMovableObjectIterator("Entity");

        Ogre::Entity* cur;

        while (it.hasMoreElements())
        {
            std::string name = mMaterials.front();

            cur = dynamic_cast<Ogre::Entity*>(it.peekNextValue());
            if (Ogre::MaterialManager::getSingleton().resourceExists(name))
                cur->setMaterialName(name);

            mMaterials.pop();
            it.moveNext();
        }

        mHydrax->mMesh->getEntity()->setVisible(true);

        mHydrax->mCamera->disableCustomNearClipPlane();
        mHydrax->mDepthPlane->getParentNode()->translate(0,-mHydrax->mPlanesError,0);
    }

    void Hydrax::setPerlinOptions(PerlinOptions PerlinOptions)
    {
        mPerlinOptions = PerlinOptions;

        if (isCreated())
        {
            LOG("[Hydrax] Updating perlin options.");

            Ogre::TextureManager::getSingleton().remove("HydraxHeight");
            Ogre::TextureManager::getSingleton().remove("HydraxNormalMap");

            mNoiseBuffer.clear();

            _createTextures();
            _updateTextures(0);

            LOG("[Hydrax] Perlin options updated.");
        }
    }

    void Hydrax::setMeshOptions(MeshOptions MeshOptions)
    {
        if (isCreated())
        {
            Ogre::String MaterialNameTmp = mMesh->getMaterialName();

            LOG("[Hydrax] Updating water mesh.");
            LOG("[Hydrax] Deleting water mesh.");

            delete mMesh;

            LOG("[Hydrax] Water mesh deleted.");

            LOG("[Hydrax] Creating water mesh.");
            mMesh = new Mesh(mSceneManager);
            mMesh->setMeshOptions(MeshOptions);
            mMesh->setMaterialName(MaterialNameTmp);
            mMesh->create(mSceneNode);
            setPosition(mPosition);
            mMesh->update(mNoiseBuffer[2]);
            LOG("[Hydrax] Water mesh created");

            return;
        }

        mMesh->setMeshOptions(MeshOptions);
    }

    void Hydrax::setRttOptions(RttOptions RttOptions)
    {
        mRttOptions = RttOptions;

        if (isCreated())
        {
            LOG("[Hydrax] Updating Rtt options.");

            Ogre::RenderTarget* mRT_TextureRefraction = mTextureRefraction->getBuffer()->getRenderTarget();
            mRT_TextureRefraction->removeAllListeners();
            mRT_TextureRefraction->removeAllViewports();

            Ogre::RenderTarget* mRT_TextureReflection = mTextureReflection->getBuffer()->getRenderTarget();
            mRT_TextureReflection->removeAllListeners();
            mRT_TextureReflection->removeAllViewports();

            if (isComponent(HYDRAX_COMPONENT_DEPTH))
            {
                Ogre::RenderTarget* mRT_TextureDepth = mTextureDepth->getBuffer()->getRenderTarget();
                mRT_TextureDepth->removeAllListeners();
                mRT_TextureDepth->removeAllViewports();

                Ogre::TextureManager::getSingleton().remove("Depth");
            }

            Ogre::TextureManager::getSingleton().remove("Reflection");
            Ogre::TextureManager::getSingleton().remove("Refraction");

            _createRttListeners();

            LOG("[Hydrax] Rtt options updated.");
        }
    }

    void Hydrax::setPolygonMode(int Tipe)
    {
        Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(mMesh->getMaterialName());

        if (Tipe == 0)
        {
            mat->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_SOLID);
        }
        else if (Tipe == 1)
        {
            mat->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_WIREFRAME);
        }
        else if (Tipe == 2)
        {
            mat->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_POINTS);
        }
    }

    void Hydrax::update(Ogre::Real timeSinceLastFrame)
    {
        if (mVelocity == 0)
        {
            return;
        }

        _updateTextures(timeSinceLastFrame);
        mMesh->update(mNoiseBuffer[2]);
    }

    void Hydrax::setComponents(HydraxComponent Components)
    {
        // Create/Delete depth rtt listeners if it's necesary
        if (isCreated())
        {
            if (isComponent(HYDRAX_COMPONENT_DEPTH))
            {
                if (!(Components & HYDRAX_COMPONENT_DEPTH))
                {
                    _createDepthRttListener(false, true);
                }
            }
            else
            {
                if (Components & HYDRAX_COMPONENT_DEPTH)
                {
                    _createDepthRttListener();
                }
            }
        }

        mComponents = Components;

        /* Shaders:

             1 - Standard
             2 - Standard, Sun
             3 - Standard, Foam
             4 - Standard, Sun, Foam

             5 - Depth
             6 - Depth, Sun
             7 - Depth, Foam
             8 - Depth, Sun, Foam

             9  - Depth, Smooth
             10 - Depth, Smooth, Sun
             11 - Depth, Smooth, Foam
             12 - Depth, Smooth, Sun, Foam

             13 - Depth, Caustics
             14 - Depth, Sun, Caustics
             15 - Depth, Foam, Caustics
             16 - Depth, Sun, Foam, Caustics

             17 - Depth, Smooth, Caustics
             18 - Depth, Smooth, Sun, Caustics
             19 - Depth, Smooth, Foam, Caustics
             20 - Depth, Smooth, Sun, Foam, Caustics
         */

        int Index = 1;

        if (isComponent(HYDRAX_COMPONENT_SUN))
        {
            Index ++;
        }

        if (isComponent(HYDRAX_COMPONENT_FOAM))
        {
            Index += 2;
        }

        if (isComponent(HYDRAX_COMPONENT_DEPTH))
        {
            Index += 4;
        }

        if (isComponent(HYDRAX_COMPONENT_SMOOTH) || isComponent(HYDRAX_COMPONENT_CAUSTICS))
        {
            if (isComponent(HYDRAX_COMPONENT_DEPTH))
            {
                Index += 4;

                if (isComponent(HYDRAX_COMPONENT_CAUSTICS))
                {
                    Index += 8;
                }
            }
            else
            {
                // Delete smooth component: Mask flags can't be removed
                // so we have to evaluate all posibilities
                // and reasign components...

                if (isComponent(HYDRAX_COMPONENT_SUN)  &&
                        isComponent(HYDRAX_COMPONENT_FOAM) &&
                        isComponent(HYDRAX_COMPONENT_PLSM2))
                {
                    mComponents = static_cast<HydraxComponent>
                                  (HYDRAX_COMPONENT_SUN  |
                                   HYDRAX_COMPONENT_FOAM |
                                   HYDRAX_COMPONENT_PLSM2);
                }
                else
                    if (isComponent(HYDRAX_COMPONENT_SUN)  &&
                            isComponent(HYDRAX_COMPONENT_FOAM))
                    {
                        mComponents = static_cast<HydraxComponent>
                                      (HYDRAX_COMPONENT_SUN  |
                                       HYDRAX_COMPONENT_FOAM);
                    }
                    else
                        if (isComponent(HYDRAX_COMPONENT_SUN)  &&
                                isComponent(HYDRAX_COMPONENT_PLSM2))
                        {
                            mComponents = static_cast<HydraxComponent>
                                          (HYDRAX_COMPONENT_SUN  |
                                           HYDRAX_COMPONENT_PLSM2);
                        }
                        else
                            if (isComponent(HYDRAX_COMPONENT_FOAM)  &&
                                    isComponent(HYDRAX_COMPONENT_PLSM2))
                            {
                                mComponents = static_cast<HydraxComponent>
                                              (HYDRAX_COMPONENT_FOAM |
                                               HYDRAX_COMPONENT_PLSM2);
                            }
                            else
                                if (isComponent(HYDRAX_COMPONENT_SUN))
                                {
                                    mComponents = static_cast<HydraxComponent>
                                                  (HYDRAX_COMPONENT_SUN);
                                }
                                else
                                    if (isComponent(HYDRAX_COMPONENT_FOAM))
                                    {
                                        mComponents = static_cast<HydraxComponent>
                                                      (HYDRAX_COMPONENT_FOAM);
                                    }
                                    else
                                        if (isComponent(HYDRAX_COMPONENT_PLSM2))
                                        {
                                            mComponents = static_cast<HydraxComponent>
                                                          (HYDRAX_COMPONENT_PLSM2);
                                        }

                LOG("[Hydrax] Requested component needs depth component... requested component desactivated.");
            }
        }

        mMesh->setMaterialName(Ogre::String("HydraxShader"+Ogre::StringConverter::toString(Index)));
    }

    bool Hydrax::isComponent(HydraxComponent Component)
    {
        if (mComponents & Component)
        {
            return true;
        }

        if (Component == HYDRAX_COMPONENTS_NONE && mComponents == HYDRAX_COMPONENTS_NONE)
        {
            return true;
        }

        if (Component == HYDRAX_COMPONENTS_ALL && mComponents == HYDRAX_COMPONENTS_ALL)
        {
            return true;
        }

        return false;
    }

    void Hydrax::_setWaterVertexProgramParameter(Ogre::String Param, float Value)
    {
        Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(mMesh->getMaterialName());
        mat->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant(Param, Value);
    }

    void Hydrax::_setWaterFragmentProgramParameter(Ogre::String Param, float Value)
    {
        Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(mMesh->getMaterialName());
        mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant(Param, Value);
    }

    void Hydrax::_setWaterVertexProgramParameter(Ogre::String Param, Ogre::Vector3 Value)
    {
        Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(mMesh->getMaterialName());
        mat->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant(Param, Value);
    }

    void Hydrax::_setWaterFragmentProgramParameter(Ogre::String Param, Ogre::Vector3 Value)
    {
        Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(mMesh->getMaterialName());
        mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant(Param, Value);
    }

    void Hydrax::_setDepthVertexProgramParameter(Ogre::String Param, float Value)
    {
        if (isComponent(HYDRAX_COMPONENT_DEPTH))
        {
            // Entity's depth shader
            Ogre::MaterialPtr mat;

            if (isComponent(HYDRAX_COMPONENT_CAUSTICS))
            {
                mat = Ogre::MaterialManager::getSingleton().getByName("HydraxShaderDepthCaustics");
            }
            else
            {
                mat = Ogre::MaterialManager::getSingleton().getByName("HydraxShaderDepth");
            }

            try
            {
                if (!mat.isNull())
                {
                    mat->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant(Param, Value);
                }
            }
            catch (...)
            {
            }

            // Terrain depth shader
            if (isComponent(HYDRAX_COMPONENT_PLSM2))
            {
                int w, h;

                mSceneManager->getOption("Width",  &w);
                mSceneManager->getOption("Height", &h);

                Ogre::String Mapa;
                Ogre::String Formato;

                mSceneManager->getOption("CurrentMap",           &Mapa);
                mSceneManager->getOption("CurrentTextureFormat", &Formato);

                for (int i = 0; i < w; i++)
                {
                    for (int j = 0; j < h; j++)
                    {
                        Ogre::String NombreMaterial = Formato + "." + Ogre::StringConverter::toString(i) + "." + Ogre::StringConverter::toString(j) + "." + Mapa;

                        mat = Ogre::MaterialManager::getSingleton().getByName(NombreMaterial);

                        if (!mat.isNull())
                        {
                            try
                            {
                                mat->getTechnique(1)->getPass(0)->getVertexProgramParameters()->setNamedConstant(Param, Value);
                            }
                            catch (...)
                            {
                            }
                        }
                    }
                }
            }
        }
    }

    void Hydrax::_setDepthFragmentProgramParameter(Ogre::String Param, float Value)
    {
        if (isComponent(HYDRAX_COMPONENT_DEPTH))
        {
            // Entity's depth shader
            Ogre::MaterialPtr mat;

            if (isComponent(HYDRAX_COMPONENT_CAUSTICS))
            {
                mat = Ogre::MaterialManager::getSingleton().getByName("HydraxShaderDepthCaustics");
            }
            else
            {
                mat = Ogre::MaterialManager::getSingleton().getByName("HydraxShaderDepth");
            }

            try
            {
                if (!mat.isNull())
                {
                    mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant(Param, Value);
                }
            }
            catch (...)
            {
            }

            // Terrain depth shader
            if (isComponent(HYDRAX_COMPONENT_PLSM2))
            {
                int w, h;

                mSceneManager->getOption("Width",  &w);
                mSceneManager->getOption("Height", &h);

                Ogre::String Mapa;
                Ogre::String Formato;

                mSceneManager->getOption("CurrentMap",           &Mapa);
                mSceneManager->getOption("CurrentTextureFormat", &Formato);

                for (int i = 0; i < w; i++)
                {
                    for (int j = 0; j < h; j++)
                    {
                        Ogre::String NombreMaterial = Formato + "." + Ogre::StringConverter::toString(i) + "." + Ogre::StringConverter::toString(j) + "." + Mapa;

                        mat = Ogre::MaterialManager::getSingleton().getByName(NombreMaterial);

                        if (!mat.isNull())
                        {
                            try
                            {
                                mat->getTechnique(1)->getPass(0)->getFragmentProgramParameters()->setNamedConstant(Param, Value);
                            }
                            catch (...)
                            {
                            }
                        }
                    }
                }
            }
        }
    }

    void Hydrax::_setDepthVertexProgramParameter(Ogre::String Param, Ogre::Vector3 Value)
    {
        if (isComponent(HYDRAX_COMPONENT_DEPTH))
        {
            // Entity's depth shader
            Ogre::MaterialPtr mat;

            if (isComponent(HYDRAX_COMPONENT_CAUSTICS))
            {
                mat = Ogre::MaterialManager::getSingleton().getByName("HydraxShaderDepthCaustics");
            }
            else
            {
                mat = Ogre::MaterialManager::getSingleton().getByName("HydraxShaderDepth");
            }

            try
            {
                if (!mat.isNull())
                {
                    mat->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant(Param, Value);
                }
            }
            catch (...)
            {
            }

            // Terrain depth shader
            if (isComponent(HYDRAX_COMPONENT_PLSM2))
            {
                int w, h;

                mSceneManager->getOption("Width",  &w);
                mSceneManager->getOption("Height", &h);

                Ogre::String Mapa;
                Ogre::String Formato;

                mSceneManager->getOption("CurrentMap",           &Mapa);
                mSceneManager->getOption("CurrentTextureFormat", &Formato);

                for (int i = 0; i < w; i++)
                {
                    for (int j = 0; j < h; j++)
                    {
                        Ogre::String NombreMaterial = Formato + "." + Ogre::StringConverter::toString(i) + "." + Ogre::StringConverter::toString(j) + "." + Mapa;

                        mat = Ogre::MaterialManager::getSingleton().getByName(NombreMaterial);

                        if (!mat.isNull())
                        {
                            try
                            {
                                mat->getTechnique(1)->getPass(0)->getVertexProgramParameters()->setNamedConstant(Param, Value);
                            }
                            catch (...)
                            {
                            }
                        }
                    }
                }
            }
        }
    }

    void Hydrax::_setDepthFragmentProgramParameter(Ogre::String Param, Ogre::Vector3 Value)
    {
        if (isComponent(HYDRAX_COMPONENT_DEPTH))
        {
            // Entity's depth shader
            Ogre::MaterialPtr mat;

            if (isComponent(HYDRAX_COMPONENT_CAUSTICS))
            {
                mat = Ogre::MaterialManager::getSingleton().getByName("HydraxShaderDepthCaustics");
            }
            else
            {
                mat = Ogre::MaterialManager::getSingleton().getByName("HydraxShaderDepth");
            }

            try
            {
                if (!mat.isNull())
                {
                    mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant(Param, Value);
                }
            }
            catch (...)
            {
            }

            // Terrain depth shader
            if (isComponent(HYDRAX_COMPONENT_PLSM2))
            {
                int w, h;

                mSceneManager->getOption("Width",  &w);
                mSceneManager->getOption("Height", &h);

                Ogre::String Mapa;
                Ogre::String Formato;

                mSceneManager->getOption("CurrentMap",           &Mapa);
                mSceneManager->getOption("CurrentTextureFormat", &Formato);

                for (int i = 0; i < w; i++)
                {
                    for (int j = 0; j < h; j++)
                    {
                        Ogre::String NombreMaterial = Formato + "." + Ogre::StringConverter::toString(i) + "." + Ogre::StringConverter::toString(j) + "." + Mapa;

                        mat = Ogre::MaterialManager::getSingleton().getByName(NombreMaterial);

                        if (!mat.isNull())
                        {
                            try
                            {
                                mat->getTechnique(1)->getPass(0)->getFragmentProgramParameters()->setNamedConstant(Param, Value);
                            }
                            catch (...)
                            {
                            }
                        }
                    }
                }
            }
        }
    }

    void Hydrax::setPosition(Ogre::Vector3 Position)
    {
        mPosition = Position;

        _setDepthVertexProgramParameter("uPlaneYPos", Position.y);

        mSceneNode->setPosition(Position);
        mPlanesSceneNode->setPosition(Position);
    }

    void Hydrax::setPlanesError(Ogre::Real PlanesError)
    {
        mPlanesError = PlanesError;
    }

    void Hydrax::setStrength(Ogre::Real Strength)
    {
        mMesh->setStrength(Strength);
    }

    void Hydrax::setVelocity(Ogre::Real Velocity)
    {
        mVelocity = Velocity;
    }

    void Hydrax::setFullReflectionDistance(Ogre::Real FullReflectionDistance)
    {
        mFullReflectionDistance = FullReflectionDistance;

        _setWaterFragmentProgramParameter("uFullReflectionDistance", FullReflectionDistance);
    }

    void Hydrax::setGlobalTransparency(Ogre::Real GlobalTransparency)
    {
        mGlobalTransparency = GlobalTransparency;

        _setWaterFragmentProgramParameter("uGlobalTransparency", GlobalTransparency);
    }

    void Hydrax::setNormalDistortion(Ogre::Real NormalDistortion)
    {
        mNormalDistortion = NormalDistortion;

        _setWaterFragmentProgramParameter("uNormalDistortion", NormalDistortion);
    }

    void Hydrax::setSunPosition(Ogre::Vector3 SunPosition)
    {
        if (!isComponent(HYDRAX_COMPONENT_SUN))
        {
            return;
        }

        mSunPosition = SunPosition;

        _setWaterFragmentProgramParameter("uSunPosition", SunPosition);
    }

    void Hydrax::setSunStrength(Ogre::Real SunStrength)
    {
        if (!isComponent(HYDRAX_COMPONENT_SUN))
        {
            return;
        }

        mSunStrength = SunStrength;

        _setWaterFragmentProgramParameter("uSunStrength", SunStrength);
    }

    void Hydrax::setSunArea(Ogre::Real SunArea)
    {
        if (!isComponent(HYDRAX_COMPONENT_SUN))
        {
            return;
        }

        mSunArea = SunArea;

        _setWaterFragmentProgramParameter("uSunArea", SunArea);
    }

    void Hydrax::setFoamMaxDistance(Ogre::Real FoamMaxDistance)
    {
        if (!isComponent(HYDRAX_COMPONENT_FOAM))
        {
            return;
        }

        mFoamMaxDistance = FoamMaxDistance;

        _setWaterFragmentProgramParameter("uFoamMaxDistance", FoamMaxDistance);
    }

    void Hydrax::setFoamScale(Ogre::Real FoamScale)
    {
        if (!isComponent(HYDRAX_COMPONENT_FOAM))
        {
            return;
        }

        mFoamScale = FoamScale;

        _setWaterFragmentProgramParameter("uFoamScale", FoamScale);
    }

    void Hydrax::setFoamStart(Ogre::Real FoamStart)
    {
        if (!isComponent(HYDRAX_COMPONENT_FOAM))
        {
            return;
        }

        mFoamStart = FoamStart;

        _setWaterFragmentProgramParameter("uFoamStart", FoamStart);
    }

    void Hydrax::setFoamTransparency(Ogre::Real FoamTransparency)
    {
        if (!isComponent(HYDRAX_COMPONENT_FOAM))
        {
            return;
        }

        mFoamTransparency = FoamTransparency;

        _setWaterFragmentProgramParameter("uFoamTransparency", FoamTransparency);
    }

    void Hydrax::setSunColor(Ogre::Vector3 SunColor)
    {
        if (!isComponent(HYDRAX_COMPONENT_SUN))
        {
            return;
        }

        mSunColor = SunColor;

        _setWaterFragmentProgramParameter("uSunColor", SunColor);
    }

    void Hydrax::setDepthColor(Ogre::Vector3 DepthColor)
    {
        if (!isComponent(HYDRAX_COMPONENT_DEPTH))
        {
            return;
        }

        mDepthColor = DepthColor;

        _setWaterFragmentProgramParameter("uDepthColor", DepthColor);
    }

    void Hydrax::setDepthLimit(Ogre::Real DepthLimit)
    {
        if (!isComponent(HYDRAX_COMPONENT_DEPTH))
        {
            return;
        }

        if (DepthLimit <= 0)
        {
            DepthLimit = 1;
        }

        mDepthLimit = DepthLimit;

        _setDepthFragmentProgramParameter("uDepthLimit", 1/DepthLimit);
    }

    void Hydrax::setSmoothPower(Ogre::Real SmoothPower)
    {
        if (!isComponent(HYDRAX_COMPONENT_SMOOTH))
        {
            return;
        }

        mSmoothPower = SmoothPower;

        _setWaterFragmentProgramParameter("uSmoothPower", SmoothPower);
    }

    void Hydrax::setCausticsScale(Ogre::Real CausticsScale)
    {
        if (!isComponent(HYDRAX_COMPONENT_CAUSTICS))
        {
            return;
        }

        mCausticsScale = CausticsScale;

        _setDepthFragmentProgramParameter("uCausticsScale", CausticsScale);
    }

    void Hydrax::setCausticsPower(Ogre::Real CausticsPower)
    {
        if (!isComponent(HYDRAX_COMPONENT_CAUSTICS))
        {
            return;
        }

        mCausticsPower = CausticsPower;

        _setWaterFragmentProgramParameter("uCausticsPower", CausticsPower);
    }

    void Hydrax::setCausticsEnd(Ogre::Real CausticsEnd)
    {
        if (!isComponent(HYDRAX_COMPONENT_CAUSTICS))
        {
            return;
        }

        mCausticsEnd = CausticsEnd;

        _setWaterFragmentProgramParameter("uCausticsEnd", CausticsEnd);
    }
}
