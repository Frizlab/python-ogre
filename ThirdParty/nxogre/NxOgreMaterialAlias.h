/** \file    NxOgreMaterialAlias.h
 *  \brief   Header for the Material Alias class.
 *  \version 1.0-21
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


#ifndef __NXOGRE_MATERIAL_ALIAS_H__
#define __NXOGRE_MATERIAL_ALIAS_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreMaterial.h"				// For SharedMap<NxString, Material>
#include "NxOgreContainer.h"

namespace NxOgre {
namespace Resources {

class NxPublicClass MaterialAlias
{

    friend class ResourceSystem;
    friend class Mesh;
    friend class ManualMesh;
    friend class ManualMeshUtil;

  public:

      struct MaterialPair
      {
       std::string  mName;
       unsigned int mIndex;
      };

      typedef Betajaen::SharedList<MaterialPair> MaterialPairs;

      MaterialAlias();
      MaterialAlias(const ResourceIdentifier&);
      MaterialAlias(Resource*);
      ~MaterialAlias();

      enum Reason
      {
       R_NotAMaterialAliasFile,
       R_InvalidHeader,
       R_InvalidData
      };

      void          insertMaterial(NxMaterialIndex, const NxString&);
      MaterialPairs getMaterialPairs();

      void load(Resource*);
      void load(const ResourceIdentifier&);
      void save(Resource*);
      void save(const ResourceIdentifier&);

  protected:

      MaterialPairs  mMaterials;

};

}; // End of Resources namespaces.
}; // End of NxOgre namespaces.

#endif
