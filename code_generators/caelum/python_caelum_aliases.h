typedef std::map<std::string, std::string> MapStringString;
typedef std::vector<std::string> StdVectorString;

typedef std::map<Ogre::Viewport*, Caelum::DepthComposerInstance*, std::less<Ogre::Viewport*> > ViewportDepthComposerInstanceMap;
typedef ::Caelum::DefaultOwnedPtrTraits< Ogre::SceneNode > DefaultOwnedPtrTraitsSceneNode;
typedef ::Caelum::OwnedResourcePtrTraits< Ogre::Material, Ogre::MaterialPtr, Ogre::MaterialManager > OwnedResourcePtrTraitsMaterial;
typedef ::Caelum::MovableObjectOwnedPtrTraits< Ogre::ManualObject > MovableObjectOwnedPtrTraitsManualObject;
typedef ::Caelum::DefaultOwnedPtrTraits< Ogre::ManualObject > DefaultOwnedPtrTraitsManualObject;
