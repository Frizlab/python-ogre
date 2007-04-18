import os
import environment


# WRAPPER_DEFINITION_ShadowRenderable =\
# """
# Ogre::ShadowRenderable * ShadowRenderable_castElement(Ogre::ShadowRenderable * r){
#     if( dynamic_cast< Ogre::Region::RegionShadowRenderable * >( r ) ){
#         return  (Ogre::Region::RegionShadowRenderable*) r ;
#     }   
#     if( dynamic_cast< Ogre::ManualObject::ManualObjectSectionShadowRenderable * >( r ) ){
#         return  (Ogre::ManualObject::ManualObjectSectionShadowRenderable*) r ;
#     }    
#     if( dynamic_cast< Ogre::Entity::EntityShadowRenderable * >( r ) ){
#         return (Ogre::Entity::EntityShadowRenderable*) r ;
#     }
#     return  ( r );
#     }
#     
# Ogre::ShadowRenderable* ShadowRenderable_getLightCapRenderable( Ogre::ShadowRenderable & me){
#     return ShadowRenderable_castElement ( me.getLightCapRenderable() );
#     }
# """
WRAPPER_REGISTRATION_ShadowRenderable = [
    'def( "getLightCapRenderable", &::ShadowRenderable_getLightCapRenderable,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    
    ]    
    

WRAPPER_DEFINITION_OverlayElement = \
"""
Ogre::OverlayElement * OverlayElement_castElement(Ogre::OverlayElement * e){
    if( dynamic_cast< Ogre::TextAreaOverlayElement * >( e ) ){
        return (Ogre::TextAreaOverlayElement*) e;
    }   
    if( dynamic_cast< Ogre::OverlayContainer * >( e ) ){
        return (Ogre::OverlayContainer*) e;
    }
    return  ( e );
    }
Ogre::OverlayElement * OverlayElement_findElementAt(Ogre::OverlayElement &me, Ogre::Real x, Ogre::Real y) {
    return OverlayElement_castElement ( me.findElementAt ( x, y) );
    }
Ogre::OverlayElement * OverlayElement_clone(Ogre::OverlayElement &me, Ogre::String const & instanceName) {
    return OverlayElement_castElement ( me.clone ( instanceName ) );
    }
/*Ogre::OverlayElement * OverlayElement_getSourceTemplate(Ogre::OverlayElement &me) {
    return OverlayElement_castElement ( me.getSourceTemplate () );
    }*/
"""
WRAPPER_REGISTRATION_OverlayElement = [
    'def( "findElementAt", &::OverlayElement_findElementAt,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',

    'def( "clone", &::OverlayElement_clone,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',

    ]
    
WRAPPER_DEFINITION_Node = \
"""
Ogre::SceneNode * 
Node_castAsSceneNode ( Ogre::Node * n ) {
    return ( (Ogre::SceneNode* ) n );
    }
Ogre::Node * Node_castNode(Ogre::Node * n){
    if( dynamic_cast< Ogre::SceneNode * >( n ) ){
        return  (Ogre::SceneNode*) n ;
    }    
    if( dynamic_cast< Ogre::Bone * >( n ) ){
        return  (Ogre::Bone*)n  ;
    }    
    return ( n );  
}

Ogre::Node * Node_getChild_short(Ogre::Node& me, unsigned short index){
    return Node_castNode( me.getChild( index ) );
}

Ogre::Node * Node_getChild_string(Ogre::Node& me, const Ogre::String& name){
    return Node_castNode( me.getChild( name ) );
}
Ogre::Node * Node_getParent(Ogre::Node& me){
    return Node_castNode( me.getParent( ) );
}

Ogre::Node * Node_removeChild1(Ogre::Node& me, unsigned short index){
    return Node_castNode( me.removeChild( index ) );
}
Ogre::Node * Node_removeChild2(Ogre::Node& me, const Ogre::String& name){
    return Node_castNode( me.removeChild( name ) );
}
Ogre::Node * Node_removeChild3(Ogre::Node& me, Ogre::Node * child){
    return Node_castNode( me.removeChild( child ) );
}
"""

