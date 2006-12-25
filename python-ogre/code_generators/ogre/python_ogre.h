#ifndef __python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
#define __python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__

//See best practices section in Py++ documentation
#include "Ogre.h"
#include "OgreParticleAffectorFactory.h"

#include "OgreBillboardParticleRenderer.h"
#include "OgreParticleSystemRenderer.h"
#include "OgreParticleEmitterFactory.h"
#include "OgreArchiveFactory.h"

#include "OgreErrorDialog.h"
#include "OgreConfigDialog.h"
#include "OgreTagPoint.h"
#include "OgreTargetManager.h"
#include "OgreOverlayElementFactory.h"
#include "OgreArchiveFactory.h"
#include "OgreParticleSystemRenderer.h"
#include "OgreParticleEmitterFactory.h"
#include "OgreParticleAffectorFactory.h"
#include "OgreSkeletonInstance.h"
#include "OgreSkeletonManager.h"
#include "OgreSkeleton.h"
#include "OgreCompositor.h"



#ifndef OGRE_VERSION_CVS        // these are needed for the 1.2.x SDK
    #include "OgreEventListeners.h"
    #include "OgreEventQueue.h"
    #include "OgreKeyEvent.h"
    #include "OgreMouseEvent.h"

namespace Ogre {
class CombinedListener : public FrameListener, public KeyListener,
                 public MouseListener, public MouseMotionListener
{
public:
    CombinedListener() {}
    virtual ~CombinedListener() {}
    
    // KeyListener methods
    virtual void keyClicked(KeyEvent *e) {}
    virtual void keyPressed(KeyEvent *e) {}
    virtual void keyReleased(KeyEvent *e) {}
    
    // MouseListener methods
    virtual void mouseClicked(MouseEvent *e) {}
    virtual void mousePressed(MouseEvent *e) {}
    virtual void mouseReleased(MouseEvent *e) {}
    virtual void mouseEntered(MouseEvent *e) {}
    virtual void mouseExited(MouseEvent *) {}
    
    // MouseMotionListener methods
    virtual void mouseMoved(MouseEvent *e) {}
    virtual void mouseDragged(MouseEvent *e) {}
    
    // FrameEvent methods
    virtual bool frameStarted (const FrameEvent &evt) {return true;}
    virtual bool frameEnded (const FrameEvent &evt) {return true;}
};

}
#endif

