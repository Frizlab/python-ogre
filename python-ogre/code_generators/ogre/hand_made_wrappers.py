import os
import environment


WRAPPER_DEFINITION_PixelBox =\
"""
// return the data buffer - can't be handled 'normally' by Py++
void * PixelBox_getData ( ::Ogre::PixelBox & me )
{
    return me.data;
}
// sets the data buffer - can't be handled 'normally' by Py++
void PixelBox_setData ( ::Ogre::PixelBox & me, void * Ptr )
{
    me.data = Ptr;
}
"""
WRAPPER_REGISTRATION_PixelBox = [
    """def( "getData", &::PixelBox_getData,\\
                "Python-Ogre Helper Function: Returns the data buffer.\\n\\
                Input: \\n\\
                Output: A void pointer to the data buffer",\\
                bp::return_value_policy< bp::return_opaque_pointer >());""",
    """def( "setData", &::PixelBox_setData,\\
                "Python-Ogre Helper Function: Set the data buffer.\\n\\
                Input: A void Pointer ie ogre.CastVoidPtr(ctypes.addressof(buff)) \\n\\
                Output: nothing");"""
    ]

WRAPPER_DEFINITION_MemoryDataStream =\
"""
// return the data buffer - can't be handled 'normally' by Py++
unsigned long MemoryDataStream_data ( ::Ogre::MemoryDataStream & me )
{
    return (unsigned long) me.getPtr();
}

void MemoryDataStream_setDataList ( ::Ogre::MemoryDataStream & me, boost::python::list listin )
{
    Ogre::uchar * data = me.getPtr();
    size_t maxlen = len ( listin );
    size_t counter = 0;
    for (counter=0;counter < maxlen; counter ++ )
        *data++ =  boost::python::extract<Ogre::uchar> (listin[counter]);
}

void MemoryDataStream_setDataStr ( ::Ogre::MemoryDataStream & me, boost::python::str listin )
{
    Ogre::uchar * data = me.getPtr();
    char const* c_str = boost::python::extract<char const*>(listin);
    size_t maxlen = len(listin);
    size_t counter;
    for (counter=0; counter < maxlen; counter ++ )
        *data++ = *c_str++;
}

boost::python::list
MemoryDataStream_getData(::Ogre::MemoryDataStream & me)
{
    boost::python::list outlist;
    size_t index;
    Ogre::uchar * data = me.getPtr();
    size_t size = me.size();
    for (index=0;index<size;index++ ) {
        outlist.append ( *data++ );
        }
    return outlist;
}
"""
WRAPPER_REGISTRATION_MemoryDataStream = [
    """def( "getDataPointer", &::MemoryDataStream_data,\\
                "Python-Ogre Helper Function: Returns the data buffer.\\n\\
                Input: \\n\\
                Output: Unsigned Long address of the data buffer");""",
    """def( "setData", &::MemoryDataStream_setDataList,\\
                "Python-Ogre Helper Function: Takes a python list and loads it into the memory buffer (as unsigned chars).\\n\\
                Input: Python List\\n\\
                Output: None");""",
    """def( "setData", &::MemoryDataStream_setDataStr,\\
                "Python-Ogre Helper Function: Takes a python string and loads it into the memory buffer (as unsigned chars).\\n\\
                Input: Python String\\n\\
                Output: None");""",
    """def( "getData", &::MemoryDataStream_getData,\\
                "Python-Ogre Helper Function: Copies the data buffer to a python list.\\n\\
                Input: None\\n\\
                Output: Python List");"""
    ]

    

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

WRAPPER_DEFINITION_Bone = \
"""
    Ogre::Node * 
    Bone_castAsNode ( Ogre::Bone * n ) {
    return ( (Ogre::Node* ) n );
    }   
    """
    
WRAPPER_REGISTRATION_Bone = [
    'def( "castAsNode", &::Bone_castAsNode,\
    "Python-Ogre Helper Function\\nCast a Bone as a pure node",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());'
    ]
    
WRAPPER_DEFINITION_SceneNode = \
"""
    Ogre::Node * 
    SceneNode_castAsNode ( Ogre::SceneNode * n ) {
    return ( (Ogre::Node* ) n );
    }   
    """
    
WRAPPER_REGISTRATION_SceneNode = [
    'def( "castAsNode", &::SceneNode_castAsNode,\
    "Python-Ogre Helper Function\\nCast a Bone as a pure node",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());'
    ]

            
