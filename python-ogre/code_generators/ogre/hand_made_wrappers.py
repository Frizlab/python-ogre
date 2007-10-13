import os
import environment


WRAPPER_DEFINITION_PixelBox =\
"""
// return the data buffer - can't be handled 'normally' by Py++
void * PixelBox_data ( ::Ogre::PixelBox & me )
{
    return me.data;
}
"""
WRAPPER_REGISTRATION_PixelBox = [
    """def( "getData", &::PixelBox_data,\\
                "Python-Ogre Helper Function: Returns the data buffer.\\n\\
                Input: \\n\\
                Output: A void pointer to the data buffer",\\
                bp::return_value_policy< bp::return_opaque_pointer >());"""
    ]


WRAPPER_WRAPPER_RenderQueueListener =\
"""
    virtual void renderQueueEnded( ::Ogre::uint8 queueGroupId, ::Ogre::String const & invocation, bool & repeatThisInvocation ){
        bool holder;
        bp::override func_renderQueueEnded = this->get_override( "renderQueueEnded" ); 
        holder = func_renderQueueEnded( queueGroupId, invocation, repeatThisInvocation );
        repeatThisInvocation = holder;
    }


    virtual void renderQueueStarted( ::Ogre::uint8 queueGroupId, ::Ogre::String const & invocation, bool & skipThisInvocation ){
        bool holder;
        bp::override func_renderQueueStarted = this->get_override( "renderQueueStarted" );
        holder = func_renderQueueStarted( queueGroupId, invocation, skipThisInvocation );
        skipThisInvocation = holder;
    }
"""



WRAPPER_DEFINITION_OverlayElement = \
"""
Ogre::OverlayElement * OverlayElement_castElementAsOverlayContainer(Ogre::OverlayElement * e){  
    if( dynamic_cast< Ogre::OverlayContainer * >( e ) ){
        return (Ogre::OverlayContainer*) e;
    }
    return  ( e );
    }
Ogre::OverlayElement * OverlayElement_castElementAsTextAreaOverlay(Ogre::OverlayElement * e){
    if( dynamic_cast< Ogre::TextAreaOverlayElement * >( e ) ){
        return (Ogre::TextAreaOverlayElement*) e;
    }   
    return  ( e );
    }   
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
"""
WRAPPER_REGISTRATION_OverlayElement = [
    'def( "findElementAt", &::OverlayElement_findElementAt,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',

    'def( "clone", &::OverlayElement_clone,\
    "Python-Ogre Hand Wrapped\\n",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    
    'def( "castElement", &::OverlayElement_castElement,\
    "Python-Ogre Hand Wrapped\\nWill return the overlay element as either a textareaoverlay or an overlaycontainer",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    
    'def( "castElementAsOverlayContainer", &::OverlayElement_castElementAsOverlayContainer,\
    "Python-Ogre Hand Wrapped\\nWill return the overlay element as an OverlayContainer (if possible)",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    
    'def( "castElementAsTextAreaOverlay", &::OverlayElement_castElementAsTextAreaOverlay,\
    "Python-Ogre Hand Wrapped\\nWill return the overlay element as a TextAreaOverlay (if possible)",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',

    ]
# WRAPPER_DEFINITION_OverlayManager = \
# """
# Ogre::OverlayElement* OverlayManager_createOverlayElement(Ogre::OverlayManager &me, const Ogre::String& typeName, 
#                   const Ogre::String& instanceName, bool isTemplate = false){
#    Ogre::OverlayElement* e = me.createOverlayElement(typeName, instanceName, isTemplate );
#    if( dynamic_cast< Ogre::TextAreaOverlayElement * >( e ) ){
#         return (Ogre::TextAreaOverlayElement*) e;
#     }   
#     if( dynamic_cast< Ogre::OverlayContainer * >( e ) ){
#         return (Ogre::OverlayContainer*) e;
#     }
#     return  ( e );
#     }
# """  
#                 
# WRAPPER_REGISTRATION_OverlayManager = [
#     'def( "createOverlayElement", &::OverlayManager_createOverlayElement,\
#     "Python-Ogre Hand Wrapped\\nShould Return the correct Type (TextAreOverlayElement or OverlayContainer)",\
#     bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
# ]

    
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
    "Python-Ogre Helper Function\\nCast a Node as a Scene Node",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());'
    'def( "castNode", &::Node_castNode,\
    "Python-Ogre Helper Function\\nCast a Node as a Scene Node or Bone Node (automatic)",\
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
Ogre::OverlayContainer * General_castElementAsOverlayContainer( Ogre::OverlayElement * e ){  
       return (Ogre::OverlayContainer*) e;
    }
    