namespace python_ogre{ namespace details{

inline void instantiate(){
    using namespace Ogre;
    
    sizeof (AlignedAllocator<Vector4, 0>  );
    sizeof (AnimableValue );
    sizeof (AnimableValuePtr );
    sizeof (ConstMapIterator<std::map<std::string, std::string > > );
    sizeof (Controller<float> );
    sizeof (MapIterator<std::map<short unsigned int, NodeAnimationTrack*> >);
    sizeof (MapIterator<std::map<short unsigned int, NumericAnimationTrack*> >);
    sizeof (MapIterator<std::map<std::string, MovableObjectFactory*> >);
    sizeof (MapIterator<std::map<std::string, Overlay*> >);
    sizeof (MapIterator<std::map<std::string, ParticleAffectorFactory*> >);
    sizeof (MapIterator<std::map<std::string, std::string > >);
    sizeof (MapIterator<std::map<std::string, std::multimap<std::string, std::string >* > >);
    sizeof (MapIterator<std::map<std::string, MovableObject*> >);
    sizeof (MapIterator<std::map<std::string, OverlayContainer*> >);
    sizeof (MapIterator<std::map<std::string, OverlayContainer*> >);
    sizeof (MapIterator<std::map<std::string, std::multimap<std::string, std::string> > >);
    sizeof (MapIterator<std::map<unsigned char, RenderQueueGroup*> >);
    sizeof (MapIterator<std::map<unsigned short, NodeAnimationTrack*> >);
    sizeof (MapIterator<std::map<unsigned short, NumericAnimationTrack*> >);
    sizeof (MapIterator<std::map<unsigned short, VertexAnimationTrack*> >);
    sizeof (MapIterator<std::multimap<std::string, std::string > >);
    sizeof (MapIterator<stdext::hash_map<std::string, MovableObject* > >);
    sizeof (MapIterator<stdext::hash_map<std::string, Node* > >); 
        
    // sizeof (ArchiveFactory );  // AJM this doesn't expose well :)
    sizeof (CompositorInstance::RenderSystemOperation );
    sizeof (ConstMapIterator<std::map<std::string, AnimationState*> >);
    sizeof (ConstMapIterator<std::map<std::string, MovableObjectFactory*> >);
    sizeof (ConstMapIterator<stdext::hash_map<std::string, MovableObject*> >);
    sizeof (ConstMapIterator<std::map<unsigned int, Vector3> >);
    sizeof (ConstVectorIterator<std::list<AnimationState*> >);
    sizeof (ConstVectorIterator<std::vector<const SceneManagerMetaData*> >);
    sizeof (ConstVectorIterator<std::vector<float> >);
    sizeof (ConstVectorIterator<std::vector<GpuProgramParameters::AutoConstantEntry> >);
    sizeof (ConstVectorIterator<std::vector<GpuProgramParameters::IntConstantEntry> >);
    sizeof (ConstVectorIterator<std::vector<GpuProgramParameters::RealConstantEntry> >);
    sizeof (ConstVectorIterator<std::vector<LinkedSkeletonAnimationSource> >);
    sizeof (ConstVectorIterator<std::vector<Node*> >);
    sizeof (ConstVectorIterator<std::vector<Pose*> >);
    sizeof (ConstVectorIterator<std::vector<ShadowTextureConfig> >);
    sizeof (ConstVectorIterator<std::vector<TextureUnitState*> >);
    sizeof (ConstVectorIterator<std::vector<VertexPoseKeyFrame::PoseRef> >);
    sizeof (DataStream );
    sizeof (DataStreamPtr );
    sizeof (FileInfo );
    sizeof (GpuProgramParameters::AutoConstantEntry );
    sizeof (HardwareIndexBuffer );
    sizeof (HardwareIndexBufferSharedPtr );  
    sizeof (HardwarePixelBufferSharedPtr );
    //???sizeof (MapIterator<std::map<class std::basic_string<char,struct std::char_traits<char>,class  );   
    sizeof (MapIterator<std::map<std::string, OverlayContainer*> >);
    sizeof (MapIterator<std::map<std::string, ParticleEmitterFactory*> >);
    sizeof (MapIterator<std::map<std::string, ParticleSystemRendererFactory*> >);
    sizeof (MapIterator<std::map<std::string, SceneManager*> >);     
    sizeof (MapIterator<std::map<std::string, Animation*> >);
    sizeof (MapIterator<std::map<std::string, Camera*> >);
    sizeof (MapIterator<std::map<std::string, MovableObject*> >);
    sizeof (MapIterator<std::map<std::string, OverlayElement*> >);
    sizeof (MapIterator<std::map<std::string, ParticleSystem*> >);
    sizeof (MapIterator<std::map<std::string, RenderTarget*> >);
    sizeof (MapIterator<std::map<std::string, ResourceManager*> >);
    sizeof (MapIterator<std::map<std::string, std::multimap<std::string, std::string> > >);
    sizeof (MapIterator<std::map<unsigned char, RenderQueueGroup*> >);
    sizeof (MapIterator<std::map<unsigned int, StaticGeometry::Region*> >);
    sizeof (MapIterator<std::map<unsigned int, Vector3> >);
    sizeof (MapIterator<std::map<unsigned long, SharedPtr<Resource> > >);
    sizeof (MapIterator<std::map<unsigned short, RenderPriorityGroup*> >);
    sizeof (MapIterator<std::multimap<std::string, std::string> >);
    sizeof (MapIterator<std::multimap<unsigned int, VertexBoneAssignment_s> >);
    sizeof (MapIterator<std::multimap<unsigned, VertexBoneAssignment_s> >);
    sizeof (MapIterator<stdext::hash_map<std::string, MovableObject*> >);
    sizeof (MapIterator<stdext::hash_map<std::string, MovableObject*> >);
    sizeof (MapIterator<std::multimap<unsigned, VertexBoneAssignment_s> >);
    sizeof (MapIterator<std::map<std::string, Ogre::StaticGeometry::MaterialBucket* > >);
    sizeof (MaterialPtr );
    sizeof (MaterialPtr );
    sizeof (MeshPtr );
    
    sizeof (OverlayElementFactory );
    sizeof (ParticleAffectorFactory );
    sizeof (ParticleEmitterFactory );
    sizeof (ParticleSystemRendererFactory );
    sizeof (PatchMeshPtr );
//    sizeof (PositionTarget  );
    sizeof (SharedPtr<ControllerValue<float> > );
    sizeof (SharedPtr<DataStream> );
    sizeof (SharedPtr<std::vector<std::string> >);
    sizeof (SharedPtr<std::vector<std::string> >);
    sizeof (SharedPtr<std::vector<std::string> >);
    sizeof (SharedPtr<std::vector<FileInfo> >);
    sizeof (SharedPtr<std::list<SharedPtr<DataStream> > >);
    sizeof (Singleton<ArchiveManager> );
    sizeof (StringVector );
    sizeof (StringVectorPtr );
    sizeof (TexturePtr );
    sizeof (VectorIterator<std::vector<StaticGeometry::LODBucket *> >);
    sizeof (VectorIterator<std::vector<Bone*> >);
    sizeof (VectorIterator<std::vector<CompositionPass*> >);
    sizeof (VectorIterator<std::vector<CompositionTechnique*> >);
    sizeof (VectorIterator<std::vector<CompositionTechnique::TextureDefinition*> >);
    sizeof (VectorIterator<std::vector<Pose*> >);
    sizeof (VectorIterator<std::vector<ShadowRenderable*> >);
    sizeof (VectorIterator<std::vector<StaticGeometry::GeometryBucket*> >);
    sizeof (VectorIterator<std::vector<Technique*> >);
    sizeof (VectorIterator<std::vector<VertexPoseKeyFrame::PoseRef> >);
    sizeof (VertexBoneAssignment_s );   
    sizeof (RadixSort<std::list<Billboard*>, Ogre::Billboard*, float >);
    sizeof (RadixSort<std::list<Particle*>, Ogre::Particle*, float >);
    sizeof (RadixSort<std::vector<RenderablePass>, Ogre::RenderablePass, float >);
    sizeof (RadixSort<std::vector<RenderablePass>, Ogre::RenderablePass, unsigned int >);
    sizeof (SharedPtr< Controller<float> >);
    sizeof (SharedPtr< std::vector<String> > );
    sizeof (SharedPtr< WaveformControllerFunction > );
    sizeof (SharedPtr<AnimableValue>  );
    sizeof (SharedPtr<ControllerFunctionRealPtr> );
    sizeof (SharedPtr<ControllerValueRealPtr> );
    sizeof (SharedPtr<DataStream> );
    sizeof (SharedPtr<HardwareIndexBuffer> );
    sizeof (SharedPtr<MemoryDataStream>);
    sizeof (SharedPtr<Resource> );
    sizeof (SharedPtr<StringVector> );
    sizeof (SharedPtr<StringVectorPtr> );
    sizeof (SharedPtr<std::vector<std::string> >);
    sizeof (Singleton<ArchiveManager>);
    sizeof (Singleton<CompositorManager>);
    sizeof (Singleton<ControllerManager>);
    sizeof (Singleton<GpuProgramManager>);
    sizeof (Singleton<HardwareBufferManager>);
    sizeof (Singleton<HighLevelGpuProgramManager>);
    sizeof (Singleton<LogManager>);
    sizeof (Singleton<MaterialManager>);
    sizeof (Singleton<MeshManager>);
    sizeof (Singleton<OverlayManager>);
    sizeof (Singleton<ParticleSystemManager>);
    sizeof (Singleton<Profiler>);
    sizeof (Singleton<ResourceBackgroundQueue>);
    sizeof (Singleton<ResourceGroupManager>);
    sizeof (Singleton<Root>);
    sizeof (Singleton<SceneManagerEnumerator>);
    sizeof (Singleton<ShadowTextureManager>);
    sizeof (Singleton<SkeletonManager>);
    sizeof (Singleton<TextureManager>);
    sizeof (std::list<AnimationState*>);
    sizeof (std::list<AnimationState*>::iterator);
    sizeof (std::list<AnimationState*>::const_iterator );
    sizeof (std::map<std::string, AnimationState*>);
    sizeof (std::map<std::string, std::string > );
    sizeof (std::map<std::string, MovableObject*> );
    sizeof (std::map<String, String> );    // NameValuePairList
    sizeof (std::map<unsigned int, StaticGeometry::Region *> );
    sizeof (std::map<unsigned, Vector3>);
    sizeof (std::multimap<RenderWindow*,WindowEventListener*>);
    sizeof (std::multimap<Vector3,Vector3>);
    sizeof (std::multimap<RenderWindow*, WindowEventListener*> ) ; // WindowEventListeners;
    sizeof (std::multimap<std::string,std::string>);
    sizeof (std::pair< std::wstring, std::wstring > );
    sizeof (std::pair<bool, float> );
    sizeof (std::pair<bool, float> );
    sizeof (std::pair<unsigned int,unsigned int> );
    sizeof (std::pair<unsigned, unsigned> );
    
    sizeof (std::set<Entity*>);
    sizeof (std::vector<LinkedSkeletonAnimationSource>::const_iterator );
    sizeof (std::vector<RenderWindow*>);
    sizeof (std::vector<ShadowRenderable*>::iterator );
    sizeof (std::vector<ShadowTextureConfig>::const_iterator );
    sizeof (std::vector<Vector4>);
    sizeof (std::vector<std::string>);
    sizeof (std::vector<String> );
    sizeof (stdext::hash_map<std::string, unsigned short>);
    sizeof (TRect<long int>);
    sizeof (VectorIterator<std::vector<Bone*> >);
    sizeof (VectorIterator<std::vector<CompositionTargetPass*> >);
    sizeof (VectorIterator<std::vector<CompositorInstance*> >);
    sizeof (VectorIterator<std::vector<IlluminationPass*> >);
    sizeof (VectorIterator<std::vector<Pass*> >);
    sizeof (VectorIterator<std::vector<RenderQueueInvocation*> >);
    sizeof (VectorIterator<std::vector<Technique*> >);
    sizeof (VectorIterator<std::vector<TextureUnitState*> >);
    sizeof (VectorIterator<std::vector<VertexPoseKeyFrame::PoseRef> >);
     
    sizeof (std::pair<std::list<std::pair<const std::string, Ogre::MovableObject*> >::iterator,std::list<std::pair<const std::string, Ogre::MovableObject*> >::iterator>);
    sizeof (std::pair<std::list<std::pair<const std::string, Ogre::MovableObject*> >::const_iterator,std::list<std::pair<const std::string, Ogre::MovableObject*> >::const_iterator>);
    
    sizeof (std::pair<const std::string,Ogre::MovableObject*>);
    sizeof (std::pair<std::list<std::pair<const std::string, Ogre::MovableObject*> >::iterator,bool>);

}

} } //python_ogre::details


#endif//__python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