WRAPPER_DEFINITION_Node = \
"""
Ogre::SceneNode * 
Node_castAsSceneNode ( Ogre::Node * n ) {
    return ( (Ogre::SceneNode* ) n );
    }
    
Ogre::Node * Node_castAutomatic(Ogre::Node * n){
    if( dynamic_cast< Ogre::SceneNode * >( n ) ){
        return  (Ogre::SceneNode*) n ;
    }    
    if( dynamic_cast< Ogre::Bone * >( n ) ){
        return  (Ogre::Bone*)n  ;
    }    
    return ( n );  
}

Ogre::Node * Node_getChild_short(Ogre::Node& me, unsigned short index){
    return Node_castAutomatic( me.getChild( index ) );
}
Ogre::Node * Node_getChild_string(Ogre::Node& me, const Ogre::String& name){
    return Node_castAutomatic( me.getChild( name ) );
}
Ogre::Node * Node_getParent(Ogre::Node& me){
    return Node_castAutomatic( me.getParent( ) );
}
Ogre::Node * Node_removeChild1(Ogre::Node& me, unsigned short index){
    return Node_castAutomatic( me.removeChild( index ) );
}
Ogre::Node * Node_removeChild2(Ogre::Node& me, const Ogre::String& name){
    return Node_castAutomatic( me.removeChild( name ) );
}
Ogre::Node * Node_removeChild3(Ogre::Node& me, Ogre::Node * child){
    return Node_castAutomatic( me.removeChild( child ) );
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
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
    'def( "castAutomatic", &::Node_castAutomatic,\
    "Python-Ogre Helper Function\\nCast a Node as a Scene Node or Bone Node (automatic)",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',
      
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
    

WRAPPER_DEFINITION_Any=\
"""
boost::python::tuple Any_getData(Ogre::Any & me )
    	{
			if (me.isEmpty()) return boost::python::make_tuple( "none", 0 ); 
            if (me.getType() == typeid(int)) 
                return boost::python::make_tuple("i", Ogre::any_cast<int>(me));
            if (me.getType() == typeid(long)) 
                return boost::python::make_tuple("l",Ogre::any_cast<long>(me));
            if (me.getType() == typeid(PyObject *)) {
                PyObject *  data = Ogre::any_cast<PyObject *>(me);
                return boost::python::make_tuple("p",boost::python::object(boost::python::handle<>(boost::python::borrowed(data))) );
                }
            if (me.getType() == typeid(unsigned int)) 
                return boost::python::make_tuple("ui", Ogre::any_cast<unsigned int>(me));
            if (me.getType() == typeid(unsigned long)) 
                return boost::python::make_tuple("ul",Ogre::any_cast<unsigned long>(me));
            if (me.getType() == typeid(Ogre::String)) 
                return boost::python::make_tuple("s", Ogre::any_cast<Ogre::String>(me));
            if (me.getType() == typeid(Ogre::Real)) 
                return boost::python::make_tuple("r", Ogre::any_cast<Ogre::Real>(me));
            if (me.getType() == typeid(unsigned char)) 
                return boost::python::make_tuple("uc", Ogre::any_cast<unsigned char>(me));
            if (me.getType() == typeid(unsigned short)) 
                return boost::python::make_tuple("us", Ogre::any_cast<unsigned short>(me));
//            if (me.getType() == typeid(Ogre::UserDefinedObject *)) 
//                return boost::python::make_tuple("ud", Ogre::any_cast<Ogre::UserDefinedObject *>(me));
			return boost::python::make_tuple( "unknown", 0 ); // TODO: really should raise an exception 
            }

"""   
WRAPPER_REGISTRATION_Any = [   
"""def( "getData", &Any_getData, 
                "Python-Ogre Helper Function: extract an python object from an Ogre.Any variable");""",
]                
WRAPPER_DEFINITION_General = \
"""
bool checkDataStreamPtr(Ogre::DataStreamPtr& stream) 
{
    std::cout << "CheckDataStreamPtr called\\n";
    return true;
}

bool checkDataStreamPtrConst(const Ogre::DataStreamPtr& stream) 
{
    std::cout << "CheckDataStreamPtrConst called\\n";
    return true;
}

unsigned long General_getAnyUnsignedLong ( Ogre::Any & me ){
    unsigned long temp = Ogre::any_cast<unsigned long>(me);
    return temp;
    }
    
int General_getAnyInt ( Ogre::Any & me ){
    int temp = Ogre::any_cast<int>(me);
    return temp;
    }
