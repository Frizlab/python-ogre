/** @mainpage NxOgre
@section licence Licence

<pre>
NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
</pre>

@section whatisit What is it?
NxOgre is a physics connector library between the physics library PhysX and the Ogre 3D rendering system. 
With a help of some short, fast and neat code you can have a quick physics system within your Ogre game 
or engine within a matter of minutes.

@section installing Installing
@subsection Ogre

The SDK or Source versions will both work with NxOgre. You should have some knowledege with working Ogre. 
If you can make a simple application that can show an Ogre head with a movement camera, you can work with 
NxOgre.
\n\n
You will need to create an environmental variable called "OGRE_HOME" pointing to the ogremain directory 
in your Ogre folder. Normally for SDK users this is automatically created by the installer, but it's work 
checking, if you installed Ogre from the source you'll need to create one.

[1] http://www.ogre3d.org/wiki/index.php/Installing_An_SDK											\n
[2] http://www.ogre3d.org/wiki/index.php/Building_From_Source										\n

@subsection PhysX
The PhysX SDK can be downloaded from the Ageia Support Center, a user account is required to download the 
SDK. Once your account has been registered (Accounts are processed by hand, so a wait of 1-2 business days 
is common), download the PhysX SDK and the SystemSoftware.
\n\n
Install the SystemSoftware first, then the PhysX SDK. I recommend you install the PhysX SDK in the same 
parent directory as Ogre. Once the installation is completed, create a Environmental variable called 
"<code>PHYSX_DIR</code>" pointing to the PhysX SDK directory. 
\n\n
[3] http://devsupport.ageia.com/																	\n

@subsection NxOgre
	- Create a "NxOgre" folder, it should be in the parent of your Ogre or nearby
	- Using Tortoise SVN or another Subversion client checkout the HEAD repositry
	- Go to the parent directory of Ogre and the PhysX SDK i.e. (c:/dev/)
	- New Folder, name as NxOgre, and go inside
	- File, SVN, SVN Checkout
	- Set the URL to http://latest.nxogre.org/nxogre3/
	- Click okay
	- Open up the project and compile.

*/

#ifndef __NXOGRE_H__
#define __NXOGRE_H__

///////////////////////////////////////////

#include "NxOgreStable.h"
#include "NxOgrePrerequisites.h"
#include "NxOgrePhysXDriver.h"
#include "NxOgreError.h"
#include "NxOgreLog.h"
#include "NxOgreExtendedTypes.h"

///////////////////////////////////////////

#include "NxOgreActor.h"
#include "NxOgreActorBlueprint.h"
#include "NxOgreBody.h"
#include "NxOgreContactStream.h"
#include "NxOgreContainer.h"
#include "NxOgreCooking.h"
#if NX_USE_LEGACY_NXCONTROLLER
#	include "NxOgreLegacyCharacter.h"
#	include "NxOgreLegacyCharacterController.h"
#	include "NxOgreLegacyCharacterHitReport.h"
#else
#	include "NxOgreCharacterSystem.h"
#	include "NxOgreCharacter.h"
#	include "NxOgreCharacterModel.h"
#	include "NxOgreCharacterMovementModel.h"
#endif
#include "NxOgreCloth.h"
#include "NxOgreClothRaycaster.h"
#include "NxOgreCompound.h"
#include "NxOgreDebugRenderer.h"
#include "NxOgreDominanceGroup.h"
#include "NxOgreEngine.h"
#include "NxOgreEnumsBlueprint.h"
#include "NxOgreFileResourceSystem.h"
#include "NxOgreFluid.h"
#include "NxOgreFluidMesh.h"
#include "NxOgreForceField.h"
#include "NxOgreGroup.h"
#include "NxOgreHelpers.h"
#include "NxOgreJointSet1.h"
#include "NxOgreJointSet2.h"
#include "NxOgreIntersection.h"
#include "NxOgreMaterial.h"
#include "NxOgreMachine.h"
#include "NxOgreNodeRenderable.h"
#include "NxOgreResourceManager.h"
#include "NxOgreOgreResourceSystem.h"
#include "NxOgreParams.h"
#include "NxOgrePose.h"
#include "NxOgreRaycaster.h"
#include "NxOgreRenderable.h"
#include "NxOgreRenderableSource.h"
#include "NxOgreRemoteDebuggerConnection.h"
#include "NxOgreResourceSystem.h"
#include "NxOgreResourceStreamPtr.h"
#include "NxOgreSerialiser.h"
#include "NxOgreScene.h"
#include "NxOgreSceneBlueprint.h"
#include "NxOgreSceneController.h"
#include "NxOgreSceneContactController.h"
#include "NxOgreSceneRenderer.h"
#include "NxOgreShape.h"
#include "NxOgreShapeBlueprint.h"
#include "NxOgreShapeBlueprintConvex.h"
#include "NxOgreShapeBlueprintMesh.h"
#include "NxOgreShapeBlueprintPrimitives.h"
#include "NxOgreShapeBlueprintTerrain.h"
#include "NxOgreShapeBlueprintWheel.h"
#include "NxOgreShapeConvex.h"
#include "NxOgreShapeMesh.h"
#include "NxOgreShapePrimitives.h"
#include "NxOgreShapeTerrain.h"
#include "NxOgreShapeWheel.h"
#include "NxOgreSimpleShape.h"
#include "NxOgreSoftBody.h"
#include "NxOgreState.h"
#include "NxOgreStateMachine.h"
#include "NxOgreTesting.h"
#include "NxOgreTrigger.h"
#include "NxOgreMemoryStream.h"
#include "NxOgreUserAllocator.h"
#include "NxOgreUserData.h"
#include "NxOgreWheelSet.h"
#include "NxOgreWorld.h"
#include "NxOgreWorldBlueprint.h"

///////////////////////////////////////////

#include "NxOgreOgreSceneRenderer.h"
#include "NxOgreOgreNodeRenderable.h"

#endif
