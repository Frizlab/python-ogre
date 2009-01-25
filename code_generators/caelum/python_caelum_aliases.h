typedef std::map<std::string, std::string> MapStringString;
typedef std::vector<std::string> StdVectorString;

typedef std::map<Ogre::Viewport*, Caelum::DepthComposerInstance*, std::less<Ogre::Viewport*> > ViewportDepthComposerInstanceMap;
typedef ::Caelum::DefaultOwnedPtrTraits< Ogre::SceneNode > DefaultOwnedPtrTraitsSceneNode;
typedef ::Caelum::OwnedResourcePtrTraits< Ogre::Material, Ogre::MaterialPtr, Ogre::MaterialManager > OwnedResourcePtrTraitsMaterial;
typedef ::Caelum::MovableObjectOwnedPtrTraits< Ogre::ManualObject > MovableObjectOwnedPtrTraitsManualObject;
typedef ::Caelum::DefaultOwnedPtrTraits< Ogre::ManualObject > DefaultOwnedPtrTraitsManualObject;
typedef ::Caelum::DefaultOwnedPtrTraits< Ogre::Entity > DefaultOwnedPtrTraitsEntity;
typedef ::Caelum::MovableObjectOwnedPtrTraits< Ogre::Entity > MovableObjectOwnedPtrTraitsEntity;
typedef ::Caelum::OwnedResourcePtrTraits< Ogre::Mesh, Ogre::MeshPtr, Ogre::MeshManager > OwnedResourcePtrTraitsMesh;
