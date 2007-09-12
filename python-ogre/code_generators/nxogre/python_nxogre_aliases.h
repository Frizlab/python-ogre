typedef std::vector<NxOgre::Joint::LimitPlane,std::allocator<NxOgre::Joint::LimitPlane> > VectorJointLimitPlane;
typedef std::vector<NxOgre::SimplePlane*,std::allocator<NxOgre::SimplePlane*> > VectorSimplePlane;

typedef std::vector<float, std::allocator<float> > VectorFloat;
typedef std::vector<std::string, std::allocator<std::string> > VectorString;
typedef std::list<std::string> stdString;
typedef std::map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > > MapPairString;
typedef std::vector<Ogre::Plane, std::allocator<Ogre::Plane> > VectorPlane;
typedef std::vector<Ogre::Vector4, std::allocator<Ogre::Vector4> > VectorVector4;
typedef std::pair<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,std::basic_string<char, std::char_traits<char>, std::allocator<char> > >\
    PairString;
    
typedef NxOgre::List<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > ListString;
typedef NxOgre::List<NxOgre::Blueprints::ActorBlueprint*> ListBlueprints;
typedef  NxOgre::Container<unsigned, NxOgre::ShapeBlueprint const&> ContainerShapeBlueprint;
typedef NxOgre::Container<std::string, NxOgre::ActorBlueprint> ContainerActorBlueprint;
//typedef NxOgre::Container<NxOgre::Scene::Renderables, float> ContainerSceneRencerables;
typedef std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,float,std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >,std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, float> > >\
    MapStrings;
typedef std::map<unsigned, float, std::less<unsigned>, std::allocator<std::pair<unsigned const, float> > >\
    MapUnsignedFloat;
typedef std::vector<NxOgre::Scene*, std::allocator<NxOgre::Scene*> > VectorScene;
typedef std::list<NxOgre::Blueprints::ActorBlueprint*, std::allocator<NxOgre::Blueprints::ActorBlueprint*> > ListBlueprintsActorBlueprint;
typedef std::list<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > >\
    ListPairString;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::DominanceGroup*>::Containee, std::less<std::string>, std::allocator<std::pair<std::string const, NxOgre::Container<std::string, NxOgre::DominanceGroup*>::Containee> > >\
    MapContainerDominanceGroup;
        
    
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::Scene*>::Containee, std::less<std::string> >\
     MapContainerScene;
typedef std::list<NxOgre::CharacterHitReport*, std::allocator<NxOgre::CharacterHitReport*> > ListCharacterHitReportPtr;
typedef std::list<NxOgre::Wheel*, std::allocator<NxOgre::Wheel*> > ListWheelPtr;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::Actor*>::Containee, std::less<std::string> >\
     MapContainerActor;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::Cloth*>::Containee, std::less<std::string> >\
     MapContainerCloth;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::Helper*>::Containee, std::less<std::string> >\
     MapContainerHelper;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::Joint*>::Containee, std::less<std::string> >\
     MapContainerJoint;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::Material*>::Containee, std::less<std::string> >\
     MapContainerMaterial;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::Character*>::Containee, std::less<std::string> >\
     MapContainerCharacter;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::Fluid*>::Containee, std::less<std::string> >\
     MapContainerFluid;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::FluidDrain*>::Containee, std::less<std::string> >\
     MapContainerFluidDrain;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::FluidEmitter*>::Containee, std::less<std::string> >\
     MapContainerFluidEmitter;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::SoftBody*>::Containee, std::less<std::string> >\
     MapContainerSoftBody;
     
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::RayCastHit>::Containee, std::less<std::string> >\
     MapContainerRayCastHit;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::ShapeGroup*>::Containee, std::less<std::string> >\
     MapContainerShapeGroup;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::Trigger*>::Containee, std::less<std::string> >\
     MapContainerTrigger;
typedef std::list<NxOgre::RemoteDebuggerConnection::Camera, std::allocator<NxOgre::RemoteDebuggerConnection::Camera> >\
    ListRemoteDebuggerConnectionCamera;
// typedef std::vector<NxOgre::Summary::Feature> VectorSummaryFeature;
typedef std::map<unsigned, NxOgre::Container<unsigned, NxOgre::Shape*>::Containee, std::less<unsigned> >\
    MapContainerUnsignedShape;
typedef std::map<unsigned, NxOgre::Container<unsigned, NxOgre::ErrorReporter*>::Containee, std::less<unsigned> >\
    MapContainerUnsignedErrorReporter;
typedef std::map<unsigned short, NxOgre::Container<unsigned short, NxOgre::ShapeGroup*>::Containee, std::less<unsigned short> >\
    MapContainerShortShapeGroup;