Ogre::OverlayContainer * General_castTextAreaAsOverlayContainer( Ogre::TextAreaOverlayElement * e ){  
        return (Ogre::OverlayContainer*) e;
    }
Ogre::OverlayContainer * General_castPanelAsOverlayContainer( Ogre::PanelOverlayElement * e ){  
        return (Ogre::OverlayContainer*) e;
    }
    
Ogre::OverlayContainer * General_castBorderPanelAsOverlayContainer( Ogre::BorderPanelOverlayElement * e ){  
        return (Ogre::OverlayContainer*) e;
    }
    
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


Ogre::VertexData *
Utility_createVertexData ( ) {
    return (new Ogre::VertexData());
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
Utility_setUint32(void * ptrin, boost::python::list listin)     // unsigned short
{
    int index;
    Ogre::uint32 * newptr = reinterpret_cast<Ogre::uint32 *>(ptrin);
    for (index=0;index<len(listin);index++ ) {
        *newptr++ = boost::python::extract<Ogre::uint32> (listin[index]);
        }
}

boost::python::list
Utility_getUint32(void * ptrin,  int size)
{
    boost::python::list outlist;
    int index;
    Ogre::uint32 * newptr = reinterpret_cast<Ogre::uint32 *>(ptrin);
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
Utility_CastVoidPtr ( unsigned long address )
{
    return (void *) address;
    }
    
// Othertimes we need the address the void * points to
unsigned long
Utility_CastInt ( void * address )
{
    return (unsigned long) address;
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

    """bp::def( "castAsOverlayContainer", &General_castElementAsOverlayContainer,
                "Python-Ogre Helper Function: Casts as Overlay Container -- needed for add2D.\\n\\
                Input: Element\\n\\
                Ouput: OverlayContainer",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def( "castAsOverlayContainer", &General_castTextAreaAsOverlayContainer,
                "Python-Ogre Helper Function: Casts as Overlay Container -- needed for add2D.\\n\\
                Input: Element\\n\\
                Ouput: OverlayContainer",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def( "castAsOverlayContainer", &General_castPanelAsOverlayContainer,
                "Python-Ogre Helper Function: Casts as Overlay Container -- needed for add2D.\\n\\
                Input: Element\\n\\
                Ouput: OverlayContainer",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def( "castAsOverlayContainer", &General_castBorderPanelAsOverlayContainer,
                "Python-Ogre Helper Function: Casts as Overlay Container -- needed for add2D.\\n\\
                Input: Element\\n\\
                Ouput: OverlayContainer",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",

    """bp::def( "createVertexData", &Utility_createVertexData,
                "Python-Ogre Helper Function: Returns a pointer to a 'new' VertexData element.\\n\\
                Input: None\\n\\
                Ouput: VertexData",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
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
    """bp::def( "getUint16", &Utility_getUint16,
                "Python-Ogre Helper Function: Reads ints into a python list.\\n\\
                Input: void *, size\\n\\
                Ouput: Python List\\n\\
                The list is populated with ints from memory starting at the pointer" );""",
    """bp::def( "setUint32", &Utility_setUint32,
                "Python-Ogre Helper Function: Write Unsigned Ints 32 to Memory.\\n\\
                Input: void *, Python List (numerics)\\n\\
                Ouput: None\\n\\
                The list is extracted as unsigned ints 32 and written to memory starting at the pointer" );""",
    """bp::def( "getUint32", &Utility_getUint32,
                "Python-Ogre Helper Function: Reads ints 32 into a python list.\\n\\
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
static unsigned long
RenderTarget_getCustomAttributeUnsignedLong(Ogre::RenderTarget& rd, const std::string& name) {
    unsigned long value(0);
    rd.getCustomAttribute( name, &value );
    return value;
}
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
    Gets a custom attribute from RenderTarget as an int" );""",
    """def( "getCustomAttributeFloat", &::RenderTarget_getCustomAttributeFloat,
    "Python-Ogre Helper Function: Get a Custom Atribute as a float\\n\\
    Input: None\\n\\
    Output: Numeric (float)\\n\\
    Gets a custom attribute from RenderTarget as a float" );""",
    """def( "getCustomAttributeUnsignedLong", &::RenderTarget_getCustomAttributeUnsignedLong,
    "Python-Ogre Helper Function: Get a Custom Atribute as an unsigned long\\n\\
    Input: None\\n\\
    Output: Numeric (unsigned long)\\n\\
    Gets a custom attribute from RenderTarget as an unsigned long" );"""
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
SubMesh_createandsetVertexData ( Ogre::SubMesh & me ) {
    me.vertexData = new Ogre::VertexData();
    }
    
  
 
"""
WRAPPER_REGISTRATION_SubMesh = [
    """def( "createVertexData", &::SubMesh_createandsetVertexData );"""
    ] 

    
#################################################################################################
WRAPPER_DEFINITION_SceneManager =\
"""
bool SceneManager_setOption( Ogre::SceneManager & me, const Ogre::String& strKey, bp::object pValue ) {

    bp::extract<Ogre::AxisAlignedBox> p( pValue );
    if( p.check() ){
        Ogre::AxisAlignedBox var = p();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<Ogre::Vector3> p3( pValue );
    if( p3.check() ){
        Ogre::Vector3 var = p3();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<Ogre::Matrix3> p4( pValue );
    if( p4.check() ){
        Ogre::Matrix3 var = p4();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<Ogre::Matrix4> p5( pValue );
    if( p5.check() ){
        Ogre::Matrix4 var = p5();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<Ogre::Quaternion> p6( pValue );
    if( p6.check() ){
        Ogre::Quaternion var = p6();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<int> p2( pValue );
    if( p2.check() ){
        int var = p2();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    bp::extract<bool> p1( pValue );
    if( p1.check() ){
        bool var = p1();
        return me.setOption ( strKey, reinterpret_cast< void * >( &var ) );
    }
    return false;
}


bp::tuple SceneManager_getOption( Ogre::SceneManager & me, const Ogre::String& strKey, bp::object returnType) { 

    bp::extract<Ogre::AxisAlignedBox> p( returnType );
    if( p.check() ){
        Ogre::AxisAlignedBox var = p();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<Ogre::Vector3> p3( returnType );
    if( p3.check() ){
        Ogre::Vector3 var = p3();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<Ogre::Matrix3> p4( returnType );
    if( p4.check() ){
        Ogre::Matrix3 var = p4();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<Ogre::Matrix4> p5( returnType );
    if( p5.check() ){
        Ogre::Matrix4 var = p5();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<Ogre::Quaternion> p6( returnType );
    if( p6.check() ){
        Ogre::Quaternion var = p6();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<int> p2( returnType );
    if( p2.check() ){
        int var = p2();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    bp::extract<bool> p1( returnType );
    if( p1.check() ){
        bool var = p1();
        bool ret = me.getOption ( strKey, reinterpret_cast< void * >( &var ) );
        return bp::make_tuple ( ret, var );
    }
    return bp::make_tuple ( false, false ); 
}

"""

WRAPPER_REGISTRATION_SceneManager = [
    """def( "setOption", &::SceneManager_setOption ,
    "Python-Ogre Helper Function: sets an option in the SceneManager\\n\\
    Input: Option Name, Option Value\\n\\
    Ouput: True/False" );""",
    """def( "getOption", &::SceneManager_getOption ,
    "Python-Ogre Helper Function: returns an option from the SceneManager\\n\\
    Input: Option Name\\n\\
    Ouput: Tuple containing bool (True/False -- success of call) and value" );"""
    ] 

#################################################################################################
WRAPPER_DEFINITION_ParticleSystem =\
"""
// Dummy function so we can show the need to change the call
void ParticleSystem_getIterator( Ogre::ParticleSystem & me ) {
        throw std::logic_error ("You shouldn't be using _getIterator - use getParticles instead");
    return;
    }
    
bp::list ParticleSystem_getParticles ( Ogre::ParticleSystem & me ) {  
    bp::list listout;
    Ogre::ParticleIterator pit = me._getIterator();
    while(!pit.end()) {
        Ogre::Particle *particle = pit.getNext();
        listout.append ( particle );
        }
    return listout;
	}
"""

WRAPPER_REGISTRATION_ParticleSystem = [
    """def( "_getIterator", &ParticleSystem_getIterator,
                "Python-Ogre Helper Function: Unavailable! Use getParticles instead");""",
    """def( "getParticles", &ParticleSystem_getParticles,
            "Python-Ogre Helper Function: returns a list of particles\\n\\
            Input: None\\n\\
            Ouput: List containing particles" );"""
    ]
    
#################################################################################################

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

    rt = mb.class_( 'PixelBox' )
    rt.add_declaration_code( WRAPPER_DEFINITION_PixelBox )
    apply_reg (rt,  WRAPPER_REGISTRATION_PixelBox )
    
#     rt = mb.class_( 'ShadowRenderable' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_ShadowRenderable )
#     apply_reg (rt,  WRAPPER_REGISTRATION_ShadowRenderable )

    rt = mb.class_( 'OverlayElement' )
    rt.add_declaration_code( WRAPPER_DEFINITION_OverlayElement )
    apply_reg (rt,  WRAPPER_REGISTRATION_OverlayElement )

#     rt = mb.class_( 'OverlayManager' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_OverlayManager )
#     apply_reg (rt,  WRAPPER_REGISTRATION_OverlayManager )

    rt = mb.class_( 'Frustum' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Frustum )
    apply_reg (rt,  WRAPPER_REGISTRATION_Frustum )
    
    rt = mb.class_( 'BillboardSet' )
    rt.add_declaration_code( WRAPPER_DEFINITION_BillboardSet )
    apply_reg (rt,  WRAPPER_REGISTRATION_BillboardSet )
 
    rt = mb.class_( 'SubMesh' )
    rt.add_declaration_code( WRAPPER_DEFINITION_SubMesh )
    apply_reg (rt,  WRAPPER_REGISTRATION_SubMesh )
    
    rt = mb.class_( 'SceneManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_SceneManager )
    apply_reg (rt,  WRAPPER_REGISTRATION_SceneManager )
    
    rt = mb.class_( 'ParticleSystem' )
    rt.add_declaration_code( WRAPPER_DEFINITION_ParticleSystem )
    apply_reg (rt,  WRAPPER_REGISTRATION_ParticleSystem )
    
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    apply_reg (mb,  WRAPPER_REGISTRATION_General )
    
    vec_iterators = mb.classes( lambda cls: cls.name.startswith( 'VectorIterator<' ) )
    for cls in vec_iterators:
        iter_as_generator_vector( cls )
        
    map_iterators = mb.classes( lambda cls: cls.name.startswith( 'MapIterator<' ) )
    for cls in map_iterators:
        iter_as_generator_map( cls ) 
        
    rt = mb.class_( 'RenderQueueListener' )   
    print "WRAPPER CODE\n\n"
    print rt.wrapper_code
    rt.add_wrapper_code ( WRAPPER_WRAPPER_RenderQueueListener )
    print "NOW\n\n"
    print rt.wrapper_code
             
#     cls = mb.class_('Animation').class_('NodeTrackIterator')
#     iter_as_generator_map( cls )
#     global_ns = mb.global_ns
#     ogre_ns = global_ns.namespace( 'Ogre' )
#     i = ogre_ns.typedef( name="NodeTrackIterator" )
#     iter_as_generator_map( i )
          
#     typedef std::map<unsigned short, NodeAnimationTrack*> NodeTrackList;
#         typedef ConstMapIterator<NodeTrackList> NodeTrackIterator;

# 		typedef std::map<unsigned short, NumericAnimationTrack*> NumericTrackList;
# 		typedef ConstMapIterator<NumericTrackList> NumericTrackIterator;

# 		typedef std::map<unsigned short, VertexAnimationTrack*> VertexTrackList;
# 		typedef ConstMapIterator<VertexTrackList> VertexTrackIterator;

