import os
import environment

#
# this HAS to be my biggest hack to date...  When you call loadObject you either get a vehicle or a ragdoll
# as the return.. If I use a wrapper function that returns a boost:python::object then it works fine for
# the vehicle -- but fails on the ragdoll as there isn't an 'init' function defined so you can't create 
# a new python object (you are meant to use ragdollFactory..

# However if I use a Object pointer as the return type and use reference_existing_object in the call defination then
# it works fine for the Ragdoll but fails in an "interesting" way for the Vehicle.. It seems to work but when you delete
# the vehicle the destructor(c++) is never called (like there is still a reference somewhere?)..
#
# main function
#    Object* DotLoader::loadObject(const String &filename, const String &object_name, const String &instance_name)
#            Returns via  objLoading = parseObject (child, object_name, instance_name);
#
#   Object* DotLoader::parseObject(const TiXmlNode *child, const String &object_name, const String &instance_name)
#         objLoading = parseVehicle (child, object_name, instance_name);
#         objLoading = parseRagdoll (child, object_name, instance_name);
#
# inside parseRagdoll....
#  ragdoll = static_cast<OgreOde_Prefab::Ragdoll*>
#             (_world->getSceneManager ()->createMovableObject(instanceFinalName, 
#                                                                             OgreOde_Prefab::RagdollFactory::FACTORY_TYPE_NAME,
#                                                                             &params)); 
#
# inside parseVehicle
#        vehicle = new Vehicle(instanceFinalName, _world, _space);



WRAPPER_DEFINITION_OgreOdeLoader=\
"""        
boost::python::object  OgreOde_Loader_loadVehicle(OgreOde_Loader::DotLoader& me, const Ogre::String &filename, 
                    const Ogre::String &object_name, const Ogre::String &instance_name = Ogre::StringUtil::BLANK)
{
   OgreOde_Prefab::Object *  r = me.loadObject( filename, object_name, instance_name );
   if( dynamic_cast< OgreOde_Prefab::Vehicle* >( r ) ){
        return  boost::python::object( (OgreOde_Prefab::Vehicle *)( r ) );
    }
    return boost::python::object( r ); //unknown type
}

OgreOde_Prefab::Object *  OgreOde_Loader_loadRagdoll(OgreOde_Loader::DotLoader& me, const Ogre::String &filename, 
                    const Ogre::String &object_name, const Ogre::String &instance_name = Ogre::StringUtil::BLANK)
{
    OgreOde_Prefab::Object *  r = me.loadObject( filename, object_name, instance_name );
     if( dynamic_cast< OgreOde_Prefab::Ragdoll* >( r ) ){
         return (OgreOde_Prefab::Ragdoll *)( r );
     }
    return  r; //unknown type
}

"""     
WRAPPER_REGISTRATION_OgreOdeLoader = [
    """def( "loadVehicle", &::OgreOde_Loader_loadVehicle,
    ( bp::arg("filename"), bp::arg("object_name"), bp::arg("instance_name")=Ogre::StringUtil::BLANK ),
    "Python-Ogre Hand Wrapped\\nReturns a vehicle or ragdoll as appropiate" );""",    
    """def( "loadRagdoll", &::OgreOde_Loader_loadRagdoll,
    ( bp::arg("filename"), bp::arg("object_name"), bp::arg("instance_name")=Ogre::StringUtil::BLANK ),
    "Python-Ogre Hand Wrapped\\nReturns a vehicle or ragdoll as appropiate",
     bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());"""
    ]
 
    