unsigned int General_getAnyUnsignedInt ( Ogre::Any & me ){
    int temp = Ogre::any_cast<unsigned int>(me);
    return temp;
    }    
long General_getAnyLong ( Ogre::Any & me ){
    int temp = Ogre::any_cast<long>(me);
    return temp;
    }
    
unsigned char General_getAnyUnsignedChar ( Ogre::Any & me ){
    int temp = Ogre::any_cast<unsigned char>(me);
    return temp;
    }
    
unsigned short General_getAnyUnsignedShort ( Ogre::Any & me ){
    int temp = Ogre::any_cast<unsigned short>(me);
    return temp;
    }
    
Ogre::Real General_getAnyReal ( Ogre::Any & me ){
    Ogre::Real temp = Ogre::any_cast<Ogre::Real>(me);
    return temp;
    }
    
Ogre::String General_getAnyString ( Ogre::Any & me ){
    Ogre::String temp = Ogre::any_cast<Ogre::String>(me);
    return temp;
    }
    
/*
Ogre::UserDefinedObject * General_getAnyUserDefinedObject ( Ogre::Any & me ){
    Ogre::UserDefinedObject * temp = Ogre::any_cast<Ogre::UserDefinedObject* >(me);
    return temp;
    }
*/

Ogre::Any * General_createAny ( boost::python::list listin ) {
    Ogre::String intype;
    int outint;
    Ogre::Any * outany;
    if (len(listin) == 2 ) {
        intype = boost::python::extract<Ogre::String> (listin[0] );
        if (intype == "i")  {
            outint = boost::python::extract<int> (listin[1]);
            outany= new Ogre::Any(outint);
            return outany;
            }
        if (intype == "ui")  {
            unsigned int out = boost::python::extract<unsigned int> (listin[1]);
            outany= new Ogre::Any(out);
            return outany;
            }
        if (intype == "l")  {
            long out = boost::python::extract<long> (listin[1]);
            outany= new Ogre::Any(out);
            return outany;
            }
        if (intype == "ul")  {
            unsigned long out = boost::python::extract<unsigned long> (listin[1]);
            outany= new Ogre::Any(out);
            return outany;
            }
        if (intype == "uc")  {
            unsigned char out = boost::python::extract<unsigned char> (listin[1]);
            outany= new Ogre::Any(out);
            return outany;
            }
        if (intype == "us")  {
            unsigned short out = boost::python::extract<unsigned short> (listin[1]);
            outany= new Ogre::Any(out);
            return outany;
            }
        if (intype == "s")  {
            Ogre::String out = boost::python::extract<Ogre::String> (listin[1]);
            outany= new Ogre::Any(out);
            return outany;
            }
        if (intype == "r")  {
            Ogre::Real out = boost::python::extract<Ogre::Real> (listin[1]);
            outany= new Ogre::Any(out);
            return outany;
            }
        if (intype == "P" ){
            unsigned int pointer =  boost::python::extract<unsigned int> (listin[1]);
            return (Ogre::Any * ) pointer;
            }
        if (intype == "p")  {
            boost::python::object t = boost::python::extract<boost::python::object>(listin[1]);
            PyObject * out = t.ptr();
            Py_INCREF( out ); // in this case I'm going to increment the reference 
            outany= new Ogre::Any((void *)out);
            return outany;
            }
        } 
    // assume it's a list with a python object
    if (len(listin) == 1 ) {
        boost::python::object t = boost::python::extract<boost::python::object>(listin[0]);
        PyObject * out = t.ptr();
        Py_INCREF( out ); // in this case I'm going to increment the reference 
        outany= new Ogre::Any((void *)out);
        return outany;
        }
    outany= new Ogre::Any(0);
    return outany;
    }

Ogre::AnyNumeric * General_createAnyNumeric ( boost::python::list listin ) {
    Ogre::String intype;
    int outint;
    Ogre::AnyNumeric * outany;
    outany = new (Ogre::AnyNumeric);
    outany = 0;
    /*if (len(listin) == 2 ) {
        intype = boost::python::extract<Ogre::String> (listin[0] );
        if (intype == "i") {
            outint = boost::python::extract<int> (listin[1]);
            return Ogre::AnyNumeric(outint);
            }
        }*/
    return outany;
    }
    
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
    
Ogre::Node * General_castBoneAsNode( Ogre::Bone * e ){  
        return (Ogre::Node*) e;
    }    