WRAPPER_REGISTRATION_Node = [
    'def( "getChild", &::Node_getChild_short,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    'def( "getChild", &::Node_getChild_string,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    'def( "getParent", &::Node_getParent,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    'def( "removeChild", &::Node_removeChild1,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    'def( "removeChild", &::Node_removeChild2,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    'def( "removeChild", &::Node_removeChild3,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    'def( "castAsSceneNode", &::Node_castAsSceneNode,\
    "Python-Ogre Helper Function\\nCase a Node as a Scene Node",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());'
    ]

WRAPPER_DEFINITION_ResourceManager = \
"""
// I'm not going to wrap ResourceManager::load in the same way as getByName etc as you can always do a load
// and then do a getByName

boost::python::object ResourceManager_CastResource ( Ogre::ResourcePtr r ) {
   if( dynamic_cast< Ogre::Texture* >( r.get() ) ){
        return boost::python::object( Ogre::TexturePtr( r ) );
    }
    if( dynamic_cast< Ogre::Material* >( r.get() ) ){
        return boost::python::object( Ogre::MaterialPtr( r ) );
    }
   if( dynamic_cast< Ogre::Compositor* >( r.get() ) ){
        return boost::python::object( Ogre::CompositorPtr( r ) );
    }
    if( dynamic_cast< Ogre::Font* >( r.get() ) ){
        return boost::python::object( Ogre::FontPtr( r ) );
    }   
    if( dynamic_cast< Ogre::GpuProgram* >( r.get() ) ){
        return boost::python::object( Ogre::GpuProgramPtr( r ) );
    }
    if( dynamic_cast< Ogre::HighLevelGpuProgram* >( r.get() ) ){
        return boost::python::object( Ogre::HighLevelGpuProgramPtr( r ) );
    }
    if( dynamic_cast< Ogre::Mesh* >( r.get() ) ){
        return boost::python::object( Ogre::MeshPtr( r ) );
    }
    if( dynamic_cast< Ogre::Skeleton* >( r.get() ) ){
        return boost::python::object( Ogre::SkeletonPtr( r ) );
    }
    return boost::python::object( r ); //unknown type
}   
 
boost::python::object ResourceManager_getByHandle(Ogre::ResourceManager& me,Ogre::ResourceHandle handle) {
    return ResourceManager_CastResource ( me.getByHandle( handle ) );
}
boost::python::object ResourceManager_getByName(Ogre::ResourceManager& me, const std::string& name){
    return ResourceManager_CastResource (me.getByName( name ) );
}
boost::python::object ResourceManager_create(Ogre::ResourceManager& me,Ogre::String const & name, 
                            Ogre::String const & group, bool isManual=false, 
                            Ogre::ManualResourceLoader * loader=0, Ogre::NameValuePairList const * createParams=0) {
    return ResourceManager_CastResource (me.create(name, group, isManual,  loader, createParams) );
}
                            
boost::python::object ResourceManager_load(Ogre::ResourceManager& me,Ogre::String const & name, 
                            Ogre::String const & group, bool isManual=false, Ogre::ManualResourceLoader * loader=0, 
                            Ogre::NameValuePairList const * loadParams=0){
                            
    return ResourceManager_CastResource (me.load(name, group, isManual, loader, loadParams) );
}
"""


WRAPPER_REGISTRATION_ResourceManager = [
    'def( "getByName", &::ResourceManager_getByName,\
    "Python-Ogre Hand Wrapped\\n" );',
    'def( "getByHandle", &::ResourceManager_getByHandle,\
    "Python-Ogre Hand Wrapped\\n" );',
    'def( "create", &::ResourceManager_create,\
    ( bp::arg("name"), bp::arg("group"), bp::arg("isManual")=(bool)(false), bp::arg("loader")=bp::object(), bp::arg("createParams")=bp::object() ),\
    "Python-Ogre Hand Wrapped\\n" );',
    'def( "load", &::ResourceManager_load,\
    ( bp::arg("name"), bp::arg("group"), bp::arg("isManual")=(bool)(false), bp::arg("loader")=bp::object(), bp::arg("loadParams")=bp::object() ),\
    "Python-Ogre Hand Wrapped\\n" );'
    ]
    


WRAPPER_DEFINITION_General = \
"""
boost::python::tuple 
GetOgreVersion () {
            return ( boost::python::make_tuple( Ogre::StringConverter::toString(OGRE_VERSION_MAJOR),
                                                Ogre::StringConverter::toString(OGRE_VERSION_MINOR),
                                                Ogre::StringConverter::toString(OGRE_VERSION_PATCH),
                                                OGRE_VERSION_NAME 
                                                ) );
}
boost::python::tuple 
GetPythonOgreVersion () {
            return ( boost::python::make_tuple( """ + environment.PythonOgreMajorVersion + """,
                                                """ + environment.PythonOgreMinorVersion + """,
                                                """ + environment.PythonOgrePatchVersion + """
                                                ) );
}
void
Utility_setFloat(void * ptrin, boost::python::list listin)
{
    int index;
    float * newptr = reinterpret_cast<float *>(ptrin);
    for (index=0;index<len(listin);index++ ) {
//        newptr[index] = boost::python::extract<float> (listin[index]);
        *newptr++ = boost::python::extract<float> (listin[index]);
        }
}


boost::python::list
Utility_getFloat(void * ptrin,  int size)
{
    boost::python::list outlist;
    int index;
    float * newptr = reinterpret_cast<float *>(ptrin);
    for (index=0;index<size;index++ ) {
        outlist.append ( *newptr++ );
        }
    return outlist;
}

void
Utility_setUint16(void * ptrin, boost::python::list listin)     // unsigned short
{
    int index;
    Ogre::uint16 * newptr = reinterpret_cast<Ogre::uint16 *>(ptrin);
    for (index=0;index<len(listin);index++ ) {
        *newptr++ = boost::python::extract<Ogre::uint16> (listin[index]);
        }
}

boost::python::list
Utility_getUint16(void * ptrin,  int size)
{
    boost::python::list outlist;
    int index;
    Ogre::uint16 * newptr = reinterpret_cast<Ogre::uint16 *>(ptrin);
    for (index=0;index<size;index++ ) {
        outlist.append ( *newptr++ );
        }
    return outlist;
}

// sometimes we need to take the ctypess addressof(), an int, and recast it as a general void *
void *
Utility_CastVoidPtr ( int address )
{
    return (void *) address;
    }
    
// Othertimes we need the address the void * points to
unsigned int
Utility_CastInt ( void * address )
{
    return (unsigned int) address;
    }
    
boost::python::object 
Utility_CastResourceToNative(Ogre::ResourcePtr& r){
   if( dynamic_cast< Ogre::Texture* >( r.get() ) ){
        return boost::python::object( Ogre::TexturePtr( r ) );
    }
    if( dynamic_cast< Ogre::Material* >( r.get() ) ){
        return boost::python::object( Ogre::MaterialPtr( r ) );
    }
   if( dynamic_cast< Ogre::Compositor* >( r.get() ) ){
        return boost::python::object( Ogre::CompositorPtr( r ) );
    }
    if( dynamic_cast< Ogre::Font* >( r.get() ) ){
        return boost::python::object( Ogre::FontPtr( r ) );
    }   
    if( dynamic_cast< Ogre::GpuProgram* >( r.get() ) ){
        return boost::python::object( Ogre::GpuProgramPtr( r ) );
    }
    if( dynamic_cast< Ogre::HighLevelGpuProgram* >( r.get() ) ){
        return boost::python::object( Ogre::HighLevelGpuProgramPtr( r ) );
    }
    if( dynamic_cast< Ogre::Mesh* >( r.get() ) ){
        return boost::python::object( Ogre::MeshPtr( r ) );
    }
    if( dynamic_cast< Ogre::Skeleton* >( r.get() ) ){
        return boost::python::object( Ogre::SkeletonPtr( r ) );
    }
    return boost::python::object( r ); //unknown type
}       

// void
// Utility_setFloat(void * ptrin, float datain)
// {
//     float * newptr = reinterpret_cast<float *>(ptrin);
//     newptr[0]= datain;
// }
"""            
WRAPPER_REGISTRATION_General = [
    """bp::def( "GetOgreVersion", &GetOgreVersion,
                "Python-Ogre Helper Function: Return the version of Ogre.\\n\\
                Input: None\\n\\
                Ouput: Tuple [major,minor,patch]");""",
    """bp::def( "GetPythonOgreVersion", &GetPythonOgreVersion,
                "Python-Ogre Helper Function: Return the version of Python-Ogre.\\n\\
                Input: None\\n\\
                Ouput: Tuple [major,minor,patch]");""",
    """bp::def( "setFloat", &Utility_setFloat,
                "Python-Ogre Helper Function: Write Floats to Memory.\\n\\
                Input: void *, Python List (numerics)\\n\\
                Ouput: None\\n\\
                The list is extracted as floats and written to memory starting at the pointer" );""",
    """bp::def( "getFloat", &Utility_getFloat,
                "Python-Ogre Helper Function: Reads floats into a python list.\\n\\
                Input: void *, size\\n\\
                Ouput: Python List\\n\\
                The list is populated with floats from memory starting at the pointer" );""",
    """bp::def( "setUint16", &Utility_setUint16,
                "Python-Ogre Helper Function: Write Unsigned Ints to Memory.\\n\\
                Input: void *, Python List (numerics)\\n\\
                Ouput: None\\n\\
                The list is extracted as unsigned ints and written to memory starting at the pointer" );""",
    """bp::def( "getUint16", &Utility_getFloat,
                "Python-Ogre Helper Function: Reads ints into a python list.\\n\\
                Input: void *, size\\n\\
                Ouput: Python List\\n\\
                The list is populated with ints from memory starting at the pointer" );""",
    """bp::def( "CastVoidPtr", &Utility_CastVoidPtr,
                bp::return_value_policy< bp::return_opaque_pointer >(),
                "Python-Ogre Helper Function: Casts a number to a void *.\\n\\
                Input: numeric value (typically CTypes.addressof(xx) )\\n\\
                Output: A void pointer with the input address");""",
    """bp::def( "CastInt", &Utility_CastInt,
                "Python-Ogre Helper Function: Returns an interger reflecting the void pointer adddress.\\n\\
                Input: void * \\n\\
                Output: A number representing the input address");""",
    """bp::def( "CastResourceToNative", &Utility_CastResourceToNative,
               "Python-Ogre Helper Function: Casts a Resource to it\'s native type.\\n\\
                Input: Resource Object\\n\\
                Output: \'Native\' Object (Texture, Font, Mesh, etc)\\n\\
                This function should not be needed, however just in case you get a resource object from Ogre\\n\\
                and you need to use it as it\'s native type");"""
     ]

##################################################################

WRAPPER_DEFINITION_BillboardSet = \
"""
boost::python::list
Billboardset_getTextureCoords (Ogre::BillboardSet & me ) {
    boost::python::list outlist;
    Ogre::FloatRect const * fr;
    Ogre::uint16 count;
    // make the underlying call
    fr = me.getTextureCoords( &count );
    while ( count-- > 0 ) {
        outlist.append ( *fr++ );
        }
    return outlist;
}

void
Billboardset_setTextureCoords ( Ogre::BillboardSet & me, boost::python::list listin )
{
    Ogre::FloatRect * fr, *start;
    fr = new Ogre::FloatRect [ len( listin ) ];
    start = fr;
    int index;
    for (index =0 ; index <len(listin); index ++) {
        *fr++ = boost::python::extract<Ogre::FloatRect> (listin[index]);
        }
    Ogre::uint16 count;
    count = len(listin);
    me.setTextureCoords( start, count );
}
"""
WRAPPER_REGISTRATION_BillboardSet = [
        """def( "getTextureCoords", &::Billboardset_getTextureCoords,
         "Python-Ogre Hand Wrapped Function: Returns a list of TextureCoords.\\n\\
                Input: Nothing\\n\\
                Output: List of FloatRec\'s");""",
        """def( "setTextureCoords", &::Billboardset_setTextureCoords,
         "Python-Ogre Hand Wrapped Function: set a list of TextureCoords.\\n\\
                Input: List of FloatRec\'s\\n\\
                Output: Nothing");"""
                
    ]
###################################################


WRAPPER_DEFINITION_RenderTarget = \
"""
static int
RenderTarget_getCustomAttributeInt(Ogre::RenderTarget& rd, const std::string& name) {
    int value(0);
    rd.getCustomAttribute( name, &value );
    return value;
}
static float
RenderTarget_getCustomAttributeFloat(Ogre::RenderTarget& rd, const std::string& name) {
    float value(0);
    rd.getCustomAttribute( name, &value );
    return value;
}
"""
WRAPPER_REGISTRATION_RenderTarget = [
    """def( "getCustomAttributeInt", &::RenderTarget_getCustomAttributeInt,
    "Python-Ogre Helper Function: Get a Custom Atribute as an int\\n\\
    Input: None\\n\\
    Output: Numeric (int)\\n\\
    Gets a custom attribure from RenderTarget as an int" );""",
    """def( "getCustomAttributeFloat", &::RenderTarget_getCustomAttributeFloat,
    "Python-Ogre Helper Function: Get a Custom Atribute as a float\\n\\
    Input: None\\n\\
    Output: Numeric (float)\\n\\
    Gets a custom attribure from RenderTarget as a float" );"""
    ]

#####################################################################

WRAPPER_DEFINITION_HardwareBufferManager = \
"""
::Ogre::HardwareIndexBufferSharedPtr 
hbm_createIndexBuffer( ::Ogre::HardwareIndexBuffer::IndexType itype, ::size_t numIndexes, 
                        ::Ogre::HardwareBuffer::Usage usage, bool useShadowBuffer=false ){
        return me.createIndexBuffer( itype, numIndexes, usage, useShadowBuffer );
    }

"""
WRAPPER_REGISTRATION_HardwareBufferManager = [
    """def( "createIndexBuffer", &::hbm_createIndexBuffer );
//           bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());"""
    ]



## Needed as boost doesn't handle this overload properly and always uses 'Plane' as the override - see renderToTexture demo
WRAPPER_DEFINITION_Frustum = \
"""
void
Frustum_enableCustomNearClipPlaneMP(Ogre::Frustum& Fr, Ogre::MovablePlane const * MP ) {
        Fr.enableCustomNearClipPlane ( MP ) ;
}
"""
WRAPPER_REGISTRATION_Frustum = [
    """def( "enableCustomNearClipPlaneMP", &::Frustum_enableCustomNearClipPlaneMP );"""
    ]

    
#################################################################################################
    
WRAPPER_DEFINITION_SubMesh =\
"""
void
SubMesh_createVertexData ( Ogre::SubMesh & me ) {
    me.vertexData = new Ogre::VertexData();
    }
 
"""
WRAPPER_REGISTRATION_SubMesh = [
    """def( "createVertexData", &::SubMesh_createVertexData );"""
    ] 


def iter_as_generator_vector( cls ):
    print "ITER:", cls
    
    code = os.linesep.join([ 
            'typedef %(cls)s iter_type;'
            , 'generators::generator_maker_vector< iter_type >::register_< %(call_policies)s >( %(exposer_name)s );'])
    cls.add_registration_code( 
            code % { 'cls' : cls.decl_string
                     , 'call_policies' : cls.mem_fun( 'getNext' ).call_policies.create_type()
                     , 'exposer_name' : cls.class_var_name }
            , works_on_instance=False )
    cls.include_files.append( 'generators.h' )
            
def iter_as_generator_map( cls ):
    print "ITER:", cls
    
    code = os.linesep.join([ 
            'typedef %(cls)s iter_type;'
            , 'generators::generator_maker_map< iter_type >::register_< %(call_policies)s >( %(exposer_name)s );'])
    cls.add_registration_code( 
            code % { 'cls' : cls.decl_string
                     , 'call_policies' : cls.mem_fun( 'getNext' ).call_policies.create_type()
                     , 'exposer_name' : cls.class_var_name }
            , works_on_instance=False )
    cls.include_files.append( 'generators.h' )

#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'RenderTarget' )
    rt.add_declaration_code( WRAPPER_DEFINITION_RenderTarget )
    apply_reg (rt,  WRAPPER_REGISTRATION_RenderTarget )
    
    rt = mb.class_( 'ResourceManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_ResourceManager )
    apply_reg (rt,  WRAPPER_REGISTRATION_ResourceManager )
    
    rt = mb.class_( 'Node' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Node )
    apply_reg (rt,  WRAPPER_REGISTRATION_Node )

#     rt = mb.class_( 'ShadowRenderable' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_ShadowRenderable )
#     apply_reg (rt,  WRAPPER_REGISTRATION_ShadowRenderable )

    rt = mb.class_( 'OverlayElement' )
    rt.add_declaration_code( WRAPPER_DEFINITION_OverlayElement )
    apply_reg (rt,  WRAPPER_REGISTRATION_OverlayElement )

    rt = mb.class_( 'Frustum' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Frustum )
    apply_reg (rt,  WRAPPER_REGISTRATION_Frustum )
    
    rt = mb.class_( 'BillboardSet' )
    rt.add_declaration_code( WRAPPER_DEFINITION_BillboardSet )
    apply_reg (rt,  WRAPPER_REGISTRATION_BillboardSet )
 
    rt = mb.class_( 'SubMesh' )
    rt.add_declaration_code( WRAPPER_DEFINITION_SubMesh )
    apply_reg (rt,  WRAPPER_REGISTRATION_SubMesh )
    
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    apply_reg (mb,  WRAPPER_REGISTRATION_General )
    
    vec_iterators = mb.classes( lambda cls: cls.name.startswith( 'VectorIterator<' ) )
    for cls in vec_iterators:
        iter_as_generator_vector( cls )
        
    map_iterators = mb.classes( lambda cls: cls.name.startswith( 'MapIterator<' ) )
    for cls in map_iterators:
        iter_as_generator_map( cls )   
             
