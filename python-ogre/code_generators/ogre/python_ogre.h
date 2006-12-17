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
class CombinedListener : public Ogre::FrameListener, public Ogre::KeyListener,
                 public Ogre::MouseListener, public Ogre::MouseMotionListener
{
public:
    CombinedListener() {}
    virtual ~CombinedListener() {}
    
    // KeyListener methods
    virtual void keyClicked(Ogre::KeyEvent *e) {}
    virtual void keyPressed(Ogre::KeyEvent *e) {}
    virtual void keyReleased(Ogre::KeyEvent *e) {}
    
    // MouseListener methods
    virtual void mouseClicked(Ogre::MouseEvent *e) {}
    virtual void mousePressed(Ogre::MouseEvent *e) {}
    virtual void mouseReleased(Ogre::MouseEvent *e) {}
    virtual void mouseEntered(Ogre::MouseEvent *e) {}
    virtual void mouseExited(Ogre::MouseEvent *) {}
    
    // MouseMotionListener methods
    virtual void mouseMoved(Ogre::MouseEvent *e) {}
    virtual void mouseDragged(Ogre::MouseEvent *e) {}
    
    // FrameEvent methods
    virtual bool frameStarted (const FrameEvent &evt) {return true;}
    virtual bool frameEnded (const FrameEvent &evt) {return true;}
};

}
#endif

