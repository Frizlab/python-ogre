/*  Master header file for the Ogre part of Python-Ogre
    
Nov 21 2007:  Major change to use Ogre.h and OgreStableHeaders.h, and then add missing files as needed.

*/
/*
include ogre.h but sometime we need to tweak defines etc to get gccxml to work
and as this is needed by many modules we have moves the 'tweaking' to a seperate
include file that everyone can use.
*/
#include "./python_ogre_include_OGRE.h"
#include "OgreStableHeaders.h"

#include "OgreAlignedAllocator.h"
#include "OgreAnimable.h"
#include "OgreArchiveFactory.h"
#include "OgreAutoParamDataSource.h"
#include "OgreBillboard.h"
#include "OgreBillboardParticleRenderer.h"
#include "OgreBlendMode.h"
#include "OgreBorderPanelOverlayElement.h"
// #include "OgreCodec.h"  //  this one causes a Py++ exception

#if (OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR < 7) // these no longer in 1.7
    #include "OgreCompiler2Pass.h"
    #include "OgreCompositorScriptCompiler.h"
    #include "OgreCompositorSerializer.h"
    #include "OgreCompositorScriptCompiler.h"
#endif
#include "OgreConfig.h"
#ifndef __APPLE_CC__  // need to exlude under MacOS X
    #include "OgreConfigDialog.h"
#endif
#include "OgreConfigOptionMap.h"
#include "OgreController.h"
#include "OgreConvexBody.h"
// #include "OgreDDSCodec.h" /// not this one
#include "OgreDefaultHardwareBufferManager.h"
#include "OgreDynLib.h"
#include "OgreDynLibManager.h"
#include "OgreEdgeListBuilder.h"
#include "OgreErrorDialog.h"
#include "OgreExternalTextureSource.h"
#include "OgreExternalTextureSourceManager.h"
#include "OgreFactoryObj.h"
#include "OgreFont.h"
#include "OgreFontManager.h"
#include "OgreFrameListener.h"
// #include "OgreFreeImageCodec.h" // not this one
#include "OgreGpuProgramUsage.h"
#include "OgreHardwareBuffer.h"
#include "OgreImage.h"
// #include "OgreImageCodec.h"  // not this one
#include "OgreIteratorWrappers.h"

#if OGRE_VERSION_MINOR == 4
    #include "OgreMaterialScriptCompiler.h"
    #include "OgreMemoryMacros.h"
    #include "OgreMemoryManager.h"
    #include "OgreNoMemoryMacros.h"
#endif    



#if OGRE_VERSION_MINOR >= 7
    #include "OgreCustomCompositionPass.h"
    #include "OgreCompositorLogic.h"
    #include "OgreUserObjectBindings.h"
#endif


#include "OgreMeshFileFormat.h"
#include "OgreMeshSerializerImpl.h"
#include "OgreMovablePlane.h"
#include "OgreNumerics.h"
#include "OgreOverlayElementCommands.h"
#include "OgreOverlayElementFactory.h"
#include "OgrePanelOverlayElement.h"
#include "OgreParticle.h"
#include "OgreParticleAffectorFactory.h"
#include "OgreParticleEmitterCommands.h"
#include "OgreParticleEmitterFactory.h"
#include "OgreParticleIterator.h"
#include "OgreParticleSystem.h"
#include "OgreParticleSystemRenderer.h"
#include "OgrePixelFormat.h"
#include "OgrePlaneBoundedVolume.h"

#include "OgrePlugin.h"
#include "OgrePolygon.h"
#include "OgrePose.h"
#include "OgrePredefinedControllers.h"
#include "OgrePrefabFactory.h"
#include "OgreProgressiveMesh.h"
#include "OgreRay.h"
#include "OgreRectangle.h"
#include "OgreRectangle2D.h"
#include "OgreRenderable.h"
#include "OgreRenderOperation.h"
#include "OgreRenderQueue.h"
#include "OgreRenderQueueSortingGrouping.h"
#include "OgreRenderSystemCapabilities.h"
#include "OgreRenderTarget.h"
#include "OgreResourceManager.h"
#include "OgreRotationalSpline.h"
#include "OgreSceneQuery.h"
#include "OgreScriptLoader.h"
// #include "OgreSearchOps.h" // not this one
#include "OgreShadowCaster.h"
#include "OgreShadowTextureManager.h"
#include "OgreShadowVolumeExtrudeProgram.h"
#include "OgreSkeletonFileFormat.h"
// #include "OgreSpotShadowFadePng.h"  // Gets defined in every obj file so has link errors, needs to be defined as static
#include "OgreStaticFaceGroup.h"
#include "OgreTagPoint.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgreTexture.h"
#include "OgreTimer.h"
#include "OgreUnifiedHighLevelGpuProgram.h"
#include "OgreVertexBoneAssignment.h"
#include "OgreVertexIndexData.h"

// for 1.6.1 -- going crasy on the includes
//#include "OgreCompositorScriptCompiler2.h"
//#include "OgreParticleScriptCompiler.h"
#include "OgreScriptCompiler.h"
#include "OgreScriptLexer.h"
#include "OgreScriptLoader.h"
#include "OgreScriptParser.h"
#include "OgreScriptTranslator.h"
#include "OgreMaterial.h"
#include "OgreString.h"
#include "OgreMath.h"
// New for 1.7
#if (OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 7) || (OGRE_VERSION_MAJOR >1 )
// SDK    #include "Property/OgreProperty.h"
     #include "OgreProperty.h"  // SOURCE
#endif