typedef std::map<unsigned short, NxOgre::Container<unsigned short, NxOgre::ActorGroup*>::Containee, std::less<unsigned short> >\
    MapContainerShortActorGroup;
typedef std::map<unsigned short, NxOgre::Container<unsigned short, NxOgre::Material*>::Containee, std::less<unsigned short> >\
    MapContainerShortMaterial;
typedef std::map<std::string, NxOgre::Container<std::string, unsigned short>::Containee, std::less<std::string> >\
    MapContainerShortString;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::ActorGroup*>::Containee, std::less<std::string> >\
    MapContainerActorGroup;
    
    
    

typedef NxOgre::List<NxOgre::RemoteDebuggerConnection::Camera> ListRemoteCamera;
// this one is protected :(    
// typedef std::map<NxOgre::Scene::Renderables, NxOgre::Container<NxOgre::Scene::Renderables, float>::Containee, std::less<NxOgre::Scene::Renderables> >\
//     MapContainerSceneRenderables;
                
typedef NxOgre::Container<NxOgre::NxString, NxOgre::RayCastHit>   RayCastReport;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::Scene*>    Scenes;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::ShapeGroup*>   ShapeGroups;
// typedef NxOgre::Container<NxOgre::NxCollisionGroup, NxOgre::ShapeGroup*>  ShapeGroupsByIndex;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::Trigger*>   Triggers;
typedef NxOgre::List<NxOgre::Wheel*>     Wheels;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::Character*>   Characters;
typedef NxOgre::List<NxOgre::CharacterHitReport*>    CharacterHitReports;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::Fluid*>    Fluids;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::FluidDrain*>   FluidDrains;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::FluidEmitter*>   FluidEmitters;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::SoftBody*>   SoftBodies;
// typedef std::map<NxOgre::TI, NxOgre::Containee > Type;
// typedef std::list<NxOgre::TT> Type;
typedef std::pair<NxOgre::NxString,NxOgre::NxString> Parameter;
typedef std::vector<Parameter> Parameters;
typedef Ogre::String NxString;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::Actor*>    Actors;
// typedef NxOgre::Container<NxOgre::NxString, ::ActorGroup*>   ActorGroups;
// typedef NxOgre::Container<NxOgre::NxActorGroup, NxOgre::ActorGroup*>  ActorGroupsByIndex;
typedef NxOgre::Container<NxOgre::NxShapeIndex, NxOgre::Shape*>   CollisionModel;
//typedef NxOgre::Container<NxOgre::NxShapeIndex, const NxOgre::ShapeDescription&> CollisionDescriptionModel;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::Cloth*>    Cloths;
typedef NxOgre::Container<NxOgre::NxErrorIndex, NxOgre::ErrorReporter*>  ErrorReporterList;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::Helper*>   Helpers;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::Joint*>    Joints;
typedef NxOgre::Container<NxOgre::NxString, NxOgre::Material*>   MaterialList;
// typedef NxOgre::Container<NxOgre::NxMaterialIndex, NxOgre::Material*>  MaterialListByIndex;
// typedef NxOgre::Container<NxOgre::NxString, NxOgre::NxMaterialIndex>  MaterialPair;

// typedef std::vector<NxOgre::Feature> FeatureSet;
typedef std::map<std::string, NxOgre::Container<std::string, std::string>::Containee, std::less<std::string>, std::allocator<std::pair<std::string const, NxOgre::Container<std::string, std::string>::Containee> > >\
    MapContainerString;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::ActorBlueprint>::Containee, std::less<std::string>, std::allocator<std::pair<std::string const, NxOgre::Container<std::string, NxOgre::ActorBlueprint>::Containee> > >\
    MapContainerActorBlueprint;
typedef std::map<std::string, NxOgre::Container<std::string, NxOgre::ForceField*>::Containee, std::less<std::string>, std::allocator<std::pair<std::string const, NxOgre::Container<std::string, NxOgre::ForceField*>::Containee> > >\
    MapContainerForceField;
typedef std::map<unsigned, NxOgre::Container<unsigned, NxOgre::ShapeBlueprint const&>::Containee, std::less<unsigned>, std::allocator<std::pair<unsigned const, NxOgre::Container<unsigned, NxOgre::ShapeBlueprint const&>::Containee> > >\
    MapContainerShapeBlueprint;
typedef std::vector<unsigned short, std::allocator<unsigned short> > VectorShort;
typedef std::vector<std::pair<Ogre::Vector3, unsigned>, std::allocator<std::pair<Ogre::Vector3, unsigned> > > VectorPairVector3;
typedef std::set<Ogre::Entity*,std::less<Ogre::Entity*>,std::allocator<Ogre::Entity*> > SetEntity;
typedef NxOgre::Material NxOgreMaterial;    