Ogre::Node * General_castSceneNodeAsNode( Ogre::SceneNode * e ){  
        return (Ogre::Node*) e;
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
            return ( boost::python::make_tuple( "\"" + environment.PythonOgreMajorVersion + "\"",
                                                "\"" + environment.PythonOgreMinorVersion + "\"",
                                                "\"" + environment.PythonOgrePatchVersion + "\""
                                                ) );
}
// Duplicating to get lower case 'get'
boost::python::tuple 
getOgreVersion () {
            return ( boost::python::make_tuple( Ogre::StringConverter::toString(OGRE_VERSION_MAJOR),
                                                Ogre::StringConverter::toString(OGRE_VERSION_MINOR),
                                                Ogre::StringConverter::toString(OGRE_VERSION_PATCH),
                                                OGRE_VERSION_NAME 
                                                ) );
}
boost::python::tuple 
getPythonOgreVersion () {
            return ( boost::python::make_tuple( "\"" + environment.PythonOgreMajorVersion + "\"",
                                                "\"" + environment.PythonOgreMinorVersion + "\"",
                                                "\"" + environment.PythonOgrePatchVersion + "\""
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

void
Utility_setUchar(void * ptrin, boost::python::list listin)     // unsigned short
{
    int index;
    Ogre::uchar * newptr = reinterpret_cast<Ogre::uchar *>(ptrin);
    for (index=0;index<len(listin);index++ ) {
        *newptr++ = boost::python::extract<Ogre::uchar> (listin[index]);
        }
}

boost::python::list
Utility_getUchar(void * ptrin,  int size)
{
    boost::python::list outlist;
    int index;
    Ogre::uchar * newptr = reinterpret_cast<Ogre::uchar *>(ptrin);
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

// Othertimes we need the address the void * points to
unsigned long
Utility_CastUnsignedLong ( void * address )
{
    return (unsigned long) address;
    }    
// Othertimes we need the address the void * points to
unsigned long
Utility_getAddress ( PyObject * in )
{
    return (unsigned long) in;
    }
    
Ogre::Any *
Utility_CastAsAny ( unsigned long address )
{
    return (Ogre::Any *) address;
    }    

PyObject *
Utility_castAsObject ( unsigned long address )
{
        return (PyObject *) address;
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

"""            
WRAPPER_REGISTRATION_General = [
    """bp::def( "checkDataStreamPtr", &checkDataStreamPtr);""",
    """bp::def( "checkDataStreamPtrConst", &checkDataStreamPtrConst ); """,
    """bp::def( "getAnyInt", &General_getAnyInt, 
                "Python-Ogre Helper Function: extract an int value from an Ogre.Any varliable");""",
    """bp::def( "getAnyUnsignedInt", &General_getAnyInt, 
                "Python-Ogre Helper Function: extract an unsigned int value from an Ogre.Any varliable");""",
    """bp::def( "getAnyUnsignedLong", &General_getAnyUnsignedLong, 
                "Python-Ogre Helper Function: extract an unsigned long value from an Ogre.Any varliable");""",
    """bp::def( "getAnyUnsignedChar", &General_getAnyUnsignedChar, 
                "Python-Ogre Helper Function: extract an unsigned Char value from an Ogre.Any varliable");""",
    """bp::def( "getAnyUnsignedShort", &General_getAnyUnsignedShort, 
                "Python-Ogre Helper Function: extract an unsigned short value from an Ogre.Any varliable");""",
    """bp::def( "getAnyLong", &General_getAnyLong, 
                "Python-Ogre Helper Function: extract an long value from an Ogre.Any varliable");""",
    """bp::def( "getAnyReal", &General_getAnyReal, 
                "Python-Ogre Helper Function: extract an real value from an Ogre.Any varliable");""",
    """bp::def( "getAnyString", &General_getAnyString, 
                "Python-Ogre Helper Function: extract a string value from an Ogre.Any varliable");""",
#    """bp::def( "getAnyUserDefinedObject", &General_getAnyUserDefinedObject, 
#                "Python-Ogre Helper Function: extract a UserDefinedObject from an Ogre.Any varliable",
#                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def( "createAny", &General_createAny, 
                "Python-Ogre Helper Function: create an Any variable with a specific base type.\\n\\
                Input: Tuple. 1st value is type to create, 2nd is value\\n\\
                Output: Ogre::Any type.\\n\\
                values can be 'i' (int), 'l' (long), 'ui' (unsigned int), 's' (string), 'r' (real)\\n\\
                'ul' (unsigned long),'us' (unsigned short), 'uc' (unsigned char).",
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def( "createAnyNumeric", &General_createAnyNumeric, 
                "Python-Ogre Helper Function: create an AnyNumeric variable with a specific base type.\\n\\
                Input: Tuple. 1st value is type to create, 2nd is value\\n\\
                Output: Ogre::Any type.\\n\\
                values can be 'i' (int), 'l' (long), 'ui' (unsigned int), 'r' (real)\\n\\
                'ul' (unsigned long),'us' (unsigned short), 'uc' (unsigned char).",
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
                
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
    """bp::def( "setUchar", &Utility_setUchar,
                "Python-Ogre Helper Function: Write Unsigned chars to Memory.\\n\\
                Input: void *, Python List (numerics)\\n\\
                Ouput: None\\n\\
                The list is extracted as unsigned ints and written to memory starting at the pointer" );""",
    """bp::def( "getUchar", &Utility_getUchar,
                "Python-Ogre Helper Function: Reads unsigned chars into a python list.\\n\\
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
    """bp::def( "castAsVoidPtr", &Utility_CastVoidPtr,
                bp::return_value_policy< bp::return_opaque_pointer >(),
                "Python-Ogre Helper Function: Casts a number to a void *.\\n\\
                Input: numeric value (typically CTypes.addressof(xx) )\\n\\
                Output: A void pointer with the input address");""",
    """bp::def( "castAsInt", &Utility_CastInt,
                "Python-Ogre Helper Function: Returns an interger reflecting the void pointer adddress.\\n\\
                Input: void * \\n\\
                Output: A number representing the input address");""",
    """bp::def( "castAsUnsignedLong", &Utility_CastUnsignedLong,
                "Python-Ogre Helper Function: Returns an interger reflecting the void pointer adddress.\\n\\
                Input: void * \\n\\
                Output: A number representing the input address");""",
    """bp::def( "getAddress", &Utility_getAddress,
                "Python-Ogre Helper Function: Returns an interger reflecting the python object adddress.\\n\\
                Input: python object \\n\\
                Output: A number representing the address");""",
    """bp::def( "castAsObject", &Utility_castAsObject,
                "Python-Ogre Helper Function: creates a pyobject to an adddress.\\n\\
                Input: address \\n\\
                Output: A python object");
               // bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def("castAsAny", &Utility_CastAsAny,
                "Python-Ogre Helper Function: Returns an Ogre.Any reflecting the given adddress.\\n\\
                Input: address of object \\n\\
                Output: An OgreAny object",
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    
    """bp::def( "castResourceToNative", &Utility_CastResourceToNative,
               "Python-Ogre Helper Function: Casts a Resource to it\'s native type.\\n\\
                Input: Resource Object\\n\\
                Output: \'Native\' Object (Texture, Font, Mesh, etc)\\n\\
                This function should not be needed, however just in case you get a resource object from Ogre\\n\\
                and you need to use it as it\'s native type");""",
    """bp::def( "castBoneToNode", &General_castBoneAsNode,
                "Python-Ogre Helper Function: Casts a bone to a node.\\n\\
                Input: Bone\\n\\
                Ouput: Node",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def( "castSceneNodeToNode", &General_castSceneNodeAsNode,
                "Python-Ogre Helper Function: Casts a SceneNode to a node.\\n\\
                Input: SceneNode\\n\\
                Ouput: Node",\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
                
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


WRAPPER_WRAPPER_GetTypeFix = \
"""
    virtual ::Ogre::String const & %(function_name)s(  ) const {
        bp::object stringin;
        static Ogre::String MyName ("");
        if (MyName.length() < 1) {
           if ( bp::override func_getType = this->get_override( "%(function_name)s" ) ) {
               stringin = func_getType(  ); // can't go direct as const on string
               MyName.clear();
               for (int x=0;x<bp::len(stringin);x++)
                  MyName += bp::extract<char> (stringin[x]);
               }
            else
               throw std::logic_error ("You need to override the %(function_name)s and return a string");
            }
        return MyName;
    }

"""  

WRAPPER_REGISTRATION_GetTypeFix=\
"""def( "%(function_name)s", bp::pure_virtual( &::Ogre::%(class_name)s::%(function_name)s)\
                , "Get the type (string name) of the object to be created",\
                bp::return_value_policy< bp::copy_const_reference >() );"""
                
                
# WRAPPER_DEFINITION_GetTypeFix = \
# """
# 
#         { //getType
#         
#             typedef ::Ogre::String const & ( ::Ogre::%(class_name)s::*%(function_name)s_function_type )(  ) const;
#             
#             %(class_name)s_exposer.def( 
#                 "%(function_name)s"
#                 , bp::pure_virtual( %(function_name)s_function_type(&::Ogre::%(class_name)s::%(function_name)s) )
#                 , bp::return_value_policy< bp::copy_const_reference >()
#                 , "Get the type (string) of the object to be created" );
#         
#         }
# """
    

 
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
    print "ITER:", cls.name
    
    try:
        code = os.linesep.join([ 
                'typedef %(cls)s iter_type;'
                , 'generators::generator_maker_vector< iter_type >::register_< %(call_policies)s >( %(exposer_name)s );'])
        cls.add_registration_code( 
                code % { 'cls' : cls.decl_string
                         , 'call_policies' : cls.mem_fun( 'getNext' ).call_policies.create_type()
                         , 'exposer_name' : cls.class_var_name }
                , works_on_instance=False )
        cls.include_files.append( 'generators.h' )
        print "OK"
    except:
        print "FAILED "
            
def iter_as_generator_map( cls ):
    print "ITER:", cls.name
    
    code = os.linesep.join([ 
            'typedef %(cls)s iter_type;'
            , 'generators::generator_maker_map< iter_type >::register_< %(call_policies)s >( %(exposer_name)s );'])
    try:
        cls.add_registration_code( 
                code % { 'cls' : cls.decl_string
                         , 'call_policies' : cls.mem_fun( 'getNext' ).call_policies.create_type()
                         , 'exposer_name' : cls.class_var_name }
                , works_on_instance=False )
        cls.include_files.append( 'generators.h' )
        print "OK"
    except:
        print "FAILED"
        
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

    rt = mb.class_( 'Any' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Any )
    apply_reg (rt,  WRAPPER_REGISTRATION_Any )
    rt.add_registration_code ( """def(bp::init< PyObject * >(( bp::arg("value") )) )""" )
    
    rt = mb.class_( 'Node' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Node )
    apply_reg (rt,  WRAPPER_REGISTRATION_Node )

    rt = mb.class_( 'Bone' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Bone )
    apply_reg (rt,  WRAPPER_REGISTRATION_Bone )

    rt = mb.class_( 'SceneNode' )
    rt.add_declaration_code( WRAPPER_DEFINITION_SceneNode )
    apply_reg (rt,  WRAPPER_REGISTRATION_SceneNode )

# #     rt = mb.class_( 'PixelBox' )
# #     rt.add_declaration_code( WRAPPER_DEFINITION_PixelBox )
# #     apply_reg (rt,  WRAPPER_REGISTRATION_PixelBox )
    
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
    
    rt = mb.class_( 'SceneManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_SceneManager )
    apply_reg (rt,  WRAPPER_REGISTRATION_SceneManager )
    
    rt = mb.class_( 'MemoryDataStream' )
    rt.add_declaration_code( WRAPPER_DEFINITION_MemoryDataStream )
    apply_reg (rt,  WRAPPER_REGISTRATION_MemoryDataStream )
    
    rt = mb.class_( 'ParticleSystem' )
    rt.add_declaration_code( WRAPPER_DEFINITION_ParticleSystem )
    apply_reg (rt,  WRAPPER_REGISTRATION_ParticleSystem )
    
#     rt = mb.class_( 'MovableObjectFactory' )
#     rt.add_wrapper_code ( WRAPPER_WRAPPER_MovableObjectFactory )
#     apply_reg (rt, WRAPPER_REGISTRATION_MovableObjectFactory )
    
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    apply_reg (mb,  WRAPPER_REGISTRATION_General )
    
    if not environment.ogre.version.startswith("1.7"):
        vec_iterators = mb.classes( lambda cls: cls.name.startswith( 'VectorIterator<' ) )
        for cls in vec_iterators:
            iter_as_generator_vector( cls )
            
        map_iterators = mb.classes( lambda cls: cls.name.startswith( 'MapIterator<' ) )
        for cls in map_iterators:
            iter_as_generator_map( cls ) 
            
    else:
        vec_iterators = mb.classes( lambda cls: cls.name.startswith( 'VectorIteratorWrapper<' ) )
        for cls in vec_iterators:
            iter_as_generator_vector( cls )
            
        map_iterators = mb.classes( lambda cls: cls.name.startswith( 'MapIteratorWrapper<' ) )
        for cls in map_iterators:
            iter_as_generator_map( cls ) 
            


