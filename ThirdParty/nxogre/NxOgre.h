/** \file    NxOgreConfig.h
 *  \brief   Configuration file of the NxOgre library, using macros to
 *           switch on and off features of NxOgre.
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

#ifndef I_HEART_NXOGRE
#define I_HEART_NXOGRE

///////////////////////////////////////////

#include "NxOgreStable.h"
#include "NxOgrePlatform.h"
#include "NxOgrePrerequisites.h"
#include "NxOgrePhysXDriver.h"
#include "BetajaenCC.h"
#include "FireState.h"
#include "NxOgreExtendedTypes.h"

/////////////////////////////////////////////

#include "NxOgreActor.h"
#include "NxOgreActors.h"
#include "NxOgreAnimatedRenderable.h"
#include "NxOgreBody.h"
#include "NxOgreCharacter.h"
#include "NxOgreCharacterController.h"
#include "NxOgreCharacterModel.h"
#include "NxOgreCharacterMovementModel.h"
#include "NxOgreCharacterSystem.h"
#include "NxOgreCloth.h"
#include "NxOgreClothRaycaster.h"
#include "NxOgreCollisionModel.h"
#include "NxOgreCompound.h"
#include "NxOgreCompoundActor.h"
#include "NxOgreConfig.h"
#include "NxOgreContactCallback.h"
#include "NxOgreContactCallbackController.h"
#include "NxOgreContactStream.h"
#include "NxOgreContainer.h"
#include "NxOgreCooking.h"
#include "NxOgreDebugRenderer.h"
#include "NxOgreDominanceGroup.h"
#include "NxOgreDualIdentifier.h"
#include "NxOgreDynamicMesh.h"
#include "NxOgreEffectsSystem.h"
#include "NxOgreEngine.h"
#include "NxOgreEnumsDefinesTypedefs.h"
#include "NxOgreError.h"
#include "NxOgreFileResourceStream.h"
#include "NxOgreFileResourceSystem.h"
#include "NxOgreFluid.h"
#include "NxOgreFluidMesh.h"
#include "NxOgreForceField.h"
#include "NxOgreGroup.h"
#include "NxOgreHash.h"
#include "NxOgreHelpers.h"
#include "NxOgreInflatable.h"
#include "NxOgreIntersection.h"
#include "NxOgreJoint.h"
#include "NxOgreJointCallback.h"
#include "NxOgreJointSet1.h"
#include "NxOgreJointSet2.h"
#include "NxOgreLegacyCharacter.h"
#include "NxOgreLegacyCharacterController.h"
#include "NxOgreLegacyCharacterHitReport.h"
#include "NxOgreLog.h"
#include "NxOgreMachine.h"
#include "NxOgreMaterial.h"
#include "NxOgreMemoryStream.h"
#include "NxOgreMeshRenderable.h"
#include "NxOgreNodeRenderable.h"
#include "NxOgreNxActorController.h"
#include "NxOgreNxCharacterController.h"
#include "NxOgreNXM.h"
#include "NxOgreNxSerialiser.h"
#include "NxOgreNxuStreamSerialiser.h"
#include "NxOgreOBJStream.h"
#include "NxOgreParams.h"
#include "NxOgreParticleSystem.h"
#include "NxOgrePerformer.h"
#include "NxOgrePose.h"
#include "NxOgreRaycaster.h"
#include "NxOgreRemoteDebuggerConnection.h"
#include "NxOgreRenderable.h"
#include "NxOgreRenderableSource.h"
#include "NxOgreResourceManager.h"
#include "NxOgreResourceMesh.h"
#include "NxOgreResourceStream.h"
#include "NxOgreResourceStreamPtr.h"
#include "NxOgreResourceSystem.h"
#include "NxOgreScene.h"
#include "NxOgreSceneContactController.h"
#include "NxOgreSceneController.h"
#include "NxOgreSceneRenderer.h"
#include "NxOgreSceneSource.h"
#include "NxOgreSceneTriggerController.h"
#include "NxOgreSerialiser.h"
#include "NxOgreShape.h"
#include "NxOgreShapeConvex.h"
#include "NxOgreShapeMesh.h"
#include "NxOgreShapePrimitives.h"
#include "NxOgreShapeTerrain.h"
#include "NxOgreShapeWheel.h"
#include "NxOgreSheet.h"
#include "NxOgreSimpleActor.h"
#include "NxOgreSimpleShape.h"
#include "NxOgreSkeletalRenderable.h"
#include "NxOgreSkeleton.h"
#include "NxOgreSleepCallback.h"
#include "NxOgreSoftBody.h"
#include "NxOgreSpriteRenderable.h"
#include "NxOgreState.h"
#include "NxOgreStateMachine.h"
#include "NxOgreTestIdea.h"
#include "NxOgreTesting.h"
#include "NxOgreTimeController.h"
#include "NxOgreTransitionalMesh.h"
#include "NxOgreTrigger.h"
#include "NxOgreTriggerCallback.h"
#include "NxOgreUserAllocator.h"
#include "NxOgreUserData.h"
#include "NxOgreVoxelRenderable.h"
#include "NxOgreWheelSet.h"
#include "NxOgreWorld.h"

// Ogre bits.
/////////	Disabled -> #include "NxOgreOgreMesh.h"
#include "NxOgreOgreMeshRenderable.h"
#include "NxOgreOgreNodeRenderable.h"
#include "NxOgreOgreResourceStream.h"
#include "NxOgreOgreResourceSystem.h"
#include "NxOgreOgreSceneRenderer.h"
#include "NxOgreOgreTimeController.h"

/** \mainpage NxOgre
\section licence Licence

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

#endif
