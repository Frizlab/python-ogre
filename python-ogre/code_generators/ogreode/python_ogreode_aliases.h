
#include "../ogre/python_ogre_aliases.h"

typedef OgreOde::CircularBuffer<OgreOde::BodyState*> CircularBufferBodyStatePtr;
typedef OgreOde::MaintainedList<OgreOde::Geometry> MaintainedListGeometry;
typedef OgreOde::MaintainedList<OgreOde::Body> MaintainedListBody;
typedef OgreOde::MaintainedList<OgreOde::Space> MaintainedListSpace;
typedef OgreOde::MaintainedList<OgreOde::JointGroup> MaintainedListJointGroup;
typedef OgreOde::MaintainedList<OgreOde::Joint> MaintainedListJoint;


typedef std::vector<unsigned short> VectorUnsignedShort;
typedef std::list<OgreOde::Geometry*>   GeometryList;
typedef std::vector<OgreOde::Geometry*> GeometryArray;
typedef std::map<unsigned char,OgreOde::Vector3Array* > BoneMapping;
typedef std::pair<unsigned short,OgreOde::Vector3Array* > BoneMappingKey;
typedef unsigned long MaterialID;
typedef std::map<OgreOde::MaterialID,OgreOde::Contact*> MaterialMap;
typedef std::pair<OgreOde::MaterialID,OgreOde::MaterialMap* > MaterialMapPair;

typedef std::vector<OgreOde_Prefab::Ragdoll::PhysicalBone*> RagdollPhysicalBoneList;
typedef std::list<OgreOde_Prefab::Object  *>   ObjectList;



        
         
         
