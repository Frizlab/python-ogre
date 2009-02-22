/***************************************************************************
OgrePagingLandScapeTexture_Splatting.cpp  -  description
	-------------------
	begin                : Mon Apr 16 2004
	copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
	email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreStringConverter.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture_Splatting4.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
	void PagingLandScapeTexture_Splatting4::setOptions(void)
    {
        mParent->getOptions()->VertexCompression = false;
        mParent->getOptions()->lodMorph = false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting4::newTexture()
    {
        return new PagingLandScapeTexture_Splatting4(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting4::isMaterialSupported()
    {                                                        
		const PagingLandScapeOptions * const opt = mParent->getOptions();
		
		if (opt->NumMatHeightSplat < 3)
			return false;
        if (opt->numTextureUnits < 2)
            return false;
            
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting4::PagingLandScapeTexture_Splatting4(PagingLandScapeTextureManager *textureMgr) 
		: 
		PagingLandScapeTexture(textureMgr, "Splatting4",  5, true)
    {
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting4::~PagingLandScapeTexture_Splatting4()
    {
    }
} //namespace
