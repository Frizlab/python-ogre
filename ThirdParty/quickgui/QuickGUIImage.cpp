#include "QuickGUIPrecompiledHeaders.h"

#include "OgreMaterial.h"
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "QuickGUIImage.h"
#include "QuickGUIManager.h"
#include "QuickGUIVector4.h"

namespace QuickGUI
{
	Image::Image(const std::string& name, GUIManager* gm) :
		Widget(name,gm),
		mMaterialName("")
	{
		mWidgetType = TYPE_IMAGE;
		mSkinComponent = ".image";
		mSize = Size(50,50);
	}

	Image::~Image()
	{

	}

	std::string Image::getMaterialName()
	{
		return mMaterialName;
	}

	bool Image::overTransparentPixel(const Point& mousePixelPosition)
	{
		if(mMaterialName == "")
			return Widget::overTransparentPixel(mousePixelPosition);
		return false;
	}

	void Image::setMaterial(const std::string& materialName)
	{
		if(mTextureLocked)
			return;

        // Remove old wrapper
        if (!mWrapper.isNull())
        {
            Ogre::MaterialManager::getSingleton().remove (mWrapper);
            mWrapper.setNull();
        }

		mMaterialName = materialName;
		mQuad->setMaterial(mMaterialName);
		mQuad->setTextureCoordinates(Vector4(0,0,1,1));
	}

	void Image::setSkin(const std::string& skinName, bool recursive)
	{
		mMaterialName = "";
		Widget::setSkin(skinName,recursive);
	}

    void Image::setTexture(const std::string& textureName)
    {
		if(mTextureLocked)
			return;

        // Find material name
        std::string materialName = "QuickGUI" + getInstanceName();

        while (Ogre::MaterialManager::getSingleton().resourceExists (materialName))
            materialName += '2';

        // Remove old wrapper
        if (!mWrapper.isNull())
        {
            Ogre::MaterialManager::getSingleton().remove (mWrapper);
            mWrapper.setNull();
        }

        // Create new wrapper
        mWrapper = Ogre::MaterialManager::getSingleton().create (materialName,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr> (mWrapper);
        Ogre::Pass *pass = material->getTechnique (0)->getPass (0);
        pass->createTextureUnitState (textureName);
        pass->setSceneBlending (Ogre::SBT_TRANSPARENT_ALPHA);

        // Use wrapping material
		mMaterialName = materialName;
		mQuad->setMaterial(mMaterialName);
		mQuad->setTextureCoordinates(Vector4(0,0,1,1));
    }
}
