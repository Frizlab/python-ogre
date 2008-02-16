/** \file    NxOgreTransitionalMesh.h
 *  \brief   Header for the TransitionalMesh class.
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

#ifndef __NXOGRE_RESOURCEMESH_H__
#define __NXOGRE_RESOURCEMESH_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreResourceManager.h"

namespace NxOgre {

	/** \brief A small class to allow your own mesh to be loaded directly into NxOgre
			   without bothering the classes that use them.
	*/
	class NxPublicClass  TransitionalMesh {

		public:


			/** \brief A TransitionalMesh disassembles a mesh (in any format; providing there
					   is a TransitionalMesh class) into lists of vertices, indices, etc. so
					   any NxOgre class can use the TransitionalMesh so they can be converted
					   into something else.
			*/
			TransitionalMesh(ResourceStreamPtr resourceStream);


			/** \brief Isn't it obvious?
			*/
			virtual ~TransitionalMesh();


			/** \brief Is the Mesh vertices, normals, texture coordinates and indices
					   empty. Used to see if the mesh has been loaded or not.
			*/
			bool isEmpty() const;


			/** \brief Erase all the vertices, indices, texture coordinates and normals.
			*/
			void empty() const;


			/** \brief Calculates the upper and lowest point of the mesh.
			*/
			Betajaen::Vector2< float3 > getBoundingBox() const;


			/** \brief Parse the contents of the mesh into the mVertices, mNormals, mTextureCoordinates,
					   and mIndices.
				\note  Loading in vertices is assumed. Other features of the mesh depends on the file
				       format.
				\param indices Load in the indices.
				\param textureCoords Load in the Texture Coordinates.
				\param nomrals Load in the Normals.
				
			*/
			virtual void load(bool indices, bool textureCoords, bool normals, bool materials) = 0;


			/** \brief Saves the transitional mesh data into the resource stream given.
			*/
			virtual void save(bool indices, bool textureCoords, bool normals, bool materials) = 0;


			/** \brief Get a pointer to the first vertex. Used with getNbVertices for a iterator
					   based interface.
			*/
			float3* getVertices();


			/** \brief Get the number of vertices.
			*/
			unsigned int getNbVertices() const;


			/** \brief Get a pointer to the first index. Used with getNbIndices for a iterator
					   based interface.
			*/
			unsigned int* getIndices();


			/** \brief Get the number of indices. 
				\note The number of Indices, Normals and Texture coordinates are always identical.
					  However it cannot be certain that the normals, texture coordinates or even
					  indices are actually loaded.

			*/
			unsigned int getNbIndices() const;


			/** \brief Get a pointer to the first normal. Used with getNbIndices for a iterator
					   based interface.
			*/
			float3*	getNormals();


			/** \brief Get the number of Normals
			*/
			unsigned int	getNbNormals() const;


			/** \brief Get a pointer to the first texture coordinate. Used with getNbIndices for 
					   a iterator based interface.
			*/
			float2*	getTextureCoordinates();


			/** \brief Get the number of Texture coordinates
			*/
			unsigned int	getNbTextureCoordinates() const;


			/** \brief Get the number of IndexMaterials
			*/
			unsigned int	getNbIndexMaterials() const;


			/** \brief Get a pointer to the first index material. Used with getNbIndices for
					   a iterator based interface.
				\note  The index material identifier should be translated to something else
					   more meaningful. Usually this is upto sub-classed TransitionalMesh
					   or the class that is using it.
			*/
			unsigned char*	getIndexMaterials();


		protected:


			/** \brief Clean's the vertices/normals/texture coords/indices up. Removes duplicates, etc.
			*/
			void _cleanMesh();

			/** \brief Reserve vertices.
			*/
			void _reserveVertices(unsigned int nbVertices);

			/** \brief Reserve indices.
			*/
			void _reserveNormals(unsigned int nbNormals);

			/** \brief Reserve indices.
			*/
			void _reserveIndices(unsigned int nbIndices);

			/** \brief Reserve indices.
			*/
			void _reserveTextureCoordinates(unsigned int nbTextureCoordinates);

			/** \brief Reserve Indices Materials
			*/
			void _reserveIndexMaterials(unsigned int nbIndexMaterials);

			float3*             mVertices;
			float3*             mNormals;
			float2*             mTextureCoordinates;
			unsigned int*       mIndices;
			unsigned char*      mIndexMaterials;
			unsigned int        mNbIndices;
			unsigned int        mNbVertices;
			unsigned int        mNbNormals;
			unsigned int        mNbTextureCoordinates;
			unsigned int        mNbIndexMaterials;
			ResourceStreamPtr   mStream;

	};


};

#endif
