/** \file    NxOgreOBJStream.h
 *  \brief   Header for the OBJStream class.
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

#ifndef __NXOGRE_OBJ_STREAM_H__
#define __NXOGRE_OBJ_STREAM_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreResourceSystem.h"
#include "NxOgreResourceStream.h"
#include "NxStream.h"

namespace NxOgre {

	/** \brief A Simple interface to write and read obj files (pressumed).

		\note	File format specification is from http://people.scs.fsu.edu/~burkardt/txt/obj_format.txt

	*/
	class NxPublicClass OBJStream {
		
		public:
			
			OBJStream(ResourceStreamPtr);
			~OBJStream();

			void load();
			void save();
			
			NxVec3* getVertices();
			NxU32	getNbVertices();
			NxU32*	getIndices();
			NxU32	getNbIndices();
			NxVec3*	getNormals();
			float2*	getTextureCoordinates();

			void	setVertices(NxVec3*, NxU32 nbVertices);
			void	setNormals(NxVec3*, NxU32 nbNormals);
			void	setTextureCoodinates(float2*, NxU32 nbCoords);
			void	setIndices(NxU32*, NxU32 nbIndices);

		protected:
			

			NxVec3*	mVertices;
			NxVec3*	mNormals;
			float2*	mTextureCoordinates;
			NxU32*	mIndices;
			
			NxU32	mNbIndices;
			NxU32	mNbVertices;
			NxU32	mNbNormals;
			NxU32	mNbTextureCoordinates;

			ResourceStreamPtr	mStream;

			void	__freeAll();
	};

};

#endif