namespace python_ogre{ namespace details{

inline void instantiate(){
    using namespace Ogre;
    
    


    
    
    
    
    
    
    
    
    sizeof (AlignedAllocator<Ogre::Vector4, 0>  );
    sizeof (AnimableValue );
    sizeof (AnimableValuePtr );
    sizeof (ConstMapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > > );
    sizeof (Controller<float> );
    sizeof (MapIterator<std::map<short unsigned int, Ogre::NodeAnimationTrack*, std::less<short unsigned int>, std::allocator<std::pair<short unsigned int const, Ogre::NodeAnimationTrack*> > > >);
    sizeof (MapIterator<std::map<short unsigned int, Ogre::NumericAnimationTrack*, std::less<short unsigned int>, std::allocator<std::pair<short unsigned int const, Ogre::NumericAnimationTrack*> > > >);
    sizeof (MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObjectFactory*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObjectFactory*> > > >);
    sizeof (MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::Overlay*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::Overlay*> > > >);
    sizeof (MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::ParticleAffectorFactory*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::ParticleAffectorFactory*> > > >);
    sizeof (MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > >);
    sizeof (MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >*> > > > );
    sizeof (MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >*> > > >);
    sizeof (MapIterator<std::map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > >);
    sizeof (MapIterator<std::map<std::string, Ogre::OverlayContainer*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::OverlayContainer*> > > >);
    sizeof (MapIterator<std::map<std::string, Ogre::OverlayContainer*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayContainer*> > > >);
    sizeof (MapIterator<std::map<std::string, std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >*, std::less<std::string>, std::allocator<std::pair<std::string const, std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >*> > > > );
    sizeof (MapIterator<std::map<unsigned char, Ogre::RenderQueueGroup*, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, Ogre::RenderQueueGroup*> > > >);
    sizeof (MapIterator<std::map<unsigned short, Ogre::NodeAnimationTrack*, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, Ogre::NodeAnimationTrack*> > > >);
    sizeof (MapIterator<std::map<unsigned short, Ogre::NumericAnimationTrack*, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, Ogre::NumericAnimationTrack*> > > >);
    sizeof (MapIterator<std::map<unsigned short, Ogre::VertexAnimationTrack*, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, Ogre::VertexAnimationTrack*> > > >);
    sizeof (MapIterator<std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > >);
    sizeof (MapIterator<stdext::hash_map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*, stdext::hash_compare<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*> > > >);
    sizeof (MapIterator<stdext::hash_map<std::string, Ogre::Node*, stdext::hash_compare<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::Node*> > > >);

    
        
    // sizeof (Ogre::ArchiveFactory );  // AJM this doesn't expose well :)
    sizeof (Ogre::CompositorInstance::RenderSystemOperation );
    sizeof (Ogre::ConstMapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::AnimationState*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::AnimationState*> > > > );
    sizeof (Ogre::ConstMapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObjectFactory*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObjectFactory*> > > > );
    sizeof (Ogre::ConstMapIterator<stdext::hash_map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*, stdext::hash_compare<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*> > > > );
    sizeof (Ogre::ConstMapIterator<stdext::hash_map<std::string, Ogre::MovableObject*, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > > );
    sizeof (Ogre::ConstMapIterator<stdext::hash_map<std::string, Ogre::MovableObject*, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > >  );
    sizeof (Ogre::ConstMapIterator<std::map<unsigned int, Ogre::Vector3, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, Ogre::Vector3> > > >  );
    sizeof (Ogre::ConstVectorIterator<std::list<Ogre::AnimationState*, std::allocator<Ogre::AnimationState*> > > );
    sizeof (Ogre::ConstVectorIterator<std::vector<const Ogre::SceneManagerMetaData*, std::allocator<const Ogre::SceneManagerMetaData*> > > );
    sizeof (Ogre::ConstVectorIterator<std::vector<float, std::allocator<float> > > );
    sizeof (Ogre::ConstVectorIterator<std::vector<Ogre::GpuProgramParameters::AutoConstantEntry, std::allocator<Ogre::GpuProgramParameters::AutoConstantEntry> > > );
    sizeof (Ogre::ConstVectorIterator<std::vector<Ogre::GpuProgramParameters::IntConstantEntry, std::allocator<Ogre::GpuProgramParameters::IntConstantEntry> > >  );
    sizeof (Ogre::ConstVectorIterator<std::vector<Ogre::GpuProgramParameters::RealConstantEntry, std::allocator<Ogre::GpuProgramParameters::RealConstantEntry> > >);
    sizeof (Ogre::ConstVectorIterator<std::vector<Ogre::LinkedSkeletonAnimationSource, std::allocator<Ogre::LinkedSkeletonAnimationSource> > > );
    sizeof (Ogre::ConstVectorIterator<std::vector<Ogre::Node*, std::allocator<Ogre::Node*> > > );
    sizeof (Ogre::ConstVectorIterator<std::vector<Ogre::Pose*, std::allocator<Ogre::Pose*> > > );
    sizeof (Ogre::ConstVectorIterator<std::vector<Ogre::ShadowTextureConfig, std::allocator<Ogre::ShadowTextureConfig> > > );
    sizeof (Ogre::ConstVectorIterator<std::vector<Ogre::TextureUnitState*, std::allocator<Ogre::TextureUnitState*> > > );
    sizeof (Ogre::ConstVectorIterator<std::vector<Ogre::VertexPoseKeyFrame::PoseRef, std::allocator<Ogre::VertexPoseKeyFrame::PoseRef> > >  );
    sizeof (Ogre::DataStream );
    sizeof (Ogre::DataStreamPtr );
    sizeof (Ogre::FileInfo );
    sizeof (Ogre::GpuProgramParameters::AutoConstantEntry );
    sizeof (Ogre::HardwareIndexBuffer );
    sizeof (Ogre::HardwareIndexBufferSharedPtr );  
    sizeof (Ogre::HardwarePixelBufferSharedPtr );
    sizeof (Ogre::MapIterator<class std::map<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >,class Ogre::StaticGeometry::MaterialBucket *,struct std::less<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > >,class std::allocator<struct std::pair<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const ,class Ogre::StaticGeometry::MaterialBucket *> > > > );   
    sizeof (Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::OverlayContainer*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::OverlayContainer*> > > > );
    sizeof (Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::ParticleEmitterFactory*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::ParticleEmitterFactory*> > > > );
    sizeof (Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::ParticleSystemRendererFactory*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::ParticleSystemRendererFactory*> > > > );
    sizeof (Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::SceneManager*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::SceneManager*> > > > );     
    sizeof (Ogre::MapIterator<std::map<std::string, Ogre::Animation*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::Animation*> > > >);
    sizeof (Ogre::MapIterator<std::map<std::string, Ogre::Camera*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::Camera*> > > >);
    sizeof (Ogre::MapIterator<std::map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > > );
    sizeof (Ogre::MapIterator<std::map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > > >);
    sizeof (Ogre::MapIterator<std::map<std::string, Ogre::ParticleSystem*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::ParticleSystem*> > > >);
    sizeof (Ogre::MapIterator<std::map<std::string, Ogre::RenderTarget*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::RenderTarget*> > > >);
    sizeof (Ogre::MapIterator<std::map<std::string, Ogre::ResourceManager*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::ResourceManager*> > > >);
    sizeof (Ogre::MapIterator<std::map<std::string, std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >*, std::less<std::string>, std::allocator<std::pair<std::string const, std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >*> > > > );
    sizeof (Ogre::MapIterator<std::map<unsigned char, Ogre::RenderQueueGroup*, std::less<unsigned char>, std::allocator<std::pair<const unsigned char, Ogre::RenderQueueGroup*> > > > );
    sizeof (Ogre::MapIterator<std::map<unsigned int, Ogre::StaticGeometry::Region*, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, Ogre::StaticGeometry::Region*> > > > ) ;
    sizeof (Ogre::MapIterator<std::map<unsigned int, Ogre::Vector3, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, Ogre::Vector3> > > > );
    sizeof (Ogre::MapIterator<std::map<unsigned long, Ogre::SharedPtr<Ogre::Resource>, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, Ogre::SharedPtr<Ogre::Resource> > > > >);
    sizeof (Ogre::MapIterator<std::map<unsigned short, Ogre::RenderPriorityGroup*, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, Ogre::RenderPriorityGroup*> > > >);
    sizeof (Ogre::MapIterator<std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > > > );
    sizeof (Ogre::MapIterator<std::multimap<unsigned int, Ogre::VertexBoneAssignment_s, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, Ogre::VertexBoneAssignment_s> > > > );
    sizeof (Ogre::MapIterator<std::multimap<unsigned, Ogre::VertexBoneAssignment_s, std::less<unsigned>, std::allocator<std::pair<unsigned const, Ogre::VertexBoneAssignment_s> > > > );
    sizeof (Ogre::MapIterator<stdext::hash_map<std::string, Ogre::MovableObject*, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > > );
    sizeof (Ogre::MapIterator<stdext::hash_map<std::string, Ogre::MovableObject*, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > >  );
    sizeof (Ogre::MapIterator<std::multimap<unsigned, Ogre::VertexBoneAssignment_s, std::less<unsigned>, std::allocator<std::pair<unsigned const, Ogre::VertexBoneAssignment_s> > > >  );
    sizeof (Ogre::MaterialPtr );
    sizeof (Ogre::MaterialPtr );
    sizeof (Ogre::MeshPtr );
    
    sizeof (Ogre::OverlayElementFactory );
    sizeof (Ogre::ParticleAffectorFactory );
    sizeof (Ogre::ParticleEmitterFactory );
    sizeof (Ogre::ParticleSystemRendererFactory );
    sizeof (Ogre::PatchMeshPtr );
//    sizeof (Ogre::PositionTarget  );
    sizeof (Ogre::SharedPtr<Ogre::ControllerValue<float> > );
    sizeof (Ogre::SharedPtr<Ogre::DataStream> );
    sizeof (Ogre::SharedPtr<std::vector<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >  );
    sizeof (Ogre::SharedPtr<std::vector<std::string, std::allocator<std::string> > > );
    sizeof (Ogre::SharedPtr<std::vector<std::string, std::allocator<std::string> > > );
    sizeof (Ogre::SharedPtr<std::vector<Ogre::FileInfo, std::allocator<Ogre::FileInfo> > >  );
    sizeof (Ogre::SharedPtr<std::list<Ogre::SharedPtr<Ogre::DataStream>, std::allocator<Ogre::SharedPtr<Ogre::DataStream> > > >  );
    sizeof (Ogre::Singleton<Ogre::ArchiveManager> );
    sizeof (Ogre::StringVector );
    sizeof (Ogre::StringVectorPtr );
    sizeof (Ogre::TexturePtr );
    sizeof (Ogre::VectorIterator<class std::vector<class Ogre::StaticGeometry::LODBucket *,class std::allocator<class Ogre::StaticGeometry::LODBucket *> > > );
    sizeof (Ogre::VectorIterator<std::vector<Ogre::Bone*, std::allocator<Ogre::Bone*> > > );
    sizeof (Ogre::VectorIterator<std::vector<Ogre::CompositionPass*, std::allocator<Ogre::CompositionPass*> > > );
    sizeof (Ogre::VectorIterator<std::vector<Ogre::CompositionTechnique*, std::allocator<Ogre::CompositionTechnique*> > > );
    sizeof (Ogre::VectorIterator<std::vector<Ogre::CompositionTechnique::TextureDefinition*, std::allocator<Ogre::CompositionTechnique::TextureDefinition*> > > );
    sizeof (Ogre::VectorIterator<std::vector<Ogre::Pose*, std::allocator<Ogre::Pose*> > > );
    sizeof (Ogre::VectorIterator<std::vector<Ogre::ShadowRenderable*, std::allocator<Ogre::ShadowRenderable*> > > );
    sizeof (Ogre::VectorIterator<std::vector<Ogre::StaticGeometry::GeometryBucket*, std::allocator<Ogre::StaticGeometry::GeometryBucket*> > > );
    sizeof (Ogre::VectorIterator<std::vector<Ogre::Technique*, std::allocator<Ogre::Technique*> > > );
    sizeof (Ogre::VectorIterator<std::vector<Ogre::VertexPoseKeyFrame::PoseRef, std::allocator<Ogre::VertexPoseKeyFrame::PoseRef> > > );
    sizeof (Ogre::VertexBoneAssignment_s );

    
    

    
        sizeof (RadixSort<std::list<Ogre::Billboard*, std::allocator<Ogre::Billboard*> >,Ogre::Billboard*,float>);
    sizeof (RadixSort<std::list<Ogre::Particle*, std::allocator<Ogre::Particle*> >,Ogre::Particle*,float>);
    sizeof (RadixSort<std::vector<Ogre::RenderablePass, std::allocator<Ogre::RenderablePass> >,Ogre::RenderablePass,float> );
    sizeof (RadixSort<std::vector<Ogre::RenderablePass, std::allocator<Ogre::RenderablePass> >,Ogre::RenderablePass,unsigned int> );
    sizeof (SharedPtr< Controller<float> >);
    sizeof (SharedPtr< std::vector<String> > );
    sizeof (SharedPtr< WaveformControllerFunction > );
    sizeof (SharedPtr<AnimableValue>  );
    sizeof (SharedPtr<ControllerFunctionRealPtr> );
    sizeof (SharedPtr<ControllerValueRealPtr> );
    sizeof (SharedPtr<DataStream> );
    sizeof (SharedPtr<Ogre::HardwareIndexBuffer> );
    sizeof (SharedPtr<Ogre::MemoryDataStream>);
    sizeof (SharedPtr<Ogre::Resource> );
    sizeof (SharedPtr<Ogre::StringVector> );
    sizeof (SharedPtr<Ogre::StringVectorPtr> );
    sizeof (SharedPtr<std::vector<std::string, std::allocator<std::string> > >);
    sizeof (Singleton<Ogre::ArchiveManager>);
    sizeof (Singleton<Ogre::CompositorManager>);
    sizeof (Singleton<Ogre::ControllerManager>);
    sizeof (Singleton<Ogre::GpuProgramManager>);
    sizeof (Singleton<Ogre::HardwareBufferManager>);
    sizeof (Singleton<Ogre::HighLevelGpuProgramManager>);
    sizeof (Singleton<Ogre::LogManager>);
    sizeof (Singleton<Ogre::MaterialManager>);
    sizeof (Singleton<Ogre::MeshManager>);
    sizeof (Singleton<Ogre::OverlayManager>);
    sizeof (Singleton<Ogre::ParticleSystemManager>);
    sizeof (Singleton<Ogre::Profiler>);
    sizeof (Singleton<Ogre::ResourceBackgroundQueue>);
    sizeof (Singleton<Ogre::ResourceGroupManager>);
    sizeof (Singleton<Ogre::Root>);
    sizeof (Singleton<Ogre::SceneManagerEnumerator>);
    sizeof (Singleton<Ogre::ShadowTextureManager>);
    sizeof (Singleton<Ogre::SkeletonManager>);
    sizeof (Singleton<Ogre::TextureManager>);
    sizeof (std::_Tree<std::_Tmap_traits<std::string, Ogre::Animation*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::Animation*> >, false> >::iterator );
    sizeof (std::_Tree<std::_Tmap_traits<std::string, Ogre::AnimationState*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::AnimationState*> >, false> >::const_iterator );
    sizeof (std::_Tree<std::_Tmap_traits<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> >, false> >::iterator );
    sizeof (std::_Tree<std::_Tmap_traits<std::string, Ogre::ParticleSystem*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::ParticleSystem*> >, false> >::iterator );
    sizeof (std::_Tree<std::_Tmap_traits<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> >, false> >::const_iterator );
    sizeof (std::_Tree<std::_Tmap_traits<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> >, true> >::iterator );
    sizeof (std::_Tree<std::_Tmap_traits<unsigned long, Ogre::SharedPtr<Ogre::Resource>, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, Ogre::SharedPtr<Ogre::Resource> > >, false> >::iterator);
    sizeof (std::_Tree<std::_Tmap_traits<unsigned short, Ogre::RenderPriorityGroup*, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, Ogre::RenderPriorityGroup*> >, false> >::iterator );
    sizeof (std::_Tree<std::_Tmap_traits<unsigned short, Ogre::VertexAnimationTrack*, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, Ogre::VertexAnimationTrack*> >, false> >::const_iterator );
    sizeof (std::list<Ogre::AnimationState*, std::allocator<Ogre::AnimationState*> > );
    sizeof (std::list<Ogre::AnimationState*, std::allocator<Ogre::AnimationState*> >);
    sizeof (std::list<Ogre::AnimationState*, std::allocator<Ogre::AnimationState*> >::const_iterator );
    sizeof (std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::AnimationState*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::AnimationState*> > > );
    sizeof (std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > );
    sizeof (std::map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > >);
    sizeof (std::map<String, String> );    // NameValuePairList
    sizeof (std::map<unsigned int,class Ogre::StaticGeometry::Region *,struct std::less<unsigned int>, class std::allocator<struct std::pair<unsigned int const ,class Ogre::StaticGeometry::Region *> > >);
    sizeof (std::map<unsigned, Ogre::Vector3, std::less<unsigned>, std::allocator<std::pair<unsigned const, Ogre::Vector3> > >);
    sizeof (std::multimap<Ogre::RenderWindow*,Ogre::WindowEventListener*,std::less<Ogre::RenderWindow*>,std::allocator<std::pair<Ogre::RenderWindow* const, Ogre::WindowEventListener*> > >);
    sizeof (std::multimap<Ogre::Vector3,Ogre::Vector3,std::less<Ogre::Vector3>,std::allocator<std::pair<const Ogre::Vector3, Ogre::Vector3> > > );
    sizeof (std::multimap<RenderWindow*, WindowEventListener*> ) ; // WindowEventListeners;
    sizeof (std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,std::basic_string<char, std::char_traits<char>, std::allocator<char> >,std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >,std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >);
    sizeof (std::pair< std::wstring, std::wstring > );
    sizeof (std::pair<bool, float> );
    sizeof (std::pair<bool, float> );
    sizeof (std::pair<unsigned int,unsigned int> );
    sizeof (std::pair<unsigned, unsigned> );
    
    sizeof (std::set<Ogre::Entity*,std::less<Ogre::Entity*>,std::allocator<Ogre::Entity*> > );
    sizeof (std::vector<Ogre::LinkedSkeletonAnimationSource, std::allocator<Ogre::LinkedSkeletonAnimationSource> >::const_iterator );
    sizeof (std::vector<Ogre::RenderWindow*,std::allocator<Ogre::RenderWindow*> > );
    sizeof (std::vector<Ogre::ShadowRenderable*, std::allocator<Ogre::ShadowRenderable*> >::iterator );
    sizeof (std::vector<Ogre::ShadowTextureConfig, std::allocator<Ogre::ShadowTextureConfig> >::const_iterator );
    sizeof (std::vector<Ogre::Vector4, std::allocator< Ogre::Vector4> > );
    sizeof (std::vector<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > >  );
    sizeof (std::vector<std::string, std::allocator<std::string> >);
    sizeof (std::vector<String> );
    sizeof (stdext::hash_map<std::string, unsigned short, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, unsigned short> > > );
    sizeof (TRect<long int>);
    sizeof (VectorIterator<std::vector<Ogre::Bone*, std::allocator<Ogre::Bone*> > >);
    sizeof (VectorIterator<std::vector<Ogre::CompositionTargetPass*, std::allocator<Ogre::CompositionTargetPass*> > >);
    sizeof (VectorIterator<std::vector<Ogre::CompositionTargetPass*, std::allocator<Ogre::CompositionTargetPass*> > >);
    sizeof (VectorIterator<std::vector<Ogre::CompositorInstance*, std::allocator<Ogre::CompositorInstance*> > >);
    sizeof (VectorIterator<std::vector<Ogre::CompositorInstance*, std::allocator<Ogre::CompositorInstance*> > >);
    sizeof (VectorIterator<std::vector<Ogre::IlluminationPass*, std::allocator<Ogre::IlluminationPass*> > >);
    sizeof (VectorIterator<std::vector<Ogre::IlluminationPass*, std::allocator<Ogre::IlluminationPass*> > >);
    sizeof (VectorIterator<std::vector<Ogre::Pass*, std::allocator<Ogre::Pass*> > >);
    sizeof (VectorIterator<std::vector<Ogre::Pass*, std::allocator<Ogre::Pass*> > >);
    sizeof (VectorIterator<std::vector<Ogre::RenderQueueInvocation*, std::allocator<Ogre::RenderQueueInvocation*> > >);
    sizeof (VectorIterator<std::vector<Ogre::Technique*, std::allocator<Ogre::Technique*> > >);
    sizeof (VectorIterator<std::vector<Ogre::TextureUnitState*, std::allocator<Ogre::TextureUnitState*> > >);
    sizeof (VectorIterator<std::vector<Ogre::TextureUnitState*, std::allocator<Ogre::TextureUnitState*> > >);
    sizeof (VectorIterator<std::vector<Ogre::VertexPoseKeyFrame::PoseRef, std::allocator<Ogre::VertexPoseKeyFrame::PoseRef> > >);
     
    sizeof (std::pair<std::list<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*>, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*> > >::const_iterator,std::list<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*>, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*> > >::const_iterator>);
    sizeof (std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >,Ogre::MovableObject*>);
    sizeof (std::pair<std::list<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*>, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*> > >::iterator,std::list<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*>, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*> > >::iterator>);
    sizeof (std::pair<std::list<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*>, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*> > >::iterator,bool>);
    sizeof (stdext::_Hmap_traits<std::string, Ogre::MovableObject*, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::MovableObject*> >, false>::value_compare);
    sizeof (stdext::_Hmap_traits<std::string, Ogre::MovableObject*, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::MovableObject*> >, false>);
    sizeof (stdext::_Hash<stdext::_Hmap_traits<std::string, unsigned short, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, unsigned short> >, false> >);
   

}

} } //python_ogre::details


#endif//__python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
