#include "CaelumPrecompiled.h"
#include "SkyColourModel.h"

namespace caelum {

SkyColourModel::SkyColourModel
(
        const Ogre::String &gradients,
        const Ogre::String &sunColours
):
        mSunColoursImage(0),
        mSkyGradientsImage(0)
{
	mSkyGradientsImage = 0;
    mFogDensityMultiplier = 1;
    setSkyGradientsImage(gradients);
    setSunColoursImage(sunColours);
}

SkyColourModel::~SkyColourModel () {
	if (mSkyGradientsImage != 0) {
		delete mSkyGradientsImage;
		mSkyGradientsImage = 0;
	}
	if (mSunColoursImage != 0) {
		delete mSunColoursImage;
		mSunColoursImage = 0;
	}
}

void SkyColourModel::setSkyGradientsImage (const Ogre::String &filename) {
	if (mSkyGradientsImage != 0) {
		delete mSkyGradientsImage;
		mSkyGradientsImage = 0;
	}

	mSkyGradientsImage = new Ogre::Image ();
	mSkyGradientsImage->load (filename, RESOURCE_GROUP_NAME);
}

void SkyColourModel::setSunColoursImage (const Ogre::String &filename) {
	if (mSunColoursImage != 0) {
		delete mSunColoursImage;
		mSunColoursImage = 0;
	}

	mSunColoursImage = new Ogre::Image ();
	mSunColoursImage->load (filename, RESOURCE_GROUP_NAME);
}

Ogre::ColourValue SkyColourModel::getFogColour (float time, const Ogre::Vector3 &sunDir) {
	float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y) * 0.5 + 0.5;
	Ogre::ColourValue col = getInterpolatedColour (elevation, 1, mSkyGradientsImage, false);
    return col;
}

float SkyColourModel::getFogDensity (float time, const Ogre::Vector3 &sunDir) {
	float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y) * 0.5 + 0.5;
	Ogre::ColourValue col = getInterpolatedColour (elevation, 1, mSkyGradientsImage, false);
    return col.a;
}

Ogre::ColourValue SkyColourModel::getSunSphereColour (float time, const Ogre::Vector3 &sunDir) {
    float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y);
    elevation = elevation * 2 + 0.4;

    if (mSunColoursImage == 0) {
        return Ogre::ColourValue::White;
    } else {
        return getInterpolatedColour (elevation, 1, mSunColoursImage, false);
    }
}

Ogre::ColourValue SkyColourModel::getSunLightColour (float time, const Ogre::Vector3 &sunDir) {
	float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y) * 0.5 + 0.5;

	// Hack: return averaged sky colours
	Ogre::ColourValue col = getInterpolatedColour (elevation, elevation, mSkyGradientsImage, false);
	return Ogre::ColourValue::White * (col.r + col.g + col.b) / 3;
}

Ogre::ColourValue SkyColourModel::getInterpolatedColour (float fx, float fy, Ogre::Image *img, bool wrapX) {
	// Don't -> all the time, and avoid unsigned warnings
    int imgWidth = static_cast<int>(img->getWidth ());
    int imgHeight = static_cast<int>(img->getHeight ());

	// Calculate pixel y coord.
    int py = Ogre::Math::IFloor(Ogre::Math::Abs (fy) * (imgHeight - 1));
    // Snap to py image bounds.
    py = std::max(0, std::min(py, imgHeight - 1));

	// Get the two closest pixels on x.
    // px1 and px2 are the closest integer pixels to px.
    float px = fx * (img->getWidth () - 1);
	int px1, px2;
    px1 = Ogre::Math::IFloor(px);
    px2 = Ogre::Math::ICeil(px);

    if (wrapX) {
        // Wrap x coords. The funny addition ensures that it does
        // "the right thing" for negative values.
        px1 = (px1 % imgWidth + imgWidth) % imgWidth;
        px2 = (px2 % imgWidth + imgWidth) % imgWidth;
    } else {
        px1 = std::max(0, std::min(px1, imgWidth - 1));
        px2 = std::max(0, std::min(px2, imgWidth - 1));
    }

	// Calculate the interpolated pixel
	Ogre::ColourValue c1, c2, cf;
    c1 = img->getColourAt (px1, py, 0);
    c2 = img->getColourAt (px2, py, 0);

    // Blend the two pixels together.
    // diff is the weight between pixel 1 and pixel 2.
    float diff = px - px1;
	cf = c1 * (1 - diff) + c2 * diff;

	return cf;
}

} // namespace caelum
