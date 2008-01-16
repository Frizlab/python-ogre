//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef __NXOGRE_MATERIAL_H__
#define __NXOGRE_MATERIAL_H__

#include "NxOgrePrerequisites.h"

namespace NxOgre {

	class NxExport Material {

		public:

			///	Material Constructor
			///  Creates a material for a shape, and automatically assign a NxMaterialIndex to it.
			///
			///
			/// Containers and Garbage Collection
			///	 Materials are used in containers. Creating a Material through the Scene factory method makes the Material "owned"
			///  by NxOgre, on deletion of the Material it will remove the presence of theMaterial and delete the pointer.
			///  Manually creating the Material "new Material()" will only add a presence, it is up to you to delete it at the
			///  correct time.
			///
			/// @params	Identifier				Unique identifier of the Material
			/// @params Material*				Scene pointer which would contain the Material.
			///
			/// @see Scene::createMaterial
			Material(const NxString& identifier, Scene*);

			
			///	Material Constructor
			///  Creates a material for a shape, with a specific NxMaterialIndex
			///
			///
			/// Containers and Garbage Collection
			///	 Materials are used in containers. Creating a Material through the Scene factory method makes the Material "owned"
			///  by NxOgre, on deletion of the Material it will remove the presence of theMaterial and delete the pointer.
			///  Manually creating the Material "new Material()" will only add a presence, it is up to you to delete it at the
			///  correct time.
			///
			/// @params	NxMaterialIndex			MaterialIndex to use.
			/// @params	Identifier				Unique identifier of the Material
			/// @params Material*				Scene pointer which would contain the Material.
			///
			/// @see Scene::createMaterial
			Material(NxMaterialIndex, const NxString&, Scene*);

			/// Scene Destructor
			///  Text
			///
			~Material();

			/// getMaterialIndex
			///  Text
			NxMaterialIndex	getMaterialIndex();
			
			NxString		getName() const							{
																		return mName;
																	}


			Scene*			getScene()								{
																		return mScene;
																	}

			void			setAll(NxReal bounce, NxReal dFriction, NxReal sFriction);
			void			setDynamicFriction(NxReal coef);
			NxReal			getDynamicFriction() const;
			void			setStaticFriction(NxReal coef);
			NxReal			getStaticFriction() const;
			void			setRestitution(NxReal rest);
			NxReal			getRestitution() const;
			void			setDynamicFrictionV(NxReal coef);
			NxReal			getDynamicFrictionV() const;
			void			setStaticFrictionV(NxReal coef);
			NxReal			getStaticFrictionV() const;
			void			setDirOfAnisotropy(const Ogre::Vector3 &vec);
			NxVec3			getDirOfAnisotropy() const;
			void			setFlags(NxU32 flags);
			NxU32			getFlags() const;
			void			setFrictionCombineMode(NxCombineMode combMode);
			NxCombineMode	getFrictionCombineMode() const;
			void			setRestitutionCombineMode(NxCombineMode combMode) ;
			NxCombineMode	getRestitutionCombineMode();

		protected:

			NxMaterial*		mMaterial;
			NxMaterialIndex mMaterialIndex;
			NxString		mName;
			Scene*			mScene;


	};

};

#endif
