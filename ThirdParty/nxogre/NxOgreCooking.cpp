/** \file    NxOgreCooking.cpp
 *  \see     NxOgreCooking.h
 *  \version 1.0-20
 *
 *  \licence NxOgre a wrapper for the PhysX physics library.
 *           Copyright (C) 2005-8 Robin Southern of NxOgre.org http://www.nxogre.org
 *           This library is free software; you can redistribute it and/or
 *           modify it under the terms of the GNU Lesser General Public
 *           License as published by the Free Software Foundation; either
 *           version 2.1 of the License, or (at your option) any later version.
 *           
 *           This library is distributed in the hope that it will be useful,
 *           but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *           Lesser General Public License for more details.
 *           
 *           You should have received a copy of the GNU Lesser General Public
 *           License along with this library; if not, write to the Free Software
 *           Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "NxOgreStable.h"
#include "NxOgreCooking.h"
#include "NxOgreMemoryStream.h"
#include "NxCooking.h"
#include "NxOgreHelpers.h"

#include "OgreImage.h"
#include "OgreResourceGroupManager.h"
#include "OgreRoot.h"
#include "OgreSingleton.h"
#include "OgreMesh.h"
#include "OgreSubMesh.h"
#include "OgreVector3.h"

namespace NxOgre {


#if 0
////////////////////////////////////////////////////////////////////////////////////////////////

NxHeightField* NxGenerateHeightFieldFromImage(const NxString& imageFilename, unsigned int nbRows,NxScene *scene) {

	Ogre::Image img;
	img.load(imageFilename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	unsigned int imageSize = img.getWidth();

	NxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.nbColumns         = nbRows;
	heightFieldDesc.nbRows            = nbRows;
	heightFieldDesc.verticalExtent      = -1000;
	heightFieldDesc.convexEdgeThreshold = 0;

	// Allocate storage for heightfield samples
	heightFieldDesc.samples         = new NxU32[imageSize * imageSize];
	heightFieldDesc.sampleStride    = sizeof(NxU32);

	char* currentByte = (char*) heightFieldDesc.samples;
	int gMatrixSize = 3;
	NxReal sixtyFourKb = 65536.0;
	NxReal thirtyTwoKb = 32768.0;

	NxMaterialIndex gMatrix[9][3] = {
		// {tesselation, material0, material1}
		{0,1,1}, {0,1,1}, {0,1,1},
		{0,1,1}, {0,1,1}, {0,1,1},
		{0,1,1}, {0,1,1}, {0,1,1}
	};
	const Ogre::uchar* pSrc = img.getData();

	for (NxU32 row = 0; row < nbRows; row++) {
		for (NxU32 column = 0; column < nbRows; column++) {

			NxU32 matrixOffset = (row % gMatrixSize) * gMatrixSize + (column % gMatrixSize);

			NxReal s = NxReal(row) / NxReal(nbRows);
			NxReal t = NxReal(column) / NxReal(nbRows);

			NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte;
			float colorValue = img.getColourAt (row, column,0).r;

			currentSample->height = img.getColourAt (row, column,0).r * 32768.0f;

			currentSample->materialIndex0 = gMatrix[matrixOffset][1];
			currentSample->materialIndex1 = gMatrix[matrixOffset][2];
			currentSample->tessFlag = gMatrix[matrixOffset][0];

			currentByte += heightFieldDesc.sampleStride;
		}
	}

	NxHeightField *hf = scene->getPhysicsSDK().createHeightField(heightFieldDesc);

	delete [] heightFieldDesc.samples;

	return hf;

}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////

NxHeightField* NxGenerateHeightFieldFromImage(const NxString& imageFilename, NxScene* scene) {

	Ogre::Image img;
	img.load(imageFilename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	NxU32 imageWidth = img.getWidth();
	NxU32 imageHeight = img.getHeight();

	NxHeightFieldDesc heightFieldDesc;
	
	heightFieldDesc.nbRows					= imageWidth;
	heightFieldDesc.nbColumns				= imageHeight;
	heightFieldDesc.verticalExtent			= -1000;
	heightFieldDesc.convexEdgeThreshold		= 0;

	// Allocate storage for heightfield samples
	heightFieldDesc.samples         = new NxU32[imageWidth * imageHeight];
	heightFieldDesc.sampleStride    = sizeof(NxU32);

	char* currentByte = (char*) heightFieldDesc.samples;
	int gMatrixSize = 3;
	
	NxMaterialIndex gMatrix[9][3] = {
		// {tesselation, material0, material1}
		{0,1,1}, {0,1,1}, {0,1,1},
		{0,1,1}, {0,1,1}, {0,1,1},
		{0,1,1}, {0,1,1}, {0,1,1}
	};
	const Ogre::uchar* pSrc = img.getData();
	NxReal colourValue = 0;

	for (NxU32 row = 0; row < imageWidth; row++) {
		for (NxU32 column = 0; column < imageHeight; column++) {

			NxU32 matrixOffset = (row % gMatrixSize) * gMatrixSize + (column % gMatrixSize);

			NxReal s = NxReal(row) / NxReal(imageWidth);
			NxReal t = NxReal(column) / NxReal(imageHeight);

			NxHeightFieldSample* currentSample = (NxHeightFieldSample*) currentByte;
			colourValue = img.getColourAt (row, column,0).r;

			currentSample->height = colourValue * 32768.0f;

			currentSample->materialIndex0 = gMatrix[matrixOffset][1];
			currentSample->materialIndex1 = gMatrix[matrixOffset][2];
			currentSample->tessFlag = gMatrix[matrixOffset][0];

			currentByte += heightFieldDesc.sampleStride;
		}
	}

	NxHeightField *hf = scene->getPhysicsSDK().createHeightField(heightFieldDesc);

#ifdef NX_PLATFORM_LINUX
	delete [] (NxU32*) heightFieldDesc.samples;
#else
	delete [] heightFieldDesc.samples;
#endif

	return hf;

}

////////////////////////////////////////////////////////////////////////////////////////////////

NxHeightField* NxGenerateHeightFieldFromRaw(const NxString& imageFilename, NxScene* scene) {

	Ogre::MemoryDataStreamPtr mRawData;
	mRawData.setNull();

	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(imageFilename);
	mRawData = Ogre::MemoryDataStreamPtr(new Ogre::MemoryDataStream(imageFilename, stream));

	// Work out the Image size by the size of the file. This algorithm only works if the texture is square. But since
	// Ogre terrain works this way as well, it isn't much of a problem.
	NxU32 fileLength = mRawData->size();
	NxReal width = NxMath::sqrt(NxReal(fileLength));
	bool  is8bit = true;

	// Check to see if the image is 16-bit. The square root of all image filesizes which are 16-bit, produce a non-integer
	// number. To get the real imageWidth, the fileLength is halfed and square-rooted again, which produces an integer.
	if (NxU32(NxMath::floor(width)) != width) {
		width = NxMath::sqrt(NxReal(fileLength * 0.5f));
		is8bit = false;
	}

	/// And here we go.

	NxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.nbColumns			= width;
	heightFieldDesc.nbRows				= width;
	heightFieldDesc.verticalExtent      = -1000;
	heightFieldDesc.convexEdgeThreshold = 0;

	// Allocate storage for heightfield samples
	heightFieldDesc.samples         = new NxU32[heightFieldDesc.nbRows * heightFieldDesc.nbColumns];
	heightFieldDesc.sampleStride    = sizeof(NxU32);

	char* currentByte = (char*) heightFieldDesc.samples;
	NxU32 gMatrixSize = 3;

	NxMaterialIndex gMatrix[9][3] = {
		// {tesselation, material0, material1}

		{0,1,1},	{0,1,1},	{0,1,1},

		{0,1,1},	{0,1,1},	{0,1,1},

		{0,1,1},	{0,1,1},	{0,1,1}

	};

	const Ogre::uchar* pSrc = mRawData->getPtr();

	NxU32 i = 0;
	NxReal height = 0;
	NxI16 short_height = 0;

	for (NxU32 row = 0; row < width; row++) {
		for (NxU32 column = 0; column < width; column++) {
		
			NxU32 matrixOffset = (row % gMatrixSize) * gMatrixSize + (column % gMatrixSize);

			NxReal s = NxReal(row) / NxReal(width);
			NxReal t = NxReal(column) / NxReal(width);

			NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte;

			i = row + (column * width);

			if (is8bit) {
				height = pSrc[i];
				height *= 257.0f;
				short_height = height;
				short_height -= 32768;
			}
			else {
				i*=2;
				short_height  =	pSrc[i];
				short_height += pSrc[i + 1] << 8;
				short_height -= 32768;
			}
			
			currentSample->height = short_height;    
			currentSample->materialIndex0 = gMatrix[matrixOffset][1];
			currentSample->materialIndex1 = gMatrix[matrixOffset][2];
			currentSample->tessFlag		  = gMatrix[matrixOffset][0];

			currentByte += heightFieldDesc.sampleStride;
		}
	}

	NxHeightField *hf = scene->getPhysicsSDK().createHeightField(heightFieldDesc);

#ifdef NX_PLATFORM_LINUX
	delete [] (NxU32*) heightFieldDesc.samples;
#else
	delete [] heightFieldDesc.samples;
#endif

	return hf;
}

////////////////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.