WRAPPER_DEFINITION_Helper = \
"""
OgreOde::TriangleMeshGeometry *
OgreOdeGeometry_makeTriangleMeshGeometry (boost::python::list vertices, unsigned int vertex_count, 
                             boost::python::list indices, unsigned int index_count,
                             OgreOde::World *world,  OgreOde::Space* space = 0) {
                             
    Ogre::Vector3 * Vectors;
    OgreOde::TriangleIndex * Triangles;
    unsigned int index;
    OgreOde::TriangleMeshGeometry * ret;
    
        
    // first lets make sure the counts aren't 'too' big
    if ( (unsigned int) bp::len (vertices) < vertex_count ) vertex_count = (unsigned int)bp::len(vertices);
    // allocate memory
    Vectors = new Ogre::Vector3 [ vertex_count ];
    // do the copy    
    for (index =0 ; index < vertex_count ; index ++) {
        Vectors[index] = bp::extract<Ogre::Vector3> (vertices[index]);
        }  
                                   
    // now the same for the indices                             
    if ( (unsigned int)bp::len (indices) < index_count ) index_count = (unsigned int)bp::len(indices);
    Triangles = new OgreOde::TriangleIndex [ index_count ];
    for (index =0 ; index < index_count; index ++) {
        Triangles[index] = bp::extract<OgreOde::TriangleIndex> (indices[index]);
        }                             
    
    ret = new OgreOde::TriangleMeshGeometry ( Vectors, vertex_count, Triangles, index_count, world, space );
    delete []  Vectors;
    delete [] Triangles;
    return (  ret );
    
    }      
    
OgreOde::ConvexGeometry *
OgreOdeGeometry_makeConvexGeometry (boost::python::list vertices, unsigned int vertex_count, 
                             boost::python::list indices, unsigned int index_count,
                             OgreOde::World *world,  OgreOde::Space* space = 0) {
                             
    Ogre::Vector3 * Vectors;
    unsigned int* Triangles;
    unsigned int index;
    OgreOde::ConvexGeometry * ret;
    
        
    // first lets make sure the counts aren't 'too' big
    if ( (unsigned int)bp::len (vertices) < vertex_count ) vertex_count = (unsigned int)bp::len(vertices);
    // allocate memory
    Vectors = new Ogre::Vector3 [ vertex_count ];
    // do the copy    
    for (index =0 ; index < vertex_count; index ++) {
        Vectors[index] = boost::python::extract<Ogre::Vector3> (vertices[index]);
        }  
                                   
    // now the same for the indices                             
    if ( (unsigned int)bp::len (indices) < index_count ) index_count = (unsigned int)bp::len(indices);
    Triangles = new unsigned int [ index_count ];
    for (index =0 ; index < index_count; index ++) {
        Triangles[index] = boost::python::extract<unsigned int> (indices[index]);
        }                             
   
    ret =  new OgreOde::ConvexGeometry ( Vectors, vertex_count, Triangles, index_count, world, space );
    delete [] Vectors;
    delete [] Triangles;
    return ret;
    }

"""    
     
WRAPPER_REGISTRATION_Helper = [
    """def( "makeTriangleMeshGeometry", &::OgreOdeGeometry_makeTriangleMeshGeometry,
    ( bp::arg("vertices"), bp::arg("vertex_count"), bp::arg("indices"), bp::arg("index_count"),
    bp::arg("world"), bp::arg("space")=0 ),
    "Python-Ogre Hand Wrapped\\nReturns a TriangleMeshGeometry object",
     bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """def( "makeConvexGeometry", &::OgreOdeGeometry_makeConvexGeometry,
    ( bp::arg("vertices"), bp::arg("vertex_count"), bp::arg("indices"), bp::arg("index_count"),
    bp::arg("world"), bp::arg("space")=0 ),
    "Python-Ogre Hand Wrapped\\nReturns a ConvexGeometry object",
     bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());"""

    ]

        
WRAPPER_DEFINITION_EntityInformer=\
"""
boost::python::list
OgreOdeEntityInformer_getBoneVertices( OgreOde::EntityInformer & me, unsigned char bone, unsigned int &vertex_count) {
    
    boost::python::list outlist;
    Ogre::Vector3 * vertices;
    
    if  ( ! me.getBoneVertices(bone,vertex_count,vertices ) )
        return outlist;
    
    while ( vertex_count-- > 0 ) {
        outlist.append ( *vertices++ );
        }
    delete[] vertices;        
    return outlist;
}
"""
WRAPPER_REGISTRATION_EntityInformer = [
    """def( "getBoneVertices", &::OgreOdeEntityInformer_getBoneVertices,
    "Python-Ogre Hand Wrapped\\nReturns a list of vertices\\n\\
    Input: Bone number, vertex count\\n\\
    Output: List of vertices (can be empty)" );"""   
    ]
       
def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'DotLoader' )
    rt.add_declaration_code( WRAPPER_DEFINITION_OgreOdeLoader )
    apply_reg (rt,  WRAPPER_REGISTRATION_OgreOdeLoader )
    rt = mb.class_( 'EntityInformer' )
    rt.add_declaration_code( WRAPPER_DEFINITION_EntityInformer )
    apply_reg (rt,  WRAPPER_REGISTRATION_EntityInformer )
    mb.add_declaration_code( WRAPPER_DEFINITION_Helper )
    apply_reg (mb,  WRAPPER_REGISTRATION_Helper )
       