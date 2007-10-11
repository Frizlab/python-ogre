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
    return col.a * mFogDensityMultiplier;
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
	Ogre::ColourValue col = getInterpolatedColour (elevation, elevation, mSkyGradientsImage);
	return Ogre::ColourValue::White * (col.r + col.g + col.b) / 3;
}

Ogre::ColourValue SkyColourModel::getInterpolatedColour (float x, float height, Ogre::Image *img, bool wrap) {
	// Get the image width
	int width = static_cast<int>(img->getWidth ()) - 1;

	// calculate the height
	height = Ogre::Math::Abs (height);
	int y = (int )((img->getHeight () - 1) * height);
	if (y >= static_cast<int>(img->getHeight ()))
		y--;

	// Get the two closest pixels
	int curPix, auxPix;
	float diff;
	curPix = (int )(width * x);
	diff = width * x - curPix;
	if (diff < 0) {
		auxPix = curPix - 1;
	}
	else {
		auxPix = curPix + 1;
	}

	// Calculate the interpolated pixel
	Ogre::ColourValue c1, c2, cf;
    c1 = img->getColourAt (wrap ? curPix % width : curPix < 0 ? 0 : curPix > width ? width : curPix, y, 0);
    c2 = img->getColourAt (wrap ? auxPix % width : auxPix < 0 ? 0 : auxPix > width ? width : auxPix, y, 0);
	cf = c1 * (1 - diff) + c2 * diff;

	return cf;
}

} // namespace caelum
