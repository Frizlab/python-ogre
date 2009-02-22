/***************************************************************************
OgrePagingLandScapeTexture_InstantBaseTexture.cpp  -  description
-------------------
begin                : Mon Apr 26 2004
copyright            : (C) 2003-2006 by Jose A Milan & Tuan Kuranes
email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/



#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreStringConverter.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture_InstantBaseTexture.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_InstantBaseTexture::newTexture()
    {
        return new PagingLandScapeTexture_InstantBaseTexture(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_InstantBaseTexture::isMaterialSupported()
    {        
		if (mParent->getOptions()->NumMatHeightSplat > 3)
			return true;
		else
			return false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTexture::PagingLandScapeTexture_InstantBaseTexture(PagingLandScapeTextureManager *textureMgr) 
        : 
        PagingLandScapeTexture(textureMgr, "InstantBase", 1, false)
    {

    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTexture::~PagingLandScapeTexture_InstantBaseTexture()
    {
	    
    }
} //namespace
