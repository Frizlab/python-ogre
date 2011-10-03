/***************************************************************************
OgrePagingLandScapeTexture_Splatting.h  -  description
	-------------------
	begin                : Mon Apr 16 2004
	copyright            : (C) 2002-2006 by Jose A Milan & Tuan Kuranes
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

#ifndef PAGINGLandScapeTEXTURE_SPLATTING2_H
#define PAGINGLandScapeTEXTURE_SPLATTING2_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeTexture_Splatting2 : public PagingLandScapeTexture
    {
        public:
            PagingLandScapeTexture_Splatting2(PagingLandScapeTextureManager *textureMgr);
             String getName() const{return String("Splatting2");}

	         ~PagingLandScapeTexture_Splatting2(void);

			 void setOptions(void);

             PagingLandScapeTexture* newTexture();
             bool isMaterialSupported(); 

    };
}

#endif
